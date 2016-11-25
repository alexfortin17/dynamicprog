// Pour tester si votre code est ecrit en C++ standard, copier le
// contenu de ce fichier sur la page http://geyron.fsg.ulaval.ca

#include "investissement.hpp"

#include <assert.h>
#include <algorithm>
#include <numeric>

unsigned int splitinvestment(const std::vector<std::vector<unsigned int> >& profits, const unsigned int& m, const unsigned int& i,
                     const std::vector<std::vector<unsigned int> >& tabcalcul){
    unsigned int valmax = 0;
    for(unsigned int x = m - 1; x >=1; x--){
        unsigned int profitautrescorps = tabcalcul[i-1][x];
        unsigned int profitcorpi = profits[i][m-x];
        unsigned int profittotal = profitautrescorps + profitcorpi;
        if (profittotal > valmax){
            valmax = profittotal;
        }
    }
    return valmax;
}

// Paremetres:
//   profits: profits[i][m] = le profit d'investir m dollars dans l'entreprise i
//   montants: parametre de sortie: montants[i] est le montant que vous pensez devoir investir dans l'entreprise i pour maximiser les profits.
void investissement(const std::vector<std::vector<unsigned int> >& profits, std::vector<unsigned int>& montants) {
    assert(profits.size() > 0 && profits.front().size() > 0);
    const unsigned int nb_compagnies = profits.size();
    const unsigned int budget = profits.front().size() - 1;
    for (std::vector<std::vector<unsigned int> >::const_iterator i = profits.begin(); i != profits.end(); i++) {
        // Note: Lorsque l'on compile avec l'option -O3, les assertions ne
        // sont pas inclus dans le code compile. Les boucles qui ne font
        // rien sont egalement retire du code. Cette boucle ne se retrouve
        // donc pas dans le code compile et ne devrait pas être analysee.
        //
        // Pour les utilisateurs de Visual Studio, la compilation en mode
        // "release" est ce qui s'approche le plus de l'option -O3
        assert(i->size() == budget + 1);
    }

    std::vector<std::vector<unsigned int> > tabcalcul(nb_compagnies);
    //la premiere ligne du tableau peut etre exrtraite directement de la premiere ligne de Profits
    for(unsigned int m = 0; m <= budget; m++){
        tabcalcul[0].push_back(profits[0][m]);
    }

    //la premiere colonne sera compose exclusivement de 0 car m = 0
    for (unsigned int i = 1;i < nb_compagnies; i++){
        tabcalcul[i].push_back(0);
    }

    for (unsigned int i = 1; i < nb_compagnies; i++){

        for (unsigned int m = 1; m <= budget; m++){

            //investir tout dans la nouvelle compagnie
            unsigned int allin = profits[i][m];

            //rien investir dans la nouvelle compagnie
            unsigned int notin = tabcalcul[i-1][m];

            //investir en partie dans la compagnie i
            unsigned int partin = splitinvestment(profits, m, i, tabcalcul);

            //On cherche lke max entre les 3 valeurs
            unsigned int interval = std::max(allin, notin);
            unsigned int res = std::max(interval, partin);

            tabcalcul[i].push_back(res);

        }

    }
    unsigned int ptrbudget = budget;
    unsigned int profitmax = tabcalcul[nb_compagnies - 1][budget];

    for (unsigned int x = nb_compagnies - 1; x >= 0; x--){
        unsigned int total = tabcalcul[x][ptrbudget];

        if(x == 0){
            montants.push_back(ptrbudget);
            break;
        }
        //si le chiffre au dessus est identique, pas d<investissement dans la compagnie actuelle
        else if(tabcalcul[x-1][ptrbudget] == tabcalcul[x][ptrbudget]){
            montants.push_back(0);
        }
        //si le chiffre correspond au chiffre dans le tableau de profit, tous les $ restants on ete mis dans cette corp.
        else if(tabcalcul[x][ptrbudget] == profits[x][ptrbudget]){
            montants.push_back(ptrbudget);
            for( int y = x - 1; y >= 0; y--){
                montants.push_back(0);
            }
            break;
        }
        else{
            for (unsigned int j = ptrbudget-1; j >= 1; j--){
                if(tabcalcul[x -1][j] + profits[x][ptrbudget-j] == tabcalcul[x][ptrbudget]){
                    montants.push_back(ptrbudget-j);
                    ptrbudget = ptrbudget - (ptrbudget-j);
                    break;
                }
            }
        }

    }
    std::reverse(montants.begin(), montants.end());//lineaire
}