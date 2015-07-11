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

        cout << compudcnet.pc.ip << endl;

        vectorCompusDCNet.AgregarAtras(compudcnet);

        diccCompusDCNet.definir(it.Siguiente().ip, &vectorCompusDCNet.Ultimo());

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

    cout << "ESTAS ROTO?" << endl;
    cout << compudcnet->pc.ip << endl;
    cout << "ESTAS ROTO?" << endl;
    return;


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
    Nat maxEnviados = 0;
    Nat i = 0;
    while(i < vectorCompusDCNet.Longitud()){
        if(!vectorCompusDCNet[i].colaPaquetesDCNet.EsVacia()){
            vectorCompusDCNet[i].paqueteAEnviar = vectorCompusDCNet[i].colaPaquetesDCNet.Desencolar();
        }
        i++;
    }

    i = 0;
    while(i < vectorCompusDCNet.Longitud()){
        if(vectorCompusDCNet[i].paqueteAEnviar.HaySiguiente()){

            vectorCompusDCNet[i].enviados++;
            if(vectorCompusDCNet[i].enviados > maxEnviados){
                laQueMasEnvio = &vectorCompusDCNet[i];
            }

            ::Paquete pAEnviar = vectorCompusDCNet[i].paqueteAEnviar.Siguiente().it.Siguiente();
            Conj<Lista<Compu> >::const_Iterador iterCaminos = topologia.CaminosMinimos(vectorCompusDCNet[i].pc, pAEnviar.destino).CrearIt();
            Compu siguienteCompu = iterCaminos.Siguiente()[1];

            if(pAEnviar.destino != siguienteCompu){

                CompuDCNet siguienteCompuDCNet = *(diccCompusDCNet.obtener(siguienteCompu.ip));
                siguienteCompuDCNet.conjPaquetes.AgregarRapido(pAEnviar);
                Lista<PaqueteDCNet>::Iterador itPaqAEnviar = vectorCompusDCNet[i].diccPaquetesDCNet.Obtener(pAEnviar.id);

                itPaqAEnviar.Siguiente().recorrido.AgregarAtras(siguienteCompu);
                siguienteCompuDCNet.colaPaquetesDCNet.Encolar(pAEnviar.prioridad, itPaqAEnviar);
                siguienteCompuDCNet.diccPaquetesDCNet.Definir(pAEnviar.id, itPaqAEnviar);
            }

            vectorCompusDCNet[i].diccPaquetesDCNet.Borrar(vectorCompusDCNet[i].paqueteAEnviar.Siguiente().it.Siguiente().id);
            vectorCompusDCNet[i].paqueteAEnviar.Siguiente().it.EliminarSiguiente();
            vectorCompusDCNet[i].paqueteAEnviar.EliminarSiguiente();
            vectorCompusDCNet[i].paqueteAEnviar = Lista<PaqueteDCNet>().CrearIt();
        }
        i++;
    }
}

Red DCNet::Red() const{
    return topologia;
}

Lista<Compu> DCNet::CaminoRecorrido(const ::Paquete& p) const{
    Nat i = 0;
    Lista<Compu> res;
    while(i < vectorCompusDCNet.Longitud()){
        if(vectorCompusDCNet[i].diccPaquetesDCNet.Definido(p.id)) {
            res = vectorCompusDCNet[i].diccPaquetesDCNet.Obtener(p.id).Siguiente().recorrido;
            break;
        }
        i++;
    }

    return res;
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
    bool boolTopo = (topologia == otra.topologia);
    bool boolVec = (vectorCompusDCNet == otra.vectorCompusDCNet);
    bool boolConj = (listaPaquetesDCNet == otra.listaPaquetesDCNet);
    bool boolMasEnvio = (*laQueMasEnvio == *(otra.laQueMasEnvio));
    return(boolTopo && boolVec && boolConj && boolMasEnvio);
}

inline bool DCNet::CompuDCNet::operator == (const DCNet::CompuDCNet& otra) const{
    bool boolPC = (pc == (otra.pc));
    bool boolConj = (conjPaquetes == otra.conjPaquetes);
    bool boolAVL = true;
    bool boolCola = true;
    bool boolPaq = (paqueteAEnviar.Siguiente() == otra.paqueteAEnviar.Siguiente());
    bool boolEnviados = (enviados == otra.enviados);

    if(boolConj){
        Conj< ::Paquete>::const_Iterador itConj = conjPaquetes.CrearIt();
        while(itConj.HaySiguiente()){
            if(otra.diccPaquetesDCNet.Definido(itConj.Siguiente().id)){
                if(!(diccPaquetesDCNet.Obtener(itConj.Siguiente().id).Siguiente() == otra.diccPaquetesDCNet.Obtener(itConj.Siguiente().id).Siguiente())){
                    boolAVL = false;
                }
            }
            else
                boolAVL = false;
            itConj.Avanzar();
        }
    }

    if(colaPaquetesDCNet.EsVacia()) {
        if(!otra.colaPaquetesDCNet.EsVacia()) {
            boolCola = false;
        }
    } else {
        if(!(colaPaquetesDCNet.Proximo().Siguiente() == otra.colaPaquetesDCNet.Proximo().Siguiente())){
            boolCola = false;
        }
    }

    return boolPC && boolConj && boolAVL && boolCola && boolPaq && boolEnviados;
}

inline bool DCNet::CompuDCNet::operator != (const DCNet::CompuDCNet& otra) const{
    return !(*this==otra);
}
