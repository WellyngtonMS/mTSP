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

ssize_t getline(char **lineptr, size_t *n, FILE *stream); //Função getline da biblioteca C++ adaptada para C e que usem sistema Windows
void criaGrafo(TGrafo *g, int n); //Função para alocação de memória do grafo (matriz quadrada)
void lerArquivo(char *nomeArquivo, TInfo *info); //Função de leitura do arquivo padrão
void criarArquivo(TInfo info); //Função que gera o arquivo de saída com custos e rotas
void alocarRota(TInfo *info); //Função para alocação de memória das rotas
void Inserir(int x, TLista *Lista); //Função de inserção de um item numa lista encadeada
void custos(TInfo *info); //Função que calcula os custos de cada rota de cada agente respectivamente
void heuristica(TInfo *info); //Implementação do heurística construtiva do Vizinho mais próximo
int Vazia(TLista Lista); //Função para verificar se uma lista está vazia
TCelula* Pesquisar(int item, TLista lista); //Retorna um ponteiro para a célula anterior à do endereço procurado
void swapValue(TLista *lista, int x, int y); //Função auxiliar do SWAP que faz apenas a troca de valores
void inverterList(TInfo *info, int a, int b, int k); //Função auxiliar do 2-Opt que inverte parte de uma lista encadeada
void swapMemoryReInsertion(TLista *lista, int a, int b); //Função auxiliar do reInsertion que reorganiza os ponteiros da lista
int swap(TInfo *info); //Faz a melhor troca entre dois valores dentre as rotas
int twoOpt(TInfo *info); //Faz a melhor inversão de endereços dentre as rotas
int reInsertion(TInfo *info); //Faz a melhor re-inserção de endereço na melhor rota
void VND(TInfo *info, int r); //Variable Neighbourhood Descent Algorithm a(1,2)
void MSG_MENU(); //Mensagens do menu interativo