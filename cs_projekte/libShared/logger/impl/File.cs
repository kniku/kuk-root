using System;

namespace knk.shared.logger.impl
{
	public class RollingFile : AbstractLogger
	{
		private string mFileName;
		private System.IO.StreamWriter mWriter;

		/// <summary>
		/// Max. Logfilegröße in Bytes
		/// </summary>
		public int MAX_FILESIZE { get; set; }
		/// <summary>
		/// Max. Anzahl Logfiles (min=2!)
		/// </summary>
		public int MAX_FILES { get; set; }

		public RollingFile(string iFilename)
			: base("File")
		{
			mFileName = iFilename;

			MAX_FILES = 10;				// max. 10 Files zu je
			MAX_FILESIZE = 1024 * 1024;	// 1 MB

			mWriter = new System.IO.StreamWriter(mFileName, true);
			mWriter.WriteLine("**********************************************");
			mWriter.WriteLine("*** Startup ({0})", DateTime.Now);
			mWriter.WriteLine("**********************************************");
			mWriter.Flush();
		}

		private void rename(string oldFile, int iIdx)
		{
			string newFile = mFileName + "." + iIdx;

			if (System.IO.File.Exists(newFile))
			{
				if (iIdx < MAX_FILES - 1)
				{
					rename(newFile, iIdx + 1);
				}
				else
				{
					System.IO.File.Delete(newFile);
				}
			}
			System.IO.File.Move(oldFile, newFile);
		}

		private void rollFiles()
		{
			if (mWriter.BaseStream.Position > MAX_FILESIZE)
			{
				mWriter.Close();
				rename(mFileName, 1);
				mWriter = new System.IO.StreamWriter(mFileName, true);
			}
		}

		public override void Log(LogLevel iLevel, string iMsg)
		{
			if (mWriter != null)
			{
				rollFiles();
				mWriter.WriteLine("{0}: {1}", DateTime.Now, iMsg);
				mWriter.Flush();
			}
		}
	}
}
