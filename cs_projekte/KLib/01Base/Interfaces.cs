
namespace KLib.Base
{
	/// <summary>
	/// Einfache Verschlüsselung von Texten OHNE Passwort
	/// Sehr schwache Verschlüsselung!!!!
	/// </summary>
	public interface IPwdCrypter
	{
		string encrypt(string iText);
		string decrypt(string iText);
	}
}
