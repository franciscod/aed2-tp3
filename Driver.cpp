#include "Driver.h"

namespace aed2 {

Driver::Driver() {}

Driver::~Driver() {}

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
    return compuConIp(c).interfaces.Cardinal();

}

const Interfaz& Driver::IesimaInterfazDe(const Computadora& c, const Nat i) const{

    Conj<Interfaz> interfaces = compuConIp(c).interfaces;
    Conj<Interfaz>::const_Iterador itInterfs = interfaces.CrearIt();

    Nat j = 0;
    while(j < i){
        itInterfs.Avanzar();
        j++;
    }

    return itInterfs.Siguiente();
}

Interfaz Driver::IntefazUsada(const Computadora& c1, const Computadora& c2) const {
    return topologia.InterfazUsada(compuConIp(c1), compuConIp(c2));
}

bool Driver::conectadas(const Computadora& c1, const Computadora& c2) const {
    return topologia.Conectadas(compuConIp(c1), compuConIp(c2));
}

void Driver::AgregarComputadora(const Computadora& ip, const Conj<Interfaz>& ci) {
    Compu c;
    c.ip = ip;
    c.interfaces = ci;
    topologia.AgregarComputadora(c);

    dcNet = DCNet(topologia);
}

void Driver::Conectar(const Computadora& c1, const Interfaz& i1, const Computadora& c2, const Interfaz& i2) {
    topologia.Conectar(compuConIp(c1), compuConIp(c2), i1, i2);

    dcNet = DCNet(topologia);
}


Nat Driver::CantidadNodosRecorridosPor(const Paquete& p) const {
    const ::Paquete& paq = paqueteConId(p);
    return dcNet.CaminoRecorrido(paq).Longitud();
}

const Computadora& Driver::IesimoNodoRecorridoPor(const Paquete& p, const Nat i) const {
    const ::Paquete& paq = paqueteConId(p);
    const Camino& cam = dcNet.CaminoRecorrido(paq);
    return cam[i].ip;
}

Nat Driver::CantidadEnviadosPor(const Computadora& c) const {
    return dcNet.CantidadEnviados(compuConIp(c));
}

Nat Driver::CantidadEnEsperaEn(const Computadora& c) const {
    return dcNet.EnEspera(compuConIp(c)).Cardinal();
}

const Paquete& Driver::IesimoEnEsperaEn(const Computadora& c, const Nat i) const {
    Conj< ::Paquete>::const_Iterador it = dcNet.EnEspera(compuConIp(c)).CrearIt();

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
    return paqueteConId(p).origen.ip;
}

const Computadora& Driver::destino(const Paquete& p) const {
    return paqueteConId(p).destino.ip;
}

Nat Driver::prioridad(const Paquete& p) const {
    return paqueteConId(p).prioridad;
}


const Compu& Driver::compuConIp(aed2::Computadora ip) const {
    // PRE: la compu con esa ip está en el dcNet
    Conj<Compu>::const_Iterador it = topologia.Computadoras().CrearIt();
     while(ip != it.Siguiente().ip) {
        it.Avanzar();
    }
    return it.Siguiente();
}

const ::Paquete& Driver::paqueteConId(aed2::Paquete id) const {
    // PRE: el paquete con ese id está en el dcNet

    Conj<Compu>::const_Iterador itCompus = topologia.Computadoras().CrearIt();

    while(itCompus.HaySiguiente()) {
        Conj< ::Paquete>::const_Iterador itPaqs = dcNet.EnEspera(itCompus.Siguiente()).CrearIt();
        while(itPaqs.HaySiguiente()) {
            if(id == itPaqs.Siguiente().id) {
                return itPaqs.Siguiente();
            }
            itPaqs.Avanzar();
        }
        itCompus.Avanzar();
    }

    cout << "ESTO NO DEBERIA PASAR POR LA PRECONDICION!" << endl;
    assert(false);
}


} // namespace aed2
