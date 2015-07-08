#include "DCNet.h"

DCNet::DCNet(const Red r){
    topologia = r;
    vectorCompusDCNet = Vector();
    diccCompusDCNet = DiccString();
    conjPaquetesDCNet = Conj();
    laQueMasEnvio = NULL;

    Conj<compu>::Iterador it = topologia.Computadoras().CrearIt();
    if(it.HaySiguiente())
        laQueMasEnvio = *it.Siguiente();

    while(it.HaySiguiente()){
        CompuDCNet compudcnet;
        compudcnet.pc = *it.Siguiente();
        compudcnet.conjPaquetes = Conj();
        compudcnet.diccPaquetesDCNet = DiccLog();
        compudcnet.colaPaquetesDCNet = ColaPrioridad();
        compudcnet.paqueteAEnviar = Conj().CrearIt();
        compudcnet.enviados = 0;

        vectorCompusDCNet.AgregarAtras(compudcnet);

        diccCompusDCNet.Definir(it.Siguiente().ip, *compudcnet);

        it.Avanzar();
    }
}

void DCNet::CrearPaquete(const Paquete p){
    CompuDCNet *compudcnet = diccCompusDCNet.Significado(p.origen.ip);
    Conj<Paquete>::Iterador itPaq = compudcnet->conjPaquetes.AgregarRapido(p);
    Lista<Compu> recorr = Lista().AgregarAtras(p.origen);

    PaqueteDCNet paqDCNet;
    paqDCNet.it = itPaq;
    paqDCNet.recorrido = recorr;

    Conj<PaqueteDCNet>::Iterador itPaqDCNet = conjPaquetesDCNet.AgregarRapido(paqDCNet);

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
                laQueMasEnvio = *vectorCompusDCNet[i];
            }

            Paquete pAEnviar = vectorCompusDCNet[i].paqueteAEnviar.Siguiente().it.Siguiente();
            Conj<Lista<Compu> >::Iterador iterCaminos = topologia.CaminosMinimos(*vectorCompusDCNet[i].pc, pAEnviar.destino).CrearIt();
            Compu siguienteCompu = iterCaminos.Siguiente()[1];

            if(pAEnviar.destino != siguienteCompu){
                
                CompuDCNet siguienteCompuDCNet = *(diccCompusDCNet.Obtener(siguienteCompu.ip));
                Conj<Paquete>::Iterador itPaquete = siguienteCompuDCNet.conjPaquetes.AgregarRapido(pAEnviar);
                Conj<PaqueteDCNet>::Iterador itPaqAEnviar = vectorCompusDCNet[i].diccPaquetesDCNet.Obtener(pAEnviar.id);

                itPaqDCNet.Siguiente().recorrido.AgregarAtras(siguienteCompu);
                siguienteCompuDCNet.colaPaquetesDCNet.Encolar(itPaqAEnviar);
                siguienteCompuDCNet.diccPaquetesDCNet.Definir(pAEnviar.id, itPaqAEnviar);
            }

            vectorCompusDCNet[i].diccPaquetesDCNet.Borrar(vectorCompusDCNet[i].paqueteAEnviar.Siguiente().it.Siguiente().id);
            vectorCompusDCNet[i].paqueteAEnviar.Siguiente().it.EliminarSiguiente();
            vectorCompusDCNet[i].paqueteAEnviar.EliminarSiguiente();
            vectorCompusDCNet[i].paqueteAEnviar = Conj().CrearIt();
        }
        i++;
    }
}

Red DCNet::Red() const{
    return(topologia);
}

Lista<Compu> DCNet::CaminoRecorrido(const Paquete p) const{
    Nat i = 0;
    while(i < vectorCompusDCNet.Longitud()){
        if(vectorCompusDCNet[i].diccPaquetesDCNet.Definido(p.id))
            return(vectorCompusDCNet[i].diccPaquetesDCNet.Obtener(p.id).Siguiente().recorrido);
        i++;
    }
}

Nat DCNet::CantidadEnviados(const Compu c) const{
    return(diccCompusDCNet.Obtener(c.ip)->enviados);
}

Nat DCNet::EnEspera(const Compu c) const{
    return(diccCompusDCNet.Obtener(c.ip)->conjPaquetes);   
}

bool DCNet::PaqueteEnTransito(const Paquete p) const{
    bool enTransito = false;
    Nat i = 0;
    while(i < vectorCompusDCNet.Longitud()){
        if(vectorCompusDCNet[i].diccPaquetesDCNet.Definido(p.id))
            enTransito = true;
        i++;
    }
    return(enTransito);
}

Compu DCNet::LaQueMasEnvio() const{
    return(laQueMasEnvio->pc);
}

bool DCNet::operator == (const DCNet& otra) const{
    bool boolTopo = (topologia == otra.topologia);
    bool boolVec = (vectorCompusDCNet == otra.vectorCompusDCNet);
    bool boolConj = (conjPaquetesDCNet == otra.conjPaquetesDCNet);
    bool boolMasEnvio = (*laQueMasEnvio == *(otra.laQueMasEnvio));
    return(boolTopo && boolVec && boolConj && boolMasEnvio);
}

bool DCNet::CompuDCNet::operator == (const DCNet::CompuDCNet& otra) const{
    bool boolPC = (*pc == *(otra.pc));
    bool boolConj = (conjPaquetes == otra.conjPaquetes);
    bool boolAVL = true;
    bool boolCola = true;
    bool boolPaq = (paqueteAEnviar.Siguiente() == otra.paqueteAEnviar.Siguiente());
    bool boolEnviados = (enviados == otra.enviados);

    if(boolConj){
        Conj<Paquete>::Iterador itConj = conjPaquetes.CrearIt();
        while(itConj.HaySiguiente()){
            if(otra.diccPaquetesDCNet.Definido(itConj.Siguiente().id)){
                if(!(diccPaquetesDCNet.Obtener(itConj.Siguiente().id).Siguiente() == otra.diccPaquetesDCNet.Obtener(itConj.Siguiente().id).Siguiente())){
                    boolAVL = false;
            }
            else
                boolAVL = false;
            itConj.Avanzar();
        }
    }

    if(colaPaquetesDCNet.EsVacia())
        if(!otra.colaPaquetesDCNet.EsVacia())
            boolCola = false;
    else
        if(!(colaPaquetesDCNet.Proximo().Siguiente() == otra.colaPaquetesDCNet.Proximo().Siguiente()))
            boolCola = false;

    return(boolPC && boolConj && boolAVL && boolCola && boolPaq && boolEnviados);
}

bool DCNet::PaqueteDCNet::operator == (const PaqueteDCNet& otro) const{
    return((it.Siguiente() == otro.it.Siguiente()) && (recorrido == otro.recorrido));
}

bool DCNet::Paquete::operator == (const Paquete& otro) const{
    bool boolID = (id == otro.id);
    bool boolPrior = (prioridad == otro.prioridad);
    bool boolOrigen = (origen == otro.origen);
    bool boolDestino = (destino == otro.destino);
    return(boolID && boolPrior && boolOrigen && boolDestino);
}

bool DCNet::Compu::operator == (const Compu& otra) const{
    return((ip == otra.ip) && (interfaces == otra.interfaces));
}