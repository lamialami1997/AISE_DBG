1-Installer une bibliothèque : 
sudo apt-get install libunwind-dev : bibliothèque qui fait facilite les appels système
    une bibliothèque qui repose sur ptrace et qui donne des fonctions plus simple à utiliser , sert à recuperer les informations sur le programme qu'on suis .
==>site internet libunwind :https://www.nongnu.org/libunwind/index.html
-UNW_CREATE_ADDR_SPACE(3)                                                                     Programming Library                                                                     UNW_CREATE_ADDR_SPACE(3)
       unw_create_addr_space -- create address space for remote unwinding

        
       #include <libunwind.h>

       unw_addr_space_t unw_create_addr_space(unw_accessors_t *ap, int byteorder);
-strsignal(signal.si_signo) : cette fonction permet d'obtenir le signal en fonction de son numéro (entier ==> chaine de caractères)

main.c : 
    execvp(argv[1],  &argv[1]);// argv[1] c'est le fichier ./crash , @du sous tableau qui contient les argument du pg qu'on débug
-le débug est le pg fils , le père est notre pg main 
    pour débuger : on doit attaché notre debuger à notre programme et pour cela on utilise ptrace 
-le pg débugger qui s'attache au pg fils 
-PTRACE_ATTACH
              Attacher le processus numéro pid, pour le suivre. L'observé va recevoir un SIGSTOP, mais
              il  ne sera peut-être pas arrêté tout de suite, utilisez waitid(2) pour attendre son ar‐
              rêt. Consultez la sous-section Attachement et détachement pour obtenir  de  plus  amples
              renseignements (addr et data sont ignorés).
- PTRACE_GETSIGINFO (depuis Linux 2.3.99-pre6)
              Récupérer des renseignements sur le signal qui a provoqué l'arrêt. Pour ce faire, copier
              une  structure siginfo_t (consultez sigaction(2)) de l'observé à l'adresse data de l'ob‐
              servateur (addr est ignoré).
-siginfo_t :siginfo_t {
               int      si_signo;    /* Numéro de signal           */



------------------------------ informations  -----------------------------------------
2-Execution gdb : Pour exécuter un programme sous gdb, on utilise la commande run :
(gdb) run [arguments du programme]
où arguments du programme sont, s'il y en a, les arguments de votre programme
3-commande backtrace <bt> :indique où l'on se situe dans la pile des appels (synonyme de where)
Exécuter un programme sous gdb
https://blog.tartanllama.xyz/writing-a-linux-debugger-setup/
4-nous permet d'observer et de contrôler l'exécution d'un autre processus en lisant les registres,
 en lisant la mémoire, en pas à pas et plus encore. L'API est très moche ; c'est une fonction unique
  que vous fournissez avec une valeur d'énumérateur pour ce que vous voulez faire, puis quelques arguments
   qui seront utilisés ou ignorés en fonction de la valeur que vous fournissez. La signature ressemble à ceci :