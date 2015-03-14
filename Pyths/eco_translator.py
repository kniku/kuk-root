# -*- coding: utf-8 -*- 

import sys,os
import hashlib
import cx_Oracle
from PyQt4 import QtGui
from PyQt4 import QtCore

os.environ['NLS_LANG'] = 'GERMAN_GERMANY.UTF8'

#DB_CONNECT = 'itsmlm/itsmlm@ora10'
DB_CONNECT = 'its90dev/its90dev@ora10'

NOT_TRANSLATED_ID = '### not yet translated ###'

def load_versions():
	conn = cx_Oracle.Connection(DB_CONNECT)
	curs = conn.cursor()
	curs.arraysize=50
	curs.execute('select distinct version from mlproduct order by version desc')
	rows = curs.fetchall()
	conn.close()
	x = []
	for row in rows:
		x.append(row[0])
	return x

def load_languages():
	conn = cx_Oracle.Connection(DB_CONNECT)
	curs = conn.cursor()
	curs.arraysize=50
	curs.execute('select language,descr from mllang order by language')
	rows = curs.fetchall()
	conn.close()
	x = []
	for row in rows:
		x.append(row[0]+' - '+row[1])
	return x

def load_translations(iModel, iApp, iVersion, iLanguage, iFilter, iTransSearch, iNotTranslated, iDialogPattern):
	iModel.clear()
	iModel.setHorizontalHeaderLabels(['Text',u'Übersetzung','Dialog','Dialogbeschreibung','Element','Version'])
	if not iFilter:
		return
	conn = cx_Oracle.Connection(DB_CONNECT)
	curs = conn.cursor()
	curs.arraysize=50

	if iTransSearch:
		wc = 'tr.translation'
	else:
		wc = 'tx.text'

	if iNotTranslated:
		wc_nottranslated = ' and not exists (select null from mltrans xtr where xtr.LANG=\''+iLanguage+'\' and xtr.TEXTKEY=tx.TEXT)'
	else:
		wc_nottranslated = ''
	
	curs.execute(
		'select tx.TEXT,nvl(tr.TRANSLATION,\'### not yet translated ###\'),tx.DIALOG,tx.DIALOGDESCR,tx.DIALOGELEMENT,app.NAME||\' \'||prd.VERSION'
		' from mltext tx'
		' join MLPRODUCT prd on prd.ID=tx.PRODUCTID'
		' join MLAPP app on app.ID=prd.APPID'
		' left join mltrans tr on tx.TEXT=tr.TEXTKEY and tr.lang=:1'
		' where app.name=:2 and prd.version=:3 and upper('+wc+') like :4 and upper(tx.dialog) like :5'
		+ wc_nottranslated +
		' and rownum<1000 order by tx.TEXT',
		[iLanguage,iApp,iVersion,iFilter.upper(),iDialogPattern.upper()]
	)
	rows = curs.fetchall()
	conn.close()
	i = 0
	for row in rows:
		for j in range(0,6):
			item = QtGui.QStandardItem(row[j].decode('utf-8'))
			item.setEditable(0)
			if j < 2:
				ff = item.font()
				ff.setBold(1)
				item.setFont(ff)
			if j == 1 and row[1] == NOT_TRANSLATED_ID:
				item.setForeground(QtCore.Qt.red)
			iModel.setItem(i, j, item)
		i += 1

class InputDialog(QtGui.QWidget):
	def __init__(self, parent=None):
		QtGui.QWidget.__init__(self, parent)

		self.setGeometry(100, 100, 800, 600)
		self.setWindowTitle(u'Übersetzungen ('+DB_CONNECT+')')

		self.layout = QtGui.QGridLayout()
		self.setLayout(self.layout)
		
		self.lab_sprache = QtGui.QLabel('Sprache:', self)
		self.layout.addWidget(self.lab_sprache, 0, 0)
		self.ef_srache = QtGui.QComboBox(self)
		self.ef_srache.addItems(load_languages())
		if self.ef_srache.currentText()[:2] == 'de':
			self.ef_srache.setCurrentIndex(1)
		self.layout.addWidget(self.ef_srache, 0, 1)
		self.lab_version = QtGui.QLabel('Version:', self)
		self.layout.addWidget(self.lab_version, 1, 0)
		self.ef_app = QtGui.QLineEdit(self)
		self.ef_app.setDisabled(1)
		self.ef_app.setText("ITSWEB")
		self.layout.addWidget(self.ef_app, 1, 1)
		self.ef_version = QtGui.QComboBox(self)
		self.ef_version.addItems(load_versions())
		self.layout.addWidget(self.ef_version, 1, 2)
		self.searchLabel = QtGui.QLabel('Suchmuster:', self)
		self.layout.addWidget(self.searchLabel, 2, 0)
		self.searchPattern = QtGui.QLineEdit(self)
		self.searchPattern.setText('%')
		self.layout.addWidget(self.searchPattern, 2, 1)
		self.rd_bereich1 = QtGui.QRadioButton('Text')
		self.rd_bereich2 = QtGui.QRadioButton(u'Übersetzung')
		self.rd_nottrans = QtGui.QRadioButton(u'Nicht übersetzt')
		self.layout.addWidget(self.rd_bereich1, 2,2)
		self.layout.addWidget(self.rd_bereich2, 2,3)
		self.layout.addWidget(self.rd_nottrans, 2,4)
		self.rd_bereich1.setChecked(1)
		self.button = QtGui.QPushButton('Suchen', self)
		self.layout.addWidget(self.button, 2, 5)
		self.connect(self.button, QtCore.SIGNAL('clicked()'), self.updateTranslations)
		self.lab_dialog = QtGui.QLabel('Dialog:', self)
		self.layout.addWidget(self.lab_dialog, 3, 0)
		self.ef_dialog = QtGui.QLineEdit(self)
		self.ef_dialog.setText('%')
		self.layout.addWidget(self.ef_dialog, 3, 1)

		self.liste = QtGui.QTableView(self)
		self.item_model = QtGui.QStandardItemModel(0, 2)
		self.liste.setModel(self.item_model)
		self.liste.setAlternatingRowColors(1)
		self.liste.setSelectionBehavior(QtGui.QAbstractItemView.SelectRows)
		self.liste.setSelectionMode(QtGui.QAbstractItemView.SingleSelection)
		self.connect(self.liste, QtCore.SIGNAL('clicked(const QModelIndex &)'), self.itemClicked)
		self.connect(self.liste, QtCore.SIGNAL('activated(const QModelIndex &)'), self.itemClicked)
		self.layout.addWidget(self.liste, 4, 0, 1, 5)
		
		self.ef_text = QtGui.QTextEdit(self)
		self.ef_text.setFixedHeight(100)
		#self.ef_text.setDisabled(1)
		self.layout.addWidget(self.ef_text, 5, 0)
		self.ef_translation = QtGui.QTextEdit(self)
		self.ef_translation.setFixedHeight(100)
		self.layout.addWidget(self.ef_translation, 5, 1, 1, 3)
		self.button_change = QtGui.QPushButton(u'Ändern', self)
		self.connect(self.button_change, QtCore.SIGNAL('clicked()'), self.changeTranslation)
		self.layout.addWidget(self.button_change, 5, 4)

		load_translations(self.item_model, None, None, None, None, None, None, None)
		
	def itemClicked(self, x):
		self.ef_text.setText(self.item_model.item(x.row(),0).text())
		if self.item_model.item(x.row(),1).text() == NOT_TRANSLATED_ID:
			self.ef_translation.setText('')
		else:
			self.ef_translation.setText(self.item_model.item(x.row(),1).text())
		self.last_index = x
		
	def updateTranslations(self):
		app.setOverrideCursor(QtGui.QCursor(QtCore.Qt.BusyCursor))
		load_translations(self.item_model, str(self.ef_app.text()), str(self.ef_version.currentText()), str(self.ef_srache.currentText())[:2], str(self.searchPattern.text().toUtf8()), self.rd_bereich2.isChecked(), self.rd_nottrans.isChecked(), str(self.ef_dialog.text().toUtf8()))
		app.restoreOverrideCursor()
		self.liste.resizeColumnsToContents()
		self.liste.resizeRowsToContents()

	def changeTranslation(self):
#		QtGui.QInputDialog.getText(self, 'last index', str(self.last_index.row())+'/'+str(self.last_index.column()))
		r = 0
		while r < self.item_model.rowCount():
			x1 = self.item_model.item(r, 0)
			if x1.text() == self.ef_text.toPlainText():
				x2 = self.item_model.item(r, 1)
				x2.setText(self.ef_translation.toPlainText())
				x2.setForeground(QtCore.Qt.green)
			r += 1
		conn = cx_Oracle.Connection(DB_CONNECT)
		curs = conn.cursor()
		md5str = hashlib.md5(str(self.ef_text.toPlainText().toUtf8())).hexdigest().upper()
		
		curs.execute(
			'delete from mltrans'
			' where MCODE=\'*\' and LOOK=\'*\' and LANG=\''+str(self.ef_srache.currentText())[:2]+
			'\' and APP=\'ITSWEB\' and MODUL=\'*\' and DIALOG=\'*\''
			' and TEXTKEY=\''+str(self.ef_text.toPlainText().toUtf8())+'\''
		)
		print str(self.ef_translation.toPlainText().toUtf8())
		curs.execute(
			'insert into mltrans'
			' (MCODE,LOOK,LANG,APP,MODUL,DIALOG,TEXTKEY,TRANSLATION,ISACTIVE,TEXTMD5)'
			' values(\'*\',\'*\',\''+str(self.ef_srache.currentText())[:2]+
			'\',\'ITSWEB\',\'*\',\'*\',\''+str(self.ef_text.toPlainText().toUtf8())+'\',\''+str(self.ef_translation.toPlainText().toUtf8())+'\',\'Y\',\''+md5str+'\')'
		)
		conn.commit()
		conn.close()

app = QtGui.QApplication(sys.argv)
icon = InputDialog()
icon.show()
app.exec_()
