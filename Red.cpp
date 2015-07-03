#include "Red.h"

using namespace std;
using namespace aed2;

Red::Red(){
	//compus = Conj<Compu>();
	//dns = DiccString<NodoRed>();
}

Conj<Compu> Red::computadoras(){
	return compus;
}

void Red::agregarComputadora(const Compu& c){
	// PRE: c no está en la red

	DiccString<NodoRed>::Iterador it(&dns);

	compus.AgregarRapido(c);

	while(it.avanzar()) {
		NodoRed nr = *it.valorActual();
		nr.caminos.definir(c.ip, Conj<Camino>());
	}

	NodoRed nr(c);

	nr.caminos.definir(c.ip, Conj<Camino>());

	dns.definir(c.ip, nr);
}

void Red::conectar(const Compu& c1, const Compu& c2, const int i1, const int i2){
	// PRE: las interfaces de esas compus existen y estan libres

	NodoRed* n1 = dns.obtener(c1.ip);
	NodoRed* n2 = dns.obtener(c2.ip);
	n1->conexiones.Definir(i1 , n2);
	n2->conexiones.Definir(i2 , n1);

	//TODO: CrearTodosLosCaminos( r);
}

bool Red::usaInterfaz( const Compu& c, const int i){
	return dns.obtener(c.ip)->conexiones.Definido(i);
}

int Red::interfazUsada(const Compu& c1, const Compu& c2){
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

bool Red::conectadas?( const Compu& c1, const Compu& c2){
	//TODO: esto esta bien? no tiene que chequear que haya algún camino?
	bool res = false;
	Iterador it =  Iterador(r.dns.obtener(c1.getIP()).getConexiones());
	while(it.HaySiguiente()){
		if c1.ip == (*(it.SiguienteSignificado().getPC()).getIP) {
			res = true
		}
		it.Avanzar();
	}
	return
}

Conj <Compu> Red::vecinos( const Compu& c){
	Conj <Compu> res = new Conj<Compu>();
	Iterador it =  Iterador(r.dns.obtener(c.getIP()).getConexiones());
	while(it.HaySiguiente()){
		res.AgregarRapido(*it.SiguienteSignificado().getPC());
		it.Avanzar();
	}
	return res;
}

Conj< Lista < Compu > >  Red::caminosMinimos( const Compu& c1, const Compu& c2){
	return r.dns.obtener(c1.getIP()).getCaminos().obtener(c2.getIP());
}

bool Red::hayCamino( const Compu& c1, const Compu& c2){
	return !EsVacio(r.dns.obtener(c1.getIP()).getCaminos().obtener(c2.getIP()));
}

Red Red::copiar(){
	Red res = new Red();
	// copia el conjunto de tuplas
	res.compus = new Conj(r.compus);
	// rearma los nodos (con conexiones en blanco) del diccionario dns
	Iterador it = Iterador(r.getCompus());
	while(it.HaySiguiente()){
		Compu c = it.Siguiente();
		NodoRed nodoAux = r.dns.obtener(c.ip);
		res.dns.definir (  c. ip , NodoRed(&c , new DiccString < Conj< Lista<Compu> > >(nodoAux.caminos) , new Dicc < int, *NodoRed() >() );
		it.Avanzar();
	}

	// rearma las conexiones
	 it = Iterador(r.getCompus());
	 while(it.HaySiguiente()){

	 	Compu c = it.Siguiente();
	 	NodoRed nodoMio = res.dns.obtener(c.ip);
	 	NodoRed nodoOtro r.dns.obtener(c.ip);
	 	Iterador itInterfs = Iterador(nodoMio.pc->interfaces);

	 	while(itInterfs.HaySiguiente()){

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
