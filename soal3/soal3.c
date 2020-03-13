#include <dirent.h>
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/stat.h>
#include <sys/wait.h>

int main() 
{ 
    
    int pid1 = fork();
    int pid2 = fork(); 
    pid_t pid3, pid4, pid5, pid6, pid7;
    int status;

    close(STDERR_FILENO);

    if (pid1 == 0 && pid2 > 0) 
    { 
        while ((wait(&status)) > 0);
        pid7= fork();
        if (pid7 == 0) 
        {
            char *argv[] = {"mkdir", "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/indomie", NULL};
            execv("/bin/mkdir", argv);
        }
        else
        {
            while ((wait(&status)) > 0);
            sleep(5);

            char *argv[] = {"mkdir", "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/sedaap", NULL};
            execv("/bin/mkdir", argv);
        }
    } 
    if (pid1 > 0 && pid2 == 0) 
    { 
        char *argv[] = {"unzip", "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/jpg.zip", NULL};
        execv("/usr/bin/unzip", argv);
    }

    if (pid1 > 0 && pid2 > 0) 
    { 
        while ((wait(&status)) > 0);

        DIR *dir=opendir("/home/bluejake3/SISOP/Modul2/Praktikum/soal3/jpg/");
        struct dirent *entry;
        if (dir)
        {
            while ((entry = readdir(dir)) != NULL)
            {
                if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

                if(entry->d_type==DT_DIR)
                {
                    pid3=fork();
                    if(pid3== 0)
                    {
                        char folder[1000];
                        sprintf(folder, "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/jpg/%s", entry->d_name);

                        char *argv[] = {"mv", folder, "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/indomie", NULL};
                        execv("/bin/mv", argv);
                    }
                    else
                    {
                        while ((wait(&status)) > 0);
                        pid4=fork();
                        if(pid4 == 0)
                        {
                            pid5=fork();
                            if(pid5 == 0)
                            {
                                char coba1[1000];
                                FILE *cb1;
                                sprintf(coba1, "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/indomie/%s/coba1.txt", entry->d_name);
                                cb1 = fopen(coba1, "w");
                                fclose(cb1);
                            }
                            else
                            {
                                while ((wait(&status)) > 0);
                                sleep(3);
                                char coba2[1000];
                                FILE *cb2;
                                sprintf(coba2, "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/indomie/%s/coba2.txt", entry->d_name);
                                cb2 = fopen(coba2, "w");
                                fclose(cb2);
                                exit(0);
                            }
                        }
                        else
                        {
                            while ((wait(&status)) > 0);
                            exit(0);
                        }
                    }
                }
                else
                {
                    while ((wait(&status)) > 0);
                    pid6=fork();
                    if(pid6 == 0)
                    {
                        while ((wait(&status)) > 0);
                        char file[1000];
                        sprintf(file, "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/jpg/%s", entry->d_name);

                        char *argv[] = {"mv", file,"/home/bluejake3/SISOP/Modul2/Praktikum/soal3/sedaap", NULL};
                        execv("/bin/mv", argv);
                    }
                }        
            }
        }
        closedir(dir);
    } 
    return 0; 
} 