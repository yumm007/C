
char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0');

	return tmp;
}

int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}

int strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc);
	
	return sc - s;
}


int strnlen(const char * s, int count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc);

	return sc - s;
}

void * memset(void * s,int c,int count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}

void * memcpy(void * dest,const void *src,int count)
{
	char *tmp = (char *) dest, *s = (char *) src;

	while (count--)
		*tmp++ = *s++;

	return dest;
}

int memcmp(const void * cs,const void * ct,int count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}


