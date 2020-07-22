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
#include <math.h>

using namespace std;

vector< Histoire * > * lireDocuments( string a_nomFichier )
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
 vector<map<string,int>> & arbresAvls,vector<string> & listeTitre );
void CalculIdf(vector<map<string,int>> & arbresAvls, map<string,int> & valeurIdf, int sizeHistoire);

void decouperRequeteEnMots (string const & requete, vector<string> & tabMots);

vector<double> calculerMetrique (vector<map<string,int>> const & arbresAvls, 
        map<string,int> & valeurIdf, vector<string> const & tabMots);

void trouverCinqMeilleuresHistoires (vector<double> metriques, 
        vector<string> listeTitre);


int main() {

    //contient les histoires
    vector< Histoire * > * histoires = lireDocuments( string( "listeDocument.xml" ) );

    //vecteur qui va contenir les arbres AVL de chaque histoire
    vector<map<string,int>> arbresAvls;

    //vecteur contient les titres de chaque histoire
    vector<string> listeTitre;

    //arbre contenant les idf(m)
    map<string,int> valeurIdf;

    creerArbresAvlHistoire(histoires,arbresAvls,listeTitre);
    int sizeHistoire = histoires->size();
    CalculIdf(arbresAvls,valeurIdf,sizeHistoire);




        // Les histoires ont une variable de classe 'titre'.
        //cout << histoire->titre() << endl;

            // p.begin() va chercher le premier mot de la Phrase p.  c'est aussi un iterateur et il peut
            // s'utiliser avec les for augment�s.
            // ici, nous affichons seulement le premier mot de la Phrase.
            //cout << *( p.begin() ) << endl;


    /* PHASE DE REQUETE */

    bool finProg = false;
    // debut de la boucle de requetes
    while (!finProg) {

        string requete;
        vector<string> tabMots;
        // on demande une suite de mots a l'utilisateur
        cout << "Entrez votre requete : " << endl;
        getline(cin, requete);

        if (requete.empty()) {
            finProg = true;
        } else {
            // on decoupe la requete en mots, on les stocke dans tabMots
            decouperRequeteEnMots(requete, tabMots);
            // on calcule la metrique V pour chaque histoire
            vector<double> metriques = calculerMetrique(arbresAvls, valeurIdf, 
                tabMots);
            // on trouve les cinq meilleures histoires et on les affiche
            trouverCinqMeilleuresHistoires(metriques, listeTitre);
        }
    }
    return 0;
}

/**
Parcourt chaque histoire du vecteur histoire et creer un
arbre AVL pour categorisant chaque mot de l'histoire ou
la clef est le mot en string et la definition est le 
nombre d'occurence du mot dans l'histoire
@param &vector< Histoire * > * histoires, le vecteur
contenant les histoire
@param vector<ArbreAVL<string>> & arbresAvls le vecteur
qui stocke les arbre avl de chaque mot de l'histoire
Construit aussi le vecteur de titres
**/

//TODO : EST CE QU'IL FAUT AVOIR DES MOTS a 0?
void creerArbresAvlHistoire(vector< Histoire * > * const & histoires,
 vector<map<string,int>> & arbresAvls, vector<string> & listeTitre  ){
    

    for( Histoire * histoire : * histoires ){
        int index = 0;
        //creer un arbre avl
        map<string,int> arbre;
        //ajouter titre
        listeTitre.push_back(histoire->titre());
        //cout << histoire->titre() << endl;
        for( Phrase p : * histoire ){
            //chaque phrase ajouter le mot a l'arbre
            for(vector< string >::const_iterator iter = p.begin();
                iter!=p.end();++iter){
                //TODO occurence + verifier que le mot existe deja
                if(arbre.find(*iter) != arbre.end()){
                    // Si il y a repition changer nbr occurence du noeud
                    arbre.at(*iter)++;

                } else{
                    arbre.insert(pair<string,int>(*iter,0));
                }
            }
        }
        arbresAvls.push_back(arbre);
        //cout << "NOMBRE DE : and : " << arbre.at("and") << endl;
        index++;
        //mettre le nouvel arbre dans le vecteur
    }
}

void CalculIdf(vector<map<string,int>> & arbresAvls, map<string,int> & valeurIdf,
int sizeHistoire ){

    for(map<string,int> arbre : arbresAvls){
        //pour chaque element de l'arbre
        //cout << "Histoire" << endl;
        for(map<string,int>::iterator iter=arbre.begin();iter != arbre.end(); iter++){
            if(valeurIdf.find((*iter).first) != arbre.end()){
                //cout << (*iter).first << endl;
                string mot = (*iter).first;
                int nbrOccurence = 0;
                for(map<string,int> arbre2 : arbresAvls){
                    
                    if(arbre2.find(mot) != arbre2.end()){
                        nbrOccurence += arbre2.at(mot);
                    }

                }
                //calculer logarithme
                if(nbrOccurence != 0){
                    int valeurLog = log2(sizeHistoire/nbrOccurence);
                    //valeurIdf.insert(pair<string,int>(mot,valeurLog));
                    cout << valeurLog << endl;
                }
            }
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
vector<double> calculerMetrique (vector<map<string,int>> const & arbresAvls, 
        map<string,int> & valeurIdf, vector<string> const & tabMots) {
    vector<double> metriquePourChaqueHistoire;
    double metriqueV;
    for(int i = 0; i < arbresAvls.size(); ++i ) {
        for (string mot : tabMots) {
            double tf = ((arbresAvls.at(i)).find(mot))->second;
            double idf = (valeurIdf.find(mot))->second; 
            metriqueV += (tf * idf);
        }
        metriquePourChaqueHistoire.push_back(metriqueV);
    }
    return metriquePourChaqueHistoire;
}

/**
 * Trouve et affiche les cinq meilleurs histoires, d'apres leur metrique V.
 * 
 * @param metriques un vecteur contenant la metrique V de chaque histoire
 * @param arbresAvls les arbres des histoires
**/
void trouverCinqMeilleuresHistoires (vector<double> copieDeMetriques,
        vector<string> listeTitre) {
    
    // on inverse les deux pour respecter l'ordre des tests
    reverse(copieDeMetriques.begin(),copieDeMetriques.end());
    reverse(listeTitre.begin(),listeTitre.end());  
    
    for (int j = 0; j < 5; ++j) {
        int valeurMax = copieDeMetriques.at(0);
        int indiceMax = 0;
        for (int i = 1; i < copieDeMetriques.size(); ++i) {
            if (copieDeMetriques.at(i) > valeurMax) {
                valeurMax = copieDeMetriques.at(i);
                indiceMax = i;
            }
        }
        cout << valeurMax << " : " << listeTitre.at(indiceMax) << endl;
        // pour ne pas qu'il prenne le meme nombre chaque fois
        copieDeMetriques.at(indiceMax) = -1;
    }
}