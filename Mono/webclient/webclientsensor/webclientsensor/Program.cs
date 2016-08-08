using System;
using System.IO;
using System.Net.Http;
using System.Threading.Tasks;
using Newtonsoft.Json;
using webclientsensor;
namespace webclientsensor
{
	class Program
	{
		private static string PmtSensor = string.Empty;
		static void Main(string[] args)
		{
			if (args.Length != 1)return;
			try {
				StreamReader file = File.OpenText("config.json");
				string s = file.ReadToEnd();
				var config = JsonConvert.DeserializeObject<Config>(s);
				var serial = config.serial;
				var version = config.version;
				var lic = config.license;
				var author = config.author;
				var value = args [0].ToUpper ();

				if(value.Contains("ENS210"))
				{
					Console.WriteLine ("ENS210:SEND--->");
					PmtSensor = config.ENS210_url;
					PmtSensor = PmtSensor + "name="+ config.name + "&serial=" + serial + "&version=" + version + "&lic=" + lic + "&author=" + author + "&value=" + value + "&cache=no";
				}

				if(value.Contains("IAQCORE"))
				{
					Console.WriteLine ("IAQCORE:SEND--->");
					PmtSensor = config.IAQCORE_url;
					PmtSensor = PmtSensor + "name="+ config.name + "&serial=" + serial + "&version=" + version + "&lic=" + lic + "&author=" + author + "&value=" + value + "&cache=no";
				}

				//Console.WriteLine (PmtSensor);
				GetSensorInfoAsync ().Wait();
			} catch (Exception ex) {
				Console.WriteLine (ex.Message);
			}

		}

		public static async Task GetSensorInfoAsync()
		{
			if (!string.IsNullOrEmpty (PmtSensor)) {
				using (var client = new HttpClient())
				{
					HttpResponseMessage response = await client.GetAsync(PmtSensor);
					if (response.IsSuccessStatusCode)
					{
						string responseBodyAsText = await response.Content.ReadAsStringAsync();
						Console.WriteLine("<---:RESPONSE:"+responseBodyAsText);
					}
				}

			}
		}
	}


}
