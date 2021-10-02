#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXN 1024

void xarg(char *argv[], int argc){

  char buf[MAXN], c;
  int readd = 1;
  while(readd){
    int i_buf = 0, arg_start = 0, i_argv = argc - 1;

    while(1){
      readd = read(0, &c, sizeof(char));
      if(readd == 0)
        exit(9);
      if(c == ' ' || c == '\n'){
	//Write string command into buf and finish the string
        buf[i_buf++] = '\0';
	//Write string into string array
        argv[i_argv++] = &buf[arg_start];
        arg_start = i_buf;
        if(c == '\n')
          break;
        }
        else
	  //Load character into char array to make a string
          buf[i_buf++] = c;
	}

    //Finish the String and execute the arguments
    argv[i_argv] = '\0';
    int pid = fork();
    if(pid == -1)
      fprintf(2, "Error\n", pid);
    if(pid == 0)
      exec(argv[0], argv);
    else
      wait(0);
    }

  exit(9);
}

int main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Xargs command\n");
    exit(9);
  }

  //Load all arguments into string array
  char *argvall[MAXARG];
  for(int i = 1; i < argc; i++)
    argvall[i - 1] = argv[i];

  xarg(argvall, argc);
  exit(0);
}
