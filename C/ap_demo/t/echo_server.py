import socket
import time
import threading
import struct


def R16(n):
	return (~n) & 0xFFFF

def R32(n):
	return (~n) & 0xFFFFFFFF

def do_writedata():
	return struct.pack("HH8sHHHHIIHH570s", 16, R16(16), 
							'16', 2, R16(2), 1, R16(1), 570, R32(570), 0x55, 0x55, 'abcd')

def do_writecmd(cmd, argv):
	return struct.pack("HH8sHHHHIIHH", 16, R16(16), 
							'16', cmd, R16(cmd), argv, R16(argv), 0, R32(0), 0x55, 0x55)

def do_recver(conn):
	print("thread running")
	while True:
		try:
			data = conn.recv(1024)
			print('recved bytes ', len(data), ':')
		except:
			break

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1) 
server_addr = ('192.168.1.118', 10001)
sock.bind(server_addr)
n = 0
flag = True

while True:
	sock.listen(1)
	n = n + 1
	conn, client_addr = sock.accept()
	print('new connect ', n)
	t = threading.Thread(target=do_recver, args={conn})
	t.start()
	while True:
		try:

			if flag == True:
				data = do_writedata()
			else:
				data = do_writecmd(n, n + 1)
			flag = not flag

			conn.send(data)
			time.sleep(0.01);
		except:
			pass
			n1 = 0
			conn.close()
			break

sock.close()

