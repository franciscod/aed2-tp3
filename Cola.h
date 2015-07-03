#ifndef COLA_H_
#define COLA_H_

#include <ostream>
#include "aed2/Lista.h"

using namespace aed2;
using namespace std;

template <typename T>
class Cola{
    public:
        Cola();

        bool esVacia() const;
        Nat tamanho() const;

        T& proximo();

        void desencolar();
        void encolar(const T&);

        bool operator == (const Cola<T>&) const;
    private:
        Lista<T> listaCola;
};

template <typename T>
Cola<T>::Cola(){
    listaCola = Lista<T>();
}

template <typename T>
bool Cola<T>::esVacia() const{
    return listaCola.EsVacia();
}

template <typename T>
Nat Cola<T>::tamanho() const{
    return listaCola.Longitud();
}

template <typename T>
T& Cola<T>::proximo(){
    #ifdef DEBUG
        assert(!esVacia());
    #endif

    return listaCola.Primero();
}

template <typename T>
void Cola<T>::encolar(const T& e){
    listaCola.AgregarAtras(e);
}

template <typename T>
void Cola<T>::desencolar(){
    #ifdef DEBUG
        assert(!esVacia());
    #endif

    listaCola.Fin();
}

template <typename T>
bool Cola<T>::operator == (const Cola<T>& p) const{
    return listaCola == p.listaCola;
}

#endif // COLA_H_INCLUDED