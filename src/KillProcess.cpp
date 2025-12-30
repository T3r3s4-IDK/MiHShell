#include <csignal>
#include <unistd.h>
#include <stdlib.h>

int ChildPid = -1;


void KillProcess(int sig){
    kill(ChildPid, SIGTERM);
}
