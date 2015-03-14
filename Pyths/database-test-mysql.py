import MySQLdb
import os

db = MySQLdb.connect("localhost","root","anlusa","test")
c = db.cursor()
c.execute("select count(*) from xxx")
f = c.fetchone()
print "count(*) of xxx:",f[0]

c.execute("select * from xxx")
#f = c.fetchone()
f = c.fetchall()
#print f

print "====================================="
for row in range(0,len(f)):
	print f[row],
	print f[row][0],f[row][1],
	print "\n----------------------------"

irows = []
rowmax=10000
for row in range(0,rowmax):
	irows.append(("x"+str(row),"hidihohidihohidiho",row,row*1.1))
#print irows

print "start of inserting",row+1,"rows...",
c.executemany(
	"""INSERT INTO xxx (id,varchar1,numint,numdbl) VALUES (%s, %s, %s, %s)""",
	irows
)

for row in range(0,rowmax):
	irows[row] = ("xxxxxxxxxxxx","x"+str(row))
print "done.\n",
print "start of updating",row+1,"rows...",
c.executemany(
	"""update xxx set varchar1=%s where id=%s""",
	irows
	)

print "done.\n",
for row in range(0,rowmax):
	irows[row] = ("x"+str(row))
print "start of deleting",row+1,"rows...",
# much faster would be:
#c.execute(
#	"""delete from xxx where id like'x%'"""
#	)
c.executemany(
	"""delete from xxx where id=%s""",
	irows
	)

c.execute("commit")
print "done.\n",
  
c.close();
