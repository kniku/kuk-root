package plugins.RESMGR;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.text.SimpleDateFormat;
import java.util.HashMap;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import clserv.shared.Helper;
import clserv.shared.I_XMLHandler;
import clserv.shared.XMLParser;


public class Resource implements I_XMLHandler {
	private static SimpleDateFormat dateformat = new SimpleDateFormat("yyyyMMddHHmmss");

	/*
	 * Sequence:
	 * <RESOURCE name="XXX"  type="sequence" value="100"/>
	 * 
	 * Value:
	 * <RESOURCE name="XXX" type="values">
	 * 	<Value value="100"/>
	 * 	<Value value="200" allocatedFrom="kuk" allocatedSince"20090530121510"/>
	 * 	<Value value="300"/>
	 * 	...
	 * </RESOURCE>
	 */

	public class Value {
		public String m_Value = null;
		public String m_AllocatedFrom = null;
		public String m_AllocatedSince = null;
	}
	
	private String m_File = null;
	
	// Attributes: member				   name
	public String m_ResourceType = null;	// sequence|values	
	public String m_ResourceName = null;	// != null
	public int m_ResourceValue = 0;
	public HashMap<String,Value> m_Values = null;
	
	@Override
	public void startElement(String uri, String localName, String name,	Attributes atts) throws SAXException {
		if (localName.equals("RESOURCE")) {
			m_ResourceType = atts.getValue("type");
			if (m_ResourceType == null) m_ResourceType = "sequence";
			m_ResourceName = atts.getValue("name");
			String s = atts.getValue("value");
			if (s != null) m_ResourceValue = Integer.parseInt(s);
		}
		if (localName.equals("Value") && atts.getValue("value") != null) {
			if (m_Values == null) m_Values = new HashMap<String,Value>();
			Value aValue = new Value();
			aValue.m_Value = atts.getValue("value");
			aValue.m_AllocatedFrom = atts.getValue("allocatedFrom");
			aValue.m_AllocatedSince = atts.getValue("allocatedSince");
			m_Values.put(aValue.m_Value, aValue);
		}
	}

	public void parseCommand(String iCommand) {
		if (iCommand != null) {
			try {
				XMLParser xmlparser = new XMLParser(this);
				xmlparser.parseString(iCommand);
			} catch (Exception e) {
				// just catch it...
			}
		}
	}
	
	public void saveResource() {
		String content = "<RESOURCE name=\""+m_ResourceName+"\" type=\""+m_ResourceType+"\"";
		if (m_ResourceType.equals("sequence")) {
			content += " value=\""+m_ResourceValue+"\"/>";
		} else {
			content += ">";
			for (Value aValue : m_Values.values()) {
				content += "\n\t<Value value=\""+aValue.m_Value+"\"";
				if (aValue.m_AllocatedFrom != null) content += " allocatedFrom=\""+aValue.m_AllocatedFrom+"\"";
				if (aValue.m_AllocatedSince != null) content += " allocatedSince=\""+aValue.m_AllocatedSince+"\"";
				content += "/>";
			} 
			content += "\n</RESOURCE>";
		}
		FileWriter writer;
		try {
			writer = new FileWriter(m_File, false);
			writer.write(content);
			writer.close();
		} catch (Exception e) {
			// just catch it...
		}
		
	}
	
	public static Resource loadResource(String iFileName) {
		Resource r = null;
		BufferedReader InputStream = null;
		try {
			InputStream = new BufferedReader(new FileReader(iFileName));
			String aLine, content = "";
			do {
				aLine = InputStream.readLine();
				if (aLine != null) content += aLine;
			} while (aLine != null);
			InputStream.close();
			r = new Resource();
			r.m_File = iFileName;
			if (content.equals("")) {
				File x = new File(iFileName);
				content = "<RESOURCE name=\"" + x.getName().substring(0, x.getName().length() - 4) + "\"/>";
			}
			r.parseCommand(content);
			if (r.isValid() == false) r = null;
		} catch (Exception e) {
			// just catch it...
			r = null;
		}
		
		return r;
	}
	
	private boolean isValid() {
		boolean r = false;
		if (m_ResourceName != null && m_ResourceType != null) {
			if (m_ResourceType.equals("values")) {
				if (m_Values != null && m_Values.isEmpty() == false) {
					r = true;
				}
			} else {
				r = true;
			}
		}
		return r;
	}

	// getter
	
	public String getName() {
		return m_ResourceName;
	}

	public String getNextValue(String iLogin) {
		String r = null;
		if (m_ResourceType.equals("sequence")) {
			m_ResourceValue++;
			r = Integer.toString(m_ResourceValue);
		} else {
			for (Value aValue : m_Values.values()) {
				if (aValue.m_AllocatedFrom == null) {
					aValue.m_AllocatedFrom = iLogin == null ? "???" : iLogin;
					aValue.m_AllocatedSince = dateformat.format(Helper.getCurrentTime());
					r = aValue.m_Value;
					break;
				}
			}
			if (r == null) r = "none";
		}
		if (r != null  && r.equals("none") == false) saveResource();
		return r;
	}
	
	public boolean releaseValue(String iLogin, String iValue) {
		boolean r = false;
		if (m_Values != null) {
			Value aValue = m_Values.get(iValue);
			if (aValue != null && aValue.m_AllocatedFrom != null) {
				aValue.m_AllocatedFrom = aValue.m_AllocatedSince = null;
				saveResource();
				r = true;
			}
		}
		return r;
	}
	
	public String getStatus() {
		String r = null;
		if (m_ResourceType.equals("sequence")) {
			r = Integer.toString(m_ResourceValue + 1);
		} else {
			r = "";
			for (Value aValue : m_Values.values()) {
				r += "\n<Value value=\"" + aValue.m_Value + "\"";
				if (aValue.m_AllocatedFrom != null) {
					r += " allocatedFrom=\"" + aValue.m_AllocatedFrom + "\"";				
					r += " allocatedSince=\"" + aValue.m_AllocatedSince + "\"";				
				}
				r += "/>";
			}
			r += "\n";
		}
		return r;
	}
}
