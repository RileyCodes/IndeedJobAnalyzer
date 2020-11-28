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

            var tasks = TasksToRun.Where(task => 
                task.status == AnalyzeTask.Status.INIT);

            foreach (var task in tasks)
            {
                task.status = await task.Run() ? AnalyzeTask.Status.COMPLETED : AnalyzeTask.Status.FAILED;
            }

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

                foreach (var analyzeTask in TasksToRun)
                {
                    taskInfos.Add(analyzeTask.GetTaskInfo());
                }

                var taskInfoJson = JsonConvert.SerializeObject(taskInfos);
                CPPGUISrv.UpdateTaskInfo(taskInfoJson);
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


        static List<AnalyzeTask> TasksToRun = new List<AnalyzeTask>();

        static void EnsureSingletonTaskThreadStarted()
        {
            if (IsRunTaskTRLive)
                return;

            IsRunTaskTRLive = true;

            Thread runTaskTR = new Thread(RunTaskTR);
            runTaskTR.Start();
        }

        public static void NewTask(string taskName, string url)
        {
            Config config = new Config();
            config.SearchStartUrl = url;
            config.TaskFileName = taskName;
            AnalyzeTask analyzeTask= new AnalyzeTask(config);

            TasksToRun.Add(analyzeTask);

            EnsureSingletonTaskThreadStarted();
        }
    }
}
