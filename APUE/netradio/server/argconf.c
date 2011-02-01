#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <medialib.h>

#define DEFAULT_CONFPATH "/etc/netradio.conf"
#define DEFAULT_MEDIA_PATH "/usr/local/netradio/medialib/*"

#define TMPBUFSIZE 1024
 
static int chkchr(char *start,char *end,int *cnt)
{
	char *ch;
	*cnt = 0;
	for(ch=start;end-ch>0;ch++,(*cnt)++){
		if(*ch!=32 && *ch!='\t'){
			return -1;
		}
	}
	(*cnt)++;
	return 0;
}

void sep_arg(struct arg_st *argp)
{
	FILE *fp;
	int i,j;
	int ret,cnt,flag;
	size_t nsiz = 0;
	char *lineptr = NULL;
	char *st;
	char tmpbuf[TMPBUFSIZE];
	char *items[] = {"media_path","recieve_port","multicast_group"};

	strncpy(argp->media_path,DEFAULT_MEDIA_PATH,MPATHSIZE);
	argp->recieve_port = DEFAULT_RCVPORT;
	strncpy(argp->multicast_group,DEFAULT_MGROUP,MPATHSIZE);

	if((fp=fopen(DEFAULT_CONFPATH,"r"))==NULL){
		syslog(LOG_ERR, "open configration file failed, use default");
		return ;
	}

	while(1){
		flag = 0;
		memset(tmpbuf,0,TMPBUFSIZE);
		ret = getline(&lineptr,&nsiz,fp);
		if(ret<=0){
			break;
		}
		memmove(tmpbuf,lineptr,ret-1);
		if(tmpbuf[0]!='#'){
			for(j = 0;j < 3;j++){
				if(strncmp(items[j],tmpbuf,strlen(items[j]))==0){
					st = strchr(tmpbuf,'=');
					if(chkchr(tmpbuf+strlen(items[j]),st,&cnt)==0){
					i = cnt+strlen(items[j]);
					while(1){
						if((tmpbuf[i]!=32&&tmpbuf[i]!='\t')||flag==1){
							flag = 1;
							if(j==0){
								strncpy(argp->media_path,tmpbuf+i,MPATHSIZE);
								if(argp->media_path[ret-i-2]=='/'){
									strcat(argp->media_path,"*");
								}else {
									strcat(argp->media_path,"/*");
								}
							}else if(j==1){
								argp->recieve_port = atoi(tmpbuf+i);
							}else if(j==2){
								strncpy(argp->multicast_group,tmpbuf+i,MPATHSIZE);
							}else ;
							break;
						}
						i++;
					}
					break;
					}
				}
			}
		}
	}

	free(lineptr);
	fclose(fp);
}

