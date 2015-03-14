import cx_Oracle

conn = cx_Oracle.Connection("its81dev/its81dev@ora10")

curs = conn.cursor()
curs.arraysize=50

curs.execute(
	'select firma,bzeic from itdb901'
	' where firma not like :1 order by firma',
	['TEST%']
)
print(curs.description)

rows = curs.fetchall()
for row in rows:
	print(row[0])
#	print(row[0],row[1].encode('utf-8'))
#	print(row[0], row[1])

conn.close()