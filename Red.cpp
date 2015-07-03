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
	DiccString<NodoRed>::Iterador it(&dns);
	while(it.avanzar()) {
		NodoRed* nrt = it.valorActual();
		nrt->caminos.definir(nr.pc.ip, Conj<Camino>());
		nr.caminos.definir(nrt->pc.ip, Conj<Camino>());
	}

	dns.definir(c.ip, nr);
}

void Red::conectar (const Compu& c1, const Compu& c2, const int i1, const int i2) {
	// PRE: las interfaces de esas compus existen y estan libres
	cout << "conectar\n";

	NodoRed* n1 = dns.obtener(c1.ip);
	NodoRed* n2 = dns.obtener(c2.ip);
	n1->conexiones.Definir(i1, n2);
	n2->conexiones.Definir(i2, n1);

	CrearTodosLosCaminos();
}



void Red::CrearTodosLosCaminos () {
	Conj<Compu>::Iterador itCompuA = compus.CrearIt();

	while (itCompuA.HaySiguiente()) {
		NodoRed* nr = dns.obtener(itCompuA.Siguiente().ip);

		Conj<Compu>::Iterador itCompuB = compus.CrearIt();

		while (itCompuB.HaySiguiente()) {
			Computadora ipDestino = itCompuB.Siguiente().ip;

			Conj<Camino> caminimos = Caminos(*nr, ipDestino);
			nr->caminos.definir(ipDestino, caminimos);

			itCompuB.Avanzar();
		}

		itCompuA.Avanzar();
	}
}

Conj<Camino> Red::Caminos (const NodoRed& c1, const Computadora& ipDestino) {
	Conj<Camino> res;

	Pila< Lista<NodoRed> > frameRecorrido;  // originalmente era {ila< Lista<Compu> >
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
	return dns.obtener(c.ip)->conexiones.Definido(i);
}

int Red::interfazUsada (const Compu& c1, const Compu& c2) {
	// PRE: c2 esta conectada a alguna interfaz de c1

	NodoRed* n1 = dns.obtener(c1.ip);

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

/*

bool Red::conectadas?( const Compu& c1, const Compu& c2) {
	//TODO: esto esta bien? no tiene que chequear que haya algún camino?
	bool res = false;
	Iterador it =  Iterador(r.dns.obtener(c1.getIP()).getConexiones());
	while(it.HaySiguiente()) {
		if c1.ip == (*(it.SiguienteSignificado().getPC()).getIP) {
			res = true
		}
		it.Avanzar();
	}
	return
}

Conj <Compu> Red::vecinos( const Compu& c) {
	Conj <Compu> res = new Conj<Compu>();
	Iterador it =  Iterador(r.dns.obtener(c.getIP()).getConexiones());
	while(it.HaySiguiente()) {
		res.AgregarRapido(*it.SiguienteSignificado().getPC());
		it.Avanzar();
	}
	return res;
}

Conj< Lista < Compu > >  Red::caminosMinimos( const Compu& c1, const Compu& c2) {
	return r.dns.obtener(c1.getIP()).getCaminos().obtener(c2.getIP());
}

bool Red::hayCamino( const Compu& c1, const Compu& c2) {
	return !EsVacio(r.dns.obtener(c1.getIP()).getCaminos().obtener(c2.getIP()));
}

Red Red::copiar() {
	Red res = new Red();
	// copia el conjunto de tuplas
	res.compus = new Conj(r.compus);
	// rearma los nodos (con conexiones en blanco) del diccionario dns
	Iterador it = Iterador(r.getCompus());
	while(it.HaySiguiente()) {
		Compu c = it.Siguiente();
		NodoRed nodoAux = r.dns.obtener(c.ip);
		res.dns.definir (  c. ip , NodoRed(&c , new DiccString < Conj< Lista<Compu> > >(nodoAux.caminos) , new Dicc < int, *NodoRed() >() );
		it.Avanzar();
	}

	// rearma las conexiones
	 it = Iterador(r.getCompus());
	 while(it.HaySiguiente()) {

	 	Compu c = it.Siguiente();
	 	NodoRed nodoMio = res.dns.obtener(c.ip);
	 	NodoRed nodoOtro r.dns.obtener(c.ip);
	 	Iterador itInterfs = Iterador(nodoMio.pc->interfaces);

	 	while(itInterfs.HaySiguiente()) {

	 		Nat interf = itInterfs.Siguiente();
	 		String ip = nodoOtro.conexiones.obtener(interf);
	 		nodoMio.conexiones.Definir(interf, &r.dns.obtener(ip));
			itInterfs.Avanzar();
	 	}
	 	it.Avanzar();
	 }
}

bool Red::operator==(const Red&) const{
	return false
}
*/
