#include "potato.h"
#include <stdio.h>
#include <unistd.h>

int main(int args, char** argp, char** envp){
    printf("Potato p = Potato_cook(11);\n");
    Potato p = Potato_cook(11);
    int pipefd[2]; 
    pipe(pipefd);
    
    Potato_sign(p,11);
    Potato_sign(p,12);

    printf("\n\nPotato_give(p, STDOUT_FILENO);\n");
    Potato_give(p, STDOUT_FILENO);
    
    printf("\n\nPotato_give(p, pipefd[1]);\n");
    Potato_give(p, pipefd[1]);

    printf("\n\nPotato q =Potato_take(pipefd[0]);\n") ;
    Potato q = Potato_take(pipefd[0]);


    printf("\n\nPotato_give(q, STDOUT_FILENO);\n");
    Potato_give(q, STDOUT_FILENO);
  
    Potato_free(p);
    Potato_free(q);

       
}

