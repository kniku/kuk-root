#include "main.h"
#include "MySAX2Handler.h"
#include "BankAcctTrnRec.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/algorithm/string.hpp>
#include <openssl/md5.h>
#include <datenmodell.h>

static BankAcctTrn iRec;
static Tag_Id currentTagId;
static bool	inside_DepAcctRec = false;
static bool	inside_BankAcctTrnRec = false;
static long currentKontoId = 0;

void MySAX2Handler::characters
    (
        const   XMLCh* const    chars
        , const unsigned int    length
    )
{
	if (currentTagId != TagId_None) {
		char* message = XMLString::transcode(chars);
		switch (currentTagId) {
		case TagId_TrnType:
			iRec.TrnType = message;
			break;
		case TagId_Memo:
			iRec.Memo += message;
			break;
		case TagId_CurCode:
			iRec.CurCode = message;
			break;
		case TagId_Amt:
			for (char *p = message; *p ; p++) if (*p == ',') *p = '.';
			iRec.Amt = atof(message);
			break;
		case TagId_EffDt:
			iRec.EffDt = message;
			break;
		case TagId_PostedDt:
			iRec.PostedDt = message;
			break;
		case TagId_AcctId:
			if (appGlobals.mode_import) {
				char KtoNr[MAX_LEN_STR035+1];
				char CurCode[MAX_LEN_WAEHRUNG+1];
				string wc = "select id,ktonr,curcode from konten where ktonr='";
				wc = wc + message + "'";
				CStatement *stmt = appGlobals.pConn->createStatement();
				stmt->Prepare(wc.c_str());
				stmt->BindVa(
						BINDRESULT_Long(currentKontoId),
						BINDRESULT_String(KtoNr, sizeof(KtoNr)),
						BINDRESULT_String(CurCode, sizeof(CurCode)),
						BIND_None);
				stmt->Execute();
				stmt->createResult()->Next();
				appGlobals.pConn->destroyStatement(stmt);
				cout << "[Konto : Id=" << currentKontoId << ", " << KtoNr << ", " << CurCode << "]" << endl;
			} else {
				cout << "[Konto : " << message << " (not verified)]" << endl;
			}
			break;
		}
//		currentTagId = TagId_None;
		XMLString::release(&message);
	}
}

void MySAX2Handler::startElement(const   XMLCh* const    uri,
							const   XMLCh* const    localname,
							const   XMLCh* const    qname,
							const   Attributes&     attrs)
{
	char* message = XMLString::transcode(localname);
	currentTagId = TagId_None;
	if (inside_DepAcctRec) {
		if (strcmp(message, TAGID_AcctId) == 0) {
			currentTagId = TagId_AcctId;
		}
	} else {
		if (inside_BankAcctTrnRec) {
			if (strcmp(message, TAGID_TrnType) == 0) {
				currentTagId = TagId_TrnType;
			} else if (strcmp(message, TAGID_Memo) == 0) {
				currentTagId = TagId_Memo;
			} else if (strcmp(message, TAGID_CurAmt) == 0) {
				// nothing to do
			} else if (strcmp(message, TAGID_CurCode) == 0) {
				currentTagId = TagId_CurCode;
			} else if (strcmp(message, TAGID_Amt) == 0) {
				currentTagId = TagId_Amt;
			} else if (strcmp(message, TAGID_EffDt) == 0) {
				currentTagId = TagId_EffDt;
			} else if (strcmp(message, TAGID_PostedDt) == 0) {
				currentTagId = TagId_PostedDt;
			} else {
				cout << "unknown element: "<< message << endl;
			}
		} else {
			if (strcmp(message, TAGID_BankAcctTrnRec) == 0) {
				iRec.reset();
				inside_BankAcctTrnRec = true;
			} else if (strcmp(message, TAGID_DepAcctRec) == 0) {
				inside_DepAcctRec = true;
			}
		}
	}
	XMLString::release(&message);
}


//static stringstream *dateFormatter;
static stringstream dateFormatter;
static boost::gregorian::date_input_facet* inputFactet = 0;
static boost::gregorian::date dateBuffer;
static CStatement *stmt = 0;
static DbiGen_M::Positionen rPos;

static void write_position(void) {
	if (inputFactet == 0) {
//		dateFormatter = new stringstream;
		inputFactet = new boost::gregorian::date_input_facet("%d.%m.%Y");
		dateFormatter.imbue(locale(dateFormatter.getloc(), inputFactet));
	}
	rPos.reset();

	rPos.getData()->KtoId = currentKontoId;

	strncpy(rPos.getData()->CurCode, iRec.CurCode.c_str(), MAX_LEN_WAEHRUNG);
	strncpy(rPos.getData()->Memo, iRec.Memo.c_str(), MAX_LEN_MEMO);
	rPos.getData()->Amt = iRec.Amt;

	boost::algorithm::trim(iRec.EffDt);
	dateFormatter.str(iRec.EffDt);
	dateFormatter >> dateBuffer;

	rPos.getData()->EffDt.Year = dateBuffer.year();
	rPos.getData()->EffDt.Month = dateBuffer.month();
	rPos.getData()->EffDt.Day = dateBuffer.day();

	boost::algorithm::trim(iRec.PostedDt);
	dateFormatter.str(iRec.PostedDt);
	dateFormatter >> dateBuffer;

	rPos.getData()->PostedDt.Year = dateBuffer.year();
	rPos.getData()->PostedDt.Month = dateBuffer.month();
	rPos.getData()->PostedDt.Day = dateBuffer.day();

	MD5_CTX md5ctx;
	MD5_Init(&md5ctx);
	MD5_Update(&md5ctx, rPos.getData(), sizeof(DbiGen_M::D_Positionen));
	unsigned char buffer[16+1];
	memset(buffer,0,sizeof(buffer));
	MD5_Final(buffer, &md5ctx);
	for (unsigned int i = 0; i < 16; i++) {
		sprintf(&rPos.getData()->Id[i*2], "%2.2x", (int)buffer[i]);
	}
	cout << "[" << rPos.getData()->Id << "] " << rPos.getData()->EffDt.Day << '.' << rPos.getData()->EffDt.Month << '.' <<  rPos.getData()->EffDt.Year << ": " << rPos.getData()->Amt << " " << rPos.getData()->CurCode << endl;
	if (stmt == 0) {
		stmt = appGlobals.pConn->createStatement();
		stmt->Prepare("insert into positionen (id,ktoid,curcode,memo,amt,effdt,posteddt) values (:a,:b,:c,:d,:e,:f,:g)");
		stmt->BindVa(
				BINDPARAM_String(rPos.getData()->Id, MAX_LEN_MD5),
				BINDPARAM_Long(rPos.getData()->KtoId),
				BINDPARAM_String(rPos.getData()->CurCode, MAX_LEN_WAEHRUNG),
				BINDPARAM_String(rPos.getData()->Memo, MAX_LEN_MEMO),
				BINDPARAM_Double(rPos.getData()->Amt),
				BINDPARAM_DateTime(rPos.getData()->EffDt),
				BINDPARAM_DateTime(rPos.getData()->PostedDt),
				BIND_None);
	}
	try {
		stmt->Execute();
	} catch (const klib::base::Exception &e) {
		if (e.getErrno() == 1062) {
			//cout << "duplicate entry!" << endl;
			appGlobals.cntPosDuplicate++;
		} else {
			throw (e);
		}
	}
}

void MySAX2Handler::endElement
(
	const XMLCh* const uri,
	const XMLCh* const localname,
	const XMLCh* const qname
	)
{
	char* message = XMLString::transcode(localname);
	if (inside_BankAcctTrnRec && strcmp(message, TAGID_BankAcctTrnRec) == 0) {
		if (appGlobals.mode_verbose) {
			iRec.dump();
		}
		appGlobals.cntPos++;
		if (iRec.Amt >= 0.0) {
			appGlobals.AmtIn += iRec.Amt;
		} else {
			appGlobals.AmtOut += iRec.Amt;
		}
		inside_BankAcctTrnRec = false;
		if (appGlobals.mode_import) {
			write_position();
		}
	} else if (inside_DepAcctRec && strcmp(message, TAGID_DepAcctRec) == 0) {
		inside_DepAcctRec = false;
	}
	currentTagId = TagId_None;
	XMLString::release(&message);
}

void MySAX2Handler::fatalError(const SAXParseException& exception)
{
	char* message = XMLString::transcode(exception.getMessage());
	cout << "Fatal Error: " << message
			<< " at line: " << exception.getLineNumber()
			<< endl;
}
