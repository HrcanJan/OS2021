#include "kernel/types.h"
#include "user/user.h"

void
makeprime(int *pin)
{
  int n, buf, p[2];
  pipe(p);
  close(pin[1]);
  //Read lowest number
  if(read(pin[0], &n, sizeof(int))){
    printf("prime %d\n", n);
    if(fork() == 0) {
      makeprime(p);
      exit(9);
    }

    close(p[0]);
    //read into buf numbers from prime / filtered pipe
    while(read(pin[0], &buf, sizeof(int)))
      if(buf % n != 0)
      //write into new pipe all numbers not divideable with n 
	write(p[1], &buf, sizeof(int));
    close(p[1]);
    wait(0);
    }
  exit(9);
}

int
main(int argc, char *argv[])
{
  if(argc > 1) {
  fprintf(2, "Primes:...\n");
  exit(1);
  }

  int primes[2];
  pipe(primes);

  //Child
  if(fork() == 0){
    //Filters out numbers through pipes
    makeprime(primes);
    exit(0);
  }

  close(primes[0]);
  //Parent
  for(int i = 2; i <= 35; ++i)
    write(primes[1], &i, sizeof(int));
  close(primes[1]);
  wait(0);

  exit(0);
}
