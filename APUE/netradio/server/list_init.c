#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>

#include <medialib.h>

#define FILE_NAME_SIZE 1024
#define ERRSIZE 512

int medialib_free_channellist(struct channel_item_st *head)
{
	struct channel_item_st *cur, *tmp;
	cur = head;
	while(cur!= NULL){
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	return 0;
}

static int pay_desc(char *path, char *buf)
{
	FILE *fp;
	int size, ret;
	char errstr[ERRSIZE] = {0};

	fp = fopen(path, "rb");
	if(fp == NULL){
		strerror_r(errno, errstr, ERRSIZE);
		syslog(LOG_ERR, "%s", errstr);
		return -1;
	}
	
	if((ret = fread(buf, DESCLEN, 1, fp)) < 0){
		strerror_r(errno, errstr, ERRSIZE);
		syslog(LOG_ERR, "%s", errstr);
		return -1;
	}

	size = strlen(buf);
	buf[size - 1] = '\0';
	fclose(fp);

	return 0;
}

int medialib_get_channellist(int *count, struct channel_item_st **head)
{
	glob_t glob_res;
	int i;
	size_t size;
	struct channel_item_st *cur, *tmp;
	struct channel_item_st *newnode = NULL;
	char filebuf[FILE_NAME_SIZE];
	char errstr[ERRSIZE] = {0};

	newnode = malloc(sizeof(struct channel_item_st));
	if(newnode == NULL){
		strerror_r(errno, errstr, ERRSIZE);
		syslog(LOG_ERR, "%s", errstr);
		return -1;
	}
	
	cur = newnode;
	*head = newnode;

	if(glob(args.media_path, 0, NULL, &glob_res) < 0){
		strerror_r(errno, errstr, ERRSIZE);
		syslog(LOG_ERR, "%s", errstr);
		return -1;
	}
	
	for(i = 0 ; i < glob_res.gl_pathc; i++){
		strcpy(filebuf, glob_res.gl_pathv[i]);
		size = strlen(filebuf);
		strcpy(filebuf + size, "/desc.txt");

		if(pay_desc(filebuf, (newnode->channel_info.desc)) < 0){
			continue;
		}

		newnode->channel_info.id = i + 1;//id
		strcpy(newnode->channel_info.path, glob_res.gl_pathv[i]);//path

		tmp = newnode;
		newnode = malloc(sizeof(struct channel_item_st));
		if(newnode == NULL){
			strerror_r(errno, errstr, ERRSIZE);
			syslog(LOG_ERR, "%s", errstr);
			newnode->next = NULL;
			medialib_free_channellist(cur);
			globfree(&glob_res);
			*head = NULL;
			return -1;
		}
		tmp->next = newnode;

	}

	free(newnode);
	if(glob_res.gl_pathc > 0){
		tmp->next = NULL;
	}else{
		syslog(LOG_ERR, "medialib is empty");
		return -1;
	}

	*count = glob_res.gl_pathc;

	globfree(&glob_res);
	return 0;
	
}

