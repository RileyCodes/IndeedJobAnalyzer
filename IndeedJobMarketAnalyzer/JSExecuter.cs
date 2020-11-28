using Newtonsoft.Json.Linq;
using PuppeteerSharp;
using System;
using System.Threading.Tasks;

namespace IndeedJobMarketAnalyzer
{
    class JSExecuter
    {
        public static async Task<JToken> JSExec(Page page, string JSName, Func<string, string> replacer = null)
        {
#if DEBUG
            const string JavaScriptPathPrefix = "../../../";
#else
            const string JavaScriptPathPrefix = "";
#endif

            try
            {
                var script = await System.IO.File.ReadAllTextAsync(JavaScriptPathPrefix + "./JavaScript/" + JSName + ".js");

                if (replacer != null)
                    script = replacer.Invoke(script);
                
                return await page.EvaluateExpressionAsync(script);
            }
            catch (Exception e)
            {
                LogMgr.Log("JSExec Failed:" + e.Message);
            }

            return "";
        }
    }
}
