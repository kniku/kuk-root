
namespace knk.shared.logger.impl
{
	public class None : AbstractLogger
	{
		public None()
			: base("None")
		{
		}

		public override void Log(LogLevel iLevel, string iMsg)
		{
			// no logging
		}
	}
}
