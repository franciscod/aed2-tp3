#ifndef COLA_H_
#define COLA_H_

#include "aed2/Lista.h"

using namespace aed2;

template <typename T>
class Cola{
    public:
        Cola();

        bool EsVacia() const;
        Nat Tamanho() const;

        T& Proximo();
        const T& Proximo() const;

        void Desencolar();
        void Encolar(const T&);

        bool operator == (const Cola<T>&) const;
    private:
        Lista<T> listaCola;
};

template <typename T>
Cola<T>::Cola(){
    listaCola = Lista<T>();
}

template <typename T>
bool Cola<T>::EsVacia() const{
    return listaCola.EsVacia();
}

template <typename T>
Nat Cola<T>::Tamanho() const{
    return listaCola.Longitud();
}

template <typename T>
T& Cola<T>::Proximo(){
    #ifdef DEBUG
        assert(!EsVacia());
    #endif

    return listaCola.Primero();
}

template <typename T>
const T& Cola<T>::Proximo() const{
    #ifdef DEBUG
        assert(!EsVacia());
    #endif

    return listaCola.Primero();
}

template <typename T>
void Cola<T>::Encolar(const T& e){
    listaCola.AgregarAtras(e);
}

template <typename T>
void Cola<T>::Desencolar(){
    #ifdef DEBUG
        assert(!EsVacia());
    #endif

    listaCola.Fin();
}

template <typename T>
bool Cola<T>::operator == (const Cola<T>& p) const{
    return listaCola == p.listaCola;
}

#endif // COLA_H_INCLUDED
