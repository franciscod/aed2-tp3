#ifndef PILA_H_
#define PILA_H_

#include <ostream>
#include "aed2/Lista.h"

using namespace aed2;
using namespace std;

template <typename T>
class Pila{
	public:
		Pila();

		bool EsVacia() const;
		Nat Tamanho() const;

		T& Tope();

		void Desapilar();
		void Apilar(const T&);

		bool operator == (const Pila<T>&) const;
	private:
		Lista<T> listaPila;
};

template <typename T>
Pila<T>::Pila(){
	listaPila = Lista<T>();
}

template <typename T>
bool Pila<T>::EsVacia() const{
	return listaPila.EsVacia();
}

template <typename T>
Nat Pila<T>::Tamanho() const{
	return listaPila.Longitud();
}

template <typename T>
T& Pila<T>::Tope(){
	#ifdef DEBUG
		assert(!EsVacia());
	#endif

	return listaPila.Primero();
}

template <typename T>
void Pila<T>::Apilar(const T& e){
	listaPila.AgregarAdelante(e);
}

template <typename T>
void Pila<T>::Desapilar(){
	#ifdef DEBUG
		assert(!EsVacia());
	#endif

	listaPila.Fin();
}

template <typename T>
bool Pila<T>::operator == (const Pila<T>& p) const{
	return listaPila == p.listaPila;
}

#endif // PILA_H_INCLUDED
