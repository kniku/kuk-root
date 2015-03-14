using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace simpleWinForm
{
	public partial class Form1 : Form
	{
		private NotifyIcon trayIcon;
		private ContextMenu trayMenu;

		private void OnExit(object sender, EventArgs e)
		{
			Application.Exit();
		}

		private void OnShow(object sender, EventArgs e)
		{
			Visible = true;
		}

		private void OnHide(object sender, EventArgs e)
		{
			Visible = false;
		}

		protected override void OnLoad(EventArgs e)
		{
			//Visible       = false; // Hide form window.
			//ShowInTaskbar = false; // Remove from taskbar.
			base.OnLoad(e);
		}

		public Form1()
		{
			InitializeComponent();
			// Create a simple tray menu with only one item.
			trayMenu = new ContextMenu();
			trayMenu.MenuItems.Add("Hide", OnHide);
			trayMenu.MenuItems.Add("Show", OnShow);
			trayMenu.MenuItems.Add("Exit", OnExit);
//			trayMenu.MenuItems[1].DefaultItem = true;
//			trayIcon.MouseDoubleClick += new MouseEventHandler(trayIcon_MouseDoubleClick);
	
			// Create a tray icon. In this example we use a
			// standard system icon for simplicity, but you
			// can of course use your own custom icon too.
			trayIcon      = new NotifyIcon();
			trayIcon.Text = "MyTrayApp";
			trayIcon.Icon = new Icon(SystemIcons.Application, 40, 40);
	 
			// Add menu to tray icon and show it.
			trayIcon.ContextMenu = trayMenu;
			trayIcon.Visible     = true;
		}

	}

}
