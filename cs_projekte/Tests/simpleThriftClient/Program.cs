using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Thrift;
using Thrift.Transport;
using Thrift.Protocol;
using Thrift.Server;
using simplethrift;

namespace simpleThriftClient
{
	class Program
	{
		static void Main(string[] args)
		{
			try
			{
				TTransport transport = new TSocket("localhost", 9090);
				TProtocol protocol = new TBinaryProtocol(transport);
				RemoteLogger.Client client = new RemoteLogger.Client(protocol);

				transport.Open();

				Thrift.Collections.THashSet<string> r = client.getLogs();

				foreach (string s in r)
				{
					Console.WriteLine("received: [{0}]", s);
				}

				transport.Close();
			}
			catch (TApplicationException x)
			{
				Console.WriteLine(x.StackTrace);
			}
		}
	}
}
