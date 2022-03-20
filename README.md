# Projet AISE 

## Fait par DENDANI Lamia 

## Dépendences :
```bash
    sudo apt-get install libunwind-dev
```
## Utilisation :
### Compilation
 ```bash
    make compile
 ```
### Execution : 
```bash
    ./main ./"Programme à débugguer"
 ```
### Commandes du débugguer :
- La commande "run" ou "r" permet de lancer le programme débugguer .
- La commande "backtrace" ou "bt" permet de  capturer l'état de l'application au moment de l'erreur .
- La commande "info" affiche les informations basiques de notre programme (PID , PPID , GID , chemin globale "localisation du programme")
- La commande "register" ou "reg" permet l'affichage des registres .
- La commande "quit" ou "q" sert à quitter le programme débugguer .

 ## Bibliothèque libunwind : 

Libunwind est une bibliothèque qui repose sur Ptrace et qui donne des fonctions plus simple à utiliser. Elle sert notament à récuperer les informations sur le programme qu'on suis .
Réference :https://www.nongnu.org/libunwind/index.html

## Fonctionnement du débugguer :

Dans de la phase "Execution" il faut donner comme argument à notre programme main (débugguer) le programme que vous souhaitez débugguer comme suit : ./main ./"programme à débugguer" "arguments de votre programme"
par exemple : ./main ./beug 4 4

Une fois fait , tapez les commande sité ci-dessus pour observer  votre programme à débugguer .
 
## Autres explications : 

Ce travail nous permet de lancer un programme débugguer et capturer son fonctionnement lors de son crash .
- Premièrement le programme nous affiche le libellé d'un  signal en fonction de son numéro (entier vers une chaine de caractères) grâce à la fonction strsignal(int sig).
- Ensuite, le programme nous permet de résumer le comportement du programme lors de son crash
- Consulter l'état de la mémoire des registres .
- Informations basiques du programme.
Quand l'analyse du débugguage sera terminée,tapez la commande "quit" ou "q" pour quitter le débugguer .
- NB: Les fonctions principales comme l'attachement et détachement des processus ainsi que backtrace , registres , run ... sont commenter dans le programme main.c