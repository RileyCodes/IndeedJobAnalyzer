using System;
using System.Collections.Generic;
using System.Text;

namespace IndeedJobMarketAnalyzer
{
    class LogMgr
    {
        public static void Log(string Message)
        {
            IGUIWrapper.AddLog(Message);
        }
    }
}
