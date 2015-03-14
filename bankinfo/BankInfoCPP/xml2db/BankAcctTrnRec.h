#ifndef BANKACCTTRN_H
#define BANKACCTTRN_H

/*
<BankAcctTrnRec>
	<TrnType>Transfer</TrnType>
	<PostedDt>31.03.2004</PostedDt>
	<EffDt> 01.04.2004</EffDt> 
	<CurAmt>
		<Amt>-109,80</Amt>
		<CurCode>EUR</CurCode>
	</CurAmt>
	<Memo>Sammelbuchung Ums&#xE4;tze BA/CA-MasterCard</Memo> 
</BankAcctTrnRec>
*/

#define TAGID_DepAcctRec		"DepAcctRec"

#define TAGID_AcctId			"AcctId"

#define TAGID_BankAcctTrnRec	"BankAcctTrnRec"

#define TAGID_TrnType			"TrnType"
#define TAGID_Memo				"Memo"
#define TAGID_CurAmt			"CurAmt"
#define TAGID_CurCode			"CurCode"
#define TAGID_Amt				"Amt"
#define TAGID_EffDt				"EffDt"
#define TAGID_PostedDt			"PostedDt"

enum Tag_Id {
	TagId_None,
	TagId_TrnType,
	TagId_Memo,
	TagId_CurAmt,
	TagId_CurCode,
	TagId_Amt,
	TagId_EffDt,
	TagId_PostedDt,
	TagId_AcctId,
	TagId_Last
};

class BankAcctTrn {
public:
	string TrnType;
	string Memo;
	string CurCode;
	double Amt;
	string PostedDt;
	string EffDt;
	void reset(void);
	void dump(void);
};

#if 0
// generated code

#define MAX_Memo	512
#define MAX_CurCode	3

struct DbiPositionen {
	long Id;
	long KtoId;
	char CurCode[MAX_CurCode+1];
	char Memo[MAX_Memo+1];
	double Amt;
	klib::dbi::DateTime EffDt;
	klib::dbi::DateTime PostedDt;
};
#endif
#endif // BANKACCTTRN_H
