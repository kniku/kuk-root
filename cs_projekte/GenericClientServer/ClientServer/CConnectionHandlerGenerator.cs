using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using log4net;

namespace ClientServer
{
	class CConnectionHandlerGenerator
	{
		static readonly ILog log = LogManager.GetLogger(typeof(CConnectionHandlerGenerator));

		public static IConnectionHandler generate(string [] MessageArray)
		{
			IConnectionHandler r = null;

			if (MessageArray != null && MessageArray.Count() > 0)
			{
				if (MessageArray[0].Equals(CConstants.CMD_HELP))
				{
					r = new ClientServer.ImplConnectionHandler.CHelp();
				}
				else if (MessageArray[0].Equals(CConstants.CMD_FILE_EXIST)
					|| MessageArray[0].Equals(CConstants.CMD_CURRENT_DIRECTORY)
					|| MessageArray[0].Equals(CConstants.CMD_GETFILE))
				{
					r = new ClientServer.ImplConnectionHandler.CFileHandler();
				}
			}
			log.Info(string.Format("handler: {0}", r == null ? "?" : r.ToString()));

			return r;
		}
	}
}
