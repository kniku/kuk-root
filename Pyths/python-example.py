# -*- coding: latin-1 -*-
print('Umlaute auf Windows Console (siehe coding ganz oben!):','äöüÄÖÜß@')

#assingnement/strings
a = 10
a,b = 3,2
s = "hi" + " du";t = "hallo "*2+s[3:];
laenge = len(t);print('laenge von ['+t+']=',laenge)
print(a,b,s[:2]);
print(t)
print(str(a))
print(int("12"))
print("----- strings fertig -----")

#conditions
if a == 1 and b == 2:
	print("ok")
elif a == 2:
	print("sic")
else:
	print("not found")
print("----- cond fertig -----")

# Loops
while a > 0 :
	print(a)
	a -= 1
else:
	print("fertig (while)")

l = ["a","b","c"]
for i in l:
	print(i)
	if i == "b":
		print("break")
		break
	elif i == "a":
		print("continue")
		continue
print("fertig (for 1)")

for i in range(6,0,-2):
	print(i, "even")
else:
	print("fertig (for 2)")
print("----- loops fertig -----")
	
#lists
l = [1,2,3]
l.append("test")
l.insert(2,"test")
l.extend(["x","y","z"])
print(len(l), l[1], l[-2]) # index < 0 --> von hinten!
del l[0]
l.remove("test")
l.pop()
print(l.index("test"))
if "y" in(l): print("true")
else: "false"
print(l)
print("----- lists fertig -----")

#hash
h = {"x":"X","y":2,"3":5}
h["b"] = 10
h["a"] = {1:-1,2:-2,3:-3}
sk = list(h.keys());
sk.sort()
print(sk,h["3"],h["b"],h["a"])
if "a" in h:
	print(h["a"][3])
for k, v in h.items():
	print((k,v))
	
print("----- hash fertig -----")

# regular expressions
import re	# using regular expressions
print("hello world!\n")
pat = re.compile("(\s+)(\w+)",re.I|re.M|re.S)	# case insensitive, multiline, DOTMATCHNEWLINE
m = pat.search("hello world!")
print("["+m.group()+"]")
m = pat.search("test hello world!")
print("["+m.group(1)+"]")
print("["+m.group(2)+"]")
m = pat.findall("test hello world!")
print(m)
print("----- regexp fertig -----")

# file handling
import os
# Arbeitsverzeichnins
print('CWD='+os.getcwd())

if not os.path.isfile("xxx.xxx"):
	print("xxx.xxx not found\n")
files = [];
dirs = [];
for f in os.listdir('.'):
	if os.path.isdir(f) :
		dirs.append(f)
	else:
		files.append(f)
print("Directories:\n",dirs)
print("Files:\n",files)
# files mit wildcards lesen
try:
	for f in glob.glob('u*.sql'):
		print(f)
except:
	print('Keine Dateien gefunden')
finally:
	print('finally')
# single file handling
F = open(files[0],"rt")
# read whole file
content = F.read()
#remove whitespaces, eol ...
content = re.compile("\s+").sub("", content)
# seek to start of file
F.seek(0)
# read single line
aLine = F.readline()
# read all lines in a list
for aLine in F.readlines():
#	print aLine,	# das komma am ende verhindert zusaetzliche \n !
	pass # verhindert fehler wegen fehlendem loop-body
# close file
F.close()

# file-wildcards ueber die cmd-line
import glob
import sys
import fileinput
for arg in sys.argv[1:]:
	for file in glob.glob(arg):
		if os.path.isdir(file) : continue
		for line in fileinput.input(file):
#			print line,
			print(fileinput.filename())
			fileinput.nextfile()

print("----- files fertig -----")

# argv handling
if len(sys.argv) > 2:
	p1 = sys.argv[1]
	p2 = sys.argv[2]
	print(p1,p2)
else:
	print("Aufruf:",sys.argv[0],"src-pattern,dst-pattern\n")
	#sys.exit(0)
import getopt
args = "-a -b --help -cCCC -x -d DDD --option=test --option2 test2 file1 file2 ".split();
try:
	opts,args = getopt.getopt(args, "abc:xd:", ["help","option=","option2="])
	# oder : opts,args = getopt.getopt(sys.argv[1:], "abc:xd:", ["help","option=","option2="])
except getopt.GetoptError:
	print("usage ...")
else:
	opts = dict(opts)
	print(opts)
	print(opts["--option"])
	if 'a' in opts:
		pass
	print(args)
print("----- argv fertig -----")

# functions
def f1(a,b):
	print(a,b)
f1(1,"TEST")
def cheeseshop(kind, defarg=55, *arguments, **keywords):
	print("kind:",kind)
	print("defarg:",defarg)
	#variable arguments
	for arg in arguments: print(arg)
	#named arguments
	print('-'*40)
	keys = list(keywords.keys())
	keys.sort()
	for kw in keys: print(kw, ':', keywords[kw])
cheeseshop(1,2,3,x=10,y=20)
cheeseshop(x=10,y=20,z=2,kind=10)

print("----- func fertig -----")

#system call
os.system("echo ok")
print("----- system fertig -----")

# environment
if "OWIL" in os.environ:
	print(os.environ["OWIL"])

#current line number
import inspect
print(inspect.currentframe().f_lineno)

# date/time support
import time
print(time.asctime())

#read from stdin
#print sys.stdin.readline()

#terminantion
sys.exit(0)
