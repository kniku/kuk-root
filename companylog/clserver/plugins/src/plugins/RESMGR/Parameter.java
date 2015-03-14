package plugins.RESMGR;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import clserv.shared.I_XMLHandler;
import clserv.shared.XMLParser;


public final class Parameter implements I_XMLHandler {
	// Attributes: member				   name
	public String m_RequestType = null;	
	public String m_ResourceName = null;	
	public String m_Login = null;	
	public String m_ResourceValue = null;	
	
	@Override
	public void startElement(String uri, String localName, String name,	Attributes atts) throws SAXException {
		if (localName.equals("msg")) {
			m_RequestType = atts.getValue("t");
			m_Login = atts.getValue("l");
			m_ResourceName = atts.getValue("name");
			m_ResourceValue = atts.getValue("value");
		}
	}
	
	public void parseCommand(String iCommand) {
		if (iCommand != null) {
			try {
				XMLParser xmlparser = new XMLParser(this);
				xmlparser.parseString(iCommand);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
}
