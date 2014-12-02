#include "timer.h"

timer_t vytvorCasovac(int signal)
{
  struct sigevent kam;
  kam.sigev_notify=SIGEV_SIGNAL;
  kam.sigev_signo=signal;

  timer_t casovac;
  timer_create(CLOCK_REALTIME, &kam, &casovac);
  return(casovac);
}
void casovac(timer_t casovac, int s,int ms)
{
  struct itimerspec casik;
  casik.it_value.tv_sec=s;
  casik.it_value.tv_nsec=(long)ms*1000000;
  casik.it_interval.tv_sec=0;
  casik.it_interval.tv_nsec=0;
  timer_settime(casovac,CLOCK_REALTIME,&casik,NULL);
}

void zastavCasovac(timer_t casovac)
{
  struct itimerspec casik;
  casik.it_value.tv_sec=0;
  casik.it_value.tv_nsec=0;
  casik.it_interval.tv_sec=0;
  casik.it_interval.tv_nsec=0;
  timer_settime(casovac,CLOCK_REALTIME,&casik,NULL);
}
void opakovanyCasovac(timer_t casovac,int ms)
{
  struct itimerspec casik;
  casik.it_value.tv_sec=0;
  casik.it_value.tv_nsec=(long)ms*1000000;
  casik.it_interval.tv_sec=0;
  casik.it_interval.tv_nsec=(long)ms*1000000;
  timer_settime(casovac,CLOCK_REALTIME,&casik,NULL);
}
void funkciaCasovac(int signal, void obsluzna(int, siginfo_t *, void *))
{
  sigset_t signalSet;
  struct sigaction nastavovacia;
  sigemptyset(&signalSet);
  nastavovacia.sa_sigaction=obsluzna;
  nastavovacia.sa_flags=SA_SIGINFO;
  nastavovacia.sa_mask=signalSet;
  sigaction(signal,&nastavovacia,NULL);
}

