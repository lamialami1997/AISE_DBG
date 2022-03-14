#include <unistd.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char  **argv)
{   //      0       1              ...
    //argv ./main   ./programme    arg de programme
    
    int pid = fork();
    if (pid==0)
    {
        execvp(argv[1],  &argv[1]);
        return 0;
    }
    else 
    {   
        int status=0;
        status = ptrace (PTRACE_ATTACH ,pid,NULL, NULL);
        if (status == -1 )
        {
            kill(pid, SIGKILL);
            return 1;
        }
        wait(NULL);

        
               
        siginfo_t  signal;
        ptrace(PTRACE_GETSIGINFO,pid,NULL,&signal);
        printf("Le programme a reçu le signal suivant : %d qui a causé son arrêt\n", signal.si_signo);
        
        status =0; 
        status = ptrace (PTRACE_DETACH ,pid,NULL, NULL);
        if (status == -1){
            kill(pid, SIGKILL);
            return 2;
        }

    }


    return 0;
}
