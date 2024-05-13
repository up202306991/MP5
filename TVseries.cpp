#include "TVseries.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <stack>

TVSeriesAPP::TVSeriesAPP() {

}
   
TVSeriesAPP::~TVSeriesAPP() {
    
}


void TVSeriesAPP::addTitleBasics(const TitleBasics& title) {
    
 }
void TVSeriesAPP::addTitleEpisodes(const TitleEpisode& episode) {
  
}


void TVSeriesAPP::addTitlePrincipal(const TitlePrincipals& principal) {

    
}



//Pergunta 1:

vector<string> TVSeriesAPP::getUniquePrincipals(const string& seriesTconst) const {
    // Verificação se a série existe na base de dados
    if (!seriesData.count(seriesTconst)) 
    {
      return {};
    }

    vector<string> uniquePrincipals; 
    unordered_set<string> principalNames; 

    // Obtém os episódios da série
    vector<TitleEpisode>& episodes = episodeData.at(seriesTconst);

    // Itera sobre os episódios da série
    for (TitleEpisode& episode : episodes) {
        // Obtém os membros da equipa e do elenco do episódio atual
        vector<TitlePrincipals>& principals = episode.crew; // !!!!!

        // Percorre os membros da equipa e do elenco do episódio atual
        for (TitlePrincipals& principal : principals) {
            // Obtém o nome do membro da equipa ou do elenco
            string& principalName = principal.getPrimaryName();

            // Verifica se o nome já está no conjunto
            if (principalNames.find(principalName) == principalNames.end()) {
                // Se o nome não existe, adiciona-o ao conjunto e ao vector
                principalNames.insert(principalName);
                uniquePrincipals.push_back(principalName);
            }
        }
    }

    // Ordena o vector alfabeticamente
    sort(uniquePrincipals.begin(), uniquePrincipals.end());

    return uniquePrincipals;
}


//PERGUNTA 2:

string TVSeriesAPP::getMostSeriesGenre() const {
    // Verificação se existem séries na base de dados
    if (seriesData.empty()) {
        return ""; // Retorna string vazia se não existirem séries
    }

    // Inicialização de variáveis
    string mostGenre = ""; // Armazena o género mais frequente
    int maxCount = 0; // Armazena a contagem máxima de séries para um género
    unordered_map<string, int> genreCount; // Mapa para contagem de géneros

    // Itera sobre as séries na base de dados
    for (const auto& pair : seriesData) {
        const TitleBasics& series = pair.second;

        // Obtém os géneros da série atual
        vector<string> genres = series.getGenres();

        // Itera sobre os géneros da série atual
        for (const string& genre : genres) {
            // Incrementa a contagem do género atual
            genreCount[genre]++;

            // Verifica se o género atual tem a contagem máxima até agora
            if (genreCount[genre] > maxCount) {
                mostGenre = genre; // Atualiza o género mais frequente
                maxCount = genreCount[genre]; // Atualiza a contagem máxima
            } else if (genreCount[genre] == maxCount) {
                // Em caso de empate, compara o tamanho dos nomes dos géneros
                if (genre.size() < mostGenre.size()) {
                    mostGenre = genre; // Atualiza o género mais frequente com nome mais curto
                }
            }
        }
    }

    // Retorna o género mais frequente ou string vazia em caso de erro
    return mostGenre;
}


//PERGUNTA 3: 

string TVSeriesAPP::getPrincipalFromCharacter(const string& character) const {

    return "";
}



//PERGUNTA 4

vector<string> TVSeriesAPP::principalsWithMultipleCategories(const string& seriesTconst ) const {

  return {};
}
 
//PERGUNTA 5:

int TVSeriesAPP::principalInMultipleGenres(vector<string> vGenres) {


return -1;
}


//PERGUNTA 6: 
vector<string> TVSeriesAPP::principalsInAllEpisodes(const string& seriesTconst) const {

    return {};
}













