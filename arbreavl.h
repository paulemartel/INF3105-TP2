
#if !defined(__ARBREAVL_H__)
#define __ARBREAVL_H__

/*
 * Permet la creation d'arbres AVL, sur lesquels sont ensuite bases les
 * arbres de la classe arbremap.h.
 * Code repris en grande partie des laboratoires.
 *
 * Auteures : Paule Martel et Gabrielle Poitras
 * Code permanent : 
 *     MARP16569700
 *     POIG16519008
 * Courriel : 
 *     martel.paule@courrier.uqam.ca
 *     poitras.gabrielle.2@courrier.uqam.ca
 * Cours : INF3105-30
 * Date : 2020-06-22
 */

#include <cassert>
#include "pile.h"

template <class T>
class ArbreAVL {
  public:
    ArbreAVL();
    ArbreAVL(const ArbreAVL&);
    ~ArbreAVL();

    // Annonce l'existence d'une classe Iterateur
    class Iterateur;

    void inserer(const T&);
    bool contient(const T&) const;
    bool vide() const;
    void vider();
    void enlever(const T&);
    int  hauteur() const;
    
    // Fonctions pour obtenir un iterateur (position dans l'arbre)    
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    // Acces aux elements de l'arbre via un iterateur
    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);
    
    // Copie d'un arbre AVL
    ArbreAVL& operator = (const ArbreAVL&);

  private:
    struct Noeud{
        Noeud(const T&);
        T contenu;
        int nbrOccurence = 1;
        int equilibre;
        Noeud *gauche,
              *droite;
    };
    Noeud* racine;

    // Fonctions internes
    bool inserer(Noeud*&, const T&);
    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);
    void vider(Noeud*&);
    void copier(const Noeud*, Noeud*&) const;
    const T& max(Noeud*) const;
    bool enlever(Noeud*&, const T& e);
    const T* trouver(const T& element, const Noeud* n) const;

  public:

    class Iterateur{
      public:
        Iterateur(const ArbreAVL& a);
        Iterateur(const Iterateur& a);
        Iterateur(const ArbreAVL& a, Noeud* c);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;
        
        const T& operator*() const;

        Iterateur& operator++();
        Iterateur operator++(int);
        Iterateur& operator = (const Iterateur&);
      private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;
        
      friend class ArbreAVL;
    };
};
//constructeurs et destructeurs
template <class T>
ArbreAVL<T>::Noeud::Noeud(const T& c)
 : contenu(c), equilibre(0), gauche(NULL), droite(NULL)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL() 
 : racine(NULL)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T>& autre)
 : racine(NULL)
{
    copier(autre.racine, racine);
}

template <class T>
ArbreAVL<T>::~ArbreAVL()
{
    vider(racine);
}

/**
* Verifie si l'arbre contient un element.
* 
* @param const T& element element a verifier
* @param return true si c'est le cas, sinon false
**/
template <class T>
bool ArbreAVL<T>::contient(const T& element) const
{
    return (trouver(element, racine) != nullptr);
}

/**
* Trouve un element dans la liste.
* 
* @param const T& element element a trouver
* @param const Noeud* n noeud ou on se trouve presentement
* @return contenu
**/
template <class T>
const T* ArbreAVL<T>::trouver(const T& element, const Noeud* n) const
{
    if (n == nullptr) {
        return nullptr;
    }
    if (n->contenu < element) {
        return trouver(element, n->droite);
    }
    if (n->contenu > element) {
        return trouver(element, n->gauche);
    }
    return &(n->contenu);
}

/**
* Insere un element dans la liste.
*
* @param const T& element element a inserer
**/
template <class T>
void ArbreAVL<T>::inserer(const T& element)
{
    inserer(racine, element);
}

/**
* Insere un element et equilibre l'arbre.
* 
* @param Noeud*& noeud noeud a equilibrer
* @param const T& element element a inserer
**/
template <class T>
bool ArbreAVL<T>::inserer(Noeud*& noeud, const T& element)
{
    if(noeud==NULL)
    {
        noeud = new Noeud(element);
        return true;
    }
    if(element < noeud->contenu){
        if(inserer(noeud->gauche, element))
        {
            noeud->equilibre++;
            if(noeud->equilibre == 0)
                return false;
            if(noeud->equilibre == 1)
                return true;
            assert(noeud->equilibre==2);
            if(noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return false;
    }
    else if(noeud->contenu < element){
        if (inserer(noeud->droite, element))
        {
            noeud->equilibre--;
            if (noeud->equilibre == 0) {
                return false;
            }
            if (noeud->equilibre == -1) {
                return true;
            }
            assert (noeud->equilibre == -2);
            if (noeud->droite->equilibre == 1) {
                rotationGaucheDroite(noeud->droite);
            }
            rotationDroiteGauche(noeud);
        }
        return false;
    } else {

        noeud->contenu = element; 
        return false;
    }
}

/**
* Fait une rotation gauche-droite sur la liste.
*
* @param Noeud*& racinesousarbre noeud a equilibrer
**/
template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre)
{
    Noeud *temp = racinesousarbre->gauche;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = -(ea>0 ? ea : 0) - 1 + eb;
    int  nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

/**
* Fais une rotation droite-gauche sur la liste.
*
* @param Noeud*& racinesousarbre noeud a equilibrer
**/
template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre)
{
    Noeud *a = racinesousarbre->droite;
    Noeud *b = racinesousarbre;
    int  ea = a->equilibre;
    int  eb = b->equilibre;
    int  ebp = eb + (-(ea) > 0 ? -(ea) : 0) + 1;
    int  eap = (ebp > 0 ? ebp : 0) + 1 + ea;

    a->equilibre = eap;
    b->equilibre = ebp;
    b->droite = a->gauche;
    a->gauche = b;
    racinesousarbre = a;
}

/**
* Verifie si la liste est vide.
*
* @return true si c'est le cas sinon false
**/
template <class T>
bool ArbreAVL<T>::vide() const
{
    return (racine == nullptr);
}

/**
* Vide la liste.
**/
template <class T>
void ArbreAVL<T>::vider(){
  vider(racine);
  racine = nullptr; 
}

/**
* Vide la liste a partir d'un certain noeud.
*
* @param Noeud*& noeud noeud ou vider
**/
template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud)
{
    if (noeud == nullptr) {
        return;
    }
    vider(noeud->gauche);
    vider(noeud->droite);
    delete noeud;
    noeud = nullptr;
}

/**
* Copie le contenu d'un noeud dans une autre.
*
* @param const Noeud* source noeud source
* @param Noeud*& noeud nouveau noeud
**/
template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const
{
    if(source!=NULL){
        noeud = new Noeud(source->contenu);
        copier(source->gauche,noeud->gauche);
        copier(source->droite,noeud->droite);
    }
}

/**
* Trouve le noeud maximum.
* 
* @param Noeud* n noeud de depart
* @return contenu noeud max
**/
template <class T>
const T& ArbreAVL<T>::max(Noeud* n) const
{
    Noeud* courant = n;
    while (courant->droite != nullptr) {
        courant = courant->droite;
    }
    return courant->contenu;
}

/**
* Enleve un element de la liste.
*
* @param const T& element element a enlever
**/
template <class T>
void ArbreAVL<T>::enlever(const T& element)
{
    enlever(racine, element);
}

/**
* Enleve un element et equilibre l'arbre.
*
* @param Noeud*& noeud noeud a equilibrer
* @param const T& element element a enlever
**/
template <class T>
bool ArbreAVL<T>::enlever(Noeud*& noeud, const T& element)
{
    if(element < noeud->contenu)
    {
        if(enlever(noeud->gauche, element))
        {
            noeud->equilibre--;
            if(noeud->equilibre == 0)
                return true;
            if(noeud->equilibre == -1)
                return false;
            assert(noeud->equilibre == -2);
            if(noeud->droite->equilibre == 1)
                rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);
        }
        return false;
    }
    else if(element > noeud->contenu)
    {
        if(enlever(noeud->droite, element))
        {   
            noeud->equilibre++;
            if(noeud->equilibre == 0) {
                return true;
            }
            if(noeud->equilibre == 1) {
                return false;
            }
            assert(noeud->equilibre == 2);
            if(noeud->gauche->equilibre == -1) {
                rotationDroiteGauche(noeud->gauche);
            }                                                                   
            rotationGaucheDroite(noeud);
        }
        return false;
    }
    else if(element == noeud->contenu)
    {
        if (noeud->gauche==NULL && noeud->droite==NULL)
        {
            delete noeud;
            noeud = NULL;
            return true;
        }
        else
        {
            if (noeud->gauche==nullptr||noeud->droite==nullptr) {
                if (noeud->gauche == nullptr) { 
                    Noeud* temp = noeud->droite;
                    delete noeud;
                    noeud = noeud->droite;
                } else { 
                    Noeud* temp = noeud->gauche;
                    delete noeud;
                    noeud = noeud->gauche;
                }
            } else {
                noeud->contenu = max(noeud->gauche);
                return enlever(noeud->gauche, noeud->contenu);
            }
            return true;
        }
    }
    return false;
}

/**
* Donne un pointeur vers le debut de la liste.
*
* @return pointeur vers le debut de la liste
**/
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const
{
    Iterateur iter(*this);
    iter.courant = racine;
    if(iter.courant!=NULL) {
        while(iter.courant->gauche!=NULL) {
            iter.chemin.empiler(iter.courant);
            iter.courant->gauche;
        }
    }
    return iter;
}

/**
* Donne un pointeur vers la fin de la liste.
*
* @return pointeur pointant a la fin de la liste
**/
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const
{
    return Iterateur(*this);
}

/**
* Rechercher dans la liste a l'aide de l'iterateur.
*
* @param const T& e element a rechercher
* @return pointeur vers l'element trouve
**/
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher
(const T& e) const
{
    Iterateur iter(*this);
    Noeud* n = racine;
    while(n) {
        if(e < n->contenu) {
            iter.chemin.empiler(n);
            n = n->gauche;
        } else if (n->contenu < e) {
            n = n->droite;
        } else {
            iter.courant = n;
            return iter;
        }
    }
    iter.chemin.vider();
    return iter;
}

/**
* Recherche dans la liste un element egal ou suivant.
*
* @return pointeur vers l'element
**/
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant
(const T& e) const
{
    Iterateur iter(*this);
    return iter;
}

/**
* Recherche dans la liste un element egal ou precedent.
*
* @return pointeur vers l'element
**/
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent
(const T& e) const
{
    Noeud* n = racine, *dernier=NULL;
    while(n) {
        if(e < n->contenu) {
            n = n->gauche;
        } else if(n->contenu < e) {
            dernier = n;
            n = n->droite;
        } else {
            return rechercher(e);
        }
    }
    if(dernier!=NULL) {
        return rechercher(dernier->contenu);
    }
    return Iterateur(*this);
}

/**
* Accede a un element dans la liste et lui assigne un contenu.
*
* @param const Iterateur& iterateur iterateur sur element
* @return le contenu
**/
template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}
/**
* Accede a un element dans la liste et lui assigne un contenu (version avec 
* passage en reference).
*
* @param const Iterateur& iterateur iterateur sur element
* @return le contenu
**/
template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur)
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}
/**
* Transpose une liste dans une nouvelle liste.
*
* @param const ArbreAVL& autre nouvelle liste
**/
template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    if(this==&autre) return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}

/**
* Contruit un iterateur.
*
* @param const ArbreAVL& a arbreAvl
**/
template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
 : arbre_associe(a), courant(NULL)
{
}

/**
* Contruit un iterateur.
*
* @param const ArbreAVL<T>::Iterateur& a iterateur sur arbre avl
**/
template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
: arbre_associe(a.arbre_associe)
{
    courant = a.courant;
    chemin = a.chemin;
}

/**
* Incremente un element dans l'arbre.
**/
template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++()
{
    assert(courant);
    Noeud* suivant = courant->droite;
    while(suivant) {
        chemin.empiler(suivant);
        suivant = suivant->gauche;
    }
    if(!chemin.vide()) {
        courant = chemin.depiler();
    } else {
        courant = NULL;
    }
    return *this;
}

/**
* Incremente un element dans l'arbre.
**/
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int)
{
    Iterateur copie(*this);
    operator++();
    return copie;
}

/**
* Indique si l'iterateur n'est pas vide.
*
* @return true si c'est le cas sinon false
**/
template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
    return courant!=NULL;
}

/**
* Indique si l'iterateur est vide.
*
* @return true si c'est le cas, sinon false
**/
template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const{
    return courant==NULL;
}

/**
* Compare deux iterateurs.
*
* @return true si pareil, sinon false
**/
template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

/**
* Compare deux iterateurs.
*
* @return true si pas pareil, sinon false
**/
template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

/**
* Accede au contenu.
*
* @return contenu
**/
template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const
{
    assert(courant!=NULL);
    return courant->contenu;
}

/**
* Associe deux arbres.
*
* @return nouvel arbre
**/
template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator
 = (const Iterateur& autre){
    assert(&arbre_associe==&autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif
