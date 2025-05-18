#include <bits/stdc++.h> // Inclui bibliotecas padrão (comum em maratonas)
// Kayo Xavier Nascimento Cavalcante Leite - 21.2.4095
//Código implementado utilizando como referência o código encontrado a seguir: https://github.com/evandrix/UVa/blob/master/10202.cpp
using namespace std; // Usa o namespace std

#define MAX 10       // Tamanho máximo de N
typedef long int Int; // Apelido para long int

// --- Variáveis Globais ---
Int a[MAX * MAX];    // Armazena as somas S_ij = N_i + N_j da entrada
Int b[MAX];          // Armazena os números originais N_1...N_n (solução)
int n;               // Número de inteiros originais (N)
int n2;              // Total de somas = N * (N - 1) / 2
// Arrays para simular uma lista duplamente encadeada dos *índices* de 'a'
int prev_[MAX * MAX]; // Ponteiro "anterior" da lista (índice do nó anterior)
int nxt[MAX * MAX];  // Ponteiro "próximo" da lista (índice do nó seguinte)
int first_item;      // Índice da menor soma atual na lista
int search_start;    // Otimização: índice onde iniciar a busca na lista

// Inicializa a lista encadeada com os índices das somas (de 2 a n2-1).
// Índices 0 e 1 são tratados separadamente. 0 indica fim da lista.
void init_list() {
    prev_[2] = 0;      // Índice 2 é o início lógico da lista
    nxt[2] = 3;        // Próximo do 2 é o 3
    for (int i = 3; i < n2; i++) {
        prev_[i] = i - 1; // Conecta anteriores
        nxt[i] = i + 1;  // Conecta próximos
    }
    first_item = 2; // Menor soma (após a[0], a[1]) está no índice 2
}

// Remove o nó de índice 'i' da lista encadeada, ajustando os vizinhos.
void remove(int i) {
    if (prev_[i] == 0) { // Se 'i' é o primeiro item
        prev_[nxt[i]] = 0; // O próximo se torna o novo primeiro
        first_item = nxt[i];
    } else { // Se 'i' está no meio/fim
        nxt[prev_[i]] = nxt[i]; // Anterior aponta para o próximo
        if (nxt[i] < n2) { // Evita acesso inválido se nxt[i] for o fim lógico
             prev_[nxt[i]] = prev_[i]; // Próximo aponta para o anterior
        }
    }
}

// Retorna o valor da menor soma atual (a[first_item]) e remove seu índice da lista.
Int get_smallest() {
    Int x = a[first_item]; // Pega o valor
    remove(first_item);   // Remove o índice da lista
    return x;             // Retorna o valor
}

// Procura a soma 'x' na lista (a partir de search_start) e a remove se encontrar.
// Usa a[n2] como sentinela para simplificar o loop.
bool find_and_remove(Int x) {
    a[n2] = x; // Define o sentinela no final do array 'a'
    int i = search_start; // Início otimizado da busca

    // Avança na lista enquanto as somas forem menores que 'x'
    while (a[i] < x) {
        i = nxt[i];
    }

    // Verifica se encontrou 'x' ou passou direto (encontrou sentinela ou valor maior)
    if (i == n2 || a[i] != x) {
        return false; // Não encontrado
    }

    // Encontrado: remove 'i' da lista
    remove(i);
    // Reinicia a próxima busca do início (poderia otimizar mais, mas é seguro)
    search_start = first_item;
    return true; // Encontrado e removido
}

// Testa a hipótese: a[t] = N2 + N3. Tenta reconstruir N1...Nn.
// Função central do algoritmo.
bool test(int t) {
    // Assume a[0] = N1+N2, a[1] = N1+N3 (após ordenação)
    // Hipótese: a[t] = N2+N3
    // Calcula 2*N1 = (N1+N2) + (N1+N3) - (N2+N3)
    Int x = a[0] + a[1] - a[t];

    // 2*N1 deve ser par e não negativo (implicitamente)
    if (x % 2 != 0) {
        return false; // Hipótese inválida
    }

    // Calcula N1, N2, N3 com base nas hipóteses
    b[0] = x / 2;           // N1
    b[1] = a[0] - b[0];     // N2 = (N1+N2) - N1
    b[2] = a[1] - b[0];     // N3 = (N1+N3) - N1

    // Verifica consistência: N2 + N3 calculado deve ser igual a a[t]
    if (b[1] + b[2] != a[t]) {
         return false; // Inconsistente
    }
    // A linha original `b[2] = a[t] - b[1]` também estaria correta matematicamente.

    // Prepara a lista com as somas restantes (excluindo a[0], a[1])
    init_list();
    // Remove a soma a[t] (N2+N3 da hipótese) da lista
    remove(t);

    // Tenta encontrar N4, N5, ..., Nn iterativamente
    for (int i = 3; i < n; i++) { // i=3 -> N4, i=4 -> N5, ...
        // Pega a menor soma restante. Assume ser N1 + N(i+1)
        x = get_smallest();
        // Calcula N(i+1) = (N1 + N(i+1)) - N1
        b[i] = x - b[0];

        // Reinicia a busca para as próximas remoções
        search_start = first_item;

        // Verifica se as somas N(i+1) + Nj (para j=1 a i-1) existem e remove-as
        // Corresponde a b[i] + b[1], b[i] + b[2], ..., b[i] + b[i-1]
        for (int j = 1; j < i; j++) {
            Int required_sum = b[i] + b[j];
            // Se alguma soma necessária não for encontrada, a hipótese falhou
            if (!find_and_remove(required_sum)) {
                return false;
            }
        }
    }
    // Se chegou aqui, todos os Ns foram encontrados e as somas bateram.
    return true; // Hipótese correta!
}

// Função de comparação segura para qsort (evita overflow).
int compare(const void *v1, const void *v2) {
    Int val1 = *(Int*)v1;
    Int val2 = *(Int*)v2;
    if (val1 < val2) return -1;
    if (val1 > val2) return 1;
    return 0;
}

// Orquestra a solução: ordena as somas, testa hipóteses, imprime o resultado.
void solve() {
    // Ordena as somas - essencial para as hipóteses a[0]=N1+N2, a[1]=N1+N3
    qsort(a, n2, sizeof(Int), compare);

    // Testa cada a[i] (para i >= 2) como possível candidato para N2+N3
    for (int i = 2; i < n; i++) {
        if (test(i)) { // Se a hipótese a[t]=a[i] funcionou
            // Imprime a solução encontrada em b[]
            printf("%ld", b[0]);
            for (int j = 1; j < n; j++) {
                printf(" %ld", b[j]);
            }
            printf("\n");
            return; // Solução encontrada, termina
        }
    }
    // Se nenhuma hipótese funcionou
    printf("Impossible\n");
}

// Função principal: lê a entrada e chama solve() para cada caso de teste.
int main() {
    // Lê N enquanto houver entrada
    while (scanf("%d", &n) == 1) {
        n2 = n * (n - 1) / 2; // Calcula número de somas
        // Lê as n2 somas
        for (int i = 0; i < n2; i++) {
            scanf("%ld", &a[i]); // Lê como long int
        }
        solve(); // Resolve o caso de teste
    }
    return 0; // Fim do programa
}