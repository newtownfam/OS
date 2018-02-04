//#include "ancestry.h"
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

struct ancestry{
  pid_t parent[10];
  pid_t children[100];
  pid_t sibling[100];
} ;
 
int main(){
    struct ancestry ancestreec, ancestreep;
    int rc = 1;//fork();
    if (rc == 0)
    {
        pid_t self = getpid();
        pid_t parent = getppid();
        syscall(378, &self, &ancestreec);
        assert(ancestreec.parent[0] == parent);
        exit(0);
    }
    else{
        pid_t self = 1824;//getpid();
        syscall(378, &self, &ancestreep);
        wait(NULL);
        assert(ancestreep.children[0] == rc);
    }
 
}
