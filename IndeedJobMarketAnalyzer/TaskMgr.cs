using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Newtonsoft.Json;

namespace IndeedJobMarketAnalyzer
{
    public static class TaskMgr
    {
        private static bool IsRunTaskTRLive = false;

        static async void RunTaskTR()
        {
            //This thread is used to run all the tasks in queue serially
            //it is not a good practice by using a dedicated thread, thread pool
            //should be used, but I do not have time at this point

            if (TasksToRun.Count == 0)
                return;

            IEnumerable<AnalyzeTask> tasks = null;

            do
            {
                tasks = TasksToRun.Values.Where(task =>
                    task.status == AnalyzeTask.Status.Initialized);

                //we make a temporary list here to prevent exception when TasksToRun changed
                var tmpList = tasks?.ToList();
                foreach (var task in tmpList)
                {
                    await task.TryRun();
                }

            } while (tasks.Any());


            IsRunTaskTRLive = false;
        }

        static void UpdateTaskInfoTR()
        {
            //fetch current task status and send to GUI 
            //it is not a good practice by using a dedicated thread, thread pool and timer
            //should be used, but I do not have time at this point
            for (;;)
            {
                Thread.Sleep(100);
                List<AnalyzeTask.TaskInfo> taskInfos = new List<AnalyzeTask.TaskInfo>();

                foreach (var analyzeTask in TasksToRun.Values.ToList())
                {
                    taskInfos.Add(analyzeTask.GetTaskInfo());
                }

                var taskInfoJson = JsonConvert.SerializeObject(taskInfos);
                IGUIWrapper.UpdateTaskInfo(taskInfoJson);
            }
        }

        static TaskMgr()
        {

        }

        public static void Run()
        {
            Thread updateTaskInfoTR = new Thread(UpdateTaskInfoTR);
            updateTaskInfoTR.Start();
        }


        static Dictionary<string, AnalyzeTask> TasksToRun =  new Dictionary<string, AnalyzeTask>();

        static void EnsureSingletonTaskThreadStarted()
        {
            if (IsRunTaskTRLive)
                return;

            IsRunTaskTRLive = true;

            Thread runTaskTR = new Thread(RunTaskTR);
            runTaskTR.Start();
        }

        public static void StartTask(string taskName, string url, bool reset = false)
        {
            Config config = new Config();
            config.SearchStartUrl = url;
            config.TaskFileName = taskName;
            config.reset = reset;
            AnalyzeTask analyzeTask= new AnalyzeTask(config);

            TasksToRun[taskName] = analyzeTask;

            EnsureSingletonTaskThreadStarted();
        }

        public static void StopTask(string taskName)
        {
            if (!TasksToRun.ContainsKey(taskName))
            {
                return;
            }

            TasksToRun[taskName]?.Stop();
        }
    }
}
