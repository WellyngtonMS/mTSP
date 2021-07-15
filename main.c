#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <time.h>
#include "biblioteca.h"

int main(){
    clock_t t;
    setlocale(LC_ALL, "Portuguese");
    TInfo info;
    printf("> Digite o nome do arquivo com a extensão (Ex.: arquivo.txt)\n> ");
    scanf("%s", info.nome);
    fflush(stdin);
    lerArquivo(info.nome, &info);
    alocarRota(&info);

    int opcao, flag = 0;
    int n = ceil((double)(info.g.n-1)/info.p);
    TCelula *aux;

    do{
        system("cls");
        MSG_MENU();
        printf("\n\nDigite uma opção: ");
        fflush(stdin);
        scanf("%d", &opcao);
        switch(opcao){
            case 1:
                t = clock();
                heuristica(&info);
                t = clock() - t;
                flag = 1;
                printf("Heurística Construtiva criada!\n");
                printf("Tempo de execução: %fs\n", ((double)t)/(CLOCKS_PER_SEC));
                system("PAUSE");
                break;
            case 2:
                do {
                    system("cls");
                    printf("\n --- Movimentos de Vizinhança:\n\n");
                    printf("\t1. SWAP\n");
                    printf("\t2. 2-Opt\n");
                    printf("\t3. re-Insertion\n");
                    printf("\t4. Voltar ao menu principal\n");
                    printf("\nDigite uma opção: ");
                    fflush(stdin);
                    scanf("%d", &opcao);
                    switch (opcao) {
                        case 1:
                            if (opcao == 1 && flag == 1) {
                                t = clock();
                                swap(&info);
                                t = clock() - t;
                                printf("Tempo de execução: %fs\n", ((double)t)/(CLOCKS_PER_SEC));
                                system("PAUSE");
                            } else {
                                printf("Execute a Heurística Construtiva primeiro!\n");
                                system("PAUSE");
                            }
                            break;
                        case 2:
                            if (opcao == 2 && flag == 1) {
                                t = clock();
                                twoOpt(&info);
                                t = clock() - t;
                                printf("Tempo de execução: %fs\n", ((double)t)/(CLOCKS_PER_SEC));
                                system("PAUSE");
                            } else {
                                printf("Execute a Heurística Construtiva primeiro!\n");
                                system("PAUSE");
                            }
                            break;
                        case 3:
                            if (opcao == 3 && flag == 1) {
                                t = clock();
                                reInsertion(&info);
                                t = clock() - t;
                                printf("Tempo de execução: %fs\n", ((double)t)/(CLOCKS_PER_SEC));
                                system("PAUSE");
                            } else {
                                printf("Execute a Heurística Construtiva primeiro!\n");
                                system("PAUSE");
                            }
                            break;
                        case 4:
                            printf("\nVoltando ao menu...\n");
                            system("PAUSE");
                            break;
                        default:
                            printf("\nERRO: Opção inválida!\n");
                            system("PAUSE");
                    }
                }while(opcao != 4);
                break;
            case 3:
                if(flag == 0){
                    printf("Execute a Heurística Construtiva primeiro!\n");
                    system("PAUSE");
                }
                else{
                    t = clock();
                    VND(&info, 3);
                    t = clock() - t;
                    printf("Tempo de execução: %fs\n", ((double)t)/(CLOCKS_PER_SEC));
                    system("PAUSE");
                }
                break;
            case 4:
                criarArquivo(info);
                printf("Arquivo criado com sucesso! Verifique a raiz do projeto.\n");
                system("PAUSE");
                break;
            case 5:
                for(int i = 0; i < n; i++){
                    aux = info.a.rota[i].primeiro->prox;
                    printf("Rota do Agente %d: ", i+1);
                    while (aux != NULL){
                        printf("%2d ", aux->item.Chave);
                        aux = aux->prox;
                    }
                    printf("\n");
                    printf("Custo da rota %d\n", info.a.custo[i]);
                }
                printf("\nCusto total %d\n", info.custo);
                system("PAUSE");
                break;
            case 6:
                system("cls");
                printf("> Digite o nome do arquivo com a extensão (Ex.: arquivo.txt)\n> ");
                scanf("%s", info.nome);
                lerArquivo(info.nome, &info);
                alocarRota(&info);
                flag = 0;
                n = ceil((double)(info.g.n-1)/info.p);
                break;
            case 7:
                printf("\nTerminando programa...\n");
                system("PAUSE");
                break;
            default:
                printf("\nERRO: Opção inválida!\n");
                system("PAUSE");
        }
    }while(opcao != 7);

    return 0;
}
