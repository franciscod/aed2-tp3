#ifndef DICC_LOG_H
#define DICC_LOG_H

#include "aed2/TiposBasicos.h"

#include "Pila.h"
#include "ArbolBinario.h"

template <typename T>
class DiccLog{
	public:
		DiccLog();

		DiccLog(const DiccLog&);

		~DiccLog();

		bool EsVacio() const;
		bool Definido(const Nat&) const;

		void Definir(const Nat&, const T&);
		void Borrar(const Nat&);

		T& Obtener(const Nat&);
		T& Minimo();

		const T& Obtener(const Nat&) const;
		const T& Minimo() const;

		DiccLog<T>& operator=(const DiccLog<T>& otro);

		bool operator == (const DiccLog<T>&) const;
	private:
		struct NodoAvl{
			NodoAvl(const Nat& c, const T& d) : clave(c), data(T(d)), balance(0) {};
			const Nat clave;
			T data;
			int balance;

			bool operator == (const NodoAvl& otro) const{
				return clave == otro.clave && data == otro.data;
			}
		};

		ArbolBinario<NodoAvl>* ptrAbAvl;
		ArbolBinario<NodoAvl>* ptrAbMin;

		ArbolBinario<NodoAvl>* crearArbol(const Nat&, const T&);

		ArbolBinario<NodoAvl>*& subArbol(ArbolBinario<NodoAvl>*&, bool);
		ArbolBinario<NodoAvl>*& subArbol(ArbolBinario<NodoAvl>*&, bool) const;

		ArbolBinario<NodoAvl>*& balancearArbol(ArbolBinario<NodoAvl>*&, bool);
		void ajustarBalance(ArbolBinario<NodoAvl>*&, bool, int);
		ArbolBinario<NodoAvl>*& removerBalanceo(ArbolBinario<NodoAvl>*&, bool, bool&);

		ArbolBinario<NodoAvl>* rotacionSimple(ArbolBinario<NodoAvl>*&, bool);
		ArbolBinario<NodoAvl>* rotacionDoble(ArbolBinario<NodoAvl>*&, bool);
};

template <typename T>
DiccLog<T>::DiccLog(){
	ptrAbAvl = new ArbolBinario<NodoAvl>();
	ptrAbMin = NULL;
}

template <typename T>
DiccLog<T>::DiccLog(const DiccLog<T>& otro){
	ptrAbAvl = new ArbolBinario<NodoAvl>();
	ptrAbMin = NULL;
	*this = otro;
}

template <typename T>
DiccLog<T>& DiccLog<T>::operator = (const DiccLog<T>& otro){
	if(this == &otro){
		return *this;
	}

	delete ptrAbAvl;
	ptrAbAvl = new ArbolBinario<NodoAvl>();
	ptrAbMin = NULL;

	if(!otro.EsVacio()){
		*ptrAbAvl = *otro.ptrAbAvl;
		ptrAbMin = ptrAbAvl;
		while(!ptrAbMin->Izq().EsNil()){
			ptrAbMin = ptrAbMin->IzqRapido();
		}
	}

	return *this;
}

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
		while(!(ptrAbIt->Raiz().clave == clave || subArbol(ptrAbIt, pilaUpDir.Tope())->EsNil())){

			ptrAbIt = subArbol(ptrAbIt, pilaUpDir.Tope());

			pilaUpDir.Apilar(ptrAbIt->Raiz().clave < clave);
			pilaUpPtrAb.Apilar(ptrAbIt);
		}

		if(ptrAbIt->Raiz().clave == clave){
			ptrAbIt->Raiz().data = data;
		}
		else{
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
}

template <typename T>
void DiccLog<T>::Borrar(const Nat& clave){
	#ifdef DEBUG
		assert(Definido(clave));
	#endif
	ArbolBinario<NodoAvl>* ptrAbIt = ptrAbAvl;
	ArbolBinario<NodoAvl>* ptrAbPadre = NULL;
	bool dirPadre = false;
	Pila<ArbolBinario<NodoAvl>*> pilaUpPtrAb;
	Pila<bool> pilaUpDir;

	// Busco el elemento a borrar
	while(ptrAbIt->Raiz().clave != clave){
		pilaUpDir.Apilar(ptrAbIt->Raiz().clave < clave);
		pilaUpPtrAb.Apilar(ptrAbIt);

		ptrAbPadre = ptrAbIt;
		dirPadre = pilaUpDir.Tope();

		ptrAbIt = subArbol(ptrAbIt, pilaUpDir.Tope());
	}

	// En caso de estar borrando el mínimo, actualizarlo
	if(ptrAbIt->Raiz().clave == ptrAbMin->Raiz().clave){
		if(!ptrAbMin->Der().EsNil()){
			ptrAbMin = ptrAbMin->DerRapido();
			while(!ptrAbMin->Izq().EsNil()){
				ptrAbMin = ptrAbMin->IzqRapido();
			}
		}
		else{
			ptrAbMin = ptrAbPadre == NULL ? NULL : ptrAbPadre;
		}
	}

	if(ptrAbIt->Izq().EsNil() || ptrAbIt->Der().EsNil()){
		// Si el nodo a borrar tenía a lo sumo un elemento, lo único que voy a
		// tener que hacer es que su hijo se conecte al padre del nodo
		bool dir = ptrAbIt->Izq().EsNil();
		ArbolBinario<NodoAvl>* ptrAbHijo = subArbol(ptrAbIt, dir);

		if(ptrAbPadre != NULL){
			// No estoy borrando la raiz
			subArbol(pilaUpPtrAb.Tope(), pilaUpDir.Tope()) = ptrAbHijo;
		}
		else{
			// Estoy borrando la raiz (no tiene padre)
			ptrAbAvl = ptrAbHijo;
		}

		subArbol(ptrAbIt, dir) = new ArbolBinario<NodoAvl>();
	}
	else{
		// Tengo que apilar el nodo que se va a swappear para rebalancear
		pilaUpDir.Apilar(true);
		pilaUpPtrAb.Apilar(ptrAbIt);

		// Me guardo la referencia
		ArbolBinario<NodoAvl>*& ptrAbApilado = pilaUpPtrAb.Tope();

		// Si el nodo tiene ambos hijos, debo buscar un sucesor que lo reemplace
		ArbolBinario<NodoAvl>* ptrAbHeredero = ptrAbIt->DerRapido();
		if(ptrAbHeredero->Izq().EsNil()){
			// Sucesor directo
			delete ptrAbHeredero->IzqRapido();
			if(ptrAbPadre != NULL){
				// No estoy borrando la raiz
				subArbol(ptrAbPadre, dirPadre) = ptrAbHeredero;
				ptrAbHeredero->IzqRapido() = ptrAbIt->IzqRapido();
			}
			else{
				// Estoy borrando la raiz (no tiene padre)
				ptrAbHeredero->IzqRapido() = ptrAbAvl->IzqRapido();
				ptrAbAvl = ptrAbHeredero;
			}
		}
		else{
			// Sucesor indirecto (tengo que buscar el mínimo)

			while(!ptrAbHeredero->Izq().EsNil()){
				pilaUpDir.Apilar(false);
				pilaUpPtrAb.Apilar(ptrAbHeredero);
				ptrAbHeredero = ptrAbHeredero->IzqRapido();
			}

			delete ptrAbHeredero->IzqRapido();
			if(ptrAbPadre != NULL){
				// No estoy borrando la raiz
				subArbol(ptrAbPadre, dirPadre) = ptrAbHeredero;
				subArbol(pilaUpPtrAb.Tope(), pilaUpDir.Tope()) = ptrAbHeredero->DerRapido();
				ptrAbHeredero->IzqRapido() = ptrAbIt->IzqRapido();
				ptrAbHeredero->DerRapido() = ptrAbIt->DerRapido();
			}
			else{
				// Estoy borrando la raiz (no tiene padre)
				subArbol(pilaUpPtrAb.Tope(), pilaUpDir.Tope()) = ptrAbHeredero->DerRapido();
				ptrAbHeredero->IzqRapido() = ptrAbIt->IzqRapido();
				ptrAbHeredero->DerRapido() = ptrAbIt->DerRapido();
				ptrAbAvl = ptrAbHeredero;
			}
		}
		// El puntero apilado pasa a apuntar al heredero
		ptrAbApilado = ptrAbHeredero;
		// El heredero pasa a tener el mismo balance que el nodo a borrar
		ptrAbHeredero->Raiz().balance = ptrAbIt->Raiz().balance;

		ptrAbIt->DerRapido() = new ArbolBinario<NodoAvl>();
		ptrAbIt->IzqRapido() = new ArbolBinario<NodoAvl>();
	}
	delete ptrAbIt;

	bool done = false;
	while(!(pilaUpPtrAb.EsVacia() || done)){
		// Actualizo el balance
		pilaUpPtrAb.Tope()->Raiz().balance += pilaUpDir.Tope() ? -1 : 1;

		if(pilaUpPtrAb.Tope()->Raiz().balance == 0){
			pilaUpPtrAb.Desapilar(); pilaUpDir.Desapilar();
		}
		else{
			if(pilaUpPtrAb.Tope()->Raiz().balance % 2 == 1){
				done = true;
			}
			else{
				pilaUpPtrAb.Tope() = removerBalanceo(pilaUpPtrAb.Tope(), pilaUpDir.Tope(), done);
				if(pilaUpPtrAb.Tamanho() > 1){
					ArbolBinario<NodoAvl>* ptrAbTope = pilaUpPtrAb.Tope();
					pilaUpPtrAb.Desapilar();
					pilaUpDir.Desapilar();
					subArbol(pilaUpPtrAb.Tope(), pilaUpDir.Tope()) = ptrAbTope;
				}
			else{
					ptrAbAvl = pilaUpPtrAb.Tope();
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
ArbolBinario<typename DiccLog<T>::NodoAvl>*& DiccLog<T>::removerBalanceo(ArbolBinario<typename DiccLog<T>::NodoAvl>*& ptrAb, bool dir, bool& done){
	ArbolBinario<NodoAvl>* ptrAbHijo = subArbol(ptrAb, !dir);

	int bal = dir ? 1 : -1;

	if(ptrAbHijo->Raiz().balance == -bal){
		ptrAb->Raiz().balance = 0;
		ptrAbHijo->Raiz().balance = 0;
		ptrAb = rotacionSimple(ptrAb, dir);
	}
	else{
		if(ptrAbHijo->Raiz().balance == bal){
			ajustarBalance(ptrAb, !dir, -bal);
			ptrAb = rotacionDoble(ptrAb, dir);
		}
		else{
			ptrAb->Raiz().balance = -bal;
			ptrAbHijo->Raiz().balance = bal;
			ptrAb = rotacionSimple(ptrAb, dir);
			done = true;
		}
	}

	return ptrAb;
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

template <typename T>
bool DiccLog<T>::operator == (const DiccLog<T>& otro) const{
	bool sonIguales = false;
	sonIguales = (EsVacio() == otro.EsVacio());

	if(sonIguales && !EsVacio()){
		sonIguales = ptrAbAvl->Inorder() == otro.ptrAbAvl->Inorder();
	}

	return sonIguales;
}
#endif
