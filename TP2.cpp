

#include "DocumentXML.hpp"
#include "ElementXML.hpp"
#include "Histoire.hpp"
#include "Lecteur.hpp"
#include "arbreavl.h"


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



vector< Histoire * > *





lireDocuments( string a_nomFichier )
{
    vector< Histoire * > * histoires = new vector< Histoire * >();
    DocumentXML * listeFichiers = lireFichierXML( a_nomFichier );

    assert( nullptr != listeFichiers );
    ElementXML * courant = listeFichiers->racine();
    assert( nullptr != courant );

    courant = courant->prochainElement();

    assert( nullptr != courant );
    for( NoeudXML * contenu : * courant )
    {
        assert( nullptr != contenu );
        if( contenu->estElement() )
        {
            ElementXML * element = ( ElementXML * )contenu;

            assert( nullptr != element );
            DocumentXML * doc = lireFichierXML( element->attribut( string( "fichier" ) ) );

            assert( nullptr != doc );
            vector< Histoire * > * h = extraireHistoires( * doc );

            assert( nullptr != h );
            assert( nullptr != histoires );
            histoires->insert( histoires->end(), h->begin(), h->end() );
        }
    }

    return histoires;
}

//declaration de fonctions
//void creerArbresAvlHistoire(&vector< Histoire * > * histoires);
void creerArbresAvlHistoire(vector< Histoire * > * const & histoires,
 vector<ArbreAVL<string>> & arbresAvls);

void decouperRequeteEnMots (string const & requete, vector<string> & tabMots);
vector<double> calculerMetrique (vector<ArbreAVL<string>> arbresAvls);

void trouverCinqMeilleuresHistoires (vector<double> metriques, 
    vector<ArbreAVL<string>> arbresAvls);


int main() {

    //contient les histoires
    vector< Histoire * > * histoires = lireDocuments( string( "listeDocument.xml" ) );

    //vecteur qui va contenir les arbres AVL de chaque histoire
    vector<ArbreAVL<string>> arbresAvls;

    creerArbresAvlHistoire(histoires,arbresAvls);




    // Pour votre projet, enlevez le code qui suit et remplacer le par votre code.
    // vous pouvez ajouter des fonctions.

    // Parcourir les histoires a l'aide de l'iterateur du vecteur.
    for( Histoire * histoire : * histoires )
    {
        // Les histoires ont une variable de classe 'titre'.
        //cout << histoire->titre() << endl;

        // Parcourir les Phrases qui compose une histoire � l'aide de l'iterateur des Histoires.
        for( Phrase p : * histoire )
        {
            // p.begin() va chercher le premier mot de la Phrase p.  c'est aussi un iterateur et il peut
            // s'utiliser avec les for augment�s.
            // ici, nous affichons seulement le premier mot de la Phrase.
            //cout << *( p.begin() ) << endl;
        }

        //cout << endl;
    }

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
        } else {
            // on decoupe la requete en mots, on les stocke dans tabMots
            decouperRequeteEnMots(requete, tabMots);
            // on calcule la metrique V pour chaque histoire
            vector<double> metriques = calculerMetrique(arbresAvls);
            // trouver 5 histoires et les afficher
            trouverCinqMeilleuresHistoires(metriques, arbresAvls); 
        }
    }
    return 0;
}

/**
* Parcourt chaque histoire du vecteur histoire et creer un
* arbre AVL pour categorisant chaque mot de l'histoire ou
* la clef est le mot en string et la definition est le 
* nombre d'occurence du mot dans l'histoire
*
* @param &vector< Histoire * > * histoires, le vecteur
* contenant les histoire
* @param vector<ArbreAVL<string>> & arbresAvls le vecteur
* qui stocke les arbre avl de chaque mot de l'histoire
**/

//vector<Pile<string,int>> arbresAvls

//TODO : EST CE QU'IL FAUT AVOIR DES MOTS a 0?
void creerArbresAvlHistoire(vector< Histoire * > * const & histoires,
 vector<ArbreAVL<string>> & arbresAvls ){
    ArbreAVL<string> arbre; //comment mettre nombre d'occurence en plus?
    for( Histoire * histoire : * histoires ){
        //cout << histoire->titre() << endl;
        for( Phrase p : * histoire ){
            //cout << *(p.end()) << endl;
            // for(int i = 0 ; i < p.end() ; i++){
            //     //cout << *( p.begin() + i ) << endl;

            // }
            //cout << *( p.begin() ) << endl

        }

    }


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

/**
 * Calcule la metrique V pour chaque histoire, la stocke dans un vecteur et 
 * la retourne.
 * 
 * @param arbresAvls les arbres des histoires
 * @param tabmots le tableau de mots extraits
 * @return un vecteur contenant la metrique V de chaque histoire
**/
vector<double> calculerMetrique (vector<ArbreAVL<string>> const & arbresAvls, 
        vector<string> const & tabMots) {
    vector<double> metriquePourChaqueHistoire;
    double metriqueV;
    for(int i = 0; i < arbresAvls.size(); ++i ) {
        for (string mot : tabMots) {
            // obtenir tf du mot dans cette histoire-la
            // obtenir idf du mot (global)
            // metriqueV += (tf * idf)
        }
        metriquePourChaqueHistoire.push_back(metriqueV);
    }
    return metriquePourChaqueHistoire;
}

/**
 * Trouve et affiche les cinq meilleurs histoires, d'apres leur metrique V
 * 
 * @param metriques un vecteur contenant la metrique V de chaque histoire
 * @param arbresAvls les arbres des histoires
**/
void trouverCinqMeilleuresHistoires (vector<double> metriques, 
        vector<ArbreAVL<string>> const & arbresAvls) {
    
    vector<string> listeDesTitres;
    // on a aussi le vector des metriques, dans meme ordre
    // on cree la liste des titres 
    for(auto arbre : arbresAvls) {
        // aller chercher le titre, voir avec Paule
    }

    // on trouve les cinq histoires
    for (int i = 0; i < 5; ++i) {
        int max; 
        int indice; 
        for (int j = 0; j < metriques.size(); ++j) {
            if (metriques.at(i + 1) > metriques.at(i)) {
                max = metriques.at(i);
                indice = i;
            }
        }
        // on affiche la metrique et le titre
        cout << max << " : " << listeDesTitres.at(i) << endl;
        // pour ne pas qu'il prenne le meme nombre chaque fois
        // on travaille avec une copie de metriques donc c'est pas grave anyway
        metriques.at(i) = 0; 
    } 
}


