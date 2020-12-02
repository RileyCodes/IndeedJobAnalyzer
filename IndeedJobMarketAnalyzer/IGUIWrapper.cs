using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using Newtonsoft.Json;

namespace IndeedJobMarketAnalyzer
{
    partial class IGUIWrapper
    {
        [DllImport("IndeedGUI.dll")]
        public static extern int StartQT();

        [DllImport("IndeedGUI.dll")]
        public static extern IntPtr GetMsg();

        [DllImport("IndeedGUI.dll")]
        public static extern void AddLog(string msg);

        [DllImport("IndeedGUI.dll")]
        public static extern void UpdateTaskInfo(string msg);

        private static bool isExited = false;
        private static int ExitCode = 0;
        
        static void Poll()
        {
            //Polls request made from IndeedGUI
            for (;;)
            {
                Thread.Sleep(1);
                if (isExited)
                    return;

                IntPtr pMsg = GetMsg();

                if (pMsg == IntPtr.Zero)
                {
                    continue;
                }

                Msg msg = (Msg)Marshal.PtrToStructure(pMsg, typeof(Msg));

                //at this point, we assume all reqBuffer is a string, therefore, we do not need to know length.
                var reqJsonStr = Marshal.PtrToStringUTF8(msg.reqBuffer);
                if(string.IsNullOrEmpty(reqJsonStr))
                    continue;

                var responseCb = Marshal.GetDelegateForFunctionPointer<ResponseCB>(msg.cb);

                new Router(reqJsonStr, responseCb, msg);
                
            }
        }

        static void _Main()
        {
            var exitCode = StartQT();
            AfterMain(exitCode);
        }

        static void AfterMain(int exitCode)
        {
            ExitCode = exitCode;
            isExited = true;
        }

        public static int Start()
        {
            Thread cppMainThread = new Thread(_Main);
            cppMainThread.Start();

            LogMgr.Log("Software Started.");
            Poll();

            return ExitCode;
        }
    }
}
