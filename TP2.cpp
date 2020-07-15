

#include "DocumentXML.hpp"
#include "ElementXML.hpp"
#include "Histoire.hpp"
#include "Lecteur.hpp"


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


int main() {
    // gardez la ligne suivante, elle lit le corpus et le place dans la structure de base.
    // Vous avez donc un vecteur d'histoire qui contient l'information sur les histoires,
    // les Phrases et les mots qu'elles contiennent.
    vector< Histoire * > * histoires = lireDocuments( string( "listeDocument.xml" ) );


    // Pour votre projet, enlevez le code qui suit et remplacer le par votre code.
    // vous pouvez ajouter des fonctions.

    // Parcourir les histoires a l'aide de l'iterateur du vecteur.
    for( Histoire * histoire : * histoires )
    {
        // Les histoires ont une variable de classe 'titre'.
        cout << histoire->titre() << endl;

        // Parcourir les Phrases qui compose une histoire à l'aide de l'iterateur des Histoires.
        for( Phrase p : * histoire )
        {
            // p.begin() va chercher le premier mot de la Phrase p.  c'est aussi un iterateur et il peut
            // s'utiliser avec les for augmentés.
            // ici, nous affichons seulement le premier mot de la Phrase.
            cout << *( p.begin() ) << endl;
        }

        cout << endl;
    }

    return 0;
}

