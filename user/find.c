#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void
find(char *path, char *file)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  //Error Outputs
  if((fd = open(path, 0)) < 0){
    fprintf(2, "Can't find the path %s\n", path);
    exit(9);
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "Can't stat %s\n", path);
    close(fd);
    exit(9);
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    fprintf(2, "Path too long\n");
    close(fd);
    exit(9);
  }

  //build string representing path
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
      //Ignore '.' and '..'
    if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = '\0';
    if (stat(buf, &st) < 0) {
      fprintf(1, "Cannot stat %s\n", buf);
	continue;
      }
      if(st.type == T_FILE && strcmp(de.name, file) == 0)
	//Print file if it has matching name
	fprintf(1, "%s\n", buf);
      else if(st.type == T_DIR)
	//Look for the file in the directory
	find(buf, file);
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 2 || argc > 3){
    fprintf(2, "Find (path) [file]\n");
    exit(9);
  }
  if(argc < 3)
    find(".", argv[1]);
  else
    find(argv[1], argv[2]);

  exit(0);
}
