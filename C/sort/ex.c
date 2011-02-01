#include <stdio.h>

#define BUFNUM 2

struct file {
      FILE *fp;
      int len;
};

void init_sort(struct file *fpr, struct file *fpw1, struct file *fpw2, int buf[], int n)
{
      FILE *fpw = fpw1->fp;
      int ret;

      fpw1->len = fpw2->len = 0;
      while (1) {
            ret = fread(buf, sizeof(int), n, fpr->fp);
            if (ret == 0) {
                  break;
            }
            sort(buf, ret);
            fwrite(buf, sizeof(int), ret, fpw);
            if (fpw == fpw1->fp) {
                  fpw1->len += ret;
                  fpw = fpw2->fp;
            } else {
                  fpw2->len += ret;
                  fpw = fpw1->fp;
            }
      }   
}

void merge(struct file dest[2], struct file src[2], int len)
{
      struct file *d = dest;
      int ret;

      dest[0].len = 0;
      dest[1].len = 0;
      while (1) {
            ret = single_merge(d, src[0], src[1], len);
            if (ret == 0) {
                  break;
            }

            d->len += ret;
            if (d == dest) {
                  d = dest + 1;
            } else {
                  d = dest;
            }
      }   
}

int main(void)
{
      struct file fp[2][2];
      struct file (*arrp1)[2] = fp; 
      struct file (*arrp2)[2] = fp + 1;
      int buf[BUFNUM];
      int len = BUFNUM;

      init_sort(&fp[0][0], &fp[1][0], &fp[1][1], buf, BUFNUM);
      while (1) {
            merge(*arrp1, *arrp2, len);
            swap(p2, p1);
            len *= 2;
      }   

      cpback();

      return 0;

}
