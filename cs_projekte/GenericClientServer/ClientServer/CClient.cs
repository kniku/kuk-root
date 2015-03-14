using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.IO;
using System.Text;
using log4net;

namespace ClientServer
{
	public class CClient
	{
		static readonly ILog log = LogManager.GetLogger(typeof(CClient));
		TcpClient client;
		NetworkStream stream;

		public bool isConnected { get; private set; }

		public CClient()
		{
			client = new TcpClient();
			isConnected = false;
		}

		public void connect(string IP4Adress, int Port)
		{
			try
			{
				client = new TcpClient();
				IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(IP4Adress), Port);
				client.Connect(serverEndPoint);
				stream = client.GetStream();
				stream.ReadTimeout = 1000;
				isConnected = true;
			}
			catch
			{
				// just catch it...
			}
		}

		public void sendString(string msg)
		{
			if (isConnected)
			{
				CTools.sendString(stream, msg);
			}
		}

		public string receiveString()
		{
			return (CTools.receiveString(stream));
		}

		public void disconnect()
		{
			isConnected = false;
			stream = null;
			//client.Client.Disconnect(true);
			client.Client.Close();
			client.Close();
		}

		public string handleHelp()
		{
			sendString(CConstants.CMD_HELP);
			return receiveString();
		}

		static int getErrorCodeFromResult(string[] ResultArray)
		{
			return CTools.getIntFromMessageArray(ResultArray, 1);
		}

		public int handleFileExist(string ServerPath)
		{
			int r = -1;
			sendString(string.Format("{0}|{1}", CConstants.CMD_FILE_EXIST, ServerPath));
			string[] result = CTools.getMessageArray(receiveString());
			if (getErrorCodeFromResult(result) == 0 && result[0].Equals(CConstants.CMD_FILE_EXIST))
			{
				r = CTools.getIntFromMessageArray(result, 2);
			}
			return r;
		}

		public int handleGetFile(string ServerPath, string LocalPath)
		{
			const int BUFSIZE = 65536;

			int r = -1;
			string msg = string.Format("{0}|{1}", CConstants.CMD_GETFILE, ServerPath);
			log.Debug("sending command: [" + msg + "]");
			sendString(msg);
			msg = receiveString();
			log.Debug("recv: [" + msg + "]");

			string[] result = CTools.getMessageArray(msg);

			int errcode = getErrorCodeFromResult(result);
			log.Info("result: [" + errcode + "]");

			if (errcode == 0 && result[0].Equals(CConstants.CMD_GETFILE))
			{
				int filesize = CTools.getIntFromMessageArray(result, 2);
				log.Info("filesize: [" + filesize + "]");
				if (filesize > 0)
				{
					log.Debug("open local file: [" + LocalPath + "]");
					FileStream fs = new FileStream(LocalPath, FileMode.Create);
					byte[] byteArr = new byte[BUFSIZE];
					int cnt;
					while (filesize > 0 && (cnt = stream.Read(byteArr, 0, (filesize > BUFSIZE ? BUFSIZE : filesize))) > 0)
					{
						filesize -= cnt;
						log.Debug("bytes received: " + cnt + ", remaining: " + filesize);
						fs.Write(byteArr, 0, cnt);
					}
					log.Debug("closing local file");
					fs.Close();
				}
			}
			return r;
		}

	}
}
