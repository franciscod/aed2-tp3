#ifndef DCNET_H_
#define DCNET_H_

#include "Tipos.h"
#include "Red.h"
#include "DiccLog.h"
#include "ColaPrioridad.h"
#include "aed2/Vector.h"
#include "aed2/dicc_trie.h"
#include "aed2/Conj.h"
#include "aed2/Lista.h"

using namespace aed2;

class DCNet{

    struct PaqueteDCNet{
        Conj< ::Paquete>::Iterador it;
        Lista<Compu> recorrido;

        bool operator == (const PaqueteDCNet& otro) const{
            return (it.Siguiente() == otro.it.Siguiente()) && (recorrido == otro.recorrido);
        }

        bool operator != (const PaqueteDCNet& otro) const{
            return !(*this==otro);
        }
    };

    struct CompuDCNet{
        Compu pc;
        Conj< ::Paquete> conjPaquetes;
        DiccLog<Lista<PaqueteDCNet>::Iterador> diccPaquetesDCNet;
        ColaPrioridad<Lista<PaqueteDCNet>::Iterador> colaPaquetesDCNet;
        Lista<PaqueteDCNet>::Iterador paqueteAEnviar;
        Nat enviados;

        bool operator == (const CompuDCNet&) const;
        bool operator != (const CompuDCNet&) const;
    };

    public:
        DCNet();
        DCNet(const class Red&);

        void CrearPaquete(const ::Paquete&);
        void AvanzarSegundo();
        class Red Red() const;
        Lista<Compu> CaminoRecorrido(const ::Paquete&) const;
        Nat CantidadEnviados(const Compu&) const;
        const Conj< ::Paquete>& EnEspera(const Compu&) const;
        bool PaqueteEnTransito(const ::Paquete&) const;
        const Compu& LaQueMasEnvio() const;

        bool operator == (const DCNet&) const;

    private:

        class Red topologia;
        Vector<CompuDCNet> vectorCompusDCNet;
        DiccString<CompuDCNet*> diccCompusDCNet;
        Lista<PaqueteDCNet> listaPaquetesDCNet;
        CompuDCNet* laQueMasEnvio;

};

#endif // DCNET_H_INCLUDED
