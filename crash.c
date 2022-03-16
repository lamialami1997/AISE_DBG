#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void apres_crash()
{
    printf("\n");
}
void crash()
{
    printf("FONCTION TEST CRASH\n");
    __asm__ volatile(
        "mov $1, %%rax;\n"
        "mov $2, %%rdx;\n"
        "mov $3, %%rcx;\n"
        "mov $4, %%rbx;\n"
        "mov $5, %%rsi;\n"
        "mov $6, %%rdi;\n"
        "mov $9, %%r8;\n"
        "mov $10, %%r9;\n"
        "mov $11, %%r10;\n"
        "mov $12, %%r11;\n"
        "mov $13, %%r12;\n"
        "mov $14, %%r13;\n"
        "mov $15, %%r14;\n"
        "mov $16, %%r15;\n"

        :

        :

        : "cc", "memory", "rax", "rdx", "rcx", "rbx", "rsi", "rdi", "r8", "r9", "r10",
          "r11", "r12", "r13", "r14", "r15");
    raise(SIGSEGV);
    int a = 1;
    kill(getpid(), SIGSEGV);
    a = 205;
    apres_crash();
}

void fonction()
{
    crash();
}

int main(int argc, char const *argv[])
{
    fonction();

    return 0;
}
