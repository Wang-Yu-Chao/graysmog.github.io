#include <stdio.h>
// #include <windows.h>
#include <stdlib.h>
// #include <process.h>

typedef struct ConfigInfo {
	int MinCheck;
	int MaxCheck;
	int MaxCustSingleLine;
	int MaxLines;
	int MaxSeqLen;
	int EasySeqLen;
	int MaxCustCheck;
	int MaxSec;
}ConfigInfo;

ConfigInfo DEFAULT = {0,0,0,0,0,0,0,0};

void init();
void configDisplay(ConfigInfo *info);

/* int main(void)
{
	init();
	while (UNFINISHED) {
		get_input();
		control();
		state_trans();
		print_message();
		time_count();
	}
	printf("Finished.\n");

	return 0;
}*/

int main() {
	init();
	printf("adsa");     //注释掉就能运行，但是运行完会崩一下  如果不注释掉就直接崩
	return 0;
}

void init() {
	FILE *fp;
	if ((fp = fopen("config.dat","ab")) == NULL) {
		printf("ERROR:Can't Open The File!\n'");
	}
	else {
		ConfigInfo *info;
        fwrite(&DEFAULT,sizeof(ConfigInfo),1,fp);
        fread(info,sizeof(ConfigInfo),1,fp);
        printf("The config information is following:\n");
        configDisplay(info);
        printf("Please input your change (for testing so if you don't change it,please input the origin data):\n");
        printf("MinCheck: ");scanf("%d",&info->MinCheck);
        printf("MaxCheck: ");scanf("%d",&info->MaxCheck);
        printf("MaxCustSingleLine: ");scanf("%d",&info->MaxCustSingleLine);
        printf("MaxLines: ");scanf("%d",&info->MaxLines);
        printf("MaxSeqLen: ");scanf("%d",&info->MaxSeqLen);
        printf("EasySeqLen: ");scanf("%d",&info->EasySeqLen);
        printf("MaxCustCheck: ");scanf("%d",&info->MaxCustCheck);
        printf("MaxSec: ");scanf("%d",&info->MaxSec);
        printf("Now,the config information is following:\n");
        configDisplay(info);

        fwrite(info,sizeof(ConfigInfo),1,fp);
        printf("Update Finished\n");
        fclose(fp);
	}
	return ;
}

void configDisplay(ConfigInfo *info) {
	printf("----------Config Information---------\n");
	printf("-------------------------------------\n");
	printf("MinCheck: %d\n",info->MinCheck);
	printf("MaxCheck: %d\n",info->MaxCheck);
	printf("MaxCustSingleLine: %d\n",info->MaxCustSingleLine);
    printf("MaxLines: %d\n",info->MaxLines);
    printf("MaxSeqLen: %d\n",info->MaxSeqLen);
    printf("EasySeqLen: %d\n",info->EasySeqLen);
    printf("MaxCustCheck: %d\n",info->MaxCustCheck);
    printf("MaxSec: %d\n",info->MaxSec);
    printf("-------------------------------------\n");
}
