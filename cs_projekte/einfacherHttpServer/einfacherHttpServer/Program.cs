using KLib.Sql;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

[assembly: KLib.Sql.XmlConfigurator(ConfigFile = "Database.config")]

namespace einfacherHttpServer
{
	class Program
	{
		static void Main(string[] args)
		{
			SimpleHTTPServer s = new SimpleHTTPServer(@"..\..\..\htdocs", 12000);
			System.Console.WriteLine("server started on port " + s.Port);
		}
	}
}
