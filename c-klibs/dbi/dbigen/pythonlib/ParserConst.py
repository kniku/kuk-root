# program options
OPT_HELP = 'h'
OPT_VERBOSE = 'v'
OPT_NOCHECK = 'N'
OPT_BUILDFILE = 'f'

# global dictionary names
DICT_TYPES = "~TY"
DICT_STRUCTS = "~ST"
DICT_TABLES = "~TA"
DICT_GENERATORS = "~GE"

# ==========================================
# XML syntax token/attr/...
# ==========================================

XML_ROOT_NODE_NAME = 'DBI_DATA_DESCRIPTION'

# valid xml tag names

XML_TAG_MODULE = 'Module'	# module specification
XML_TAG_IMPORT = 'Import'	# import of other xml files; within TAG_MODULE
XML_TAG_GENERATOR = 'Generator'	# generator definitions; within TAG_MODULE
XML_TAG_TYPE = 'Type'	# base types
XML_TAG_STRUCTURE = 'Structure'	# structures
XML_TAG_MEMBER = 'Member'	# struct member; within TAG_STRUCTURE
XML_TAG_TABLE = 'Table'	# tables
XML_TAG_PRIMKEY = 'PrimKey'	# primary key; within TAG_TABLE
XML_TAG_COLUMN = 'Column'	# column name; within TAG_PRIMKEY
XML_TAG_FORKEY = 'ForKey'	# foreign key; within TAG_TABLE
XML_TAG_FORTABLE = 'ForTable'	# foreign key; within TAG_FORKEY
XML_TAG_INDEX = 'Index'	# table index; within TAG_TABLE
XML_TAG_SQL = 'Sql'	# sql stuff; within TAG_TYPE, TAG_MEMBER
XML_TAG_DEFAULT = 'Default'	# sql default value; within TAG_SQL

# valid attributes

XML_ATTR_TYPE = 'Type'	# within TAG_TYPE
XML_ATTR_FILE = 'File'	# within TAG_IMPORT
XML_ATTR_NAME = 'Name'	# within TAG_GENERATOR, TAG_TYPE, TAG_STRUCTURE, TAG_MEMBER, TAG_TABLE, TAG_FORKEY, TAG_INDEX
XML_ATTR_TARGET = 'Target'	# build target; within TAG_GENERATOR
XML_ATTR_REFTYPE = 'RefType'	# within TAG_MEMBER, TAG_TABLE
XML_ATTR_LENGTH = 'Length'	# within TAG_TYPE
XML_ATTR_SCALE = 'Scale'	# within TAG_TYPE
XML_ATTR_MODE = 'Mode'	# within TAG_GENERATOR
XML_ATTR_NULL = 'Null'	# within TAG_MEMBER
XML_ATTR_DESCR = 'Descr'	# within TAG_TYPE, TAG_MEMBER, TAG_STRUCTURE, TAG_TABLE
XML_ATTR_UNIQUE = 'Unique'	# within TAG_INDEX

# special values

XML_VALUE_TYPE_STRING = 'string'	# type attr:string; for TAG_TYPE:ATTR_TYPE
XML_VALUE_TYPE_DATE = 'date'	# type attr:date; for TAG_TYPE:ATTR_TYPE
XML_VALUE_TYPE_NUMERIC = 'numeric'	# type attr:numeric; for TAG_TYPE:ATTR_TYPE
XML_VALUE_TYPE_AUTO = 'auto'	# type attr:autoincrement; for TAG_TYPE:ATTR_TYPE
XML_VALUE_SQLDEFAULT_DEFAULT = '#default#'	# type default value; within TAG_SQLDEFAULT (Type/Member)
XML_VALUE_SQLDEFAULT_NODEFAULT = '#nodefault#'	# no default please (Member!); within TAG_SQLDEFAULT of Member
XML_VALUE_GENERATOR_GENSQL = 'GenSql'	# sql stuff generator
XML_VALUE_GENERATOR_GENCDECL = 'GenCDecl'	# c stuff generator
XML_VALUE_MODE_MYSQL5 =	"mysql5"
XML_VALUE_MODE_ORACLE9 =	"oracle9"
XML_VALUE_TRUE =	"true"
XML_VALUE_FALSE =	"false"

# ==========================================
# helper functions
# ==========================================

# element check

HASH_VALID_ELEMENTS_IN_ROOT = {
	XML_TAG_MODULE : 1,
	XML_TAG_TYPE : 1,
	XML_TAG_STRUCTURE : 1,
	XML_TAG_TABLE : 1
}

HASH_VALID_ELEMENTS_IN_MODULE = {
	XML_TAG_IMPORT : 1,
	XML_TAG_GENERATOR : 1
}

HASH_VALID_ELEMENTS_IN_TYPE = {
	XML_TAG_SQL : 1
}

HASH_VALID_ELEMENTS_IN_STRUCTURE = {
	XML_TAG_MEMBER : 1
}

HASH_VALID_ELEMENTS_IN_MEMBER = {
	XML_TAG_SQL : 1
}

HASH_VALID_ELEMENTS_IN_TABLE = {
	XML_TAG_FORKEY : 1,
	XML_TAG_PRIMKEY : 1,
	XML_TAG_INDEX : 1
}

HASH_VALID_ELEMENTS_IN_PRIMKEY = {
	XML_TAG_COLUMN : 1
}

HASH_VALID_ELEMENTS_IN_FORKEY = {
	XML_TAG_COLUMN : 1,
	XML_TAG_FORTABLE : 1
}

HASH_VALID_ELEMENTS_IN_FORTABLE = {
	XML_TAG_COLUMN : 1
}

HASH_VALID_ELEMENTS_IN_INDEX = {
	XML_TAG_COLUMN : 1
}

HASH_VALID_ELEMENTS_IN_SQL = {
	XML_TAG_DEFAULT : 1
}

# Attr check

HASH_VALID_ATTRS_OF_MODULE = {
	XML_ATTR_NAME : 1
}

HASH_VALID_ATTRS_OF_IMPORT = {
	XML_ATTR_FILE : 1
}

HASH_VALID_ATTRS_OF_GENERATOR = {
	XML_ATTR_MODE : 1,
	XML_ATTR_TARGET : 1,
	XML_ATTR_NAME : 1
}

HASH_VALID_ATTRS_OF_TYPE = {
	XML_ATTR_TYPE : 1,
	XML_ATTR_DESCR : 1,
	XML_ATTR_LENGTH : 1,
	XML_ATTR_SCALE : 1,
	XML_ATTR_NAME : 1
}

HASH_VALID_ATTRS_OF_STRUCTURE = {
	XML_ATTR_DESCR : 1,
	XML_ATTR_NAME : 1
}

HASH_VALID_ATTRS_OF_MEMBER = {
	XML_ATTR_DESCR : 1,
	XML_ATTR_REFTYPE : 1,
	XML_ATTR_NULL : 1,
	XML_ATTR_NAME : 1
}

HASH_VALID_ATTRS_OF_TABLE = {
	XML_ATTR_DESCR : 1,
	XML_ATTR_REFTYPE : 1,
	XML_ATTR_NAME : 1
}

HASH_VALID_ATTRS_OF_FORKEY = {
	XML_ATTR_NAME : 1
}

HASH_VALID_ATTRS_OF_FORTABLE = {
	XML_ATTR_NAME : 1
}

HASH_VALID_ATTRS_OF_INDEX = {
	XML_ATTR_UNIQUE : 1,
	XML_ATTR_NAME : 1
}

HASH_VALID_VALUES_FOR_ATTR_NAME_OF_GENERATOR = {
	XML_VALUE_GENERATOR_GENCDECL : 1,
	XML_VALUE_GENERATOR_GENSQL : 1
}

HASH_VALID_VALUES_FOR_ATTR_MODE_OF_GENSQL = {
	XML_VALUE_MODE_ORACLE9 : 1,
	XML_VALUE_MODE_MYSQL5 : 1
}

HASH_VALID_VALUES_FOR_ATTR_MODE_OF_GENCDECL = {
	'' : 1,
#	XML_VALUE_MODE_HEADER : 1,
#	XML_VALUE_MODE_SOURCE : 1
}

VALUE_TYPE_STRING, VALUE_TYPE_NUMERIC, VALUE_TYPE_DATE, VALUE_TYPE_AUTO = range(1, 5)

HASH_VALID_VALUES_FOR_ATTR_TYPE_OF_TYPE = {
	XML_VALUE_TYPE_STRING : VALUE_TYPE_STRING,
	XML_VALUE_TYPE_NUMERIC : VALUE_TYPE_NUMERIC,
	XML_VALUE_TYPE_DATE : VALUE_TYPE_DATE,
	XML_VALUE_TYPE_AUTO : VALUE_TYPE_AUTO
}

HASH_VALID_VALUES_FOR_ATTR_NULL_OF_MEMBER = {
	'' : 1,
	XML_VALUE_FALSE : 1,
	XML_VALUE_TRUE : 1
}

HASH_VALID_VALUES_FOR_ATTR_UNIQUE_OF_INDEX = HASH_VALID_VALUES_FOR_ATTR_NULL_OF_MEMBER


# other ...

def mapXMLType(iXMLType):
	return HASH_VALID_VALUES_FOR_ATTR_TYPE_OF_TYPE[iXMLType]
	
def isBoolTrue(iString):
#	if iString.upper() == 'TRUE' or iString == '1':
	if iString == 'true':
		r = 1
	else:
		r = 0
	return r

def getChildElementsByTagName(iNode, iTagname):
	r = []
	for i in iNode.getElementsByTagName(iTagname):
		if i.parentNode == iNode: r.append(i)
	return r

# eof