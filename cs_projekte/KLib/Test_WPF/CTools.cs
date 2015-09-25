using KLib.Wpf;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Test_WPF
{
	public class CTools
	{
		public static void ensureLoggerIsOpen(Window iOwner)
		{
			if (!WndLogger.IsAlreadyOpen())
			{
				KLib.Wpf.WndLogger dlg = WndLogger.CreateOrGetWndLogger(iOwner);
				if (iOwner != null)
				{
					dlg.Top = iOwner.Top;
					dlg.Left = iOwner.Left + iOwner.Width;
				}
				dlg.Show();
			}
		}
	}
}
