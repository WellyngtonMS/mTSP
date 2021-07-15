#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "biblioteca.h"

ssize_t getline(char **lineptr, size_t *n, FILE *stream){
    size_t pos;
    int c;

    if(lineptr == NULL || stream == NULL || n == NULL){
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if(c == EOF){
        return -1;
    }

    if(*lineptr == NULL){
        *lineptr = malloc(128);
        if(*lineptr == NULL){
            return -1;
        }
        *n = 128;
    }

    pos = 0;
    while(c != EOF){
        if(pos + 1 >= *n){
            size_t new_size = *n + (*n >> 2);
            if(new_size < 128){
                new_size = 128;
            }
            char *new_ptr = realloc(*lineptr, new_size);
            if(new_ptr == NULL){
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char *)(*lineptr))[pos ++] = c;
        if(c == '\n'){
            break;
        }
        c = getc(stream);
    }

    (*lineptr)[pos] = '\0';
    return pos;
}

void criaGrafo(TGrafo *g, int n){
    g->mat = (int **)malloc(sizeof(int *)*n); //aloca o n�mero de linhas da matriz
    if(g->mat == NULL){
        printf("Erro: Mem�ria Insuficiente.\n");
        exit(1);
    }
    int i;
    for(i = 0; i < n; i++){
        g->mat[i] = (int*)malloc(sizeof(int)*n); //aloca para cada linha, o mesmo n�mero de linhas
        if (g->mat[i] == NULL){
            printf("Erro: Mem�ria Insuficiente.\n");
            exit(1);
        }
    }
}

void lerArquivo(char *nomeArquivo, TInfo *info){
    FILE *file_descriptor;
    if((file_descriptor = fopen(nomeArquivo, "r")) == NULL){
        printf("Problemas na abertura do arquivo ou arquivo inexistente!\n");
        exit(1);
    }
    char palavra[15];

    fscanf(file_descriptor, "%11[^' ']%d", palavra, &info->g.n); //l� o n�mero da DIMENSION
    fscanf(file_descriptor, "%3[^' ']%d", palavra, &info->p); //l� o n�mero de P
    criaGrafo(&info->g, info->g.n); //chama a fun��o de aloca��o do grafo

    char *line = NULL, *aux = NULL;
    size_t len = 0;
    ssize_t read;
    getline(&line, &len, file_descriptor);
    getline(&line, &len, file_descriptor);

    int i, nLinha = 0;
    while((read = getline(&line, &len, file_descriptor)) != -1){ //la�o para percorrer todas as linhas da matriz do arquivo
        for(i = 0; i < info->g.n; i++){
            if(nLinha == i){ //verifica se estou na diagonal
                info->g.mat[nLinha][i] = 9999;
            }
            else if(i == 0){ //caso o primeiro elemento n�o for a diagonal nem a primeira linha
                if(line[2] == ' '){
                    aux = &line[3];
                    aux[2] = '\0';
                    info->g.mat[nLinha][i] = atoi(aux);
                }
                else if(line[1] == ' '){
                    aux = &line[2];
                    aux[3] = '\0';
                    info->g.mat[nLinha][i] = atoi(aux);
                }
                else{
                    aux = &line[1];
                    aux[4] = '\0';
                    info->g.mat[nLinha][i] = atoi(aux);
                }
            }

            else{ //restante dos elementos da linhas
                if(line[8 + 6*(i-1)] == ' '){ //verifica se o valor tem dois d�gitos
                    aux = &line[9 + 6*(i-1)];
                    aux[2] = '\0';
                    info->g.mat[nLinha][i] = atoi(aux);
                }
                else if(line[7 + 6*(i-1)] == ' '){ //verifica se o valor tem tr�s d�gitos
                    aux = &line[8 + 6*(i-1)];
                    aux[3] = '\0';
                    info->g.mat[nLinha][i] = atoi(aux);
                }
                else{ //caso possua mais que tr�s d�gitos
                    aux = &line[7 + 6*(i-1)];
                    aux[4] = '\0';
                    info->g.mat[nLinha][i] = atoi(aux);
                }
            }
        }
        i = 0;
        nLinha++;
        if(nLinha == info->g.n){
            break;
        }
    }

    free(line);
    fclose(file_descriptor);
}

void criarArquivo(TInfo info){
    FILE *arquivo;
    arquivo = fopen("resultado.txt", "w");

    if(arquivo == NULL){
        printf("Erro na abertura do arquivo!");
        exit(1);
    }

    int i;
    int n = ceil((double)(info.g.n-1)/info.p);
    TCelula *aux;
    for(i = 0; i < n; i++){ //la�o para percorrer todas as rotas n geradas na solu��o
        aux = info.a.rota[i].primeiro->prox;
        while (aux != NULL){ //la�o que percorre todos os n endere�os visitados pelo agente i
            if(aux->prox == NULL){ //se for o �ltimo endere�o do agente, insere-o junto com ; para delimitar o final da rota
                fprintf(arquivo, "%d ; ", aux->item.Chave);
                aux = aux->prox;
            }
            else{
                fprintf(arquivo, "%d, ", aux->item.Chave);
                aux = aux->prox;
            }
        }
    }
    fprintf(arquivo, "\nCusto total %d\n", info.custo); //escreve no arquivo o custo total de todas as rotas

    fclose(arquivo);
}

void alocarRota(TInfo *info){
    int n = ceil((double)(info->g.n-1)/info->p); //calcula quantas rotas ser�o necess�rias para atender o n�mero de endere�os-sede
    info->a.custo = (int *)malloc(n*sizeof(int)); //aloca um vetor de custos de cada rota, de tamanho n
    memset(info->a.custo, 0, n*sizeof(info->a.custo[0])); //seta o vetor de custos para zero (0)

    info->a.rota = (TLista *)malloc(n*sizeof(TLista)); //aloca um vetor de listas encadeadas, cada posi��o do vetor possui uma lista (rota) de cada agente
    int i;
    for (i = 0; i < n; i++) { //inicializa cada lista de cada rota
        info->a.rota[i].primeiro = (TCelula*)malloc(sizeof(TCelula));
        info->a.rota[i].ultimo = info->a.rota[i].primeiro;
        info->a.rota[i].primeiro->prox = NULL;
    }
    info->custo = 0; //seta o valor do custo total para zero (0)
}

void Inserir(int x, TLista *Lista){
    Lista->ultimo->prox = (TCelula*)malloc(sizeof(TCelula));
    Lista->ultimo = Lista->ultimo->prox;
    Lista->ultimo->item.Chave = x;
    Lista->ultimo->prox = NULL;
}

void custos(TInfo *info){
    int n = ceil((double)(info->g.n-1)/info->p);
    memset(info->a.custo, 0, n*sizeof(info->a.custo[0]));
    info->custo = 0;
    TCelula *aux;
    for(int i = 0; i < n; i++){ //la�o para percorrer as n rotas
        aux = info->a.rota[i].primeiro->prox;
        while (aux->prox != NULL){ //percorre os endere�os visitados na rota n pelo agente i
            info->custo += info->g.mat[aux->item.Chave][aux->prox->item.Chave]; //soma do custo total
            info->a.custo[i] += info->g.mat[aux->item.Chave][aux->prox->item.Chave]; //soma do custo total da rota n
            aux = aux->prox;
        }
    }
}

void heuristica(TInfo *info){
    info->visitados = (int*)malloc(info->g.n*sizeof(int)); //vetor de tamanho igual a DIMENSION para controlar os endere�os j� visitados
    memset(info->visitados, 0, info->g.n*sizeof(info->visitados[0])); //seta o vetor para zero (0) n�o visitado
    int i, j, k = 0, p = 0, x = 0, flag = 0, cont = 1;
    int vizinho = info->g.mat[0][1]; //pega o primeiro vizinho da primeira linha
    info->a.nAgente = 1; //chama o primeiro agente

    info->visitados[0] = 1; //sede sempre como "visitada"
    Inserir(0, &info->a.rota[info->a.nAgente-1]); //insere a sede na rota o primeiro agente
    for(i = 0; i < info->g.n; ){ //la�o para percorrer os endere�os
        i = x;
        for(j = 0; j < info->g.n; j++){ //la�o para percorrer os endere�os de uma dada linha
            if(i == j){ //verifica se � a diagonal
                continue;
            }
            else{
                if(info->visitados[j] == 1){ //verifica se o endere�o j� foi visitado
                    continue;
                }
                else if(info->g.mat[i][j] < vizinho){ //compara o vizinho com o valor da posi��o da matriz no momento
                    vizinho = info->g.mat[i][j]; //faz a troca
                    x = j; //guarda o n�mero (�ndice) do endere�o
                    flag = 1;
                }
            }
        }
        if(j == info->g.n){ //verifica se percorreu todos os endere�os dispon�veis
            if(flag == 0){ //caso n�o tenha encontrado um vizinho mais barato
                x = k;
                k = 0;
            }
            if(flag == 0 && cont == 1){ //caso o mais barato foi de cara o primeiro da primeira linha
                x = 1;
                k = 0;
            }
            info->visitados[x] = 1; //marca como visitado
            Inserir(x, &info->a.rota[info->a.nAgente-1]); //insere na lista da rota do agente
            p++;
            cont += 1; //incrementa o controlador do for mais externo
            if(p == info->p){ //verifica se aquele agente j� visitou o limite de endere�os
                Inserir(0, &info->a.rota[info->a.nAgente-1]); //insere a sede ao final da rota
                info->a.nAgente++;
                vizinho = info->g.mat[0][k]; //pega um novo vizinho para o pr�ximo agente
                p = 0;
                i = 0;
                x = 0;
                flag = 0;
                if(cont == info->g.n){ //verifica se acabou os endere�os a serem visitados
                    break;
                }
                Inserir(0, &info->a.rota[info->a.nAgente-1]); //insere a sede na rota o primeiro agente
                continue;
            }
            while(k < info->g.n){ //la�o para pegar o pr�ximo vizinho do agente, que ainda n�o foi visitado e ent�o comparar com os demais
                if(info->visitados[k] == 0){
                    vizinho = info->g.mat[x][k];
                    if(flag == 0){
                        x = k;
                    }
                    else{
                        flag = 0;
                    }
                    break;
                }
                k++;
            }
        }
        if(cont == info->g.n){ //verifica se acabou os endere�os a serem visitados, mas o agente ainda podia visitar mais
            Inserir(0, &info->a.rota[info->a.nAgente-1]); //insere ent�o a sede no final da rota
            break;
        }
    }
    custos(info); //chama a fun��o de custos para calcular tais valores
}

int Vazia(TLista Lista){
    return(Lista.primeiro == Lista.ultimo);
}

TCelula* Pesquisar(int item, TLista lista){
    TCelula* aux, *aux2;
    aux = lista.primeiro->prox;
    aux2 = lista.primeiro;
    if(!Vazia(lista)){
        while(aux != NULL){
            if(aux->item.Chave == item){
                return aux2;
            }
            aux = aux->prox;
            aux2 = aux2->prox;
        }
    }
    return aux2;
}

void swapValue(TLista *lista, int x, int y){
    TCelula *antX = Pesquisar(x, *lista);
    TCelula *antY = Pesquisar(y, *lista);

    antX->prox->item.Chave = y;
    antY->prox->item.Chave = x;
}

void inverterList(TInfo *info, int a, int b, int k){
    TCelula *prev, *head, *current, *next;
    prev = Pesquisar(a, info->a.rota[k]);
    head = prev->prox;
    current = prev->prox;

    while(current->item.Chave != b){
        next = current->prox;
        current->prox = prev;
        prev = current;
        current = next;
    }
    next = current->prox;
    current->prox = prev;
    head->prox->prox = current;
    head->prox = next;

    free(prev);
    free(head);
    free(current);
    free(next);
}

void swapMemoryReInsertion(TLista *lista, int a, int b){
    TCelula *aux, *troca, *old;
    aux = Pesquisar(a, *lista);
    old = Pesquisar(b, *lista);
    old = old->prox;
    troca = aux->prox;
    aux->prox = troca->prox;
    troca->prox = old->prox;
    old->prox = troca;
}

int swap(TInfo *info){
    int n = ceil((double)(info->g.n-1)/info->p); //calcula o n�mero de rotas que existe
    int i, j, p, k = 0, flag = 0;
    TCelula *auxA, *auxB, *aux;
    int a, b, newgain, gain = 0, dif = 0;
    int best[2];

    while(k < n){ //la�o para percorres todas as rotas
        auxA = info->a.rota[k].primeiro->prox; //pega a rota n
        for(i = 1; auxA->prox->prox->prox != NULL && auxA->prox->prox->prox->item.Chave != 0; i++){ //la�o para percorrer todos os endere�os da rota n, verificando quando chega ao final ou encontra a sede (0)
            a = auxA->item.Chave; //recebe o primeiro valor da rota n
            auxB = auxA;
            for(j = i; (auxB->prox->item.Chave != 0 && auxB->prox != NULL); j++){ //la�o que percorre todos os endere�os comparando-os
                if((j) == (i)){ // verifica se � o primeiro valor (sede)
                    a = auxA->prox->item.Chave; //recebe o pr�ximo caso o atual seja a sede
                    auxB = auxA->prox->prox;
                    b = auxB->item.Chave;
                    continue;
                }
                b = auxB->prox->item.Chave; //recebe o valor que ser� trocado com o primeiro da rota n
                aux = Pesquisar(b, info->a.rota[k]);
                newgain = info->a.custo[k] -
                        info->g.mat[auxA->item.Chave][a] - info->g.mat[a][auxA->prox->prox->item.Chave] - info->g.mat[aux->item.Chave][b] - info->g.mat[b][aux->prox->prox->item.Chave] +
                        info->g.mat[auxA->item.Chave][b] + info->g.mat[b][auxA->prox->prox->item.Chave] + info->g.mat[aux->item.Chave][a] + info->g.mat[a][aux->prox->prox->item.Chave];
                        //calcula o ganho do custo caso seja feita a troca entre o A e o B
                auxB = auxB->prox; //avan�a com o valor do for mais interno
                dif = newgain - info->a.custo[k]; //calcula a diferen�a do custo da nova rota menos a original
                if(dif < gain){ //verifica se houve algum deficit
                    gain = dif;
                    best[0] = a; //guarda o valor de A
                    best[1] = b; //guarda o valor de B
                    flag = 1;
                    p = k; //guarda o �ndice da rota da melhor troca
                }
            }
            auxA = auxA->prox; //avan�a com o valor do for mais externo
        }
        k++; //avan�a com o valor do while externo
    }
    if(flag == 1){ //se a flag for 1, significa que encontrou algum troca que reduzisse o custo
        info->custo += gain;
        info->a.custo[p] += gain;
        swapValue(&info->a.rota[p], best[0], best[1]); //chama a fun��o auxiliar que troca os valores de A e B na melhor rota
        flag = 0;
    }
    return gain; //retorna o valor do ganho, sempre negativo ou zero, caso n�o haja swap
}

int twoOpt(TInfo *info){
    int n = ceil((double)(info->g.n-1)/info->p); //calcula o n�mero de rotas que existe
    TCelula *aux, *auxA, *auxD;
    int  k = 0, a, d, p, gain = 0, dif = 0, newgain;
    int flag = 0;
    int best[2];

    while(k < n){ //la�o para percorres todas as rotas
        aux = info->a.rota[k].primeiro->prox; //pega a rota n
        a = aux->prox->item.Chave; //recebe o primeiro elemento da rota ap�s a sede
        while(aux->item.Chave != 0){ //la�o para percorrer todos os endere�os da rota
            while(aux->prox->prox->prox->prox->item.Chave != 0){ //la�o para ir aumentando o n�mero de endere�os � se aplicar a invers�o
                d = aux->prox->prox->prox->prox->item.Chave; //pega inicialmente o 4� elemento e depois o pr�ximo em diante
                auxA = Pesquisar(a, info->a.rota[k]);
                auxD = Pesquisar(d, info->a.rota[k]);
                newgain = info->a.custo[k] -
                        info->g.mat[auxA->item.Chave][a] - info->g.mat[d][auxD->prox->prox->item.Chave] +
                        info->g.mat[auxA->item.Chave][d] + info->g.mat[a][auxD->prox->prox->item.Chave];
                        //calcula o ganho do custo caso seja feita a invers�o entre o endere�o A e o D
                aux = aux->prox; //avan�a com o valor do while mais interno
                dif = newgain - info->a.custo[k]; //calcula a diferen�a do custo da nova rota menos a original
                if(dif < gain){ //verifica se houve algum deficit (ganho de custo)
                    gain = dif;
                    best[0] = a; //guarda o valor de A
                    best[1] = d; //guarda o valor de D
                    p = k; //guarda o �ndice da rota da melhor invers�o
                    flag = 1;
                }
            }
            aux = aux->prox; //avan�a com o valor do la�o do meio
        }
        k++; //avan�a com o valor do while mais externo
    }
    if(gain < 0 && flag == 1){ //se a flag for 1, significa que encontrou alguma invers�o que reduzisse o custo
        info->custo += gain;
        info->a.custo[p] += gain;
        inverterList(info, best[0], best[1], p); //chama a fun��o auxiliar que inverte a lista do ponto A ao D
    }
    return gain; //retorna o valor do ganho, sempre negativo ou zero, caso n�o seja feito o movimento de vizinhan�a
}

int reInsertion(TInfo *info){
    int n = ceil((double)(info->g.n-1)/info->p); //calcula o n�mero de rotas que existe
    int i, j, p, k = 0, flag = 0;
    TCelula *auxA, *auxB, *aux;
    int a, b, newgain, gain = 0, dif = 0;
    int best[2];

    while(k < n){ //la�o para percorres todas as rotas
        auxA = info->a.rota[k].primeiro->prox; //pega a rota n
        auxB = auxA->prox;
        for(i = 1; auxA->prox != NULL && auxA->prox->item.Chave != 0; i++){ //la�o para percorrer todos os endere�os da rota n, verificando quando chega ao final ou encontra a sede (0)
            a = auxA->prox->item.Chave; //pega um valor na rota n
            aux = Pesquisar(a, info->a.rota[k]);
            for(j = 1; (auxB->item.Chave != 0 && auxB->prox != NULL); j++){ //la�o que percorre todos os endere�os inserindo o endere�o A
                b = auxB->item.Chave; //pega um segundo valor na rota n
                if(a == b){ //verifica se o endere�o A � igual ao B, para n�o re-inserir no mesmo lugar que ele j� est�
                    auxB = auxB->prox; //se for igual, ent�o avan�a com o valor de B
                    continue;
                }
                newgain = info->a.custo[k] -
                        info->g.mat[auxA->item.Chave][a] - info->g.mat[a][aux->prox->prox->item.Chave] - info->g.mat[b][auxB->prox->item.Chave] +
                        info->g.mat[auxA->item.Chave][aux->prox->prox->item.Chave] + info->g.mat[b][a] + info->g.mat[a][auxB->prox->item.Chave];
                        //calcula o ganho do custo caso seja feita a re-inser��o do elemento A na aresta de B
                auxB = auxB->prox; //avan�a com o valor de B, do la�o mais interno
                dif = newgain - info->a.custo[k]; //calcula a diferen�a do custo da nova rota menos a original
                if(dif < gain){ //verifica se houve algum deficit (ganho de custo)
                    gain = dif;
                    best[0] = a; //guarda o valor de A
                    best[1] = b; //guarda o valor de B
                    flag = 1;
                    p = k;
                }
            }
            auxB = info->a.rota[k].primeiro->prox->prox; //pega novamente a mesma rota
            auxA = auxA->prox; //avan�a com o valor do primeiro for
        }
        k++; //avan�a com a rota
    }
    if(gain < 0 && flag == 1){ //se a flag for 1, significa que encontrou alguma re-inser��o que reduzisse o custo
        info->custo += gain;
        info->a.custo[p] += gain;
        swapMemoryReInsertion(&info->a.rota[p], best[0], best[1]); //chama a fun��o auxiliar que re-insere o A na aresta de B
        flag = 0;
    }
    return gain; //retorna o valor do ganho, sempre negativo ou zero, caso n�o seja feito o movimento de vizinhan�a
}

void VND(TInfo *info, int r){
    int k = 1;
    int sInicial = 0, sMelhor = 0;

    while(k <= r){
        switch(k){
            case 1:
                sMelhor = swap(info);
                break;
            case 2:
                sMelhor = twoOpt(info);
                break;
            case 3:
                sMelhor = reInsertion(info);
                break;
        }
        if(sMelhor < sInicial){
            k = 1;
        }
        else{
            k += 1;
        }
    }
}

void MSG_MENU(){
    printf("\tAn�lise e Projeto de Algoritmos\n");
    printf("\n --- Menu:\n\n");
    printf("\t1. Heur�stica Construtiva - Vizinho mais pr�ximo\n");
    printf("\t2. Movimentos de Vizinhan�a\n");
    printf("\t3. Algoritmo de Busca Local - VND (Variable Neighborhood Descent)\n");
    printf("\t4. Criar arquivo de sa�da\n");
    printf("\t5. Imprimir rotas e custos\n");
    printf("\t6. Testar outro arquivo de entrada\n");
    printf("\t7. Sair\n");
}