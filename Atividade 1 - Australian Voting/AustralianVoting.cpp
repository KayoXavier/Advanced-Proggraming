#include <iostream>
#include <vector>
#include <sstream>
// Kayo Xavier Nascimento Cavalcante Leite - 21.2.4095
//Código implementado utilizando como referência o código encontrado no GitHub a seguir: https://github.com/Diusrex/UVA-Solutions/blob/master/10142%20Australian%20Voting.cpp
 
using namespace std;

int main()
{
    int Test; // Número de casos de teste
    cin >> Test;

    string endingSeperator = ""; // Separador para formatar a saída entre casos de teste

    while (Test--) // Processar cada caso de teste
    {
        int n; // Número de candidatos
        cin >> n;

        vector<string> names(n); // Vetor para armazenar os nomes dos candidatos
        vector<bool> eliminated(n, false); // Vetor para rastrear se um candidato foi eliminado

        cin.ignore(); // Ignorar o caractere de nova linha após a entrada do número de candidatos

        // Ler os nomes dos candidatos
        for (int i = 0; i < n; ++i)
            getline(cin, names[i]);

        string temp;
        getline(cin, temp); // Ler a primeira linha das cédulas (ou uma linha vazia)

        vector<vector<int>> ratings; // Matriz para armazenar as cédulas (ordem de preferência dos eleitores)

        // Ler todas as cédulas enquanto a entrada não estiver vazia
        while (temp != "")
        {
            stringstream ss;
            ss << temp; // Criar um stream da linha atual
            vector<int> order(n); // Vetor para armazenar a ordem de preferência da cédula atual

            for (int i = 0; i < n; ++i)
            {
                ss >> order[i]; // Ler a preferência do eleitor
                --order[i];     // Ajustar o índice para zero-based
            }

            ratings.push_back(order); // Adicionar a cédula à matriz de cédulas

            if (cin.eof()) // Verificar se é o fim da entrada
                break;

            getline(cin, temp); // Ler a próxima linha das cédulas
        }

        int numRatings = ratings.size(); // Total de cédulas lidas
        vector<int> posInRatings(numRatings, 0); // Vetor para rastrear a posição atual em cada cédula

        int winner = -1; // Índice do vencedor (-1 indica que ainda não há vencedor)

        vector<int> count(n, 0); // Vetor para contar os votos de cada candidato

        // Contar os votos iniciais considerando apenas as primeiras preferências
        for (int i = 0; i < numRatings; ++i)
            ++count[ratings[i][0]];

        while (winner == -1) // Enquanto não houver vencedor
        {
            for (int i = 0; i < numRatings; ++i)
            {
                bool changed(false); // Flag para verificar se a preferência mudou

                // Ignorar candidatos eliminados e atualizar a posição na cédula
                while (eliminated[ratings[i][posInRatings[i]]])
                {
                    ++posInRatings[i];
                    changed = true;
                }

                if (changed) // Se mudou, atualizar a contagem de votos
                    ++count[ratings[i][posInRatings[i]]];
            }

            int highest(0);  // Maior número de votos entre os candidatos restantes
            int lowest(1000); // Menor número de votos entre os candidatos restantes

            for (int i = 0; i < n; ++i)
            {
                if (eliminated[i]) // Ignorar candidatos eliminados
                    continue;
                if (count[i] > highest) 
                    highest = count[i]; // Atualizar o maior número de votos
                if (count[i] < lowest)
                    lowest = count[i];  // Atualizar o menor número de votos
            }

            // Verificar condições de vitória ou empate
            if (highest == lowest || highest * 2 > numRatings)
                winner = highest; // Determinar o vencedor com maioria absoluta ou empate
            
            else
            {
                for (int i = 0; i < n; ++i)
                    if (count[i] == lowest) 
                        eliminated[i] = true; // Eliminar candidatos com o menor número de votos
            }
        }

        cout << endingSeperator;
        endingSeperator = "\n"; // Adicionar separador entre os casos na saída

        for (int i = 0; i < n; ++i)
            if (count[i] == winner && !eliminated[i])            
                cout << names[i] << '\n'; // Exibir o nome do(s) vencedor(es)
    }
}
