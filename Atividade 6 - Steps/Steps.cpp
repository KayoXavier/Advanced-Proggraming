#include <stdio.h> // Biblioteca padrão de entrada/saída
// Kayo Xavier Nascimento Cavalcante Leite - 21.2.4095
//Código implementado utilizando como referência o código encontrado a seguir: https://github.com/limon2009/ACM-UVA-Online-Judge-solution/blob/master/846.cpp

#define MAXN 46345    // Define um limite superior (sqrt(2^31) aprox, relevante para int)
typedef long long ss; // Define 'ss' como um apelido para 'long long'

ss A[MAXN]; // Array global para armazenar valores pré-calculados

// Pré-calcula os valores A[i] = i * (i+1)
void GEN() {
    ss sum;
    ss i;
    A[0] = 0; // Caso base
    // Calcula A[i] para i de 1 até um limite prático (46341)
    for (i = 1; i <= 46341; i++) {
        sum = i * (i + 1); // Calcula i * (i+1)
        A[i] = sum;        // Armazena no array
    }
}

// Busca binária modificada (e um pouco incomum) no array pré-calculado A.
// O objetivo parece ser encontrar o maior índice 'p' tal que A[p] < key.
// A condição de parada `dif != mid` é particularmente estranha.
int Search(int key) {
    int lo = 0, up = 46341, mid; // Limites da busca
    int dif;                     // Guarda a diferença key - A[mid]

    // Primeira iteração fora do loop para inicializar 'mid' e 'dif'
    if (lo > up) return -1; // Caso de array vazio ou limites inválidos (segurança)
    mid = lo + (up - lo) / 2; // Calcula o meio inicial
    dif = key - A[mid];

    // Loop não padrão: continua enquanto lo <= up E a diferença não for igual ao índice médio (?)
    while (lo <= up && dif != mid) {
        if (dif > 0) { // key > A[mid]
            // Condição crucial: Se key <= A[mid+1], retorna mid.
            // Isso encontra p tal que A[p] < key <= A[p+1].
            // É preciso checar o limite do array antes de acessar A[mid+1].
            if (mid + 1 <= 46341 && (key - A[mid + 1]) <= 0) {
                 return mid; // Encontrou o índice 'p' desejado
            }
            lo = mid + 1; // Se key > A[mid+1], busca na metade superior
        } else {          // key <= A[mid]
            up = mid - 1; // Busca na metade inferior
        }

        // Recalcula mid e dif *antes* da próxima checagem do while
        if (lo > up) break; // Sai se os limites se cruzarem
        mid = lo + (up - lo) / 2;
        // Verifica se A[mid] existe antes de subtrair (evitar erro se mid for inválido)
        if (mid >= 0 && mid <= 46341) {
             dif = key - A[mid];
        } else {
             break; // Sai se mid for inválido
        }
    }
    // Retorna o último 'mid' calculado, seja porque lo > up ou porque dif == mid.
    // O valor retornado pode não ser o 'p' ideal se o loop terminou por dif == mid.
    // No entanto, a lógica em Cal() depende deste comportamento específico.
    // Se key for menor que A[1], 'mid' pode terminar como 0 ou próximo.
    return mid;
}


// Calcula o resultado baseado na diferença m-n.
void Cal(int n, int m) {
    int target = m - n; // Diferença entre os pontos (chave da busca)
    // Encontra p usando a busca peculiar. 'p' será o índice retornado por Search.
    int p = Search(target);
    // Calcula quanto 'target' excede A[p] (assumindo que A[p] foi o último valor < target)
    // Nota: Se Search retornou um 'mid' onde target <= A[mid], este cálculo pode ser negativo.
    // A lógica funciona porque Search deve retornar p tal que A[p] < target <= A[p+1]
    // (devido ao `return mid` dentro do `if`) para a maioria dos casos.
    int diff = target - A[p];

    // Compara a diferença 'diff' com 'p+1'.
    // A[p+1] - A[p] = 2*p + 2. O "meio" do intervalo (A[p], A[p+1]] é A[p] + p + 1.
    // Se diff <= p+1, target está na primeira metade do intervalo.
    if (p + 1 >= diff) { // Equivalente a target <= A[p] + p + 1
        printf("%d\n", p * 2 + 1); // Resultado para a primeira metade
    } else { // diff > p+1, target está na segunda metade
        printf("%d\n", p * 2 + 2); // Resultado para a segunda metade
    }
}

int main() {
    int kase; // Número de casos de teste
    int n, m; // Entrada para cada caso

    GEN(); // Pré-calcula o array A uma vez no início

    scanf("%d", &kase); // Lê o número de casos de teste
    while (kase--) {    // Loop para cada caso de teste
        scanf("%d%d", &n, &m); // Lê n e m

        // Caso especial: se n e m são iguais, a distância/resultado é 0
        if (m == n) {
            printf("0\n");
            continue; // Pula para o próximo caso de teste
        }

        // Calcula e imprime o resultado para o caso atual
        Cal(n, m);
    }
    return 0; // Fim do programa
}