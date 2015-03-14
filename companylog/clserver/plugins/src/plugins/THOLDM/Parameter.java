package plugins.THOLDM;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import clserv.shared.I_XMLHandler;
import clserv.shared.XMLParser;


public final class Parameter implements I_XMLHandler {
	// Attributes: member				   name
	public String m_RequestType = null;	// t
	public int m_GameId = -1;			// game_id
	public String m_PlayerName = null;	// l
	public String m_GameName = null;	// game_name
	public int m_Amount = -1;			// bet
	
	@Override
	public void startElement(String uri, String localName, String name,	Attributes atts) throws SAXException {
		if (localName.equals("msg")) {
			m_RequestType = atts.getValue("t");
			String tmp = atts.getValue("game_id");
			if (tmp != null) {
				m_GameId = Integer.parseInt(tmp);
			}
			m_PlayerName = atts.getValue("l");
			m_GameName = atts.getValue("game_name");
			tmp = atts.getValue("amount");
			if (tmp != null) {
				m_Amount = Integer.parseInt(tmp);
			}
		}
	}
	
	public void parseCommand(String iCommand) {
		try {
			XMLParser xmlparser = new XMLParser(this);
			xmlparser.parseString(iCommand);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
