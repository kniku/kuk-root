using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using knk.shared;
using knk.shared.tools;
using knk.shared.tools.impl;

namespace toolPwdCrypter
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
			comboEncryptionKind.SelectedIndex = 0;
		}

		private void butEncrypt_Click(object sender, EventArgs e)
		{
			Button but = (Button)sender;
			IPwdCrypter crypter = null;

			switch (comboEncryptionKind.Text)
			{
				case "simple":
					crypter = new PwdCrypter_simple();
				break;
			}

			if (crypter != null)
			{
				if (but == butEncrypt)
				{
					textOutput.Text = crypter.encrypt(textInput.Text);
				}
				else
				{
					textOutput.Text = crypter.decrypt(textInput.Text);
				}
			}
		}

		private void butClear_Click(object sender, EventArgs e)
		{
			textOutput.Text = textInput.Text = "";
		}

		private void butExit_Click(object sender, EventArgs e)
		{
			Close();
		}
	}
}
