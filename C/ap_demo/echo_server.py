import socket
import time
import threading
import struct
import random
import os


def R16(n):
	return (~n) & 0xFFFF

def R32(n):
	return (~n) & 0xFFFFFFFF

def do_writedata():
	n = int(random.random() * 1000)
	f = "HH8sHHHHIIHH" + str(n) + "s"
	return struct.pack(f, 16, R16(16), 
							'16', 2, R16(2), 1, R16(1), n, R32(n), 0x55, 0x55, 'abcd')

def do_writecmd(cmd, argv):
	return struct.pack("HH8sHHHHIIHH", 16, R16(16), 
							'16', cmd, R16(cmd), argv, R16(argv), 0, R32(0), 0x55, 0x55)

def do_recver(conn):
	print("thread running")
	tf = "tempfile"
	while True:
		try:
			f = open(tf, 'wb');
			header = conn.recv(32)
			f.write(header)
			lenth = do_check_header(header)
			while lenth > 0:
				body = conn.recv(lenth)
				lenth -= len(body)
				f.write(body)
			f.close()
			f = open(tf, 'rb');
			body = f.read()
			print('recved bytes ', len(body))
			f.close()
			os.remove(tf)
		except:
			os.remove(tf)
			break

def do_check_header(data):
	v, v_s, v_str, op, op_s, para, para_s, lenth, lenth_s, rev1, rev2 = \
		struct.unpack("HH8sHHHHIIHH", data)
	#print(v, v_s, v_str, op, op_s, para, para_s, lenth, lenth_s, rev1, rev2)
	if R16(v) != v_s or R16(op) != op_s or R16(para) != para_s:
		return 0
	else: 
		return lenth

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1) 
server_addr = ('127.0.0.1', 10001)
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
			if n == 200:
				conn.close()
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

