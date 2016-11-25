// ATTENTION: NE PAS MODIFIER CE FICHIER

#ifndef INVESTISSEMENT
#define INVESTISSEMENT

#include <vector>

// Paremetres:
//   profits: profits[i][m] = le profit d'investir m dollars dans l'entreprise i
//   montants: parametre de sortie: montants[i] est le montant que vous pensez devoir investir dans l'entreprise i pour maximiser les profits.
void investissement(const std::vector<std::vector<unsigned int> >& profits, std::vector<unsigned int>& montants);

#endif
