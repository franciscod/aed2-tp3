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
	topologia = otro.topologia;
	// limpio todo
	vectorCompusDCNet = Vector<CompuDCNet>();
	diccCompusDCNet = DiccString<CompuDCNet*>();
	listaPaquetesDCNet = Lista<PaqueteDCNet>();
	laQueMasEnvio = NULL;

	//regenero vector mas o menos, y diccCompusDCNet

	for(Nat i = 0; i < otro.vectorCompusDCNet.Longitud(); i++){
		CompuDCNet otraCDN = otro.vectorCompusDCNet[i];
		CompuDCNet nuevaCDN(otraCDN.pc);
		nuevaCDN.conjPaquetes = otraCDN.conjPaquetes;
		nuevaCDN.enviados = otraCDN.enviados;
		//nuevaCDN.diccPaquetesDCNet pendiente
		//nuevaCDN.colaPaquetesDCNet pendiente

		vectorCompusDCNet.AgregarAtras(nuevaCDN);
		diccCompusDCNet.definir(nuevaCDN.pc.ip, &vectorCompusDCNet.Ultimo());

		if(otro.laQueMasEnvio != NULL && *otro.laQueMasEnvio == otraCDN){
			laQueMasEnvio = &vectorCompusDCNet[i];
		}
	}
	// Acá tengo laQueMasEnvio, topología, diccCompusDCNet y vectorCompusDCNet (mas o menos)

	// Genero lista PaquetesDCNet
	Lista<PaqueteDCNet>::const_Iterador itPDCNet = otro.listaPaquetesDCNet.CrearIt();
	while(itPDCNet.HaySiguiente()){
		PaqueteDCNet otroPaqDCN = itPDCNet.Siguiente();
		// paquetedcnet es un iterador al paquete en el conjunto de paquetes de
		// la compu ultima del recorrido.

		PaqueteDCNet pdcn;
		pdcn.recorrido = otroPaqDCN.recorrido;
		Computadora& ultimaDelRecorrido = pdcn.recorrido.Ultimo().ip;

		// conseguimos el iterador al conjunto de la compu
		Conj< ::Paquete>::Iterador itPaquete = diccCompusDCNet.obtener(ultimaDelRecorrido)->conjPaquetes.CrearIt();
		// nos movemos hasta encontrar el paquete
		while(itPaquete.Siguiente() != otroPaqDCN.it.Siguiente()) {
			itPaquete.Avanzar();
		}
		// itPaquete apunta a donde queremos
		pdcn.it = itPaquete;

		// el PaqueteDCnet está listo, inserto
		listaPaquetesDCNet.AgregarAtras(pdcn);


		itPDCNet.Avanzar();
	}
	// Acá tengo listaPaquetesDCNet, y ya tenia laQueMasEnvio, topología, diccCompusDCNet y vectorCompusDCNet (mas o menos)

	// Ahora que la lista de paquetes esta completa puedo rellenar lo que me falta de las compusdcnet:

		// por cada paquete del conjPaquetes, agregarlo a ambas estructuras:
		// diccPaquetesDCNet
		// colaPaquetesDCNet

	for(unsigned int i = 0; i < vectorCompusDCNet.Longitud(); i++) {
		CompuDCNet& compu = vectorCompusDCNet[i];

		Conj< ::Paquete>::const_Iterador itPaquetes = compu.conjPaquetes.CrearIt();
		while(itPaquetes.HaySiguiente()) {

			::Paquete paq = itPaquetes.Siguiente();
			// Esta es la tupla paquete, necesitamos obtener un iterador al PaqueteDCNet dentro de la lista.

			Lista<PaqueteDCNet>::Iterador itPaqDCN = listaPaquetesDCNet.CrearIt();
			while(itPaqDCN.Siguiente().it.Siguiente() != paq){
				itPaqDCN.Avanzar();
			}
			// Ahora agregamos itPaqDCN al dicc
			compu.diccPaquetesDCNet.Definir(paq.id, itPaqDCN);
			itPaquetes.Avanzar();
		}

		// ahora la cola, en el mismo orden que la original
		ColaPrioridad<Lista<PaqueteDCNet>::Iterador> copiaColaPrioridadOtro(otro.vectorCompusDCNet[i].colaPaquetesDCNet);
		while(!copiaColaPrioridadOtro.EsVacia()){
			Lista<PaqueteDCNet>::Iterador itPaqDCNOtro = copiaColaPrioridadOtro.Proximo();
			::Paquete paq = itPaqDCNOtro.Siguiente().it.Siguiente();

			compu.colaPaquetesDCNet.Encolar(paq.prioridad, compu.diccPaquetesDCNet.Obtener(paq.id));
			copiaColaPrioridadOtro.Desencolar();
		}
	}

	// listo!

	return *this;
}

void DCNet::CrearPaquete( ::Paquete p){
    CompuDCNet *compudcnet = diccCompusDCNet.obtener(p.origen.ip);

    Conj< ::Paquete>::Iterador itPaq = compudcnet->conjPaquetes.AgregarRapido(p);

    Lista<Compu> recorr;
    recorr.AgregarAtras(p.origen);

    PaqueteDCNet paqDCNet;
    paqDCNet.it = itPaq;
    paqDCNet.recorrido = recorr;

    Lista<PaqueteDCNet>::Iterador itPaqDCNet = listaPaquetesDCNet.AgregarAtras(paqDCNet);

    compudcnet->diccPaquetesDCNet.Definir(p.id, itPaqDCNet); // aca rompe


    compudcnet->colaPaquetesDCNet.Encolar(p.prioridad, itPaqDCNet);
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

const Lista<Compu>& DCNet::CaminoRecorrido(const ::Paquete& p) const{
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
