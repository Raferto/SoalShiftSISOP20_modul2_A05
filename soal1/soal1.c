#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) 
{
    if(argc!=5)
    {
        printf("Input salah\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        int arr[4], f;
    
        for(int i=1;i<4;i++)
        {
            char str[sizeof(argv[i])+1];
            strcpy(str,argv[i]);
            if(strncmp(argv[i],"*",1)==0)
            {
                arr[i]=-1;
            }
            else
            {
                f=0;
                for(int k=0; k<strlen(str);k++)
                {
                    if(str[k]>='0' && str[k]<='9')
                    f++;
                }
                if(f==strlen(str))
                {
                    arr[i]=atoi(argv[i]);
                    
                }
                else
                {
                    printf("Input salah\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
        if(arr[1]<-1 || arr[1]>59 || arr[2]<-1 || arr[2]>59 || arr[3]<-1 || arr[3]>23)
        {
            printf("Input salah\n");
            exit(EXIT_FAILURE);
        } 

        pid_t pid, sid;        // Variabel untuk menyimpan PID

        pid = fork();     // Menyimpan PID dari Child Process

        /* Keluar saat fork gagal
        * (nilai variabel pid < 0) */
        if (pid < 0) {
        exit(EXIT_FAILURE);
        }

        /* Keluar saat fork berhasil
        * (nilai variabel pid adalah PID dari child process) */
        if (pid > 0) {
        exit(EXIT_SUCCESS);
        }

        umask(0);

        sid = setsid();
        if (sid < 0) {
        exit(EXIT_FAILURE);
        }

        if ((chdir("/home/bluejake3/SISOP/Modul2/Praktikum/soal1/")) < 0) {
        exit(EXIT_FAILURE);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        while (1) 
        {
            // Tulis program kalian di sini
            time_t waktu;
            struct tm *jam;
        
            waktu=time(NULL);
            jam=localtime(&waktu);

            if ((jam->tm_sec == arr[1] || arr[1] == -1) && (jam->tm_min == arr[2] || arr[2] == -1) 
            && (jam->tm_hour == arr[3] || arr[3] == -1))
            {

                int status;
                pid_t pid;
                pid=fork();
                
                if(pid==0)
                {
                    char *a[]={"bash", argv[4], NULL};
                    execv("/bin/bash", a);
                }
            }
            sleep(1);
        }
    }
}


