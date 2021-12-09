/*
** UC: 21111 - Sistemas Operativos
** e-fólio A 2019-20 (ab.c)
**
**  Aluno: 1701898 - Diogo Antão
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void gera_cadeia(int n, pid_t pid, char c)
{
  /* função que gera uma cadeia de processos */

  int i, status;

  for(i=1;i<=n;i++){
    /* ciclo que cria n-1 processos novos em cadeia */

    if(pid==0){
      /* numa cadeia só um processo filho Xi pode criar um processo X(i+1) */

      if(i<n){
        /* código para cada processo da cadeia, exceto o último */

        printf("Processo %c%d tem PID = %5d e PPID = %5d\n", c, i, (int) getpid(), (int) getppid());
        pid = fork();
        if(pid==-1){
          perror("Erro na funcao fork()");
          exit(EXIT_FAILURE);
        }

        /* processo Xi espera que os seus processos filhos terminem */
        wait(&status);
      }
      else {
        /*codigo para o último processo da cadeia */

        printf("Processo %c%d tem PID=%5d e PPID=%5d\n", c, i, (int) getpid(), (int) getppid());
      }
    }
  }
}

void arvore_ab(int n)
{
  /* função que cria uma árvore de processos em V invertido com duas cadeias de processos (A e B) */

  pid_t pid_a, pid_b;
  int status;

  /* print inicial do processo AB */
  printf("Cadeia de processos em V invertido com n = %d:\n", n);
  printf("Processo AB tem PID = %5d\n",(int) getpid());

  /* se n=0, não são criados novos processos e o programa termina */
  if(n==0){
    exit(EXIT_SUCCESS);
  }

  /* cria processo filho A1 */
  pid_a = fork();
  if(pid_a==-1){
    perror("Erro na funcao fork()");
    exit(EXIT_FAILURE);
  }

  if(pid_a){
    /* pid_a>0, codigo para o processo pai (AB) */

    /* cria processo filho B1 */
    pid_b = fork();
    if(pid_b==-1){
      perror("Erro na funcao fork()");
      exit(EXIT_FAILURE);
    }

    if(pid_b==0){
      /* código para o processo filho B1 */
      gera_cadeia(n,pid_b,'B');
    }

    /* processo AB espera que os seus processos filhos terminem */
    wait(&status);
  }
  else{
    /* pid_a=0, codigo para o processo filho A1 */
    gera_cadeia(n,pid_a,'A');
  }
}

int main(int argc, char *argv[])
{
  int n;

  /* testar número de argumentos */
  if(argc!=2){
    perror("Numero de argumentos invalido");
    exit(EXIT_FAILURE);
  }

  /* converter argumento dado num inteiro */
  n=atoi(argv[1]);

  /* valor do argumento não pode ser negativo nem superior a 32 */
  if(n<0||n>32){
    perror("Argumento invalido");
    exit(EXIT_FAILURE);
  }

  arvore_ab(n);
  return 0;
}
