#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0,SIGKILL),\
		     exit(EXIT_FAILURE))





void usage(char * name){
	fprintf(stderr,"USAGE: %s n\n",name);
	fprintf(stderr,"0<n<=10 - number of children\n");
	exit(EXIT_FAILURE);
}


void create_children_and_pipes(int n, int* inPipe, int* outPipe) {
	if (n>0){
	int PC[2];
	int CP[2];
	if(pipe(PC)) ERR("pipe Parent Child");
	if(pipe(CP))ERR("pipe  Child Parent");


	int pid = fork();   
    if (pid == 0) { // child   
        printf("Child  [%d]\n",getpid());

        create_children_and_pipes(n-1,PC,CP);
        if(n-1==0) sleep(1);
    }
    else if (pid==-1) {  /// ERR 
        ERR("FORK FAILED");
    }
    // try to wait for any children while there exists at least one
	while ((wait(&pid))!=-1) {
  	printf("Process %d terminated\n",getpid());
	}
	}
}


	



int main(int argc, char** argv) {
	int n, *fds,R[2];
	if(2!=argc) usage(argv[0]);
	n = atoi(argv[1]);
	if (n<=0||n>10) usage(argv[0]);

	int fromMain[2];
	int toMain[2];
	if(pipe(fromMain)) ERR("pipe Main to  Child");
	if(pipe(toMain)) ERR("pipe   Child to Main");
	create_children_and_pipes(n,fromMain,toMain);


	
	return EXIT_SUCCESS;
}