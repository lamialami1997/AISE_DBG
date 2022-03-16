#include <unistd.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <libunwind.h>
#include <string.h>
#include <libunwind-ptrace.h>

//---- librairie libunwind ----
struct UPT_info *ui;
unw_addr_space_t addr_space;
unw_cursor_t cursor;

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
        ptrace(PTRACE_SEIZE, pid, NULL, NULL);
        ptrace(PTRACE_INTERRUPT, pid, NULL,NULL);
        wait(NULL);
        ptrace(PTRACE_CONT, pid, NULL,NULL);
        /* status = ptrace (PTRACE_ATTACH ,pid,NULL, NULL);
        if (status == -1 )
        {
            printf("Erreur lors de l'attachement \n");
            kill(pid, SIGKILL);
            return 1;
        }*/
        wait(NULL);

        /*---- Création de l'espace de travail du Backtrace ----*/

         addr_space = unw_create_addr_space(&_UPT_accessors, 0);
         if (!addr_space)
         {
             printf("Erreur lors de la creation de l'espace d'adressage \n");
             return 2;
         }
         ui = _UPT_create(pid);
          if (!ui) 
            {
            kill(pid,SIGKILL);
            printf("Erreur lors de la creation de la fonction _UPT_create\n");
            return 4;
            }
         status = 0 ; 
         status = unw_init_remote(&cursor , addr_space, ui);
         
         if (status!= 0) 
            {
                _UPT_destroy(ui);
                if (-status == UNW_EINVAL) 
                {
                    printf("unw_init_remote: UNW_EINVAL\n");
                    return 5;
                } 
                else if (-status == UNW_EUNSPEC) 
                {
                    printf("unw_init_remote: UNW_EUNSPEC\n");
                    return 6;
                } 
                else if (-status == UNW_EBADREG) 
                {
                    printf("unw_init_remote: UNW_EBADREG\n");
                    return 7;
                } 
                else 
                {
                    printf("unw_init_remote: UNKNOWN\n");
                    return 8;
                }
            }

         /*----- Backtrace -----*/

         int val = 1 ;
         while(val>0)
         {
             unw_word_t offset ;
             char c[250];
             memset(c, 0, sizeof(char)*250);
             unw_get_proc_name(&cursor, c , sizeof(char)*250, &offset);
             printf("%s 0x%lx \n",c , offset);
             val = unw_step(&cursor);


         }
        /*---- Detruire l'espace de travail du  Backtrace ----*/

        unw_destroy_addr_space(addr_space);
        _UPT_destroy(ui);

        /*---- FIN Backtrace / Récupération du signal ----*/    
        siginfo_t  signal;
        ptrace(PTRACE_GETSIGINFO,pid,NULL,&signal);
        printf("Le programme a reçu le signal suivant : %d qui a causé son arrêt\n", signal.si_signo);
        
        status =0; 
        status = ptrace (PTRACE_DETACH ,pid,NULL, NULL);
        if (status == -1){
             printf("Erreur lors du déttachement \n");
            kill(pid, SIGKILL);
            return 3;
        }

    }


    return 0;
}
