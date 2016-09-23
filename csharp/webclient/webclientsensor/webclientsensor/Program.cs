using System;
using System.Diagnostics;
using System.IO;
using System.Net.Http;
using System.Threading.Tasks;
using Newtonsoft.Json;
using webclientsensor;
namespace webclientsensor
{
	class Program
	{
		//private static string PmtSensor = string.Empty;
		static void Main(string[] args)
		{
			try {
				Execute ("python", "/home/pi/bootstrap.py");			
			} catch (Exception ex) {
				Console.WriteLine (ex.Message);
			}
		}
		public static async Task GetSensorInfoAsync(string pythonValue)
		{
			if (!string.IsNullOrEmpty (pythonValue)) {
				StreamReader file = File.OpenText("config.json");
				string s = file.ReadToEnd();
				var config = JsonConvert.DeserializeObject<Config>(s);
				var serial = config.serial;
				var version = config.version;
				var lic = config.license;
				var author = config.author;
				var value = pythonValue.ToUpper ();
				var PmtSensor = config.All_url;
				PmtSensor = PmtSensor + "name="+ config.name + "&serial=" + serial + "&version=" + version + "&lic=" + lic + "&author=" + author + "&value=" + value + "&cache=no";
				Console.WriteLine ("ALLCORE:SEND--->");
				Console.WriteLine (PmtSensor);
				using (var client = new HttpClient())
				{
					HttpResponseMessage response = await client.GetAsync(PmtSensor);
					if (response.IsSuccessStatusCode)
					{
						await response.Content.ReadAsStringAsync();
						string responseBodyAsText = await response.Content.ReadAsStringAsync();
						Console.WriteLine("<---:RESPONSE:"+responseBodyAsText);
					}
				}

			}
		}
		public static int Execute(string exe, string args) 
		{ 
			string StandardOutput = string.Empty;
			//string StandardError = string.Empty;
			ProcessStartInfo oInfo = new ProcessStartInfo(exe, args); 
			oInfo.UseShellExecute = false; 
			oInfo.CreateNoWindow = true; 
			oInfo.RedirectStandardOutput = true; 
			oInfo.RedirectStandardError = true; 
			StreamReader srOutput = null; 
			StreamReader srError = null; 

			Process proc = System.Diagnostics.Process.Start(oInfo); 
			proc.WaitForExit(); 
			srOutput = proc.StandardOutput; 
			StandardOutput = srOutput.ReadToEnd(); 
			srError = proc.StandardError; 
			//StandardError = srError.ReadToEnd(); 
			int exitCode = proc.ExitCode; 
			proc.Close(); 
            //Console.WriteLine(StandardOutput);
			GetSensorInfoAsync (StandardOutput).Wait();
			return exitCode; 
		} 


	}


}
