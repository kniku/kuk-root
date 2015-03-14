using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ClientServer;
using log4net.Core;

namespace testServer
{
	public partial class FormMain : Form
	{
		CServer server;
		int mPort = 23000;

		public FormMain()
		{
			InitializeComponent();

			server = new CServer(mPort);
			butStartStop_Click(null, null);
			statusUpdate();
			timer.Start();
		}

		void statusUpdate()
		{
			textInfo.Text = string.Format("port: {1}{0}state: {2}", Environment.NewLine, mPort, server.isStarted ? "listening" : "stopped");
		}

		private void butStartStop_Click(object sender, EventArgs e)
		{
			if (server.isStarted)
			{
				server.stop();
				butStartStop.Text = "start tcp server";
			}
			else
			{
				server.start();
				butStartStop.Text = "stop tcp server";
			}
			statusUpdate();
		}

		private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
		{
			server.stop();
		}

		private void timer_Tick(object sender, EventArgs e)
		{
			listClients.Items.Clear();

			IEnumerator Enumerator = server.enumClients();
			if (Enumerator == null) return;

			Enumerator.Reset();
			while(Enumerator.MoveNext())
			{
				CClientInfo ci = (Enumerator.Current as CClientInfo);
				listClients.Items.Add(string.Format("{0}, {1} ({2})",
					ci.ID,
					ci.isConnected ? "CONNECTED" : "disconnected",
					ci.isConnected ? ci.connectionTime : ci.disconnectionTime));
			}

			foreach (LoggingEvent logevent in Program.LogMemAppender.GetEvents())
			{
				listLog.SelectedIndex = listLog.Items.Add(string.Format("{2} {0}: {1}", logevent.Level, logevent.RenderedMessage, logevent.TimeStamp));
			}
			Program.LogMemAppender.Clear();
		}
	}
}
