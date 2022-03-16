#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void apres_crash(){
    printf("\n");
}
void crash(){
    printf("FONCTION TEST CRASH\n");
    int a =1 ;
    kill(getpid(), SIGSEGV);
    a=205;
    apres_crash();
}

void fonction(){
    crash();

}



int main(int argc, char const *argv[])
{
    fonction();

    return 0;
}
