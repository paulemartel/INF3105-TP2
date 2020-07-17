//#include "DocumentXML.hpp"
//#include "ElementXML.hpp"
//#include "Histoire.hpp"
//#include "Lecteur.hpp"
//#include "arbreavl.h"


#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// declarations
void decouperRequeteEnMots (string const & requete, vector<string> & tabMots);

int main() {
    
bool finProg = false;
    // debut de la boucle de requetes
    while (!finProg) {

        string requete;
        vector<string> tabMots;
        // on demande une suite de mots a l'utilisateur
        cout << "Entrez votre requete : " << endl;
        cin >> requete;

        if (requete.empty()) {
            finProg = true;
            cout << "Fin normale du programme" << endl;
        } else {
            // on decoupe la requete en mots, on les stocke dans tabMots
            decouperRequeteEnMots(requete, tabMots);

            for (string mot : tabMots) {
                cout << mot << endl;
            }
            // on calcule la metrique V pour chaque histoire
            //vector<double> metriques = calculerMetrique(arbresAvls);
            // trouver 5 histoires et les afficher
            //trouverCinqMeilleuresHistoires(metriques, arbresAvls); 
        }
    }
    return 0;

}


/**
 * Extrait les differents mots de la requete entree par l'utilisateur, les 
 * insere dans un vecteur et les retourne.
 * 
 * @param requete la requete de l'utilisateur
 * @param tabmots le tableau de mots extraits
**/
void decouperRequeteEnMots (string const & requete, vector<string> & tabMots) {
    string temp = "";
    for(int i = 0; i < requete.length(); ++i) {
        if (isalpha(requete.at(i)) || requete.at(i) == '-') {
           temp += requete.at(i);
           if (i == requete.length() - 1) {
               tabMots.push_back(temp);
           }
        } else if (temp.length() != 0) {
            tabMots.push_back(temp);
            temp = "";
        }
    }
}