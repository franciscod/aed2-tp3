#include "Red.h"

Red::Red(){
	compus = Conj();
	dns = DiccString();
}

void Red::agregarComputadora(Red r,  const Compu c){
	
	Iterador it =  Iterador(r.dns);
	while{
		it.valorActual().definir(it.valorActual().getCaminos(), c.getIP(), new DiccString < Conj< Lista<Compu> > >() );
	}do(it.avanzar());

	r.compus.AgregarRapido(c&);
	r.dns.definir (  c. ip , NodoRed(&c , new DiccString < Conj< Lista<Compu> > >() ,Dicc < int, *NodoRed() > );
	inicializarConjCaminos(r , c);

}

void Red::inicializarConjCaminos(Red r,  const Compu c){
	Iterador it = Iterador(r.getCompus());
	while(it.HaySiguiente()){
		r.dns.obtener(c.getIP()).getCaminos().Definir(it.Siguiente().getIP(), new DiccString < Conj< Lista<Compu> > >());
		it.avanzar();
	}
}

void Red::conectar(Red r, const Compu c1, const Compu c2, const int i1, const int i2){
	r.dns.obtener(c1.getIP()).getConexiones().definir(i1 , r.dns.obtener(c2.getIP()));
	r.dns.obtener(c2.getIP()).getConexiones().definir(i2 , r.dns.obtener(c1.getIP()));
	CrearTodosLosCaminos( r);
}

Conj<Compu> Red::computadoras(const Red r){
	return r.compus;
}

bool Red::conectadas?(const Red r, const Compu c1, const Compu c2){
	bool res = false;
	Iterador it =  Iterador(r.dns.obtener(c1.getIP()).getConexiones());
	while(it.HaySiguiente()){
		if c1.ip == (*(it.SiguienteSignificado()getPC()).getIP) {
			res = true
		}
		it.avanzar();
	}
	return 
}

int Red::interfazUsada(const Red r, const Compu c1, const Compu c2){
	int res ;
	Iterador it =  Iterador(r.dns.obtener(c1.getIP()).getConexiones());
	while(it.HaySiguiente()){
		if c1.ip == (*(it.SiguienteSignificado()getPC()).getIP) {
			res = it.SiguienteClave();
		}
	}
	return res;
}

Conj <Compu> Red::vecinos(const Red r, const Compu c){
	Conj <Compu> res = new Conj<Compu>();
	Iterador it =  Iterador(r.dns.obtener(c.getIP()).getConexiones());
	while(it.HaySiguiente()){
		res.AgregarRapido(*it.SiguienteSignificado().getPC());
		it.avanzar();
	}
	return res;
}

bool Red::usaInterfaz(const Red r, const Compu c, const int i){
	return r.dns.obtener(c.getIP()).Definido(i);
}

Conj< Lista < Compu > >  Red::caminosMinimos(const Red r, const Compu c1, const Compu c2){
	return r.dns.obtener(c1.getIP()).getCaminos().obtener(c2.getIP());

}
bool Red::hayCamino(const Red r, const Compu c1, const Compu c2){
	return !EsVacio(r.dns.obtener(c1.getIP()).getCaminos().obtener(c2.getIP()));
}