typedef struct{
    int Chave;
}TipoItem;

typedef struct celula{
    TipoItem item;
    struct celula* prox;
}TCelula;

typedef struct lista{
    TCelula* primeiro;
    TCelula* ultimo;
}TLista; //Estrutura de lista encadeada

typedef struct grafo{
    int n;
    int **mat;
}TGrafo; //Estrutura do grafo (matriz quadrada)

typedef struct agente{
    int nAgente;
    TLista *rota;
    int *custo;
}TAgente; //Estrutura dos agentes

typedef struct info{
    int custo;
    int p;
    int *visitados;
    TAgente a;
    TGrafo g;
    char nome[50];
}TInfo; //Estrutura do programa

ssize_t getline(char **lineptr, size_t *n, FILE *stream); //Fun��o getline da biblioteca C++ adaptada para C e que usem sistema Windows
void criaGrafo(TGrafo *g, int n); //Fun��o para aloca��o de mem�ria do grafo (matriz quadrada)
void lerArquivo(char *nomeArquivo, TInfo *info); //Fun��o de leitura do arquivo padr�o
void criarArquivo(TInfo info); //Fun��o que gera o arquivo de sa�da com custos e rotas
void alocarRota(TInfo *info); //Fun��o para aloca��o de mem�ria das rotas
void Inserir(int x, TLista *Lista); //Fun��o de inser��o de um item numa lista encadeada
void custos(TInfo *info); //Fun��o que calcula os custos de cada rota de cada agente respectivamente
void heuristica(TInfo *info); //Implementa��o do heur�stica construtiva do Vizinho mais pr�ximo
int Vazia(TLista Lista); //Fun��o para verificar se uma lista est� vazia
TCelula* Pesquisar(int item, TLista lista); //Retorna um ponteiro para a c�lula anterior � do endere�o procurado
void swapValue(TLista *lista, int x, int y); //Fun��o auxiliar do SWAP que faz apenas a troca de valores
void inverterList(TInfo *info, int a, int b, int k); //Fun��o auxiliar do 2-Opt que inverte parte de uma lista encadeada
void swapMemoryReInsertion(TLista *lista, int a, int b); //Fun��o auxiliar do reInsertion que reorganiza os ponteiros da lista
int swap(TInfo *info); //Faz a melhor troca entre dois valores dentre as rotas
int twoOpt(TInfo *info); //Faz a melhor invers�o de endere�os dentre as rotas
int reInsertion(TInfo *info); //Faz a melhor re-inser��o de endere�o na melhor rota
void VND(TInfo *info, int r); //Variable Neighbourhood Descent Algorithm a(1,2)
void MSG_MENU(); //Mensagens do menu interativo