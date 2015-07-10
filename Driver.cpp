#include "Driver.h"

namespace aed2 {

Driver::Driver() {
    // TODO
}

Driver::~Driver() {
    // TODO
}

// TAD RED
Nat Driver::CantidadComputadoras() const {

    return topologia.Computadoras().Cardinal();

}

const Computadora& Driver::IesimaComputadora(const Nat i) const {
    Conj<Compu>::Iterador it = compus.CrearIt();
    int j = 0
    while(j < i){
        it.Avanzar();
        j++;
    }
	return it.Siguiente();
}

Nat Driver::CantidadInterfacesDe(const Computadora& c) const {
    Conj<Compu>::Iterador itCompuB = compus.CrearIt();
     while(c != it.Siguiente()){
        it.Avanzar();
    }
    return topologia.Interfaces(it.Siguiente()).Cardinal();

}

const Interfaz& Driver::IesimaInterfazDe(const Computadora& c, const Nat i) const{
    Conj<Compu>::Iterador itCompus= compus.CrearIt();
    while(c != itCompus.Siguiente()){
        itCompus.Avanzar();
    }
    Conj<Interfaz> interfaces = topologia.Interfaces(it.Siguiente());
    Conj<Compu>::Iterador itInterfs = compus.CrearIt();
    int j = 0
    while(j < i){
        itInterfs.Avanzar();
        j++;
    }
    return itInterfs.Siguiente();

}

const Interfaz& Driver::IntefazUsada(const Computadora& c1, const Computadora& c2) const {
    // TODO
    return topologia.InterfazUsada(c1,c2);

}

bool Driver::Conectadas(const Computadora& c1, const Computadora& c2) const {
    // TODO
    return topologia.Conectadas(c1,c2);
}

// TAD DCNET
void Driver::AgregarComputadora(const Computadora& ip, const Conj<Interfaz>& ci) {
    // TODO
}

void Driver::Conectar(const Computadora& c1, const Interfaz& i1, const Computadora& c2, const Interfaz& i2) {
    // TODO
}


Nat Driver::CantidadNodosRecorridosPor(const Paquete& p) const {
    // TODO
    return 0;
}

const Computadora& Driver::IesimoNodoRecorridoPor(const Paquete& p, const Nat i) const {
    // TODO
}

Nat Driver::CantidadEnviadosPor(const Computadora& c) const {
    // TODO
    return 0;
}

Nat Driver::CantidadEnEsperaEn(const Computadora& c) const {
    // TODO
    return 0;
}

const Paquete& Driver::IesimoEnEsperaEn(const Computadora& c, const Nat i) const {
    // TODO
    return 0;
}

void Driver::CrearPaquete(const Computadora& origen, const Computadora& destino, Nat prioridad) {
    // TODO
}

void Driver::AvanzarSegundo() {
    // TODO
}

const Computadora& Driver::laQueMasEnvio() const {
    // TODO
    return 0;
}

const Computadora& Driver::origen(const Paquete& p) const {
    // TODO
    return 0;
}

const Computadora& Driver::destino(const Paquete& p) const {
    // TODO
    return 0;
}

Nat Driver::prioridad(const Paquete& p) const {
    // TODO
    return 0;
}



} // namespace aed2
