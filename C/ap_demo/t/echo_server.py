import socket
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1) 
server_addr = ('127.0.0.1', 10001)
sock.bind(server_addr)
n = 0
n1 = 0

while True:
	sock.listen(1)
	n = n + 1
	conn, client_addr = sock.accept()
	print('new connect ', n)
	while True:
		try:
			data = 'abcdefg'
			conn.send(data)
			#if n1 > 100 and n1 % 100 == 1 :
			#	data = conn.recvall()
			#	print('recved bytes ', len(data))
			time.sleep(0.01);
			n1 = n1 + 1
		except:
			pass
			n1 = 0
			conn.close()
			break

sock.close()

