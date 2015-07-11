#ifndef DRIVER_H_
#define DRIVER_H_

#include "Tipos.h"
#include "Red.h"
#include "DCNet.h"
#include "aed2/Conj.h"

namespace aed2 {

class Driver
{
    public:

        Driver();

        ~Driver();

        /// Observadores Red //////////////////////////////////////////////////////////

        Nat CantidadComputadoras() const;
        const Computadora& IesimaComputadora(const Nat i) const ;

        Nat CantidadInterfacesDe(const Computadora& c) const;
        const Interfaz& IesimaInterfazDe(const Computadora& c, const Nat i) const ;

        Interfaz IntefazUsada(const Computadora& c1, const Computadora& c2) const;

        bool conectadas(const Computadora& c1, const Computadora& c2) const;

        /// Armado Red ////////////////////////////////////////////////////////////////

        void AgregarComputadora(const Computadora& ip, const Conj<Interfaz>& ci);

        void Conectar(const Computadora& c1, const Interfaz& i1, const Computadora& c2, const Interfaz& i2);


       /// Observadores DCNet //////////////////////////////////////////////////////////////

        Nat CantidadNodosRecorridosPor(const Paquete& p) const;
        const Computadora& IesimoNodoRecorridoPor(const Paquete& p, const Nat i) const;

        Nat CantidadEnviadosPor(const Computadora& c) const;

        Nat CantidadEnEsperaEn(const Computadora& c) const;
        const Paquete& IesimoEnEsperaEn(const Computadora& c, const Nat i) const;

        /// Acciones DCNet ////////////////////////////////////////////////////////////

        void CrearPaquete(const Computadora& origen, const Computadora& destino, Nat prioridad);

        void AvanzarSegundo();

        const Computadora& laQueMasEnvio() const;

        // Auxiliares para ver los paquetes
        const Computadora& origen(const Paquete& p) const;
        const Computadora& destino(const Paquete& p) const;
        Nat prioridad(const Paquete& p) const;

    private:

    DCNet dcNet;

    Red topologia;
    /*********************************************************************
     * TODO: Va a ser necesario instanciar privadamente un Modulo DCNet *
     * con el cuál interactuar. Además, pueden declarar todas las        *
     * funciones auxiliares que les hagan falta.                         *
     *********************************************************************/

}; // class Driver


} // namespace aed2

#endif // DRIVER_H_
