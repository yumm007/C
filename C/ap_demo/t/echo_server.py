import socket
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_addr = ('127.0.0.1', 10001)
sock.bind(server_addr)

while True:
	sock.listen(1)
	conn, client_addr = sock.accept()

	while True:
		try:
			data = 'abcdefg'
			conn.send(data)
			time.sleep(0.01);
		except:
			pass
	conn.close()

