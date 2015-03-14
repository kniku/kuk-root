using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using log4net;
using System.IO;

namespace ClientServer.ImplConnectionHandler
{
	class CFileHandler : IConnectionHandler
	{
		static readonly ILog log = LogManager.GetLogger(typeof(CFileHandler));

		public bool handle(System.Net.Sockets.NetworkStream clientStream, string[] InitialMessageArray)
		{
			string tmp;
			bool bool_tmp;

			if (InitialMessageArray != null && InitialMessageArray.Count() > 0)
			{
				if (InitialMessageArray[0].Equals(CConstants.CMD_FILE_EXIST))
				{
					if (InitialMessageArray.Count() < 2)
					{
						CTools.sendError(clientStream, CConstants.ERRORCODE.Unspecified, "invalid command");
						return true;
					}

					bool exists = false, is_writeable = false, is_directory = false;
					long filesize = 0;
					DateTime filetimestamp = DateTime.MinValue;
					bool_tmp = true;
					try
					{
						log.Info(string.Format("retrieving infos for [{0}]", InitialMessageArray[1]));
						FileInfo fi = new FileInfo(InitialMessageArray[1]);
						if (fi.Attributes >= 0)
						{
							exists = true;
							if ((fi.Attributes & FileAttributes.Directory) == 0)
							{
								is_directory = false;
								is_writeable = (fi.Attributes & FileAttributes.ReadOnly) == 0;
								filesize = fi.Length;
								filetimestamp = fi.LastWriteTime;
							}
							else
							{
								is_directory = true;
								is_writeable = (fi.Attributes & FileAttributes.ReadOnly) == 0;
								filetimestamp = fi.LastWriteTime;
							}
						}
					}
					catch (Exception ex)
					{
						bool_tmp = false;
						log.Error("could not access " + InitialMessageArray[1], ex);
					}
					CTools.sendString(clientStream, string.Format("{0}|{1}|{2}|{3}|{4}|{5}",
						CTools.createReturnMessagePrefix(CConstants.CMD_FILE_EXIST, bool_tmp ? CConstants.ERRORCODE.OK : CConstants.ERRORCODE.Unspecified),
						CTools.convertBoolToStrBool(exists),
						is_directory ? CConstants.CONST_FILETYPE_DIRECTORY : CConstants.CONST_FILETYPE_FILE,
						CTools.convertBoolToStrBool(is_writeable),
						filesize,
						CTools.convertDateTimeToStr(filetimestamp)));
				}
				else if (InitialMessageArray[0].Equals(CConstants.CMD_CURRENT_DIRECTORY))
				{
					tmp = InitialMessageArray.Count() > 1 ? InitialMessageArray[1] : null;
					bool_tmp = true;
					if (tmp != null)
					{
						try
						{
							log.Info("changing directory to " + tmp);
							Directory.SetCurrentDirectory(tmp);
						}
						catch (Exception ex)
						{
							bool_tmp = false;
							log.Error("could not change directory to " + tmp, ex);
						}
					}
					CTools.sendString(clientStream, string.Format("{0}|{1}",
						CTools.createReturnMessagePrefix(CConstants.CMD_CURRENT_DIRECTORY, bool_tmp ? CConstants.ERRORCODE.OK : CConstants.ERRORCODE.Unspecified),
						Directory.GetCurrentDirectory()));
				}
				else if (InitialMessageArray[0].Equals(CConstants.CMD_GETFILE))
				{
					if (InitialMessageArray.Count() < 2)
					{
						CTools.sendError(clientStream, CConstants.ERRORCODE.Unspecified, "invalid command");
						return true;
					}
					try
					{
						FileInfo fi = new FileInfo(InitialMessageArray[1]);
						if (fi.Attributes < 0)
						{
							CTools.sendError(clientStream, CConstants.ERRORCODE.NotExisting, "file not found");
							return true;
						}

						CTools.sendString(clientStream, string.Format("{0}|{1}",
							CTools.createReturnMessagePrefix(CConstants.CMD_GETFILE, CConstants.ERRORCODE.OK),
							fi.Length));

						const int BUFSIZE = 65536;

						log.Debug("open file: [" + fi.FullName + "]");
						FileStream fs = new FileStream(fi.FullName, FileMode.Open);
						byte[] byteArr = new byte[BUFSIZE];
						// Datenstrom in ein Byte-Array einlesen

						int cnt;
						log.Debug("sending file...");
						while ((cnt = fs.Read(byteArr, 0, BUFSIZE)) > 0)
						{
							clientStream.Write(byteArr, 0, cnt);
						}
						log.Debug("closing file");
						fs.Close();
					}
					catch (Exception ex)
					{
						log.Error("could not access " + InitialMessageArray[1], ex);
						CTools.sendError(clientStream, CConstants.ERRORCODE.NotExisting, "file not found");
					}
				}
			}

			return true;
		}
	}
}
