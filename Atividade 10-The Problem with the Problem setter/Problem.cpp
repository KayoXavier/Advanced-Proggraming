#include <bits/stdc++.h> // Inclui bibliotecas padrao (comum em maratonas)

using namespace std;

// --- Variaveis Globais ---
// Grafo representado como lista de adjacencia com capacidades
// graph[u][v] = capacidade da aresta u -> v
unordered_map<int, unordered_map<int, int>> graph;
// Vetor para armazenar o caminho encontrado pela BFS (pai de cada no)
vector<int> parents;
// k: numero de categorias, p: numero de problemas
int k, p;
// Variaveis auxiliares para leitura
int x, v;
// f: fluxo encontrado no caminho atual (augmenting path)
int f;

// --- Funcao para Aumentar o Fluxo ---
// Percorre o caminho encontrado pela BFS (de tras para frente, do sumidouro para a fonte)
// e atualiza as capacidades residuais das arestas e arestas reversas.
// 'cur' e o no atual no caminho.
// 'minEdge' e a capacidade gargalo encontrada ate o momento no caminho.
void augment(int cur, int minEdge) {
    // Caso base: Chegou na fonte (no 0)
    if (cur == 0) {
        f = minEdge; // Define 'f' como a capacidade gargalo do caminho completo
        return;      // Encerra a recursao para este caminho
    } else if (parents[cur] != -1) { // Se o no atual tem um pai no caminho encontrado
        int par = parents[cur]; // Pega o no pai
        // Chama recursivamente para o pai, passando o minimo entre a capacidade gargalo atual
        // e a capacidade da aresta pai -> cur. Isso garante que 'minEdge' seja
        // a menor capacidade em todo o caminho ate a fonte.
        augment(par, min(minEdge, graph[par][cur]));
        // Apos a recursao retornar (e 'f' ter sido definido na fonte):
        // Atualiza as capacidades residuais:
        graph[par][cur] -= f; // Diminui capacidade da aresta direta (fluxo passou por aqui)
        graph[cur][par] += f; // Aumenta capacidade da aresta reversa (permite "desfazer" o fluxo se necessario)
    }
}

// --- Funcao Principal ---
int main() {
    // Loop principal: le K e P ate que ambos sejam 0
    while (scanf("%d %d", &k, &p), (k + p) != 0) {
        graph.clear(); // Limpa o grafo para o novo caso de teste

        // --- Construcao do Grafo de Fluxo ---
        // Mapeamento dos nos:
        // 0: Fonte (source)
        // 1: Sumidouro (sink)
        // 2 a K+1: Nos representando as categorias
        // K+2 a K+P+1: Nos representando os problemas

        int problemsNeeded = 0; // Contador do total de problemas necessarios

        // Le a demanda de cada categoria e cria arestas Fonte -> Categoria
        for (int i = 0; i < k; i++) {
            scanf("%d", &v); // v = numero de problemas necessarios para categoria i
            // Aresta da Fonte (0) para o no da Categoria (2+i) com capacidade 'v'
            graph[0][2 + i] = v;
            problemsNeeded += v; // Acumula o total de problemas necessarios
        }

        // Le as categorias de cada problema e cria arestas Categoria -> Problema e Problema -> Sumidouro
        for (int i = 0; i < p; i++) {
            scanf("%d", &x); // x = numero de categorias que o problema i pertence
            while (x--) {
                scanf("%d", &v); // v = indice da categoria (base 1)
                v--;             // Ajusta para indice base 0 (correspondendo aos nos 2 a K+1)
                // Aresta da Categoria (2+v) para o Problema (2+k+i) com capacidade 1
                // Indica que este problema pode satisfazer uma unidade da demanda desta categoria.
                graph[2 + v][2 + k + i] = 1;
            }
            // Aresta do Problema (2+k+i) para o Sumidouro (1) com capacidade 1
            // Indica que cada problema pode ser "usado" (alocado) no maximo uma vez.
            graph[2 + k + i][1] = 1;
        }

        // --- Calculo do Fluxo Maximo (Algoritmo de Edmonds-Karp) ---
        int mf = 0; // Fluxo maximo total encontrado
        while (1) { // Loop principal do Edmonds-Karp
            f = 0; // Reseta o fluxo encontrado no caminho atual

            // Prepara para a busca em largura (BFS)
            vector<bool> visited(2 + k + p); // Vetor de nos visitados na BFS atual
            queue<int> bfs;                  // Fila para a BFS
            parents.assign(2 + k + p, -1);   // Reseta o vetor de pais (nenhum caminho encontrado ainda)

            // Inicia a BFS a partir da fonte (0)
            bfs.push(0);
            visited[0] = true;

            // Executa a BFS para encontrar um caminho aumentante
            while (!bfs.empty()) {
                int cur = bfs.front(); bfs.pop(); // Pega o proximo no da fila
                // Se chegou ao sumidouro (1), um caminho foi encontrado
                if (cur == 1) break;

                // Explora os vizinhos do no atual 'cur'
                // 'neigh.first' e o no vizinho, 'neigh.second' e a capacidade cur -> neigh.first
                for (auto& neigh : graph[cur]) {
                    // Se existe capacidade residual (neigh.second > 0) e o vizinho ainda nao foi visitado
                    if (neigh.second > 0 && !visited[neigh.first]) {
                        parents[neigh.first] = cur;     // Define o pai do vizinho como 'cur' (marca o caminho)
                        visited[neigh.first] = true;    // Marca o vizinho como visitado
                        bfs.push(neigh.first);         // Adiciona o vizinho na fila para explorar depois
                    }
                }
            }

            // Apos a BFS, chama 'augment' para calcular 'f' (gargalo do caminho)
            // e atualizar o fluxo e as capacidades residuais no grafo.
            // Comeca do sumidouro (1) e vai voltando pelos pais ate a fonte.
            // 1e7 (10 milhoes) e usado como infinito inicial para o gargalo.
            augment(1, 1e7);

            // Se f == 0 apos chamar augment, significa que a BFS nao encontrou
            // caminho da fonte ao sumidouro (nenhum caminho aumentante existe mais).
            if (f == 0) break; // Termina o loop do Edmonds-Karp

            mf += f; // Adiciona o fluxo encontrado neste caminho ao fluxo maximo total
        }

        // --- Verificacao e Impressao do Resultado ---
        // Se o fluxo maximo for igual ao total de problemas necessarios, a alocacao e possivel.
        if (mf != problemsNeeded) {
            printf("0\n"); // Nao e possivel satisfazer todas as demandas
        } else {
            printf("1\n"); // E possivel satisfazer todas as demandas
            // Imprime a alocacao de problemas para cada categoria
            for (int i = 0; i < k; i++) { // Para cada categoria 'i' (no 2+i)
                bool first = true; // Flag para controlar espacos na saida
                // Itera sobre os vizinhos do no da categoria 'i'
                // Estes vizinhos sao potencialmente os problemas associados a ela.
                for (auto& neigh : graph[2 + i]) {
                    // Para saber se o problema 'neigh.first' foi ALOCADO para a categoria '2+i',
                    // verificamos a ARESTA REVERSA no grafo residual.
                    // Se graph[problema][categoria] > 0, significa que fluxo passou de
                    // categoria para problema, e a aresta reversa ganhou capacidade.
                    int problema_node = neigh.first;
                    int categoria_node = 2 + i;
                    // Verifica se e um no de problema e se ha fluxo reverso
                    if (problema_node >= 2 + k && graph[problema_node][categoria_node] > 0) {
                        if (!first) printf(" "); // Imprime espaco antes do segundo problema em diante
                        first = false;
                        // Imprime o indice do problema (base 1)
                        // Indice do problema = no_problema - (k+2) + 1 = no_problema - k - 1
                        printf("%d", problema_node - k - 1);
                    }
                }
                printf("\n"); // Nova linha para a proxima categoria
            }
        }
    }
    return 0; // Fim do programa
}