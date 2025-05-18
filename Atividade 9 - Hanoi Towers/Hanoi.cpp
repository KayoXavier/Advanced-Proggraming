#include <bits/stdc++.h> // Biblioteca padrão para competições, inclui I/O e outras utilidades
// Kayo Xavier Nascimento Cavalcante Leite - 21.2.4095
//Código implementado utilizando como referência o código encontrado a seguir: https://github.com/evandrix/UVa/blob/master/10276.cpp

using namespace std; // Utiliza o namespace padrão (para cin, cout, endl, etc.)

int main() // Função principal do programa
{
    // Declara um array para armazenar 'pesos' ou valores pré-calculados
    int pesos[52];

    // Inicializa os dois primeiros valores da sequência de pesos
    pesos[0] = 1;
    pesos[1] = 2;

    // Loop para preencher o restante do array 'pesos'
    // Itera sobre índices pares (i = 2, 4, 6, ..., 50)
    for (int i = 2; i < 52; i = i + 2)
    {
        // Define o peso para o índice par 'i' como i + 2
        pesos[i] = i + 2;
        // Define o peso para o próximo índice ímpar 'i + 1' com o mesmo valor (i + 2)
        pesos[i + 1] = i + 2;
        // Resultado: pesos = [1, 2, 4, 4, 6, 6, 8, 8, ..., 52, 52]
    }

    // Declara um array para armazenar as somas acumuladas (prefix sums) dos 'pesos'
    int bolas[52];

    // Inicializa o primeiro valor da soma acumulada
    bolas[0] = 1;

    // Loop para calcular as somas acumuladas
    // Itera de i = 1 até 51
    for (int i = 1; i < 52; i++)
    {
        // O valor atual de 'bolas' é o valor anterior mais o 'peso' do índice anterior
        // bolas[i] = bolas[i-1] + pesos[i-1]
        // Ex: bolas[1] = bolas[0] + pesos[0] = 1 + 1 = 2
        //     bolas[2] = bolas[1] + pesos[1] = 2 + 2 = 4
        //     bolas[3] = bolas[2] + pesos[2] = 4 + 4 = 8
        bolas[i] = bolas[i - 1] + pesos[i - 1];
    }

    // Variável para armazenar o número de casos de teste
    int cases = 0;
    // Lê o número de casos de teste da entrada padrão
    cin >> cases;

    // Loop para processar cada caso de teste
    for (int i = 0; i < cases; i++)
    {
        // Variável para armazenar o valor de entrada 'n' para o caso atual
        int n = 0;
        // Lê o valor 'n' da entrada padrão
        cin >> n;

        // Imprime o resultado para o valor 'n' consultando o array pré-calculado 'bolas'
        // O valor impresso é bolas[n] - 1
        cout << bolas[n] - 1 << endl;
    }

    // Retorna 0 indicando que o programa terminou com sucesso
    return 0;
}