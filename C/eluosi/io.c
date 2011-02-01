#include "myhead.h"

extern FILE *fp, *fplog, *fplog2;

int read_key_from_file(int flag){
	int way;
	fread(&way,sizeof(int),1, fp);
	fprintf(fplog2, "read way is %d\n",way);
	fflush(fplog2);
	usleep(40000);
	return way;
}

void read_box_from_file(struct brickbox *b) {
	int way;
	long ind;

	do {	
		fread(&way, sizeof(way), 1, fp);
		if (way >= UP && way <= LEFT) {
			fprintf(fplog2, "read way is %d\n",way);
			fflush(fplog2);
		} else {
			ind = ftell(fp);
			fseek(fp, ind - sizeof(int), SEEK_SET);
			break;
		}
	} while ( way >= UP && way <= LEFT);

	fread(b,sizeof(*b), 1,fp);
	fprintf(fplog2, "read box is %d->%d, size is %d\n",b->b1, b->b2, sizeof(*b));
	fflush(fplog2);
}

int save_key_to_file(int way) {
	fwrite(&way,sizeof(int),1, fp);
	fprintf(fplog, "save way is %d\n",way);
}

int save_box_to_file(const struct brickbox *b) {	
		
	fwrite(b,sizeof(*b), 1,fp);
	fprintf(fplog, "save box is %d->%d, size is %d\n",b->b1, b->b2, sizeof(*b));
}
