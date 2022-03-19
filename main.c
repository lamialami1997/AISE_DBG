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
int pid = 0;

int Attach()
{
    int status = 0;
    /*---- Attachement au processus fils ----*/

    ptrace(PTRACE_SEIZE, pid, NULL, NULL);     // attachement
    ptrace(PTRACE_INTERRUPT, pid, NULL, NULL); // mise en pause du fils pour attendre le père ( fork ==> parrallèle on sait pas quel processus démarre en 1er )
    wait(NULL);                                // Mise en pause le père pour attendre le fils
    ptrace(PTRACE_CONT, pid, NULL, NULL);      // Relancer le fils "Continue"
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
        kill(pid, SIGKILL);
        printf("Erreur lors de la creation de la fonction _UPT_create\n");
        return 4;
    }
    status = 0;
    status = unw_init_remote(&cursor, addr_space, ui);

    if (status != 0)
    {
        _UPT_destroy(ui);
        if (-status == UNW_EINVAL)
        {
            printf("ERREUR: unw_init_remote: UNW_EINVAL\n");
            return 5;
        }
        else if (-status == UNW_EUNSPEC)
        {
            printf("ERREUR: unw_init_remote: UNW_EUNSPEC\n");
            return 6;
        }
        else if (-status == UNW_EBADREG)
        {
            printf("ERREUR: unw_init_remote: UNW_EBADREG\n");
            return 7;
        }
        else
        {
            printf("ERREUR: unw_init_remote: UNKNOWN\n");
            return 8;
        }
    }
    return 0;
}
int Detach()
{
    /*---- Detruire l'espace de travail du Libunwind ----*/

    unw_destroy_addr_space(addr_space);
    _UPT_destroy(ui);
    /*---- Détachement du processus fils ----*/

    int status = 0;
    status = ptrace(PTRACE_DETACH, pid, NULL, NULL);
    if (status == -1)
    {
        printf("Erreur lors du déttachement \n");
        kill(pid, SIGKILL);
        return 3;
    }
    kill(pid, SIGKILL);
    pid=0;
    return 0;
}
void Backtrace()
{
    if (pid == 0)
    {
        printf("ERREUR: aucun processus n'est en cours d'execution\n ");
    }
    else
    {
        unw_cursor_t tmp_cursor = cursor;
        int val = 1;
        printf("----- Backtrace -----\n");
        while (val > 0)

        {
            unw_word_t offset;
            char c[250];
            memset(c, 0, sizeof(char) * 250);
            unw_get_proc_name(&cursor, c, sizeof(char) * 250, &offset);
            printf("%s 0x%lx \n", c, offset);
            val = unw_step(&cursor);
        }
        cursor = tmp_cursor;
        printf("----- Fin Backtrace -----\n");
    }
}

void Registre()
{
    unw_word_t reg[17];
    for (int i = 0; i < 17; i++)
    {
        unw_get_reg(&cursor, i, &reg[i]);
    }
    printf("----- Registres -----\n");
    printf("rax: \t0x%lx\t%ld\n", reg[0], reg[0]);
    printf("rdx: \t0x%lx\t%ld\n", reg[1], reg[1]);
    printf("rcx: \t0x%lx\t%ld\n", reg[2], reg[2]);
    printf("rbx: \t0x%lx\t%ld\n", reg[3], reg[3]);
    printf("rsi: \t0x%lx\t%ld\n", reg[4], reg[4]);
    printf("rdi: \t0x%lx\t%ld\n", reg[5], reg[5]);
    printf("rbp: \t0x%lx\t%ld\n", reg[6], reg[6]);
    printf("rsp: \t0x%lx\t%ld\n", reg[7], reg[7]);
    printf("r8:  \t0x%lx\t%ld\n", reg[8], reg[8]);
    printf("r9:  \t0x%lx\t%ld\n", reg[9], reg[9]);
    printf("r10: \t0x%lx\t%ld\n", reg[10], reg[10]);
    printf("r11: \t0x%lx\t%ld\n", reg[11], reg[11]);
    printf("r12: \t0x%lx\t%ld\n", reg[12], reg[12]);
    printf("r13: \t0x%lx\t%ld\n", reg[13], reg[13]);
    printf("r14: \t0x%lx\t%ld\n", reg[14], reg[14]);
    printf("r15: \t0x%lx\t%ld\n", reg[15], reg[15]);
    printf("rip: \t0x%lx\t%ld\n", reg[16], reg[16]);
    printf("----- Fin Registres -----\n");
}
int run(int argc, char **argv)
{
    if (pid != 0)
    {
        /*---- Détachement du processus fils ----*/
        int status = Detach();
        if (status != 0)
        {
            return status;
        }
    }
    pid = fork();

    /*--- Processus fils ---*/

    if (pid == 0)
    {
        execvp(argv[1], &argv[1]);
        return 0;
    }
    else /*--- Processus père  ---*/
    {
        printf("PID du programme testé :%d\n", pid);
        int status = Attach();
        if (status != 0)
        {
            return status;
        }
        /*---- Récupération du signal ----*/

        siginfo_t signal;
        ptrace(PTRACE_GETSIGINFO, pid, NULL, &signal);
        printf("Le programme %d a reçu le signal suivant : %d : %s à l'adresse 0x%p qui a causé son arrêt\n", pid,signal.si_signo, strsignal(signal.si_signo), signal.si_addr);
    }
    return 0;
}

void parse_commande(char *commande)
{
    for (int i = 0; i < 250; i++)
    {
        if (commande[i] == '\n')
        {
            commande[i] = '\0';
        }
    }
}

int main(int argc, char **argv)
{ //      0       1              ...
    // argv ./main   ./programme    arg de programme
    if (argc < 2)
    {
        printf("ERREUR: Pas assez d'argument\nUtilisation : %s 'Programme à débug' 'Liste d'arguments'\n", argv[0]);
        return 1;
    }
    /*----- Interface -----*/
    char commande[250];
    memset(commande, 0, sizeof(char) * 250);
    while (1)
    {
        fgets(commande, 250, stdin);
        parse_commande(commande);
        if (strcmp(commande, "run") == 0 || strcmp(commande, "r") == 0)
        {
            run(argc, argv);
        }
        else if (strcmp(commande, "backtrace") == 0 || strcmp(commande, "bt") == 0)
        {
            Backtrace();
        }
        else
        {
            printf("ERREUR: Commande '%s' non reconnue\n", commande);
        }
    }
    return 0;
}
