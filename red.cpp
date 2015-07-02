#include "Red.h"

Red::Red(){
	compus = Conj();
	dns = DiccString();
}

void Red::agregarComputadora( const Compu& c){
	
	Iterador it =  Iterador(r.dns);
	while{
		it.valorActual().definir(it.valorActual().getCaminos(), c.getIP(), new DiccString < Conj< Lista<Compu> > >() );
	}do(it.Avanzar());

	r.compus.AgregarRapido(c&);
	r.dns.definir (  c. ip , NodoRed(&c , new DiccString < Conj< Lista<Compu> > >() ,Dicc < int, *NodoRed() > );
	inicializarConjCaminos(r , c);

}

void Red::inicializarConjCaminos( const Compu& c){
	Iterador it = Iterador(r.getCompus());
	while(it.HaySiguiente()){
		r.dns.obtener(c.getIP()).getCaminos().Definir(it.Siguiente().getIP(), new DiccString < Conj< Lista<Compu> > >());
		it.Avanzar();
	}
}

void Red::conectar(const Compu& c1, const Compu& c2, const int i1, const int i2){
	r.dns.obtener(c1.getIP()).getConexiones().definir(i1 , r.dns.obtener(c2.getIP()));
	r.dns.obtener(c2.getIP()).getConexiones().definir(i2 , r.dns.obtener(c1.getIP()));
	CrearTodosLosCaminos( r);
}

Conj<Compu> Red::computadoras(){
	return r.compus;
}

bool Red::conectadas?( const Compu& c1, const Compu& c2){
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

int Red::interfazUsada(const Compu& c1, const Compu& c2){
	int res ;
	Iterador it =  Iterador(r.dns.obtener(c1.getIP()).getConexiones());
	while(it.HaySiguiente()){
		if c1.ip == (*(it.SiguienteSignificado()getPC()).getIP) {
			res = it.SiguienteClave();
		}
	}
	return res;
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

bool Red::usaInterfaz( const Compu& c, const int i){
	return r.dns.obtener(c.getIP()).Definido(i);
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