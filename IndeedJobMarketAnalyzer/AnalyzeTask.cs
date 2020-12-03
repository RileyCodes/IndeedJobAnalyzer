using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using HtmlAgilityPack;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using PuppeteerSharp;

namespace IndeedJobMarketAnalyzer
{
    class AnalyzeTask
    {
        public enum Status
        {
            Initialized,
            Running,
            Completed,
            Failed,
            Stopping
        }

        public Status status = Status.Initialized;

        private Config config;

        public string taskName => config.TaskFileName;

        private Browser browser = null;
        //private Dictionary<string,List<JobData>> JobdataByKeyword = new Dictionary<string, List<JobData>>();
        
        private List<JobData> JobDatas = new List<JobData>();
        //private List<JobData> JobDatasFailed = new List<JobData>();

        private Dictionary<string,JobData> JobdataByTitleAndCompany = new Dictionary<string, JobData>();
        private int _pageCount = 1;
        private bool _needStop = false;
        private bool _stopped = true;
        private string currentUrl = "";


        public AnalyzeTask(Config config)
        {
            this.config = config;
            this.currentUrl = config.SearchStartUrl;

            //foreach (var keyword in config.TechKeyword)
            //{
            //    JobdataByKeyword[keyword] = new List<JobData>();
            //}
        }
        public enum JobStatus
        {
            Pending,
            Failed,
            Complete
        }
        
        class JobData
        {
            public string JobTitle;
            public string JobCompany;
            public string id;
            public JobStatus JobStatus = JobStatus.Pending;
            public string JobDesc;
            public string JobUrl;


            //JobTitle.Id  doesn't seem to be unique in Indeed.
            //therefore, we will just use title and company instead

            public string UniqueIdentifier => JobTitle + JobCompany;
        }

        public void Stop()
        {
            _needStop = true;

            status = _stopped ? Status.Completed : Status.Stopping;

            //while(!_stopped)
            //    Thread.Sleep(5);
        }
        
        public struct TaskInfo
        {
            public string TaskName;
            public int JobCount;
            public String Status;
            public int PagesAt;
            public string Url;
        }
        
        public TaskInfo GetTaskInfo()
        {
            TaskInfo taskInfo = new TaskInfo()
            {
                TaskName = config.TaskFileName, 
                JobCount = JobdataByTitleAndCompany.Count,
                Status = status.ToString(),
                PagesAt = _pageCount,
                Url = currentUrl
            };

            return taskInfo;
        }

        async Task<List<JobData>> GetJobDatas(Page page)
        {
            List<JobData> jobDatas = new List<JobData>();
            var res = await JSExecuter.JSExec(page, "GetJobDatas");

            foreach (var jToken in res.Children())
            {
                var html = jToken.ToString();

                var doc = new HtmlDocument();
                doc.LoadHtml(html);
                
                var jobData = new JobData();
                
                var JobTitle =
                    doc.DocumentNode.SelectNodes("//a[contains(@class, 'jobtitle')]").FirstOrDefault();

                if (JobTitle == null)
                {
                    LogMgr.Log("Failed to find JobTitle:" + html);
                    continue;
                }
                
                jobData.id = JobTitle.Id;

                var titleAttribute = JobTitle.Attributes.FirstOrDefault(attr => attr.Name == "title");
                if (titleAttribute == null)
                {
                    LogMgr.Log("JobTitle Failed - No Title Attributes");
                    continue;
                }
                jobData.JobTitle = titleAttribute.Value;

                
                var JobCompany =
                    doc.DocumentNode.SelectNodes("//span[contains(@class, 'company')]").FirstOrDefault();

                if (JobCompany == null)
                {
                    LogMgr.Log("Failed to find CompanyName:" + html);
                    continue;
                }
                
                jobData.JobCompany = JobCompany.InnerText;

                if (JobdataByTitleAndCompany.ContainsKey(jobData.UniqueIdentifier))
                {
                    LogMgr.Log("Skip: " + jobData.id + " name:" + JobdataByTitleAndCompany[jobData.UniqueIdentifier].JobTitle);
                    continue;
                }

                jobDatas.Add(jobData);
            }
            return jobDatas;
        }

        void LoadFromJobDatas()
        {
            foreach (var jobData in JobDatas)
            {
                JobdataByTitleAndCompany[jobData.UniqueIdentifier] = jobData;
            }
        }

        public async Task<bool> TryRun()
        {
            status = await Run() ? Status.Completed : Status.Failed;
            _stopped = true;
            return status == Status.Completed;
        }
        
        private async Task<bool> Run()
        {
            if (_needStop)
            {
                status = Status.Completed;
                return true;
            }
            _stopped = false;

            if (!config.reset)
            {
                if (!TryLoadTaskData())
                {
                    return false;
                }
            }


            
            //await new BrowserFetcher().DownloadAsync(BrowserFetcher.DefaultRevision);

            currentUrl = config.SearchStartUrl;
            status = Status.Running;

            for (;;)
            {
                try
                {
                    browser = await Puppeteer.LaunchAsync(new LaunchOptions
                    {
                        Headless = false,
                        DefaultViewport = new ViewPortOptions() {Height = 500, Width = 500},
                        Args = new[] { "--window-size=500,500" }
                    });

                    var page = await browser.NewPageAsync();
                    await page.GoToAsync(currentUrl);

                    for (;;)
                    {
                        CheckNeedStop();

                        var jobDatas = await GetJobDatas(page);
                        await ProcessAllJobDatasInCurrentPage(jobDatas, page);
                        var nextPageResult = (await JSExecuter.JSExec(page, "TryGotoNextPage")).ToString();

                        if (!await TryGotoNextPage(nextPageResult, page))
                        {
                            //False == at end of all pages
                            return OnTaskEnded();
                        }

                        await page.WaitForSelectorAsync("html");
                        currentUrl = page.Url;
                    }
                }
                catch (UserStoppedException userStoppedException)
                {
                    LogMgr.Log("AnalyzeTask stopped by user.");
                    return OnTaskEnded();
                }
                catch (Exception e)
                {
                    LogMgr.Log("an error occurred: " + e.Message + " at:" + e.StackTrace);
                    await CloseBrowserIgnoreAllErr();
                }
            }

            LogMgr.Log("error: unexpected execution path");
            return false;
        }

        private void CheckNeedStop()
        {
            if (_needStop)
            {
                throw new UserStoppedException();
            }
        }

        private async Task CloseBrowserIgnoreAllErr()
        {
            try
            {
                await browser.CloseAsync();
            }
            catch (Exception exception)
            {
                //ignore all exception, if it fails it fails.
            }
        }

        private bool OnTaskEnded()
        {
            Save();
            _needStop = false;
            CloseBrowserIgnoreAllErr(); //no need to wait
            return true;
        }

        private bool TryLoadTaskData()
        {
            try
            {
                if (!string.IsNullOrEmpty(config.TaskFileName))
                {
                    var taskFilePath = Config.TaskDir + config.TaskFileName;

                    if (File.Exists(taskFilePath))
                    {
                        var jobDataInJSON = System.IO.File.ReadAllText(taskFilePath);

                        SavedTask savedTask = JsonConvert.DeserializeObject<SavedTask>(jobDataInJSON);
                        JobDatas = savedTask.JobDatas;
                        LoadFromJobDatas();
                    }
                }
                else
                {
                    config.TaskFileName = Util.ShortUUID();
                }
            }
            catch (Exception e)
            {
                LogMgr.Log("Error: failed while load or create task data: " + e.Message + " at:" + e.StackTrace);
                return false;
            }

            return true;
        }

        struct SavedTask
        {
            public string TaskName;
            public int JobCount;
            public int PagesAt;
            public string Url;
            public List<JobData> JobDatas;
        }

        private void Save()
        {
            SavedTask savedTask = new SavedTask();
            savedTask.TaskName = config.TaskFileName;
            savedTask.PagesAt = _pageCount;
            savedTask.JobCount = JobDatas.Count;
            savedTask.Url = currentUrl;
            savedTask.JobDatas = JobDatas;

            var jsonData = JsonConvert.SerializeObject(savedTask);
            var filePath = Config.TaskDir + config.TaskFileName;
            
            System.IO.FileInfo file = new System.IO.FileInfo(filePath);
            file.Directory.Create();
            System.IO.File.WriteAllText(file.FullName, jsonData);
            LogMgr.Log("Task saved to: " + filePath);
        }
        
        private async Task<bool> TryGotoNextPage(string nextPageResult, Page page)
        {
            //false = all page completed
            //true = at next page
            for (;;)
            {
                switch (nextPageResult)
                {
                    case "completed":
                        return false;
                    case "ok":
                        Thread.Sleep(1000);
                        _pageCount++;

//#if DEBUG
//                        if (_pageCount >= 2)//only do two pages in debug mode.
//                        {
//                            return false;
//                        }
//#endif

                        return true;
                    default:
                        LogMgr.Log("Error while going to next page, Refresh...");
                        await page.ReloadAsync();
                        break;
                }
            }
        }
        
        private async Task ProcessAllJobDatasInCurrentPage(List<JobData> jobDatas, Page page)
        {
            for(var i=0; i  < jobDatas.Count; i++)
            {
                CheckNeedStop();

                var jobData = jobDatas[i];
                LogMgr.Log("Processing: " + jobData.JobTitle);

                await JSExecuter.JSExec(page, "ClickOnJob",
                    (JSCode => JSCode.Replace("[elementId]", jobData.id)));

                Page ViewJobPage = null;

                ViewJobPage = await WaitForNewPageByUrl("viewjob");

                jobData.JobUrl = ViewJobPage?.Url ?? "";

                if (ViewJobPage == null)
                {
                    LogMgr.Log("Failed to open job id:" + jobData.id);
                    jobData.JobStatus = JobStatus.Failed;
                    continue;
                }


                var res = await JSExecuter.JSExec(ViewJobPage, "GetJobDescription");

                var innerHTML = res.ToString();

                if (innerHTML.Any())
                {
                    ProcessJobDesc(innerHTML, jobData);
                }
                else
                {
                    LogMgr.Log("GetJobDescription Failed at: " + jobData.JobTitle + " id:" + jobData.id);
                    i--;//do not move to next job
                }

                await EnsureAllPageClosedByUrl("viewjob");
                Thread.Sleep(3000); //slow down to prevent being flagged as a bot
            }
        }

        private void ProcessJobDesc(string innerText,JobData jobData)
        {
            jobData.JobDesc = innerText;
            jobData.JobStatus = JobStatus.Complete;
            JobDatas.Add(jobData);

            JobdataByTitleAndCompany[jobData.id] = jobData;
        }

        private async Task<Page>  WaitForNewPageByUrl(string UrlKeyWord, int RetryMax = 10)
        {
            for (var i = 0; i < RetryMax; i++)
            {
                //https://github.com/hardkoded/puppeteer-sharp/issues/1587
                var ver = await browser.GetVersionAsync();
                var pages = await browser.PagesAsync();

                var matchedPages = pages.Where(page => page.Url.Contains(UrlKeyWord)).ToList();
                if (matchedPages.Any())
                {
                    return matchedPages[0];
                }
                Thread.Sleep(1000);
            }
            return null;
        }


        private async Task EnsureAllPageClosedByUrl(string UrlKeyWord, int RetryMax = 10)
        {
            for (;;)
            {
                for (var i = 0; i < RetryMax; i++)
                {
                    var ver = await browser.GetVersionAsync();
                    var pages = await browser.PagesAsync();
                    var matchedPages = pages.Where(page => page.Url.Contains(UrlKeyWord)).ToList();

                    if (matchedPages.Count == 0)
                        return;

                    foreach (var matchedPage in matchedPages)
                    {
                        await matchedPage.CloseAsync();
                    }

                    Thread.Sleep(1000);
                }
                LogMgr.Log("Extra page not closed after 10s.");
            }
            return;
        }

        
        //private static JobData GetJobData(HtmlNodeCollection JobTitleAndCompanyName)
        //{
        //    var jobData = new JobData();

        //    foreach (var htmlNode in JobTitleAndCompanyName)
        //    {
        //        var className = htmlNode.Attributes.FirstOrDefault(attr => attr.Name == "class");

        //        if (className == null)
        //        {
        //            LogMgr.Log("JobTitleAndCompanyName Failed - No Class Attributes");
        //            return null;
        //        }

        //        switch (className.Value)
        //        {
        //            case "jobtitle turnstileLink ":

        //                jobData.id = htmlNode.Id;
        //                var titleAttribute = htmlNode.Attributes.FirstOrDefault(attr => attr.Name == "title");

        //                if (titleAttribute == null)
        //                {
        //                    LogMgr.Log("JobTitleAndCompanyName Failed - No Title Attributes");
        //                    return null;
        //                }


        //                jobData.JobTitle = titleAttribute.Value;
        //                break;
        //            case "turnstileLink":
        //                jobData.JobCompany = htmlNode.InnerHtml;
        //                break;
        //            default:
        //                LogMgr.Log("JobTitleAndCompanyName Failed - Unexpected class name");
        //                return null;
        //        }
        //    }

        //    return jobData;
        //}
    }

    internal class UserStoppedException : Exception
    {
    }
}
