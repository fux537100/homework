#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<stdbool.h>
#define MAXSIZE 5
#define MAXLEN 50
typedef struct tableHead{
        char    id[8];
        char    age[8];
        char    dept[16];
        char    name[32];
}TABLEHEAD,*LPTABLEHEAD;

typedef struct student{
        int     id;
        char    name[32];
        int     age;
        char    dept[16];
}STUDENT,*LPSTUDENT;

void PrintfStudent(STUDENT studentInfo){
        printf("%d\t",studentInfo.id);
        printf("%s\t",studentInfo.name);
        printf("%s\t\t",studentInfo.dept);
        printf("%d\n",studentInfo.age);
}

void PrintfTableHead(TABLEHEAD tableHead){
        printf("%s\t\t",tableHead.id);
        printf("%s\t\t",tableHead.name);
        printf("%s\t",tableHead.dept);
        printf("%s",tableHead.age);
}

bool readFile(FILE *fpRead,char token[][32]){
        char *lpCommaLocation, *lpHeadLocation;
        char buffer[MAXLEN];
        int i = 0;
        if(fgets(buffer,MAXLEN,fpRead)!=NULL){
                lpHeadLocation = buffer;
                for(lpCommaLocation = lpHeadLocation ; *lpCommaLocation != '\0'; lpCommaLocation++){
                        if (*lpCommaLocation == ','){ 
                                *lpCommaLocation = '\0';
                                strcpy(token[i],lpHeadLocation);
                                //          printf("%s\t",token[i]);
                                i++;
                                lpHeadLocation = lpCommaLocation+1;
                        }
                }
                strcpy(token[i],lpHeadLocation);
                //  printf("%s\n",token[i]);
                return true;
        }
        else
                return false;
}


void storeTableHead(TABLEHEAD *tableHead,char token[][32]){
        strcpy((*tableHead).id,token[0]);
        strcpy((*tableHead).name,token[1]);
        strcpy((*tableHead).dept,token[2]);
        strcpy((*tableHead).age,token[3]);
}

void storeInfo(STUDENT *studentInfo,char token[][32]){
        (*studentInfo).id = atoi(token[0]);
        strcpy((*studentInfo).name,token[1]);
        strcpy((*studentInfo).dept,token[2]);
        (*studentInfo).age = atoi(token[3]);
}

void stuHost2Net(STUDENT *studentInfo){
        (*studentInfo).id = ntohl((*studentInfo).id);
        (*studentInfo).age = ntohl((*studentInfo).age);
        //PrintfStudent(*studentInfo);
}

void stuNet2Host(STUDENT *studentInfo){
        (*studentInfo).id = htonl((*studentInfo).id);
        (*studentInfo).age = htonl((*studentInfo).age);
        //PrintfStudent(*studentInfo);
}

void writeBinFile(FILE *fpbin,STUDENT *studentInfo){
        if(fwrite(studentInfo,sizeof(STUDENT),1,fpbin)!= 1){
                printf("error write\n");
        }
}

int readAndStore(TABLEHEAD *tableHead,STUDENT *stu){
        FILE *fpcsv;
        int stuNum;
        char token[4][32];
        if((fpcsv=fopen("CUnixHomeWork6.csv","r"))==NULL){
                puts("csv can not be opened.");
                return 0;
        }
        //处理表头
        readFile(fpcsv,token);
        storeTableHead(tableHead,token);
        // PrintfTableHead(*tableHead);
        //存储学生信息
        stuNum = 0;
        while(readFile(fpcsv,token) == true){
                if(stuNum < MAXSIZE){
                        storeInfo(&stu[stuNum],token);
                        // PrintfStudent(stu[stuNum]);
                        stuNum++;
                }
                else {
                        puts("store full");
                        break;
                }
        }
        fclose(fpcsv);

        return stuNum;
}


int writeTable(TABLEHEAD *tableHead,STUDENT *stu,int stuNum){
        FILE *fpbin;
        if((fpbin=fopen("CUnixHomeWork6.bin","wb"))==NULL){
                printf("bin can not be created.\n");
                return 0;
        }
        //写表头
        if(fwrite(tableHead,sizeof(TABLEHEAD),1,fpbin)!= 1){
                printf("error write\n");
        }
        //写数据
        int i; 
        for(i = 0; i < stuNum; i++){
                stuHost2Net(&stu[i]);
                writeBinFile(fpbin,&stu[i]);
                //      printf("%d\n",i);
        }

        fclose(fpbin);
        return 1;
}


void PrintfBinFile(){
        FILE *fpbin;
        TABLEHEAD tableHeadBin;
        STUDENT stuBin;
        if((fpbin=fopen("CUnixHomeWork6.bin","rb"))==NULL){
                printf("bin can not be opened.\n");
                return ;
        }

        if(fread(&tableHeadBin,sizeof(TABLEHEAD),1,fpbin) != 1){
                printf("bin file read tablehead error\n");
                return ;
        }
        PrintfTableHead(tableHeadBin);

        while(fread(&stuBin,sizeof(STUDENT),1,fpbin) == 1){
                stuNet2Host(&stuBin);
                PrintfStudent(stuBin);
        }

        fclose(fpbin);        
        return ;
}

int main(){

        int stuNum = 0;
        TABLEHEAD tableHead;
        STUDENT stu[MAXSIZE];
        //读取CSV文件中的信息，返回学生数长度
        stuNum =  readAndStore(&tableHead,stu);

        //写二进制文件
        if(writeTable(&tableHead,stu,stuNum) == 0){
                puts("write file error");
                return 0;
        }

        //读二进制文件，并打印
        PrintfBinFile();

        return 0;
}
