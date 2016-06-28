using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace einfacherHttpServer
{
	class Program
	{
		static void Main(string[] args)
		{
			SimpleHTTPServer s = new SimpleHTTPServer(@"c:\knk\tmp\htdocs", 12000);
			System.Console.WriteLine("server started on port " + s.Port);
		}
	}
}
