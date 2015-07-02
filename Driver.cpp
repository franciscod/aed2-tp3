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
    // TODO
    return 0;

}

const Computadora& Driver::IesimaComputadora(const Nat i) const {
    // TODO
	return 0;
}
        
Nat Driver::CantidadInterfacesDe(const Computadora& c) const {
    // TODO
    return 0;

}

const Interfaz& Driver::IesimaInterfazDe(const Computadora& c, const Nat i) const{
    // TODO
    return 0;

} 

const Interfaz& Driver::IntefazUsada(const Computadora& c1, const Computadora& c2) const {
    // TODO
    return 0;

}

bool Driver::conectadas(const Computadora& c1, const Computadora& c2) const {
    // TODO
    return 0;
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

const Computadora& Driver::IesimoNodoRecorridoPor(const Paquete& p) const {
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

const Paquete& Driver::IesimoEnEsperaEn(const Computadora& c) const {
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

