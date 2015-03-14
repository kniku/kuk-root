using System;
using System.Linq;
using System.Text;
using System.Security.Principal;
using System.Diagnostics;

namespace sudo
{
	class Program
	{
		static void Main(string[] args)
		{
			if (args.Count() < 1)
			{
				Console.Out.WriteLine("\nsudo v0.1, (c) BTA (by KnK) - 2012\nEin Programm mit Adminrechten ausführen.\nAufruf: sudo [program [options]]\n");
				Environment.Exit(0);
			}
			StringBuilder sb = new StringBuilder();
			String fileName = args[0];
			for (int i = 1; i < args.Length; i++)
			{
				sb.Append(args[i] + " ");
			}
			String arguments = sb.ToString();
			Console.WriteLine(fileName + " " + arguments);

			WindowsPrincipal principal = new WindowsPrincipal(WindowsIdentity.GetCurrent());
			bool hasAdministrativeRight = principal.IsInRole(WindowsBuiltInRole.Administrator);

			if (!hasAdministrativeRight)
			{
				RunElevated(fileName, arguments);
			}
			else
			{
				Console.WriteLine("Already elevated.");
				Process.Start(fileName, arguments);
			}
			Environment.Exit(0);

		}

		public static void RunElevated(string fileName, string arguments)
		{
			//MessageBox.Show("Run: " + fileName);
			ProcessStartInfo processInfo = new ProcessStartInfo();
			processInfo.UseShellExecute = true;
			processInfo.Verb = "runas";
			processInfo.FileName = fileName;
			processInfo.Arguments = arguments;
			try
			{
				Console.WriteLine("Running elevated.");
				Process.Start(processInfo);
			}
			catch (System.ComponentModel.Win32Exception e)
			{
				Console.WriteLine("  Win32Exception caught!");
				Console.WriteLine("  Win32 error = {0}",
					e.Message);
			}
		}

	}
}
