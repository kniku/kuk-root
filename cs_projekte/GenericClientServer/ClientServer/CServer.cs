using System;
using System.Collections;
using System.Linq;
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.Text;
using log4net;

namespace ClientServer
{
	public class CClientInfo
	{
		public string ID;
		public DateTime connectionTime;
		public DateTime disconnectionTime;
		public bool isConnected;
	}

	public class CServer
	{
		static readonly ILog log = LogManager.GetLogger(typeof(CServer));

		int mPort;
		TcpListener tcpListener;
		Thread listenThread;
		public bool isStarted { get; private set; }
		System.Collections.Hashtable hashClients = new System.Collections.Hashtable();

		string getClientID(TcpClient Client)
		{
			return Client.Client.ProtocolType + "/" + Client.Client.RemoteEndPoint.ToString();
		}

		void HandleClientComm(object client)
		{
			TcpClient tcpClient = (TcpClient)client;
			string ID = getClientID(tcpClient);

			NetworkStream clientStream = tcpClient.GetStream();

			byte[] message = new byte[4096];
			int bytesRead;

			while (true)
			{
				bytesRead = 0;

				try
				{
					//blocks until a client sends a message
					bytesRead = clientStream.Read(message, 0, 4096);
				}
				catch (Exception e)
				{
					//a socket error has occured
					//System.Diagnostics.Debug.WriteLine("exception");
					log.Error(ID, e);
					break;
				}

				if (bytesRead == 0)
				{
					//the client has disconnected from the server
					log.Info(ID + " - client disconnected.");
					break;
				}

				//message has successfully been received
				ASCIIEncoding encoder = new ASCIIEncoding();
				string ClientMessage = encoder.GetString(message, 0, bytesRead);
				//System.Diagnostics.Debug.WriteLine(encoder.GetString(message, 0, bytesRead));
				log.Info("recv from " + ID + ": [" + ClientMessage + "]");

				string[] MessageArray = CTools.getMessageArray(ClientMessage);
				IConnectionHandler handler = CConnectionHandlerGenerator.generate(MessageArray);
				if (handler != null)
				{
					if (!handler.handle(clientStream, MessageArray))
					{
						break;
					}
				}
				else
				{
					log.Error("invalid command!");
					CTools.sendError(clientStream, CConstants.ERRORCODE.Unspecified, "invalid command");
				}

			}

			CClientInfo ci = hashClients[getClientID(tcpClient)] as CClientInfo;
			if (ci != null)
			{
				ci.isConnected = false;
				ci.disconnectionTime = DateTime.Now;
			}
			clientStream.Close();
			tcpClient.Close();
		}

		void ListenForClients()
		{
			tcpListener.Start();

			while (true)
			{
				TcpClient client;
				try
				{
					//blocks until a client has connected to the server
					client = tcpListener.AcceptTcpClient();
				}
				catch
				{
					// just catch it...
					break;
				}

				CClientInfo ci = new CClientInfo();
				ci.ID = getClientID(client);
				log.Info(ci.ID + " - client connected.");
				if (!hashClients.Contains(ci.ID))
				{
					ci.isConnected = true;
					ci.connectionTime = DateTime.Now;
					hashClients.Add(ci.ID, ci);
					//create a thread to handle communication 
					//with connected client
					Thread clientThread = new Thread(new ParameterizedThreadStart(HandleClientComm));
					clientThread.Start(client);
				}
				else
				{
					client.Close();
				}
			}
		}

		public CServer(int ListenerPort)
		{
			mPort = ListenerPort;
			tcpListener = new TcpListener(IPAddress.Any, mPort);
		}

		public void start()
		{
			if (!isStarted)
			{
				isStarted = true;
				listenThread = new Thread(new ThreadStart(ListenForClients));
				listenThread.Start();
			}
		}

		public void stop()
		{
			if (isStarted)
			{
				tcpListener.Stop();
				listenThread.Abort();
				isStarted = false;
			}
		}

		public IEnumerator enumClients()
		{
			return hashClients.Values.GetEnumerator();
		}
	}
}
