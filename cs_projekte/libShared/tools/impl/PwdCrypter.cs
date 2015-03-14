using System;
using System.Linq;

namespace knk.shared.tools.impl
{
	public class PwdCrypter_simple : IPwdCrypter
	{
		Random random = new Random();

		char RANDOM_CHAR()
		{
			char r;
			do
			{
//				r = Convert.ToChar(Convert.ToInt32(Math.Floor(26 * random.NextDouble() + 65)));
				r = Convert.ToChar(Convert.ToInt32(Math.Floor(96 * random.NextDouble() + 32)));
			} while ("\"&'<>".Contains(r));	// avoiding xml parse errors...

			return r;
		}

		public string encrypt(string iText)
		{
			string r = "";
			if (iText != null)
			{
				r += RANDOM_CHAR();
				r += RANDOM_CHAR();

				for (int i = iText.Length - 1; i >= 0; i--)
				{
					r += iText.Substring(i, 1);
					r += RANDOM_CHAR();
				}
			}
			return r;
		}

		public string decrypt(string iText)
		{
			string r = "";
			if (iText != null && iText.Length > 2)
			{
				string tmp = iText.Substring(2);

				while (tmp.Length > 1)
				{
					r += tmp[0];
					tmp = tmp.Remove(0, 2);
				}

				tmp = r;
				r = "";

				for (int i = tmp.Length - 1; i >= 0; i--)
				{
					r += tmp.Substring(i, 1);
				}
			}
			return r;
		}
	}
}
