#include "Red.h"
#include "Pila.h"

using namespace std;
using namespace aed2;

Red::Red () {
	//compus = Conj<Compu>();
	//dns = DiccString<NodoRed>();
}

Conj<Compu> Red::computadoras () {
	return compus;
}

void Red::agregarComputadora (const Compu& c) {
	// PRE: c no está en la red

	NodoRed nr(c);

	compus.AgregarRapido(c);

	// inicializarConjCaminos esta embebida en este loop
	Dicc<String, NodoRed>::Iterador it = dns.CrearIt();
	while(it.HaySiguiente()) {
		NodoRed* nrt = &it.SiguienteSignificado();
		nrt->caminos.Definir(nr.pc.ip, Conj<Camino>());
		nr.caminos.Definir(nrt->pc.ip, Conj<Camino>());
		it.Avanzar();
	}

	dns.Definir(c.ip, nr);
}

void Red::conectar (const Compu& c1, const Compu& c2, const int i1, const int i2) {
	// PRE: las interfaces de esas compus existen y estan libres

	NodoRed* n1 = &dns.Significado(c1.ip);
	NodoRed* n2 = &dns.Significado(c2.ip);
	n1->conexiones.Definir(i1, n2);
	n2->conexiones.Definir(i2, n1);

	CrearTodosLosCaminos();
}



void Red::CrearTodosLosCaminos () {
	Conj<Compu>::Iterador itCompuA = compus.CrearIt();

	while (itCompuA.HaySiguiente()) {
		NodoRed* nr = &dns.Significado(itCompuA.Siguiente().ip);

		Conj<Compu>::Iterador itCompuB = compus.CrearIt();

		while (itCompuB.HaySiguiente()) {
			Computadora ipDestino = itCompuB.Siguiente().ip;

			Conj<Camino> caminimos = Caminos(*nr, ipDestino);
			nr->caminos.Definir(ipDestino, caminimos);

			itCompuB.Avanzar();
		}

		itCompuA.Avanzar();
	}
}

Conj<Camino> Red::Caminos (const NodoRed& c1, const Computadora& ipDestino) {
	Conj<Camino> res;

	Pila< Lista<NodoRed> > frameRecorrido;  // originalmente era Pila< Lista<Compu> >
	Pila< Lista<NodoRed> > frameCandidatos;

	Lista<NodoRed> iCandidatos = listaNodosVecinos(c1);
	Lista<NodoRed> iRecorrido; // originalmente era Lista<Compu>
	iRecorrido.AgregarAdelante(c1.pc);

	frameRecorrido.Apilar(iRecorrido);
	frameCandidatos.Apilar(iCandidatos);

	// Compu pCandidatos; movido abajo para simplificar.
	Lista<NodoRed> fCandidatos;

	while(!frameRecorrido.EsVacia()) {
		iRecorrido = frameRecorrido.Tope();
		iCandidatos = frameCandidatos.Tope();

		frameRecorrido.Desapilar();
		frameCandidatos.Desapilar();

		if (!iCandidatos.EsVacia()) {
			// esta linea estaba afuera del if
			NodoRed pCandidatos = iCandidatos.Primero(); // originalmente era Compu

			iCandidatos.Fin();
			fCandidatos = iCandidatos;

			if (iRecorrido.Ultimo().pc.ip == ipDestino) {
				res.AgregarRapido(compusDeNodos(iRecorrido)); // originalmente no habia compusDeNodos
			} else {
				frameRecorrido.Apilar(iRecorrido);
				frameCandidatos.Apilar(fCandidatos);

				if (!nodoEnLista(pCandidatos, iRecorrido)) {
					// iRecorrido = Copiar(iRecorrido)
					// no hace falta porque se apiló por copia
					iRecorrido.AgregarAtras(pCandidatos);
					frameRecorrido.Apilar(iRecorrido);
					frameCandidatos.Apilar(listaNodosVecinos(pCandidatos));
				}
			}
		}
	}

	return res;
}

// originalmetnte no existia
Lista<Compu> Red::compusDeNodos (const Lista<NodoRed>& ns) {
	Lista<Compu> res;
	Lista<NodoRed>::const_Iterador itNodos = ns.CrearIt();
	while (itNodos.HaySiguiente()) {
		res.AgregarAtras(itNodos.Siguiente().pc);
		itNodos.Avanzar();
	}
	return res;
}

Conj<Camino> Red::Minimos (const Conj<Camino>& caminos) {
	Conj<Camino> res;
	Nat longMinima;
	Conj<Camino>::const_Iterador itCaminos = caminos.CrearIt(); // agregado const_
	if (itCaminos.HaySiguiente()) {
		longMinima = itCaminos.Siguiente().Longitud();
		itCaminos.Avanzar();
		while(itCaminos.HaySiguiente()) {
			if (itCaminos.Siguiente().Longitud() < longMinima) {
				longMinima = itCaminos.Siguiente().Longitud();
			}
			itCaminos.Avanzar();
		}
		itCaminos = caminos.CrearIt();
		while(itCaminos.HaySiguiente()) {
			if (itCaminos.Siguiente().Longitud() == longMinima) {
				res.AgregarRapido(itCaminos.Siguiente());
			}
			itCaminos.Avanzar();
		}
	}
	return res;
}

Lista<Red::NodoRed> Red::listaNodosVecinos (const NodoRed& n) {
	Lista<Red::NodoRed> res;
	Dicc<Interfaz, NodoRed*>::const_Iterador itVecinos = n.conexiones.CrearIt(); // agregado const_
	while (itVecinos.HaySiguiente()) {
		res.AgregarAdelante(*itVecinos.SiguienteSignificado());
		itVecinos.Avanzar();
	}
	return res;
}

bool Red::nodoEnLista (const NodoRed& n, const Lista<NodoRed>& ns) {
	Lista<NodoRed>::const_Iterador itNodos = ns.CrearIt(); // agregado el const_
	while (itNodos.HaySiguiente()) {
		if (&itNodos.Siguiente() == &n) { // agregados & para chequear que sean el mismo nodo
			return true;
		};
		itNodos.Avanzar();
	}
	return false;
}


bool Red::usaInterfaz( const Compu& c, const int i) {
	return dns.Significado(c.ip).conexiones.Definido(i);
}

int Red::interfazUsada (const Compu& c1, const Compu& c2) {
	// PRE: c2 esta conectada a alguna interfaz de c1

	NodoRed* n1 = &dns.Significado(c1.ip);

	Dicc <Interfaz, NodoRed*>::Iterador it = n1->conexiones.CrearIt();

	while (it.HaySiguiente()) {
		NodoRed* n2 = it.SiguienteSignificado();
		if (c2.ip == n2->pc.ip) {
			return it.SiguienteClave();
		}
		it.Avanzar();
	}

	return -1; // esto no deberia alcanzarse por la PRE
}

Conj<Compu> Red::vecinos (const Compu& c) {
	// PRE: c esta en la red
	Conj<Compu> res;
	Dicc<Interfaz, NodoRed*>::const_Iterador it = dns.Significado(c.ip).conexiones.CrearIt();
	while(it.HaySiguiente()) {
		res.AgregarRapido(it.SiguienteSignificado()->pc);
		it.Avanzar();
	}
	return res;
}
/*

bool Red::conectadas?( const Compu& c1, const Compu& c2) {
	//TODO: esto esta bien? no tiene que chequear que haya algún camino?
	bool res = false;
	Iterador it =  Iterador(r.dns.Significado(c1.getIP()).getConexiones());
	while(it.HaySiguiente()) {
		if c1.ip == (*(it.SiguienteSignificado().getPC()).getIP) {
			res = true
		}
		it.Avanzar();
	}
	return
}


Conj< Lista < Compu > >  Red::caminosMinimos( const Compu& c1, const Compu& c2) {
	return r.dns.Significado(c1.getIP()).getCaminos().Significado(c2.getIP());
}

bool Red::hayCamino( const Compu& c1, const Compu& c2) {
	return !EsVacio(r.dns.Significado(c1.getIP()).getCaminos().Significado(c2.getIP()));
}
*/

Red Red::copiar() {
	Red res;
	// copia el conjunto de tuplas
	res.compus = compus;
	// rearma los nodos (con conexiones en blanco) del diccionario dns
	Conj<Compu>::Iterador it = compus.CrearIt();
	while(it.HaySiguiente()) {
		Compu c = it.Siguiente();
		NodoRed* nr = &dns.Significado(c.ip);
		Dicc<String, Conj<Camino> > auxCaminos = nr->caminos;
		Dicc <Interfaz, NodoRed* > auxConexiones;
		NodoRed auxNr;
		auxNr.pc = c;
		auxNr.caminos = auxCaminos;
		auxNr.conexiones = auxConexiones;
		res.dns.Definir(c.ip, auxNr);
		it.Avanzar();
	}
	
	// rearma las conexiones
	it = compus.CrearIt();
	while(it.HaySiguiente()) {
	 	Compu c = it.Siguiente();
	 	NodoRed nodoMio = res.dns.Significado(c.ip);
	 	NodoRed nodoOtro = dns.Significado(c.ip);

	 	Dicc <Interfaz, NodoRed*>::Iterador itInterfs = nodoMio.conexiones.CrearIt();

	 	while(itInterfs.HaySiguiente()) {

	 		Interfaz interf = itInterfs.SiguienteClave();
	 		String ip = nodoOtro.conexiones.Significado(interf)->pc.ip;
	 		nodoMio.conexiones.Definir(interf, &res.dns.Significado(ip));
			itInterfs.Avanzar();
	 	}
	 	it.Avanzar();
	}
	
	return res;
}

bool Red::operator==(const Red& otro) const{
	return ( (otro.dns == dns) && (otro.compus == compus)  ) ;
}

