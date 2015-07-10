#include "DCNet.h"

#include "Tipos.h"
using namespace aed2;

DCNet::DCNet(const class Red& r){
    topologia = r;
    // estas cosas se inicializan solas
    //vectorCompusDCNet = Vector();
    //diccCompusDCNet = DiccString();
    //conjPaquetesDCNet = Conj();
    laQueMasEnvio = NULL;

    Conj<Compu>::Iterador it = topologia.Computadoras().CrearIt();
    /*
    no tiene sentido asignar una Compu a laQueMasEnvio (tipo CompuDCNet*)
    if(it.HaySiguiente())
        laQueMasEnvio = it.Siguiente();
*/
    while(it.HaySiguiente()){
        CompuDCNet compudcnet;
        compudcnet.pc = it.Siguiente();
        /*
        de todo esto se encarga el constructor defautl de cpp
        compudcnet.conjPaquetes = Conj();
        compudcnet.diccPaquetesDCNet = DiccLog();
        compudcnet.colaPaquetesDCNet = ColaPrioridad();
        compudcnet.paqueteAEnviar = Conj().CrearIt();
        compudcnet.enviados = 0;
        */

        vectorCompusDCNet.AgregarAtras(compudcnet);

        diccCompusDCNet.definir(it.Siguiente().ip, &vectorCompusDCNet.Ultimo());

        it.Avanzar();
    }
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
            Conj<Lista<Compu> >::Iterador iterCaminos = topologia.CaminosMinimos(vectorCompusDCNet[i].pc, pAEnviar.destino).CrearIt();
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

Conj< ::Paquete> DCNet::EnEspera(const Compu& c) const{
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

Compu DCNet::LaQueMasEnvio() const{
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
