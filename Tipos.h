#ifndef AED2_TIPOS_H_
#define AED2_TIPOS_H_

#include "aed2/TiposBasicos.h"
#include "aed2/Conj.h"

namespace aed2{

typedef Nat Prioridad;
typedef Nat Interfaz;
typedef Nat ID;
typedef String Computadora;
typedef Nat Paquete;

} // namespace aed2

using namespace aed2;

struct Compu {
	String ip;
	Conj<Nat> interfaces;

	bool operator==(const Compu& rhs) const {
	    return (ip == rhs.ip) && (interfaces == rhs.interfaces);
	}

	bool operator!=(const Compu& rhs) const {
	    return (ip != rhs.ip) || !(interfaces == rhs.interfaces);
	}

	Compu () {};
	Compu (const Computadora& ip) : ip(ip){};
	Compu (const Computadora& ip, Nat numInterfaces) : ip(ip) {
		for (Nat n = 1; n <= numInterfaces; n++) {
			interfaces.Agregar(n);
		}
	};
};

inline std::ostream& operator<<(std::ostream& os, const Compu& compu) {
	os << "[Compu " << compu.ip << "]";
	return os;
}

struct Paquete{
    int id;
    int prioridad;
    Compu origen;
    Compu destino;

	bool operator == (const Paquete& otro) const{
	    bool boolID = (id == otro.id);
	    bool boolPrior = (prioridad == otro.prioridad);
	    bool boolOrigen = (origen == otro.origen);
	    bool boolDestino = (destino == otro.destino);
	    return boolID && boolPrior && boolOrigen && boolDestino;
	}

	bool operator != (const Paquete& otro) const{
	    return !(*this==otro);
	}
};


#endif // AED2_TIPOS_H_
