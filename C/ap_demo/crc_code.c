
static UINT8 file_crc_check(file_id_t fd) {
	UINT16 old_crc, crc = 0;
	UINT32 i, j, size;
	UINT8 c;
	f_read(fd, 0, (UINT8 *)&old_crc, 2);	//头2个字节为CRC检验码
	f_read(fd, 2, (UINT8 *)&size, 4);		//读入4字节文件长度
		
	for (j = 2; j < size; j++) {
		f_read(fd, j, &c, 1);
		for(i = 0x80; i != 0; i /= 2) {
			crc *= 2;
			if((crc & 0x8000) != 0)
				crc ^= 0x1021;
			if((c & i) != 0) 
				crc ^= 0x1021;
 		}		
	}
	
	return old_crc - crc;
}	
