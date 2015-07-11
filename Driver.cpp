#include "Driver.h"

namespace aed2 {

Driver::Driver() {

}

Driver::~Driver() {
    // TODO
}

// TAD RED
Nat Driver::CantidadComputadoras() const {

    return topologia.Computadoras().Cardinal();

}

const Computadora& Driver::IesimaComputadora(const Nat i) const {
    Conj<Compu>::const_Iterador it = topologia.Computadoras().CrearIt();
    Nat j = 0;
    while(j < i){
        it.Avanzar();
        j++;
    }
	return it.Siguiente().ip;
}

Nat Driver::CantidadInterfacesDe(const Computadora& c) const {
    Conj<Compu>::const_Iterador it = topologia.Computadoras().CrearIt();
     while(c != it.Siguiente().ip) {
        it.Avanzar();
    }
    return it.Siguiente().interfaces.Cardinal();

}

const Interfaz& Driver::IesimaInterfazDe(const Computadora& c, const Nat i) const{
    Conj<Compu>::const_Iterador itCompus = topologia.Computadoras().CrearIt();

    while(c != itCompus.Siguiente().ip) {
        itCompus.Avanzar();
    }

    Conj<Interfaz> interfaces = itCompus.Siguiente().interfaces;
    Conj<Interfaz>::const_Iterador itInterfs = interfaces.CrearIt();
    cout << "Interfaces: " << interfaces <<endl;
    Nat j = 0;
    while(j < i){
        itInterfs.Avanzar();
        j++;
    }

    return itInterfs.Siguiente();
}

Interfaz Driver::IntefazUsada(const Computadora& c1, const Computadora& c2) const {
    Conj<Compu>::const_Iterador it = topologia.Computadoras().CrearIt();
     while(c1 != it.Siguiente().ip) {
        it.Avanzar();
    }
    Compu compu1 = it.Siguiente();

    it = topologia.Computadoras().CrearIt();
     while(c2 != it.Siguiente().ip) {
        it.Avanzar();
    }
    Compu compu2 = it.Siguiente();

    return topologia.InterfazUsada(compu1, compu2);
}

bool Driver::conectadas(const Computadora& c1, const Computadora& c2) const {

    return topologia.Conectadas(c1,c2);
}

// TAD DCNET
void Driver::AgregarComputadora(const Computadora& ip, const Conj<Interfaz>& ci) {
    Compu c;
    c.ip = ip;
    c.interfaces = ci;
    topologia.AgregarComputadora(c);

    dcNet = DCNet(topologia);
}

void Driver::Conectar(const Computadora& c1, const Interfaz& i1, const Computadora& c2, const Interfaz& i2) {

    Conj<Compu>::const_Iterador it = topologia.Computadoras().CrearIt();
     while(c1 != it.Siguiente().ip) {
        it.Avanzar();
    }
    Compu compu1 = it.Siguiente();

    it = topologia.Computadoras().CrearIt();
     while(c2 != it.Siguiente().ip) {
        it.Avanzar();
    }
    Compu compu2 = it.Siguiente();

    topologia.Conectar(compu1, compu2, i1, i2);

    dcNet = DCNet(topologia);
}


Nat Driver::CantidadNodosRecorridosPor(const Paquete& p) const {

    Conj<Compu>::const_Iterador itCompus = topologia.Computadoras().CrearIt();
    while(itCompus.HaySiguiente()){
        Conj< ::Paquete>::const_Iterador itPaqs = dcNet.EnEspera(itCompus.Siguiente()).CrearIt();
        while(itPaqs.HaySiguiente()){
            if(p == itPaqs.Siguiente().id) {
                return dcNet.CaminoRecorrido(itPaqs.Siguiente()).Longitud();
            }
            itPaqs.Avanzar();
        }
        itCompus.Avanzar();
    }

}

const Computadora& Driver::IesimoNodoRecorridoPor(const Paquete& p, const Nat i) const {

    Conj<Compu>::const_Iterador itCompus = topologia.Computadoras().CrearIt();
    while(itCompus.HaySiguiente()){
        Conj< ::Paquete>::const_Iterador itPaqs = dcNet.EnEspera(itCompus.Siguiente()).CrearIt();
        while(itPaqs.HaySiguiente()){
            if(p == itPaqs.Siguiente().id) {
                return dcNet.CaminoRecorrido(itPaqs.Siguiente())[i].ip;
            }
            itPaqs.Avanzar();
        }
        itCompus.Avanzar();
    }
}

Nat Driver::CantidadEnviadosPor(const Computadora& c) const {

    return dcNet.CantidadEnviados(c);
}

Nat Driver::CantidadEnEsperaEn(const Computadora& c) const {

    return dcNet.EnEspera(c).Cardinal();
}

const Paquete& Driver::IesimoEnEsperaEn(const Computadora& c, const Nat i) const {
    Conj< ::Paquete>::const_Iterador it = dcNet.EnEspera(c).CrearIt();

    Nat j = 0;
    while(j < i){
        it.Avanzar();
        j++;
    }

    return it.Siguiente().id;
}

void Driver::CrearPaquete(const Computadora& origen, const Computadora& destino, Nat prioridad) {

    Nat maxId = 0;
    Compu cOrigen;
    Compu cDestino;

    Conj<Compu>::const_Iterador itCompus = topologia.Computadoras().CrearIt();

    while(itCompus.HaySiguiente()){
        if (origen == itCompus.Siguiente().ip) {
            cOrigen = itCompus.Siguiente();
        }

        if (destino == itCompus.Siguiente().ip) {
            cDestino = itCompus.Siguiente();
        }

        Conj< ::Paquete>::const_Iterador itPaqs = dcNet.EnEspera(itCompus.Siguiente()).CrearIt();
        while(itPaqs.HaySiguiente()){
            if(maxId <= itPaqs.Siguiente().id) {
                maxId = itPaqs.Siguiente().id;
            }
            itPaqs.Avanzar();
        }
        itCompus.Avanzar();
    }

    ::Paquete p;
    p.origen = cOrigen;
    p.destino = cDestino;
    p.prioridad = prioridad;
    p.id = maxId + 1;

    dcNet.CrearPaquete(p);
}

void Driver::AvanzarSegundo() {
    dcNet.AvanzarSegundo();
}

const Computadora& Driver::laQueMasEnvio() const {

    return dcNet.LaQueMasEnvio().ip;
}

const Computadora& Driver::origen(const Paquete& p) const {
    Conj<Compu>::const_Iterador itCompus = topologia.Computadoras().CrearIt();
    while(itCompus.HaySiguiente()){
        Conj< ::Paquete>::const_Iterador itPaqs = dcNet.EnEspera(itCompus.Siguiente()).CrearIt();
        while(itPaqs.HaySiguiente()){
            if(p == itPaqs.Siguiente().id) {
                return itPaqs.Siguiente().origen.ip;
            }
            itPaqs.Avanzar();
        }
        itCompus.Avanzar();
    }
}

const Computadora& Driver::destino(const Paquete& p) const {
    Conj<Compu>::const_Iterador itCompus = topologia.Computadoras().CrearIt();
    while(itCompus.HaySiguiente()){
        Conj< ::Paquete>::const_Iterador itPaqs = dcNet.EnEspera(itCompus.Siguiente()).CrearIt();
        while(itPaqs.HaySiguiente()){
            if(p == itPaqs.Siguiente().id) {
                return itPaqs.Siguiente().destino.ip;
            }
            itPaqs.Avanzar();
        }
        itCompus.Avanzar();
    }
}

Nat Driver::prioridad(const Paquete& p) const {
    Nat res;
    Conj<Compu>::const_Iterador itCompus = topologia.Computadoras().CrearIt();


    while(itCompus.HaySiguiente()){
        Conj< ::Paquete>::const_Iterador itBuffer = dcNet.EnEspera(itCompus.Siguiente()).CrearIt();
        while(itBuffer.HaySiguiente()){
            if(p == itBuffer.Siguiente().id)
                res = itBuffer.Siguiente().prioridad;
            itBuffer.Avanzar();
        }
        itCompus.Avanzar();
    }

    return res;
}

} // namespace aed2
