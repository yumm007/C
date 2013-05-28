import socket
import time
import threading
import struct

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1) 
server_addr = ('127.0.0.1', 10001)
sock.bind(server_addr)
n = 0

def do_recver(conn):
	print("thread running")
	while True:
		try:
			data = conn.recv(1024)
			print('recved bytes ', len(data), ':')
		except:
			break

while True:
	sock.listen(1)
	n = n + 1
	conn, client_addr = sock.accept()
	print('new connect ', n)
	t = threading.Thread(target=do_recver, args={conn})
	t.start()
	while True:
		try:
			#data = struct.pack("HH10sHHHHIIHH", 16, 16, 'aa', 16, 16, 16, 16, 16, 16, 16, 16)
			data = "abcdefg1234567"
			conn.send(data)
			time.sleep(0.01);
		except:
			pass
			n1 = 0
			conn.close()
			break

sock.close()

