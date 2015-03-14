import sys,re,os

updateurl_base = "http://download.fedora.redhat.com/pub/fedora/linux/core/updates/5/i386/"
updateurl_indexfile = "index.html"
update_dir = "Updates"
new_update_dir = "Updates-New"

isCheck = 0

if len(sys.argv) > 1:
	if sys.argv[1] == "-getindex":
		if os.path.isfile(updateurl_indexfile):
			os.remove(updateurl_indexfile)
		os.system("wget "+updateurl_base)
		sys.exit(0)
	elif sys.argv[1] == "-check":
		mysearch = "."
		isCheck = 1
		print "checking files against",updateurl_indexfile,":"
	else:
		mysearch = sys.argv[1]	
else:
	print "== Fedora Update Loader == (kuk, 2005)"
	print "Aufruf: (python)",sys.argv[0],"[-getindex] | [-check] | [search-pattern]"
	print "-getindex ... hole neues Indexfile"
	print "-check ...... pruefe aktuelles Indexfile gegen existierende Files"
	print "z.B.:",sys.argv[0],". (alle zeigen)"
	print "     ",sys.argv[0],"\"^kde.*\" (alles was mit kde beginnt)"
	print "Einfach Ergebnis in file umleiten und alles mit \"wget -ifile\" runtersaugen..."
	print "\nKonfiguration:\nupdate-url:\n",updateurl_base
	print "FedoraUpdDir:",update_dir
	print "Index File:",updateurl_indexfile+",",
	if os.path.isfile(updateurl_indexfile): print "Ok"
	else: print "NICHT gefunden"
	if os.path.isfile("wget.exe"): print "wget: Ok"
	sys.exit(0)
	
if not os.path.isfile(updateurl_indexfile):
	print "!!!", updateurl_indexfile, "nicht gefunden!\n"
	sys.exit(0)

# reading indexfile
F = open(updateurl_indexfile,"rt")
content = F.read()
F.close()

# enter update directory
start_dir = os.getcwd()
os.chdir(update_dir)

#reading existing file-list
filehash = {}
for f in os.listdir('.'):
	if os.path.isfile(f) :
		filehash[f] = 1

pat = re.compile("<a href=\"(.*?rpm)\">")
m = pat.findall(content)

pat = re.compile(mysearch,re.I)
cntGes = cntNew = cntOld = cntObsolete = 0
lstNew=[]
for e in m:
	if pat.search(e):
		cntGes = cntGes + 1
		if not filehash.has_key(e):
			cntNew = cntNew + 1
			if not isCheck: print updateurl_base+e
			else: print "new:",e
			lstNew.append(e)
		else:
			cntOld = cntOld + 1
			filehash[e] = 0
			
if isCheck:
	#checking for obsolete files (not in index anymore)
	for e in filehash.keys():
		if filehash[e] == 1:
			cntObsolete = cntObsolete + 1
			print "obsolete:",e
	print "packages (indexfile):",cntGes,"\ndownloaded:",cntOld,"\nnew:",cntNew, "\nobsolete:",cntObsolete
	if cntObsolete > 0:
		print "delete the obsolete files (y/n):",
		ans = sys.stdin.readline()
		if ans[0] == "y":
			print "deleting:",
			for e in filehash.keys():
				if filehash[e] == 1:
					os.remove(e)
					if os.path.isfile("../"+new_update_dir+"/"+e):
						os.remove("../"+new_update_dir+"/"+e)
						print ":",
					else:
						print ".",

if len(lstNew) > 0:
	print "\ndownload new files (y/n):",
	ans = sys.stdin.readline()
	if ans[0] == "y":
		os.chdir(start_dir)
		os.chdir(new_update_dir)
		for e in lstNew:
			os.system("wget "+updateurl_base+e)
			os.system("touch ../"+update_dir+"/"+e) 