import Tkinter
import tkMessageBox
import tkSimpleDialog


def callback():
	tkMessageBox.showinfo("tkMessageBox","called the callback!")
	tkSimpleDialog.askstring("tkSimpleDialog", "Eingabe")
	
def create_menu(root):
	menu = Tkinter.Menu(root)
	filemenu = Tkinter.Menu(menu)
	menu.add_cascade(label="File", menu=filemenu)
	filemenu.add_command(label="New", command=callback)
	filemenu.add_command(label="Open...", command=callback)
	filemenu.add_separator()
	filemenu.add_command(label="Exit", command=root.quit)
	helpmenu = Tkinter.Menu(menu)
	menu.add_cascade(label="Help", menu=helpmenu)
	helpmenu.add_command(label="About...", command=callback)
	root.config(menu=menu)

def create_toolbar(root):
	toolbar = Tkinter.Frame(root)
	b = Tkinter.Button(toolbar, text="tollbar-1", width=6, command=callback)
	b.pack(side=Tkinter.LEFT, padx=1, pady=1)
	b = Tkinter.Button(toolbar, text="toolbar-2", width=6, command=callback)
	b.pack(side=Tkinter.LEFT, padx=1, pady=1)
	toolbar.pack(side=Tkinter.TOP, fill=Tkinter.X)

def create_statusbar(root):
	status = Tkinter.Label(root, text="statusbar...", bd=1, relief=Tkinter.SUNKEN, anchor=Tkinter.W)
	status.pack(side=Tkinter.BOTTOM, fill=Tkinter.X)

root = Tkinter.Tk()
create_menu(root)
create_toolbar(root)
create_statusbar(root)

w = Tkinter.Label(root, text="Label")
w.pack()
w = Tkinter.Checkbutton(root, text="Checkbutton")
w.pack()
#w.pack(side="left")
w = Tkinter.Button(root, text="Button", command=callback)
w.pack()

MODES = [
	("Monochrome", "1"),
	("Grayscale", "L"),
	("True color", "RGB"),
	("Color separation", "CMYK"),
]
v1 = Tkinter.StringVar()
v1.set("L") # initialize
for text, mode in MODES:
	b = Tkinter.Radiobutton(root, text=text, variable=v1, value=mode)#, indicatoron=0)
	b.pack(anchor=Tkinter.W)
v2 = Tkinter.StringVar()
v2.set("RGB") # initialize
for text, mode in MODES:
	b = Tkinter.Radiobutton(root, text=text, variable=v2, value=mode, indicatoron=0)
	b.pack(anchor=Tkinter.W)


root.mainloop()
