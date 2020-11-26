using System;
using System.Collections.Generic;
using System.Text;

namespace IndeedJobMarketAnalyzer
{
    class Config
    {
        public  string SearchStartUrl = "https://www.indeed.com/jobs?q=C%23&l=Daly%20City%2C%20CA&radius=100";
        public  List<string> TechKeyword = new List<string>();
        //public string TaskFileName = null;

        public string TaskFileName = "57bfd63af349c.txt";

        public const string TaskDir = @"./Tasks/";


        public Config()
        {
            TechKeyword.Add("NoKeyword");

            
            //DummyData
            TechKeyword.AddRange(new[]
            {
                "Rust", "C++", "C#", "ASP", "PHP", "Java", ".NET", "React", "Vue", "Angular", "QT", "Python", "Redis",
                "MSSQL", "MYSQL",
                "JavaScript", "TypeScript", "NodeJS", "Assembly", "Win32", "Windows", "Unity", "Unreal",
                "GUI", "HTML", "CSS", "X86", "X64", "IDA", "Olly", "Legacy", "Algorithm", "Data Structure",
                "Memcache","Multithread","Go","Swift","IOS","Android","RPA","Bot","Mono","Bachelor","BSCS","MCSD"
            });
        }

    }
}
