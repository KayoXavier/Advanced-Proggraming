#include <iostream> // Para cin/cout
#include <string>   // Para strings
#include <algorithm> // Para min/max
// Kayo Xavier Nascimento Cavalcante Leite - 21.2.4095
//Código implementado utilizando como referência o código encontrado a seguir: https://github.com/Diusrex/UVA-Solutions/blob/master/10201%20Adventures%20in%20Moving.cpp

using namespace std;

// --- Variáveis Globais ---
int numStations;             // Número de postos válidos
const int MaxCost = 10005 * 2005; // Custo "infinito" para estados impossíveis
const int MaxFuel = 200;     // Capacidade máxima do tanque

// --- Estruturas de Dados ---
int distanceTravelled[105]; // distanceTravelled[i] = distância até o posto i
int gasPrice[105];          // gasPrice[i] = preço no posto i
// bestCost[i][j] = memoização: menor custo do posto i (chegando com j de combustível) até o destino
int bestCost[105][205];

// --- Função Recursiva com Memoização ---
// Calcula o menor custo do posto 'station' até o destino, chegando em 'station' com 'gasLeft' de combustível.
int GetBestCost(int station, int gasLeft) {
    // --- Casos Base ---
    if (gasLeft < 0) return MaxCost + 10; // Estado inválido (sem combustível)
    if (station == numStations) { // Chegou ao destino final
        // Condição específica? (chegar com >= 100 de combustível)
        return (gasLeft < 100) ? MaxCost + 10 : 0; 
    }

    // --- Memoização ---
    int &best = bestCost[station][gasLeft]; // Referência à célula da DP
    if (best != -1) return best; // Retorna resultado se já calculado

    // --- Cálculo/Recursão ---
    best = MaxCost + 10; // Inicializa com custo alto
    int gasToMakeItToNext = distanceTravelled[station + 1] - distanceTravelled[station]; // Comb. p/ próx. posto

    // Tenta todas as quantidades de combustível ('gasUsed') após abastecer neste posto
    for (int gasUsed = max(gasLeft, gasToMakeItToNext); gasUsed <= MaxFuel; ++gasUsed) {
        // Custo = (custo de abastecer) + (custo recursivo a partir do próximo posto)
        int currentCost = GetBestCost(station + 1, gasUsed - gasToMakeItToNext) 
                          + (gasUsed - gasLeft) * gasPrice[station];
        best = min(best, currentCost); // Atualiza o menor custo
    }

    return best; // Retorna o menor custo calculado/memoizado
}

// --- Função Principal ---
int main() {
    int T;
    cin >> T; // Lê número de casos de teste
    string sep = "", ignore;

    while (T--) { // Loop para cada caso de teste
        cout << sep;
        sep = "\n";

        int initialDist;
        cin >> initialDist; // Lê distância final

        numStations = 0; // Reseta contador de postos
        // Lê dados dos postos (distância, preço), ignorando os que estão além de initialDist
        while (getline(cin, ignore), cin.peek() >= '0' && cin.peek() <= '9') {
            cin >> distanceTravelled[numStations] >> gasPrice[numStations];
            if (distanceTravelled[numStations] <= initialDist) {
                ++numStations;
            }
        }
        distanceTravelled[numStations] = initialDist; // Marca a distância final

        // Inicializa a tabela de memoização/DP com -1 (não calculado)
        for (int i = 0; i <= numStations; ++i) {
            for (int j = 0; j <= MaxFuel; ++j) {
                bestCost[i][j] = -1;
            }
        }

        // Calcula o estado inicial (combustível ao chegar no primeiro posto) e chama a função
        int cost = GetBestCost(0, 100 - distanceTravelled[0]);

        // Imprime o resultado
        if (cost > MaxCost) {
            cout << "Impossible\n";
        } else {
            cout << cost << '\n';
        }
    }
    return 0;
}