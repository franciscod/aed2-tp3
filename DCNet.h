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
    
    struct compu{
        String ip;
        Conj<int> interfaces;
    };

    struct paquete{
        int id;
        int prioridad;
        compu origen;
        compu destino;
    };

    struct paqueteDCNet{
        Conj<paquete>::Iterador it;
        Lista<compu> recorrido;
    };
    
    struct compuDCNet{
        compu *pc;
        Conj<paquete> conjPaquetes;
        DiccLog<Conj<paqueteDCNet>::Iterador> diccPaquetesDCNet;
        ColaPrioridad<Conj<paqueteDCNet>::Iterador> colaPaquetesDCNet;
        Conj<paqueteDCNet>::Iterador paqueteAEnviar;
        int enviados;
    };

    public:
        
        DCNet(Red);
        ~DCNet();

        void crearPaquete(const paquete);
        void avanzarSegundo();
        Red red() const;
        Lista<compu> caminoRecorrido(const paquete) const;
        int cantidadDeEnviados(const compu) const;
        Conj<paquete> enEspera(const compu) const;
        bool paqueteEnTransito(paquete) const;
        compu laQueMasEnvio() const;

        bool operator == (const DCNet) const;

    private:

        Red topologia;
        Vector<compuDCNet> vectorCompusDCNet;
        DiccString<*compuDCNet> diccCompusDCNet;
        Conj<paqueteDCNet> conjPaquetesDCNet;
        compuDCNet *laQueMasEnvio;

};

#endif // DCNET_H_INCLUDED