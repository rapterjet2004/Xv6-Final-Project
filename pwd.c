#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

#define MAX_DEPTH 20

char names[MAX_DEPTH][DIRSIZ+1];

int
main(int argc, char *argv[])
{
  struct stat st, parent_st;
  struct dirent de;
  int fd, depth = 0;

  // save starting directory
  int start = open(".", 0);

  while(1){
    if(stat(".", &st) < 0 || stat("..", &parent_st) < 0){
      printf(2, "pwd error\n");
      break;
    }

    // reached root
    if(st.ino == parent_st.ino)
      break;

    // go to parent
    chdir("..");

    fd = open(".", 0);

    // find name of directory we came from
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == st.ino){
        memmove(names[depth], de.name, DIRSIZ);
        names[depth][DIRSIZ] = 0;
        depth++;
        break;
      }
    }

    close(fd);
  }

  // print path
  printf(1, "/");
  for(int i = depth-1; i >= 0; i--){
    printf(1, "%s", names[i]);
    if(i > 0)
      printf(1, "/");
  }

  // restore directory by walking back using names
for(int i = 0; i < depth; i++){
  chdir(names[i]);
}
  close(start);

  exit();
}
