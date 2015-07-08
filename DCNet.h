#ifndef DCNET_H_
#define DCNET_H_

#include "red.h"
#include "DiccLog.h"
#include "ColaPrioridad.h"
#include "aed2/Vector.h"
#include "aed2/dicc_trie.h"
#include "aed2/Conj.h"
#include "aed2/Lista.h"

class DCNet{
    
    struct Compu{
        String ip;
        Conj<int> interfaces;

        bool operator == (const Compu) const;
    };

    struct Paquete{
        int id;
        int prioridad;
        Compu origen;
        Compu destino;

        bool operator == (const Paquete) const;
    };

    struct PaqueteDCNet{
        Conj<Paquete>::Iterador it;
        Lista<Compu> recorrido;

        bool operator == (const PaqueteDCNet) const;
    };
    
    struct CompuDCNet{
        Compu *pc;
        Conj<Paquete> conjPaquetes;
        DiccLog<Conj<PaqueteDCNet>::Iterador> diccPaquetesDCNet;
        ColaPrioridad<Conj<PaqueteDCNet>::Iterador> colaPaquetesDCNet;
        Conj<PaqueteDCNet>::Iterador paqueteAEnviar;
        int enviados;

        bool operator == (const CompuDCNet) const;
    };

    public:
        
        DCNet(Red);
        ~DCNet();

        void CrearPaquete(const Paquete);
        void AvanzarSegundo();
        Red Red() const;
        Lista<Compu> CaminoRecorrido(const Paquete) const;
        int CantidadDeEnviados(const Compu) const;
        Conj<Paquete> EnEspera(const Compu) const;
        bool PaqueteEnTransito(Paquete) const;
        Compu LaQueMasEnvio() const;

        bool operator == (const DCNet) const;

    private:

        Red topologia;
        Vector<CompuDCNet> vectorCompusDCNet;
        DiccString<*CompuDCNet> diccCompusDCNet;
        Conj<PaqueteDCNet> conjPaquetesDCNet;
        CompuDCNet *laQueMasEnvio;

};

#endif // DCNET_H_INCLUDED