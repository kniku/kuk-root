#include "sqlworksheet.h"
#include "ui_sqlworksheet.h"

SqlWorksheet::SqlWorksheet(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::SqlWorksheet)
{
    m_ui->setupUi(this);
}

SqlWorksheet::~SqlWorksheet()
{
    delete m_ui;
}

void SqlWorksheet::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SqlWorksheet::on_pushButtonAusfuehren_clicked()
{
	QString statement;
	QString result;
	statement=this->m_ui->textEditStatement->toPlainText();
	this->m_ui->textEditResult->clear();
	if (statement.size()>0) {
		SqlTunnel sqlTunnel;
		if (sqlTunnel.sendPlainSqlStatement(statement,0)) {
			RecordList* recordList = sqlTunnel.lastResultAsRecordList();
			if (recordList) {
				int sizeColumnDescriptor=recordList->sizeColumnDescriptor();
				QString headerLine;
				for (int i=0;i<sizeColumnDescriptor;i++) {
					RecordColumnDescriptor rd=recordList->recordColumnDescriptor(i);
					headerLine.append(rd.name()).append(";");
				}
                if (headerLine.size()>0) {
                    headerLine+="\n";
                }
                if (!recordList->haveError()) {
                    for (int r=0;r<recordList->size();r++) {
                        RecordRow rr=recordList->row(r);
                        for(int c=0;c<rr.size();c++) {
                            RecordValue v=rr.column(c);
                            if (v.type()==SQL_BASETYPE_STRING) {
                                headerLine.append(v.valueString());
                            } else if (v.type()==SQL_BASETYPE_INT) {
                                headerLine.append(QString::number(v.valueInt()));
                            } else if (v.type()==SQL_BASETYPE_DOUBLE) {
                                headerLine.append(QString::number(v.valueDouble()));
                            } else if (v.type()==SQL_BASETYPE_DATETIME) {
                                headerLine.append(v.valueDateTime().toString("yyyyMMddhhmmss"));
                            } else if (v.type()==SQL_BASETYPE_BLOB) {
                                headerLine.append("BLOB");
                            }
                            headerLine.append(";");
                        }
                        headerLine+="\n";
                    }
                } else {
                    headerLine+=recordList->errorMessage()+"\n";
                }
				this->m_ui->textEditResult->insertPlainText(headerLine);
				delete recordList;
			}
		}
	}
}
