

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
 vector<map<string,int>> & arbresAvls);


int main() {

    //contient les histoires
    vector< Histoire * > * histoires = lireDocuments( string( "listeDocument.xml" ) );

    //vecteur qui va contenir les arbres AVL de chaque histoire
    vector<map<string,int>> arbresAvls;

    creerArbresAvlHistoire(histoires,arbresAvls);




    // Pour votre projet, enlevez le code qui suit et remplacer le par votre code.
    // vous pouvez ajouter des fonctions.

    // Parcourir les histoires a l'aide de l'iterateur du vecteur.
    for( Histoire * histoire : * histoires )
    {
        // Les histoires ont une variable de classe 'titre'.
        //cout << histoire->titre() << endl;

        // Parcourir les Phrases qui compose une histoire à l'aide de l'iterateur des Histoires.
        for( Phrase p : * histoire )
        {
            // p.begin() va chercher le premier mot de la Phrase p.  c'est aussi un iterateur et il peut
            // s'utiliser avec les for augmentés.
            // ici, nous affichons seulement le premier mot de la Phrase.
            //cout << *( p.begin() ) << endl;
        }

        //cout << endl;
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
**/

//TODO : EST CE QU'IL FAUT AVOIR DES MOTS a 0?
void creerArbresAvlHistoire(vector< Histoire * > * const & histoires,
 vector<map<string,int>> & arbresAvls ){
    

    for( Histoire * histoire : * histoires ){
        int index = 0;
        //creer un arbre avl
        map<string,int> arbre;
        //cout << histoire->titre() << endl;
        for( Phrase p : * histoire ){
            //chaque phrase ajouter le mot a l'arbre
            for(vector< string >::const_iterator iter = p.begin();
                iter!=p.end();++iter){
                //TODO occurence + verifier que le mot existe deja
                if(arbre.find(*iter) != arbre.end()){
                    //cout << "REPETITION" <<endl;
                    // Si il y a repition changer nbr occurence du noeud
                    //*position.ajoutOccurence();
                    arbre.at(*iter)++;

                } else{
                    arbre.insert(pair<string,int>(*iter,0));
                }
                //cout << *iter <<endl;
            }
        }
        arbresAvls.push_back(arbre);
        cout << "NOMBRE DE : and : " << arbre.at("and") << endl;
        index++;
        //mettre le nouvel arbre dans le vecteur


    }


}




