#!/usr/bin/python

from pythonlib import Parser
from pythonlib.ParserConst import *
import sys,inspect,os
import getopt

opts,args = getopt.getopt(sys.argv[1:], OPT_HELP+OPT_VERBOSE+OPT_NOCHECK+OPT_BUILDFILE+':', [])
opts = dict(opts)
if opts.has_key('-'+OPT_HELP):
	print 'usage',sys.argv[0],'[-'+ OPT_HELP+OPT_VERBOSE+OPT_NOCHECK+'] [-'+OPT_BUILDFILE+' buildfile]'
	print '-'+OPT_HELP+' ... this help'
	print '-'+OPT_VERBOSE+' ... verbose mode'
	print '-'+OPT_NOCHECK+' ... NO syntax check (speedup parsing)'
	print '-'+OPT_BUILDFILE+' ... use file given as input instead of \'build.xml\''
	sys.exit(-1)

fileName = 'build.xml'
if opts.has_key('-'+OPT_BUILDFILE):
	fileName = opts['-'+OPT_BUILDFILE]
print '\n# parsing input'
dictDbi = Parser.ParseXML2DBI(fileName, opts)
print '# done.'

def generate(dictDbi):
	dictGen = dictDbi[Parser.DICT_GENERATORS]
	if len(dictGen.keys()) == 0:
		raise SyntaxError("! no module/generator found!")
	for iType in dictGen.keys():
		print '# generator:', dictGen[iType].getAttribute(XML_ATTR_NAME), '->', iType
		genPath = os.path.dirname(dictGen[iType].getAttribute(XML_ATTR_TARGET))
		if genPath and not os.path.exists(genPath):
			os.makedirs(genPath)
		if dictGen[iType].getName() == XML_VALUE_GENERATOR_GENSQL:
			from pythonlib.dbd import GenSql
			GenSql.generate(dictDbi, dictGen[iType])
		elif dictGen[iType].getName() == XML_VALUE_GENERATOR_GENCDECL:
			from pythonlib.dbd import GenCDecl
			GenCDecl.generate(dictDbi, dictGen[iType])
			
generate(dictDbi)
