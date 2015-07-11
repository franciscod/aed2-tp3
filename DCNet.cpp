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

        //cout << compudcnet.pc.ip << endl;

        vectorCompusDCNet.AgregarAtras(compudcnet);

        diccCompusDCNet.definir(it.Siguiente().ip, &vectorCompusDCNet.Ultimo());

        if (laQueMasEnvio == NULL) {
            laQueMasEnvio = &vectorCompusDCNet.Ultimo();
        }

        it.Avanzar();
    }
}

void DCNet::CrearPaquete(const ::Paquete& p){
    CompuDCNet *compudcnet = diccCompusDCNet.obtener(p.origen.ip);
    Conj< ::Paquete>::Iterador itPaq = compudcnet->conjPaquetes.AgregarRapido(p);

    /*
    cout << "ESTAS ROTO?" << endl;
    cout << compudcnet->pc.ip << endl;
    cout << "ESTAS ROTO?" << endl;
    return;
    */

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
