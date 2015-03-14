#include "main.h"
#include "BankAcctTrnRec.h"

void BankAcctTrn::reset(void) {
	TrnType = ""; Memo = ""; CurCode = ""; Amt = 0.0; PostedDt = ""; EffDt = "";
}

void BankAcctTrn::dump(void) {
	cout << "[" << endl
		<< "\tTrnType=" << TrnType << endl
		<< "\tMemo=" << Memo << endl
		<< "\tCurCode=" << CurCode << endl
		<< "\tAmt=" << Amt << endl
		<< "\tPostedDt=" << PostedDt << endl
		<< "\tEffDt=" << EffDt << endl
		<< "]" << endl;
}
