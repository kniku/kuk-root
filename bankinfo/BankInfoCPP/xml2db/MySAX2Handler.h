
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
//#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_USE

class MySAX2Handler : public DefaultHandler {
public:
	void startElement(
		const   XMLCh* const    uri,
		const   XMLCh* const    localname,
		const   XMLCh* const    qname,
		const   Attributes&     attrs
	);
    virtual void endElement
	(
		const XMLCh* const uri,
		const XMLCh* const localname,
		const XMLCh* const qname
	);
	virtual void characters
    (
        const   XMLCh* const    chars
        , const unsigned int    length
    );
	void fatalError(const SAXParseException&);
};
