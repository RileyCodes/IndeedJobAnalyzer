using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using Newtonsoft.Json;
using Msg = IndeedJobMarketAnalyzer.CPPGUISrv.Msg;
using MsgType = IndeedJobMarketAnalyzer.CPPGUISrv.MsgType;

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
                case MsgType.Test:
                    OnTest();
                    break;
            }
        }

        private void OnTest()
        {
            int count = Int32.Parse(JsonReq["count"].ToString());
            string test = "count:" + count++;

            IntPtr pMsg = Marshal.AllocHGlobal(Marshal.SizeOf(msg));
            Marshal.StructureToPtr(msg, pMsg, false);
            responseCb.Invoke(pMsg, test);
            Marshal.FreeHGlobal(pMsg);
        }

        void OnMsgStartTask()
        {
            string test = "test";

            IntPtr pMsg = Marshal.AllocHGlobal(Marshal.SizeOf(msg));
            Marshal.StructureToPtr(msg, pMsg, false);
            responseCb.Invoke(pMsg, test);
            Marshal.FreeHGlobal(pMsg);
            int x = 0;
        }
    }
}
