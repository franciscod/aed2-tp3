#include "Driver.h"
#include "mini_test.h"
#include "aed2/Lista.h"
#include "aed2/Conj.h"
#include "aed2/Dicc.h"
#include "aed2/dicc_trie.h"

#include <string>
#include <iostream>

// Modulos del grupo
#include "Pila.h"
#include "ArbolBinario.h"
#include "Red.h"

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
	ArbolBinario<int>* a = new ArbolBinario<int>(new ArbolBinario<int>(), 1, b);

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


// Red

void check_red_nueva(){
	Red r;
	ASSERT(r.computadoras().EsVacio());
}

void check_red_agregar_compu(){
	Red r;
	ASSERT(r.computadoras().EsVacio())
	Compu c1;
	c1.ip = "gugel.com";
	c1.interfaces.Agregar(1);
	c1.interfaces.Agregar(2);
	c1.interfaces.Agregar(3);

	Compu c2;
	c2.ip = "hamason.com";
	c2.interfaces.Agregar(4);
	c2.interfaces.Agregar(5);
	c2.interfaces.Agregar(6);

	r.agregarComputadora(c1);
	r.agregarComputadora(c2);

	ASSERT(r.computadoras().Pertenece(c1));
	ASSERT(r.computadoras().Pertenece(c2));

}


void check_red_conectar(){
	Red r;
	Compu c1;
	c1.ip = "riquelme";
	c1.interfaces.Agregar(1);
	c1.interfaces.Agregar(2);

	Compu c2;
	c2.ip = "pekerman";
	c2.interfaces.Agregar(3);
	c2.interfaces.Agregar(4);

	r.agregarComputadora(c1);
	r.agregarComputadora(c2);

	r.conectar(c1, c2, 1, 3);

	ASSERT(r.conectadas(c1, c2));

	ASSERT(r.usaInterfaz(c1, 1));
	ASSERT(!r.usaInterfaz(c1, 2));
	ASSERT(r.usaInterfaz(c2, 3));
	ASSERT(!r.usaInterfaz(c2, 4));

	ASSERT_EQ(r.interfazUsada(c1, c2), 1);
	ASSERT_EQ(r.interfazUsada(c2, c1), 3);


	Conj <Compu> v1 = r.vecinos(c1) ;
	ASSERT(v1.Pertenece(c2));
	Conj <Compu> v2 = r.vecinos(c2) ;
	ASSERT(v2.Pertenece(c1));
	//ASSERT_EQ(r.vecinos(c1),r.vecinos(c2));
	ASSERT(r == r.copiar());
}

void check_red_conecta_vecinos(){
	Red r;
	Compu c1;
	c1.ip = "1";
	c1.interfaces.Agregar(1);
	c1.interfaces.Agregar(2);

	Compu c2;
	c2.ip = "2";
	c2.interfaces.Agregar(1);
	c2.interfaces.Agregar(2);

	Compu c3;
	c3.ip = "3";
	c3.interfaces.Agregar(1);
	c3.interfaces.Agregar(2);

	r.agregarComputadora(c1);
	r.agregarComputadora(c2);
	r.agregarComputadora(c3);

	ASSERT(r.vecinos(c1).EsVacio());
	ASSERT(r.vecinos(c2).EsVacio());
	ASSERT(r.vecinos(c3).EsVacio());

	r.conectar(c1, c2, 1, 1);
	ASSERT_EQ(r.vecinos(c1).Cardinal(), 1);
	ASSERT_EQ(r.vecinos(c2).Cardinal(), 1);
	ASSERT_EQ(r.vecinos(c3).Cardinal(), 0);
	ASSERT(r.vecinos(c1).Pertenece(c2));
	ASSERT(r.vecinos(c2).Pertenece(c1));

	r.conectar(c1, c3, 2, 1);
	ASSERT_EQ(r.vecinos(c1).Cardinal(), 2);
	ASSERT_EQ(r.vecinos(c2).Cardinal(), 1);
	ASSERT_EQ(r.vecinos(c3).Cardinal(), 1);
	ASSERT(r.vecinos(c1).Pertenece(c2));
	ASSERT(r.vecinos(c1).Pertenece(c3));
	ASSERT(r.vecinos(c2).Pertenece(c1));
	ASSERT(r.vecinos(c3).Pertenece(c1));

	r.conectar(c2, c3, 2, 2);

	ASSERT_EQ(r.vecinos(c1).Cardinal(), 2);
	ASSERT_EQ(r.vecinos(c2).Cardinal(), 2);
	ASSERT_EQ(r.vecinos(c3).Cardinal(), 2);
	ASSERT(r.vecinos(c1).Pertenece(c2));
	ASSERT(r.vecinos(c1).Pertenece(c3));
	ASSERT(r.vecinos(c2).Pertenece(c1));
	ASSERT(r.vecinos(c2).Pertenece(c3));
	ASSERT(r.vecinos(c3).Pertenece(c1));
	ASSERT(r.vecinos(c3).Pertenece(c2));

	ASSERT(r.hayCamino(c1,c2));
	ASSERT(r.hayCamino(c1,c3));
	ASSERT(r.hayCamino(c2,c3));

	ASSERT(r == r.copiar());

}

void check_red_caminimos_huge(){

	/*
		¡¡¡ CALATE ESTA RED !!!

	       /-----A----\
	      |   |   |   |
	      |   |   |   |
	J-- JAB  AB1 AB2 AB3
	      \   |  |  /
	            B----------Z-\
	         BC1 BC2          X
	            C----------Y-/
	*/
	Red r;

	cout << "\n > Agregando compus...\n";

	Compu J; J.ip = "J";
	J.interfaces.Agregar(1);
	r.agregarComputadora(J);

	Compu A; A.ip = "A";
	A.interfaces.Agregar(1);
	A.interfaces.Agregar(2);
	A.interfaces.Agregar(3);
	A.interfaces.Agregar(4);
	r.agregarComputadora(A);

	Compu B; B.ip = "B";
	B.interfaces.Agregar(1);
	B.interfaces.Agregar(2);
	B.interfaces.Agregar(3);
	B.interfaces.Agregar(4);
	B.interfaces.Agregar(5);
	B.interfaces.Agregar(6);
	B.interfaces.Agregar(7);
	r.agregarComputadora(B);

	Compu C; C.ip = "C";
	C.interfaces.Agregar(1);
	C.interfaces.Agregar(2);
	C.interfaces.Agregar(3);
	r.agregarComputadora(C);

	Compu JAB; JAB.ip = "JAB";
	JAB.interfaces.Agregar(1);
	JAB.interfaces.Agregar(2);
	JAB.interfaces.Agregar(3);
	r.agregarComputadora(JAB);

	Compu AB1; AB1.ip = "AB1";
	AB1.interfaces.Agregar(1);
	AB1.interfaces.Agregar(2);
	r.agregarComputadora(AB1);

	Compu AB2; AB2.ip = "AB2";
	AB2.interfaces.Agregar(1);
	AB2.interfaces.Agregar(2);
	r.agregarComputadora(AB2);

	Compu AB3; AB3.ip = "AB3";
	AB3.interfaces.Agregar(1);
	AB3.interfaces.Agregar(2);
	r.agregarComputadora(AB3);

	Compu BC1; BC1.ip = "BC1";
	BC1.interfaces.Agregar(1);
	BC1.interfaces.Agregar(2);
	r.agregarComputadora(BC1);

	Compu BC2; BC2.ip = "BC2";
	BC2.interfaces.Agregar(1);
	BC2.interfaces.Agregar(2);
	r.agregarComputadora(BC2);

	Compu Y; Y.ip = "Y";
	Y.interfaces.Agregar(1);
	Y.interfaces.Agregar(2);
	r.agregarComputadora(Y);

	Compu X; X.ip = "X";
	X.interfaces.Agregar(1);
	X.interfaces.Agregar(2);
	r.agregarComputadora(X);

	Compu Z; Z.ip = "Z";
	Z.interfaces.Agregar(1);
	Z.interfaces.Agregar(2);
	r.agregarComputadora(Z);

	// ufffff, a conectar... cómo era el dibujito?
    /*
		¡¡¡ CALATE ESTA RED !!!

	       /-----A----\
	      |   |   |   |
	      |   |   |   |
	J-- JAB  AB1 AB2 AB3
	      \   |  |  /
	            B----------Z-\
	         BC1 BC2          X
	            C----------Y-/
	*/
	cout << " > Conectando y generando caminos...\n";

		r.conectar(A, AB3, 4, 1);
		r.conectar(A, AB2, 3, 1);
		r.conectar(A, AB1, 2, 1);
		r.conectar(A, JAB, 1, 1);
	r.conectar(J, JAB, 1, 3);
	    r.conectar(B, JAB, 1, 2);
		r.conectar(B, AB1, 2, 2);
		r.conectar(B, AB2, 3, 2);
		r.conectar(B, AB3, 4, 2);
		                r.conectar(B, Z, 5, 1);
	r.conectar(B, BC1, 6, 1);          r.conectar(Z, X, 2, 1);
	r.conectar(B, BC2, 7, 1);          r.conectar(X, Y, 2, 1);
		                r.conectar(C, X, 1, 2);
	r.conectar(C, BC1, 2, 2);
	r.conectar(C, BC2, 3, 2);

	ASSERT(r.hayCamino(A,JAB));
	ASSERT(r.hayCamino(A,AB1));
	ASSERT(r.hayCamino(A,AB2));
	ASSERT(r.hayCamino(A,AB3));
	ASSERT(r.hayCamino(A,J));

	ASSERT(r.hayCamino(A,B));
	ASSERT(r.hayCamino(A,Z));

	ASSERT(r.hayCamino(A,BC1));
	ASSERT(r.hayCamino(A,BC2));
	ASSERT(r.hayCamino(A,X));

	ASSERT(r.hayCamino(A,C));
	ASSERT(r.hayCamino(A,Y));

	Conj< Camino > caminos = r.caminosMinimos(A, C);
	cout << endl;
	cout << caminos << endl;


	ASSERT(r == r.copiar());

	// waaaaaa no exploto nada?
	//cout << " > Listo!\n";

	//cout << r.dns.Significado(B.ip)->caminos.Significado(J.ip) << '\n';

}


void check_red_caminimos_mini(){
	Red r;
	Compu Y; Y.ip = "Y";
	Y.interfaces.Agregar(1);
	Y.interfaces.Agregar(2);
	r.agregarComputadora(Y);

	Compu X; X.ip = "X";
	X.interfaces.Agregar(1);
	X.interfaces.Agregar(2);
	r.agregarComputadora(X);

	Compu Z; Z.ip = "Z";
	Z.interfaces.Agregar(1);
	Z.interfaces.Agregar(2);
	r.agregarComputadora(Z);

	r.conectar(Y, Z, 1, 2);
	r.conectar(Z, X, 1, 2);
	r.conectar(X, Y, 1, 2);
	Conj< Camino > caminos = r.caminosMinimos(X, Y);
	cout << endl;
	cout << caminos << endl;
	// ASSERT( r.caminosMinimos(X,Y) == r.caminosMinimos(Y,X) );
	// ASSERT( r.caminosMinimos(X,Z) == r.caminosMinimos(Z,X) );
	// ASSERT( r.caminosMinimos(Y,Z) == r.caminosMinimos(Z,Y) );

	ASSERT(r == r.copiar());
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

	// Red
	RUN_TEST(check_red_nueva);
	RUN_TEST(check_red_agregar_compu);
	RUN_TEST(check_red_conectar);
	RUN_TEST(check_red_conecta_vecinos);
	RUN_TEST(check_red_caminimos_mini);
	//RUN_TEST(check_red_caminimos_huge);
	
	return 0;
}
