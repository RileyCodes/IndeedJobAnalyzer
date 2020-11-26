using System;
using System.Collections.Generic;
using System.Text;

namespace IndeedJobMarketAnalyzer
{
    class Util
    {
        public static string ShortUUID()
        {
            var ticks = new DateTime(2016, 1, 1).Ticks;
            var ans = DateTime.Now.Ticks - ticks;
            var uniqueId = ans.ToString("x");
            return uniqueId;
        }
    }
}
