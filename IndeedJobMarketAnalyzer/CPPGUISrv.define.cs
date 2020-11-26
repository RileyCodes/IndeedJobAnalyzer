using System;
using System.Collections.Generic;
using System.Text;


namespace IndeedJobMarketAnalyzer
{
    //WARNING: Must match with IndeedGUI
    partial class CPPGUISrv
    {
        public enum MsgType
        {
            MsgStartTask,
            Test
        };

        public struct Msg
        {
            //Reuqest buffer, which is a string at this point
            public IntPtr reqBuffer;

            //Request length, not currently used
            public int reqLen;

            //Request Id, used to identify request after callback
            public int reqId;

            //pointer to requestMgr used by callback function, this way its much better then make a global 
            //RequestMgr , and easier then construct a thiscall from IJMA
            public IntPtr requestMgr;

            //function pointer for response callback
            public IntPtr cb;
        }

        
    }
}
