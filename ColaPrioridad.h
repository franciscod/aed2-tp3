#ifndef COLA_PRIORIDAD_H_
#define COLA_PRIORIDAD_H_

#include "Cola.h"
#include "aed2/Dicc.h"
#include <iostream>

using namespace aed2;
using namespace std;

template <typename T>
class ColaPrioridad{


    public:

        ColaPrioridad();
        // ~ColaPrioridad();

        bool esVacia() const;
        
        T& proximo();
        
        T desencolar();
        void encolar(const Nat, const T);

        bool operator == (const ColaPrioridad<T>&) const;

    private:
        
        struct nodosEncolados{
            Cola<T> encolados;
            Nat prioridad;
        };

        Dicc<Nat,nodosEncolados> diccCola; 

};

template <typename T>
ColaPrioridad<T>::ColaPrioridad(){}

template <typename T>
bool ColaPrioridad<T>::esVacia() const{
    return(diccCola.CantClaves() == 0);
}

template <typename T>
T& ColaPrioridad<T>::proximo(){
    #ifdef DEBUG
        assert(!esVacia());
    #endif

    Dicc<Nat, nodosEncolados>::Iterador it = diccCola.CrearIt();
    Nat maxPrioridad = 0;
    while(it.HaySiguiente()){
        cout << "clave: " << it.SiguienteClave() << endl;
        if(it.SiguienteClave() > maxPrioridad)
            maxPrioridad = it.SiguienteClave();
        it.Avanzar();
    }

    return(diccCola.Significado(maxPrioridad).proximo())
}

template <typename T>
T ColaPrioridad<T>::encolar(const Nat p, const T elem){
    if(diccCola.Definido(p)){
        diccCola.Significado(p).encolados.encolar(elem);
    }
    else{
        nodosEncolados nuevoNodo;
        nuevoNodo.encolados = Cola();
        nuevoNodo.prioridad = p;
        nuevoNodo.encolados.encolar(elem);
        diccCola.Definir(p, nuevoNodo);
    }
}



#endif // COLA_PRIORIDAD_H_INCLUDED