#!/bin/python

from xml.dom import minidom
from ParserConst import *

opts = 0

class CBase:
	def __init__(self, obj):
		self.data = {"~obj":obj}
	def getObject(self):
		return self.data["~obj"]
	def getParentObject(self):
		return self.data["~obj"].parentNode
	def getAttribute(self, iAttr):
		return self.data["~obj"].getAttribute(iAttr)
	def getName(self):
		return self.getAttribute(XML_ATTR_NAME)

class CType(CBase):
	def getType(self):
		return self.getAttribute(XML_ATTR_TYPE)
	def getLength(self):
		return self.getAttribute(XML_ATTR_LENGTH)
	def getScale(self):
		return self.getAttribute(XML_ATTR_SCALE)

class CMember(CBase):
	def __init__(self, obj, iType):
		self.data = {"~obj":obj, "~reftype":iType}
	def getType(self):
		return self.data["~reftype"]

class CStructure(CBase):
	def __init__(self, obj, iMemberHash):
		self.data = {"~obj":obj, "~members":iMemberHash}
	def getMemberHash(self):
		return self.data["~members"]
	def getMember(self, iMember):
		return self.data["~members"][iMember]

class CTable(CBase):
	def __init__(self, obj, iStructure):
		self.data = {"~obj":obj, "~refstruct":iStructure}
	def getStructure(self):
		return self.data["~refstruct"]
#	def getPrimKey(self):
#		return self.getAttribute(XML_ATTR_PRIMKEY)

class CGenerator(CBase):
	def getMode(self):
		return self.getAttribute(XML_ATTR_MODE)
	def getTarget(self):
		return self.getAttribute(XML_ATTR_TARGET)
	
def ParseXML2DBI(filename, programOptions):
	globals()['opts'] = programOptions
	dbiDict = {DICT_TYPES:{}, DICT_STRUCTS:{}, DICT_TABLES:{}, DICT_GENERATORS:{} }
	_ParseXML2DBI(filename,dbiDict)
	return dbiDict

def checkNODEAttr(iNode, iLevel, namePath):
	for i in range(0, iNode.attributes.length):
		check = 1
		if opts.has_key('-'+OPT_VERBOSE): print (iLevel+1)*' ',iNode.attributes.item(i).name+'=\''+iNode.attributes.item(i).nodeValue+'\''
		if iNode.nodeName == XML_ROOT_NODE_NAME:
			check = 0
		elif iNode.nodeName == XML_TAG_MODULE:
			if not HASH_VALID_ATTRS_OF_MODULE.has_key(iNode.attributes.item(i).name):
				check = 0
		elif iNode.nodeName == XML_TAG_IMPORT:
			if not HASH_VALID_ATTRS_OF_IMPORT.has_key(iNode.attributes.item(i).name):
				check = 0
		elif iNode.nodeName == XML_TAG_GENERATOR:
			if not HASH_VALID_ATTRS_OF_GENERATOR.has_key(iNode.attributes.item(i).name):
				check = 0
		elif iNode.nodeName == XML_TAG_TYPE:
			if not HASH_VALID_ATTRS_OF_TYPE.has_key(iNode.attributes.item(i).name):
				check = 0
		elif iNode.nodeName == XML_TAG_STRUCTURE:
			if not HASH_VALID_ATTRS_OF_STRUCTURE.has_key(iNode.attributes.item(i).name):
				check = 0
		elif iNode.nodeName == XML_TAG_MEMBER:
			if not HASH_VALID_ATTRS_OF_MEMBER.has_key(iNode.attributes.item(i).name):
				check = 0
		elif iNode.nodeName == XML_TAG_TABLE:
			if not HASH_VALID_ATTRS_OF_TABLE.has_key(iNode.attributes.item(i).name):
				check = 0
		elif iNode.nodeName == XML_TAG_SQL:
			check = 0
		elif iNode.nodeName == XML_TAG_FORKEY:
			if not HASH_VALID_ATTRS_OF_FORKEY.has_key(iNode.attributes.item(i).name):
				check = 0
		elif iNode.nodeName == XML_TAG_FORTABLE:
			if not HASH_VALID_ATTRS_OF_FORTABLE.has_key(iNode.attributes.item(i).name):
				check = 0
		elif iNode.nodeName == XML_TAG_INDEX:
			if not HASH_VALID_ATTRS_OF_INDEX.has_key(iNode.attributes.item(i).name):
				check = 0
		elif iNode.nodeName == XML_TAG_DEFAULT:
			check = 0
		else:
			print '! UNRECOGNIZED Element for attr-check found: \''+iNode.nodeName+'\' in',namePath
		if not check:
			print '! INVALID Attr found: \''+iNode.attributes.item(i).name+'\' in element',namePath
	
	# enum attr checks...
	if iNode.nodeName == XML_TAG_GENERATOR:
		if not HASH_VALID_VALUES_FOR_ATTR_NAME_OF_GENERATOR.has_key(iNode.getAttribute(XML_ATTR_NAME)):
			print '! invalid value \''+iNode.getAttribute(XML_ATTR_NAME)+'\' for attribute \''+XML_ATTR_NAME+'\' in',namePath
		if iNode.getAttribute(XML_ATTR_NAME) == XML_VALUE_GENERATOR_GENSQL:
			if not HASH_VALID_VALUES_FOR_ATTR_MODE_OF_GENSQL.has_key(iNode.getAttribute(XML_ATTR_MODE)):
				print '! invalid value \''+iNode.getAttribute(XML_ATTR_MODE)+'\' for attribute \''+XML_ATTR_MODE+'\' in',namePath,'of type',XML_VALUE_GENERATOR_GENSQL
		if iNode.getAttribute(XML_ATTR_NAME) == XML_VALUE_GENERATOR_GENCDECL:
			if not HASH_VALID_VALUES_FOR_ATTR_MODE_OF_GENCDECL.has_key(iNode.getAttribute(XML_ATTR_MODE)):
				print '! invalid value \''+iNode.getAttribute(XML_ATTR_MODE)+'\' for attribute \''+XML_ATTR_MODE+'\' in',namePath,'of type',XML_VALUE_GENERATOR_GENCDECL
	if iNode.nodeName == XML_TAG_TYPE:
		if not HASH_VALID_VALUES_FOR_ATTR_TYPE_OF_TYPE.has_key(iNode.getAttribute(XML_ATTR_TYPE)):
			print '! invalid value \''+iNode.getAttribute(XML_ATTR_TYPE)+'\' for attribute \''+XML_ATTR_TYPE+'\' in',namePath
	if iNode.nodeName == XML_TAG_MEMBER:
		if not HASH_VALID_VALUES_FOR_ATTR_NULL_OF_MEMBER.has_key(iNode.getAttribute(XML_ATTR_NULL)):
			print '! invalid value \''+iNode.getAttribute(XML_ATTR_NULL)+'\' for attribute \''+XML_ATTR_NULL+'\' in',namePath
	if iNode.nodeName == XML_TAG_INDEX:
		if not HASH_VALID_VALUES_FOR_ATTR_UNIQUE_OF_INDEX.has_key(iNode.getAttribute(XML_ATTR_UNIQUE)):
			print '! invalid value \''+iNode.getAttribute(XML_ATTR_UNIQUE)+'\' for attribute \''+XML_ATTR_UNIQUE+'\' in',namePath
	# reqired attr check
	if iNode.nodeName != XML_ROOT_NODE_NAME and iNode.nodeName != XML_TAG_DEFAULT and iNode.nodeName != XML_TAG_SQL and iNode.nodeName != XML_TAG_MEMBER and iNode.nodeName != XML_TAG_PRIMKEY and iNode.nodeName != XML_TAG_COLUMN and iNode.nodeName != XML_TAG_INDEX and iNode.nodeName != XML_TAG_FORKEY and iNode.nodeName != XML_TAG_IMPORT:
		if not iNode.getAttribute(XML_ATTR_NAME):
			print '! node must have a name'
	
def checkNODE(iNode, iLevel, namePath):
	if opts.has_key('-'+OPT_VERBOSE): print iLevel*' ','<',iNode.nodeName
	for iChild in iNode.childNodes:
		if iChild.nodeType == iNode.ELEMENT_NODE:
			check = 1
			if iNode.nodeName == XML_ROOT_NODE_NAME:
				if not HASH_VALID_ELEMENTS_IN_ROOT.has_key(iChild.nodeName):
					check = 0
			elif iNode.nodeName == XML_TAG_MODULE:
				if not HASH_VALID_ELEMENTS_IN_MODULE.has_key(iChild.nodeName):
					check = 0
			elif iNode.nodeName == XML_TAG_IMPORT:
				check = 0
			elif iNode.nodeName == XML_TAG_GENERATOR:
				check = 0
			elif iNode.nodeName == XML_TAG_TYPE:
				if not HASH_VALID_ELEMENTS_IN_TYPE.has_key(iChild.nodeName):
					check = 0
			elif iNode.nodeName == XML_TAG_STRUCTURE:
				if not HASH_VALID_ELEMENTS_IN_STRUCTURE.has_key(iChild.nodeName):
					check = 0
			elif iNode.nodeName == XML_TAG_MEMBER:
				if not HASH_VALID_ELEMENTS_IN_MEMBER.has_key(iChild.nodeName):
					check = 0
			elif iNode.nodeName == XML_TAG_TABLE:
				if not HASH_VALID_ELEMENTS_IN_TABLE.has_key(iChild.nodeName):
					check = 0
			elif iNode.nodeName == XML_TAG_PRIMKEY:
				if not HASH_VALID_ELEMENTS_IN_PRIMKEY.has_key(iChild.nodeName):
					check = 0
			elif iNode.nodeName == XML_TAG_FORKEY:
				if not HASH_VALID_ELEMENTS_IN_FORKEY.has_key(iChild.nodeName):
					check = 0
			elif iNode.nodeName == XML_TAG_FORTABLE:
				if not HASH_VALID_ELEMENTS_IN_FORTABLE.has_key(iChild.nodeName):
					check = 0
			elif iNode.nodeName == XML_TAG_INDEX:
				if not HASH_VALID_ELEMENTS_IN_INDEX.has_key(iChild.nodeName):
					check = 0
			elif iNode.nodeName == XML_TAG_COLUMN:
				if not HASH_VALID_ELEMENTS_IN_COLUMN.has_key(iChild.nodeName):
					check = 0
			elif iNode.nodeName == XML_TAG_SQL:
				if not HASH_VALID_ELEMENTS_IN_SQL.has_key(iChild.nodeName):
					check = 0
			else:
				print '! UNRECOGNIZED Element found: \''+iChild.nodeName+'\' in',namePath
			if not check:
				print '! INVALID Element found: \''+iChild.nodeName+'\' in',namePath
				
			checkNODE(iChild, iLevel+1, namePath+'.'+iChild.nodeName)
	# non empty child element list
	if iNode.nodeName == XML_TAG_STRUCTURE:
		if len(getChildElementsByTagName(iNode,XML_TAG_MEMBER)) == 0:
			print '! Structure must have at least ONE member'
	if iNode.nodeName == XML_TAG_TABLE:
		if len(getChildElementsByTagName(iNode,XML_TAG_PRIMKEY)) != 1:
			print '! table must have a primary key'
	if iNode.nodeName == XML_TAG_PRIMKEY:
		if len(getChildElementsByTagName(iNode,XML_TAG_COLUMN)) == 0:
			print '! primary key must have at least ONE column'
	if iNode.nodeName == XML_TAG_FORKEY:
		if len(getChildElementsByTagName(iNode,XML_TAG_FORTABLE)) != 1:
			print '! foreign key must have a table to refer (ForTable)'
		elif len(getChildElementsByTagName(getChildElementsByTagName(iNode,XML_TAG_FORTABLE)[0],XML_TAG_COLUMN)) == 0:
			print '! foreign key must have at least ONE foreign column'
		elif len(getChildElementsByTagName(iNode,XML_TAG_COLUMN)) == 0:
			print '! foreign key must have at least ONE column'
	if iNode.nodeName == XML_TAG_INDEX:
		if len(getChildElementsByTagName(iNode,XML_TAG_COLUMN)) == 0:
			print '! index must have at least ONE column'
			
	checkNODEAttr(iNode, iLevel, namePath)
	if opts.has_key('-'+OPT_VERBOSE): print (iLevel+1)*' '+'>'
	
def _ParseXML2DBI(filename, dbiDict):
	print '{',filename,'}'
	typeHash = dbiDict[DICT_TYPES]; structHash = dbiDict[DICT_STRUCTS]; tableHash = dbiDict[DICT_TABLES]; genHash = dbiDict[DICT_GENERATORS];
	xmlDesc = minidom.parse(filename)
	for iNode in xmlDesc.childNodes:
		if iNode.nodeType == iNode.ELEMENT_NODE and iNode.nodeName == XML_ROOT_NODE_NAME:
			if not opts.has_key('-'+OPT_NOCHECK): checkNODE(iNode, 1, XML_ROOT_NODE_NAME)
			
			if len(getChildElementsByTagName(iNode, XML_TAG_MODULE)) > 0:
				if len(getChildElementsByTagName(iNode, XML_TAG_MODULE)) > 1:
					raise SyntaxError("more than one module specified!")
				iModule = getChildElementsByTagName(iNode, XML_TAG_MODULE)[0]
				for iImport in getChildElementsByTagName(iModule, XML_TAG_IMPORT):
					if not iImport.getAttribute(XML_ATTR_FILE):
						raise SyntaxError("import declaration without a file")
					_ParseXML2DBI(iImport.getAttribute(XML_ATTR_FILE), dbiDict)

				for iGen in getChildElementsByTagName(iModule, XML_TAG_GENERATOR):
					if not iGen.getAttribute(XML_ATTR_TARGET):
						raise SyntaxError("generator "+iGen.getAttribute(XML_ATTR_NAME)+" has no target")
					genHash[iGen.getAttribute(XML_ATTR_TARGET)] = CGenerator(iGen)
#			else:
#				raise SyntaxError("no (or more than one) module specified!")
				
			typeList = getChildElementsByTagName(iNode,XML_TAG_TYPE)
			for iType in typeList:
				if typeHash.has_key(iType.getAttribute(XML_ATTR_NAME)):
					raise SyntaxError("duplicate type found: "+iType.getAttribute(XML_ATTR_NAME))
				typeHash[iType.getAttribute(XML_ATTR_NAME)] = CType(iType)
					
			structList = getChildElementsByTagName(iNode,XML_TAG_STRUCTURE)
			for iStruct in structList:
				if structHash.has_key(iStruct.getAttribute(XML_ATTR_NAME)):
					raise SyntaxError("duplicate structure found: "+iStruct.getAttribute(XML_ATTR_NAME))
				iMemberHash = {}
				for iMember in getChildElementsByTagName(iStruct,XML_TAG_MEMBER):
					if iMemberHash.has_key(iMember.getAttribute(XML_ATTR_NAME)):
						raise SyntaxError("Structure: "+iStruct.getAttribute(XML_ATTR_NAME)+": duplicate member found: "+iMember.getAttribute(XML_ATTR_NAME))
					if not iMember.getAttribute(XML_ATTR_REFTYPE):
						raise SyntaxError("Structure "+iStruct.getAttribute(XML_ATTR_NAME)+": member must have a type reference")
					elif not typeHash.has_key(iMember.getAttribute(XML_ATTR_REFTYPE)):
						raise SyntaxError("Structure "+iStruct.getAttribute(XML_ATTR_NAME)+": invalid type reference: "+iMember.getAttribute(XML_ATTR_REFTYPE))
					iMemberHash[iMember.getAttribute(XML_ATTR_NAME)] = CMember(iMember, typeHash[iMember.getAttribute(XML_ATTR_REFTYPE)])
				structHash[iStruct.getAttribute(XML_ATTR_NAME)] = CStructure(iStruct, iMemberHash)
					
			tableList = getChildElementsByTagName(iNode,XML_TAG_TABLE)
			for iTable in tableList:
				if tableHash.has_key(iTable.getAttribute(XML_ATTR_NAME)):
					raise SyntaxError("duplicate table found: "+iTable.getAttribute(XML_ATTR_NAME))
				if not iTable.getAttribute(XML_ATTR_REFTYPE):
					raise SyntaxError("table "+iTable.getAttribute(XML_ATTR_NAME)+": table has no structure reference")
				elif not structHash.has_key(iTable.getAttribute(XML_ATTR_REFTYPE)):
					raise SyntaxError("table "+iTable.getAttribute(XML_ATTR_NAME)+": invalid struct reference: "+iTable.getAttribute(XML_ATTR_REFTYPE))
				tableHash[iTable.getAttribute(XML_ATTR_NAME)] = CTable(iTable, structHash[iTable.getAttribute(XML_ATTR_REFTYPE)])
	
		else:
			raise SyntaxError("invalid node: "+iNode.nodeName)
			pass

