#include "xmlformatter.h"

#if 0
<!-- datetime format fix: "ccyymmddhhmmss" -->
<!-- double comma fix: "." -->


<!-- request -->
<esm t="sql">
	<stmt>select * from XXX where adate=:a and name=:b order by abc</stmt>
	<values>
		<value type="string|int|double|datetime">20091001120000</value>
		<value type="string|int|double|datetime">Huber</value>
	</values>
</esm>

<!-- response (result set) -->

<ecm t="sql">
	<header>
		<col type="string|int|double|datetime|blob">date</col>
		<col type="string|int|double|datetime|blob">name</col>
		<col type="string|int|double|datetime|blob">count</col>
	</header>
	<rows>
		<row>
			<value>20091001120000</value>
			<value>Huber</value>
			<value>100</value>
		</row>
		<row>
			<value>20091001120000</value>
			<value>Kremser</value>
			<value>250</value>
		</row>
	</rows>
</ecm>

#endif

XmlFormatter::XmlFormatter(QObject *parent) :QObject(parent)
{
	lastXmlResult="";
}

bool XmlFormatter::createESMHeader(QString t) {
	doc.clear();
	if (t.size()>0) {
		esm=doc.createElement("esm");
		esm.setAttribute("t",t);
		doc.appendChild(esm);
		return true;
	}
	return false;
}

bool XmlFormatter::appendSqlBindingValue(QDomElement *parentElement, QString type, QString valueString) {
	if (parentElement && type.size()>0) {
		QDomElement value=doc.createElement("value");
		value.setAttribute("type",type);
		QDomText t = doc.createTextNode(valueString);
		value.appendChild(t);
		QDomNode valueNode=parentElement->appendChild(value);
		//valueNode.setNodeValue(valueString);
		return true;
	}
	return false;
}

bool XmlFormatter::appendSqlBindings(QDomElement *parentElement, va_list vl) {
	int *p=0;
	bool have_first=false;
	if (parentElement && vl) {
		QDomElement values=doc.createElement("values");
		parentElement->appendChild(values);

		while((p=va_arg(vl, int*))) {
			if ((*p)==SQL_BASETYPE_STRING) {
				/* string */
				QString *v=va_arg(vl,QString*);
				if (v) {
					appendSqlBindingValue(&values, SQL_BASETYPE_STRING_V, *v);
				}
			} else if ((*p)==SQL_BASETYPE_INT) {
				int* v=va_arg(vl,int*);
				if (v) {
					appendSqlBindingValue(&values, SQL_BASETYPE_INT_V, QString::number(*v));
				}
			} else if ((*p)==SQL_BASETYPE_DOUBLE) {
				double* v=va_arg(vl,double*);
				if (v) {
					appendSqlBindingValue(&values, SQL_BASETYPE_DOUBLE_V, QString::number(*v));
				}
			} else if ((*p)==SQL_BASETYPE_DATETIME) {
				QDateTime* v=va_arg(vl,QDateTime*);
				if (v) {
					appendSqlBindingValue(&values, SQL_BASETYPE_DATETIME_V, v->toString("yyyyMMddhhmmss"));
				}
			}
		}
	}
	return true;
}

bool XmlFormatter::createSqlRequest(QString statement, va_list vl) {
	bool rv=false;
	if (statement.size()>0) {
		if (createESMHeader("sql")) {
			QDomElement stmtElement=doc.createElement("stmt");
			QDomText t = doc.createTextNode(statement);
			stmtElement.appendChild(t);

			//QDomElement stmtMsg=doc.createElement("msg");
			//QDomNode msg_node=esm.appendChild(stmtMsg);

			//QDomNode stmt_node=stmtMsg.appendChild(stmtElement);
			QDomNode stmt_node=esm.appendChild(stmtElement);
			////stmt_node.setNodeValue(statement);
			rv=true;
			if (vl && 1==0) {
				appendSqlBindings(&esm, vl);
			}
		}
	}
	return rv;
}

QString XmlFormatter::getLastFormattedXmlString() {
	return doc.toString();
}
