// 10/2/2026/Kaine Branch/U45775211/ This program is to simulate a simple shell that allows the user to path to different files in each directory, use ls to see all files in the directory and be able to execute games using a command line

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
//macros set to be 64
#define MAX_DIRS 64



//holds the paths that are entered
char *repo_path[MAX_DIRS];



void execute_games(char *tokens[]);



void error(){



	//error message sent when there is an error
	char error_message[] = "An error has occurred\n";



	//prints message coming from stderr(standard error)
	write(STDERR_FILENO,error_message,strlen(error_message));



	//makes solution prin tthings in expected order
	fflush(stderr);

}




//compares the strings in qsort
int compare(const void *a, const void *b){
	return strcmp(*(char**)a,*(char **)b);
}



void process_command(char *line, char *argv[]){
	


	//stores the tokens created from each commmand line
	char *tokens[100];



	//count is for whats typed in command line
	int count = 0;



	//stores command line to be separated from  \t\n
	char *ptr = line;



	//represents each separated word to be stored into tokens
	char *token;
	
	
	
	
	//cycles throuhg the lines coming from argv and stores them into tokens with its words separated with \t\n
	while((token = strsep(&ptr," \t\n"))!=NULL){
	
	
	//if the token equal to null it keeps the loop going
		if(*token == '\0')
			continue;
	
	
	
		//adds the command line into tokens and gets split with \t\n
		tokens[count++] = token;
	
	}
	
	
	
	
	
	//tries again if nothing is typed into gsh>
	if(count == 0)
		return;
	
	
	
	//makes last entry in tokens null
	tokens[count] = NULL;
	

	
	
	if(strcmp(tokens[0],"exit")==0){
	
	
	
		//sends error if there is more than exit entered
		if(count != 1){


			//prints error when exit isn't the only word
			error();
	
	
			//returns to prompt
			return;
		}
	
	
	
		//exits program
		exit(0);
	}
	
	


	if(strcmp(tokens[0],"path")==0){
		
		
		
		//sends error if there is more words than path and path destination
		if(count !=2){
		
		
			//sends error if there isnt word path then the full path written
			error();
		
		
		
			return;
		}
		
		
		
		//opens the directory
		DIR *dir = opendir(repo_path[0]);


		//frees and saves path when using path command
		repo_path[0] = strdup(tokens[1]);

		

		
		//checks to see if the directory is found
		if(dir == NULL){
		
			
			
			//error if directory isn't found
			error();
		



			//returns to prompt
			return;
		}	
		
		
		
		//closes directory once done
		closedir(dir);
		
		

		//returns to prompt
		return;

					
	}	

	if(strcmp(tokens[0],"ls")==0){
		
		
		//if not only ls then error is printed
		if(count != 1){
			error();
			return;
		}	
		
		
		
		//opens directory with path coming from repo_path
		DIR *dir = opendir(repo_path[0]);
		
		
		
		//checks to see if the directory opened
		if(dir == NULL){
		
		
		//sends error if directory cant be opened
			error();
		
		
		//returns to prompt
			return;



		}
		
		
		
		//holds info for a directory files names and directory name 	
		struct dirent *entry;

		
		
		//holds the paths name
		char *files[1024];

		
		
		//counter for how many files are being store
		int count = 0;
	



		//loops through directory
		while((entry = readdir(dir)) != NULL){
			
			
			
			//compares the entry to see if its at the current director and parent directory
		       	if (strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
			       continue;
			
			
			//stores each file into the array of strings
			files[count++] = strdup(entry->d_name);
			
	
		}
		//closes the dir
		closedir(dir);
		
		
		//sorts the files in order
		qsort(files,count,sizeof(char *),compare);
		


		for(int i = 0; i < count;i++){
			
			
			//stores the fullpath from command line
			char fullpath[1024];
			
			
			//puts the full path into the fullpath variable
			snprintf(fullpath,sizeof(fullpath),"%s/%s",repo_path[0],files[i]);
			


			//creates the child and parent process
			pid_t pid = fork();



			if(pid == 0){
			
			
			
				//stores the names of the files into a file,O_WRONLY is used for wriitng, O_CREATE creates file if there isnt one, O_TRUNC empties the file of old things stored, and 0644 is the permissions to read and write in file
				int fd = open("help.txt", O_WRONLY | O_CREAT| O_TRUNC,0644);
			
			
			
				//this redirects the stdout to the file
				dup2(fd,STDOUT_FILENO);
			
			
			
				//closes the file
				close(fd);
			
			
			
				//array of strings containing the command line arguments
				char *args[] = {files[i],"--help",NULL};
		
		
		
				//executes game with --help to get the description
				execvp(fullpath,args);
		
		
				//ends the program
				exit(1);
			}
		
		
		
			//makes parent process wait until the child process is finished
			waitpid(pid,NULL,0);
		
		
		
			//opens up a file that saves the description of each game
			FILE *fp = fopen("help.txt","r");
		
		
		
			//stores the description
			char description[1024];

		
		
			//checks to see if the file is found and grabs the description from the file to be placed into description array
			if(fp && fgets(description,sizeof(description),fp)){

				
				//saves the description into the description array and the \n are replaced with null character
				description[strcspn(description,"\n")] = '\0';
			
			
			
				//prints in the files array of strings and description array
				printf("%s: %s\n",files[i],description);
			}
			
			
			
			else{
			
			
			
				//any files that doesnt have a description prints here
				printf("%s (empty)\n",files[i]);
			}
			
			
			
			//cloeses the file
			if(fp)
			
			
			
				fclose(fp);
		
		}

		
		
		
		return;
	}
		


	
	//executes the game

	execute_games(tokens);


	

}


void execute_games(char *tokens[]){
	
	
	
	//stores the full path that is entered as command line
	char fullpath[1024];
	
	
	//puts the fullpath into its array
	snprintf(fullpath,sizeof(fullpath),"%s/%s",repo_path[0],tokens[0]);

	//sets the redirect number to be -1 to be error if its redirect is changed
	int redirect = -1;



	//Finds < for redirection
	for(int i = 0; tokens[i] != NULL;i++){


		//chekcs if tokens ever equals to <
		if(strcmp(tokens[i],"<")==0){



			if(redirect != -1){


				//prints error when redirect is changed
				error();


				//returns to prompt
				return;


		
			}


			redirect = i;
		}

		//if redirect is different number it checks if the redirection is possible
		if(redirect != -1){

			//if the isnt anything after the < it returns an error
			if(tokens[redirect+1] == NULL || tokens[redirect+2] != NULL){


				//prints error
				error();


				//returns to prompt
				return;



			}


		}


	}




	//pid number variable
	pid_t pid;
	
	
	
	//starts fork process to create child and parent
	pid = fork();
	
	
	
	//if fork fails it prints error and return
	if(pid < 0){
		
		
		//prints error if the fork fails
		error();
		
		
		//returns back to the prompt
		return;
	}
	//if fork succeeds it starts the game
	if(pid == 0){
		
		if(redirect != -1){
					
			//opens the file after <
			int fd = open(tokens[redirect+1],O_RDONLY);


			if(fd < 0){

				//prints error
				error();

				//exits out program
				exit(1);


			}

			//redirects file into fd
			dup2(fd,STDIN_FILENO);
			




			//closes file
			close(fd);



			//removes < and file name form argv
			tokens[redirect] = NULL;



		}
		
		//launches the game
		execvp(fullpath,tokens);
		
		
		
		//sends error if it returns and exits
		error();


		//ends the program
		exit(1);
	}



	//waits for child process to finish
	waitpid(pid,NULL,0);



}
		


int main(int argc, char *argv[]){
	//lines is for the command line to be stored
	char *line = NULL;



	//size keeps track of the buffer from the command line
	size_t size = 0;



	//stores information about a file
	struct stat file;



	//checks if there is 2 lines entered into the command line ex: ./gsh path/to/bin
	if(argc != 2){



		//sends error if evaluates true
		error();


		//exits the program
		exit(1);
	}



	//saves the command line path
	repo_path[0] = argv[1];




//checks to see if the path exists
	if(stat(argv[1],&file)!=0 || !S_ISDIR(file.st_mode)){
		
		
		//sends error if path doesnt exist
		error();
		
		
		//exits the program
		exit(1);
	}
	


	//starts the shell
	while(1){


		//prints the prompt
		printf("gsh> ");
		
		
		//makes the solution prit everything in expected order
		fflush(stdout);
		
		
		if(getline(&line,&size,stdin) == -1){
			free(line);
			exit(0);
		}



		//takes command line to be used for the commands
		process_command(line,argv);

		

	}




	//frees the memory that line has after use
	free(line);
	
	return 0;

}
