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

		DiccLog<T>& operator=(const DiccLog<T>& otro);
*/
	private:
		struct NodoAvl{
			NodoAvl(const Nat& c, const T& d) : clave(c), data(T(d)), balance(0) {};
			Nat clave;
			T data;
			int balance;
		};

		ArbolBinario<NodoAvl>* ptrAbAvl;
		ArbolBinario<NodoAvl>* ptrAbMin;

		ArbolBinario<NodoAvl>* crearArbol(const Nat&, const T&);

		ArbolBinario<NodoAvl>*& subArbol(ArbolBinario<NodoAvl>*&, bool);
		ArbolBinario<NodoAvl>*& subArbol(ArbolBinario<NodoAvl>*&, bool) const;

		ArbolBinario<NodoAvl>*& balancearArbol(ArbolBinario<NodoAvl>*&, bool);
		void ajustarBalance(ArbolBinario<NodoAvl>*&, bool, int);

		ArbolBinario<NodoAvl>* rotacionSimple(ArbolBinario<NodoAvl>*&, bool);
		ArbolBinario<NodoAvl>* rotacionDoble(ArbolBinario<NodoAvl>*&, bool);
};

template <typename T>
DiccLog<T>::DiccLog(){
	ptrAbAvl = new ArbolBinario<NodoAvl>();
	ptrAbMin = NULL;
}

//template <typename T>
//DiccLog<T>::DiccLog(const DiccLog<T>& otro){
//	*this = otro;
//}

template <typename T>
DiccLog<T>::~DiccLog(){
	delete ptrAbAvl;
}

template <typename T>
bool DiccLog<T>::EsVacio() const{
	return ptrAbAvl->EsNil();
}

template <typename T>
bool DiccLog<T>::Definido(const Nat& clave) const{
	bool definido = false;
	ArbolBinario<NodoAvl>* ptrAbIt = ptrAbAvl;

	while(!(ptrAbIt->EsNil() || definido)){
		definido = (ptrAbIt->Raiz().clave == clave);
		ptrAbIt = subArbol(ptrAbIt, (ptrAbIt->Raiz().clave < clave));
	}

	return definido;
}

template <typename T>
void DiccLog<T>::Definir(const Nat& clave, const T& data){
	if(ptrAbAvl->EsNil()){
		delete ptrAbAvl;
		ptrAbAvl = crearArbol(clave, data);
		ptrAbMin = ptrAbAvl;
	}
	else{
		ArbolBinario<NodoAvl>* ptrAbIt = ptrAbAvl;
		Pila<ArbolBinario<NodoAvl>*> pilaUpPtrAb;
		Pila<bool> pilaUpDir;

		pilaUpDir.Apilar(ptrAbIt->Raiz().clave < clave);
		pilaUpPtrAb.Apilar(ptrAbIt);

		// Busco dónde insertar el nuevo elemento
		while(!subArbol(ptrAbIt, pilaUpDir.Tope())->EsNil()){
			ptrAbIt = subArbol(ptrAbIt, pilaUpDir.Tope());

			pilaUpDir.Apilar(ptrAbIt->Raiz().clave < clave);
			pilaUpPtrAb.Apilar(ptrAbIt);
		}
		// Creo el ab para el nuevo elemento
		delete subArbol(ptrAbIt, pilaUpDir.Tope());
		subArbol(ptrAbIt, pilaUpDir.Tope()) = crearArbol(clave, data);

		// Si la clave del nuevo elemento es menor al actual, lo actualizo
		if(clave < ptrAbMin->Raiz().clave){
			ptrAbMin = subArbol(ptrAbIt, pilaUpDir.Tope());
		}

		bool done = false;
		while(!(done || pilaUpPtrAb.EsVacia())){
			// Actualizo el balance
			pilaUpPtrAb.Tope()->Raiz().balance += pilaUpDir.Tope() ? 1 : -1;

			if(pilaUpPtrAb.Tope()->Raiz().balance == 0){
				// Si estoy completamente balanceado terminé
				done = true;
			}
			else{
				if(pilaUpPtrAb.Tope()->Raiz().balance % 2 == 0){
					// Si estoy desbalanceado hago la rotación necesaria
					pilaUpPtrAb.Tope() = balancearArbol(pilaUpPtrAb.Tope(), pilaUpDir.Tope());

					if(pilaUpPtrAb.Tamanho() > 1){
						// Si no soy la raiz, actualizo en mi padre quién es mi
						// nueva raíz luego de haber realizado las rotaciones
						ArbolBinario<NodoAvl>* abTope = pilaUpPtrAb.Tope();
						pilaUpPtrAb.Desapilar();
						pilaUpDir.Desapilar();
						subArbol(pilaUpPtrAb.Tope(), pilaUpDir.Tope()) = abTope;
					}
					else{
						// Si la soy, me asigno al padre del diccionario
						ptrAbAvl = pilaUpPtrAb.Tope();
					}

					// Como ahora estoy balanceado, termino
					done = true;
				}
				else{
					// Si estoy parcialmente balanceado sigo subiendo el árbol
					pilaUpPtrAb.Desapilar();
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
	ArbolBinario<NodoAvl>* ptrAbIt = ptrAbAvl;

	while(ptrAbIt->Raiz().clave != clave){
		ptrAbIt = subArbol(ptrAbIt, (ptrAbIt->Raiz().clave < clave));
	}

	return ptrAbIt->Raiz().data;
}

template <typename T>
const T& DiccLog<T>::Obtener(const Nat& clave) const{
	#ifdef DEBUG
		assert(Definido(clave));
	#endif
	ArbolBinario<NodoAvl>* ptrAbIt = ptrAbAvl;

	while(ptrAbIt->Raiz().clave != clave){
		ptrAbIt = subArbol(ptrAbIt, (ptrAbIt->Raiz().clave < clave));
	}

	return ptrAbIt->Raiz().data;
}

template <typename T>
T& DiccLog<T>::Minimo(){
	#ifdef DEBUG
		assert(!EsVacio());
	#endif
	return ptrAbMin->Raiz().data;
}

template <typename T>
const T& DiccLog<T>::Minimo() const{
	#ifdef DEBUG
		assert(!EsVacio());
	#endif
	return ptrAbMin->Raiz().data;
}

template <typename T>
ArbolBinario<typename DiccLog<T>::NodoAvl>* DiccLog<T>::crearArbol(const Nat& c, const T& d){
	return new ArbolBinario<NodoAvl>(ArbolBinario<NodoAvl>(), NodoAvl(c,d), ArbolBinario<NodoAvl>());
}

template <typename T>
ArbolBinario<typename DiccLog<T>::NodoAvl>*& DiccLog<T>::subArbol(ArbolBinario<typename DiccLog<T>::NodoAvl>*& ptrAb, bool dir){
	if(dir){
		return ptrAb->DerRapido();
	}
	else{
		return ptrAb->IzqRapido();
	}
}

template <typename T>
ArbolBinario<typename DiccLog<T>::NodoAvl>*& DiccLog<T>::subArbol(ArbolBinario<typename DiccLog<T>::NodoAvl>*& ptrAb, bool dir) const{
	if(dir){
		return ptrAb->DerRapido();
	}
	else{
		return ptrAb->IzqRapido();
	}
}

template <typename T>
ArbolBinario<typename DiccLog<T>::NodoAvl>*& DiccLog<T>::balancearArbol(ArbolBinario<typename DiccLog<T>::NodoAvl>*& ptrAb, bool dir){
	ArbolBinario<NodoAvl>* ptrAbHijo = subArbol(ptrAb, dir);

	int bal = dir ? 1 : -1;

	if(ptrAbHijo->Raiz().balance == bal){
		ptrAb->Raiz().balance = 0;
		ptrAbHijo->Raiz().balance = 0;
		ptrAb = rotacionSimple(ptrAb, !dir);
	}
	else{
		ajustarBalance(ptrAb, dir, bal);
		ptrAb = rotacionDoble(ptrAb, !dir);
	}

	return ptrAb;
}

template <typename T>
void DiccLog<T>::ajustarBalance(ArbolBinario<typename DiccLog<T>::NodoAvl>*& ptrAb, bool dir, int bal){
	ArbolBinario<NodoAvl>*& ptrAbHijo = subArbol(ptrAb, dir);
	ArbolBinario<NodoAvl>*& ptrAbNieto = subArbol(ptrAbHijo, !dir);

	if(ptrAbNieto->Raiz().balance == 0){
		ptrAb->Raiz().balance = 0;
		ptrAbHijo->Raiz().balance = 0;
	}
	else{
		if(ptrAbNieto->Raiz().balance == bal){
			ptrAb->Raiz().balance = -bal;
			ptrAbHijo->Raiz().balance = 0;
		}
		else{
			ptrAb->Raiz().balance = 0;
			ptrAbHijo->Raiz().balance = bal;
		}
	}

	ptrAbNieto->Raiz().balance = 0;
}

template <typename T>
ArbolBinario<typename DiccLog<T>::NodoAvl>* DiccLog<T>::rotacionSimple(ArbolBinario<typename DiccLog<T>::NodoAvl>*& ptrAb, bool dir){
	// Hijo que pasará a ser la raiz
	ArbolBinario<NodoAvl>* ptrAbHijo = subArbol(ptrAb, !dir);

	// Le asigno al padre el hijo de la nueva raiz
	subArbol(ptrAb, !dir) = subArbol(ptrAbHijo, dir);
	subArbol(ptrAbHijo, dir) = ptrAb;

	// Le asigno a la nueva raiz como hijo al padre
	return ptrAbHijo;
}

template <typename T>
ArbolBinario<typename DiccLog<T>::NodoAvl>* DiccLog<T>::rotacionDoble(ArbolBinario<typename DiccLog<T>::NodoAvl>*& ptrAb, bool dir){
	ArbolBinario<NodoAvl>* ptrAbNieto = subArbol(subArbol(ptrAb, !dir), dir);

	subArbol(subArbol(ptrAb, !dir), dir) = subArbol(ptrAbNieto, !dir);
	subArbol(ptrAbNieto, !dir) = subArbol(ptrAb, !dir);
	subArbol(ptrAb, !dir) = ptrAbNieto;
	ptrAbNieto = subArbol(ptrAb, !dir);
	subArbol(ptrAb, !dir) = subArbol(ptrAbNieto, dir);
	subArbol(ptrAbNieto, dir) = ptrAb;

	return ptrAbNieto;
}

#endif
