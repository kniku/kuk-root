using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ClientServer.ImplConnectionHandler
{
	class CHelp : IConnectionHandler
	{
		#region IConnectionHandler Member

		public bool handle(System.Net.Sockets.NetworkStream clientStream, string[] InitialMessageArray)
		{
			CTools.sendString(clientStream, string.Format("{0}|no help available", CTools.createReturnMessagePrefix(CConstants.CMD_HELP, CConstants.ERRORCODE.OK)));
			return true;
		}

		#endregion
	}
}
