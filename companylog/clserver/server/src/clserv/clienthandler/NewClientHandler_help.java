package clserv.clienthandler;

import clserv.shared.I_ClientHandler;
import clserv.shared.I_ConnectionHandler;

public class NewClientHandler_help implements I_ClientHandler/*extends AClientHandler*/ {


/*	import javax.xml.parsers.DocumentBuilder;
	import javax.xml.parsers.DocumentBuilderFactory;
	import javax.xml.parsers.ParserConfigurationException;
	import javax.xml.transform.Transformer;
	import javax.xml.transform.TransformerConfigurationException;
	import javax.xml.transform.TransformerException;
	import javax.xml.transform.TransformerFactory;
	import javax.xml.transform.TransformerFactoryConfigurationError;
	import javax.xml.transform.dom.DOMSource;
	import javax.xml.transform.stream.StreamResult;

	import org.w3c.dom.Document;
	import org.w3c.dom.Element;
*/	
	void dummy() {
		/*	
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		Document doc = builder.newDocument();
		DOMSource source = new DOMSource(doc);


		Element element = doc.createElement("root");
		doc.appendChild(element);
*/
/*
for (int i = 0; i < array.length; i++) {
	Element elem = doc.createElement("Zeile");
	element.appendChild(elem);
	elem.setAttribute("id", "" + i);
	for (int j = 0; j < array[i].length; j++) {
		Element e = doc.createElement("spalte" + j);
		e.setTextContent(array[i][j]);
		elem.appendChild(e);
	}
}
StreamResult result = new StreamResult(new File("c:/data.xml"));

Transformer transformer = TransformerFactory.newInstance()
		.newTransformer();

transformer.transform(source, result);

} catch (ParserConfigurationException e) {
e.printStackTrace();
} catch (TransformerConfigurationException e) {
e.printStackTrace();
} catch (TransformerFactoryConfigurationError e) {
e.printStackTrace();
} catch (TransformerException e) {
e.printStackTrace();
}
}
*/
		
	}
	
	@Override
	public void handle(I_ConnectionHandler iConn, String command) throws Exception {
		iConn.sendMessageToClientImmediately(iConn.envelopClientMessage("help", null, createResponse_help()));
		return;
	}

	public String createResponse_help() {
		String r = "<cmd t=\"info\"/>";
		r += "<cmd t=\"help\"/>";
//		r += "<cmd t=\"xxx\"><parm p=\"i\" v=\"a,b,c\" descr=\"hallo welt\"/></cmd>";
		return r;
	}

}
