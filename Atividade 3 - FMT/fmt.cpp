#include <iostream> // Para entrada/saída (cin, cout, getline)
#include <string>   // Para manipulação de strings
// Kayo Xavier Nascimento Cavalcante Leite - 21.2.4095
//Código implementado utilizando como referência o código encontrado a seguir: https://codingstrife.wordpress.com/2013/07/23/solution-uva848-pc110308-fmt/

using namespace std;

// Remove espaços do final da string 'line'.
void trimBack(string &line) {
    // Itera de trás para frente, apagando espaços.
    for (int i = (int)line.length() - 1; !line.empty() && i >= 0 && line[i] == ' '; i--) {
        line.erase(i, 1);
    }
}

// Remove espaços do início da string 'line'.
void trimFront(string &line) {
    // Itera enquanto o primeiro caractere for espaço, apagando-o.
    while (!line.empty() && line[0] == ' ') {
        line.erase(0, 1);
    }
}

int main() {
    string output = ""; // Armazena o texto formatado final
    string line = "";   // Armazena o segmento de linha atual sendo processado
    string buffer = ""; // Armazena texto que não coube na linha anterior (para início da próxima)
    bool LineCreated = false; // Flag: true se 'line' contém um resto da linha anterior que foi quebrada

    // Loop principal de processamento
    while (true) {
        line = ""; // Reseta a linha atual
        // Só lê uma nova linha da entrada se não houver resto da linha anterior
        if (!LineCreated && !getline(cin, line)) {
            break; // Sai do loop se a leitura falhar (EOF)
        }

        // Se usamos o resto da linha anterior, reseta a flag para a próxima iteração
        if (LineCreated) {
            LineCreated = false;
        }

        // Condição de parada: linha começa com '*'
        if (!line.empty() && line[0] == '*') {
            break;
        }

        trimBack(line);          // Remove espaços do final da linha lida/resto
        line = buffer + line;    // Prepara a linha atual juntando o buffer (resto) + nova parte
        buffer = "";             // Limpa o buffer, pois seu conteúdo foi movido para 'line'

        // Caso 1: Linha atual excede 72 caracteres
        if (line.length() > 72) {
            bool BigWord = true;     // Assume inicialmente que é uma palavra > 72 ou sem quebra boa
            int LastGoodPos = -1;    // Posição do último espaço encontrado

            // Procura o último espaço para quebrar a linha
            for (int i = (int)line.length() - 1; i > 0; i--) {
                if (line[i] == ' ') {
                    LastGoodPos = i; // Guarda a posição do último espaço
                    // Se o espaço está DENTRO do limite de 72...
                    if (i <= 72) {
                        // Quebra ideal: move o que vem depois do espaço para o buffer
                        buffer = line.substr(i + 1); 
                        line = line.substr(0, i); // Mantém o que vem antes na linha atual
                        BigWord = false;          // Marca que a quebra foi feita
                        break;                   // Para de procurar espaços
                    }
                }
            }

            // Se não achou quebra ideal (<=72), mas achou *algum* espaço...
            // (Isso trata palavras longas no início da linha > 72)
            if (LastGoodPos != -1 && BigWord) {
                // Quebra no último espaço encontrado, mesmo que > 72
                buffer = line.substr(LastGoodPos + 1);
                line = line.substr(0, LastGoodPos);
                BigWord = false; // Marca que a quebra (não ideal) foi feita
            }
            
            // Se conseguimos quebrar a linha (BigWord == false)...
            if (!BigWord) {
                 LineCreated = true; // Indica que 'line' tem conteúdo e 'buffer' também pode ter
            } 
            // Se BigWord ainda é true, significa que a linha é uma única palavra > 72 chars 
            // ou não tem espaços. A linha será impressa como está (potencialmente > 72).
            // O buffer permanece vazio neste caso.

            trimBack(line);      // Garante que a linha atual não termina com espaço
            trimFront(buffer);   // Garante que o buffer não começa com espaço
        }
        // Caso 2: Linha atual tem menos de 72 caracteres
        else if (line.length() < 72) {
            // Verifica se a linha atual deve ser movida para o buffer para juntar com a próxima entrada
            if (!line.empty() && line.back() != ' ' && cin.peek() != ' ' && cin.peek() != '\n' && !cin.eof()) {
                // Se a linha não está vazia, não termina em espaço, E
                // o próximo caractere na entrada NÃO é espaço/newline/EOF,
                // então a palavra atual não terminou.
                buffer = line + " "; // Move a linha atual + um espaço para o buffer
                line = "";           // Limpa a linha atual (não será impressa agora)
                continue;            // Pula para a próxima iteração para ler mais entrada
            }
             // Se não entrou no if acima, a linha/palavra termina aqui.
             // O buffer já foi limpo no início do loop ou permanece vazio.
        }
        // Caso 3: Linha atual tem exatamente 72 caracteres
        // O buffer já foi limpo no início do loop. Nenhuma ação extra necessária.

        // Adiciona a linha processada (que agora tem <= 72, a menos que seja uma palavra única > 72) ao output
        // Se line foi movida para o buffer (no caso 2), ela está vazia e nada é adicionado aqui.
        if (!line.empty()) {
             output += line + '\n';
        } else if (buffer.empty() && !output.empty() && output.back() == '\n') {
             // Se a linha atual ficou vazia E o buffer está vazio (significa fim de parágrafo talvez)
             // e já existe algo no output, adiciona uma quebra de linha extra se necessário
             // (Este else if pode precisar de ajuste dependendo do comportamento exato desejado para linhas vazias)
             // -> Na verdade, o código original não parece tratar linhas vazias de forma especial aqui,
             //    simplesmente não adiciona nada se 'line' ficou vazia.
        }

    } // Fim do while(true)

    // Adiciona o conteúdo final do buffer, se houver (caso o texto termine sem fechar uma linha)
    if (!buffer.empty()) {
         trimBack(buffer); // Garante sem espaço final
         output += buffer + '\n';
    }

    // Remove o último caractere do output se for um '\n' extra
    if (!output.empty() && output.back() == '\n') {
        output.pop_back(); 
    }

    cout << output << endl; // Imprime o resultado final

    return 0;
}