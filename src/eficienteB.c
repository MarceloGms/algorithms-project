#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
#include <sys/time.h>

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

typedef struct Registo{
    char matricula[7];
    char infracao[SIZE];
    int gravidade;
    char infrator[SIZE];
}Registo;

void radix(Registo *reg, int size) {
    int contagem;
    for (int c = 5; c >= 0; c--) {
        int count[256] = {0};
        Registo *novo = NULL;
        novo = (Registo*)malloc(size * sizeof(Registo));

        for (int i = 0; i < size; i++) {
            count[(unsigned char)reg[i].matricula[c]]++;
        }

        for (int i = 1; i < 256; i++) {
            count[i] += count[i - 1];
        }

        for (int i = size - 1; i >= 0; i--) {
            novo[count[(unsigned char)reg[i].matricula[c]] - 1] = reg[i];
            count[(unsigned char)reg[i].matricula[c]]--;
        }

        for (int i = 0; i < size; i++) {
            reg[i] = novo[i];
            ++contagem;
        }
        free(novo);
    }
    printf("%d\n", contagem);
}

int main(){

    char comando[SIZE] = {""};
    char comando2[SIZE] = {""};
    Registo *reg = NULL;
    char str[SIZE];
    char str_tmp[SIZE];
    int size = 0;
    char *parte;
    struct timeval t0;
    struct timeval t1;
    float elapsed;

    while(1){
        
        fgets(str_tmp, SIZE, stdin);
        parte = strtok(str_tmp, " ");
        if(parte != NULL) strcpy(comando, parte);
        parte = strtok(NULL, "\0");
        if(parte != NULL) strcpy(comando2, parte);

        if(strcmp(comando, "DIM_BD") == 0){
            reg = (Registo*)malloc(atoi(comando2) * sizeof(Registo));
            for(int i = 0; i < atoi(comando2); ++i){
                fgets(str,SIZE,stdin);
                strcpy(reg[i].matricula, strtok(str, " "));
                while((parte = strtok(NULL, " ")) != NULL){
                    if(atoi(parte) == 0){
                        strcat(strcat(reg[i].infracao," "), parte);
                    }
                    else{
                        if(atoi(parte) < 1 || atoi(parte) > 5) exit(1);
                        else{
                            reg[i].gravidade = atoi(parte);
                            break;
                        }
                    }
                }
                strcpy(reg[i].infracao, reg[i].infracao + 1);

                while((parte = strtok(NULL, " ")) != NULL){
                    strcat(strcat(reg[i].infrator, " "), parte);
                }
                strcpy(reg[i].infrator, reg[i].infrator + 1);
            }
            size = atoi(comando2);
            printf("BD_ATUALIZADA\n");
            gettimeofday(&t0, 0);
            radix(reg, size);
            gettimeofday(&t1, 0);

            elapsed = timedifference_msec(t0, t1);
            printf("Code executed in %f milliseconds.\n", elapsed);
        }

        else if (strcmp(comando, "CONSULTA_MATRICULA") == 0){
            int count = 0;
            for(int i = 0; i < size; ++i){
                comando2[strcspn(comando2, "\n")] = '\0';
                if(strcmp(reg[i].matricula, comando2) == 0){
                    printf("%s %d %s", reg[i].infracao, reg[i].gravidade, reg[i].infrator);
                    ++count;
                }
            }
            if (count == 0) printf("REGISTOS NAO ENCONTRADOS\n");
            printf("FIM\n");
        }

        else if (strcmp(comando, "CONSULTA_CONDUTOR") == 0){
            int count = 0;
            for(int i = 0; i < size; ++i){
                if(strcmp(reg[i].infrator, comando2) == 0){
                    printf("%s %s %d\n", reg[i].matricula, reg[i].infracao, reg[i].gravidade);
                    ++count;
                }
            }
            if (count == 0) printf("REGISTOS NAO ENCONTRADOS\n");
            printf("FIM\n");
        }

        else if(strcmp(comando, "CONSULTA_BD\n") == 0){
            for(int i = 0; i < size; ++i){
                printf("%s %s %d %s", reg[i].matricula, reg[i].infracao, reg[i].gravidade, reg[i].infrator);
            }
            printf("FIM\n");
        }

        else if (strcmp(comando, "TCHAU\n") == 0){
            free(reg);
            return 0;
        } 
    }
}