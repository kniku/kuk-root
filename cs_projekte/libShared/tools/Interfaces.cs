using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace knk.shared.tools
{
	/// <summary>
	/// Einfache Verschlüsselung von Texten OHNE Passwort
	/// </summary>
	public interface IPwdCrypter
	{
		string encrypt(string iText);
		string decrypt(string iText);
	}
}
