#include "Driver.h"
#include "mini_test.h"
#include "aed2/Lista.h"
#include "aed2/Conj.h"
#include "aed2/Dicc.h"
#include "aed2/TiposBasicos.h"
#include "aed2/dicc_trie.h"

#include <string>
#include <iostream>

// Modulos del grupo
#include "Pila.h"
#include "ArbolBinario.h"
#include "ColaPrioridad.h"
#include "DiccLog.h"

using namespace std;
using namespace aed2;

/**
 * Imprime un elemento a un string, en vez de a una pantalla,
 * a través del operador <<
 */
template <typename T>
std::string to_str(const T& t){
	std::stringstream ss;
	ss << t;

	return ss.str();
}

/**
 * Esta función se puede utilizar para comparar dos colecciones
 * iterables que representen conjuntos, es decir, que no tengan
 * elementos repetidos.
 */
template<typename T, typename S>
bool Comparar(const T& t, const S& s){
	typename T::const_Iterador it1 = t.CrearIt();
	typename S::const_Iterador it2 = s.CrearIt();

	// me fijo si tienen el mismo tamanho

	Nat len1 = 0;
	while( it1.HaySiguiente() ) {
		len1++;
		it1.Avanzar();
	}

	Nat len2 = 0;
	while( it2.HaySiguiente() ) {
		len2++;
		it2.Avanzar();
	}

	if ( len1 != len2 )
		return false;

	it1 = t.CrearIt();
	it2 = s.CrearIt();

	// me fijo que tengan los mismos elementos

	while( it1.HaySiguiente() ){
		bool esta = false;
		it2 = s.CrearIt();

		while( it2.HaySiguiente() ) {
			if ( it1.Siguiente() == it2.Siguiente() ) {
				esta = true;
				break;
			}
			it2.Avanzar();
		}

		if ( !esta ) {
			return false;
		}

		it1.Avanzar();
	}

	return true;
}

// Pila

void check_pila_vacia(){
	Pila<int> pila;

	ASSERT(pila.EsVacia());
}

void check_pila_apilar(){
	Pila<int> pila;

	pila.Apilar(3);

	ASSERT_EQ(pila.Tope(), 3);

	pila.Apilar(-3);

	ASSERT_EQ(pila.Tope(), -3);
}

void check_pila_desapilar(){
	Pila<int> pila;

	pila.Apilar(3);
	pila.Apilar(-3);

	pila.Desapilar();

	ASSERT_EQ(pila.Tope(), 3);

	pila.Apilar(42);
	pila.Apilar(13);

	pila.Desapilar();

	ASSERT_EQ(pila.Tope(), 42);
}

void check_pila_tamanho(){
	Pila<int> pila;

	ASSERT_EQ(pila.Tamanho(), 0);

	pila.Apilar(1);

	ASSERT_EQ(pila.Tamanho(), 1);

	pila.Desapilar();

	ASSERT_EQ(pila.Tamanho(), 0);
}

void check_pila_igualdad(){
	Pila<int> p1, p2;

	p2.Apilar(4);

	ASSERT(!(p1 == p2));

	p1.Apilar(4);

	ASSERT(p1 == p2);
}

// Arbol Binario

void check_arbol_binario_nil(){
	ArbolBinario<int> a;

	ASSERT(a.EsNil());
}

void check_arbol_binario_bin(){
	ArbolBinario<int> i, d, nil;
	ArbolBinario<int> a (i, 1, d);

	ASSERT(!a.EsNil());
	ASSERT(a.Izq().EsNil());
	ASSERT(a.Der().EsNil());
	ASSERT_EQ(a.Raiz(), 1);

	i = ArbolBinario<int>(nil, 0, nil);

	ASSERT(!i.EsNil());
	ASSERT_EQ(i.Raiz(),0);
	ASSERT(a.Izq().EsNil());

	a.Izq() = i;

	ASSERT(!a.Izq().EsNil());
	ASSERT_EQ(a.Izq().Raiz(), 0);

	d = ArbolBinario<int>(nil, 2, nil);

	a.Izq().Der() = d;

	ASSERT(!a.Izq().Der().EsNil());
	ASSERT_EQ(a.Izq().Der().Raiz(), 2);
}

void check_arbol_binario_igualdad(){
	ArbolBinario<int> nil;
	ArbolBinario<int> a, b;

	ASSERT(a == b);

	a = ArbolBinario<int>(nil, 2, nil);

	ASSERT(!(a == b));

	b = ArbolBinario<int>(nil, 3, nil);

	ASSERT(!(a == b));

	b.Raiz() = 2;

	ASSERT(a == b);

	b.Der() = b;

	ASSERT(!(a == b));

	a.Der() = b.Der();

	ASSERT(a == b);
}

void check_arbol_binario_destructor(){
	ArbolBinario<int> nil;
	ArbolBinario<int> a (nil, 3, nil);
	ArbolBinario<int> b (nil, 4, a);
	ArbolBinario<int> c (b, 5, a);
}

void check_arbol_binario_asignacion(){
	ArbolBinario<int> nil;
	ArbolBinario<int> a (nil, 5, nil);
	ArbolBinario<int> b (nil, 4, nil);

	a = b;

	ASSERT_EQ(a.Raiz(), 4);

	a.Der() = a;
	a.Der().Raiz() = 5;

	ASSERT_EQ(a.Der().Raiz(), 5);

	a.Izq() = a;
	a.Izq().Der().Raiz() = 6;

	ASSERT_EQ(a.Izq().Der().Raiz(), 6);
}

void check_arbol_binario_swap(){
	ArbolBinario<int>* b = new ArbolBinario<int>(ArbolBinario<int>(), 2, ArbolBinario<int>());
	ArbolBinario<int>* a = new ArbolBinario<int>(ArbolBinario<int>(), 1, ArbolBinario<int>());
	delete a->DerRapido();
	a->DerRapido() = b;

	ASSERT_EQ(a->Raiz(), 1);
	ASSERT_EQ(a->Der().Raiz(), 2);

	// como voy a pisar el a donde apunta el puntero derecho de b, me encargo de
	// borrar el viejo puntero ya que de otra forma este queda inaccesible
	delete b->DerRapido();
	b->DerRapido() = a;
	a->DerRapido() = new ArbolBinario<int>();

	ASSERT_EQ(b->Raiz(), 2);
	ASSERT_EQ(b->Der().Raiz(), 1);

	// sólo borro b, ya que este se va a encargar de borrar todos los
	// sub-árboles que tenga linkeados
	delete b;
}

//Cola

void check_cola_vacia(){
	Cola<int> cola;

	ASSERT(cola.EsVacia());
}

void check_cola_encolar(){
	Cola<int> cola;

	cola.Encolar(3);

	ASSERT_EQ(cola.Proximo(), 3);
	
	cola.Encolar(-3);

	ASSERT_EQ(cola.Proximo(), 3);
}

void check_cola_desencolar(){
	Cola<int> cola;

	cola.Encolar(3);
	cola.Encolar(-3);

	cola.Desencolar();

	ASSERT_EQ(cola.Proximo(), -3);

	cola.Encolar(42);
	cola.Encolar(13);

	cola.Desencolar();

	ASSERT_EQ(cola.Proximo(), 42);
}

void check_cola_tamanho(){
	Cola<int> cola;

	ASSERT_EQ(cola.Tamanho(), 0);

	cola.Encolar(1);

	ASSERT_EQ(cola.Tamanho(), 1);

	cola.Desencolar();

	ASSERT_EQ(cola.Tamanho(), 0);
}

void check_cola_igualdad(){
	Cola<int> c1, c2;

	c2.Encolar(4);

	ASSERT(!(c1 == c2));

	c1.Encolar(4);

	ASSERT(c1 == c2);

	c1.Encolar(3);
}

// Cola de prioridad
void check_cola_prioridad_vacia(){
	ColaPrioridad<int> cola;
	ASSERT(cola.EsVacia());
}

void check_cola_prioridad_encolar(){
	ColaPrioridad<int> cola;

	cola.Encolar(4, 4);

	ASSERT_EQ(cola.Proximo(), 4);

	cola.Encolar(4, 8);

	ASSERT_EQ(cola.Proximo(), 4);

	cola.Encolar(5, 1);

	ASSERT_EQ(cola.Proximo(), 4);

	cola.Encolar(1, 7);

	ASSERT_EQ(cola.Proximo(), 7);
}

void check_cola_prioridad_desencolar(){
	ColaPrioridad<int> cola;

	cola.Encolar(2, 2);
	cola.Encolar(3, 3);
	cola.Encolar(1, 1);

	ASSERT_EQ(cola.Desencolar(), 1);
	ASSERT_EQ(cola.Desencolar(), 2);
	ASSERT_EQ(cola.Desencolar(), 3);
	ASSERT(cola.EsVacia());
}

void check_cola_de_prioridad_igualdad(){
	ColaPrioridad<int> c1, c2;

	c1.Encolar(1, 1);
	c2.Encolar(2, 2);

	ASSERT(!(c1 == c2));
}

void check_arbol_binario_rotacion_simple(){
	ArbolBinario<int>* a = new ArbolBinario<int>(ArbolBinario<int>(), 1, ArbolBinario<int>());
	ArbolBinario<int>* b = new ArbolBinario<int>(ArbolBinario<int>(), 2, ArbolBinario<int>());
	ArbolBinario<int>* c = new ArbolBinario<int>(ArbolBinario<int>(), 3, ArbolBinario<int>());

	delete a->DerRapido();
	a->DerRapido() = b;

	delete a->Der().DerRapido();
	a->Der().DerRapido() = c;

	ASSERT_EQ(a->Raiz(), 1);
	ASSERT_EQ(a->Der().Raiz(), 2);
	ASSERT_EQ(a->Der().Der().Raiz(), 3);

	delete b->IzqRapido();
	b->IzqRapido() = a;

	a->DerRapido() = new ArbolBinario<int>();

	delete b;
}

void check_arbol_binario_inorder(){
	ArbolBinario<int> nil;
	ArbolBinario<int> a(nil, 3, nil);

	a.Izq() = ArbolBinario<int>(ArbolBinario<int>(nil, 0, nil), 1, ArbolBinario<int>(nil, 2, nil));
	a.Der() = ArbolBinario<int>(ArbolBinario<int>(nil, 4, nil), 5, ArbolBinario<int>(nil, 6, nil));

	ostringstream ossInorder;
	Mostrar(ossInorder, a.Inorder(), '[', ']');
	ASSERT_EQ(ossInorder.str(), "[0,1,2,3,4,5,6]");
}

// Dicc Log
void check_dicc_log_vacio(){
	DiccLog<Nat> d;

	ASSERT(d.EsVacio());
}

void check_dicc_log_definir_uno(){
	DiccLog<Nat> d;

	d.Definir(1, 42);

	ASSERT(d.Definido(1));
}

void check_dicc_log_obtener_uno(){
	DiccLog<Nat> d;

	d.Definir(1, 42);

	ASSERT_EQ(d.Obtener(1), 42);
}

void check_dicc_log_minimo_uno(){
	DiccLog<Nat> d;

	d.Definir(1, 42);

	ASSERT_EQ(d.Minimo(), 42);
}

void check_dicc_log_borrar_uno(){
	DiccLog<Nat> d;

	d.Definir(1, 42);
	d.Borrar(1);

	ASSERT(d.EsVacio());
}

void check_dicc_log_definir_sin_rotacion(){
	DiccLog<Nat> d;

	d.Definir(1, 8);	// padre
	d.Definir(0, 4);	// hijo izquierdo
	d.Definir(2, 15);	// hijo derecho

	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
}

void check_dicc_log_obtener_sin_rotacion(){
	DiccLog<Nat> d;

	d.Definir(1, 8);	// padre
	d.Definir(0, 4);	// hijo izquierdo
	d.Definir(2, 15);	// hijo derecho

	ASSERT_EQ(d.Obtener(0), 4);
	ASSERT_EQ(d.Obtener(1), 8);
	ASSERT_EQ(d.Obtener(2), 15);
}

void check_dicc_log_minimo_sin_rotacion(){
	DiccLog<Nat> d;

	d.Definir(1, 8);	// padre

	ASSERT_EQ(d.Minimo(), 8);

	d.Definir(0, 4);	// hijo izquierdo (mínimo)

	ASSERT_EQ(d.Minimo(), 4);

	d.Definir(2, 15);	// hijo derecho

	ASSERT_EQ(d.Minimo(), 4);
}

void check_dicc_log_borrar_sin_rotacion(){
	DiccLog<Nat> d;

	d.Definir(1, 1);	// padre
	d.Definir(0, 0);	// hijo izquierdo

	d.Borrar(0);

	ASSERT(!d.Definido(0));
	ASSERT(d.Definido(1));

	d.Borrar(1);
	ASSERT(!d.Definido(1));
	ASSERT(d.EsVacio());

	d.Definir(1, 1);	// padre
	d.Definir(2, 2);	// hijo derecho

	d.Borrar(2);

	ASSERT(!d.Definido(2));
	ASSERT(d.Definido(1));

	d.Borrar(1);

	ASSERT(!d.Definido(2));
	ASSERT(d.EsVacio());
}

void check_dicc_log_borrar_directo_raiz_sin_rotacion(){
	DiccLog<Nat> d;

	d.Definir(1, 1);	// padre
	d.Definir(0, 0);	// hijo izquierdo
	d.Definir(2, 2);	// hijo derecho

	d.Borrar(1);

	ASSERT(!d.Definido(1));
	ASSERT(d.Definido(0));
	ASSERT(d.Definido(2));
}

void check_dicc_log_borrar_directo_sin_rotacion(){
	DiccLog<Nat> d;

	d.Definir(1, 1);	// padre
	d.Definir(0, 0);	// hijo izquierdo
	d.Definir(3, 3);	// hijo derecho (padre1)
	d.Definir(4, 4);	// hijo derecho padre1
	d.Definir(2, 2);	// hijo izquierdo padre1

	d.Borrar(3);

	ASSERT(!d.Definido(3));
	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(4));
}

void check_dicc_log_borrar_indirecto_raiz_sin_rotacion(){
	DiccLog<Nat> d;

	d.Definir(2, 2);	// padre0
	d.Definir(1, 1);	// hijo izquierdo (padre1)
	d.Definir(5, 5);	// hijo derecho (padre2)
	d.Definir(0, 0);	// hijo izquierdo padre1
	d.Definir(6, 6);	// hijo derecho padre2
	d.Definir(3, 3);	// hijo izquierdo padre2 (padre3)
	d.Definir(4, 4);	// hijo derecho padre3

	d.Borrar(2);

	ASSERT(!d.Definido(2));
	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));
	ASSERT(d.Definido(5));
	ASSERT(d.Definido(6));
}

void check_dicc_log_borrar_indirecto_sin_rotacion(){
	DiccLog<Nat> d;

	d.Definir(2, 2);	// padre
	d.Definir(1, 1);	// hijo izquierdo (padre1)
	d.Definir(4, 4);	// hijo derecho (padre2)
	d.Definir(0, 0);	// hijo izquierdo padre1
	d.Definir(3, 3);	// hijo izquierdo padre2
	d.Definir(6, 6);	// hijo derecho padre2 (padre3)
	d.Definir(7, 7);	// hijo derecho padre3
	d.Definir(5, 5);	// hijo izquierdo padre3

	d.Borrar(4);

	ASSERT(!d.Definido(4));
	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(5));
	ASSERT(d.Definido(6));
	ASSERT(d.Definido(7));
}

void check_dicc_log_definir_rotacion_simple_izq(){
	DiccLog<Nat> d;

	d.Definir(4, 4);	// padre
	d.Definir(5, 5);	// hijo derecho
	d.Definir(2, 2);	// hijo izquierdo (padre2)
	d.Definir(3, 3);	// hijo derecho de padre2
	d.Definir(1, 1);	// hijo izquierdo de padre2 (padre3)
	d.Definir(0, 0);	// hijo izquierdo de padre2 REBALANCEO

	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));
	ASSERT(d.Definido(5));
}

void check_dicc_log_definir_rotacion_simple_der(){
	DiccLog<Nat> d;

	d.Definir(1, 1);	// padre
	d.Definir(0, 0);	// hijo izquierdo
	d.Definir(3, 3);	// hijo derecho (padre2)
	d.Definir(2, 2);	// hijo izquierdo de padre2
	d.Definir(4, 4);	// hijo derecho de padre2 (padre3)
	d.Definir(5, 5);	// hijo derecho de padre2 REBALANCEO

	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));
	ASSERT(d.Definido(5));
}

void check_dicc_log_obtener_rotacion_simple_izq(){
	DiccLog<Nat> d;

	d.Definir(4, 4);	// padre
	d.Definir(5, 5);	// hijo derecho
	d.Definir(2, 2);	// hijo izquierdo (padre2)
	d.Definir(3, 3);	// hijo derecho de padre2
	d.Definir(1, 1);	// hijo izquierdo de padre2 (padre3)
	d.Definir(0, 0);	// hijo izquierdo de padre2 REBALANCEO

	ASSERT_EQ(d.Obtener(0), 0);
	ASSERT_EQ(d.Obtener(1), 1);
	ASSERT_EQ(d.Obtener(2), 2);
	ASSERT_EQ(d.Obtener(3), 3);
	ASSERT_EQ(d.Obtener(4), 4);
	ASSERT_EQ(d.Obtener(5), 5);
}

void check_dicc_log_obtener_rotacion_simple_der(){
	DiccLog<Nat> d;

	d.Definir(1, 1);	// padre
	d.Definir(0, 0);	// hijo izquierdo
	d.Definir(3, 3);	// hijo derecho (padre2)
	d.Definir(2, 2);	// hijo izquierdo de padre2
	d.Definir(4, 4);	// hijo derecho de padre2 (padre3)
	d.Definir(5, 5);	// hijo derecho de padre2 REBALANCEO

	ASSERT_EQ(d.Obtener(0), 0);
	ASSERT_EQ(d.Obtener(1), 1);
	ASSERT_EQ(d.Obtener(2), 2);
	ASSERT_EQ(d.Obtener(3), 3);
	ASSERT_EQ(d.Obtener(4), 4);
	ASSERT_EQ(d.Obtener(5), 5);
}

void check_dicc_log_minimo_rotacion_simple_izq(){
	DiccLog<Nat> d;

	d.Definir(4, 4);	// padre

	ASSERT_EQ(d.Minimo(), 4);

	d.Definir(5, 5);	// hijo derecho

	ASSERT_EQ(d.Minimo(), 4);

	d.Definir(2, 2);	// hijo izquierdo (padre2)

	ASSERT_EQ(d.Minimo(), 2);

	d.Definir(3, 3);	// hijo derecho de padre2

	ASSERT_EQ(d.Minimo(), 2);

	d.Definir(1, 1);	// hijo izquierdo de padre2 (padre3)

	ASSERT_EQ(d.Minimo(), 1);

	d.Definir(0, 0);	// hijo izquierdo de padre2 REBALANCEO

	ASSERT_EQ(d.Minimo(), 0);
}

void check_dicc_log_minimo_rotacion_simple_der(){
	DiccLog<Nat> d;

	d.Definir(1, 1);	// padre

	ASSERT_EQ(d.Minimo(), 1);

	d.Definir(0, 0);	// hijo izquierdo

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(3, 3);	// hijo derecho (padre2)

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(2, 2);	// hijo izquierdo de padre2

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(4, 4);	// hijo derecho de padre2 (padre3)

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(5, 5);	// hijo derecho de padre2 REBALANCEO

	ASSERT_EQ(d.Minimo(), 0);
}

void check_dicc_log_definir_rotacion_doble_uno_der(){
	DiccLog<int> d;

	d.Definir(0, 0);	// padre0
	d.Definir(2, 2);	// hijo derecho padre0 (padre1)
	d.Definir(1, 1);	// hijo izquierdo padre1 REBALANCEO

	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
}

void check_dicc_log_definir_rotacion_doble_uno_izq(){
	DiccLog<int> d;

	d.Definir(2, 2);	// padre0
	d.Definir(0, 0);	// hijo izquierdo padre0 (padre1)
	d.Definir(1, 1);	// hijo derecho padre1 REBALANCEO

	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
}

void check_dicc_log_obtener_rotacion_doble_uno_der(){
	DiccLog<int> d;

	d.Definir(0, 0);	// padre0
	d.Definir(2, 2);	// hijo derecho padre0 (padre1)
	d.Definir(1, 1);	// hijo izquierdo padre1 REBALANCEO

	ASSERT_EQ(d.Obtener(0), 0);
	ASSERT_EQ(d.Obtener(1), 1);
	ASSERT_EQ(d.Obtener(2), 2);
}

void check_dicc_log_obtener_rotacion_doble_uno_izq(){
	DiccLog<int> d;

	d.Definir(2, 2);	// padre0
	d.Definir(0, 0);	// hijo izquierdo padre0 (padre1)
	d.Definir(1, 1);	// hijo derecho padre1 REBALANCEO

	ASSERT_EQ(d.Obtener(0), 0);
	ASSERT_EQ(d.Obtener(1), 1);
	ASSERT_EQ(d.Obtener(2), 2);
}

void check_dicc_log_minimo_rotacion_doble_uno_der(){
	DiccLog<int> d;

	d.Definir(0, 0);	// padre0

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(2, 2);	// hijo derecho padre0 (padre1)

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(1, 1);	// hijo izquierdo padre1 REBALANCEO

	ASSERT_EQ(d.Minimo(), 0);
}

void check_dicc_log_minimo_rotacion_doble_uno_izq(){
	DiccLog<int> d;

	d.Definir(2, 2);	// padre0

	ASSERT_EQ(d.Minimo(), 2);

	d.Definir(0, 0);	// hijo derecho padre0 (padre1)

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(1, 1);	// hijo izquierdo padre1 REBALANCEO

	ASSERT_EQ(d.Minimo(), 0);
}

void check_dicc_log_definir_rotacion_doble_dos_der(){
	DiccLog<int> d;

	d.Definir(1, 1);	// padre0
	d.Definir(0, 0);	// hijo izquierdo padre0
	d.Definir(4, 4);	// hijo derecho padre0 (padre1)
	d.Definir(5, 5);	// hijo derecho padre1
	d.Definir(2, 2);	// hijo izquierdo padre1 (padre2)
	d.Definir(3, 3);	// hijo derecho padre2 REBALANCEO

	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));
	ASSERT(d.Definido(5));
}

void check_dicc_log_definir_rotacion_doble_dos_izq(){
	DiccLog<int> d;

	d.Definir(4, 4);	// padre0
	d.Definir(5, 5);	// hijo derecho padre0
	d.Definir(1, 1);	// hijo izquierdo padre0 (padre1)
	d.Definir(0, 0);	// hijo izquierdo padre1
	d.Definir(3, 3);	// hijo derecho padre1 (padre2)
	d.Definir(2, 2);	// hijo izquierdo padre2 REBALANCEO

	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));
	ASSERT(d.Definido(5));
}

void check_dicc_log_obtener_rotacion_doble_dos_der(){
	DiccLog<int> d;

	d.Definir(1, 1);	// padre0
	d.Definir(0, 0);	// hijo izquierdo padre0
	d.Definir(4, 4);	// hijo derecho padre0 (padre1)
	d.Definir(5, 5);	// hijo derecho padre1
	d.Definir(2, 2);	// hijo izquierdo padre1 (padre2)
	d.Definir(3, 3);	// hijo derecho padre2 REBALANCEO

	ASSERT_EQ(d.Obtener(0), 0);
	ASSERT_EQ(d.Obtener(1), 1);
	ASSERT_EQ(d.Obtener(2), 2);
	ASSERT_EQ(d.Obtener(3), 3);
	ASSERT_EQ(d.Obtener(4), 4);
	ASSERT_EQ(d.Obtener(5), 5);
}

void check_dicc_log_obtener_rotacion_doble_dos_izq(){
	DiccLog<int> d;

	d.Definir(4, 4);	// padre0
	d.Definir(5, 5);	// hijo derecho padre0
	d.Definir(1, 1);	// hijo izquierdo padre0 (padre1)
	d.Definir(0, 0);	// hijo izquierdo padre1
	d.Definir(3, 3);	// hijo derecho padre1 (padre2)
	d.Definir(2, 2);	// hijo izquierdo padre2 REBALANCEO

	ASSERT_EQ(d.Obtener(0), 0);
	ASSERT_EQ(d.Obtener(1), 1);
	ASSERT_EQ(d.Obtener(2), 2);
	ASSERT_EQ(d.Obtener(3), 3);
	ASSERT_EQ(d.Obtener(4), 4);
	ASSERT_EQ(d.Obtener(5), 5);
}

void check_dicc_log_minimo_rotacion_doble_dos_der(){
	DiccLog<int> d;

	d.Definir(1, 1);	// padre0

	ASSERT_EQ(d.Minimo(), 1);

	d.Definir(0, 0);	// hijo izquierdo padre0

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(4, 4);	// hijo derecho padre0 (padre1)

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(5, 5);	// hijo derecho padre1

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(2, 2);	// hijo izquierdo padre1 (padre2)

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(3, 3);	// hijo derecho padre2 REBALANCEO

	ASSERT_EQ(d.Minimo(), 0);
}

void check_dicc_log_minimo_rotacion_doble_dos_izq(){
	DiccLog<int> d;

	d.Definir(4, 4);	// padre0

	ASSERT_EQ(d.Minimo(), 4);

	d.Definir(5, 5);	// hijo derecho padre0

	ASSERT_EQ(d.Minimo(), 4);

	d.Definir(1, 1);	// hijo izquierdo padre0 (padre1)

	ASSERT_EQ(d.Minimo(), 1);

	d.Definir(0, 0);	// hijo izquierdo padre1

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(3, 3);	// hijo derecho padre1 (padre2)

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(2, 2);	// hijo izquierdo padre2 REBALANCEO

	ASSERT_EQ(d.Minimo(), 0);
}

void check_dicc_log_definir_rotacion_doble_tres_der(){
	DiccLog<int> d;

	d.Definir(1, 1);	// padre0
	d.Definir(0, 0);	// hijo izquierdo padre0
	d.Definir(4, 4);	// hijo derecho padre0 (padre1)
	d.Definir(5, 5);	// hijo derecho padre1
	d.Definir(3, 3);	// hijo izquierdo padre1 (padre2)
	d.Definir(2, 2);	// hijo izquierdo padre2

	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));
	ASSERT(d.Definido(5));
}

void check_dicc_log_definir_rotacion_doble_tres_izq(){
	DiccLog<int> d;

	d.Definir(4, 4);	// padre0
	d.Definir(5, 5);	// hijo derecho padre0
	d.Definir(1, 1);	// hijo izquierdo padre0 (padre1)
	d.Definir(0, 0);	// hijo izquierdo padre1
	d.Definir(2, 2);	// hijo derecho padre1 (padre2)
	d.Definir(3, 3);	// hijo derecho padre2 REBALANCEO

	ASSERT(d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));
	ASSERT(d.Definido(5));
}

void check_dicc_log_obtener_rotacion_doble_tres_der(){
	DiccLog<int> d;

	d.Definir(1, 1);	// padre0
	d.Definir(0, 0);	// hijo izquierdo padre0
	d.Definir(4, 4);	// hijo derecho padre0 (padre1)
	d.Definir(5, 5);	// hijo derecho padre1
	d.Definir(3, 3);	// hijo izquierdo padre1 (padre2)
	d.Definir(2, 2);	// hijo izquierdo padre2

	ASSERT_EQ(d.Obtener(0), 0);
	ASSERT_EQ(d.Obtener(1), 1);
	ASSERT_EQ(d.Obtener(2), 2);
	ASSERT_EQ(d.Obtener(3), 3);
	ASSERT_EQ(d.Obtener(4), 4);
	ASSERT_EQ(d.Obtener(5), 5);
}

void check_dicc_log_obtener_rotacion_doble_tres_izq(){
	DiccLog<int> d;

	d.Definir(4, 4);	// padre0
	d.Definir(5, 5);	// hijo derecho padre0
	d.Definir(1, 1);	// hijo izquierdo padre0 (padre1)
	d.Definir(0, 0);	// hijo izquierdo padre1
	d.Definir(2, 2);	// hijo derecho padre1 (padre2)
	d.Definir(3, 3);	// hijo derecho padre2 REBALANCEO

	ASSERT_EQ(d.Obtener(0), 0);
	ASSERT_EQ(d.Obtener(1), 1);
	ASSERT_EQ(d.Obtener(2), 2);
	ASSERT_EQ(d.Obtener(3), 3);
	ASSERT_EQ(d.Obtener(4), 4);
	ASSERT_EQ(d.Obtener(5), 5);
}

void check_dicc_log_minimo_rotacion_doble_tres_der(){
	DiccLog<int> d;

	d.Definir(1, 1);	// padre0

	ASSERT_EQ(d.Minimo(), 1);

	d.Definir(0, 0);	// hijo izquierdo padre0

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(4, 4);	// hijo derecho padre0 (padre1)

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(5, 5);	// hijo derecho padre1

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(3, 3);	// hijo izquierdo padre1 (padre2)

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(2, 2);	// hijo izquierdo padre2

	ASSERT_EQ(d.Minimo(), 0);
}

void check_dicc_log_minimo_rotacion_doble_tres_izq(){
	DiccLog<int> d;

	d.Definir(4, 4);	// padre0

	ASSERT_EQ(d.Minimo(), 4);

	d.Definir(5, 5);	// hijo derecho padre0

	ASSERT_EQ(d.Minimo(), 4);

	d.Definir(1, 1);	// hijo izquierdo padre0 (padre1)

	ASSERT_EQ(d.Minimo(), 1);

	d.Definir(0, 0);	// hijo izquierdo padre1

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(2, 2);	// hijo derecho padre1 (padre2)

	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(3, 3);	// hijo derecho padre2 REBALANCEO

	ASSERT_EQ(d.Minimo(), 0);
}

void check_dicc_log_borrar_rotacion_simple_uno(){
	DiccLog<Nat> d;

	d.Definir(2, 2);	// padre
	d.Definir(1, 1);	// hijo izquierdo (padre0)
	d.Definir(4, 4);	// hijo derecho (padre1)
	d.Definir(0, 0);	// hijo izquierdo padre0
	d.Definir(3, 3);	// hijo izquierdo padre1
	d.Definir(5, 5);	// hijo derecho padre1 (padre2)
	d.Definir(6, 6);	// hijo derecho padre2

	ASSERT_EQ(d.Minimo(), 0);

	d.Borrar(0);

	ASSERT(!d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));
	ASSERT(d.Definido(5));
	ASSERT(d.Definido(6));

	ASSERT_EQ(d.Minimo(), 1);
}

void check_dicc_log_borrar_rotacion_simple_dos(){
	DiccLog<Nat> d;

	d.Definir(1, 1);	// padre
	d.Definir(0, 0);	// hijo izquierdo
	d.Definir(3, 3);	// hijo derecho (padre0)
	d.Definir(2, 2);	// hijo izquierdo padre0
	d.Definir(4, 4);	// hijo derecho padre0

	ASSERT_EQ(d.Minimo(), 0);

	d.Borrar(0);

	ASSERT(!d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));

	ASSERT_EQ(d.Minimo(), 1);
}

void check_dicc_log_borrar_rotacion_doble_uno(){
	DiccLog<Nat> d;

	d.Definir(2, 2);	// padre
	d.Definir(1, 1);	// hijo izquierdo (padre0)
	d.Definir(6, 6);	// hijo derecho (padre1)
	d.Definir(0, 0);	// hijo izquierdo padre0
	d.Definir(7, 7);	// hijo derecho padre1
	d.Definir(4, 4);	// hijo izquierdo padre1 (padre2)
	d.Definir(3, 3);	// hijo izquierdo padre2
	d.Definir(5, 5);	// hijo derecho padre2

	ASSERT_EQ(d.Minimo(), 0);

	d.Borrar(0);

	ASSERT(!d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));
	ASSERT(d.Definido(5));
	ASSERT(d.Definido(6));
	ASSERT(d.Definido(7));

	ASSERT_EQ(d.Minimo(), 1);
}

void check_dicc_log_borrar_rotacion_doble_dos(){
	DiccLog<Nat> d;

	d.Definir(2, 2);	// padre
	d.Definir(1, 1);	// hijo izquierdo (padre0)
	d.Definir(5, 5);	// hijo derecho (padre1)
	d.Definir(0, 0);	// hijo izquierdo padre0
	d.Definir(3, 3);	// hijo izquierdo padre1 (padre2)
	d.Definir(6, 6);	// hijo derecho padre1
	d.Definir(4, 4);	// hijo derecho padre2

	ASSERT_EQ(d.Minimo(), 0);

	d.Borrar(0);

	ASSERT(!d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));
	ASSERT(d.Definido(5));
	ASSERT(d.Definido(6));

	ASSERT_EQ(d.Minimo(), 1);
}

void check_dicc_log_borrar_rotacion_doble_tres(){
	DiccLog<Nat> d;

	d.Definir(2, 2);	// padre
	d.Definir(1, 1);	// hijo izquierdo (padre0)
	d.Definir(5, 5);	// hijo derecho (padre1)
	d.Definir(0, 0);	// hijo izquierdo padre0
	d.Definir(4, 4);	// hijo izquierdo padre1 (padre2)
	d.Definir(6, 6);	// hijo derecho padre1
	d.Definir(3, 3);	// hijo izquierdo padre2

	ASSERT_EQ(d.Minimo(), 0);

	d.Borrar(0);

	ASSERT(!d.Definido(0));
	ASSERT(d.Definido(1));
	ASSERT(d.Definido(2));
	ASSERT(d.Definido(3));
	ASSERT(d.Definido(4));
	ASSERT(d.Definido(5));
	ASSERT(d.Definido(6));

	ASSERT_EQ(d.Minimo(), 1);
}

void check_dicc_log_borrar_minimo(){
	DiccLog<Nat> d;

	// Creo el árbol

	d.Definir(3, 3);	// padre
	ASSERT_EQ(d.Minimo(), 3);

	d.Definir(1, 1);	// hijo izquierdo (padre0)
	ASSERT_EQ(d.Minimo(), 1);

	d.Definir(5, 5);	// hijo derecho (padre1)
	ASSERT_EQ(d.Minimo(), 1);

	d.Definir(0, 0);	// hijo izquierdo padre0
	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(2, 2);	// hijo derecho padre0
	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(4, 4);	// hijo izquierdo padre1
	ASSERT_EQ(d.Minimo(), 0);

	d.Definir(6, 6);	// hijo derecho padre1
	ASSERT_EQ(d.Minimo(), 0);

	// Borro todo el árbol

	d.Borrar(d.Minimo());	// min = 0
	ASSERT_EQ(d.Minimo(), 1);

	d.Borrar(d.Minimo()); // min = 1
	ASSERT_EQ(d.Minimo(), 2);

	d.Borrar(d.Minimo()); // min = 2
	ASSERT_EQ(d.Minimo(), 3);

	d.Borrar(d.Minimo()); // min = 3
	ASSERT_EQ(d.Minimo(), 4);

	d.Borrar(d.Minimo()); // min = 4
	ASSERT_EQ(d.Minimo(), 5);

	d.Borrar(d.Minimo()); // min = 5
	ASSERT_EQ(d.Minimo(), 6);

	d.Borrar(d.Minimo()); // min = 6
	ASSERT(d.EsVacio());
}

void check_dicc_log_asignacion(){
	DiccLog<Nat> d0, d1;

	d1.Definir(0, 0);
	d1.Definir(1, 1);
	d1.Definir(2, 2);
	d1.Definir(3, 3);

	d0 = d1;

	ASSERT(d0.Definido(0));
	ASSERT(d0.Definido(1));
	ASSERT(d0.Definido(2));
	ASSERT(d0.Definido(3));

	ASSERT_EQ(d0.Minimo(), 0);

	d0.Obtener(0) = 42;

	ASSERT_EQ(d0.Obtener(0), 42);
	ASSERT_EQ(d1.Obtener(0), 0);
}

void check_dicc_log_copia(){
	DiccLog<Nat> d1;

	d1.Definir(0, 0);
	d1.Definir(1, 1);
	d1.Definir(2, 2);
	d1.Definir(3, 3);

	DiccLog<Nat> d0(d1);

	ASSERT(d0.Definido(0));
	ASSERT(d0.Definido(1));
	ASSERT(d0.Definido(2));
	ASSERT(d0.Definido(3));

	ASSERT_EQ(d0.Minimo(), 0);

	d0.Obtener(0) = 42;

	ASSERT_EQ(d0.Obtener(0), 42);
	ASSERT_EQ(d1.Obtener(0), 0);
}

void check_dicc_log_igualdad(){
	DiccLog<Nat> d0, d1;

	d0.Definir(0, 0);
	d0.Definir(1, 1);
	d0.Definir(2, 2);
	d0.Definir(3, 3);
	d0.Definir(4, 4);

	d1.Definir(4, 4);
	d1.Definir(3, 3);
	d1.Definir(1, 1);
	d1.Definir(0, 0);
	d1.Definir(2, 2);

	ASSERT(d0 == d1);
}

void check_dicc_log_redefinir(){
	DiccLog<Nat> d;

	d.Definir(0, 0);
	d.Definir(1, 1);
	d.Definir(2, 2);

	ASSERT_EQ(d.Obtener(0), 0);

	d.Definir(0, 10);
	ASSERT_EQ(d.Obtener(0), 10);

	ASSERT_EQ(d.Obtener(1), 1);

	d.Definir(1, 11);
	ASSERT_EQ(d.Obtener(1), 11);

	ASSERT_EQ(d.Obtener(2), 2);

	d.Definir(2, 12);
	ASSERT_EQ(d.Obtener(2), 12);
}



// ---------------------------------------------------------------------

/**
* Ejemplo de caso de test, con llamadas a las rutinas de aserción
* definidas en mini_test.h
* TODO: descomentar cuando esté dcnet

void test_dcnet_ejemplo() {
	Conj<Interfaz> conjIc1;
	Conj<Interfaz> conjIc2;
	Conj<Interfaz> conjIc3;

	conjIc1.Agregar(1);
	conjIc1.Agregar(2);
	conjIc1.Agregar(3);

	conjIc2.Agregar(1);
	conjIc2.Agregar(2);
	conjIc2.Agregar(3);

	conjIc3.Agregar(1);
	conjIc3.Agregar(2);

	Computadora c1 = "dc.uba.ar";
	Computadora c2 = "uba.ar";
	Computadora c3 = "dm.uba.ar";

	Driver dcnet;

	dcnet.AgregarComputadora(c1, conjIc1);
	dcnet.AgregarComputadora(c2, conjIc2);
	dcnet.AgregarComputadora(c3, conjIc3);

	// ejemplo - Indexado en 0
	Interfaz i1 = dcnet.IesimaInterfazDe(c1, 0);
	Interfaz i2 = dcnet.IesimaInterfazDe(c2, 2);

	dcnet.Conectar(c1, i1, c2, i2);
	dcnet.CrearPaquete(c1, c2, 3);
	dcnet.AvanzarSegundo();

}
*/

void check_trie() {
	DiccString<int> trai;
	trai.definir("uno", 1);
	trai.definir("dos", 2);
	trai.definir("tres", 3);
	trai.definir("cuatro", 4);

	ASSERT(trai.definido("uno"));
	ASSERT_EQ(trai.obtener("dos"), 2);
	ASSERT(trai.definido("tres"));
	ASSERT_EQ(trai.obtener("cuatro"), 4);

	trai.borrar("uno");
	ASSERT(!trai.definido("uno"));

}

int main(int argc, char **argv){

	// Trie
	RUN_TEST(check_trie);

	// Pila
	RUN_TEST(check_pila_vacia);
	RUN_TEST(check_pila_apilar);
	RUN_TEST(check_pila_desapilar);
	RUN_TEST(check_pila_tamanho);
	RUN_TEST(check_pila_igualdad);

	// Arbol Binario
	RUN_TEST(check_arbol_binario_nil);
	RUN_TEST(check_arbol_binario_bin);
	RUN_TEST(check_arbol_binario_igualdad);
	RUN_TEST(check_arbol_binario_destructor);
	RUN_TEST(check_arbol_binario_asignacion);
	RUN_TEST(check_arbol_binario_swap);

	//Cola
	RUN_TEST(check_cola_vacia);
	RUN_TEST(check_cola_encolar);
	RUN_TEST(check_cola_desencolar);
	RUN_TEST(check_cola_tamanho);
	RUN_TEST(check_cola_igualdad);

	//Cola de prioridad
	RUN_TEST(check_cola_prioridad_vacia);
	RUN_TEST(check_cola_prioridad_encolar);
	RUN_TEST(check_cola_prioridad_desencolar);
	RUN_TEST(check_cola_de_prioridad_igualdad);

	RUN_TEST(check_arbol_binario_rotacion_simple);
	RUN_TEST(check_arbol_binario_inorder);

	// Dicc Log
	RUN_TEST(check_dicc_log_vacio);
	RUN_TEST(check_dicc_log_definir_uno);
	RUN_TEST(check_dicc_log_obtener_uno);
	RUN_TEST(check_dicc_log_minimo_uno);
	RUN_TEST(check_dicc_log_borrar_uno);

	RUN_TEST(check_dicc_log_definir_sin_rotacion);
	RUN_TEST(check_dicc_log_obtener_sin_rotacion);
	RUN_TEST(check_dicc_log_minimo_sin_rotacion);

	RUN_TEST(check_dicc_log_borrar_sin_rotacion);
	RUN_TEST(check_dicc_log_borrar_directo_raiz_sin_rotacion);
	RUN_TEST(check_dicc_log_borrar_directo_sin_rotacion);
	RUN_TEST(check_dicc_log_borrar_indirecto_raiz_sin_rotacion);
	RUN_TEST(check_dicc_log_borrar_indirecto_sin_rotacion);

	RUN_TEST(check_dicc_log_definir_rotacion_simple_izq);
	RUN_TEST(check_dicc_log_definir_rotacion_simple_der);
	RUN_TEST(check_dicc_log_obtener_rotacion_simple_izq);
	RUN_TEST(check_dicc_log_obtener_rotacion_simple_der);
	RUN_TEST(check_dicc_log_minimo_rotacion_simple_izq);
	RUN_TEST(check_dicc_log_minimo_rotacion_simple_der);

	RUN_TEST(check_dicc_log_definir_rotacion_doble_uno_der);
	RUN_TEST(check_dicc_log_definir_rotacion_doble_uno_izq);
	RUN_TEST(check_dicc_log_obtener_rotacion_doble_uno_der);
	RUN_TEST(check_dicc_log_obtener_rotacion_doble_uno_izq);
	RUN_TEST(check_dicc_log_minimo_rotacion_doble_uno_der);
	RUN_TEST(check_dicc_log_minimo_rotacion_doble_uno_izq);

	RUN_TEST(check_dicc_log_definir_rotacion_doble_dos_der);
	RUN_TEST(check_dicc_log_definir_rotacion_doble_dos_izq);
	RUN_TEST(check_dicc_log_obtener_rotacion_doble_dos_der);
	RUN_TEST(check_dicc_log_obtener_rotacion_doble_dos_izq);
	RUN_TEST(check_dicc_log_minimo_rotacion_doble_dos_der);
	RUN_TEST(check_dicc_log_minimo_rotacion_doble_dos_izq);

	RUN_TEST(check_dicc_log_definir_rotacion_doble_tres_der);
	RUN_TEST(check_dicc_log_definir_rotacion_doble_tres_izq);
	RUN_TEST(check_dicc_log_obtener_rotacion_doble_tres_der);
	RUN_TEST(check_dicc_log_obtener_rotacion_doble_tres_izq);
	RUN_TEST(check_dicc_log_minimo_rotacion_doble_tres_der);
	RUN_TEST(check_dicc_log_minimo_rotacion_doble_tres_izq);

	RUN_TEST(check_dicc_log_borrar_rotacion_simple_uno);
	RUN_TEST(check_dicc_log_borrar_rotacion_simple_dos);
	RUN_TEST(check_dicc_log_borrar_rotacion_doble_uno);
	RUN_TEST(check_dicc_log_borrar_rotacion_doble_dos);
	RUN_TEST(check_dicc_log_borrar_rotacion_doble_tres);
	RUN_TEST(check_dicc_log_borrar_minimo);

	RUN_TEST(check_dicc_log_asignacion);
	RUN_TEST(check_dicc_log_copia);
	RUN_TEST(check_dicc_log_igualdad);
	RUN_TEST(check_dicc_log_redefinir);
	
	return 0;
}
