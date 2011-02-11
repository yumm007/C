#include <stdio.h>
#include <stdlib.h>

#define FNAME "/tmp/big"

int
main()
{
	FILE *fp;
	off_t offset, len;

	fp = fopen(FNAME, "wb");
	if (fp=NULL) {
		perror("fopen()");
		exit(1);
	}

	offset = 5ll*1024LL*1024LL*1024LL;
	fseeko(fp, offset, SEEK_SET);

	fwrite("", 1, 1, fp);

	fseeko(fp, 0, SEEK_END);
	len = ftello(fp);

#if _FILE_OFFSET_BITS==64
	printf("%lld\n", len);
#else
	printf("%ld\n", len);
#endif

	fclose(fp);

	return 0;
}

