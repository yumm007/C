#ifndef HS_MYVARARG_H
#define HS_MYVARARG_H

typedef char *myva_list;

#define MYALIGN sizeof(int)
#define myoffset(obj) \
      ((sizeof(obj) + MYALIGN - 1) / MYALIGN * MYALIGN)

#define myva_start(ap, last) \
      ((ap) = (myva_list)&(last) + myoffset(last))

#define myva_arg(ap, type) \
      ((ap) += myoffset(type), *(type *)((ap) - myoffset(type)))

#define myva_end(ap)

#endif

