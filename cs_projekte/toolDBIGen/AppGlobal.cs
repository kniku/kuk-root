using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using knk.shared.logger;

namespace dbigen
{
	class CAppGlobal
	{
		public ILogger log;
		public CLookup lookup;
		public CXMLParser parser;

		public CAppGlobal()
		{
			log = Generator.createLogger(LogProvider.Console);
			lookup = new CLookup();
			parser = new CXMLParser();
		}
	}
}
