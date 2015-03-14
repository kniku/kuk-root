using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;

namespace KLib.Base
{
	/// <summary>
	/// Versiegeln von Dateien
	/// Ansatz: Verschlüsselte Prüfsumme der Datei rechnen und in separaten Datei ablegen
	/// </summary>
	public static class CFileSealing
	{
		static IPwdCrypter crypter = PwdCrypterGenerator.generate(PwdCrypterImpl.simple);

		static string _createSealPath(string Path)
		{
			return Path + ".seal";
		}

		/// <summary>
		/// Prüft, ob geg. Datei gültig versiegelt ist
		/// </summary>
		/// <param name="Path"></param>
		/// <returns></returns>
		public static bool FileIsSealed(string Path)
		{
			string seal;
			var Path_Seal = _createSealPath(Path);

			if (!File.Exists(Path_Seal))
			{
				return false;
			}

			// Siegeldatei einlesen
			using (var stream = new StreamReader(Path_Seal))
			{
				seal = crypter.decrypt(stream.ReadLine());
			}

			var seal_calc = calcFileSeal(Path);

			return seal.Equals(seal_calc);
		}
		
		/// <summary>
		/// Ermittelt für geg. Datei das Siegel
		/// </summary>
		/// <param name="Path"></param>
		/// <returns></returns>
		static string calcFileSeal(string Path)
		{
			using (var md5 = MD5.Create())
			{
				using (var stream = File.OpenRead(Path))
				{
					return BitConverter.ToString(md5.ComputeHash(stream).Reverse().ToArray()).Replace("-", "").ToLower();
				}
			}
		}

		/// <summary>
		/// Versiegelt geg. Datei. Wenn Ok, wird das Siegel zurückgeliefert.
		/// </summary>
		/// <param name="Path"></param>
		/// <returns></returns>
		public static string sealFile(string Path)
		{
			var r = crypter.encrypt(calcFileSeal(Path));
			var Path_Seal = _createSealPath(Path);

			using (var stream = new StreamWriter(Path_Seal))
			{
				stream.WriteLine(r);
			}
			return r;
		}

		/// <summary>
		/// Entfernt (löscht) Siegel einer Datei
		/// </summary>
		/// <param name="Path"></param>
		public static void removeSealOfFile(string Path)
		{
			var Path_Seal = _createSealPath(Path);
			if (File.Exists(Path_Seal))
			{
				File.Delete(Path_Seal);
			}
		}
	}
}
