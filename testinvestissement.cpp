#include "investissement.hpp"

#include <assert.h>
#include <iostream>
#include <numeric>
#include <string>

// Complexite: Theta(nb_entreprises)
bool instance_valide(const std::vector<std::vector<unsigned int> >& profits) {
    if (profits.size() == 0 || profits.front().size() == 0)
        return false;
    const unsigned int budget = profits.front().size() - 1;
    for (std::vector<std::vector<unsigned int> >::const_iterator i = profits.begin(); i != profits.end(); i++) {
        if (i->size() != budget + 1 || i->front() != 0) {
            return false;
        }
    }
    return true;
}

// Complexite: Theta(nb_entreprises)
bool solution_valide(const std::vector<std::vector<unsigned int> >& profits, const std::vector<unsigned int>& solution) {
    assert(instance_valide(profits));
    const unsigned int budget = profits.front().size() - 1;
    return (solution.size() == profits.size()) && (std::accumulate(solution.begin(), solution.end(), 0) <= budget);
}

// Complexite: Theta(nb_entreprises)
unsigned int profit(const std::vector<std::vector<unsigned int> >& profits, const std::vector<unsigned int>& solution) {
    assert(solution_valide(profits, solution));

    const unsigned int nb_entreprises = profits.size();
    unsigned int resultat = 0;
    for (unsigned int i = 0; i < nb_entreprises; i++) {
        resultat += profits[i][solution[i]];
    }
    return resultat;
}

// Complexite: Theta(budget^nb_entreprises)
void force_brute(const std::vector<std::vector<unsigned int> >& profits, std::vector<unsigned int>& solution) {
    assert(instance_valide(profits));
    const unsigned int nb_entreprises = profits.size();
    const unsigned int budget = profits.front().size() - 1;
    std::vector<unsigned int> meilleure_solution;
    unsigned int meilleur_profit = 0;
    std::vector<unsigned int> solution_courrante(nb_entreprises, 0);
    while (solution_courrante[nb_entreprises - 1] != budget) {
        unsigned int i = 0;
        while (solution_courrante[i] == budget) {
            solution_courrante[i++] = 0;
        }
        solution_courrante[i]++;

        if (solution_valide(profits, solution_courrante)) {
            const unsigned int profit_solution = profit(profits, solution_courrante);
            if (profit_solution > meilleur_profit) {
                meilleure_solution = solution_courrante;
                meilleur_profit = profit_solution;
            }
        }
    }
    solution.swap(meilleure_solution);
}

// Complexitre: Theta(nb_entreprises)
void imprime_solution(const std::string& etiquette, const std::vector<unsigned int>& solution, const std::vector<std::vector<unsigned int> >& profits) {
    std::cout << etiquette << " [";
    for (std::vector<unsigned int>::const_iterator i = solution.begin(); i != solution.end(); i++) { // Theta(nb_entreprises)
        std::cout << " " << *i;
    }
    std::cout << " ] (somme = " << std::accumulate(solution.begin(), solution.end(), 0); // Theta(nb_entreprises)
    if (solution_valide(profits, solution)) { // Theta(nb_entreprises)
        std::cout << ", profit = " << profit(profits, solution) << ")" << std::endl; // Theta(nb_entreprises)
    } else {
        std::cout << ")" << std::endl; // Theta(1)
    }
}

// Complexite: Theta(budget^nb_entreprises)
bool test(const std::string& nom_du_test, const std::vector<std::vector<unsigned int> >& profits) {
    assert(instance_valide(profits));

    std::vector<unsigned int> solution_programmation_dynamique;
    investissement(profits, solution_programmation_dynamique);

    std::vector<unsigned int> solution_force_brute;
    force_brute(profits, solution_force_brute);

    const bool resultat = solution_valide(profits, solution_programmation_dynamique)
                          && profit(profits, solution_programmation_dynamique) == profit(profits, solution_force_brute);

    if (resultat) {
        std::cout << "+1 point: Le test " << nom_du_test << " a passe" << std::endl;
    } else {
        std::cout << "+0 point: Le test " << nom_du_test << " a echoue" << std::endl;
    }
    imprime_solution("  Solution force brute:", solution_force_brute, profits);
    imprime_solution("  Solution programmation dynamique:", solution_programmation_dynamique, profits);
    return resultat;
}

// Complexite: Theta(nb_entreprises * budget)
void convertion_en_vecteur(unsigned int* profits, unsigned int nb_entreprises, unsigned int budget, std::vector<std::vector<unsigned int> >& sortie) {
    std::vector<std::vector<unsigned int> > resultat; // Theta(1)
    resultat.reserve(nb_entreprises);		    // Theta(1)
    unsigned int* debut = profits;
    unsigned int* fin;
    for (unsigned int i = 0; i < nb_entreprises; i++) { // Theta(nb_entreprises * budget)
        fin = debut + budget + 1;
        resultat.push_back(std::vector<unsigned int>(debut, fin)); // Theta(budget)
        debut = fin;
    }
    sortie.swap(resultat);	// Theta(1)
}

bool test_simple() {
    const unsigned int nb_entreprises = 3;
    const unsigned int budget = 3;
    unsigned int profits[nb_entreprises * (budget + 1)] = {0, 1, 1, 1,  // Entreprise 0
                                                           0, 0, 3, 3,  // Entreprise 1
                                                           0, 1, 2, 2}; // Entreprise 2
    std::vector<std::vector<unsigned int> > vecteur_profits;
    convertion_en_vecteur(profits, nb_entreprises, budget, vecteur_profits);
    return test("simple", vecteur_profits);
}

bool test_cinq_entreprises() {
    const unsigned int nb_entreprises = 5;
    const unsigned int budget = 5;
    unsigned int profits[nb_entreprises * (budget + 1)] = {0, 1, 2, 3, 4, 5,  // Entreprise 0
                                                           0, 2, 3, 3, 3, 3,  // Entreprise 1
                                                           0, 4, 4, 4, 4, 4,  // Entreprise 2
                                                           0, 0, 1, 1, 4, 4,  // Entreprise 3
                                                           0, 2, 2, 3, 3, 4}; // Entreprise 4
    std::vector<std::vector<unsigned int> > vecteur_profits;
    convertion_en_vecteur(profits, nb_entreprises, budget, vecteur_profits);
    return test("5 entreprises", vecteur_profits);
}

bool test_six_entreprises() {
    const unsigned int nb_entreprises = 6;
    const unsigned int budget = 7;
    unsigned int profits[nb_entreprises * (budget + 1)] = {0, 1, 2, 3, 4, 5, 6, 7,  // Entreprise 0
                                                           0, 2, 3, 3, 3, 3, 3, 3,  // Entreprise 1
                                                           0, 4, 4, 4, 4, 4, 4, 5,  // Entreprise 2
                                                           0, 0, 1, 1, 4, 4, 7, 7,  // Entreprise 3
                                                           0, 2, 2, 3, 3, 4, 4, 5,  // Entreprise 4
                                                           0, 1, 3, 5, 6, 7, 8, 9}; // Entreprise 5
    std::vector<std::vector<unsigned int> > vecteur_profits;
    convertion_en_vecteur(profits, nb_entreprises, budget, vecteur_profits);
    return test("6 entreprises", vecteur_profits);
}

bool test_unimodal() {
    const unsigned int nb_entreprises = 6;
    const unsigned int budget = 7;
    unsigned int profits[nb_entreprises * (budget + 1)] = {0, 1, 2, 3, 2, 1, 0, 0,  // Entreprise 0
                                                           0, 2, 3, 2, 1, 0, 0, 0,  // Entreprise 1
                                                           0, 1, 2, 0, 0, 0, 0, 0,  // Entreprise 2
                                                           0, 0, 1, 4, 2, 1, 0, 0,  // Entreprise 3
                                                           0, 2, 2, 3, 2, 1, 0, 0,  // Entreprise 4
                                                           0, 1, 3, 5, 3, 1, 0, 0}; // Entreprise 5
    std::vector<std::vector<unsigned int> > vecteur_profits;
    convertion_en_vecteur(profits, nb_entreprises, budget, vecteur_profits);
    return test("unimodal", vecteur_profits);
}

bool test_grosse_instance() {
    const unsigned int nb_entreprises = 9;
    const unsigned int budget = 6;
    unsigned int profits[nb_entreprises * (budget + 1)] = {0, 1, 2, 3, 2, 1, 0,  // Entreprise 0
                                                           0, 2, 3, 2, 1, 0, 0,  // Entreprise 1
                                                           0, 1, 2, 0, 0, 0, 0,  // Entreprise 2
                                                           0, 0, 1, 4, 2, 1, 0,  // Entreprise 3
                                                           0, 1, 0, 3, 2, 1, 0,  // Entreprise 4
                                                           0, 2, 1, 5, 3, 1, 0,  // Entreprise 5
                                                           0, 3, 1, 4, 4, 2, 1,  // Entreprise 6
                                                           0, 4, 1, 8, 2, 0, 0,  // Entreprise 7
                                                           0, 1, 4, 2, 9, 4, 3}; // Entreprise 8

    std::vector<std::vector<unsigned int> > vecteur_profits;
    convertion_en_vecteur(profits, nb_entreprises, budget, vecteur_profits);
    return test("grosse instance", vecteur_profits);
}

bool test_surplus_de_budget() {
    const unsigned int nb_entreprises = 3;
    const unsigned int budget = 5;
    unsigned int profits[nb_entreprises * (budget + 1)] = {0, 0, 2, 1, 0, 0,  // Entreprise 0
                                                           0, 0, 2, 1, 0, 0,  // Entreprise 1
                                                           0, 0, 0, 0, 0, 1}; // Entreprise 2
    std::vector<std::vector<unsigned int> > vecteur_profits;
    convertion_en_vecteur(profits, nb_entreprises, budget, vecteur_profits);
    return test("surplus de budget", vecteur_profits);
}

bool testcustom(){
    const unsigned int nb_entreprises = 5;
    const unsigned int budget = 5;
    unsigned int profits[nb_entreprises * (budget + 1)] = {0, 4, 6, 2, 4, 5,  // Entreprise 0
                                                           0, 2, 4, 4, 2, 6,  // Entreprise 1
                                                           0, 3, 7, 7, 8, 8,  // Entreprise 2
                                                           0, 2, 2, 2, 2, 2,  // Entreprise 3
                                                           0, 2, 6, 6, 7, 7}; // Entreprise 4
    std::vector<std::vector<unsigned int> > vecteur_profits;
    convertion_en_vecteur(profits, nb_entreprises, budget, vecteur_profits);
    return test("custom", vecteur_profits);
}

int main(void) {
    test_simple();
    test_cinq_entreprises();
    test_six_entreprises();
    test_unimodal();
    test_grosse_instance();
    test_surplus_de_budget();
//    testcustom();
    return 0;
}
