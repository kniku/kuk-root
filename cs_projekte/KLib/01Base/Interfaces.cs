
namespace KLib.Base
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
