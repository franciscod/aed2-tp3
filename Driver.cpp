#include "Driver.h"

namespace aed2 {

Driver::Driver() {
}

Driver::~Driver() {
    // TODO
}

// TAD RED
Nat Driver::CantidadComputadoras() const {

    return dcNet.red().Computadoras().Cardinal();

}

const Computadora& Driver::IesimaComputadora(const Nat i) const {
    Conj<Compu>::Iterador it = topologia.Computadoras().CrearIt();
    int j = 0
    while(j < i){
        it.Avanzar();
        j++;
    }
	return it.Siguiente();
}

Nat Driver::CantidadInterfacesDe(const Computadora& c) const {
    Conj<Compu>::Iterador it = topologia.Computadoras().CrearIt();
     while(c != it.Siguiente()){
        it.Avanzar();
    }
    return topologia.Interfaces(it.Siguiente()).Cardinal();

}

const Interfaz& Driver::IesimaInterfazDe(const Computadora& c, const Nat i) const{
    Conj<Compu>::Iterador itCompus= topologia.Computadoras().CrearIt();
    while(c != itCompus.Siguiente()){
        itCompus.Avanzar();
    }
    Conj<Interfaz> interfaces = dcNet.red().Interfaces(it.Siguiente());
    Conj<Compu>::Iterador itInterfs = topologia.Computadoras().CrearIt();
    int j = 0
    while(j < i){
        itInterfs.Avanzar();
        j++;
    }
    return itInterfs.Siguiente();

}

const Interfaz& Driver::IntefazUsada(const Computadora& c1, const Computadora& c2) const {

    return topologia.InterfazUsada(c1,c2);

}

bool Driver::Conectadas(const Computadora& c1, const Computadora& c2) const {

    return topologia.Conectadas(c1,c2);
}

// TAD DCNET
void Driver::AgregarComputadora(const Computadora& ip, const Conj<Interfaz>& ci) {
    Compu c;
    c.ip = ip;
    c.interfaces = ci;
    topologia.AgregarComputadora(c);
}

void Driver::Conectar(const Computadora& c1, const Interfaz& i1, const Computadora& c2, const Interfaz& i2) {
    topologia.Conectar(ci.ip,c2.ip);
}


Nat Driver::CantidadNodosRecorridosPor(const Paquete& p) const {
    
    return dcNet.CaminoRecorrido(p).Longitud();
}

const Computadora& Driver::IesimoNodoRecorridoPor(const Paquete& p, const Nat i) const {
    Lista<Compu> camino = dcNet.CaminoRecorrido(p);
   
    return &res[i];
}

Nat Driver::CantidadEnviadosPor(const Computadora& c) const {

    return dcNet.CantidadEnviados(c);
}

Nat Driver::CantidadEnEsperaEn(const Computadora& c) const {
    
    return dcNet.EnEspera(c).Cardinal();
}

const Paquete& Driver::IesimoEnEsperaEn(const Computadora& c, const Nat i) const {
    Conj< ::Paquete>  buffer =  dcNet.EnEspera(c);
    Conj<Compu>::Iterador it buffer.CrearIt();
    int j = 0
    while(j < i){
        it.Avanzar();
        j++;
    }

    return it.Siguiente();
}

void Driver::CrearPaquete(const Computadora& origen, const Computadora& destino, Nat prioridad) {
    if(dcNet != NULL){
        DCNet(topologia) dcNet;
    }else{
        dcNet.CrearPaquete(origen,destino,prioridad);
    }
}

void Driver::AvanzarSegundo() {
    dcNet.AvanzarSegundo();
}

const Computadora& Driver::laQueMasEnvio() const {

    return dcNet.laQueMasEnvio();
}

const Computadora& Driver::origen(const Paquete& p) const {

    return dcNet.CaminoRecorrido(p).Primero();
}

const Computadora& Driver::destino(const Paquete& p) const {

    return dcNet.CaminoRecorrido(p).Ultimo();
}

Nat Driver::prioridad(const Paquete& p) const {
    Nat res;
    Conj<Compu>::Iterador itCompus = dcNet.red().compus.CrearIt();


    while(itCompus.HaySiguiente()){
        Conj<Paquete>::Iterador itBuffer = dcNet.EnEspera(itCompus.Siguiente()).CrearIt();
        while(itBuffer.HaySiguiente()){
            if(p == itBuffer.Siguiente())
                res = itBuffer.Siguiente().prioridad;
            itBuffer.Avanzar();
        }        
        itCompus.Avanzar();
    }
    return 0;
}



} // namespace aed2
