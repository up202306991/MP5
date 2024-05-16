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


//PERGUNTA 3

vector<string> TVSeriesAPP::principalsWithMultipleCategories(const string& seriesTconst ) const {
  // Check if series exists
  if (series.find(seriesTconst) == series.end()) {
    return {};
  }

  // Set to store unique actors with multiple categories
  unordered_set<string> uniqueActors;

  // Map to store categories for each actor in an episode
  unordered_map<string, unordered_set<string>> actorCategories;

  // Access episodes for the series
  const vector<TitleEpisode>& episodes = episodeBySerie.at(seriesTconst);

  // Iterate through episodes
  for (const TitleEpisode& episode : episodes) {
    // Access actors for the episode
    const vector<TitlePrincipals>& actorsForEpisode = actorsInfo.at(episode.tconst);

    // Iterate through actors in the episode
    for (const TitlePrincipals& actor : actorsForEpisode) {
      const string& actorName = actor.primaryName;

      // Check if actor already has categories
      if (actorCategories.count(actorName)) {
        // Actor found before, add current category
        actorCategories[actorName].insert(actor.category);
      } else {
        // New actor, create entry with current category
        unordered_set<string> categories = {actor.category};
        actorCategories[actorName] = categories;
      }

      // Check if actor has multiple categories
      if (actorCategories[actorName].size() > 1) {
        uniqueActors.insert(actorName);
      }
    }
  }

  // Convert to vector and return
  vector<string> result(uniqueActors.begin(), uniqueActors.end());
    sort(result.begin(), result.end());
    return result;
}

//PERGUNTA 4: 

vector<string> TVSeriesAPP::principalsInAllEpisodes(const string& seriesTconst) const {
  // Check if series exists
  if (series.find(seriesTconst) == series.end()) {
    return {};
  }

  // Get all episodes for the series
  const vector<TitleEpisode>& episodes = episodeBySerie.at(seriesTconst);

  // Set to store actors who appear in all episodes
  unordered_set<string> allEpisodesActors;

  // Iterate through all episodes
  for (const TitleEpisode& episode : episodes) {
    // Get actors for the current episode
    const vector<TitlePrincipals>& actorsForEpisode = actorsInfo.at(episode.tconst);

    // Check if this is the first episode
    if (allEpisodesActors.empty()) {
      // First episode, add all actors to the set
      for (const TitlePrincipals& actor : actorsForEpisode) {
        allEpisodesActors.insert(actor.primaryName);
      }
    } else {
      // Not the first episode, check intersection with existing set
      unordered_set<string> intersection;
    for (const TitlePrincipals& actor : actorsForEpisode) {
        if (allEpisodesActors.count(actor.primaryName) > 0) {
            intersection.insert(actor.primaryName);
        }
    }
    allEpisodesActors = intersection;
    }
  }

    // Convert to sorted vector and return
    vector<string> result(allEpisodesActors.begin(), allEpisodesActors.end());
    sort(result.begin(), result.end());
    return result;
}

 
//PERGUNTA 5:

int TVSeriesAPP::principalInMultipleGenres(vector<string> vGenres) int TVSeriesAPP::principalInMultipleGenres(const vector<string>& vGenres) const {
  // Counter to store actors in matching genres
  int matchingActors = 0;

  // Iterate through all series
  for (const auto& series : series) {
    const string& seriesTconst = series.first;
    const TitleBasics& serieData = series.second;

    // Check if series genres intersect with vGenres
    bool hasMatchingGenre = false;
    for (const string& genre : serieData.genres) {
      if (find(vGenres.begin(), vGenres.end(), genre) != vGenres.end()) {
        hasMatchingGenre = true;
        break;
      }
    }

    // If series has matching genre, count actors
     if (hasMatchingGenre) {
            for (const auto& actor : actorsInfo.at(seriesTconst)) {
                if (uniqueActors.insert(actor.nconst).second) { 
                    matchingActors++; 
                }
            }
        }
    }
    return matchingActors;
}



//PERGUNTA 6: 
string TVSeriesAPP::getPrincipalFromCharacter(const string& character) const {

    unordered_map<string, int> characterCount;

    for(auto& episodePrincipals : principalsByEpisode){
        for(auto& principal : episodePrincipals.second){
            for(string& charName : principal.characters){
                string normalizedCharName = charName;

                normalizedCharName.erase(remove(normalizedCharName.begin(), normalizedCharName.end, '['), normalizedCharName.end());
                normalizedCharName.erase(remove(normalizedCharName.begin(), normalizedCharName.end, ']'), normalizedCharName.end());
                normalizedCharName.erase(remove(normalizedCharName.begin(), normalizedCharName.end, '/'), normalizedCharName.end());
                normalizedCharName.erase(remove(normalizedCharName.begin(), normalizedCharName.end, ' '), normalizedCharName.end());

                if(normalizeCharName == character){
                    characterCount[principal.primaryName]++
                }
            }
        }
    }
    string mostFrequentActor;
    int maxCount = 0;

    for(auto& count : characterCount){
        if(count.second > maxCount || (count.second == maxCount && count.first < mostFrequentActor)){
            maxCount = count.second;
            mostFrequentActor = count.first;
        }
    }

    return mostFrequentActor.empty() > " " : mostFrequentActor;
}













