using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using KLib.Base;
using System.IO;

namespace UnitTestBase
{
	[TestClass]
	public class UnitTestBase
	{
		[TestMethod]
		public void BasicTests_KDateTimeExt()
		{
			DateTime dt;

			Assert.IsFalse(KDateTimeExt.parseDateTimeStr("2013090212131x", out dt));
			Assert.IsTrue(KDateTimeExt.parseDateTimeStr("20120902121314", out dt));
			Assert.AreEqual("20120902121314", KDateTimeExt.convert2DateTimeStr(dt));
			Assert.AreEqual("20120902", KDateTimeExt.convert2DateStr(dt));
			Assert.AreEqual("121314", KDateTimeExt.convert2TimeStr(dt));
		}

		[TestMethod]
		public void BasicTests_IPwdCrypter()
		{
			const string teststr = "A1_ßü€x//\\{";
			string str;
			IPwdCrypter crypter = PwdCrypterGenerator.generate();
			Assert.IsTrue(crypter.GetType().ToString().Equals("KLib.Base.internals.CPwdCrypter_simple"));
			str = crypter.encrypt(teststr);
			Assert.AreEqual(teststr.Length * 2 + 2, str.Length);
			str = crypter.decrypt(str);
			Assert.AreEqual(teststr, str);
		}

		[TestMethod]
		public void BasicTests_CFileSealing()
		{
			var test_file = "TEST.DAT";
			CFileSealing.removeSealOfFile(test_file);
			File.Delete(test_file);

			var test_data = "1234567890" + Environment.NewLine + "abcdefgh" + Environment.NewLine + "äöß?<>@€";
			try
			{
				CFileSealing.sealFile(test_file);
				Assert.Fail();
			}
			catch (Exception e)
			{
				Assert.IsInstanceOfType(e, typeof(FileNotFoundException));
			}

			using (var stream = new StreamWriter(test_file))
			{
				stream.WriteLine(test_data);
			}

			IPwdCrypter crypter = PwdCrypterGenerator.generate(PwdCrypterImpl.simple);

			Assert.IsFalse(CFileSealing.FileIsSealed(test_file));

			var seal = CFileSealing.sealFile(test_file);
			Assert.IsTrue(File.Exists(test_file));
			Assert.IsTrue(File.Exists(test_file + ".seal"));
			Assert.AreEqual("2cf7127ab6d89fbb925999d456de2770", crypter.decrypt(seal));
			Assert.IsTrue(CFileSealing.FileIsSealed(test_file));

			using (var stream = new StreamWriter(test_file))
			{
				stream.WriteLine(test_data + "1");
			}
			Assert.IsFalse(CFileSealing.FileIsSealed(test_file));
			
			CFileSealing.sealFile(test_file);
			Assert.IsTrue(CFileSealing.FileIsSealed(test_file));

			CFileSealing.removeSealOfFile(test_file);
			Assert.IsFalse(CFileSealing.FileIsSealed(test_file));
			File.Delete(test_file);
		}
	}
}
