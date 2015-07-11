#ifndef RED_H_
#define RED_H_

#include "aed2/Conj.h"
#include "aed2/dicc_trie.h"
#include "Tipos.h"

using namespace aed2;

typedef Lista<Compu> Camino;

struct NodoRed {
	// esto originalmente era un puntero, para que apuntase a la compu dentro de compus (el conj de arriba)
	// por principios de abstraccion/encapsulamiento/modularidad no sentido que sea un
	// puntero a algo (eventualmente, desconocido) dentro del tipo Conj: tiene más sentido guardar la Compu completa acá

	Compu pc;
	DiccString < Conj<Camino> > caminos;
	Dicc <Interfaz, NodoRed*> conexiones;
	NodoRed() {};
	NodoRed(const Compu& c) : pc(c) {};
	NodoRed(const NodoRed& n){
		pc = n.pc;
		caminos = n.caminos;
		conexiones = n.conexiones;
	}

	bool operator==(const NodoRed& otro) const {
		// OJO, esto no chequea conexiones
		return (pc == otro.pc) && (caminos == otro.caminos);
	};

};



inline std::ostream& operator << (std::ostream& os, const NodoRed& nr) {
	return os << "[[Nodo con IP " << nr.pc.ip << "]]";
};



class Red{
private:
	Conj<Compu> compus;
	DiccString <NodoRed> dns;

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

	const Conj<Compu>& Computadoras() const;

	bool Conectadas(const Compu& c1, const Compu& c2) const;

	Interfaz InterfazUsada( const Compu& c1, const Compu& c2) const;

	Conj<Compu> Vecinos( const Compu& c) const;

	bool UsaInterfaz( const Compu& c, const int i) const;

	const Conj< Camino >& CaminosMinimos( const Compu& c1, const Compu& c2) const;

	bool HayCamino( const Compu& c1, const Compu& c2) const;

	bool operator==(const Red&) const;

	Red& operator=(const Red& otro);
};

#endif
