using System;

using System.Threading;
using System.IO;
using System.Net;
using System.Net.Sockets;

namespace DCS.ManagedBeans.Implementations
{
	class MB_DCSController : AbstractManagedBean
	{
		Thread myThread = null;

		public override string getName()
		{
			return "DCS Controller";
		}

		public override string getDescription()
		{
			return "Steuerung des DCS";
		}

		public override void init()
		{
			base.init();
//			AppGlobal.instance.logger.pushPrefix(getName()).LogDebug(": init...\n").popPrefix();
		}

		void run()
		{
			TcpListener server = null;
			try
			{
				// Set the TcpListener on port 13000.
				Int32 port = 13000;
				IPAddress localAddr = IPAddress.Parse("127.0.0.1");

				// TcpListener server = new TcpListener(port);
				server = new TcpListener(localAddr, port);

				// Start listening for client requests.
				server.Start();

				// Buffer for reading data
				Byte[] bytes = new Byte[256];
				String data = null;

				// Enter the listening loop.
				while (true)
				{
//					AppGlobal.instance.logger.LogDebug("Waiting for a connection...\n");

					// Perform a blocking call to accept requests.
					// You could also user server.AcceptSocket() here.
					TcpClient client = server.AcceptTcpClient();
//					AppGlobal.instance.logger.LogDebug("Connected: {0}\n", client.Client.RemoteEndPoint.ToString());

					data = null;

					// Get a stream object for reading and writing
					NetworkStream stream = client.GetStream();

					int i;
					byte[] msg;

					// Loop to receive all the data sent by the client.
					while ((i = stream.Read(bytes, 0, bytes.Length)) != 0)
					{
						// Translate data bytes to a ASCII string.
						data = System.Text.Encoding.ASCII.GetString(bytes, 0, i);
						data = data.Trim();
						if (data.Length == 0) continue;
//						AppGlobal.instance.logger.LogDebug("Received: {0}\n", data);

						if (data.StartsWith("stop"))
						{
							if (data.Length > 4)
							{
								int _i = int.Parse(data.Substring(5));
								foreach (IManagedBean cb in BeanManager.instance.Beans.Values)
								{
									if (_i == 0)
									{
										cb.stop();
										msg = System.Text.Encoding.ASCII.GetBytes(string.Format("*** bean \"{0}\" has been stopped...\r\n", cb.getName()));
										stream.Write(msg, 0, msg.Length);
										break;
									}
									_i--;
								}
							}
							else
							{
								client.Close();
//								BeanManager.instance.isRunning = false;
//								BeanManager.instance.stop();
								return;
							}
						}
						else if (data.StartsWith("start"))
						{
							if (data.Length > 5)
							{
								int _i = int.Parse(data.Substring(6));
								foreach (IManagedBean cb in BeanManager.instance.Beans.Values)
								{
									if (_i == 0)
									{
										if (cb.getState() != BeanState.running)
										{
											cb.start();
											msg = System.Text.Encoding.ASCII.GetBytes(string.Format("*** bean \"{0}\" has been started...\r\n", cb.getName()));
										}
										else
										{
											msg = System.Text.Encoding.ASCII.GetBytes(string.Format("*** bean \"{0}\" already running...\r\n", cb.getName()));
										}
										stream.Write(msg, 0, msg.Length);
										break;
									}
									_i--;
								}
							}
						}
						else if (data.Equals("status"))
						{
							int _i = 0;
							data = "\r\n*** installed beans:\r\n";
							foreach (IManagedBean cb in BeanManager.instance.Beans.Values)
							{
								data = data + "(" + _i + ") " + cb.getName() + ": " + cb.getDescription() + " (" + cb.getState() + ")\r\n";
								_i++;
							}
							msg = System.Text.Encoding.ASCII.GetBytes(data);
							stream.Write(msg, 0, msg.Length);
						}
						else if (data.Equals("help"))
						{
							msg = System.Text.Encoding.ASCII.GetBytes(
								"*** valid commands are:\r\n" +
								" - status ..... list beans\r\n" +
								" - stop [n] ... stop bean(s)\r\n" +
								" - start n .... start bean\r\n" +
								" - help ....... this help\r\n"
							);
							stream.Write(msg, 0, msg.Length);
						}
						else
						{
							msg = System.Text.Encoding.ASCII.GetBytes("*** unknown command!\r\n*** try help to get assistance...\r\n");
							stream.Write(msg, 0, msg.Length);
						}
						msg = System.Text.Encoding.ASCII.GetBytes("\r\n(DCS)>> ");
						stream.Write(msg, 0, msg.Length);
					}

					// Shutdown and end connection
//					AppGlobal.instance.logger.LogDebug("Disconnected!\n");
					client.Close();
				}
			}
			catch (SocketException e)
			{
//				AppGlobal.instance.logger.LogError("SocketException: {0}", e);
			}
			finally
			{
				// Stop listening for new clients.
				server.Stop();
			}
		}

		public override void start()
		{
			base.start();
//			AppGlobal.instance.logger.pushPrefix(getName()).LogDebug(": start...\n").popPrefix();
			myThread = new Thread(new ThreadStart(run));
			myThread.Start();
		}

		public override void stop()
		{
			base.stop();
//			AppGlobal.instance.logger.pushPrefix(getName()).LogDebug(": stop...\n").popPrefix();
			if (myThread != null)
			{
				myThread.Abort();
			}
			myThread = null;
		}

		public override void done()
		{
			base.done();
//			AppGlobal.instance.logger.pushPrefix(getName()).LogDebug(": done...\n").popPrefix();
		}

		public override void beanRun()
		{
			// nothing to do...
		}
	}
}
