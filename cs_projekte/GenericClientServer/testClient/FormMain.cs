using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ClientServer;
using log4net.Core;

namespace testClient
{
	public partial class FormMain : Form
	{
		CClient client;
//		string mServerAddr = "127.0.0.1";
//		int mPort = 23000;

		public FormMain()
		{
			InitializeComponent();

			client = new CClient();
			comboParams.SelectedIndex = 0;
			comboCommand.SelectedIndex = 0;
			timer1.Start();
		}

		private void butConnect_Click(object sender, EventArgs e)
		{
			if (client.isConnected)
			{
				client.disconnect();
				butConnect.Text = "connect";
			}
			else
			{
				int port = 0;

				string[] a = comboServer.Text.Split(':');
				if (a.Count() > 1) int.TryParse(a[1], out port);

				inform("try to connect...");
				client.connect(a[0], port);
				if (client.isConnected)
				{
					butConnect.Text = "disconnect";
					inform("success.");
				}
				else
				{
					inform("FAILED!");
				}
			}
		}

		void inform(string msg)
		{
			listInfo.SelectedIndex = listInfo.Items.Add(msg);
		}

		private void butSend_Click(object sender, EventArgs e)
		{
			if (client.isConnected && !string.IsNullOrEmpty(comboCommand.Text))
			{
				if (comboCommand.Text.Equals(CConstants.CMD_GETFILE))
				{
					client.handleGetFile(comboParams.Text, "c:\\big1\\tmp\\test-copy.txt");
				}
				else
				{
					string msg = comboCommand.Text;
					if (!string.IsNullOrEmpty(comboParams.Text))
					{
						msg += "|" + comboParams.Text;
					}

					inform("sending: [" + msg + "]");
					client.sendString(msg);
					inform(string.Format("answer=[{0}]", client.receiveString()));
				}
			}
			else
			{
				inform("NOT CONNECTED!");
			}
		}

		private void butHelp_Click(object sender, EventArgs e)
		{
			if (client.isConnected)
			{
				inform(string.Format("answer=[{0}]", client.handleHelp()));
			}
			else
			{
				inform("NOT CONNECTED!");
			}
		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			foreach (LoggingEvent logevent in Program.LogMemAppender.GetEvents())
			{
				listInfo.SelectedIndex = listInfo.Items.Add(string.Format("{2} {0}: {1}", logevent.Level, logevent.RenderedMessage, logevent.TimeStamp));
			}
			Program.LogMemAppender.Clear();

		}
	}
}
