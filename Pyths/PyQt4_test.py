import sys
from PyQt4 import QtGui
from PyQt4 import QtCore


class InputDialog(QtGui.QWidget):
	def __init__(self, parent=None):
		QtGui.QWidget.__init__(self, parent)

		self.setGeometry(300, 300, 350, 180)
		self.setWindowTitle('InputDialog')

		self.button = QtGui.QPushButton('Dialog', self)
		self.button.setFocusPolicy(QtCore.Qt.NoFocus)

		self.layout=QtGui.QGridLayout(self)
		
#		self.button.move(20, 20)
		self.connect(self.button, QtCore.SIGNAL('clicked()'), self.showDialog)
		self.connect(self.button, QtCore.SIGNAL('clicked()'), self.showDialog2)
		self.setFocus()

		self.layout.addWidget(self.button,1,1)
		
		self.x = QtGui.QTableWidget(self)
		self.x.setColumnCount(2)
		self.x.setRowCount(2)
#		self.m=QtGui.QStandardItemModel(2,2)
		self.x.setItem(0,0,QtGui.QTableWidgetItem("test1"))
		self.x.setItem(1,1,QtGui.QTableWidgetItem("test2"))
#		self.x.setModel(self.m)
#		self.connect(self.x, QtCore.SIGNAL('itemSelectionChanged()'), self.showDialog2)
#		self.connect(self.x, QtCore.SIGNAL('cellClicked(int,int)'), self.showDialog2)
		self.connect(self.x, QtCore.SIGNAL("itemClicked(QTableWidgetItem*)"), self.showDialog2)
		
		self.layout.addWidget(self.x,0,1)

		self.label = QtGui.QLineEdit(self)
#		self.label.move(130, 22)
		self.layout.addWidget(self.label,1,0)


	def showDialog2(self):
		text, ok = QtGui.QInputDialog.getText(self, 'Input Dialog2', 'x/y: ')

	def showDialog(self):
		text, ok = QtGui.QInputDialog.getText(self, 'Input Dialog', 'Enter your name:')

		if ok:
			self.label.setText(unicode(text))



app = QtGui.QApplication(sys.argv)
icon = InputDialog()
icon.show()
app.exec_()
