using System;
using System.Threading;
using System.Threading.Tasks;
using PuppeteerSharp;

namespace IndeedJobMarketAnalyzer 
{
    class Program
    {
        static void Main(string[] args)
        {
            TaskMgr.Run();
            IGUIWrapper.Start();
        }
    }
}
