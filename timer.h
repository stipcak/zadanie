#include <signal.h>
#include <time.h>

timer_t vytvorCasovac(int);
void casovac(timer_t, int, int);
void opakovanyCasovac(timer_t, int);
void funkciaCasovac(int, void (int, siginfo_t *, void *));
void zastavCasovac(timer_t casovac);
