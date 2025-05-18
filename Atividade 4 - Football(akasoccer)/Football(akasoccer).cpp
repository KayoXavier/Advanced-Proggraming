#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
// Kayo Xavier Nascimento Cavalcante Leite - 21.2.4095

using namespace std;

// Estrutura para armazenar estatísticas de cada time
struct TeamStats {
    int points = 0;        // Pontos totais
    int played = 0;        // Jogos disputados
    int wins = 0;          // Vitórias
    int ties = 0;          // Empates
    int losses = 0;        // Derrotas
    int scored = 0;        // Gols marcados
    int against = 0;       // Gols sofridos
    string lower_name;     // Nome em minúsculas para comparação lexicográfica
};

// Função de comparação para ordenação conforme as regras do problema
bool CompareTeams(const pair<string, TeamStats>& a, const pair<string, TeamStats>& b) {
    const auto& statsA = a.second;
    const auto& statsB = b.second;
    
    // 1. Maior número de pontos
    if (statsA.points != statsB.points) 
        return statsA.points > statsB.points;
    
    // 2. Maior número de vitórias
    if (statsA.wins != statsB.wins) 
        return statsA.wins > statsB.wins;
    
    // 3. Maior saldo de gols
    int goalDiffA = statsA.scored - statsA.against;
    int goalDiffB = statsB.scored - statsB.against;
    if (goalDiffA != goalDiffB) 
        return goalDiffA > goalDiffB;
    
    // 4. Maior número de gols marcados
    if (statsA.scored != statsB.scored) 
        return statsA.scored > statsB.scored;
    
    // 5. Menor número de jogos disputados
    if (statsA.played != statsB.played) 
        return statsA.played < statsB.played;
    
    // 6. Ordem lexicográfica (case insensitive)
    return statsA.lower_name < statsB.lower_name;
}

int main() {
    int numTournaments;
    cin >> numTournaments;
    cin.ignore(); // Ignora o '\n' restante
    
    string separator = ""; // Controla a separação entre torneios

    while (numTournaments--) {
        // Leitura do nome do torneio
        string tournamentName;
        getline(cin, tournamentName);
        
        // Imprime separador e nome do torneio
        cout << separator << tournamentName << '\n';
        separator = "\n"; // Atualiza separador para próximos torneios
        // Leitura do número de times
        int numTeams;
        cin >> numTeams;
        cin.ignore();
        
        // Mapa para armazenar estatísticas dos times
        map<string, TeamStats> teams;

        // Leitura dos nomes dos times
        for (int i = 0; i < numTeams; ++i) {
            string teamName;
            getline(cin, teamName);
            
            // Pré-cadastra o time e armazena versão lowercase do nome
            TeamStats& stats = teams[teamName];
            stats.lower_name = teamName;
            transform(teamName.begin(), teamName.end(), stats.lower_name.begin(), ::tolower);
        }

        // Leitura do número de jogos
        int numGames;
        cin >> numGames;
        cin.ignore();

        // Processamento dos jogos
        for (int i = 0; i < numGames; ++i) {
            string gameResult;
            getline(cin, gameResult);
            
            // Parsing do resultado do jogo
            size_t pos = gameResult.find('#');
            string team1 = gameResult.substr(0, pos);
            
            // Extração dos gols do time 1
            int goals1 = 0;
            for (++pos; gameResult[pos] != '@'; ++pos) 
                goals1 = goals1 * 10 + (gameResult[pos] - '0');
            
            // Extração dos gols do time 2
            int goals2 = 0;
            for (++pos; gameResult[pos] != '#'; ++pos) 
                goals2 = goals2 * 10 + (gameResult[pos] - '0');
            
            // Extração do nome do time 2
            string team2 = gameResult.substr(pos + 1);

            // Atualiza estatísticas dos times
            TeamStats& stats1 = teams[team1];
            TeamStats& stats2 = teams[team2];

            // Atualiza jogos disputados
            stats1.played++;
            stats2.played++;

            // Atualiza gols marcados/sofridos
            stats1.scored += goals1;
            stats1.against += goals2;
            stats2.scored += goals2;
            stats2.against += goals1;

            // Determina resultado do jogo
            if (goals1 > goals2) { // Vitória do time 1
                stats1.points += 3;
                stats1.wins++;
                stats2.losses++;
            } 
            else if (goals1 == goals2) { // Empate
                stats1.points += 1;
                stats2.points += 1;
                stats1.ties++;
                stats2.ties++;
            } 
            else { // Vitória do time 2
                stats2.points += 3;
                stats2.wins++;
                stats1.losses++;
            }
        }

        // Converte o mapa para vetor e ordena
        vector<pair<string, TeamStats>> sortedTeams(teams.begin(), teams.end());
        sort(sortedTeams.begin(), sortedTeams.end(), CompareTeams);

        // Imprime a classificação
        int rank = 1;
        for (const auto& team : sortedTeams) {
            const TeamStats& s = team.second;
            printf("%d) %s %dp, %dg (%d-%d-%d), %dgd (%d-%d)\n",
                   rank++, 
                   team.first.c_str(), 
                   s.points, 
                   s.played, 
                   s.wins, 
                   s.ties, 
                   s.losses, 
                   (s.scored - s.against), 
                   s.scored, 
                   s.against);
        }
    }
    
    return 0;
}