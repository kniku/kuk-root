from pythonlib import Parser
from pythonlib.ParserConst import *

def getDefaultValueForType(defValue, iType):
	sqlType = mapXMLType(iType.getType())
	if defValue == XML_VALUE_SQLDEFAULT_DEFAULT:
		if sqlType == VALUE_TYPE_STRING:
			defValue = ' ' * int(iType.getAttribute(XML_ATTR_LENGTH))
		elif sqlType == VALUE_TYPE_NUMERIC:
			defValue = '0'
		elif sqlType == VALUE_TYPE_DATE:
			defValue = '00010201000000'
		else:
			pass
	elif defValue == XML_VALUE_SQLDEFAULT_NODEFAULT:
		defValue = 0
	return defValue
	
def getSqlType(iTable, iMember, iMode):
	oAfterTableText = ""
	iType = iMember.getType()
	sqlType = mapXMLType(iType.getType())
	
	defValue = 0
	if len(getChildElementsByTagName(iMember.getObject(),XML_TAG_SQL)) > 0:
		if len(getChildElementsByTagName(getChildElementsByTagName(iMember.getObject(),XML_TAG_SQL)[0],XML_TAG_DEFAULT)) > 0:
			defValue = getChildElementsByTagName(getChildElementsByTagName(iMember.getObject(),XML_TAG_SQL)[0],XML_TAG_DEFAULT)[0].firstChild.data
	elif len(getChildElementsByTagName(iMember.getType().getObject(),XML_TAG_SQL)) > 0:
		if len(getChildElementsByTagName(getChildElementsByTagName(iMember.getType().getObject(),XML_TAG_SQL)[0],XML_TAG_DEFAULT)) > 0:
			defValue = getChildElementsByTagName(getChildElementsByTagName(iMember.getType().getObject(),XML_TAG_SQL)[0],XML_TAG_DEFAULT)[0].firstChild.data
	if defValue:
		defValue = getDefaultValueForType(defValue, iType)
	
	if iMode == XML_VALUE_MODE_MYSQL5:
		if sqlType == VALUE_TYPE_STRING:
			sqlType = "varchar("+iType.getLength()+")"
			if defValue:
				defValue = '\''+defValue+'\''
		elif sqlType == VALUE_TYPE_NUMERIC:
			#sqlType = "numeric("+iType.getLength()+","+iType.getScale()+")"
			if int(iType.getAttribute(XML_ATTR_LENGTH)) < 10 and int(iType.getAttribute(XML_ATTR_SCALE)) == 0:
				sqlType = "int"
			else:
				sqlType = "double"	
		elif sqlType == VALUE_TYPE_DATE:
			sqlType = 'datetime'
			if defValue:
				if len(defValue) == 8:
					defValue += '000000'
				defValue = '\''+defValue[0:4]+'-'+defValue[4:6]+'-'+defValue[6:8]+' '+defValue[8:10]+':'+defValue[10:12]+':'+defValue[12:14]+'\''
		elif sqlType == VALUE_TYPE_AUTO:
			sqlType = 'int unsigned auto_increment'
			defValue = 0
		else:
			raise SyntaxError("can't determine sql type for type: " + iType.getType())
			
		if defValue:
			sqlType = sqlType + ' default ' + defValue
		if not isBoolTrue(iMember.getAttribute(XML_ATTR_NULL)):
			sqlType = sqlType + ' NOT NULL'
		if iMember.getAttribute(XML_ATTR_DESCR):
			sqlType = sqlType + ' comment \'' + iMember.getAttribute(XML_ATTR_DESCR) + '\''
			
	elif iMode == XML_VALUE_MODE_ORACLE9:
		if sqlType == VALUE_TYPE_STRING:
			sqlType = "varchar2("+iType.getLength()+")"
			if defValue:
				defValue = '\''+defValue+'\''
		elif sqlType == VALUE_TYPE_NUMERIC:
			sqlType = "numeric("+iType.getLength()+","+iType.getScale()+")"
		elif sqlType == VALUE_TYPE_DATE:
			sqlType = 'date'
			if defValue:
				if len(defValue) == 8:
					defValue += '000000'
				defValue = 'to_date(\''+defValue[0:4]+'.'+defValue[4:6]+'.'+defValue[6:8]+' '+defValue[8:10]+':'+defValue[10:12]+':'+defValue[12:14]+'\',\'YYYY.MM.DD HH24:MI:SS\')'
		elif sqlType == VALUE_TYPE_AUTO:
			sqlType = 'numeric(9,0)'
			oAfterTableText += 'create sequence seq_'+iTable.getName()+'_'+iMember.getName()+' start with 1 increment by 1 maxvalue 999999999;\n'
			oAfterTableText += 'create trigger trg_'+iTable.getName()+'_'+iMember.getName()+' before insert on '+iTable.getName()+' for each row begin select seq_'+iTable.getName()+'_'+iMember.getName()+'.nextval into :new.'+iMember.getName()+' from dual; end;\n/\n'
			defValue = 0
		else:
			raise SyntaxError("can't determine sql type for type: " + iType.getType())
		
		if defValue:
			sqlType = sqlType + ' default ' + defValue
		if not isBoolTrue(iMember.getAttribute(XML_ATTR_NULL)):
			sqlType = sqlType + ' NOT NULL'
	else:
		raise SyntaxError("invalid DBMS Type: "+iMode)
	
	return sqlType, oAfterTableText
	
def getForKeyString(iTable, iForkey, iMode, iCount):
	conName = iForkey.getAttribute(XML_ATTR_NAME);
	colStr = tmpKomma = ''
	for iCol in getChildElementsByTagName(iForkey, XML_TAG_COLUMN):
		colStr += tmpKomma+iCol.firstChild.data
		tmpKomma = ','
	fortabStr = getChildElementsByTagName(iForkey,XML_TAG_FORTABLE)[0].getAttribute(XML_ATTR_NAME)
	forcolStr = tmpKomma = ''
	for iCol in getChildElementsByTagName(getChildElementsByTagName(iForkey,XML_TAG_FORTABLE)[0],XML_TAG_COLUMN):
		forcolStr += tmpKomma+iCol.firstChild.data
		tmpKomma = ','
	if not conName:
		conName = 'FK_'+iTable.getAttribute(XML_ATTR_NAME)+'_'+fortabStr+'_'+iCount
	if iMode == XML_VALUE_MODE_MYSQL5 or iMode == XML_VALUE_MODE_ORACLE9:
		r = 'alter table '+iTable.getAttribute(XML_ATTR_NAME)+' add constraint ' + conName+' foreign key (' + colStr + ')'+' references '+fortabStr+' ('+forcolStr+');'
	else:
		raise SyntaxError("invalid DBMS Type: "+iMode)
	
	return r

def getIndexString(iTable, iIndex, iMode, iCount):
	conName = iIndex.getAttribute(XML_ATTR_NAME);
	if not conName:
		conName = 'IDX_'+iTable.getAttribute(XML_ATTR_NAME)+'_'+iCount
		
	indexStr = tmpKomma = ''
	for iCol in getChildElementsByTagName(iIndex,XML_TAG_COLUMN):
		indexStr += tmpKomma+iCol.firstChild.data
		tmpKomma = ','
	if iMode == XML_VALUE_MODE_MYSQL5:
		if isBoolTrue(iIndex.getAttribute(XML_ATTR_UNIQUE)):
			r = 'alter table ' + iTable.getAttribute(XML_ATTR_NAME) + ' add constraint unique index ' + conName + ' (' + indexStr + ');'
		else:
			r = 'alter table ' + iTable.getAttribute(XML_ATTR_NAME) + ' add index ' + conName + ' (' + indexStr + ');'
	elif iMode == XML_VALUE_MODE_ORACLE9:
		if isBoolTrue(iIndex.getAttribute(XML_ATTR_UNIQUE)): iUnique = ' unique '
		else: iUnique = ' '
		r = 'create'+iUnique+'index ' + conName + ' on ' + iTable.getAttribute(XML_ATTR_NAME) + ' (' + indexStr + ');'
	else:
		raise SyntaxError("invalid DBMS Type: "+iMode)
	
	return r

def generate(dictDbi, iGenerator):
	Mode = iGenerator.getMode()
	Target = iGenerator.getTarget()
	F = open(Target, "wt")
	F.write("/*\n * DBD-GenSql auto generated sql script (create table)\n * Mode = "+Mode+"\n */\n\n")

	dictTables = dictDbi[Parser.DICT_TABLES]
	for iTableName in dictTables.keys():
		iTable = dictTables[iTableName]
		if iTable.getAttribute(XML_ATTR_DESCR): F.write("/* " + iTable.getAttribute(XML_ATTR_DESCR) + '*/\n')
		F.write("/* implements structure " + iTable.getStructure().getName() + '*/\n')
		F.write("create table "+iTableName+" (\n")
		AfterTableText = ""
		for ixMember in getChildElementsByTagName(iTable.getStructure().getObject(),XML_TAG_MEMBER):
			oAfterTableText = ""
			iMember = iTable.getStructure().getMemberHash()[ixMember.getAttribute(XML_ATTR_NAME)]
			oColumnText, oAfterTableText = getSqlType(iTable, iMember, Mode)
			F.write(" "+iMember.getName()+" "+oColumnText+",\n")
			if len(oAfterTableText) > 0:
				AfterTableText += oAfterTableText
		F.write(' primary key ('),
		tmpKomma = ''
		for iCol in getChildElementsByTagName(getChildElementsByTagName(iTable.getObject(),XML_TAG_PRIMKEY)[0],XML_TAG_COLUMN):
			F.write(tmpKomma+iCol.firstChild.data)
			tmpKomma = ','
		F.write('))\n')
		F.write(" ENGINE=InnoDB\n")
		F.write(" DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci\n")
		if iTable.getAttribute(XML_ATTR_DESCR): F.write(' COMMENT=\''+iTable.getAttribute(XML_ATTR_DESCR)+'\'\n')
		F.write(';\n')
		if len(AfterTableText) > 0:
			F.write('/* simulating auto increment columns for table */\n')
			F.write(AfterTableText+'\n')
		F.write("\n")
		
	# second pass: constraints, indices
	F.write("/*\n * constraints and indices\n */\n")
	for iTableName in dictTables.keys():
		iTable = dictTables[iTableName]
		iTableObj = iTable.getObject();
		i = 1
		for iMember in getChildElementsByTagName(iTableObj,XML_TAG_INDEX):
			F.write(getIndexString(iTable, iMember, Mode, str(i))+'\n')
			i+=1
		i = 1
		for iMember in getChildElementsByTagName(iTableObj,XML_TAG_FORKEY):
			F.write(getForKeyString(iTable, iMember, Mode, str(i))+'\n')
			i+=1
	F.write("\n")

	F.write("/* eof */")
	F.close()
