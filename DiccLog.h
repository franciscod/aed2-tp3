#ifndef DEBUG
#define DEBUG
#endif

#ifndef DICC_LOG_H
#define DICC_LOG_H

#include "aed2/TiposBasicos.h"

#include "Pila.h"
#include "ArbolBinario.h"

template <typename T>
class DiccLog{
	public:
		DiccLog();

		~DiccLog();

		bool EsVacio() const;
		bool Definido(const Nat&) const;

		void Definir(const Nat&, const T&);

		T& Obtener(const Nat&);
		T& Minimo();

		const T& Obtener(const Nat&) const;
		const T& Minimo() const;
/*
		DiccLog(const DiccLog&);



		void Borrar(const int&);


		const T& Minimo() const;
		const T& Obtener(const int&) const;

		DiccLog<T>& operator=(const DiccLog<T>& otro);
*/
	private:
		struct NodoAvl{
			NodoAvl(const Nat& c, const T& d) : clave(c), data(T(d)), balance(0) {};
			Nat clave;
			T data;
			int balance;
		};

		ArbolBinario<NodoAvl>* abAvlPtr;
		ArbolBinario<NodoAvl>* abMinPtr;

		ArbolBinario<NodoAvl>* crearArbol(const Nat&, const T&);

		ArbolBinario<NodoAvl>*& subArbol(ArbolBinario<NodoAvl>*&, bool);
		ArbolBinario<NodoAvl>*& subArbol(ArbolBinario<NodoAvl>*&, bool) const;
};

template <typename T>
DiccLog<T>::DiccLog(){
	abAvlPtr = new ArbolBinario<NodoAvl>();
	abMinPtr = NULL;
}

//template <typename T>
//DiccLog<T>::DiccLog(const DiccLog<T>& otro){
//	*this = otro;
//}

template <typename T>
DiccLog<T>::~DiccLog(){
	delete abAvlPtr;
}

template <typename T>
bool DiccLog<T>::EsVacio() const{
	return abAvlPtr->EsNil();
}

template <typename T>
bool DiccLog<T>::Definido(const Nat& clave) const{
	bool definido = false;
	ArbolBinario<NodoAvl>* abIt = abAvlPtr;

	while(!(abIt->EsNil() || definido)){
		definido = (abIt->Raiz().clave == clave);
		abIt = subArbol(abIt, (abIt->Raiz().clave < clave));
	}

	return definido;
}

template <typename T>
void DiccLog<T>::Definir(const Nat& clave, const T& data){
	if(abAvlPtr->EsNil()){
		delete abAvlPtr;
		abAvlPtr = crearArbol(clave, data);
		abMinPtr = abAvlPtr;
	}
	else{
		ArbolBinario<NodoAvl>* abIt = abAvlPtr;
		Pila<ArbolBinario<NodoAvl>*> pilaUpAb;
		Pila<bool> pilaUpDir;

		pilaUpDir.Apilar(abIt->Raiz().clave < clave);
		pilaUpAb.Apilar(abIt);

		// Busco dónde insertar el nuevo elemento
		while(!subArbol(abIt, pilaUpDir.Tope())->EsNil()){
			abIt = subArbol(abIt, pilaUpDir.Tope());

			pilaUpDir.Apilar(abIt->Raiz().clave < clave);
			pilaUpAb.Apilar(abIt);
		}
		// Creo el ab para el nuevo elemento
		delete subArbol(abIt, pilaUpDir.Tope());
		subArbol(abIt, pilaUpDir.Tope()) = crearArbol(clave, data);

		// Si la clave del nuevo elemento es menor al actual, lo actualizo
		if(clave < abMinPtr->Raiz().clave){
			abMinPtr = subArbol(abIt, pilaUpDir.Tope());
		}

		bool done = false;
		while(!(pilaUpAb.EsVacia() || done)){
			// Actualizo el balance
			pilaUpAb.Tope()->Raiz().balance += pilaUpDir.Tope() ? 1 : -1;

			if(pilaUpAb.Tope()->Raiz().balance == 0){
				done = true;
			}
			else{
				if(pilaUpAb.Tope()->Raiz().balance % 2 == 1){
					// TODO: rotaciones
					//pilaUpAb.Tope() = balancearArbol(pilaUpAb.Tope(), pilaUpDir.Tope());

					if(pilaUpAb.Tamanho() > 1){
						ArbolBinario<NodoAvl>* abTope = pilaUpAb.Tope();
						pilaUpAb.Desapilar();
						pilaUpDir.Desapilar();
						subArbol(pilaUpAb.Tope(), pilaUpDir.Tope()) = abTope;
					}
					else{
						abAvlPtr = pilaUpAb.Tope();
					}

					done = true;
				}
				else{
					pilaUpAb.Desapilar();
					pilaUpDir.Desapilar();
				}
			}
		}
	}
}

template <typename T>
T& DiccLog<T>::Obtener(const Nat& clave){
	#ifdef DEBUG
		assert(Definido(clave));
	#endif
	ArbolBinario<NodoAvl>* abIt = abAvlPtr;

	while(abIt->Raiz().clave != clave){
		abIt = subArbol(abIt, (abIt->Raiz().clave < clave));
	}

	return abIt->Raiz().data;
}

template <typename T>
const T& DiccLog<T>::Obtener(const Nat& clave) const{
	#ifdef DEBUG
		assert(Definido(clave));
	#endif
	ArbolBinario<NodoAvl>* abIt = abAvlPtr;

	while(abIt->Raiz().clave != clave){
		abIt = subArbol(abIt, (abIt->Raiz().clave < clave));
	}

	return abIt->Raiz().data;
}

template <typename T>
T& DiccLog<T>::Minimo(){
	#ifdef DEBUG
		assert(!EsVacio());
	#endif
	return abMinPtr->Raiz().data;
}

template <typename T>
const T& DiccLog<T>::Minimo() const{
	#ifdef DEBUG
		assert(!EsVacio());
	#endif
	return abMinPtr->Raiz().data;
}

template <typename T>
ArbolBinario<typename DiccLog<T>::NodoAvl>* DiccLog<T>::crearArbol(const Nat& c, const T& d){
	return new ArbolBinario<NodoAvl>(ArbolBinario<NodoAvl>(), NodoAvl(c,d), ArbolBinario<NodoAvl>());
}

template <typename T>
ArbolBinario<typename DiccLog<T>::NodoAvl>*& DiccLog<T>::subArbol(ArbolBinario<typename DiccLog<T>::NodoAvl>*& abPtr, bool dir){
	if(dir){
		return abPtr->DerRapido();
	}
	else{
		return abPtr->IzqRapido();
	}
}

template <typename T>
ArbolBinario<typename DiccLog<T>::NodoAvl>*& DiccLog<T>::subArbol(ArbolBinario<typename DiccLog<T>::NodoAvl>*& abPtr, bool dir) const{
	if(dir){
		return abPtr->DerRapido();
	}
	else{
		return abPtr->IzqRapido();
	}
}

#endif
