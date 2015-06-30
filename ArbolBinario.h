#ifndef DEBUG
#define DEBUG
#endif

#ifndef ARBOL_BINARIO_H_
#define ARBOL_BINARIO_H_

#include <ostream>
#include <iostream>
#include "Pila.h"

using namespace aed2;
using namespace std;

template <typename T>
class ArbolBinario{
	public:
		ArbolBinario();

		~ArbolBinario();

		ArbolBinario(const ArbolBinario&);

		ArbolBinario(const ArbolBinario&, const T&, const ArbolBinario&);

		bool EsNil() const;

		ArbolBinario<T>& Izq();
		ArbolBinario<T>& Der();
		T& Raiz();

		const ArbolBinario<T>& Izq() const;
		const ArbolBinario<T>& Der() const;
		const T& Raiz() const;

		ArbolBinario<T>& operator = (const ArbolBinario<T>&);

		bool operator == (const ArbolBinario<T>&) const;
	private:
		struct NodoAb
		{
			NodoAb(const ArbolBinario<T>& i, const T& r, const ArbolBinario<T>& d) : raiz(T(r)){
				hijos[0] = ArbolBinario<T>(i);
				hijos[1] = ArbolBinario<T>(d);
			};
			T raiz;
			ArbolBinario hijos[2];
		};

		NodoAb* nodo;
};


template <typename T>
ArbolBinario<T>::ArbolBinario(){
	nodo = NULL;
}

template <typename T>
ArbolBinario<T>::~ArbolBinario()
{
	NodoAb* nodoIterador = nodo;
	Pila<NodoAb*> pilaIt;
	Pila<int> pilaDir;
	bool done = false;

	while(!done){
		if(nodoIterador != NULL){
			pilaIt.Apilar(nodoIterador);
			pilaDir.Apilar(0);
			nodoIterador = nodoIterador->hijos[0].nodo;
		}
		else{
			if(!pilaIt.EsVacia()){
				if(pilaIt.Tope()->hijos[1].nodo == NULL){
					delete pilaIt.Tope();
					pilaIt.Desapilar();
					pilaDir.Desapilar();
					if(!pilaIt.EsVacia()){
						pilaIt.Tope()->hijos[pilaDir.Tope()].nodo = NULL;
					}
				}
				else{
					pilaDir.Tope() = 1;
					nodoIterador = pilaIt.Tope()->hijos[1].nodo;
				}
			}
			else{
				done = true;
			}
		}
	}
}

template <typename T>
ArbolBinario<T>::ArbolBinario(const ArbolBinario<T>& otro){
	*this = otro;
}

template <typename T>
ArbolBinario<T>& ArbolBinario<T>::operator = (const ArbolBinario<T>& otro){
	if(this == &otro){
		return *this;
	}

	nodo = NULL;

	if(!otro.EsNil()){
		nodo = new NodoAb(otro.Izq(), otro.Raiz(), otro.Der());
	}

	return *this;
}

template <typename T>
ArbolBinario<T>::ArbolBinario(const ArbolBinario<T>& izq, const T& raiz, const ArbolBinario<T>& der){
	nodo = new NodoAb(izq, raiz, der);
}

template <typename T>
bool ArbolBinario<T>::EsNil() const{
	return nodo == NULL;
}

template <typename T>
ArbolBinario<T>& ArbolBinario<T>::Izq(){
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return nodo->hijos[0];
}

template <typename T>
const ArbolBinario<T>& ArbolBinario<T>::Izq() const{
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return nodo->hijos[0];
}

template <typename T>
ArbolBinario<T>& ArbolBinario<T>::Der(){
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return nodo->hijos[1];
}

template <typename T>
const ArbolBinario<T>& ArbolBinario<T>::Der() const{
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return nodo->hijos[1];
}

template <typename T>
T& ArbolBinario<T>::Raiz(){
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return nodo->raiz;
}

template <typename T>
const T& ArbolBinario<T>::Raiz() const{
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return nodo->raiz;
}

template <typename T>
bool ArbolBinario<T>::operator == (const ArbolBinario<T>& otro) const{
	bool sonIguales = false;
	sonIguales = (EsNil() == otro.EsNil());

	if(sonIguales && !EsNil()){
		sonIguales = (Raiz() == otro.Raiz()) && (Der() == otro.Der()) && (Izq() == otro.Izq());
	}

	return sonIguales;
}
#endif // ARBOL_BINARIO_H_INCLUDED
