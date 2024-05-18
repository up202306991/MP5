#include "TVseries.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <stack>
#include <unordered_set>

TVSeriesAPP::TVSeriesAPP() {

}
   
TVSeriesAPP::~TVSeriesAPP() {
    
}


void TVSeriesAPP::addTitleBasics(const TitleBasics& title) {
    series[title.tconst] = title;
 }
void TVSeriesAPP::addTitleEpisodes(const TitleEpisode& episode) {
    episodeBySerie[episode.parentTconst].push_back(episode);
}


void TVSeriesAPP::addTitlePrincipal(const TitlePrincipals& principal) {
   actorsInfo[principal.tconst].push_back(principal);
}



//Pergunta 1:

vector<string> TVSeriesAPP::getUniquePrincipals(const string& seriesTconst ) const {
    // Verifica se a série existe
    if (series.find(seriesTconst) == series.end()) {
        return {};
    }

    // Coleta de dados
    unordered_set<string> uniqueNames;

    // Obtém os episódios da série
    const vector<TitleEpisode>& episodes = episodeBySerie.at(seriesTconst);

    // Itera pelos episódios e coleta atores associados
    for (const TitleEpisode& episode : episodes) {
        const vector<TitlePrincipals>& actorsForEpisode = actorsInfo.at(episode.tconst);
        for (const TitlePrincipals& actor : actorsForEpisode) {
            uniqueNames.insert(actor.primaryName);
        }
    }

    // Converte o unordered_set para vector e ordena alfabeticamente
    vector<string> result(uniqueNames.begin(), uniqueNames.end());
    sort(result.begin(), result.end());
    
  return result;
}

//PERGUNTA 2:



string TVSeriesAPP::getMostSeriesGenre() const {
    // Verificação se existem séries na base de dados
    if (series.empty()) {
        return ""; // Retorna string vazia se não existirem séries
    }

    string mostGenre = ""; // Armazena o género mais frequente
    int maxCount = 0; // Armazena a contagem máxima de séries para um género
    unordered_map<string, int> genreCount; // Mapa para contagem de géneros

    // Itera sobre as séries na base de dados
    for (const auto& serie : series) {
        const TitleBasics& series = serie.second;

        // Obtém os géneros da série atual
        vector<string> genres = series.genres;

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

vector<string> TVSeriesAPP::principalsWithMultipleCategories(const string& seriesTconst) const {
    // Check if series exists
    if (series.find(seriesTconst) == series.end()) {
        return {};
    }

    unordered_set<string> uniqueActors;
    unordered_map<string, unordered_set<string>> actorCategories;

    const vector<TitleEpisode>& episodes = episodeBySerie.at(seriesTconst);

    for (const TitleEpisode& episode : episodes) {
        // Verifica se o episódio tem atores associados
        if (actorsInfo.count(episode.tconst) > 0) { 
            const vector<TitlePrincipals>& actorsForEpisode = actorsInfo.at(episode.tconst);

            for (const TitlePrincipals& actor : actorsForEpisode) {
                const string& actorName = actor.primaryName;

                if (actorCategories.count(actorName)) {
                    actorCategories[actorName].insert(actor.category);
                } else {
                    unordered_set<string> categories = {actor.category};
                    actorCategories[actorName] = categories;
                }

                if (actorCategories[actorName].size() > 1) {
                    uniqueActors.insert(actorName);
                }
            }
        }
    }

    vector<string> result(uniqueActors.begin(), uniqueActors.end());
    sort(result.begin(), result.end());
    return result;
}


//PERGUNTA 4

vector<string> TVSeriesAPP::principalsInAllEpisodes(const string& seriesTconst) const {
    // Check if series exists
    auto itr = episodeBySerie.find(seriesTconst);
    if (itr == episodeBySerie.end() || itr->second.empty()) return {};

    const vector<TitleEpisode>& episodes = episodeBySerie.at(seriesTconst);
    unordered_set<string> allEpisodesActors;

    for (const TitleEpisode& episode : episodes) {
        // Verifica se o episódio tem atores associados
        if (actorsInfo.count(episode.tconst) > 0) {
            const vector<TitlePrincipals>& actorsForEpisode = actorsInfo.at(episode.tconst);

            // Verifica se o vetor de atores está vazio
            if (!actorsForEpisode.empty()) {

                if (allEpisodesActors.empty()) {
                    for (const TitlePrincipals& actor : actorsForEpisode) {
                        allEpisodesActors.insert(actor.primaryName);
                    }
                } else {
                    unordered_set<string> intersection;
                    for (const TitlePrincipals& actor : actorsForEpisode) {
                        if (allEpisodesActors.count(actor.primaryName) > 0) {
                            intersection.insert(actor.primaryName);
                        }
                    }
                    allEpisodesActors = intersection;
                }

            // Se o vetor de atores estiver vazio, pula para o próximo episódio
            } else {
                continue; 
            }
        }
    }

    vector<string> result(allEpisodesActors.begin(), allEpisodesActors.end());
    sort(result.begin(), result.end());
    return result;
}




 
//PERGUNTA 5:

int TVSeriesAPP::principalInMultipleGenres(vector<string> vGenres) {
    int matchingActors = 0;
    unordered_set<string> uniqueActors; // Para contar atores de forma única

    for (const auto& seriesPair : series) {
        const string& seriesTconst = seriesPair.first;
        const TitleBasics& serieData = seriesPair.second;

        // Verifica se a série possui pelo menos um dos gêneros desejados
        bool hasMatchingGenre = any_of(serieData.genres.begin(), serieData.genres.end(),
                                       [&](const string& genre) {
                                           return find(vGenres.begin(), vGenres.end(), genre) != vGenres.end();
                                       });

        if (hasMatchingGenre) {
            // Verifica se 'actorsInfo' possui informações de atores para a série atual
            if (actorsInfo.find(seriesTconst) != actorsInfo.end()) {
                // Itera sobre os atores da série
                for (const auto& actor : actorsInfo.at(seriesTconst)) {
                    // Insere o ator no conjunto e verifica se foi inserido com sucesso
                    if (uniqueActors.insert(actor.nconst).second) {
                        matchingActors++;
                    }
                }
            }
        }
    }
    return matchingActors;
}


//PERGUNTA 6: 
string TVSeriesAPP::getPrincipalFromCharacter(const string& character) const {
    unordered_map<string, int> characterCount;

    for (const auto& episodePrincipals : actorsInfo) {
        for (const auto& principal : episodePrincipals.second) {
            for (const string& charName : principal.characters) {
                string normalizedCharName = charName;
                // Remove all non-alphabetic characters
                normalizedCharName.erase(remove_if(normalizedCharName.begin(), normalizedCharName.end(), [](char c){ return !isalpha(c); }), normalizedCharName.end());

                if (normalizedCharName == character) {
                    characterCount[principal.primaryName]++;
                }
            }
        }
    }

    string mostFrequentActor;
    int maxCount = 0;
    for (const auto& count : characterCount) {
        if (count.second > maxCount || (count.second == maxCount && count.first < mostFrequentActor)) {
            mostFrequentActor = count.first;
            maxCount = count.second;
        }
    }

    return mostFrequentActor.empty() ? " " : mostFrequentActor; // Return space if no actor found
}










