#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<getopt.h>
//#include<readline/readline.h>
//#include<readline/history.h>
#include<limits.h>
#include"functions.h"

  
int main(int argc,char **argv){
    
    char *buffer;
    size_t bufsize = 32;
    size_t line;
    buffer = (char *)malloc(bufsize * sizeof(char));
    char buffer2[200];
    FILE *p;
    int background=0;
    int pipe=0;
    

    while(1){
        //prompt
        signals_off();
         if(argc>1)
         {
            //batchfile
            p=fopen("/home/oem/Desktop/test","r");
            while(feof(p)==0)
            {
                if(fgets(buffer2,200,p))
                {
                    for(int i=0;i<strlen(buffer2);i++)
                    {
                        if(buffer2[i]=='&')
                        {
                        printf("%s\n","BACKGROUND");
                        background=1;
                        strtok(buffer2,"&");
                        //printf("%s\n",buffer2);
                        break;
                        }
                    }
                    //printf("%s",buffer2);
                    read_command(buffer2);
                }continue;
            }break;
        }

        type_prompt();
        line=getline(&buffer,&bufsize,stdin);
        strcpy(buffer2,buffer);
    
        if(strncmp("quit",buffer2,4)==0){
            exit(EXIT_SUCCESS);
        }else{
            read_command(buffer2);
            } 
        }
    return 0;
    }
    




