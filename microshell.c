#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define COLOR   "\x1B[33m"
#define RESET "\x1B[0m"

void help()											
{
	printf("**Projekt MicroShell**\n");
	printf("Program zawiera wlasna implementacje polecenia rm (remove), polecenia mv(move) oraz polecenia cd(change directory)\n");
	printf("Autor:Aleksandra Zdybal\n");	
	
}

int main()
{
char command[BUFFER_SIZE];

char kor[] = " ";
char kor2[]="\0";
char kor3[]="/";

char * sch;
char * sch2;
char * sch3 = NULL;
char * sch4 = NULL;

int err, err2;
int cd, dire;
char s[2048];
char buf[BUFFER_SIZE];
char com;
pid_t pot=1;

int fd_in, fd_out, n, fd_in2, fd_out2;
char *directory = 0;
char *directory2 = 0;

char A[100][100];
int i=0;

umask(0);

while((strcmp(command, "exit")) != 0) 
{

	printf("[%s"COLOR ":%s"RESET"]",getenv("USER"), getcwd(s, 2048));
	printf("$ ");
	
	fgets(command,sizeof(command),stdin);                              											
	command[strcspn(command, "\n")] = '\0';
	
	if(strcmp(command, "help") ==0 ) help();
   
   
    	sch = strtok( command, kor );
   	sch2 = strtok( NULL, kor2 );
    
if(sch != NULL)
{
  directory = getcwd(s, 2048);
  
  if(strcmp(sch, "cd") == 0)      												                                                          								 
   {
   	if(sch2 != NULL)
   	{		
   				
   		if(strcmp(sch2, "~") == 0)
   		{ 
   			cd=chdir(getenv("HOME"));
   			strcpy(A[i],directory);
 			i=i+1;
   			directory = getcwd(s, 2048);
   			if(cd != 0) perror("Error");

   		}
   		if(strcmp(sch2, "-") == 0) 
   		{
   			cd=chdir(A[i-1]);
   			if(cd != 0) perror("Error");
   		}					     								
   		if((strcmp(sch2, "~") != 0) && (strcmp(sch2, "-") != 0))
   		{
   		 	cd=chdir(sch2);
   		 	strcpy(A[i],directory);
 			i=i+1;
   		 	directory = getcwd(s, 2048);
   		 	if(cd != 0) perror("Error");
		}	
   	}
   	else chdir(getenv("HOME"));
   	
   }
 
   if(strcmp(sch, "mv") == 0)                                                                 								
   {
   	sch2 = strtok( sch2, kor);
   	sch3 = strtok( NULL, kor);
   	sch4 = strtok( NULL, kor);
   	
   	
   	if((strcspn( sch2, "/" ) == strlen(sch2)) && (strcspn( sch3, "/" )) == strlen(sch3))      								
   	{
   		if(sch4 == NULL) 
   		{
   			com=rename(sch2, sch3);   								
   			if(com != 0) perror("Error");        										
   		}
   		
   		if((sch4 != NULL) && ((strcspn( sch4, "/" )) != strlen(sch4)))        											
   		{
				directory = getcwd(s, 2048); 
				err = fd_in  = open(sch2, O_RDWR);
				err2 = chdir(sch4);
				
				
				if((err != -1) && (err2 == 0))
				{
					fd_out = open(sch2, O_RDWR | O_TRUNC | O_CREAT, 0666);

					while((n = read(fd_in, &buf, BUFFER_SIZE)) > 0)
					{
						write(fd_out, &buf, n);
					}
					chdir(directory);
					remove(sch2);
				}
				else perror("Error");
				chdir(directory);
		
			
			
				err = fd_in2  = open(sch3, O_RDWR);
				err2 = chdir(sch4);
				
				
				if((err != -1) && (err2 == 0))
				{
					fd_out2 = open(sch3, O_RDWR | O_CREAT | O_TRUNC, 0666);

					while((n = read(fd_in2, &buf, BUFFER_SIZE)) > 0)
					{
						write(fd_out2, &buf, n);
					}
					chdir(directory);
					remove(sch3);
				}
				else perror("Error");
				chdir(directory);
   		}                                                     
   	}
   	
   	if((com=strcspn( sch2, "/" )) != strlen(sch2) && (com=strcspn( sch3, "/" )) != strlen(sch3))  					
   	{
   		directory = getcwd(s, 2048);
   		err = chdir(sch3);
   		if(err != 0) rename(sch2, sch3);
		else perror("Error");
   		
   	}
   	
   	
   	if((strcspn( sch2, "/" ) == strlen(sch2)) && (strcspn( sch3, "/" )) != strlen(sch3))          					
   	{
   		directory = getcwd(s, 2048);
   		err = fd_in  = open(sch2, O_RDWR);
		err2 = chdir(sch3);
		if((err != -1) && (err2 == 0))
		{
			fd_out = open(sch2, O_RDWR | O_CREAT | O_TRUNC, 0666);
			
			while((n = read(fd_in, &buf, BUFFER_SIZE)) > 0)
			{
				write(fd_out, &buf, n);
			}
			chdir(directory);
			remove(sch2);
		}
		else perror("Error");
		chdir(directory);
   	}

   }
   
   if(strcmp(sch, "rm") == 0)                                                               								 
   {
   	sch2 = strtok( sch2, kor);
   	sch3 = strtok( NULL, kor);
   	
   	if((strcspn( sch2, "/" ) == strlen(sch2)) && (sch3 == NULL))                      							
   	{
   		fd_in  = open(sch2, O_RDWR);
  		err = remove(sch2);
   		if(err != 0) perror("Error");
   	}
   	if((strcspn( sch2, "/" ) < strlen(sch2)) && (strcspn( sch2, "/" ) != strlen(sch2)+1))                    				 
   	{
   		directory = getcwd(s, 2048);
   		sch2 = strtok( sch2, kor3);
   		sch3 = strtok( NULL, kor);
   		
   		
   			err = chdir(sch2);
   			if(err != 0) perror("Error");
   			else
   			{
   				if((fd_in  = open(sch3, O_RDWR) != -1))
   				{
   				remove(sch3);
     				}
     				else perror("Error");
     				chdir(directory);
     			}
     		
   	}
   	if(strcmp(sch2, "-r") == 0)												
   	{
   		
   		DIR *dir;
		struct dirent *entr;

		if ((dir = opendir(sch3)) == 0) perror("Error");
		else
		{
			while((entr = readdir(dir)) != NULL)
			{
				directory = getcwd(s, 2048);
				chdir(sch3);
				fd_in  = open(entr->d_name, O_RDWR);
				remove(entr->d_name);
				chdir(directory);
			}
			closedir(dir);
			rmdir(sch3);
		}
   	}                                
   
   }


   if ((strcmp(sch, "help") != 0) && (strcmp(sch, "exit") != 0) && (strcmp(sch, "cd") != 0) && (strcmp(sch, "mv") != 0) && (strcmp(sch, "rm") != 0)) 
    {
    	
    	pot = fork();
    	
    	if(pot == 0)
    	{
        	char* const av[] = {sch, sch2, NULL};
        	com = execvp(sch, av);
        	if(com == -1)
        	{
        		printf("Nie znaleziono polecenia\n");
        		printf("Error: %d\n", errno);
            		exit(EXIT_FAILURE);
       	}
       }	
        
       else
       {
        	wait(0);
       }
    }	
}
}
return 0;
}

