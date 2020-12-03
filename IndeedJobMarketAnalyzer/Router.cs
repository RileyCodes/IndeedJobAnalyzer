using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using Newtonsoft.Json;
using Msg = IndeedJobMarketAnalyzer.IGUIWrapper.Msg;
using MsgType = IndeedJobMarketAnalyzer.IGUIWrapper.MsgType;

namespace IndeedJobMarketAnalyzer
{
    delegate void ResponseCB(IntPtr Msg, string Response);
    class Router
    {
        private dynamic JsonReq;
        private ResponseCB responseCb;
        private Msg msg;


        public Router(string JsonStr, ResponseCB responseCb, Msg msg)
        {
            JsonReq = JsonConvert.DeserializeObject(JsonStr);
            this.responseCb = responseCb;
            this.msg = msg;

            switch ((MsgType)JsonReq["type"])
            {
                case MsgType.MsgStartTask:
                    OnMsgStartTask();
                    break;
                case MsgType.MsgTest:
                    OnTest();
                    break;
                case MsgType.MsgRestartTask:
                    OnMsgRestartTask();
                    break;
                case MsgType.MsgStopTask:
                    OnMsgStopTask();
                    break;
            }
        }

        void OnTest()
        {
            DoResponse("");
        }

        void OnMsgStartTask()
        {
            string taskName = JsonReq["taskName"];
            string url = JsonReq["url"];

            TaskMgr.StartTask(taskName,url);
            DoResponse("");
        }


        private void OnMsgStopTask()
        {
            string taskName = JsonReq["taskName"];
            TaskMgr.StopTask(taskName);
            DoResponse("");
        }

        private void OnMsgRestartTask()
        {
            string taskName = JsonReq["taskName"];
            string url = JsonReq["url"];

            TaskMgr.StartTask(taskName, url,true);
            DoResponse("");
        }


        void DoResponse(string res)
        {
            IntPtr pMsg = Marshal.AllocHGlobal(Marshal.SizeOf(msg));
            Marshal.StructureToPtr(msg, pMsg, false);
            responseCb.Invoke(pMsg, res);
            Marshal.FreeHGlobal(pMsg);
        }
    }
}
