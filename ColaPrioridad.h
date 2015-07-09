#ifndef COLA_PRIORIDAD_H_
#define COLA_PRIORIDAD_H_

#include "Cola.h"
#include "DiccLog.h"

using namespace aed2;
using namespace std;

template <typename T>
class ColaPrioridad{


    public:

        ColaPrioridad();

        bool EsVacia() const;
        
        T& Proximo();
        
        T Desencolar();
        void Encolar(const Nat, const T);

        bool operator == (const ColaPrioridad<T>&) const;

    private:
        
        struct NodosEncolados{
            Cola<T> encolados;
            Nat prioridad;

            bool operator == (const NodosEncolados& otro) const{
                return((encolados == otro.encolados) && (prioridad == otro.prioridad));
            }
            
        };

        DiccLog<NodosEncolados> diccCola; 

};

template <typename T>
ColaPrioridad<T>::ColaPrioridad(){}

template <typename T>
bool ColaPrioridad<T>::EsVacia() const{
    return(diccCola.EsVacio());
}

template <typename T>
T& ColaPrioridad<T>::Proximo(){
    #ifdef DEBUG
        assert(!EsVacia());
    #endif

    return(diccCola.Minimo().encolados.Proximo());
}

template <typename T>
void ColaPrioridad<T>::Encolar(const Nat p, const T elem){
    if(diccCola.Definido(p)){
        diccCola.Obtener(p).encolados.Encolar(elem);
    }
    else{
        NodosEncolados nuevoNodo;
        nuevoNodo.encolados = Cola<T>();
        nuevoNodo.prioridad = p;
        nuevoNodo.encolados.Encolar(elem);
        diccCola.Definir(p, nuevoNodo);
    }
}

template <typename T>
T ColaPrioridad<T>::Desencolar(){

    NodosEncolados& minimo = diccCola.Minimo();

    T res = minimo.encolados.Proximo();

    minimo.encolados.Desencolar();
    if(minimo.encolados.EsVacia()){
        diccCola.Borrar(minimo.prioridad);
    }

    return(res);
}

template <typename T>
bool ColaPrioridad<T>::operator == (const ColaPrioridad& otra) const{
    return(diccCola == otra.diccCola);
}

#endif // COLA_PRIORIDAD_H_INCLUDED
