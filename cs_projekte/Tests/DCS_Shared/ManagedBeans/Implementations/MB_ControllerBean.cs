using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace DCS.ManagedBeans.Implementations
{

	class MB_ControllerBean : AbstractManagedThreadedBean
	{
		HttpListener listener;

		public override void start()
		{
			// Create a listener.
			listener = new HttpListener();
			// Add the prefixes. 
//			listener.Prefixes.Add("http://*:8080/"); // BERECHTIGUNGSPROBLEM!!!!
			listener.Prefixes.Add("http://localhost:13000/");
			listener.Start();
			base.start();
		}

		public override void stop()
		{
			if (listener != null) listener.Stop();
			base.stop();
		}

		protected override int getTimeoutMS()
		{
			return 100;
		}

		public override string getName()
		{
			return "Controller";
		}

		public override string getDescription()
		{
			return "Zur Steuerung des Bean Managers";
		}

		private void sendResponse(HttpListenerResponse response, string iMessage)
		{
			byte[] buffer = System.Text.Encoding.UTF8.GetBytes(iMessage);
			// Get a response stream and write the response to it.
			response.ContentLength64 = buffer.Length;
			System.IO.Stream output = response.OutputStream;
			output.Write(buffer, 0, buffer.Length);
			// You must close the output stream.
			output.Close();
		}

		public override void beanRun()
		{
			// Warte auf Request...
			HttpListenerContext context = listener.GetContext();
			HttpListenerRequest request = context.Request;

			// Obtain a response object.
//			HttpListenerResponse response = context.Response;

			if (request.RawUrl.Contains("/STOPALL"))
			{
				sendResponse(context.Response, "processes down...");
				BeanManager.instance.stop();
				return;
			}

			int v = 0;
			if (request.RawUrl.Contains("/START"))
			{
				v = 6;
			}
			if (request.RawUrl.Contains("/STOP"))
			{
				v = 5;
			}
			if (v > 0) 
			{
				int _i = int.Parse(request.RawUrl.Substring(v));
				int _idx = 0;
				foreach (IManagedBean aBean in BeanManager.instance.Beans.Values)
				{
					if (_idx == _i)
					{
						if (v == 6) aBean.start(); else aBean.stop();
						break;
					}
					_idx++;
				}
				if (request.UrlReferrer != null)
					context.Response.Redirect(request.UrlReferrer.AbsoluteUri);
			}
		
			// Construct a response. 
			string responseString =
@"
<HTML>
<BODY>
<meta http-equiv=""refresh"" content=""5"" >
<h1>Redwave Data Collecting System</h1>
<p>Processes:
<form>
<table border=""1"" style=""border-collapse:collapse"">
<tr>
      <th>Name</th>
      <th>Description</th>
      <th>State</th>
    </tr>
";
			int idx = 0;
			foreach (IManagedBean aBean in BeanManager.instance.Beans.Values)
			{
				responseString += string.Format("<tr><td>{0}</td><td>{1}</td><td><span style=\"color:{5}\">{2}</span></td><td><a href=\"{4}{3}\">{4}</a></td></tr>",
					aBean.getName(),											// Name
					System.Net.WebUtility.HtmlEncode(aBean.getDescription()),	// Description
					aBean.getState(),											// Status
					idx,														// Index
					aBean.getState() == BeanState.running ? "STOP" : "START",	// Action
					aBean.getState() == BeanState.running ? "green" : "red");	// Farbe
				idx++;
			}

			responseString +=
@"
</table>
<p><a href=""STOPALL"">STOP SYSTEM</a>
</form>
</BODY>
</HTML>
";
			sendResponse(context.Response, responseString);
#if false
			// Buffer for reading data
			Byte[] bytes = new Byte[256];
			String data = null;

			try
			{
				// Warte auf Verbindungsaufnahme...
				TcpClient client = server.AcceptTcpClient();

				// Get a stream object for reading and writing
				NetworkStream stream = client.GetStream();
				stream.ReadTimeout = 1;

				int i;
				// Loop to receive all the data sent by the client.
				while ((i = stream.Read(bytes, 0, bytes.Length)) != 0)
				{
					// Translate data bytes to a ASCII string.
					data = System.Text.Encoding.ASCII.GetString(bytes, 0, i);
					logger.LogDebug("Received: [{0}]", data);

					// HTTP Request?
					if (data.StartsWith("GET"))
					{
						byte[] msg;
						string str = "Hallo Welt";
						msg = System.Text.Encoding.ASCII.GetBytes(str);
						stream.Write(msg, 0, msg.Length);

					}

					//				data = data.Trim();
					//				if (data.Length == 0) continue;
				}
				logger.LogDebug("closing connection...", data);
				client.Close();
			}
			catch (Exception e)
			{
				logger.LogError("Exception: {0}", e);
			}
			finally
			{
				// Stop listening for new clients.
				server.Stop();
			}
#endif
		}
	}
}
