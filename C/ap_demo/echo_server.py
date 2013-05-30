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
	return struct.pack(f, 16, R16(16), '16', 2, R16(2), 1, \
								R16(1), n, R32(n), 0x55, 0x55, 'abcd')

def do_writecmd(cmd, argv):
	return struct.pack("HH8sHHHHIIHH", 16, R16(16), '16', \
								cmd, R16(cmd), argv, R16(argv), 0, R32(0), 0x55, 0x55)

def do_recver(conn):
	tf = "tempfile"
	f = open(tf, 'wb');
	ret = True
	header = conn.recv(32)
	f.write(header)
	lenth = do_check_header(header)
	while lenth > 0:
		body = conn.recv(lenth)
		lenth -= len(body)
		f.write(body)
	f.close()
	f = open(tf, 'rb');
	data = f.read()
	esl_num = do_check_body(data)
	print 'recved %d data, total %d' % (esl_num, len(data))
	f.close()
	os.remove(tf)
	return ret

def do_check_header(data):
	v, v_s, v_str, op, op_s, para, para_s, lenth, lenth_s, rev1, rev2 = \
		struct.unpack("=HH8sHHHHIIHH", data)
	#print(v, v_s, v_str, op, op_s, para, para_s, lenth, lenth_s, rev1, rev2)
	if R16(v) != v_s or R16(op) != op_s or R16(para) != para_s:
		return 0
	else: 
		return lenth

def do_check_body(data):
	if len(data) <= 32:
		return 0
	try:
		ctrl, para, powermode, wkup_id, rf_ch, \
			data_esl_num, sleep_esl_num = struct.unpack("=BBBIBHH", data[32:44])
		#print(ctrl, para, powermode, wkup_id, rf_ch, data_esl_num, sleep_esl_num)
		return data_esl_num
	except:
		print("do_check_body failed")
		return 0

def do_ack():
	return do_writecmd(0x0100, 0)

def do_nack():
	return do_writecmd(0x0200, 0)

def do_main():
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1) 
	server_addr = ('127.0.0.1', 10001)
	sock.bind(server_addr)

	while True:
		sock.listen(1)
		conn, client_addr = sock.accept()
		while True:
			try:
				if do_recver(conn) == True:
					conn.send(do_ack())
					print("send ack")
				else:
					conn.send(do_nack())
					print("send nack")
			except:
				conn.close();
				break;

	sock.close()

do_main()


