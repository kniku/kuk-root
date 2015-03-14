package bitools.xml2db;

import java.io.FileInputStream;
import java.text.ParseException;
import java.text.SimpleDateFormat;

import org.apache.log4j.Logger;
import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.InputSource;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.XMLReaderFactory;


public class KontoinfoParser implements ContentHandler {
	private static Logger logger = Logger.getLogger(KontoinfoParser.class);

	private IKontoinfoHandler InfoHandler;

	private boolean inKonto = false;
	private boolean inKontonummer = false;
	private boolean inPosition = false;
	private boolean inPosBetrag = false;
	private boolean inPosDatum = false;
	private boolean inPosBuchungsdatum = false;
	private boolean inPosWaehrung = false;
	private boolean inPosInfo = false;
	
	private String Value = "";
	
	private SimpleDateFormat DateFormat = new SimpleDateFormat("dd.MM.yyyy");
	
	public KontoinfoParser(IKontoinfoHandler infoHandler) {
		InfoHandler = infoHandler;
	}
	
	public void parseKontoinfoDatei(String iFileName) throws Exception {
		logger.debug("Verarbeite Auszugsdatei: " + iFileName);
		XMLReader xmlreader = XMLReaderFactory.createXMLReader();
		xmlreader.setContentHandler(this);
		xmlreader.setFeature("http://apache.org/xml/features/nonvalidating/load-external-dtd", false);
		FileInputStream f = new FileInputStream(iFileName);
		InfoHandler.ParserStart();
		xmlreader.parse(new InputSource(f));
		InfoHandler.ParserEnde();
		logger.debug("Auszugsdatei fertig: " + iFileName);
	}
	
	@Override
	public void characters(char[] arg0, int arg1, int arg2) throws SAXException {
		if (inPosInfo||inPosWaehrung||inPosDatum||inPosBuchungsdatum||inPosBetrag||inKontonummer) {
			Value += new String(arg0, arg1, arg2);
		}
	}

	@Override
	public void startElement(String arg0, String arg1, String arg2,
			Attributes arg3) throws SAXException {
		setFlags(arg1, true);
	}

	private void setFlags(String iAttr, boolean iStart) {
		if (iAttr.equals("BankAcctTrnRec")) {
			inPosition = iStart;
			if (iStart) InfoHandler.PositionStart();
			else InfoHandler.PositionEnde();
		} else if (iAttr.equals("DepAcctRec")) {
			inKonto = iStart;
			if (iStart) InfoHandler.KontoStart();
			else InfoHandler.KontoEnde();
		} else if (inPosition) {
			if (iAttr.equals("Amt")) {
				inPosBetrag = iStart;
				if (!iStart) {
					Value = Value.replace(",", ".");
					InfoHandler.SetPosBetrag(Double.parseDouble(Value));
				}
			} else if (iAttr.equals("PostedDt")) {
				inPosBuchungsdatum = iStart;
				if (!iStart) {
					try {
						InfoHandler.SetPosBuchungsdatum(DateFormat.parse(Value));
					} catch (ParseException e) {
						logger.error(e);
					}
				}
			} else if (iAttr.equals("EffDt")) {
				inPosDatum = iStart;
				if (!iStart) {
					try {
						InfoHandler.SetPosDatum(DateFormat.parse(Value));
					} catch (ParseException e) {
						logger.error(e);
					}
				}
			} else if (iAttr.equals("CurCode")) {
				inPosWaehrung = iStart;
				if (!iStart) InfoHandler.SetPosWaehrung(Value);
			} else if (iAttr.equals("Memo")) {
				inPosInfo = iStart;
				if (!iStart) InfoHandler.SetPosInfo(Value);
			}
		} else if (inKonto) {
			if (iAttr.equals("AcctId")) {
				inKontonummer = iStart;
				if (!iStart) InfoHandler.SetKontoNummer(Value);
			}
		}
	}
	
	@Override
	public void endElement(String arg0, String arg1, String arg2)
			throws SAXException {
		setFlags(arg1, false);
		Value = "";
	}

	@Override
	public void endPrefixMapping(String arg0) throws SAXException {
		// nothing to do
	}

	@Override
	public void ignorableWhitespace(char[] arg0, int arg1, int arg2)
			throws SAXException {
		// nothing to do
	}

	@Override
	public void processingInstruction(String arg0, String arg1)
			throws SAXException {
		// nothing to do
	}

	@Override
	public void setDocumentLocator(Locator arg0) {
		// nothing to do
	}

	@Override
	public void skippedEntity(String arg0) throws SAXException {
		// nothing to do
	}

	@Override
	public void startDocument() throws SAXException {
		// nothing to do
	}

	@Override
	public void endDocument() throws SAXException {
		// nothing to do
	}

	@Override
	public void startPrefixMapping(String arg0, String arg1)
			throws SAXException {
		// nothing to do
	}

}
