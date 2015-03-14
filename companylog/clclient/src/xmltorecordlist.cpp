#include "xmltorecordlist.h"

#if 0
<ecm t="sql">
	<header>
		<col t="string|int|double|datetime|blob">date</col>
		<col t="string|int|double|datetime|blob">name</col>
		<col t="string|int|double|datetime|blob">count</col>
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

XmlToRecordList::XmlToRecordList(QObject *parent) :QObject(parent)
{
}

SQL_BASETYPE XmlToRecordList::getTypeFromString(QString tString) {
	if (!tString.compare(SQL_BASETYPE_STRING_V,Qt::CaseInsensitive)) {
		return SQL_BASETYPE_STRING;
	}
	if (!tString.compare(SQL_BASETYPE_INT_V,Qt::CaseInsensitive)) {
		return SQL_BASETYPE_INT;
	}
	if (!tString.compare(SQL_BASETYPE_DOUBLE_V,Qt::CaseInsensitive)) {
		return SQL_BASETYPE_DOUBLE;
	}
	if (!tString.compare(SQL_BASETYPE_DATETIME_V,Qt::CaseInsensitive)) {
		return SQL_BASETYPE_DATETIME;
	}
	if (!tString.compare(SQL_BASETYPE_BLOB_V,Qt::CaseInsensitive)) {
		return SQL_BASETYPE_BLOB;
	}
	return SQL_BASETYPE_STRING;
}

/*
 * rv RecordList is allocated!!
 */
RecordList *XmlToRecordList::translate(QString *xmlFile) {
	int		header_col_count=0;
	RecordList *recordList=new RecordList();

	recordList->clear();
	if (xmlFile && xmlFile->size()>0) {
		QDomDocument doc( "AdBookML" );
		doc.setContent(*xmlFile);
		QDomElement root = doc.documentElement();
		QString t=root.attribute("t", "" );
		if (t=="sql") {
			QDomNodeList header=root.elementsByTagName("header");
			if (header.count()>0) {
				QDomNode header0_node=header.item(0);
				if (header0_node.isElement()) {
					QDomElement header0 = header0_node.toElement();
					QDomNodeList cols=header0.elementsByTagName("col");
					header_col_count=cols.count();
					for (int col_index=0;col_index<header_col_count;col_index++) {
						QDomNode col_node=cols.item(col_index);
						if (col_node.isElement()) {
							QDomElement col = col_node.toElement();
							QString type = col.attribute( "t", "" );
							QDomNodeList content_node_list=col.childNodes();
							if (content_node_list.count()>0) {
								QDomNode content_node=content_node_list.item(0);
								if (content_node.isText()) {
									QDomText txt=content_node.toText();
									RecordColumnDescriptor rd;
									rd.setType(getTypeFromString(type));
									rd.setName(txt.data());
									recordList->appendRecordDescriptor(rd);
								}
							}
						}
					}
				}
			}
			QDomNodeList rowsxx=root.elementsByTagName("rows");
			if (rowsxx.count()>0) {
				QDomNode rowxx0_node=rowsxx.item(0);
				if (rowxx0_node.isElement()) {
					QDomElement rowxx0 = rowxx0_node.toElement();
                                        QDomNodeList rows=rowxx0.elementsByTagName("r");

					for (int row_index=0;row_index<rows.count();row_index++) {
						QDomNode row_node=rows.item(row_index);
						if (row_node.isElement()) {
							QDomElement row = row_node.toElement();

                                                        QDomNodeList vals=row.elementsByTagName("v");

							RecordRow rr;

							int val_col_count=vals.count();
							if (val_col_count==header_col_count) {
								for (int val_index=0;val_index<val_col_count;val_index++) {
									QDomNode val_node=vals.item(val_index);
									QDomNodeList content_node_list=val_node.childNodes();
									if (content_node_list.size()>0) {
										QDomNode content_node=content_node_list.item(0);
										if (content_node.isText()) {
											QDomText val=content_node.toText();
											QString test=val.data();

											RecordValue rv;
											RecordColumnDescriptor rd=recordList->recordColumnDescriptor(val_index);
											if (rd.type()==SQL_BASETYPE_STRING) {
												rv.setValueString(val.data());
											} else if (rd.type()==SQL_BASETYPE_INT) {
												rv.setValueInt(val.data().toInt());
											} else if (rd.type()==SQL_BASETYPE_DOUBLE) {
												rv.setValueDouble(val.data().toDouble());
											} else if (rd.type()==SQL_BASETYPE_DATETIME) {
												rv.setValueDateTime(QDateTime::fromString(val.data(),"yyyyMMddhhmmss"));
											} else if (rd.type()==SQL_BASETYPE_BLOB) {
												rv.setValueBlob(val.data().toAscii());
											}
											rr.appendColumn(rv);
											/* insert to list */
										}
									}
								}
								recordList->appendRow(rr);
							}
						}
					}
				}
			}
        } else if (t=="error") {
            QDomNodeList msg=root.elementsByTagName("msg");
            QString errorMessage="";
            if (msg.count()>0) {
                QDomNode msg0_node=msg.item(0);
                QDomNodeList msg_node_list=msg0_node.childNodes();
                if (msg_node_list.size()>0) {
                    QDomNode msg0_node_content=msg_node_list.item(0);
                    if (msg0_node_content.isText()) {
                        QDomText val=msg0_node_content.toText();
                        errorMessage=val.data();
                    }
                }
            }
            if (errorMessage.size()==0) {
                errorMessage="unknown error";
            }
            recordList->setErrorMessage(errorMessage);
        }
	}
	return recordList;
}
