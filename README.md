# mTSP
O Problema do Caixeiro Viajante múltiplo (MTSP) é uma extensão do famoso Problema do Caixeiro Viajante (TSP) que visita cada endereço exatamente uma vez sem sub-rotas. O MTSP envolve a atribuição de p agentes a n endereços, e cada endereço deve ser visitada por um agente, ao mesmo tempo que exige um custo total mínimo.

## Descrição do Projeto
Seja G = (V, A) um grafo onde, V é o conjunto de vértices que representa endereços, e A é o conjunto de arcos do grafo. Um arco (i, j) ∈ A representa o caminho que o agente deve percorrer para ir do endereço i ao j e o tempo (em minutos) para percorrer tal arco é dado por Tij . Além disso, a empresa pode estipular que qualquer agente pode realizar no máximo p questionários por dia. Sendo assim, nesse problema deseja-se atribuir um conjunto de endereços para cada um dos agentes e estabelecer a rota que eles deverão seguir (o percurso) de forma a minimizar o tempo total gasto e garantir que:
- (i) todos os endereços são visitados exatamente uma vez;
- (ii) cada agente inicia e termina sua rota na sede da empresa;
- (iii) a rota de cada um dos agentes não visita mais que p endereços. Considerando que os agentes são contratados conforme a demanda, vamos assumir que não existe um limite para o número de agentes disponíveis.

O projeto possui as seguintes implementações:
- Heurística Construtiva (Vizinho mais próximo)
- Movimentos de Vizinhança (Swap, 2-opt e Re-Insertion)
- Algoritmo de busca local VND (Variable Neighborhood Descent)

## Formato do arquivo de entrada
````
DIMENSION: <numero de vertices = n>
P: <numero maximo de endereços visitados em cada rota>
COST
 <matriz de adjacencia n x n contendo o custo/tempo para se percorrer cada uma das arestas>

````

## Instruções para compilação e execução
- Compilação
````
$ gcc main.c funcoes.c -o main
````
- Execução
````
$ main
````
