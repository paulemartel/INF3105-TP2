
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

    // Annonce l'existence d'une classe Iterateur.
    class Iterateur;

    void inserer(const T&);
    bool contient(const T&) const;
    bool vide() const;
    void vider();
    void enlever(const T&);
    int  hauteur() const;
    
    // Fonctions pour obtenir un itérateur (position dans l'arbre)    
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    // Accès aux éléments de l'arbre via un intérateur.
    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);
    
    // Copie d'un arbre AVL.
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

template <class T>
bool ArbreAVL<T>::contient(const T& element) const
{
    return (trouver(element, racine) != nullptr);
}

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

template <class T>
void ArbreAVL<T>::inserer(const T& element)
{
    inserer(racine, element);
}

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

template <class T>
bool ArbreAVL<T>::vide() const
{
    return (racine == nullptr);
}

template <class T>
void ArbreAVL<T>::vider(){
  vider(racine);
  racine = nullptr; 
}

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

template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const
{
    if(source!=NULL){
        noeud = new Noeud(source->contenu);
        copier(source->gauche,noeud->gauche);
        copier(source->droite,noeud->droite);
    }
}

template <class T>
const T& ArbreAVL<T>::max(Noeud* n) const
{
    Noeud* courant = n;
    while (courant->droite != nullptr) {
        courant = courant->droite;
    }
    return courant->contenu;
}

template <class T>
void ArbreAVL<T>::enlever(const T& element)
{
    enlever(racine, element);
}

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

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const
{
    return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const
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

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(const T& e) const
{
    Iterateur iter(*this);
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(const T& e) const
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

template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur)
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    if(this==&autre) return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
 : arbre_associe(a), courant(NULL)
{
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
: arbre_associe(a.arbre_associe)
{
    courant = a.courant;
    chemin = a.chemin;
}

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

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int)
{
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
    return courant!=NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const{
    return courant==NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const
{
    assert(courant!=NULL);
    return courant->contenu;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre){
    assert(&arbre_associe==&autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif
