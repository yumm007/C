unsigned int __aeabi_uidiv(unsigned int num, unsigned int d)
{
	unsigned int count = 0;

	if (d == 0)
		while(1);
	if (d == 1)
		return num;

	while(num >= d)
	{
		num -= d;
		count++;
	}

	return count;
}

unsigned int __aeabi_i2d(unsigned int num, unsigned int d) {
	return 0;
}

unsigned int __aeabi_dsub(unsigned int num, unsigned int d) {

	return 0;
}

unsigned int __aeabi_dmul(unsigned int num, unsigned int d) {

	return 0;
}

unsigned int __aeabi_d2iz(unsigned int num, unsigned int d) {

	return 0;
}

