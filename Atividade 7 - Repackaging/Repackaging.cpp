#include <bits/stdc++.h>

// Kayo Xavier Nascimento Cavalcante Leite - 21.2.4095
//Código implementado utilizando como referência o código encontrado a seguir: https://github.com/Sharknevercries/Online-Judge/blob/master/UVA/Volume%20C/10089%20Repackaging.cpp

#define PI acos(-1.0) // Define a constante PI (aproximadamente 3.14159...)
using namespace std;    // Usa o namespace std

double angle[1005]; // Array para armazenar os ângulos calculados (em radianos)
int n;              // Número de ângulos/pontos

int main() {
    // Continua lendo 'n' enquanto a leitura for bem-sucedida e n for diferente de 0
    while (scanf("%d", &n) == 1 && n) {

        // Loop para ler n conjuntos de dados e calcular os ângulos
        for (int i = 0; i < n; i++) {
            int a, b, c; // Variáveis temporárias para leitura
            scanf("%d%d%d", &a, &b, &c);
            // Calcula o ângulo usando atan2(y, x).
            // Aqui, y = b - a e x = c - a. Isso representa o ângulo do vetor
            // do ponto (a, a) para o ponto (c, b) com o eixo x positivo,
            // ou simplesmente o ângulo do ponto (c-a, b-a) relativo à origem.
            angle[i] = atan2(b - a, c - a);
        }

        // Ordena os ângulos em ordem crescente
        sort(angle, angle + n);

        double gap = 0; // Variável para armazenar o maior espaço (gap) entre ângulos

        // Calcula o maior espaço entre ângulos consecutivos (após ordenar)
        for (int i = 0; i < n - 1; i++) {
            gap = max(gap, angle[i + 1] - angle[i]);
        }

        // Calcula o espaço "circular" entre o último e o primeiro ângulo
        // 2*PI é o círculo completo. (angle[n-1] - angle[0]) é o arco coberto pelos ângulos.
        // A diferença é o espaço restante ao dar a volta no círculo.
        gap = max(gap, 2 * PI - (angle[n - 1] - angle[0]));

        // Verifica se o maior espaço encontrado é maior que PI (180 graus)
        // Se for maior, significa que existe um semicírculo vazio.
        puts(gap > PI ? "No" : "Yes"); // Imprime "No" se houver semicírculo vazio, "Yes" caso contrário
    }
    return 0; // Fim do programa
}