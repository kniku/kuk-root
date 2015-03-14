from pythonlib import Parser
from pythonlib.ParserConst import *
import random
import os.path

def generate(dictDbi, iGenerator):
	genHeader(dictDbi, iGenerator)
	genSource(dictDbi, iGenerator)

def genSource(dictDbi, iGenerator):
	Target = iGenerator.getTarget()
	headerFileName = os.path.basename(Target)
	F = open(Target + '.cpp', "wt")
	F.write("/*\n * DBD-GenCDecl auto generated source file\n */\n")
	F.write('#define DBI_GENERATED_CODE\n#include <klibdbi.h>\n')
	F.write('#include \"'+ headerFileName +'.h\"\n')
	F.write('namespace DbiGen_' + iGenerator.getParentObject().getAttribute(XML_ATTR_NAME) + ' {\n\n')

	dict = dictDbi[Parser.DICT_STRUCTS]
	for iName in dict.keys():
		iStruct = dict[iName]
		F.write('// ' + iStruct.getAttribute(XML_ATTR_DESCR) + '\nconst klib::dbi::BindDesc BD_' + iName + '[] = {')
		cntMember = 0
		_str = ''
		for ixMember in getChildElementsByTagName(iStruct.getObject(), XML_TAG_MEMBER):
			if cntMember == 0: _str += '\n {'
			else: _str += ',\n {'
			iMember = iStruct.getMemberHash()[ixMember.getAttribute(XML_ATTR_NAME)]
			iType = iMember.getType()
			baseType = mapXMLType(iType.getType())
			if baseType == VALUE_TYPE_STRING:
				_str += ('klib::dbi::DataType_String, \"' + iMember.getName()+ '\", MAX_LEN_' + iType.getName() + '+1')
			elif baseType == VALUE_TYPE_AUTO:
				_str += ('klib::dbi::DataType_Long, \"' + iMember.getName()+ '\", sizeof(long)')
			elif baseType == VALUE_TYPE_NUMERIC:
				if int(iType.getAttribute(XML_ATTR_LENGTH)) < 10 and int(iType.getAttribute(XML_ATTR_SCALE)) == 0:
					_str += ('klib::dbi::DataType_Long, \"' + iMember.getName()+ '\", sizeof(long)')
				else :
					_str += ('klib::dbi::DataType_Double, \"' + iMember.getName()+ '\", sizeof(double)')
			elif baseType == VALUE_TYPE_DATE:
				_str += ('klib::dbi::DataType_DateTime, \"' + iMember.getName()+ '\", sizeof(klib::dbi::DateTime)')
			else:
				raise SyntaxError("unsupported type: " + iType.getType())
			_str += ', offsetof(D_' + iName + ', '+ iMember.getName() + ')'
			_str += '}'	
			cntMember += 1
		F.write(_str + ',\n {klib::dbi::DataType_None}\n};\n')
		F.write('klib::dbi::BindDescPtr '+iName+'::getBindDesc(const unsigned int iIndex) const {\n if (iIndex < cntColumns) return &BD_' + iName + '[iIndex];\n else return 0;\n}\n')
		F.write('void *'+iName+'::getStdData(void) const {\n return (void *)&Data;\n}\n')
		F.write('struct D_'+iName+' *'+iName+'::getData(void) const {\n return (struct D_'+iName+'*)&Data;\n}\n')
		F.write('void ' + iName + '::reset(void) {\n memset(&Data, 0, sizeof(Data));\n}\n\n')

	dict = dictDbi[Parser.DICT_TABLES]
	for iName in dict.keys():
		iTable = dict[iName]
		F.write('klib::dbi::BindDescPtr PK_' + iName + '[] = {')
		cntMember = 0
		for ixMember in getChildElementsByTagName(getChildElementsByTagName(iTable.getObject(), XML_TAG_PRIMKEY)[0], XML_TAG_COLUMN):
			iixIndex = 0
			for iixMember in getChildElementsByTagName(iTable.getStructure().getObject(), XML_TAG_MEMBER):
				if iixMember.getAttribute(XML_ATTR_NAME) == ixMember.firstChild.data: break
				iixIndex += 1
			if cntMember > 0: F.write(',')
			F.write('&BD_' + iTable.getStructure().getName() + '[' + str(iixIndex) + ']')
			cntMember += 1
		F.write(',0};\n')
	F.write('const klib::dbi::TableDesc TD[] = {\n')
	F.write('// tablename, primary key binddesc, binddesc, length primary key, length binddesc\n')
	for iName in dict.keys():
		iTable = dict[iName]
		F.write('{"' + iName + '\"')
		F.write(', PK_' + iName)
		F.write(', BD_' + iTable.getStructure().getName())
		F.write(', ' + str(len(getChildElementsByTagName(getChildElementsByTagName(iTable.getObject(), XML_TAG_PRIMKEY)[0], XML_TAG_COLUMN))))
		F.write(', ' + str(len(getChildElementsByTagName(iTable.getStructure().getObject(), XML_TAG_MEMBER))) + '},\n')
	F.write('{0}};\n')

	F.write('\nvoid RegisterTablesOf_' + iGenerator.getParentObject().getAttribute(XML_ATTR_NAME) + '(void) {\n')
	F.write(' klib::dbi::RegisterTables(TD);\n}\n\n')

	
	F.write('\n} // namespace DbiGen\n')
	F.write("// eof\n")
	F.close()

def genHeader(dictDbi, iGenerator):
	Target = iGenerator.getTarget()
	F = open(Target + '.h', "wt")
	random.seed()
	uniqueId = random.randint(1,999999999)
	uniqueId = '_DBIGEN_'+str(uniqueId)+'_H'
	F.write('#ifndef '+uniqueId+'\n#define '+uniqueId+'\n')
	F.write("/*\n * DBD-GenCDecl auto generated header file\n */\n\n")
	F.write('namespace DbiGen_' + iGenerator.getParentObject().getAttribute(XML_ATTR_NAME) + ' {\n\n')

	F.write('// maximum length of string types\n')
	dict = dictDbi[Parser.DICT_TYPES]
	for iName in dict.keys():
		iType = dict[iName]
		if iType.getType() == XML_VALUE_TYPE_STRING:
			F.write('#define MAX_LEN_' + iName + '\t\t' + iType.getAttribute(XML_ATTR_LENGTH) + '\n')
			
	F.write('\n')
	dict = dictDbi[Parser.DICT_STRUCTS]
	for iName in dict.keys():
		iStruct = dict[iName]
		F.write('// '+iStruct.getAttribute(XML_ATTR_DESCR) + "\nstruct D_"+iName+" {")
		cntMembers = 0
		for ixMember in getChildElementsByTagName(iStruct.getObject(),XML_TAG_MEMBER):
			cntMembers += 1
			iMember = iStruct.getMemberHash()[ixMember.getAttribute(XML_ATTR_NAME)]
			iType = iMember.getType()
			baseType = mapXMLType(iType.getType())
			if baseType == VALUE_TYPE_STRING:
				baseType = 'char ' + iMember.getName() + '[MAX_LEN_' + iType.getName() + '+1]'
			elif baseType == VALUE_TYPE_NUMERIC:
				if int(iType.getAttribute(XML_ATTR_LENGTH)) < 10 and int(iType.getAttribute(XML_ATTR_SCALE)) == 0:
					baseType = 'long ' + iMember.getName()
				else:
					baseType = 'double ' + iMember.getName()
			elif baseType == VALUE_TYPE_DATE:
				baseType = 'klib::dbi::DateTime ' + iMember.getName()
			elif baseType == VALUE_TYPE_AUTO:
				baseType = 'long ' + iMember.getName()
			else:
				raise SyntaxError("unsupported type: " + iType.getType())
			F.write('\n\t' + baseType + ';')
			if iMember.getAttribute(XML_ATTR_DESCR): F.write("\t// " + iMember.getAttribute(XML_ATTR_DESCR))
		F.write('\n};\nclass '+iName+' : public klib::dbi::CStdRecord {\nprivate:\n\tD_'+iName+' Data;\npublic:')
		F.write('\n\tstatic const unsigned int cntColumns = ' + str(cntMembers) + ';')
		F.write('\n\tvirtual klib::dbi::BindDescPtr getBindDesc(const unsigned int iIndex) const;')
		F.write('\n\tvirtual void *getStdData(void) const;')
		F.write('\n\tstruct D_'+iName+' *getData(void) const;')
		F.write('\n\tvirtual void reset(void);\n};\n\n')

#	F.write('extern Dbi::TableDescPtr TD_' + iGenerator.getParentObject().getAttribute(XML_ATTR_NAME) + ';\n\n')
	F.write('void RegisterTablesOf_' + iGenerator.getParentObject().getAttribute(XML_ATTR_NAME) + '(void);\n\n')
	F.write('} // namespace DbiGen\n')
	F.write('#endif // '+uniqueId+'\n')
	F.write("// eof\n")
	F.close()
