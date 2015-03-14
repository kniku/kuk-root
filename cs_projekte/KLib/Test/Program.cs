using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using KLib.Base;

namespace Test
{
	class Program
	{
		static void Main(string[] args)
		{
			log4net.Config.BasicConfigurator.Configure();

			IPwdCrypter crypter = PwdCrypterGenerator.generate();

			DateTime x = new DateTime();
			x = DateTime.Now;
			System.Console.WriteLine(KDateTimeExt.convert2DateTimeStr(x));
			System.Console.WriteLine(KDateTimeExt.convert2DateStr(x));
			System.Console.WriteLine(KDateTimeExt.convert2TimeStr(x));
			System.Console.WriteLine(KDateTimeExt.parseDateTimeStr("20120902121314", out x));
			System.Console.WriteLine(KDateTimeExt.convert2DateTimeStr(x));
			System.Console.WriteLine(KDateTimeExt.convert2DateStr(x));
			System.Console.WriteLine(KDateTimeExt.convert2TimeStr(x));
			System.Console.WriteLine(KDateTimeExt.parseDateStr("20111019", out x));
			System.Console.WriteLine(KDateTimeExt.convert2DateTimeStr(x));
			System.Console.WriteLine(KDateTimeExt.convert2DateStr(x));
			System.Console.WriteLine(KDateTimeExt.convert2TimeStr(x));
			System.Console.WriteLine(KDateTimeExt.parseTimeStr("132037", out x));
			System.Console.WriteLine(KDateTimeExt.convert2DateTimeStr(x));
			System.Console.WriteLine(KDateTimeExt.convert2DateStr(x));
			System.Console.WriteLine(KDateTimeExt.convert2TimeStr(x));
		}
	}
}
