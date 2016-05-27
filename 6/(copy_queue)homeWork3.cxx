#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include<stdbool.h>
typedef struct student{
    char    id[32];
    char    name[32];
    char    age[16];
    char    dept[16];
}STUDENT,*LPSTUDENT;

typedef struct studentnode{
    STUDENT studentInfo;
    struct studentnode *lpNext;
}TSTUDENTNODE,*LPTSTUDENTNODE;

typedef struct studentqueue{
    LPTSTUDENTNODE lpHead;
    LPTSTUDENTNODE lpRear;
    int nSize;
}TSTUDENTQUEUE,*LPTSTUDENTQUEUE;

void InitQueue(TSTUDENTQUEUE **lpStudentQueue){
    *lpStudentQueue = ( LPTSTUDENTQUEUE ) malloc (sizeof(TSTUDENTQUEUE));
    (*lpStudentQueue)->lpHead = (LPTSTUDENTNODE) malloc (sizeof(TSTUDENTNODE));
    (*lpStudentQueue)->lpRear = (*lpStudentQueue)->lpHead;
    (*lpStudentQueue)->lpHead->lpNext = NULL;
    (*lpStudentQueue)->nSize = 0;
}

void EnQueue(TSTUDENTQUEUE*lpStudentQueue, STUDENT *studentInfo){
    LPTSTUDENTNODE lpStudentNode = (LPTSTUDENTNODE)malloc(sizeof(TSTUDENTNODE));
    lpStudentNode->studentInfo = *studentInfo;
    lpStudentQueue->lpRear->lpNext = lpStudentNode;
    lpStudentQueue->lpRear = lpStudentQueue->lpRear->lpNext;
    lpStudentQueue->nSize++;
}

void DeQueue(TSTUDENTQUEUE*lpStudentQueue, STUDENT *studentInfo){
    LPTSTUDENTNODE lpTemp = lpStudentQueue->lpHead->lpNext;
    *studentInfo = lpTemp->studentInfo;
    lpStudentQueue->lpHead->lpNext = lpTemp->lpNext;
    free(lpTemp);
    lpStudentQueue->nSize--;
}

bool IsQueueEmpty(LPTSTUDENTQUEUE lpStudentQueue){
    return lpStudentQueue->nSize == 0;
}

void DestroyQueue(TSTUDENTQUEUE*lpStudentQueue){
    LPTSTUDENTNODE lpNextCarNode = NULL;
    LPTSTUDENTNODE lpStudentNode = lpStudentQueue->lpHead;
    for(;lpStudentNode != NULL;lpStudentNode = lpNextCarNode){
        lpNextCarNode = lpStudentNode->lpNext;
        free(lpStudentNode);
    }
    free(lpStudentQueue);
    lpStudentQueue = NULL;
}

void PrintfStudent(STUDENT studentInfo){
    printf("%s\t",studentInfo.id);
    printf("%s\t",studentInfo.name);
    printf("%s\t",studentInfo.dept);
    printf("%s\n",studentInfo.age);
}

int main(){
    
    char buffer[1000];
    char *lpCommaLocation, *lpHeadLocation;
    char token[4][32];
//    STUDENT student1 = {NULL};
    FILE *fpcsv,*fpbin;
    
    if((fpcsv=fopen("CUnixHomeWork3.csv","r"))==NULL){
        puts("csv can not be opened.");
        return 0;
    }
    //初始化队列，队列的表头不存任何信息
    LPTSTUDENTQUEUE lpStudentQueue ; 
    InitQueue(&lpStudentQueue);
     
     while(fgets(buffer,50,fpcsv)!=NULL){
            STUDENT studentInfo;
            lpHeadLocation = buffer;
            int i = 0;
            for(lpCommaLocation = lpHeadLocation ; *lpCommaLocation != '\0'; lpCommaLocation++){
                if (*lpCommaLocation == ','){ 
                    *lpCommaLocation = '\0';
                    strcpy(token[i],lpHeadLocation);
    //                printf("%s\t",token[i]);
                    i++;
                    lpHeadLocation = lpCommaLocation+1;
                }
            }
            strcpy(token[i],lpHeadLocation);
    //        printf("%s\n",token[i]);
            strcpy(studentInfo.id,token[0]);
            strcpy(studentInfo.name,token[1]);
            strcpy(studentInfo.dept,token[2]);
            strcpy(studentInfo.age,token[3]);
//            PrintfStudent(studentInfo);
            EnQueue(lpStudentQueue,&studentInfo);
     //       puts("error1");
     }
     
    if((fpbin=open("CUnixHomeWork3.bin","wb"))==NULL){
        printf("bin can not be opened.\n");
        return 0;
    }

     while(false == IsQueueEmpty(lpStudentQueue)){
        STUDENT tmpStudent; 
        DeQueue(lpStudentQueue,&tmpStudent);
 //       PrintfStudent(tmpStudent);
 //       printf("%s\txx",(char*)&tmpStudent);
        printf("%d\n",fpbin);
        int res = 0;
        if(res = write(fpbin,tmpStudent,sizeof(tmpStudent))!= sizeof(tmpStudent)){
    //        write(fpbin,tmpStudent,sizeof(tmpStudent));
              printf("error write\n");
        }
     };
//    int i;
//    int binBuffer[1000]={0};
//    while(fgets(buffer,30,fpcsv)!=NULL){
//        for(i = 0; i < 1000;i++){
//            printf("%d",binBuffer[i]);
//            if(buffer[i] == '\n') break;
//            binBuffer[i] = strtol(buffer[i],NULL,2);
//            printf("%d",binBuffer[i]);
//        }
//        fwrite(binBuffer,sizeof(binBuffer),1,fpbin);
//    }
//    fread()
    fclose(fpcsv);
    close(fpbin);
    return 0;
}
