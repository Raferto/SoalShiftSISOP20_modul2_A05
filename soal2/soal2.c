#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char** argv){
	pid_t pid, sid;

  	pid = fork();

  	if (pid < 0) {
    		exit(EXIT_FAILURE);
  	}

  	if (pid > 0) {
    		exit(EXIT_SUCCESS);
  	}

	umask(0);

  	sid = setsid();
  	if (sid < 0) {
    		exit(EXIT_FAILURE);
  	}

	if ((chdir("/home/raferto/Documents/4. Sisop/Praktikum 2/soal2")) < 0) 
		exit(EXIT_FAILURE);


	time_t t,epoch;
	struct tm *tmp;
	char date[50],download[100],simpan[100],temp[50];
	long int i,e;
	int status;
	pid_t child_id;

  	close(STDIN_FILENO);
  	close(STDOUT_FILENO);
  	close(STDERR_FILENO);

	if(argc > 1){
    	if(argv[1][1]=='a'){
	    	child_id = fork();
	    	if(child_id == 0){
			FILE *f;
			f = fopen ("Killer.c", "w");
			fprintf(f, "#include<stdio.h>\n");
			fprintf(f, "#include<stdlib.h>\n");
			fprintf(f, "#include<sys/types.h>\n");
			fprintf(f, "#include<sys/stat.h>\n");
			fprintf(f, "#include<fcntl.h>\n");
			fprintf(f, "#include<errno.h>\n");
			fprintf(f, "#include<wait.h>\n");
			fprintf(f, "#include<unistd.h>\n");
			fprintf(f, "#include<syslog.h>\n");
			fprintf(f, "#include<string.h>\n\n");
			fprintf(f, "int main(){\n");
			fprintf(f, "int status;\n");
			fprintf(f, "pid_t child_id = fork();\n");
			fprintf(f, "if(child_id){\n");
			fprintf(f, "while ((wait(&status)) > 0);\n");
			fprintf(f, "char *argv[] = { \"rmv\",\"./Killer\", NULL};\n");
			fprintf(f, "execv(\"/bin/rm\", argv);\n");
			fprintf(f, "}\n");
			fprintf(f, "else{\n");
			fprintf(f, "char *argv[] = { \"pkill\",\"soal2\", NULL};\n");
			fprintf(f, "execv(\"/usr/bin/pkill\", argv);\n");
			fprintf(f, "}\n");
			fprintf(f, "}\n");
			fclose(f);

			char *argv[] = { "gcc","./Killer.c","-o","./Killer", NULL };
			execv("/usr/bin/gcc", argv);
	    	}

	    	while ((wait(&status)) > 0);
	    	child_id = fork();
	    	if(child_id == 0){
	    		char *argv[] = { "rmv","./Killer.c", NULL };
			execv("/bin/rm", argv);
	    	}
	    }
	    else if(argv[1][1]=='b'){
	    	child_id = fork();
	    	if(child_id == 0){
			FILE *f;
			f = fopen ("Killer.c", "w");
			fprintf(f, "#include<stdio.h>\n");
			fprintf(f, "#include<stdlib.h>\n");
			fprintf(f, "#include<sys/types.h>\n");
			fprintf(f, "#include<sys/stat.h>\n");
			fprintf(f, "#include<fcntl.h>\n");
			fprintf(f, "#include<errno.h>\n");
			fprintf(f, "#include<wait.h>\n");
			fprintf(f, "#include<unistd.h>\n");
			fprintf(f, "#include<syslog.h>\n");
			fprintf(f, "#include<string.h>\n\n");
			fprintf(f, "int main(){\n");
			fprintf(f, "int status;\n");
			fprintf(f, "pid_t child_id = fork();\n");
			fprintf(f, "if(child_id){\n");
			fprintf(f, "while ((wait(&status)) > 0);\n");
			fprintf(f, "char *argv[] = { \"rmv\",\"./Killer\", NULL};\n");
			fprintf(f, "execv(\"/bin/rm\", argv);\n");
			fprintf(f, "}\n");
			fprintf(f, "else{\n");
			fprintf(f, "char *argv[] = { \"kill\",\"%d\", NULL};\n",sid);
			fprintf(f, "execv(\"/bin/kill\", argv);\n");
			fprintf(f, "}\n");
			fprintf(f, "}\n");
			fclose(f);

			char *argv[] = { "gcc","./Killer.c","-o","./Killer", NULL };
			execv("/usr/bin/gcc", argv);
	    	}

	    	while ((wait(&status)) > 0);
	    	child_id = fork();
	    	if(child_id == 0){
	    		char *argv[] = { "rmv","./Killer.c", NULL };
			execv("/bin/rm", argv);
	    	}	
	    }
    }

	while (1){
		child_id = fork();
		if(child_id == 0){
			time(&t);
			tmp = localtime(&t); 
			strftime(date, sizeof(date), "%F_%X", tmp);

			child_id = fork();

			if(child_id == 0){
				sprintf(date,"%s/",date);
			    char *argv[] = {"mkdir", date, NULL};
				execv("/bin/mkdir", argv);
			}
			else{
				while ((wait(&status)) > 0);
				for(i=0;i<20;i++){
					child_id = fork();
					if(child_id == 0){
						epoch = time(NULL);
						e = (epoch%1000)+100;
						sprintf(download,"https://picsum.photos/%ld/%ld",e,e);
						time(&t);
						tmp = localtime(&t); 
						strftime(temp, sizeof(temp), "%F_%X", tmp);

						sprintf(simpan,"%s/%s",date,temp);

						char *argv[] = {"wget",download,"-O",simpan, NULL};
						execv("/usr/bin/wget", argv);
					}
					else;
						sleep(5);
				}

				while ((wait(&status)) > 0);
				child_id = fork();

				if(child_id == 0){
					sprintf(temp,"%s.zip",date);
					char *argv[] = {"zip","-r",temp,date, NULL};
					execv("/usr/bin/zip", argv);
				}

				while ((wait(&status)) > 0);
				char *argv[] = {"rmv","-r",date, NULL};
				execv("/bin/rm", argv);
			}
		}
		else{
			sleep(30);
		}
	}


}
