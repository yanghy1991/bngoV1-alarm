#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <errno.h>

//#define MAX_TEXT 512

struct msg_st
{
	long int msg_type;
	int flag;
	char text[4096];
};

typedef struct doalarm_t{
	char alarmId[64];
	char alarm_switch[10];
	char alarm_msg[10];
	char alarm_text[4012];
}doalarm_t;


int main(int argc,char **argv)
{	
	if(argc < 4)
	{
		printf("usage: cmd id topic switch msg\n");
		return 0;
	}
	printf("argv1 = %s,argv2 = %s,argv3 = %s,argv4 = %s\n", argv[0],argv[1],argv[2],argv[3],argv[4]);
	
	struct msg_st alarm_data;
	doalarm_t doalarm_data;
	memset(&doalarm_data,0,sizeof(doalarm_data));
	//char buffer[4096];
	int msgid = -1;
	
	//建立消息队列
	msgid = msgget((key_t)4781, 0666 | IPC_EXCL);
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	//向消息队列中写消息
	//strcpy(buffer,argv[2]);
	alarm_data.msg_type = 1;    //注意2
	alarm_data.flag = 1;
	//strcpy(alarm_data.text, buffer);
	strcpy(doalarm_data.alarmId,argv[1]);
	strcpy(doalarm_data.alarm_text,argv[2]);
	strcpy(doalarm_data.alarm_switch,argv[3]);
	strcpy(doalarm_data.alarm_msg,argv[4]);
	memcpy(alarm_data.text,&doalarm_data,sizeof(doalarm_data));
		
	//向队列发送数据
	if(msgsnd(msgid,&alarm_data,sizeof(struct msg_st),0) == -1)
	{
		fprintf(stderr, "msgsnd failed\n");
		exit(EXIT_FAILURE);
	}
	printf("msgsend end ......\n");	
	exit(EXIT_SUCCESS);
	return 0;
}