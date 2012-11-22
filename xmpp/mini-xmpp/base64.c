/** Base64 encoding routines. Implemented according to RFC 3548 */

/** map of all byte values to the base64 values, or to
    '65' which indicates an invalid character. '=' is '64' */
static const char _base64_invcharmap[256] = {
    65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65,
    65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65,
    65,65,65,65, 65,65,65,65, 65,65,65,62, 65,65,65,63,
    52,53,54,55, 56,57,58,59, 60,61,65,65, 65,64,65,65,
    65, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,65, 65,65,65,65,
    65,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,65, 65,65,65,65,
    65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65,
    65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65,
    65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65,
    65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65,
    65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65,
    65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65,
    65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65,
    65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65 
};

/** map of all 6-bit values to their corresponding byte
    in the base64 alphabet. Padding char is the value '64' */
static const char _base64_charmap[65] = {
    'A','B','C','D', 'E','F','G','H',
    'I','J','K','L', 'M','N','O','P',
    'Q','R','S','T', 'U','V','W','X',
    'Y','Z','a','b', 'c','d','e','f',
    'g','h','i','j', 'k','l','m','n',
    'o','p','q','r', 's','t','u','v',
    'w','x','y','z', '0','1','2','3',
    '4','5','6','7', '8','9','+','/',
    '='
};

char *base64_encode(char *cbuf, const char *buffer, int len)
{
    int clen;
    char *c;
    unsigned int word, hextet;
    int i;

   clen = ((len + 2)/3) << 2;
	c = cbuf;
	/* loop over data, turning every 3 bytes into 4 characters */
	for (i = 0; i < len - 2; i += 3) {
	    word = buffer[i] << 16 | buffer[i+1] << 8 | buffer[i+2];
	    hextet = (word & 0x00FC0000) >> 18;
	    *c++ = _base64_charmap[hextet];
	    hextet = (word & 0x0003F000) >> 12;
	    *c++ = _base64_charmap[hextet];
	    hextet = (word & 0x00000FC0) >> 6;
	    *c++ = _base64_charmap[hextet];
	    hextet = (word & 0x000003F);
	    *c++ = _base64_charmap[hextet];
	}
	/* zero, one or two bytes left */
	switch (len - i) {
	    case 0:
		break;
	    case 1:
		hextet = (buffer[len-1] & 0xFC) >> 2;
		*c++ = _base64_charmap[hextet];
		hextet = (buffer[len-1] & 0x03) << 4;
		*c++ = _base64_charmap[hextet];
		*c++ = _base64_charmap[64]; /* pad */
		*c++ = _base64_charmap[64]; /* pad */
		break;
	    case 2:
		hextet = (buffer[len-2] & 0xFC) >> 2;
		*c++ = _base64_charmap[hextet];
		hextet = ((buffer[len-2] & 0x03) << 4) |
			 ((buffer[len-1] & 0xF0) >> 4);
		*c++ = _base64_charmap[hextet];
		hextet = (buffer[len-1] & 0x0F) << 2;
		*c++ = _base64_charmap[hextet];
		*c++ = _base64_charmap[64]; /* pad */
		break;
	}
	/* add a terminal null */
	*c = '\0';
	return cbuf;
}

#if 0
#include <stdio.h>
int main(int argc, char *argv[]) {
	char a[100], b[100];
	FILE *fp;
	int n;

	memset(b, 0, 100);
	memset(a, 0, 100);

	fp = fopen(argv[1], "r");
	n = fread(a, 1, 100, fp);

	base64_encode(b, a, n);
	fprintf(stderr,"%s\n", b);

	return 0;
}
#endif
