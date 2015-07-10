#ifndef RED_H_
#define RED_H_

#include "aed2/Conj.h"
#include "Tipos.h"

using namespace aed2;

typedef Lista<Compu> Camino;

struct NodoRed {
	// esto originalmente era un puntero, para que apuntase a la compu dentro de compus (el conj de arriba)
	// por principios de abstraccion/encapsulamiento/modularidad no sentido que sea un
	// puntero a algo (eventualmente, desconocido) dentro del tipo Conj: tiene más sentido guardar la Compu completa acá

	Compu pc;
	Dicc <String , Conj<Camino> > caminos;
	Dicc <Interfaz, NodoRed*> conexiones;
	NodoRed() {};
	NodoRed(const Compu& c) : pc(c) {};

	bool operator==(const NodoRed& otro) const{
		return (pc == otro.pc);
	};

};



inline std::ostream& operator << (std::ostream& os, const NodoRed& nr) {
	return os << "[[Nodo con IP " << nr.pc.ip << "]]";
};



class Red{
private:
	Conj<Compu> compus;
	Dicc <String, NodoRed> dns;

	void crearTodosLoscaminos();
	Conj<Camino> caminos(const NodoRed& c1, const Computadora& ipDestino);
	Lista<Compu> compusDeNodos (const Lista<NodoRed>& ns);
	Conj<Camino> minimos(const Conj<Camino>& caminos);
	Lista<NodoRed> agregarListaNodosVecinos(const NodoRed& n, const Lista<NodoRed>& original);
	bool nodoEnLista(const NodoRed& n, const Lista<NodoRed>& ns);

public:
	Red();

	Red (const Red& otro);

	void AgregarComputadora( const Compu& c);

	void Conectar(const Compu& c1, const Compu&c2, const int i1, const  int i2);

	Conj<Compu>& Computadoras();

	bool Conectadas(const Compu& c1, const Compu& c2);

	int InterfazUsada( const Compu& c1, const Compu& c2);

	Conj<Compu> Vecinos( const Compu& c);

	bool UsaInterfaz( const Compu& c, const int i);

	Conj< Camino > CaminosMinimos( const Compu& c1, const Compu& c2);

	bool HayCamino( const Compu& c1, const Compu& c2);

	bool operator==(const Red&) const;

	Red& operator=(const Red& otro);

	Conj<Interfaz> Interfaces(const Compu& c);
};

Conj<Camino> agregarCompuAlPrimerCaminoDeConjuntoDeCaminos(Compu c, Conj<Camino> cc);

#endif
