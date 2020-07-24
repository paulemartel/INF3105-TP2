
#if !defined(__ARBREMAP_H__)
#define __ARBREMAP_H__

/*
 * Permet la construction d'arbres map pour le stockage des mots contenus
 * dans les histoires et des statistiques connexes.
 *
 * Auteures : Paule Martel et Gabrielle Poitras
 * Code permanent : 
 *     MARP16569700
 *     POIG16519008
 * Courriel : 
 *     martel.paule@courrier.uqam.ca
 *     poitras.gabrielle.2@courrier.uqam.ca
 * Cours : INF3105-30
 * Date : 2020-07-23
 */

#include "arbreavl.h"

template <class K, class V>
class ArbreMap
{
  private:
        class Entree {
        public:
            Entree(const K& c): cle(c),valeur(){}
            K cle;
            V valeur;
            bool operator < (const Entree& e) const{
                return cle < e.cle;
            }
            bool operator > (const Entree& e) const{
                return cle > e.cle;
            }
            bool operator == (const Entree& e) const{
                return cle == e.cle;
            }
    };

    ArbreAVL<Entree> entrees;

  public:
        class Iterateur{
        public:
            Iterateur(const ArbreMap& a) : iter(a.entrees.debut()){}
            Iterateur(typename ArbreAVL<Entree>::Iterateur i) : iter(i){}
            operator bool() const;
            bool operator!() const;

            Iterateur& operator++();
            Iterateur operator++(int);

            const K& cle() const;
            const V& valeur() const;

        private:
            typename ArbreAVL<Entree>::Iterateur iter;
    };

    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const K& cle) const;
    bool contient(const K&) const;
    void enlever(const K&);
    void vider();
    bool vide() const;
    const V& operator[] (const K&) const;
    V& operator[] (const K&);
};

/**
Vide l'arbre de son contenu
**/
template <class K, class V>
void ArbreMap<K,V>::vider(){
    entrees.vider();
}

/**
Indique si l'arbre est vide
@return true si c'est le cas sinon false
**/
template <class K, class V>
bool ArbreMap<K,V>::vide() const{
    return entrees.vide();
}

/**
Enleve un element a l'arbre
@param const K& c clef a enlever
**/
template <class K, class V>
void ArbreMap<K,V>::enlever(const K& c)
{
    entrees.enlever(Entree(c));
}

/**
Accede au contenu d'un element de l'arbre
@param const K& c clef de l'element
@return la valeur attachee a la clef
**/
template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const
{
    typename ArbreAVL<Entree>::Iterateur iter=entrees.rechercher(c);
    return entrees[iter].valeur;
}

/**
Accede au contenu d'un element de l'arbre
mais la recherche avant
@param const K& c clef de l'element
@return la valeur attachee a la clef
**/
template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c) 
{
    typename ArbreAVL<Entree>::Iterateur iter=entrees.rechercher(Entree(c));
    if(!iter){
        entrees.inserer(Entree(c));
        iter = entrees.rechercher(c);
    }
    return entrees[iter].valeur;
}

/**
Pointe au debut de l'arbre map
@return l'iterateur pointant au debut
**/
template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::debut() const{
    return Iterateur(*this);
}

/**
Pointe a la fin de l'arbre map
@return l'iterateur pointant a la fin
**/
template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::fin() const{
    typename ArbreAVL<Entree>::Iterateur iter(entrees);
    return Iterateur(iter);
}

/**
Va chercher la clef
@return contenu de la clef
**/
template <class K, class V>
const K& ArbreMap<K,V>::Iterateur::cle() const{
    return (*iter).cle;
}

/**
Va chercher la valeur
@return contenu de la valeur
**/
template <class K, class V>
const V& ArbreMap<K,V>::Iterateur::valeur() const{
    return (*iter).valeur;
}

/**
Incremente la valeur d'un int dans l'arbre
**/
template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::Iterateur::operator ++(int){
    Iterateur copie(*this);
    operator++();
    return copie;
}

/**
Incremente une valeur dans l'arbre
**/
template <class K, class V>
typename ArbreMap<K,V>::Iterateur& ArbreMap<K,V>::Iterateur::operator ++(){
    if(!*this) return *this;
    iter++;
    return *this;
}

/**
indique si l'iterateur existe
@return true si c'est le cas sinon false
**/
template <class K, class V>
ArbreMap<K,V>::Iterateur::operator bool() const{
    return iter;
}

/**
indique si l'iterateur existe pas
@return true si c'est le cas sinon false
**/
template <class K, class V>
bool ArbreMap<K,V>::Iterateur::operator!() const{
    return !iter;
}

/**
Recherche dans l'arbre map a l'aide de l'iterateur
@param const K& e element a rechercher
@return l'iterateur pointant sur la valeur trouvee
**/
template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::rechercher(const K& e) const{
    return entrees.rechercher(Entree(e));
}

#endif

