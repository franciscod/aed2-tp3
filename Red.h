#ifndef RED_H_
#define RED_H_

#include "aed2/dicc_trie.h"
#include "aed2/Conj.h"
#include "Tipos.h"

using namespace aed2;

typedef Lista<Compu> Camino;

class Red{
private:
	Conj<Compu> compus;

	struct NodoRed {
		// esto originalmente era un puntero, para que apuntase a la compu dentro de compus (el conj de arriba)
		// por principios de abstraccion/encapsulamiento/modularidad no sentido que sea un
		// puntero a algo (eventualmente, desconocido) dentro del tipo Conj: tiene más sentido guardar la Compu completa acá
		NodoRed(){
			// Compu pc;
			// DiccString< Conj<Camino> > caminos;
			// Dicc <Interfaz, NodoRed*> conexiones;
		};

		bool operator==(const NodoRed& otro) const{
			return (pc == otro.pc);
		};

		std::ostream& operator<<(std::ostream& os){
		  return os << pc.ip;
		}

		Compu pc;

		Dicc <String , Conj<Camino> > caminos;
		Dicc <Interfaz, NodoRed*> conexiones;

		NodoRed(const Compu& c) : pc(c) {};
		NodoRed(const NodoRed& o) : pc(o.pc) {};
	};

	Dicc <String, NodoRed> dns;

	void CrearTodosLosCaminos();
	Conj<Camino> Caminos(const NodoRed& c1, const Computadora& ipDestino);
	Lista<Compu> compusDeNodos (const Lista<NodoRed>& ns);
	Conj<Camino> Minimos(const Conj<Camino>& caminos);
	Lista<NodoRed> listaNodosVecinos(const NodoRed& n);
	bool nodoEnLista(const NodoRed& n, const Lista<NodoRed>& ns);

public:
	Red();

	void agregarComputadora( const Compu& c);

	void conectar(const Compu& c1, const Compu&c2, const int i1, const  int i2);

	Conj<Compu> computadoras();

	bool conectadas(const Compu& c1, const Compu& c2);

	int interfazUsada( const Compu& c1, const Compu& c2);

	Conj<Compu> vecinos( const Compu& c);

	bool usaInterfaz( const Compu& c, const int i);

	Conj< Camino > caminosMinimos( const Compu& c1, const Compu& c2);

	bool hayCamino( const Compu& c1, const Compu& c2);

	Red copiar();

	bool operator==(const Red&) const;
};

#endif
