
#if !defined(__PILE__H__)
#define __PILE__H__

/*
 * Permet la creation de pile necessaire a la classe arbreAVL.h.
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

template <class T>
class Pile
{
  public:
    Pile();
    Pile(const Pile&);
    ~Pile();

    void empiler(const T&);
  
    void depiler(T& out);  // Depiler et copier le sommet dans out.
    T depiler();

    bool vide() const { return sommet==NULL;}
    void vider();

    const Pile<T>&    operator = (const Pile<T>&);

  private:
    
    class Cellule
    {
      public:
        Cellule(const T& e, Cellule* c);
        T contenu;
        Cellule* suivante;
    };

    Cellule* sommet;
};

#include "assert.h"
#include "pile.h"

template <class T>
Pile<T>::Pile()
{
    sommet = NULL;
}

template <class T>
Pile<T>::~Pile()
{
    vider();
}

template <class T>
Pile<T>::Pile(const Pile<T>& autre)
{
    sommet = NULL;
    *this = autre;
}

template <class T>
void Pile<T>::vider()
{
    while(!vide())
        depiler();  
}

template <class T>
Pile<T>::Cellule::Cellule(const T& e, Cellule* c)
 : suivante(c)
{
    contenu=e;
}

template <class T>
void Pile<T>::empiler(const T& e)
{
    sommet = new Cellule(e, sommet);
    assert(sommet);
}

template <class T>
T Pile<T>::depiler()
{
    assert(sommet);
    Cellule c(*sommet);
    delete sommet;
    sommet = c.suivante;
    return c.contenu;
}

template <class T>
void Pile<T>::depiler(T& e)
{
    assert(sommet!=NULL);
    e = sommet->contenu;
    Cellule* c = sommet;
    sommet = sommet->suivante;
    delete c;
}

template <class T>
const Pile<T>& Pile<T>::operator = (const Pile<T>& autre)
{
    if(&autre == this) return *this;
 
    Cellule** t=&sommet;
    Cellule* a=autre.sommet;
    while(a){
        if((*t) == NULL)
             *t = new Cellule(a->contenu, NULL);
        else
             (*t)->contenu = a->contenu;
        t = &((*t)->suivante);
        a = a->suivante;
    }
    a = *t;
    *t = NULL;
    while(a!=NULL){
        Cellule* temp = a->suivante;
        delete a;
        a = temp;
    }
    return *this;
}

#endif

