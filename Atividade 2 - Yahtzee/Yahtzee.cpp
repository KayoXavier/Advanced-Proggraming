#include <bits/stdc++.h> // Inclui bibliotecas padrão (comum em competições)
// Kayo Xavier Nascimento Cavalcante Leite - 21.2.4095
//Código implementado utilizando como referência o código encontrado no GitHub a seguir: https://github.com/evandrix/UVa/blob/master/10149.cpp

using namespace std;

// --- Constantes ---
#define STATE 8192 // 1 << 13: Representa os estados das 13 categorias usadas (bitmask)
#define STATE2 64  // Limite para a soma do bônus (0 a 63)
#define DSIZE 13   // Número de categorias / rodadas de dados

// --- Variáveis Globais ---
int DICE[DSIZE][5]; // Armazena as 13 rodadas de 5 dados cada

// --- Funções Auxiliares ---

// Calcula a pontuação para 5 dados (dice[]) na categoria 'cat' (1-13).
// Assume que 'dice[]' está ordenado.
int score_cat(int dice[], int cat) {
    int tot = 0, i, D[7]; // D[] é auxiliar para sequências
    switch (cat) {
        // Categorias 1-6: Soma dos dados com valor 'cat'
        case 1: case 2: case 3: case 4: case 5: case 6:
            for (i = 0; i < 5; i++) {
                if (dice[i] == cat) {
                    tot += cat;
                }
            }
            break;
        // Categoria 7: Chance (soma de todos os dados)
        case 7: 
            for (i = 0; i < 5; i++) {
                tot += dice[i];
            }
            break;
        // Categoria 8: Trinca (soma de todos se houver 3 iguais)
        case 8:
            if (dice[0] == dice[2] || dice[1] == dice[3] || dice[2] == dice[4]) { // Checa trinca em dados ordenados
                for (i = 0; i < 5; i++) tot += dice[i];
            }
            break;
        // Categoria 9: Quadra (soma de todos se houver 4 iguais)
        case 9:
            if (dice[0] == dice[3] || dice[1] == dice[4]) { // Checa quadra em dados ordenados
                for (i = 0; i < 5; i++) tot += dice[i];
            }
            break;
        // Categoria 10: Yahtzee/General (5 iguais)
        case 10:
            if (dice[0] == dice[4]) { // Checa 5 iguais em dados ordenados
                tot = 50;
            }
            break;
        // Categoria 11: Sequência Curta (4 dados em sequência)
        case 11:
            memset(D, 0, sizeof(D)); // Zera array auxiliar
            for (i = 0; i < 5; i++) D[dice[i]] = 1; // Marca presença dos dados
            for (i = 1; i <= 3; i++) { // Verifica sequências de 4 (1234, 2345, 3456)
                if (D[i] && D[i + 1] && D[i + 2] && D[i + 3]) tot = 25;
            }
            break;
        // Categoria 12: Sequência Longa (5 dados em sequência)
        case 12:
            for (i = 0; i < 4; i++) { // Verifica 12345 ou 23456 em dados ordenados
                if (dice[i] != dice[i + 1] - 1) return 0; // Se não for sequencial, retorna 0
            }
            tot = 35; // Pontuação fixa
            break;
        // Categoria 13: Full House (3 iguais + 2 iguais)
        case 13:
            // Checa as duas combinações possíveis em dados ordenados (AAABB ou AABBB)
            if ((dice[0] == dice[1] && dice[2] == dice[4]) || (dice[0] == dice[2] && dice[3] == dice[4])) {
                tot = 40;
            }
            break;
    }
    return tot;
}

// Conta quantos bits estão ativos (setados como 1) em um inteiro 'n'.
// Usado para verificar quantas categorias foram usadas em um estado 'mask'.
int cnt_bit(int n) {
    int count = 0;
    for (int i = 0; i < DSIZE; i++) {
        if ((n >> i) & 1) { // Verifica o i-ésimo bit
            count++;
        }
    }
    return count;
}

// --- Estruturas da Programação Dinâmica ---
// dp[mask][bonus_sum]: Pontuação máxima usando categorias do 'mask', com soma 'bonus_sum' na seção superior (1-6).
int dp[STATE][STATE2];
// score[k][j]: Pontuação pré-calculada da k-ésima rodada de dados (DICE[k]) para a categoria j+1.
int score[DSIZE][DSIZE];
// arg_dp[mask][bonus_sum][0]: Última categoria adicionada para chegar a este estado.
// arg_dp[mask][bonus_sum][1]: Soma do bônus do estado anterior. (Usado para backtracking).
int arg_dp[STATE][STATE2][2];

// --- Função Principal da Solução (Programação Dinâmica) ---
void sol_dp() {
    int i, j, k, p; // Índices de loop

    // 1. Pré-calcula todas as pontuações possíveis
    for (k = 0; k < DSIZE; k++) { // Para cada rodada de dados k
        for (j = 0; j < DSIZE; j++) { // Para cada categoria j
            score[k][j] = score_cat(DICE[k], j + 1); // Calcula e armazena
        }
    }

    // 2. Inicializa a DP
    memset(dp, -1, sizeof(dp)); // Marca todos os estados como não alcançados (-1)
    dp[0][0] = 0; // Estado inicial: nenhuma categoria usada (mask=0), soma do bônus=0, pontuação=0

    // 3. Preenche a tabela DP
    int current_score, new_state_mask, upper_section_score_increase, current_bonus_sum, new_bonus_sum;
    // Itera sobre o número de categorias já preenchidas (k)
    for (k = 0; k < DSIZE; k++) { 
        // Itera sobre todas as máscaras de estado possíveis (i)
        for (i = 0; i < STATE; i++) { 
            // Processa apenas estados que têm exatamente 'k' categorias preenchidas
            if (cnt_bit(i) == k) { 
                 // Itera sobre a próxima categoria (j) a ser preenchida (usando a k-ésima rodada de dados)
                for (j = 0; j < DSIZE; j++) {
                    // Verifica se a categoria 'j' NÃO está na máscara 'i' (ou seja, está livre)
                    if (!((i >> j) & 1)) { 
                        current_score = score[k][j]; // Pontuação obtida usando dados k na categoria j
                        new_state_mask = i | (1 << j); // Nova máscara com a categoria j adicionada
                        // Soma para o bônus (se j for categoria 1-6, senão 0)
                        upper_section_score_increase = (j < 6) ? current_score : 0; 

                        // Itera sobre as possíveis somas de bônus (p) do estado anterior (i)
                        for (current_bonus_sum = 0; current_bonus_sum < STATE2; current_bonus_sum++) {
                            // Se o estado anterior (i, p) for alcançável
                            if (dp[i][current_bonus_sum] >= 0) {
                                // Calcula a nova soma do bônus (limitada a 63)
                                new_bonus_sum = min(current_bonus_sum + upper_section_score_increase, 63);
                                // Verifica se encontramos um caminho melhor para o novo estado (nstate, new_bonus_sum)
                                if (dp[new_state_mask][new_bonus_sum] < dp[i][current_bonus_sum] + current_score) {
                                    dp[new_state_mask][new_bonus_sum] = dp[i][current_bonus_sum] + current_score; // Atualiza a pontuação máxima
                                    // Armazena informações para backtracking
                                    arg_dp[new_state_mask][new_bonus_sum][0] = j; // Qual categoria foi adicionada
                                    arg_dp[new_state_mask][new_bonus_sum][1] = current_bonus_sum; // Qual era a soma do bônus anterior
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // 4. Encontra a melhor pontuação final
    int max_total_score = 0;
    int final_bonus_sum_idx = 0; // Índice da soma de bônus que levou à pontuação máxima
    int bonus_points = 0;        // Pontos de bônus (0 ou 35)

    // Verifica pontuações finais sem o bônus explícito (soma < 63)
    for (i = 0; i < 63; i++) {
        if (dp[STATE - 1][i] > max_total_score) { // STATE-1 é a máscara com todas as 13 categorias preenchidas
            max_total_score = dp[STATE - 1][i];
            final_bonus_sum_idx = i;
        }
    }

    // Verifica se a pontuação COM o bônus (soma >= 63) é melhor
    // dp[STATE-1][63] representa estados onde a soma da seção superior foi >= 63
    if (dp[STATE - 1][63] >= 0 && dp[STATE - 1][63] + 35 > max_total_score) {
        max_total_score = dp[STATE - 1][63] + 35;
        final_bonus_sum_idx = 63; // Índice que representa bônus atingido
        bonus_points = 35;        // Adiciona os 35 pontos de bônus
    }

    // 5. Backtracking para reconstruir a solução
    int category_assignment[DSIZE]; // category_assignment[k] = categoria usada pela rodada k
    int final_scores[DSIZE];      // final_scores[j] = pontuação da categoria j
    int current_mask = STATE - 1; // Começa com todas as categorias preenchidas

    for (i = DSIZE - 1; i >= 0; i--) { // Itera das últimas rodadas para as primeiras
        int last_category_added = arg_dp[current_mask][final_bonus_sum_idx][0]; // Qual categoria foi adicionada por último
        int prev_bonus_sum = arg_dp[current_mask][final_bonus_sum_idx][1];    // Qual era a soma de bônus anterior

        category_assignment[i] = last_category_added; // Associa a rodada 'i' com a categoria 'last_category_added'
        // A pontuação da 'last_category_added' foi definida pela rodada 'i'
        final_scores[last_category_added] = score[i][last_category_added]; 

        final_bonus_sum_idx = prev_bonus_sum;        // Volta para o estado de bônus anterior
        current_mask ^= (1 << last_category_added); // Remove a categoria da máscara para voltar ao estado anterior
    }

    // 6. Imprime o resultado
    for (i = 0; i < DSIZE; i++) {
        printf("%d ", final_scores[i]); // Imprime a pontuação de cada categoria (índice 0-12)
    }
    printf("%d %d\n", bonus_points, max_total_score); // Imprime o bônus e a pontuação total
}

// --- Função Main ---
int main() {
    int i, j;
    while (true) { // Loop infinito, espera EOF para parar
        // Lê as 13 rodadas de 5 dados
        for (i = 0; i < DSIZE; i++) {
            for (j = 0; j < 5; j++) {
                if (scanf("%d", &DICE[i][j]) != 1) { // Se a leitura falhar (EOF)
                    return 0; // Termina o programa
                }
            }
            sort(DICE[i], DICE[i] + 5); // Ordena os 5 dados de cada rodada (necessário para score_cat)
        }
        sol_dp(); // Resolve o problema para o conjunto de dados lido
    }
    return 0;
}