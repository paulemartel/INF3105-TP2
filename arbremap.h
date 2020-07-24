
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

template <class K, class V>
void ArbreMap<K,V>::vider(){
    entrees.vider();
}

template <class K, class V>
bool ArbreMap<K,V>::vide() const{
    return entrees.vide();
}

template <class K, class V>
void ArbreMap<K,V>::enlever(const K& c)
{
    entrees.enlever(Entree(c));
}

template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const
{
    typename ArbreAVL<Entree>::Iterateur iter=entrees.rechercher(c);
    return entrees[iter].valeur;
}

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

template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::debut() const{
    return Iterateur(*this);
}

template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::fin() const{
    typename ArbreAVL<Entree>::Iterateur iter(entrees);
    return Iterateur(iter);
}

template <class K, class V>
const K& ArbreMap<K,V>::Iterateur::cle() const{
    return (*iter).cle;
}

template <class K, class V>
const V& ArbreMap<K,V>::Iterateur::valeur() const{
    return (*iter).valeur;
}

template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::Iterateur::operator ++(int){
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class K, class V>
typename ArbreMap<K,V>::Iterateur& ArbreMap<K,V>::Iterateur::operator ++(){
    if(!*this) return *this;
    iter++;
    return *this;
}

template <class K, class V>
ArbreMap<K,V>::Iterateur::operator bool() const{
    return iter;
}

template <class K, class V>
bool ArbreMap<K,V>::Iterateur::operator!() const{
    return !iter;
}

template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::rechercher(const K& e) const{
    return entrees.rechercher(Entree(e));
}

#endif

