
namespace KLib.Base
{
	#region PwdCrypter

	/// <summary>
	/// Mögliche PwdCrypter Typen
	/// </summary>
	public enum PwdCrypterImpl
	{
		simple = 1
	}

	/// <summary>
	/// Erzeugt einen PwdCrypter vom gewünschtem Typ
	/// </summary>
	public static class PwdCrypterGenerator
	{
		static log4net.ILog Logger = log4net.LogManager.GetLogger(typeof(PwdCrypterGenerator));

		public static IPwdCrypter generate()
		{
			return generate(PwdCrypterImpl.simple);
		}

		public static IPwdCrypter generate(PwdCrypterImpl iCrypterType)
		{
			Logger.Debug("generating crypter: " + iCrypterType);
			return new internals.CPwdCrypter_simple();
		}
	}

	#endregion PwdCrypter
}
