using System;
using System.Collections.Generic;
using System.Text;

namespace IndeedJobMarketAnalyzer
{
    class Config
    {
        public  string SearchStartUrl = "";
        public string TaskFileName = null;

        public const string TaskDir = @"./Tasks/";


        public Config()
        {
            //DummyData
            //TechKeyword.AddRange(new[]
            //{
            //    "Rust", "C++", "C#", "ASP", "PHP", "Java", ".NET", "React", "Vue", "Angular", "QT", "Python", "Redis",
            //    "MSSQL", "MYSQL",
            //    "JavaScript", "TypeScript", "NodeJS", "Assembly", "Win32", "Windows", "Unity", "Unreal",
            //    "GUI", "HTML", "CSS", "X86", "X64", "IDA", "Olly", "Legacy", "Algorithm", "Data Structure",
            //    "Memcache","Multithread","Go","Swift","IOS","Android","RPA","Bot","Mono","Bachelor","BSCS","MCSD"
            //});
        }

    }
}
