package clserv.clienthandler;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.StringReader;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;

import javax.xml.stream.XMLEventFactory;
import javax.xml.stream.XMLEventWriter;
import javax.xml.stream.XMLOutputFactory;

import org.apache.log4j.Logger;
import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.InputSource;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.XMLReaderFactory;

import clserv.shared.I_ClientHandler;
import clserv.shared.I_ConnectionHandler;
import clserv.tools.AppContext;


public class NewClientHandler_sql implements I_ClientHandler /*extends AClientHandler*/ {
	private final Logger logger = Logger.getLogger(this.getClass());

	class Parameter implements ContentHandler {

		private boolean inStmt = false;
		private String theStmt = "";
		private boolean humanReadable = false;
		
		void parseCommand(String iCommand) throws SAXException, IOException {
			XMLReader xmlreader = XMLReaderFactory.createXMLReader();
			xmlreader.setContentHandler(this);
			xmlreader.parse(new InputSource(new StringReader(iCommand)));
		}

		@Override
		public void characters(char[] ch, int start, int length)
				throws SAXException {
			if (inStmt) {
				theStmt += new String(ch, start, length);
			}
		}

		@Override
		public void endDocument() throws SAXException {
		}

		@Override
		public void endElement(String uri, String localName, String qName) throws SAXException {
			if (localName.equals("stmt")) {
				inStmt  = false;
			}
		}

		@Override
		public void endPrefixMapping(String prefix) throws SAXException {
		}

		@Override
		public void ignorableWhitespace(char[] ch, int start, int length) throws SAXException {
		}

		@Override
		public void processingInstruction(String target, String data) throws SAXException {
		}

		@Override
		public void setDocumentLocator(Locator locator) {
		}

		@Override
		public void skippedEntity(String name) throws SAXException {
		}

		@Override
		public void startDocument() throws SAXException {
		}

		@Override
		public void startElement(String uri, String localName, String qName, Attributes atts) throws SAXException {
			if (localName.equals("stmt")) {
				inStmt  = true;
				String hr=atts.getValue("humanReadable");
				if (hr!=null && hr.equals("1")) {
					humanReadable =true; 
				}
			}
		}

		@Override
		public void startPrefixMapping(String prefix, String uri) throws SAXException {
		}
		
	}

	@Override
	public void handle(I_ConnectionHandler iConn, String command) throws Exception {
		AppContext ac = AppContext.getInstance();
		Parameter param = new Parameter();
		try {
			if (command.endsWith("\n")) {
				command=command.substring(0,command.length()-1);
			}			param.parseCommand(command);
			if (param.theStmt != null && !param.theStmt.isEmpty()) {
				Connection conn = ac.getDatabase().getConnection();
				logger.debug("Statement to execute=[" + param.theStmt + "]");
				PreparedStatement StmtPrep = conn.prepareStatement(param.theStmt);
				StmtPrep.execute();
				ResultSet rs = StmtPrep.getResultSet();
				
				ByteArrayOutputStream xxx = new ByteArrayOutputStream(1000);
				XMLEventWriter eventWriter = XMLOutputFactory.newInstance().createXMLEventWriter(xxx);
				XMLEventFactory eventFactory = XMLEventFactory.newInstance();

				eventWriter.add(eventFactory.createStartElement("", "", "ecm"));
				eventWriter.add(eventFactory.createAttribute("t", "sql"));
				if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\n"));

				ResultSetMetaData rsmd = rs.getMetaData();
				int colCount = rsmd.getColumnCount();
				int idx;
				if (colCount > 0) {
					if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
					eventWriter.add(eventFactory.createStartElement("", "", "header"));
					if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\n"));
					for (idx = 1; idx <= colCount ; idx++) {
						if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
						if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
						eventWriter.add(eventFactory.createStartElement("", "", "col"));
						String x = rsmd.getColumnClassName(idx);
						int p = x.lastIndexOf('.');
						if (p >= 0) x = x.substring(p + 1);
						eventWriter.add(eventFactory.createAttribute("t", x));
						x = rsmd.getColumnName(idx);
						if (x == null || x.isEmpty()) x = "COLUMN " + idx;
						eventWriter.add(eventFactory.createCharacters(x));
						eventWriter.add(eventFactory.createEndElement("", "", "col"));
						if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\n"));
					}
					if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
					eventWriter.add(eventFactory.createEndElement("", "", "header"));
				}
				eventWriter.flush();
				iConn.sendMessageToClientImmediately(xxx.toString("utf-8"));
				xxx.reset();
				
				if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\n"));
				if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
				eventWriter.add(eventFactory.createStartElement("", "", "rows"));
				for (; rs.next() ;) {
					if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\n"));
					if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
					if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
					eventWriter.add(eventFactory.createStartElement("", "", "r"));
					for (idx = 1; idx <= colCount ; idx++) {
						if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\n"));
						if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
						if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
						if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
						eventWriter.add(eventFactory.createStartElement("", "", "v"));
						Object x = rs.getObject(idx);
						eventWriter.add(eventFactory.createCharacters(x == null ? "null" : x.toString()));
						eventWriter.add(eventFactory.createEndElement("", "", "v"));
					}
					if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\n"));
					if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
					if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
					eventWriter.add(eventFactory.createEndElement("", "", "r"));
					eventWriter.flush();
					iConn.sendMessageToClientImmediately(xxx.toString("utf-8"));
					xxx.reset();
				}
				if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\n"));
				if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\t"));
				eventWriter.add(eventFactory.createEndElement("", "", "rows"));
				if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\n"));
				eventWriter.add(eventFactory.createEndElement("", "", "ecm"));
				if (param.humanReadable) eventWriter.add(eventFactory.createDTD("\n"));
				eventWriter.close();
				iConn.sendMessageToClientImmediately(xxx.toString("utf-8"));

				} else {
				iConn.sendMessageToClientImmediately(iConn.createResponse_error("no stmt!"));
			}
		} catch (Exception e) {
			logger.error(null, e);
			iConn.sendMessageToClientImmediately(iConn.createResponse_error(e.getMessage()));
		}
	}

}

