
CREATE TABLE MLTRANS_HISTORY (
	mcode VARCHAR2(3) DEFAULT '   ' NOT NULL,
	Look VARCHAR2(10) DEFAULT '          ' NOT NULL,
	Lang VARCHAR2(3) DEFAULT '   ' NOT NULL,
	App VARCHAR2(10) DEFAULT '          ' NOT NULL,
	Modul VARCHAR2(80) DEFAULT '                                                                                ' NOT NULL,
	Dialog VARCHAR2(80) DEFAULT '                                                                                ' NOT NULL,
	TextKey VARCHAR2(1000) DEFAULT '                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ' NOT NULL,
	Translation VARCHAR2(1000),
	isActive VARCHAR2(1) DEFAULT 'Y' NOT NULL CONSTRAINT CMLTRANS_HISTORY_isActive CHECK (isActive IN ('N', 'Y')),
	TextMD5 VARCHAR2(32) DEFAULT '                                ' NOT NULL,
	UpdKz VARCHAR2(1) DEFAULT 'N' NOT NULL CONSTRAINT CMLTRANS_HISTORY_UpdKz CHECK (UpdKz IN ('N', 'C', 'D')),
	UpdIP VARCHAR2(35),
	UpdTs DATE DEFAULT TO_DATE('0001.01.02 00:00:00','YYYY.MM.DD HH24:MI:SS') NOT NULL,
	CONSTRAINT PK_MLTRANS_HISTORY PRIMARY KEY (mcode,Look,Lang,App,Modul,Dialog,TextMD5,UpdTs)
);
