#include "Red.h"

Red::Red(){
	compus = Conj();
	dns = DiccString();
}

Red::agregarComputadora(Red r,  const Compu c){
	
	Iterador it =  Iterador(r.dns);
	while{
		it.valorActual().definir(it.valorActual().getCaminos(), c.getIP(), DiccString < Conj< Lista<Compu> > >() );
	}do(it.avanzar());

	r.compus.AgregarRapido(c&);
	r.dns.definir (  c. ip , NodoRed(&c , DiccString < Conj< Lista<Compu> > >() ,Dicc < int, *NodoRed() > );
	inicializarConjCaminos(r , c);

}

Red::inicializarConjCaminos(Red r,  const Compu c){
	Iterador it = Iterador(r.getCompus())
	while(it.HaySiguiente()){
		r.getCaminos().Definir(it.Siguiente().getIP(), DiccString < Conj< Lista<Compu> > >());
		it.avanzar();
	}
}


