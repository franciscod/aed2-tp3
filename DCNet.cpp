#include "DCNet.h"
#include "Tipos.h"

using namespace aed2;
DCNet::DCNet() {}

DCNet::DCNet(const class Red& r){
    topologia = r;
    laQueMasEnvio = NULL;
    Conj<Compu>::const_Iterador it = topologia.Computadoras().CrearIt();

    while(it.HaySiguiente()){
        CompuDCNet compudcnet(it.Siguiente());

        vectorCompusDCNet.AgregarAtras(compudcnet);

        diccCompusDCNet.definir(it.Siguiente().ip, &vectorCompusDCNet.Ultimo());

        if (laQueMasEnvio == NULL) {
            laQueMasEnvio = &vectorCompusDCNet.Ultimo();
        }

        it.Avanzar();
    }
}

DCNet::DCNet(const DCNet& otro){
	*this = otro;
}

DCNet& DCNet::operator = (const DCNet& otro){
	if(this == &otro){
		return *this;
	}
	// limpio todo 
	vectorCompusDCNet = Vector<CompuDCNet>();
	diccCompusDCNet = DiccString<CompuDCNet*>();
	listaPaquetesDCNet = Lista<PaqueteDCNet>();
	laQueMasEnvio = NULL;

	topologia = otro.topologia;
  Conj<Compu>::const_Iterador it = topologia.Computadoras().CrearIt();

  while(it.HaySiguiente()){
      CompuDCNet compudcnet(it.Siguiente());
			// Copio del otro los campos sin punteros
			const CompuDCNet* compudcnetOtro = otro.diccCompusDCNet.obtener(it.Siguiente().ip);	

			// Copio conjPaquetes y enviados
			compudcnet.conjPaquetes = compudcnetOtro->conjPaquetes;
			compudcnet.enviados = compudcnetOtro->enviados;	

      vectorCompusDCNet.AgregarAtras(compudcnet);
      diccCompusDCNet.definir(it.Siguiente().ip, &vectorCompusDCNet.Ultimo());
      it.Avanzar();
  }
	// Acá tengo vectorCompusDCNet, diccCompusDCNet y topología

	// Genero lista PaquetesDCNet
	Lista<PaqueteDCNet>::const_Iterador itPDCNet = otro.listaPaquetesDCNet.CrearIt();
	while(itPDCNet.HaySiguiente()){
		PaqueteDCNet pdcn;
		pdcn.recorrido = itPDCNet.Siguiente().recorrido;
		
		Conj< ::Paquete>::Iterador itPaquetes = diccCompusDCNet.obtener(pdcn.recorrido.Ultimo().ip)->conjPaquetes.CrearIt();
		while(itPaquetes.Siguiente() != itPDCNet.Siguiente().it.Siguiente()){
			itPaquetes.Avanzar();
		}
		pdcn.it = itPaquetes;
		listaPaquetesDCNet.AgregarAtras(pdcn);

		itPDCNet.Avanzar();
	}
	// Acá tengo PaquetesDCNet copiado
	
	for(unsigned int i = 0; i < vectorCompusDCNet.Longitud(); i++){
		if(otro.laQueMasEnvio != NULL && otro.laQueMasEnvio->pc == vectorCompusDCNet[i].pc){
			laQueMasEnvio = &vectorCompusDCNet[i];
		}

		Conj< ::Paquete>::const_Iterador itPaquetes = vectorCompusDCNet[i].conjPaquetes.CrearIt();
		while(itPaquetes.HaySiguiente()){
			Lista<PaqueteDCNet>::Iterador itPDCNet = listaPaquetesDCNet.CrearIt();
			while(itPDCNet.Siguiente().it.Siguiente() != itPaquetes.Siguiente()){
				itPDCNet.Avanzar();
			}
			vectorCompusDCNet[i].diccPaquetesDCNet.Definir(itPaquetes.Siguiente().id, itPDCNet);
			itPaquetes.Avanzar();	
		}

		// Acá tengo diccPaquetesDCNet
		ColaPrioridad<Lista<PaqueteDCNet>::Iterador> copiaColaPrioridadOtro(otro.vectorCompusDCNet[i].colaPaquetesDCNet);

		while(!copiaColaPrioridadOtro.EsVacia()){
			::Paquete paqueteAEncolar = copiaColaPrioridadOtro.Proximo().Siguiente().it.Siguiente();
			vectorCompusDCNet[i].colaPaquetesDCNet.Encolar(paqueteAEncolar.prioridad, 
																											vectorCompusDCNet[i].diccPaquetesDCNet.Obtener(paqueteAEncolar.id));
			copiaColaPrioridadOtro.Desencolar();
		}
		// Acá tengo colaDePrioridad
	}
	return *this;
}

void DCNet::CrearPaquete(const ::Paquete& p){
    CompuDCNet *compudcnet = diccCompusDCNet.obtener(p.origen.ip);
    Conj< ::Paquete>::Iterador itPaq = compudcnet->conjPaquetes.AgregarRapido(p);

    Lista<Compu> recorr;
    recorr.AgregarAtras(p.origen);

    PaqueteDCNet paqDCNet;
    paqDCNet.it = itPaq;
    paqDCNet.recorrido = recorr;

    Lista<PaqueteDCNet>::Iterador itPaqDCNet = listaPaquetesDCNet.AgregarAtras(paqDCNet);

    (compudcnet->diccPaquetesDCNet).Definir(p.id, itPaqDCNet);
    (compudcnet->colaPaquetesDCNet).Encolar(p.prioridad, itPaqDCNet);
}

void DCNet::AvanzarSegundo(){

    Nat i = 0;
    while(i < vectorCompusDCNet.Longitud()){
        if(!vectorCompusDCNet[i].colaPaquetesDCNet.EsVacia()){
            vectorCompusDCNet[i].paqueteAEnviar = vectorCompusDCNet[i].colaPaquetesDCNet.Desencolar();
        }
        i++;
    }

    i = 0;

    while(i < vectorCompusDCNet.Longitud()){

        if((vectorCompusDCNet[i].paqueteAEnviar.HaySiguiente())){

            CompuDCNet& cDNorigen = vectorCompusDCNet[i];

            cDNorigen.enviados++;
            if(cDNorigen.enviados > laQueMasEnvio->enviados){
                laQueMasEnvio = &cDNorigen;
            }


            Lista<PaqueteDCNet>::Iterador& itPaq = cDNorigen.paqueteAEnviar;
            PaqueteDCNet& paqDN                  = itPaq.Siguiente();
            ::Paquete p                          = paqDN.it.Siguiente();

            Compu siguienteCompu = topologia.CaminosMinimos(cDNorigen.pc, p.destino) // consigue el camino
                .CrearIt().Siguiente() // falso dameUno
                [1]; // primera compu del camino

            cDNorigen.diccPaquetesDCNet.Borrar(p.id);

            if(p.destino != siguienteCompu){ // todavia no esta en destino

                CompuDCNet& cDNdestino = *diccCompusDCNet.obtener(siguienteCompu.ip);

                paqDN.recorrido.AgregarAtras(siguienteCompu);

                paqDN.it.EliminarSiguiente(); // borra el paquete del conjPaquetes de la compu original
                paqDN.it = cDNdestino.conjPaquetes.AgregarRapido(p); // lo inserta en el nuevo y actualiza el iterador
                cDNdestino.diccPaquetesDCNet.Definir(p.id, itPaq);   // lo inserta en el dicclog
                cDNdestino.colaPaquetesDCNet.Encolar(p.prioridad, itPaq); // y lo encola para mandarlo

            } else { // llego a destino
                paqDN.it.EliminarSiguiente(); // borra el paquete del conjPaquetes de la compu original
                itPaq.EliminarSiguiente(); // destruye el elemento de la lista principal de paquetesdcnet

            }

            cDNorigen.paqueteAEnviar = Lista<PaqueteDCNet>().CrearIt(); // falso null a enviar

        }
        i++;
    }
}

Red DCNet::Red() const{
    return topologia;
}

Lista<Compu> DCNet::CaminoRecorrido(const ::Paquete& p) const{
    Nat i = 0;

    while(i < vectorCompusDCNet.Longitud()){
        if(vectorCompusDCNet[i].diccPaquetesDCNet.Definido(p.id)) {
            return vectorCompusDCNet[i].diccPaquetesDCNet.Obtener(p.id).Siguiente().recorrido;
        }
        i++;
    }

    cout << "AAAAAAAAA EXPLOSIONNNNN MUERE TODOOOOO AAAAAAAA" << endl;
    assert(false);

    return Lista<Compu>();
}

Nat DCNet::CantidadEnviados(const Compu& c) const{
    return diccCompusDCNet.obtener(c.ip)->enviados;
}

const Conj< ::Paquete>& DCNet::EnEspera(const Compu& c) const{
    return diccCompusDCNet.obtener(c.ip)->conjPaquetes;
}

bool DCNet::PaqueteEnTransito(const ::Paquete& p) const{
    bool enTransito = false;
    Nat i = 0;
    while(i < vectorCompusDCNet.Longitud()){
        if(vectorCompusDCNet[i].diccPaquetesDCNet.Definido(p.id))
            enTransito = true;
        i++;
    }
    return enTransito;
}

const Compu& DCNet::LaQueMasEnvio() const{
    return laQueMasEnvio->pc;
}


bool DCNet::operator == (const DCNet& otra) const{
    if (!(topologia == otra.topologia)) {
        return false;
    }

    if (!(vectorCompusDCNet == otra.vectorCompusDCNet)) {
        return false;
    }

    // esto asume que estan bien formadas listaPaquetesDCNet y laqueMasEnvio
    // y que las compudcnet vienen bien formadas tambien
    
    return true;
}

inline bool DCNet::CompuDCNet::operator == (const DCNet::CompuDCNet& otra) const{
    if (!(pc == otra.pc)) {
        return false;
    }
    if (!(conjPaquetes == otra.conjPaquetes)) {
        return false;
    }
    if (!(enviados == otra.enviados)) {
        return false;
    }

	// OJO, esto no chequea diccPaquetesDCNet ni colaPaquetesDCNet
    // pero es redundante a la data que hay en conjPaquetes
    // (con mas o menos info para salvar complejidades)

    return true;
}

inline bool DCNet::CompuDCNet::operator != (const DCNet::CompuDCNet& otra) const{
    return !(*this==otra);
}
