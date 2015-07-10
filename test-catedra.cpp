#include "Driver.h"
#include "mini_test.h"
#include "aed2/Lista.h"
#include "aed2/Conj.h"
#include "aed2/Dicc.h"

#include <string>
#include <iostream>

using namespace aed2;

/** 
 * Imprime un elemento a un string, en vez de a una pantalla, 
 * a través del operador << 
 */
template <typename T>
std::string to_str(const T& t)
{
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
bool Comparar(const T& t, const S& s)
{
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

	while( it1.HaySiguiente() )
	{
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

void test_conectar_computadoras() {
	Driver dcnet;
	Computadora c1 = "skynet";
	Computadora c2 = "terminator";
	Computadora c3 = "connor";

	Interfaz i1 = 1;
	Interfaz i2 = 2;
	Interfaz i3 = 3;
	Interfaz i4 = 4;
	Interfaz i5 = 5;

	Conj<Interfaz> conjIc1;
	Conj<Interfaz> conjIc2;
	Conj<Interfaz> conjIc3;

	conjIc1.Agregar(i1);
	
	conjIc2.Agregar(i2);
	conjIc2.Agregar(i5);
	
	conjIc3.Agregar(i4);
	conjIc3.Agregar(i3);
	
	dcnet.AgregarComputadora(c1,conjIc1);
	dcnet.AgregarComputadora(c2,conjIc2);
	dcnet.AgregarComputadora(c3,conjIc3);
	
	dcnet.Conectar(c1, i1,c2, i2);	
	dcnet.Conectar(c2, i5,c3, i3);	

	ASSERT_EQ(dcnet.conectadas(c1,c2), true);
	ASSERT(dcnet.IntefazUsada(c1,c2) == i1);
	ASSERT(dcnet.IntefazUsada(c2,c1) == i2);
	
	ASSERT_EQ(dcnet.conectadas(c2,c3), true);
	ASSERT(dcnet.IntefazUsada(c2,c3) == i5);
	ASSERT(dcnet.IntefazUsada(c3,c2) == i3);

	ASSERT_EQ(dcnet.conectadas(c1,c3), false);
	
	ASSERT_EQ(dcnet.CantidadInterfacesDe(c1), 1);
	ASSERT_EQ(dcnet.CantidadInterfacesDe(c2), 2);
	ASSERT_EQ(dcnet.CantidadInterfacesDe(c3), 2);
	
}

void test_crear_paquetes() {
	Driver dcnet;
	Computadora c1 = "skynet";
	Computadora c2 = "terminator";
	Computadora c3 = "connor";

	Interfaz i1 = 1;
	Interfaz i2 = 2;
	Interfaz i3 = 3;
	Interfaz i4 = 4;
	Interfaz i5 = 5;

	Conj<Interfaz> conjIc1;
	Conj<Interfaz> conjIc2;
	Conj<Interfaz> conjIc3;

	conjIc1.Agregar(i1);
	
	conjIc2.Agregar(i2);
	conjIc2.Agregar(i5);
	
	conjIc3.Agregar(i4);
	conjIc3.Agregar(i3);
	
	dcnet.AgregarComputadora(c1,conjIc1);
	dcnet.AgregarComputadora(c2,conjIc2);
	dcnet.AgregarComputadora(c3,conjIc3);
	
	dcnet.Conectar(c1, i1,c2, i2);	
	dcnet.Conectar(c2, i5,c3, i3);	

	dcnet.CrearPaquete(c1,c2,2);
	dcnet.CrearPaquete(c1,c2,1);
	dcnet.CrearPaquete(c2,c3,3);
	dcnet.CrearPaquete(c2,c3,2);
	dcnet.CrearPaquete(c2,c3,1);
	dcnet.CrearPaquete(c2,c3,1);
	
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c1), 2);
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c2), 4);
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c3), 0);	
	
	dcnet.AvanzarSegundo();
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c1), 1);
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c2), 3);
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c3), 0);	
	
	ASSERT_EQ(dcnet.CantidadEnviadosPor(c1), 1);
	ASSERT_EQ(dcnet.CantidadEnviadosPor(c2), 1);
	ASSERT_EQ(dcnet.CantidadEnviadosPor(c3), 0);
	
}

void test_respeta_priodades() {
	Driver dcnet;
	Computadora c1 = "skynet";
	Computadora c2 = "sky";

	Interfaz i1 = 1;
	Interfaz i2 = 2;

	Conj<Interfaz> conjIc1;
	Conj<Interfaz> conjIc2;

	conjIc1.Agregar(i1);
	
	conjIc2.Agregar(i2);
	
	dcnet.AgregarComputadora(c1,conjIc1);
	dcnet.AgregarComputadora(c2,conjIc2);
	
	dcnet.Conectar(c1, i1,c2, i2);	

	dcnet.CrearPaquete(c1,c2,2);
	dcnet.CrearPaquete(c1,c2,1);
	dcnet.CrearPaquete(c1,c2,3);

	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c1), 3);

	dcnet.AvanzarSegundo();
	
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c1), 2);
	ASSERT_EQ(dcnet.CantidadEnviadosPor(c1), 1);

	bool respetaPrioridad = true;
	for (int i =0; i < dcnet.CantidadEnEsperaEn(c1) ; i++) { 
		Nat pr = dcnet.prioridad(dcnet.IesimoEnEsperaEn(c1, i));
		if (pr == 1) {
			respetaPrioridad = false;
		}
	}

	ASSERT_EQ(respetaPrioridad, true);

}

void test_recorrido_simple() {
	Driver dcnet;
	Computadora c1 = "skynet";
	Computadora c2 = "terminator";
	Computadora c3 = "connor";
	Computadora c4 = "sky";

	Interfaz i1 = 1;
	Interfaz i2 = 2;
	Interfaz i3 = 3;
	Interfaz i4 = 4;
	Interfaz i5 = 5;
	Interfaz i6 = 6;
	
	Conj<Interfaz> conjIc1;
	Conj<Interfaz> conjIc2;
	Conj<Interfaz> conjIc3;
	Conj<Interfaz> conjIc4;

	conjIc1.Agregar(i1);
	
	conjIc2.Agregar(i2);
	conjIc2.Agregar(i5);
	
	conjIc3.Agregar(i4);
	conjIc3.Agregar(i3);

	conjIc4.Agregar(i6);
	
	dcnet.AgregarComputadora(c1,conjIc1);
	dcnet.AgregarComputadora(c2,conjIc2);
	dcnet.AgregarComputadora(c3,conjIc3);
	dcnet.AgregarComputadora(c4,conjIc4);
	
	dcnet.Conectar(c1, i1,c2, i2);	
	dcnet.Conectar(c2, i5,c3, i3);
	dcnet.Conectar(c4, i6,c3, i4);

	dcnet.CrearPaquete(c1,c4,1);
	Nat paq = dcnet.IesimoEnEsperaEn(c1,0);
	
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c1), 1);
	
	dcnet.AvanzarSegundo();
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c2), 1);

	dcnet.AvanzarSegundo();
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c3), 1);

	
	ASSERT_EQ(dcnet.CantidadNodosRecorridosPor(paq),3);
	ASSERT(dcnet.IesimoNodoRecorridoPor(paq,0) == c1);
	ASSERT(dcnet.IesimoNodoRecorridoPor(paq,1) == c2);
	ASSERT(dcnet.IesimoNodoRecorridoPor(paq,2) == c3);
	
	
}

void test_recorrido_dos_minimos() {
	Driver dcnet;
	Computadora c1 = "skynet";
	Computadora c2 = "terminator";
	Computadora c3 = "connor";
	Computadora c4 = "sky";

	Interfaz i1 = 1;
	Interfaz i2 = 2;
	Interfaz i3 = 3;
	Interfaz i4 = 4;
	Interfaz i5 = 5;
	Interfaz i6 = 6;
	Interfaz i7 = 7;
	Interfaz i8 = 8;
	
	Conj<Interfaz> conjIc1;
	Conj<Interfaz> conjIc2;
	Conj<Interfaz> conjIc3;
	Conj<Interfaz> conjIc4;


	conjIc1.Agregar(i1);
	conjIc1.Agregar(i2);
	
	conjIc2.Agregar(i3);
	conjIc2.Agregar(i4);
	
	conjIc3.Agregar(i5);
	conjIc3.Agregar(i6);

	conjIc4.Agregar(i7);
	conjIc4.Agregar(i8);

	
	dcnet.AgregarComputadora(c1,conjIc1);
	dcnet.AgregarComputadora(c2,conjIc2);
	dcnet.AgregarComputadora(c3,conjIc3);
	dcnet.AgregarComputadora(c4,conjIc4);	
	
	dcnet.Conectar(c1, i1,c2, i3);	
	dcnet.Conectar(c1, i2,c3, i5);

	dcnet.Conectar(c2, i4,c4, i7);

	dcnet.Conectar(c3, i6,c4, i8);


	dcnet.CrearPaquete(c1,c4,1);
	Nat paq = dcnet.IesimoEnEsperaEn(c1,0);
	
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c1), 1);
	
	dcnet.AvanzarSegundo();
	
	// Puede ir por c2 o c3 , tiene que estar en alguno de los dos
	ASSERT_EQ(dcnet.CantidadEnEsperaEn(c2)+dcnet.CantidadEnEsperaEn(c3), 1);

	
	ASSERT_EQ(dcnet.CantidadNodosRecorridosPor(paq),2);
	ASSERT(dcnet.IesimoNodoRecorridoPor(paq,0) == c1);
	ASSERT(dcnet.IesimoNodoRecorridoPor(paq,1) == c2 || dcnet.IesimoNodoRecorridoPor(paq,1) == c3 );

}
 
void test_recorrido_complejo() {
	Driver dcnet;
	Computadora c1 = "skynet";
	Computadora c2 = "terminator";
	Computadora c3 = "connor";
	Computadora c4 = "sky";
	Computadora c5 = "sarah";

	Interfaz i1 = 1;
	Interfaz i2 = 2;
	Interfaz i3 = 3;
	Interfaz i4 = 4;
	Interfaz i5 = 5;
	Interfaz i6 = 6;
	Interfaz i7 = 7;
	Interfaz i8 = 8;
	Interfaz i9 = 9;
	Interfaz i10 = 10;
	Interfaz i11 = 11;
	Interfaz i12 = 12;
	
	Conj<Interfaz> conjIc1;
	Conj<Interfaz> conjIc2;
	Conj<Interfaz> conjIc3;
	Conj<Interfaz> conjIc4;
	Conj<Interfaz> conjIc5;

	conjIc1.Agregar(i1);
	conjIc1.Agregar(i2);
	
	conjIc2.Agregar(i3);
	conjIc2.Agregar(i4);
	
	conjIc3.Agregar(i5);
	conjIc3.Agregar(i6);
	conjIc3.Agregar(i7);

	conjIc4.Agregar(i8);
	conjIc4.Agregar(i9);
	conjIc4.Agregar(i10);

	conjIc5.Agregar(i11);
	conjIc5.Agregar(i12);

	
	dcnet.AgregarComputadora(c1,conjIc1);
	dcnet.AgregarComputadora(c2,conjIc2);
	dcnet.AgregarComputadora(c3,conjIc3);
	dcnet.AgregarComputadora(c4,conjIc4);
	dcnet.AgregarComputadora(c5,conjIc5);
	
	dcnet.Conectar(c1, i1,c2, i3);	
	dcnet.Conectar(c1, i2,c3, i5);

	dcnet.Conectar(c2, i4,c4, i8);

	dcnet.Conectar(c3, i6,c4, i9);
	dcnet.Conectar(c3, i7,c5, i11);

	dcnet.Conectar(c4, i10,c5, i12);

	dcnet.CrearPaquete(c1,c5,1);
	Nat paq = dcnet.IesimoEnEsperaEn(c1,0);
	
	dcnet.AvanzarSegundo();

	ASSERT_EQ(dcnet.CantidadNodosRecorridosPor(paq),2);
	ASSERT(dcnet.IesimoNodoRecorridoPor(paq,0) == c1);
	ASSERT(dcnet.IesimoNodoRecorridoPor(paq,1) == c3);
}

void test_la_que_mas_envio() {
	Driver dcnet;
	Computadora c1 = "skynet";
	Computadora c2 = "terminator";
	Computadora c3 = "connor";

	Interfaz i1 = 1;
	Interfaz i2 = 2;
	Interfaz i3 = 3;
	Interfaz i4 = 4;

	Conj<Interfaz> conjIc1;
	Conj<Interfaz> conjIc2;
	Conj<Interfaz> conjIc3;

	conjIc1.Agregar(i1);
	
	conjIc2.Agregar(i2);
	conjIc2.Agregar(i3);
	
	conjIc3.Agregar(i4);

	dcnet.AgregarComputadora(c1,conjIc1);
	dcnet.AgregarComputadora(c2,conjIc2);
	dcnet.AgregarComputadora(c3,conjIc3);

	// c1 -- c2 -- c3	
	dcnet.Conectar(c1, i1,c2, i2);	
	dcnet.Conectar(c2, i3,c3, i4);

	dcnet.CrearPaquete(c1,c3,1);
	dcnet.CrearPaquete(c2,c3,1);
	
	dcnet.AvanzarSegundo();
	dcnet.AvanzarSegundo();

	ASSERT_EQ(dcnet.laQueMasEnvio(), c2);
	dcnet.CrearPaquete(c1,c2,1);
	dcnet.CrearPaquete(c1,c2,1);
	dcnet.AvanzarSegundo();
	dcnet.AvanzarSegundo();

	ASSERT_EQ(dcnet.laQueMasEnvio(), c1);

}


// ---------------------------------------------------------------------

/**
 * Ejemplo de caso de test, con llamadas a las rutinas de aserción 
 * definidas en mini_test.h
 */
 
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
	
	Computadora c1 = "dcubaar";
	Computadora c2 = "ubaar";
	Computadora c3 = "dmubaar";
	
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

	ASSERT_EQ(dcnet.laQueMasEnvio(), c1);
		
}


int main(int argc, char **argv)
{
    RUN_TEST(test_dcnet_ejemplo);
    RUN_TEST(test_conectar_computadoras);
    RUN_TEST(test_crear_paquetes);
    RUN_TEST(test_respeta_priodades);
    RUN_TEST(test_recorrido_simple);
	RUN_TEST(test_recorrido_dos_minimos);
	RUN_TEST(test_recorrido_complejo);
	RUN_TEST(test_la_que_mas_envio);

	return 0;
}
