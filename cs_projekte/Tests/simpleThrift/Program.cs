using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Thrift;
using Thrift.Transport;
using Thrift.Protocol;
using Thrift.Server;

namespace simpleThrift
{
	public class MyHandler : simplethrift.RemoteLogger.Iface
	{
		public Thrift.Collections.THashSet<string> getLogs()
		{
			Thrift.Collections.THashSet<string> r = new Thrift.Collections.THashSet<string>();
			r.Add("kuk");
			r.Add("knk");
			return r;
		}
	}

	class Program
	{
		static void Main(string[] args)
		{
			try
			{
				MyHandler handler = new MyHandler();
				simplethrift.RemoteLogger.Processor processor = new simplethrift.RemoteLogger.Processor(handler);
				TServerTransport serverTransport = new TServerSocket(9090);
				TServer server = new TThreadPoolServer(processor, serverTransport);
				Console.WriteLine("Starting the server...");
				server.Serve();
			}
			catch (Exception x)
			{
				Console.WriteLine(x.StackTrace);
			}
			Console.WriteLine("done.");
		}
	}
}
