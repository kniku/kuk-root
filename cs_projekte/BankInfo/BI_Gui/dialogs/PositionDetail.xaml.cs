using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace BI_Gui.dialogs
{
	/// <summary>
	/// Interaktionslogik für PositionDetail.xaml
	/// </summary>
	public partial class PositionDetail : Window
	{

		public PositionDetail()
		{
			InitializeComponent();
		}

		public void setNewPosition(CPosition iPos)
		{
			if (iPos != null)
			{
				CPosition mPos = new CPosition();
				mPos.amt = iPos.amt;
				mPos.effdt = iPos.effdt;
				mPos.memo = iPos.memo;
				mPos.posteddt = iPos.posteddt;
				mPos.voucher = iPos.voucher != null ? iPos.voucher.Replace(" ", Environment.NewLine) : "";
				mPos.amt = iPos.amt;
				DataContext = mPos;
			}
		}
	}
}
