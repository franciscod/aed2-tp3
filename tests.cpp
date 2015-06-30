#include "mini_test.h"
#include "Pila.h"
#include "ArbolBinario.h"

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

int main(){
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
	return 0;
}
