/* UQAM / Département d'informatique
   INF3105 - Structures de données et algorithmes
   Squelette pour classe générique ArbreMap<K,V> pour le Lab8 et le TP2.

   AUTEUR(S):
    1) Nom + Code permanent du l'étudiant.e 1
    2) Nom + Code permanent du l'étudiant.e 2
*/

#if !defined(__ARBREMAP_H__)
#define __ARBREMAP_H__

#include "arbreavl.h"

template <class K, class V>
class ArbreMap
{
  private:
    class Entree{
        public:
            Entree(const K& c): cle(c).valeur(){}
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

    }
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
            typename ArbreAVL<Entree>Iterateur iter;
    };        

    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const K& cle) const;
    Iterateur rechercherEgalOuSuivant(const K& cle) const;
    Iterateur rechercherEgslOuPrecedent(const K& cle) const;
    const V& operator[] (const Iterateur&) const;
    V& operator[] (const Iterateur&);
            
    };
    // À compléter : voir la Section 8.11 (Arbres associatifs (Mapping Trees)) des notes de cours.


    ArbreAVL<Entree> entrees;

  public:
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
bool ArbreMap<K,V>::contient(const K& c) const
{
    // À compléter
    return false;
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

// faire opeérateur > =

//---------------------------------------------------- Iterateur






template <class K, class V>
typename ArbreMap<K,V>::Iterateur::ArbreMap<K,V>::debut() const{
    return Iterateur(*this)
}

template <class K, class V>
typename ArbreMap<K,V>::Iterateur::ArbreMap<K,V>::fin() const{
    typename ArbreAVL<Entree>::Iterateur iter(entrees);
    return Iterateur(iter);
}


template <class K, class V>
typename ArbreMap<K,V>::Iterateur::ArbreMap<K,V>::rechercher(const K& cle) const{
    return entrees.rechercher(Entree(e));
}

template <class K, class V>
typename ArbreMap<K,V>::Iterateur::ArbreMap<K,V>::rechercherEgalOuSuivant(const K& cle) const{
    return entrees.rechercherEgalOuSuivant(Entree(e));
}
template <class K, class V>
typename ArbreMap<K,V>::Iterateur::ArbreMap<K,V>::rechercherEgslOuPrecedent(const K& cle) const{
    return entrees.rechercherEgslOuPrecedent(Entree(e));
}



//------------------
//Iterateur


template <class K, class V>
const K& ArbreMap<K,V>::Iterateur::operator bool() const{
    return iter;
}


template <class K, class V>
bool ArbreMap<K,V>::Iterateur::operator!() const{

    return !iter;
}


//pre-increment
template <class K, class V>
typename ArbreMap<K,V>::Iterateur& ArbreMap<K,V>::Iterateur::operator ++(){
    if(!*this) return *this;
    iter++;
    return *this;
    
}

//post-increment
template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::Iterateur::operator ++(int){
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class K, class V>
const K& ArbreMap<K,V>::Iterateur::cle() const{
    return (*iter).cle;
}
template <class K, class V>
const V& ArbreMap<K,V>::Iterateur::valeur() const{
    return (*iter).valeur;
}


#endif

