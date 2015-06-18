#ifndef AED2_CONJACOTADO_H_INCLUDED
#define AED2_CONJACOTADO_H_INCLUDED

#include <iostream>
#include "TiposBasicos.h"
#include "Arreglo.h"
#include "Conj.h"


namespace aed2 {

    /**
     * Esta clase implementa el módulo conjunto acotado de Naturales.
     * El módulo conjunto acotado de Naturales provee un conjunto en el que se
     * pueden insertar únicamente los elementos que se encuentran en un rango [l, r]
     * de Naturales. La inserción, eliminación y testeo de pertenencia de un
     * elemento se pueden resolver en tiempo constante. El principal costo se paga
     * cuando se crea la estructura, dado que cuesta tiempo lineal en r - l.
     * En cuanto al recorrido de los elementos, se provee un iterador bidireccional
     * que también permite eliminar los elementos iterados.
     */
    class ConjAcotado {
        
        public:
            class Iterador; 
            class const_Iterador;

            ConjAcotado(Nat infimo, Nat supremo);
            ConjAcotado(const ConjAcotado& conjunto);
            ~ConjAcotado() {};
            
            void Agregar(Nat e);
            Nat Infimo() const;
            Nat Supremo() const;
            bool EsVacio() const;
            bool Pertenece(Nat e) const;
            void Eliminar(Nat e);
            Nat Cardinal() const;

            Iterador CrearIt();
            const_Iterador CrearIt() const;
           
            class Iterador {
                public:
                    bool HaySiguiente() const;
                    bool HayAnterior() const;
                    Nat Siguiente() const;
                    Nat Anterior() const;
                    void Avanzar();
                    void Retroceder();
                    void EliminarSiguiente();
                    void EliminarAnterior();
                    
                    Iterador() : conj(NULL) {};
                    Iterador(const ConjAcotado::Iterador& otro) : it(otro.it), conj(otro.conj) {}
                    Iterador& operator=(ConjAcotado::Iterador& otro) 
                    	{it = otro.it; conj = otro.conj; return *this;}

					bool operator==(const ConjAcotado::Iterador& otro) const {return it == otro.it;}

                private:
                    Iterador(ConjAcotado* conjunto);
                    Conj<Nat>::Iterador it;
                    ConjAcotado* conj;
                    friend ConjAcotado::Iterador ConjAcotado::CrearIt();
                    friend class ConjAcotado::const_Iterador;
            };

            class const_Iterador {
                public:
                	const_Iterador() {}
					const_Iterador(const ConjAcotado::Iterador& otro) : it(otro.it) {}
                    const_Iterador(const ConjAcotado::const_Iterador& otro) : it(otro.it) {}
                    const_Iterador& operator=(ConjAcotado::const_Iterador& otro) 
                    	{it = otro.it; return *this;}
                    
                    bool operator==(const ConjAcotado::const_Iterador& otro) const {return it == otro.it;}
                    
                    bool HaySiguiente() const;
                    bool HayAnterior() const;
                    Nat Siguiente() const;
                    Nat Anterior() const;
                    void Avanzar();
                    void Retroceder();

                private:
                    const_Iterador(const ConjAcotado* conjunto);
                    friend ConjAcotado::const_Iterador ConjAcotado::CrearIt() const;
                    Conj<Nat>::const_Iterador it;
            };

        private:

            bool EnRango(Nat e) const;
            int Indice(Nat e) const;

            Nat infimo;
            
            Conj<Nat> elementos;
            Arreglo< Conj<Nat>::Iterador > pertenencia;

    };

    bool operator==(const ConjAcotado&, const ConjAcotado&);
    std::ostream& operator<<(std::ostream& os, const ConjAcotado&);
}
#endif //AED2_CONJACOTADO_H_INCLUDED
