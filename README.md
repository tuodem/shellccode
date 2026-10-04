Simple Shell program

The purpose of this program is to be able to search and execute games within an Unix environment such as Sudoku, 2048, and TicTacToe.

This code is created in the language C and was created in a Unix environment to run.

To download and use this program you must have an Unix environment that would allow C code to be able to compile and run. To compile the code you would simply have to use the make file named "Makefile" by simply typing make in the command line.

Now to start the program you must type "./gsh" with the path to your files of games you would use with the shell program. In the repository I have a folder of games you can use to path to for the shell program. 

An example of a path you can do is "./gsh shell/games/src/sudoku" this would allow you to execute the sudoku game in one path to the game. Another example could be "./gsh shell/games" and that would path you to before you access the games.

Another feature of this program is the commands that come with the program such as "exit", "path", and "ls". 

The exit command ends the program to stop it from continuing to run. 

Path command allows you to enter a path to your directories that contain your games, or other directories you'd like to enter. 

The last command is "ls" which allows you to see what is contained within each directory and possibly give you a description if you are in a repository where the games executable is.

Another thing to expect is the error message you are given if what you type is incorrect and example is when you run the program your are met with the prompt "gsh>", now if you hit enter the program will give you an error message prompting you to try again.

The program also understands if you leave white space within the program such as example is "gsh>    ls" the program will read this and still print out the files within the directory. 
