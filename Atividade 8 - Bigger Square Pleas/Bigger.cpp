#include <bits/stdc++.h> // Inclui bibliotecas padrão (comum em maratonas)
// Kayo Xavier Nascimento Cavalcante Leite - 21.2.4095
//Código implementado utilizando como referência o código encontrado a seguir: https://github.com/evandrix/UVa/blob/master/10270.cpp
using namespace std;

const int MAX_N = 50; // Tamanho máximo do lado do quadrado principal
// Grade booleana para representar o quadrado N x N.
// true = ocupado/barreira, false = livre.
// Tamanho extra (+3) para facilitar o manejo das bordas/barreiras.
bool Square[MAX_N + 3][MAX_N + 3];

// Cria uma barreira de celulas ocupadas ao redor da grade N x N.
// Isso simplifica as verificacoes de limite dentro das funcoes de colocacao.
void BuildBarrier(int N) {
    for (int i = 0; i <= N + 1; i++) {
        Square[0][i] = true;       // Barreira superior
        Square[N + 1][i] = true; // Barreira inferior
        Square[i][0] = true;       // Barreira esquerda
        Square[i][N + 1] = true; // Barreira direita
    }
}

// Preenche (ou limpa) uma regiao quadrada de tamanho 'w' na grade 'Square'.
// Comeca na posicao (i0, j0) e define o valor booleano 'v'.
void SetUpSquare(int i0, int j0, int w, bool v) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < w; j++) {
            Square[i0 + i][j0 + j] = v; // Define o valor 'v' para a celula
        }
    }
}

// Verifica se uma regiao quadrada de tamanho 'w' com canto superior esquerdo em (i0, j0)
// esta completamente livre (contem apenas 'false').
bool IsPlaceClear(int i0, int j0, int w) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < w; j++) {
            if (Square[i0 + i][j0 + j]) { // Se encontrar alguma celula ocupada ('true')
                return false; // A regiao nao esta livre
            }
        }
    }
    return true; // Nenhuma celula ocupada foi encontrada
}

// Estrutura para guardar informacoes de um quadrado colocado:
// posicao (i, j) do canto superior esquerdo e tamanho N.
struct Trace {
    int i, j; // Coordenadas (linha, coluna)
    int N;    // Tamanho do lado do quadrado
};

// Array global para armazenar o "traco" da solucao encontrada pela simulacao.
Trace tr[MAX_N * MAX_N];

// Estrutura para armazenar a contagem de quadrados disponiveis de cada tamanho.
// 'c[n]' guarda quantos quadrados de tamanho 'n' ainda podem ser usados.
struct Place_t {
    char c[MAX_N + 1]; // Usa char para economizar memoria, assume que a contagem e pequena
};

// Encontra a proxima posicao (i0, j0) livre na grade N x N.
// A busca comeca a partir da posicao seguinte a (i, j), varrendo linha por linha.
// A logica original de busca (com r e k) e mantida, apesar de parecer complexa.
void FindNextSquarePosition(int i, int j, int n, int N, int *i0, int *j0) {
    int r = (i - 1) * N + (j - 1); // Indice linear aproximado da posicao atual
    // Busca linear a partir de um deslocamento 'k' (iniciado em 'n'?)
    for (int k = 0; k < N * N; k++) { // Busca exaustiva por seguranca
        *i0 = 1 + (r + k) / N;     // Calcula linha candidata
        *j0 = 1 + ((r + k) % N); // Calcula coluna candidata
        // Verifica se a posicao e valida e livre
        if (*i0 >= 1 && *i0 <= N && *j0 >= 1 && *j0 <= N) {
             if (!Square[*i0][*j0]) {
                break; // Encontrou a proxima posicao livre
             }
        }
        // Se (r+k) exceder a area, pode indicar que nao ha mais espaco
        if (r + k >= N * N -1) {
             *i0 = N + 1; // Sinaliza que nao encontrou (fim da grade)
             *j0 = N + 1;
             break;
        }
    }
}


Place_t Squares; // Variavel global para contagem de quadrados disponiveis

// Funcao recursiva (backtracking) para simular a COLOCACAO FISICA de um conjunto
// de quadrados (definido externamente em 'Squares') na grade N x N.
// Verifica se um dado conjunto de quadrados pode ladrilhar a grade.
// 'best' e o numero total de quadrados no conjunto.
// 'depth' e a profundidade da recursao (quantos quadrados ja foram colocados).
// (i, j) e a posicao onde tentar colocar o proximo quadrado.
bool Simulate_Placing(int N, int best, int i, int j, int depth = 0) {
    // Caso base: Se a proxima posicao a verificar esta fora da grade,
    // significa que preenchemos tudo com sucesso.
    if (i > N || j > N) {
        return true;
    }

    // Caso especial da primeira chamada (depth == 0), geralmente envolvendo
    // a colocacao inicial de 3 quadrados especificos (n, N-n, N-n).
    if (depth == 0) {
        // Tenta a configuracao inicial com o maior 'n' disponivel
        for (int n = N - 1; n >= 1; n--) {
            if (Squares.c[n] > 0 && Squares.c[N - n] >= 2) { // Verifica disponibilidade
                // Guarda no traco
                tr[0].i = i; tr[0].j = j; tr[0].N = n;
                tr[1].i = i + n; tr[1].j = j; tr[1].N = N - n;
                tr[2].i = i; tr[2].j = j + n; tr[2].N = N - n;
                // Marca na grade
                SetUpSquare(i, j, n, true);
                SetUpSquare(i + n, j, N - n, true);
                SetUpSquare(i, j + n, N - n, true);
                // Atualiza contagem
                Squares.c[n] -= 1;
                Squares.c[N - n] -= 2;
                // Acha proxima posicao
                int i0, j0;
                FindNextSquarePosition(i, j, n, N, &i0, &j0);
                // Chama recursao
                if (Simulate_Placing(N, best, i0, j0, depth + 3)) {
                    return true;
                }
                // Backtrack
                Squares.c[n] += 1;
                Squares.c[N - n] += 2;
                SetUpSquare(i, j, n, false);
                SetUpSquare(i + n, j, N - n, false);
                SetUpSquare(i, j + n, N - n, false);
                break; // So tenta o maior 'n' (conforme logica original)
            }
        }
        return false; // Falhou na colocacao inicial
    }
    else { // Caso geral (depth > 0) - colocar o proximo quadrado
        // Encontra o maior tamanho 'Nr' que cabe na posicao (i, j)
        int Nr = N;
        while (Nr >= 1 && !IsPlaceClear(i, j, Nr)) {
             Nr--;
        }
        if (Nr < 1) return false; // Nao cabe nenhum quadrado aqui? (Inesperado se (i,j) estava livre)

        // Tenta colocar quadrados de tamanho 'n' (do maior que cabe para baixo)
        for (int n = Nr; n >= 1; n--) {
            if (Squares.c[n] > 0) { // Se ha quadrados disponiveis
                // Guarda no traco
                tr[depth].i = i;
                tr[depth].j = j;
                tr[depth].N = n;
                // Marca na grade
                SetUpSquare(i, j, n, true);
                Squares.c[n]--;
                // Acha proxima posicao
                int i0, j0;
                FindNextSquarePosition(i, j, n, N, &i0, &j0);
                // Chama recursao
                if (Simulate_Placing(N, best, i0, j0, depth + 1)) {
                    return true;
                }
                // Backtrack
                SetUpSquare(i, j, n, false);
                Squares.c[n]++;
            }
        }
        return false; // Nenhuma opcao funcionou a partir daqui
    }
}


// Funcoes max e min simples
int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

// Funcao recursiva (backtracking) para ESCOLHER O CONJUNTO de quadrados.
// Tenta encontrar um conjunto de 'best' quadrados cuja area total seja N*N.
// 'S' e a area restante a ser preenchida.
// 'start0' otimiza a busca, limitando os tamanhos a serem testados.
// 'used' e o numero de quadrados ja escolhidos no conjunto.
bool Place(int N, int best, int S = 0, int start0 = -1, int used = 0) {
    // Condicoes de Parada / Poda
    if (S < 0 || used > best) {
        return false;
    }
    if (used == best) { // Numero correto de quadrados?
        if (S == 0) { // Area correta?
            // Verifica se este conjunto pode ser fisicamente arranjado
            return Simulate_Placing(N, best, 1, 1);
        } else {
            return false;
        }
    }

    // Passo Recursivo
    if (used == 0) { // Primeira chamada - caso especial de 3 pecas
        S = N * N;
        // Testa a combinacao inicial (1 de tamanho 'i', 2 de tamanho 'N-i')
        for (int i = max(1, N / 2); i < N; i++) {
            if (S - i * i - 2 * (N - i) * (N - i) >= 0 && used + 3 <= best) {
                Squares.c[i] += 1;
                Squares.c[N - i] += 2;
                // Chama recursao para o restante
                if (Place(N, best, S - i * i - 2 * (N - i) * (N - i), N - i - 1, used + 3)) {
                    return true;
                }
                // Backtrack
                Squares.c[N - i] -= 2;
                Squares.c[i] -= 1;
            }
        }
    } else { // Chamadas subsequentes - adiciona mais quadrados
        // Testa adicionar 'k' quadrados de tamanho 'i'
        for (int i = start0; i >= 1; i--) {
            for (int k = min(5, S / (i * i)); k >= 1; k--) { // Limite 'k' em 5?
                 if (used + k <= best) {
                    Squares.c[i] += k;
                    // Chama recursao
                    if (Place(N, best, S - k * i * i, i - 1, used + k)) {
                        return true;
                    }
                    // Backtrack
                    Squares.c[i] -= k;
                 }
            }
        }
    }
    return false; // Nenhuma combinacao funcionou
}

// Bloco de Resultados Pre-calculados - NAO MODIFICAR/COMENTAR ESTE BLOCO
char Results[MAX_N + 1][512] =
{
	"",
	"",
	"4\n1 1 1\n2 1 1\n1 2 1\n2 2 1\n",
	"6\n1 1 2\n3 1 1\n1 3 1\n2 3 1\n3 2 1\n3 3 1\n",
	"4\n1 1 2\n3 1 2\n1 3 2\n3 3 2\n",
	"8\n1 1 3\n4 1 2\n1 4 2\n3 4 2\n4 3 1\n5 3 1\n5 4 1\n5 5 1\n",
	"4\n1 1 3\n4 1 3\n1 4 3\n4 4 3\n",
	"9\n1 1 4\n5 1 3\n1 5 3\n4 5 2\n4 7 1\n5 4 1\n5 7 1\n6 4 2\n6 6 2\n",
	"4\n1 1 4\n5 1 4\n1 5 4\n5 5 4\n",
	"6\n1 1 6\n7 1 3\n1 7 3\n4 7 3\n7 4 3\n7 7 3\n",
	"4\n1 1 5\n6 1 5\n1 6 5\n6 6 5\n",
	"11\n1 1 6\n7 1 5\n1 7 5\n6 7 2\n6 9 2\n6 11 1\n7 6 1\n7 11 1\n8 6 4\n8 10 2\n10 10 2\n",
	"4\n1 1 6\n7 1 6\n1 7 6\n7 7 6\n",
	"11\n1 1 7\n8 1 6\n1 8 6\n7 8 2\n7 10 4\n8 7 1\n9 7 3\n11 10 1\n11 11 3\n12 7 2\n12 9 2\n",
	"4\n1 1 7\n8 1 7\n1 8 7\n8 8 7\n",
	"6\n1 1 10\n11 1 5\n1 11 5\n6 11 5\n11 6 5\n11 11 5\n",
	"4\n1 1 8\n9 1 8\n1 9 8\n9 9 8\n",
	"12\n1 1 9\n10 1 8\n1 10 8\n9 10 2\n9 12 4\n9 16 2\n10 9 1\n11 9 3\n11 16 2\n13 12 1\n13 13 5\n14 9 4\n",
	"4\n1 1 9\n10 1 9\n1 10 9\n10 10 9\n",
	"13\n1 1 10\n11 1 9\n1 11 9\n10 11 2\n10 13 7\n11 10 1\n12 10 3\n15 10 3\n17 13 1\n17 14 3\n17 17 3\n18 10 2\n18 12 2\n",
	"4\n1 1 10\n11 1 10\n1 11 10\n11 11 10\n",
	"6\n1 1 14\n15 1 7\n1 15 7\n8 15 7\n15 8 7\n15 15 7\n",
	"4\n1 1 11\n12 1 11\n1 12 11\n12 12 11\n",
	"13\n1 1 12\n13 1 11\n1 13 11\n12 13 2\n12 15 5\n12 20 4\n13 12 1\n14 12 3\n16 20 1\n16 21 3\n17 12 7\n17 19 2\n19 19 5\n",
	"4\n1 1 12\n13 1 12\n1 13 12\n13 13 12\n",
	"8\n1 1 15\n16 1 10\n1 16 10\n11 16 10\n16 11 5\n21 11 5\n21 16 5\n21 21 5\n",
	"4\n1 1 13\n14 1 13\n1 14 13\n14 14 13\n",
	"6\n1 1 18\n19 1 9\n1 19 9\n10 19 9\n19 10 9\n19 19 9\n",
	"4\n1 1 14\n15 1 14\n1 15 14\n15 15 14\n",
	"14\n1 1 15\n16 1 14\n1 16 14\n15 16 2\n15 18 5\n15 23 7\n16 15 1\n17 15 3\n20 15 3\n20 18 3\n20 21 2\n22 21 1\n22 22 8\n23 15 7\n",
	"4\n1 1 15\n16 1 15\n1 16 15\n16 16 15\n",
	"15\n1 1 16\n17 1 15\n1 17 15\n16 17 2\n16 19 2\n16 21 11\n17 16 1\n18 16 5\n23 16 5\n27 21 1\n27 22 5\n27 27 5\n28 16 4\n28 20 2\n30 20 2\n",
	"4\n1 1 16\n17 1 16\n1 17 16\n17 17 16\n",
	"6\n1 1 22\n23 1 11\n1 23 11\n12 23 11\n23 12 11\n23 23 11\n",
	"4\n1 1 17\n18 1 17\n1 18 17\n18 18 17\n",
	"8\n1 1 21\n22 1 14\n1 22 14\n15 22 14\n22 15 7\n29 15 7\n29 22 7\n29 29 7\n",
	"4\n1 1 18\n19 1 18\n1 19 18\n19 19 18\n",
	"15\n1 1 19\n20 1 18\n1 20 18\n19 20 2\n19 22 5\n19 27 4\n19 31 7\n20 19 1\n21 19 3\n23 27 1\n23 28 3\n24 19 7\n24 26 2\n26 26 12\n31 19 7\n",
	"4\n1 1 19\n20 1 19\n1 20 19\n20 20 19\n",
	"6\n1 1 26\n27 1 13\n1 27 13\n14 27 13\n27 14 13\n27 27 13\n",
	"4\n1 1 20\n21 1 20\n1 21 20\n21 21 20\n",
	"15\n1 1 23\n24 1 18\n1 24 18\n19 24 7\n19 31 11\n24 19 3\n24 22 2\n26 22 1\n26 23 5\n26 28 3\n27 19 4\n29 28 2\n29 30 1\n30 30 12\n31 19 11\n",
	"4\n1 1 21\n22 1 21\n1 22 21\n22 22 21\n",
	"16\n1 1 22\n23 1 21\n1 23 21\n22 23 2\n22 25 5\n22 30 7\n22 37 7\n23 22 1\n24 22 3\n27 22 3\n27 25 3\n27 28 2\n29 28 1\n29 29 15\n30 22 7\n37 22 7\n",
	"4\n1 1 22\n23 1 22\n1 23 22\n23 23 22\n",
	"6\n1 1 30\n31 1 15\n1 31 15\n16 31 15\n31 16 15\n31 31 15\n",
	"4\n1 1 23\n24 1 23\n1 24 23\n24 24 23\n",
	"16\n1 1 24\n25 1 23\n1 25 23\n24 25 2\n24 27 1\n24 28 6\n24 34 14\n25 24 1\n25 27 1\n26 24 4\n30 24 10\n38 34 4\n38 38 10\n40 24 8\n40 32 2\n42 32 6\n",
	"4\n1 1 24\n25 1 24\n1 25 24\n25 25 24\n",
	"9\n1 1 28\n29 1 21\n1 29 21\n22 29 14\n22 43 7\n29 22 7\n29 43 7\n36 22 14\n36 36 14\n",
	"4\n1 1 25\n26 1 25\n1 26 25\n26 26 25\n"
};


// Enum para definir o modo de operacao
enum TYPES {
    GENERATION, // Modo para gerar/encontrar as solucoes
    JUDGE       // Modo para usar as solucoes pre-calculadas
};

// Define o modo atual de operacao do programa
const int TYPE = JUDGE; // Alterar para GENERATION se quiser gerar as solucoes

// Variaveis usadas apenas no modo GENERATION para armazenar/processar a solucao
Trace solution[MAX_N + 1][MAX_N];    // Guarda a solucao (posicoes/tamanhos) para cada N
int solution_len[MAX_N + 1];         // Guarda o numero de quadrados na solucao para cada N
char Solution_Chart[MAX_N + 2][MAX_N + 2]; // Grade para visualizacao da solucao gerada

int main() {
    // --- Modo de Geracao ---
    if (TYPE == GENERATION) {
        printf("Gerando solucoes...\n");
        int bestPrime = 2; // Guarda o numero minimo de quadrados (comeca buscando a partir de 2)
        // Loop para gerar solucoes para N de 2 ate MAX_N
        for (int i = 2; i <= MAX_N; i++) {
            printf("%.2i : ", i);
            // Encontra o menor divisor de 'i' (otimizavel)
            int div = 2;
            while (div < i && i % div != 0) {
                div++;
            }

            // Se 'i' for composto (encontrou divisor < i)
            if (div < i) {
                 printf("Composto (fator %d). Escalando solucao...\n", div);
                 // Deriva a solucao para 'i' escalando a solucao do divisor 'div'
                 // Assume que a solucao para N composto pode ser construida a partir da solucao de seus fatores.
                 solution_len[i] = solution_len[div]; // Mesmo numero de quadrados
                 for (int k = 0; k < solution_len[i]; k++) {
                      // Escala as coordenadas e tamanhos proporcionalmente
                      solution[i][k].i = 1 + (solution[div][k].i - 1) * i / div;
                      solution[i][k].j = 1 + (solution[div][k].j - 1) * i / div;
                      solution[i][k].N = (solution[div][k].N) * i / div;
                 }
            }
            // Se 'i' for primo ou caso base (menor divisor e ele mesmo)
            else {
                printf("Primo/Base. Buscando solucao...\n");
                // Prepara a grade e reseta contadores
                BuildBarrier(i);
                for (int k = 0; k <= MAX_N; k++) Squares.c[k] = 0; // Reseta contagem

                // Procura pelo menor numero ('bestPrime') de quadrados que ladrilham 'i'
                bestPrime = 2; // Reseta a busca pelo minimo
                while (true) {
                     printf(" Tentando com %d quadrados...\n", bestPrime);
                     // Chama a funcao 'Place' que tenta achar um CONJUNTO de 'bestPrime'
                     // quadrados que some a area correta e que possa ser ENCAIXADO
                     // (atraves da chamada interna a Simulate_Placing).
                     if (Place(i, bestPrime)) {
                         printf("  Sucesso com %d quadrados!\n", bestPrime);
                         break; // Encontrou a solucao otima (menor K) para 'i'
                     }
                     bestPrime++; // Se nao deu, tenta com K+1 quadrados
                     if (bestPrime > 50) { // Limite de seguranca razoavel
                          printf("  Limite de busca atingido!\n"); break;
                     }
                }
                // Armazena a solucao encontrada no traco 'tr' para o array 'solution'
                solution_len[i] = bestPrime;
                for (int k = 0; k < solution_len[i]; k++) {
                    solution[i][k] = tr[k]; // Copia direta da solucao encontrada
                }
            }

            // Imprime a solucao no formato necessario para colar no array 'Results'
            printf("\"%i\\n", solution_len[i]);
            for (int k = 0; k < solution_len[i]; k++) {
                 printf("%i %i %i\\n", solution[i][k].i, solution[i][k].j, solution[i][k].N);
            }
            printf("\",\n");

            // Codigo opcional para gerar visualizacao da solucao (comentado)
            /* ... codigo de visualizacao ... */
        }
    }
    // --- Modo de Julgamento ---
    else {
        int testCases;
        scanf("%i", &testCases); // Le numero de casos de teste
        int n;
        while (testCases--) {
            scanf("%i", &n); // Le o tamanho N do quadrado para o caso de teste
            // Imprime o resultado pre-calculado diretamente do array 'Results'
            printf("%s", Results[n]);
        }
        return 0; // Fim do programa no modo Judge
    }
    return 0; // Fim do programa no modo Generation
}