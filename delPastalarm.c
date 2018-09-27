#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "szhy_get.h"

//存在已过去闹钟 返回0  不存在 返回-1
int check_past_alarm(char *line_str)
{	
	/*******解析bngo接受到的json数据********/	
	char *token;	
	int i = 0;
	char alarmDay[5] = {0};
	char alarmMon[5] = {0};
	char alarmId[64] = {0};
	char topic[20] = {0};
	char Timestamps[20] = {0};

	for(token = strtok(line_str, " "); token != NULL; token = strtok(NULL, " ")) {    
        //printf(token);
		if(i == 2)
		 	strcpy(alarmDay,token);
		else if(i == 3)
			strcpy(alarmMon,token);
		else if(i == 6)
			strcpy(alarmId,token);
		else if(i == 9)
			strcpy(topic,token);  
		else if(i == 10)
			strcpy(Timestamps,token); 		
        i++;    
    }
	//判断是否缺少参数
	if(i != 11)
		return -1;

	//printf("day:%s mon:%s alarmId:%s topic:%s Timestamps:%s\n",alarmDay,alarmMon,alarmId,topic,Timestamps);

	//判断 是否单次闹钟 月 日 是否为*
	if(strcmp(alarmDay,"*") && strcmp(alarmMon,"*")){
			//获取当前时间戳
			time_t timep;
			//struct tm *p;
			time(&timep); /*当前time_t类型UTC时间*/
			//printf("time():%ld/n",timep);
			
			if(atol(Timestamps) > timep){
				return -1;				
			} else {
				if(strcmp(topic,"cozy") == 0){
					//生成舒适睡眠的文件路径
					char cozySleepPath[50] = {0};
					sprintf(cozySleepPath,"%s%s%s","/usr/data/",alarmId,".cozy");
					if(szhy_get("del_cozy",alarmId) != 0)
						return -1;
					
					remove(cozySleepPath);
				} else if(strcmp(topic,"bgalarm") == 0 || strcmp(topic,"apalarm") == 0){
					if(szhy_get("del_alarm",alarmId) != 0)
						return -1;
				}
			}

	} else {
		return -1;
	}
	return 0;
}

int main(int argc,char *argv[])
{
	//char s[] = "24 10 22 04 1,2,3,4,5,6,0 /usr/fs/usr/sbin/doalarm 272 DEL 1 bgalarm";
	//check_past_alarm(s);	
#if 1
	char* file_from = "/var/spool/cron/crontabs/root";
    char* file_to = "/var/spool/cron/crontabs/root.new";

	if(access(file_from,F_OK) != 0)
		return 0;
	
    FILE *fpf, *fpt;
    char line_str[100];
	char root_str[100];
    fpf = fopen(file_from, "r+");
    fpt = fopen(file_to, "w+");
    if(fpf == NULL || fpt == NULL)
    {
        printf("open file error!\n");
        return 2;
    }
	//获取一行
    while((fgets(line_str,100,fpf)) != NULL)
    {	
        //if(strstr(line_str,alarmId) == NULL)
           // fprintf(fpt, "%s", line_str);
		//check past alarm
		strcpy(root_str,line_str);
		if(check_past_alarm(root_str) != 0)
			 fprintf(fpt, "%s", line_str);
    }
    fclose(fpf);
    fclose(fpt);

	rename(file_to,file_from);
#endif
	return 0;
}
