import sys,socket

VERBOSE = 0

def usage():
	print('usage: '+sys.argv[0]+' [host] [port]')
	sys.exit(0)

print('=================================')
print('ecoserv shell, v0.1, (c) kuk 2009')
print('=================================')
if len(sys.argv) < 3:
	usage()

es_host = sys.argv[1]
es_port = sys.argv[2]

print('trying to connect ecoserv on '+es_host+' port '+es_port+'...')

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
	s.connect((es_host, int(es_port)))
except:
	print('### connection failed')
	raise

print('retrieving status...')
#s.send('<info/>')
s.send('<esm t="info"/>')
data = s.recv(1024)
s.close()
print('['+data+']')
print('=========================================')

while True:
	print('ES-SHELL> '),
	input = sys.stdin.readline()
	input = input[:-1]
	print('['+input+']')
	if input == 'h':
		print('valid commands are:')
		print('   es.COMMAND .... send COMMAND to ecoserv')
		print('                   e.g.: PZE request:')
		print('                   \'es.es3.5:PZE:kuk                           :0000000000\'')
		print('                   use \'es.<help/>\' to get some help from the server...')
		print('   est.COMMAND ... send COMMAND to ecoserv (telnet protocol)')
		print('                   use \'est.help\' to get some help from the server...')
		print('   v ............. toggle verbose mode')
		print('   q ............. exit ES shell')
	elif input == 'q':
		sys.exit(0)
	if input == 'v':
		if VERBOSE: VERBOSE = 0;print('verbose mode OFF')
		else: VERBOSE = 1;print('verbose mode ON')
		
	elif len(input) > 4 and input[:4] == 'est.':
		input = input[4:]
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.connect((es_host, int(es_port)))
		if VERBOSE: print('->[<*>]')
		s.send('<*>')
		data = s.recv(1024)
		print(data)
		data = s.recv(1024)
		print(data)
		print('->['+input+']')
		s.send(input+'\n')
		data = s.recv(1024)
		s.close()
		print('===== ecoserv start =====')
		print(data)
		print('===== ecoserv end =======')
		
	elif len(input) > 3 and input[:3] == 'es.':
		input = input[3:]
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.connect((es_host, int(es_port)))
		if VERBOSE: print('->['+input+']')
		s.send(input)
		print('===== ecoserv start =====')
		while True:
			data = s.recv(1024)
			if not data: break
			print(data),
		s.close()
		print('===== ecoserv end =======')
		
	else:
		print('unknown command! enter \'h\' for informations...')
