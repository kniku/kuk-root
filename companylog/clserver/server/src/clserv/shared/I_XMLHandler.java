package clserv.shared;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

public interface I_XMLHandler {
	
	public void startElement(String uri, String localName, String name,
			Attributes atts) throws SAXException;
}
