# -*- coding: iso-8859-1 -*-
import sys, glob, os
from datetime import datetime,timedelta
from time import strptime
import getopt

EXCEL_WORKBOOKDIRECTORY = os.getcwd()
#u"C:\\kuk\\kukHttp\\htdocs\\ecoserv\\"

# defaults
mode = 'html'
filter = '2007'
ck3path = '.'
summary_numcols = 6
mode_summary = 0
mode_detail = 0

def show_usage():
	print '\nAufruf: ck32file --ck3path=CK3PATH [--details] [--summary] [--summary_cols] [--output=MODE] --filter=DATEPATTERN'
	print 'ck3path ... Pfad zu den Datenfiles'
	print '            normalerweise \\\\nt02\\vol_i\\ecoserv\\ck3; ask kuk)'
	print 'output .... Gewuenschter Output (html,csv)'
	print 'summary ... Zusammenfassung erwuenscht?'
	print 'summary_cols ... Anzahl Columns in Summary'
	print 'details ... Details erwuenscht?'
	print 'pattern ... Datumsangabe in der Form YYYYMMDD'
	sys.exit(0)
	return

try:
	opts,args = getopt.getopt(sys.argv[1:],'',['summary_cols=','summary','details','filter=','ck3path=','output='])
except getopt.GetoptError:
	show_usage()
else:
	opts = dict(opts)
	if opts.has_key('--ck3path'):
		ck3path = opts['--ck3path']
	if not os.path.isfile(ck3path+'/pze.dat'):
		print '\n### Ungueltiger CK3 Pfad! ('+ck3path+')'
		show_usage()
	if opts.has_key('--output'):
		mode = opts['--output']
	if opts.has_key('--filter'):
		filter = opts['--filter']
	if opts.has_key('--details'):
		mode_detail = 1
	if opts.has_key('--summary'):
		mode_summary = 1
	if opts.has_key('--summary_cols'):
		summary_numcols = int(opts['--summary_cols'])
		
def write_csv():
	import win32com.client
	
	# Verbindung zu Excel aufnehmen
	app = win32com.client.dynamic.Dispatch("Excel.Application")

	# Neue Arbeitsmappe
	workbook = app.Workbooks.Add()

	# Neues Arbeitsblatt
	worksheet = workbook.Worksheets.Add()
	worksheet.Name = "Details"
	worksheet.Cells(1, 1).Value = 'Details (Sortierung: Datum)'
	worksheet.Cells(1, 1).Font.Bold = True
	worksheet.Cells(1, 1).Font.Size = 20
	worksheet.Cells(2, 1).Value = 'Auswertung mit Filter = \"'+filter+'\"'
	worksheet.Cells(3, 1).Value = 'Auswertungszeitpunkt = '+str(datetime.today())

	rowindex = 5
	for k1 in hglobal_keys:
		v1 = hglobal[k1]
		worksheet.Cells(rowindex, 1).Value = str(k1)
		worksheet.Cells(rowindex, 1).Font.Bold = True
		worksheet.Cells(rowindex, 1).Font.Size = 12
		worksheet.Cells(rowindex, 3).Value = 'First IN'
		worksheet.Cells(rowindex, 3).Font.Bold = True
		worksheet.Cells(rowindex, 4).Value = 'Last OUT'
		worksheet.Cells(rowindex, 4).Font.Bold = True
		worksheet.Cells(rowindex, 5).Value = 'Total IN'
		worksheet.Cells(rowindex, 5).Font.Bold = True
		rowindex += 1
		for k, v in v1['hash'].iteritems():
#			print 'A'+str(rowindex)
#			cells = worksheet.Range('A'+str(rowindex), worksheet.Cells(1, 5))
#			cells.Value = (v['name'], v['vname'], 'XXX', 'YYY', str(v['tin']))
			
			if 1:
				worksheet.Cells(rowindex, 1).Value = v['name']
				worksheet.Cells(rowindex, 2).Value = v['vname']
				if v.has_key('first_in'):
					worksheet.Cells(rowindex, 3).Value = str(v['first_in'].time())
				else:
					worksheet.Cells(rowindex, 3).Value = '0:00:00'
				if v.has_key('last_out'):
					worksheet.Cells(rowindex, 4).Value = str(v['last_out'].time())
				else:
					worksheet.Cells(rowindex, 4).Value = '0:00:00'
				worksheet.Cells(rowindex, 5).Value = str(v['tin'])
			rowindex += 1
		rowindex += 1

	# Neues Arbeitsblatt
	worksheet = workbook.Worksheets.Add()
	worksheet.Name = "Zusammenfassung"

	worksheet.Cells(1, 1).Value = 'Zusammenfassung (Sortierung: Name, Datum)'
	worksheet.Cells(1, 1).Font.Bold = True
	worksheet.Cells(1, 1).Font.Size = 20
	worksheet.Cells(2, 1).Value = 'Auswertung mit Filter = \"'+filter+'\"'
	worksheet.Cells(3, 1).Value = 'Auswertungszeitpunkt = '+str(datetime.today())

#	for index, fieldname in enumerate(fieldnames):
#		worksheet.Cells(1, (index + 1)).Value = fieldname
#		worksheet.Cells(1, (index + 1)).Font.Bold = True

	colindex = 1
	for k in hnames_keys:
		v = hnames[k]
		worksheet.Cells(5, colindex).Value = hglobal[v['liste'][0]]['hash'][k]['name']+' '+hglobal[v['liste'][0]]['hash'][k]['vname']
		worksheet.Cells(5, colindex).Font.Bold = True
		rowindex = 6
		cntMoreThan4 = 0
		for le in v['liste']:
			worksheet.Cells(rowindex, colindex).Value = str(le)
			worksheet.Cells(rowindex, colindex + 1).Value = str(hglobal[le]['hash'][k]['tin'])
			rowindex += 1
			if hglobal[le]['hash'][k]['tin'].seconds/60/60 >= 4:
				cntMoreThan4 += 1
		worksheet.Cells(rowindex, colindex).Font.Bold = True
		worksheet.Cells(rowindex, colindex).Value = 'Protokollierte Tage: '+str(rowindex-6)
		worksheet.Cells(rowindex + 1, colindex).Font.Bold = True
		worksheet.Cells(rowindex + 1, colindex).Value = 'Davon mind. 4h AZ: '+str(cntMoreThan4)
		colindex += 2

	# Workbook speichern und schließen
	workbook.SaveAs(EXCEL_WORKBOOKDIRECTORY+"\\"+filter+".xls")
	workbook.Close()

	# Excel anzeigen
	#app.visible = True

	# Excel schließen
	app.Quit()

	return
	
def write_html():
	print '<h1>Auswertung mit Filter = \"'+filter+'\"</h1>'
	print '<h4>Auswertungszeitpunkt:',datetime.today(),'</h4>'
	if mode_summary:
		print '<h2>Zusammenfassung (Sortierung: Name, Datum)</h2>'
		i = 0
		topen = 0
		for k in hnames_keys:
			v = hnames[k]
			if (i%summary_numcols) == 0:
				print '<table border="0">'
				print '<tr valign="top">'
				topen = 1
			print '<td>'
			print '<h4>'+hglobal[v['liste'][0]]['hash'][k]['name'],hglobal[v['liste'][0]]['hash'][k]['vname']+'</h4>'
			print '<table border="1">'
			for le in v['liste']:
				print '<tr>'
				print '<td>'+str(le)+'</td>',
				print '<td>'+str(hglobal[le]['hash'][k]['tin'])+'</td>'
				print '</tr>'
			print '</table>'
			print '</td>'
			i += 1
			if i%summary_numcols == 0:
				print '</tr>'
				print '</table>'
				topen = 1
		if topen:
			print '</tr>'
			print '</table>'
			topen = 1
		
	if mode_detail:
		print '<h2>Details (Sortierung: Datum)</h2>'
		for k1 in hglobal_keys:
			v1 = hglobal[k1]
			print '<h3>'+str(k1)+'</h3>'
			print '<table border="1">'
			print '<tr bgcolor="gray"><td>Name</td><td>Vorname</td><td>Erstes Login</td><td>Letztes Logout</td><td>Total IN</td></tr>'
			for k, v in v1['hash'].iteritems():
				print '<tr>'
				print '<td>' + v['name'] + '</td>',
				print '<td>' + v['vname'] + '</td>',
				if v.has_key('first_in'):
					print '<td>' + str(v['first_in'].time()) + '</td>',
				else:
					print '<td>0:00:00</td>',
				if v.has_key('last_out'):
					print '<td>' + str(v['last_out'].time()) + '</td>',
				else:
					print '<td>0:00:00</td>',
				print '<td>' + str(v['tin']) + '</td>',
				print '</tr>'
			print '</table>'
	return

def process_file(iFile):
	F = open(iFile, "rt")
	for aLine in F.readlines():
		io = aLine[32:33]
		name = aLine[38:54]
		vname = aLine[55:71]
		if vname.endswith(')'):
			vname = vname[:-7]
		name = name.rstrip()
		vname = vname.rstrip()
		dt = datetime(*strptime(aLine[14:31], '%H:%M:%S,%d-%m-%y')[0:6])
		if not hglobal.has_key(dt.date()):
			h = {}
			hglobal[dt.date()] = {'hash':h}
		else:
			h = hglobal[dt.date()]['hash']
		if not h.has_key(name+vname):
			h[name+vname] = {'name':name,'vname':vname,'tin':timedelta()}
			h[name+vname]['liste'] = [{'io':io, 'dt':dt}]
		else:
			h[name+vname]['liste'].append({'io':io, 'dt':dt})
		
	F.close()
	return


# =====================================================
# M A I N
# =====================================================

hglobal = {}
hnames = {}

for file in glob.glob(ck3path+'/'+filter+'*'):
	process_file(file)

for k1, v1 in hglobal.iteritems():
	for k, v in v1['hash'].iteritems():
		lkind = 'x'
# Tagesanfang 0:0:0 eintragen, wenn erster Eintrag='O'
#	if v['liste'][0]['io'] == 'O':
#		v['liste'].insert(0, {'io':'I', 'dt':datetime(v['liste'][0]['dt'].year,v['liste'][0]['dt'].month,v['liste'][0]['dt'].day,0,0,0)})
#	v['liste'].append({'io':'O', 'dt':datetime(v['liste'][0]['dt'].year,v['liste'][0]['dt'].month,v['liste'][0]['dt'].day,23,59,59)})
# Tagesabschluss um Mitternacht, falls sich wer nicht ausgeloggt hat...
		v['liste'].append({'io':'O', 'dt':datetime(v['liste'][0]['dt'].year,v['liste'][0]['dt'].month,v['liste'][0]['dt'].day,23,59,59),'ignore':1})
		for le in v['liste']:
			if le['io'] == 'I' and not v.has_key('first_in'):
				v['first_in'] = le['dt']
			if le['io'] == 'O' and not le.has_key('ignore'):
				v['last_out'] = le['dt']
			if lkind == 'I' and le['io'] == 'O':
				v['tin'] = v['tin'] + (le['dt'] - ldt)
				lkind = 'x'
			elif lkind == 'x' and le['io'] == 'I':
				lkind = le['io']
				ldt = le['dt']

hglobal_keys = hglobal.keys();
hglobal_keys.sort()

for k1 in hglobal_keys:
	v1 = hglobal[k1]
	for k, v in v1['hash'].iteritems():
		if not hnames.has_key(k):
			hnames[k] = {'liste':[]}
		hnames[k]['liste'].append(k1)

hnames_keys = hnames.keys();
hnames_keys.sort()

if 1:
	if mode == 'html':
		write_html()
	if mode == 'csv':
		write_csv()
if 0:
	for k, v in hnames.iteritems():
		print hglobal[v['liste'][0]]['hash'][k]['name'],hglobal[v['liste'][0]]['hash'][k]['vname']
		for le in v['liste']:
			print le,hglobal[le]['hash'][k]['tin']#.seconds/60/60