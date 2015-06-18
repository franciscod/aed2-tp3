#include "ConjAcotado.h"

/*****************************************************************************/
/***************************** ConjAcotado *******************************/
/*****************************************************************************/

namespace aed2{ 

    /**
     * Constructor de un conjunto acotado.
     * Genera un conjunto vac�o en el rango dado.
     * Implementa la funci�n VAC�O(in l: nat, in r: nat) -> res : conjAcotado
     * PRE: { l <= r }
     * POS: { res = 0(l,r) }
     * Complejidad: O(r-l)
     */
    ConjAcotado::ConjAcotado(Nat inf, Nat sup) :
        infimo(inf), pertenencia(sup - inf + 1) {
        assert(inf <= sup);
    }

    /**
     * Constructor por copia de un conjunto acotado.
     * Genera una copia nueva del conjuto.
     * Implementa la funci�n COPIAR(in c: conjAcotado) -> res : conjAcotado
     * PRE: { true }
     * POS: { res = c }
     * Complejidad: O(Supremo(c) - Infimo(c))
     */
    ConjAcotado::ConjAcotado(const ConjAcotado& conjunto) : 
        infimo(conjunto.infimo), pertenencia(conjunto.pertenencia.Tamanho()) {

        for(ConjAcotado::const_Iterador it = conjunto.CrearIt(); it.HaySiguiente(); it.Avanzar())
            Agregar(it.Siguiente());
    }

    /**
     * Agrega un elemento al conjunto.
     * Implementa la funci�n AGREGAR(in/out c: conjAcotado, in e : nat)
     * PRE: { c = c_0 ^ Infimo(c) <= e <= Supremo(c) }
     * POS: { c = Ag(e, c_0) }
     * Complejidad: O(1)
     */
    void ConjAcotado::Agregar(Nat e) {
        assert(EnRango(e));
        if (!Pertenece(e)) {
			pertenencia.Definir(Indice(e), elementos.AgregarRapido(e));
        }
    }

    /**
     * Devuelve el valor m�nimo que se puede agregar al conjunto.
     * Implementa la funci�n INFIMO(in c: conjAcotado) -> res : nat
     * PRE: { true }
     * POS: { res = Infimo(c) }
     * Complejidad: O(1)
     */
    Nat ConjAcotado::Infimo() const {
        return infimo;
    }

    /**
     * Devuelve el valor m�ximo que se puede agregar al conjunto.
     * Implementa la funci�n SUPREMO(in c: conjAcotado) -> res : nat
     * PRE: { true }
     * POS: { res = Supremo(c) }
     * Complejidad: O(1)
     */
    Nat ConjAcotado::Supremo() const {
        return infimo + pertenencia.Tamanho() - 1;
    }

    /**
     * Devuelve true si y s�lo si c esta vac�o.
     * Implementa la funci�n ESVAC�O(in c: conjAcotado) -> res : nat
     * PRE: { true }
     * POS: { res = 0?(ConjSuby(c) }
     * Complejidad: O(1)
     */
    bool ConjAcotado::EsVacio() const {
        return elementos.EsVacio();
    }

    /**
     * Devuelve true si y s�lo e pertenece al conjunto.
     * Notar que no es requerido que e pertenezca al rango de c.
     * Implementa la funci�n PERTENECE?(in c: conjAcotado, in e: nat) -> res : bool
     * PRE: { true }
     * POS: { res = e � ConjSuby(c) }
     * Complejidad: O(1)
     */
    bool ConjAcotado::Pertenece(Nat e) const {
        return EnRango(e) and pertenencia.Definido(Indice(e));
    }

    /**
     * Elimina a de c, si es que estaba.
     * Observar que no es requerido que e pertenezca al rango de c.
     * Implementa la funci�n ELIMINAR(in/out c: conjAcotado, in e: nat)
     * PRE: { c = c_0 }
     * POS: { ConjSuby(c) = ConjSuby(c_0) \ {e} ^ Rango(c) = Rango(c_0) }
     * Complejidad: O(1)		
     */
    void ConjAcotado::Eliminar(Nat e) {
        if (EnRango(e) && Pertenece(e)) {
            pertenencia[Indice(e)].EliminarSiguiente();
            pertenencia.Borrar(Indice(e));
        }
    }
    
    /**
     * Crea un iterador modificable.
     */
    ConjAcotado::Iterador ConjAcotado::CrearIt() {
		return ConjAcotado::Iterador(this);
	}
	
	/**
	 * Crea un iterador constante.
	 */
	ConjAcotado::const_Iterador ConjAcotado::CrearIt() const {
		return ConjAcotado::const_Iterador(this);	
	}

    /**
     * Devuelve la cantidad de elementos del conjunto.
     * Implementa la funci�n CARDINAL(in c: conjAcotado) -> res : nat
     * PRE: { true }
     * POS: { res = #ConjSuby(c) }
     * Complejidad: O(1)
     */
    Nat ConjAcotado::Cardinal() const {
        return elementos.Cardinal();
    }

    /** M�todo privado para validar que un elemento est� dentro el rango. */
    bool ConjAcotado::EnRango(Nat e) const {
        return Infimo() <= e && e <= Supremo();
    }

    /** M�todo privado para obtener el hash de un elemento. PRE: { enRango(e) } */
    int ConjAcotado::Indice(Nat e) const {
        return e - infimo;
    }

    /*****************************************************************************/
    /*************************** Iterador *****************************/
    /*****************************************************************************/


    /**
     * Crea un iterador bidireccional del conjunto, de forma tal que HayAnterior
     * eval�e a false (i.e., que se pueda recorrer los elementos aplicando
     * iterativamente Siguiente).
     * Implementa la funci�n CrearIt(in c: conjAcotado) -> res : itConj
     * PRE: { true }
     * POS: { esPermutaci�n?(SecySuby(res), ConjSuby(c)) ^ vacia?(Anteriores(res)) }
     * Complejidad: O(1)
     */
    ConjAcotado::Iterador::Iterador(ConjAcotado* conjunto) :
        it(conjunto->elementos.CrearIt()), conj(conjunto) {}

    /**
     * Devuelve true si en el iterador todav�a quedan elementos por avanzar.
     * Implementa la funci�n HAYSIGUIENTE(in it: itConj) -> res : bool
     * PRE: { true }
     * POS: { res = haySiguiente?(it) }
     * Complejidad: O(1)
     */
    bool ConjAcotado::Iterador::HaySiguiente() const {
        return it.HaySiguiente();
    }

    /**
     * Devuelve true si en el iterador todav�a quedan elementos por retroceder.
     * Implementa la funci�n HAYANTERIOR(in it: itConj) -> res : bool
     * PRE: { true }
     * POS: { res = hayAnterior?(it) }
     * Complejidad: O(1)
     */
    bool ConjAcotado::Iterador::HayAnterior() const {
        return it.HayAnterior();
    }

    /**
     * Devuelve el elemento siguiente a la posici�n del iterador.
     * Implementa la funci�n SIGUIENTE(in it: itConj) -> res : nat
     * PRE: { HaySiguiente?(it) }
     * POS: { res = Siguiente(it) }
     * Complejidad: O(1)
     */
    Nat ConjAcotado::Iterador::Siguiente() const {
        assert(HaySiguiente());
        return it.Siguiente();
    }

    /**
     * Devuelve el elemento anterior a la posici�n del iterador.
     * Implementa la funci�n ANTERIOR(in it: itConj) -> res : nat
     * PRE: { HayAnterior?(it) }
     * POS: { res = Anterior(it) }
     * Complejidad: O(1)
     */
    Nat ConjAcotado::Iterador::Anterior() const {
        assert(HayAnterior());
        return it.Anterior();
    }

    /**
     * Avanza a la posici�n siguiente del iterador.
     * Implementa la funci�n AVANZAR(in/out it: itConj)
     * PRE: { it = it_0 ^ HaySiguiente?(it) }
     * POS: { it = Avanzar(it_0) }
     * Complejidad: O(1)
     */
    void ConjAcotado::Iterador::Avanzar() {
        assert(HaySiguiente());
        it.Avanzar();
    }

    /**
     * Retrocede a la posici�n anterior del iterador.
     * Implementa la funci�n RETROCEDER(in/out it: itConj)
     * PRE: { it = it_0 ^ HayAnterior?(it) }
     * POS: { it = Retroceder(it_0) }
     * Complejidad: O(1)
     */
    void ConjAcotado::Iterador::Retroceder() {
        assert(HayAnterior());
        it.Retroceder();
    }

    /**
     * Elimina de la lista iterada el valor que se encuentra en la posici�n
     * siguiente del iterador.
     * Implmenta la funci�n ELIMINARSIGUIENTE(in/out it: itConj)
     * PRE: { it = it_0 ^ HaySiguiente?(it) }
     * POS: { it = EliminarSiguiente(it_0) }
     * Complejidad: O(1)
     */
    void ConjAcotado::Iterador::EliminarSiguiente() {
        assert(HaySiguiente());
        it.Avanzar();
        conj->Eliminar(it.Anterior());
    }

    /**
     * Elimina de la lista iterada el valor que se encuentra en la posici�n
     * anterior del iterador.
     * Implmenta la funci�n ELIMINARANTERIOR(in/out it: itConj)
     * PRE: { it = it_0 ^ HayAnterior?(it) }
     * POS: { it = EliminarAnterior(it_0) }
     * Complejidad: O(1)
     */
    void ConjAcotado::Iterador::EliminarAnterior() {
        assert(HayAnterior());
        conj->Eliminar(it.Anterior());
    }


    /*****************************************************************************/
    /**************************** const_Iterador ******************************/
    /*****************************************************************************/


    /**
     * Crea un iterador bidireccional del conjunto, de forma tal que HayAnterior
     * eval�e a false (i.e., que se pueda recorrer los elementos aplicando
     * iterativamente Siguiente). Este iterador no permite modificar el conjunto.
     * Implementa la funci�n CrearIt(in c: conjAcotado) -> res : itConj
     * PRE: { true }
     * POS: { esPermutaci�n?(SecySuby(res), ConjSuby(c)) ^ vacia?(Anteriores(res)) }
     * Complejidad: O(1)
     */
    ConjAcotado::const_Iterador::const_Iterador(const ConjAcotado* conjunto) : 
        it(conjunto->elementos.CrearIt()) {}

    /**
     * Devuelve true si en el iterador todav�a quedan elementos por avanzar.
     * Implementa la funci�n HAYSIGUIENTE(in it: itConj) -> res : bool
     * PRE: { true }
     * POS: { res = haySiguiente?(it) }
     * Complejidad: O(1)
     */
    bool ConjAcotado::const_Iterador::HaySiguiente() const {
        return it.HaySiguiente();
    }

    /**
     * Devuelve true si en el iterador todav�a quedan elementos por retroceder.
     * Implementa la funci�n HAYANTERIOR(in it: itConj) -> res : bool
     * PRE: { true }
     * POS: { res = hayAnterior?(it) }
     * Complejidad: O(1)
     */
    bool ConjAcotado::const_Iterador::HayAnterior() const {
        return it.HayAnterior();
    }

    /**
     * Devuelve el elemento siguiente a la posici�n del iterador.
     * Implementa la funci�n SIGUIENTE(in it: itConj) -> res : nat
     * PRE: { HaySiguiente?(it) }
     * POS: { res = Siguiente(it) }
     * Complejidad: O(1)
     */
    Nat ConjAcotado::const_Iterador::Siguiente() const {
        assert(HaySiguiente());
        return it.Siguiente();
    }

    /**
     * Devuelve el elemento anterior a la posici�n del iterador.
     * Implementa la funci�n ANTERIOR(in it: itConj) -> res : nat
     * PRE: { HayAnterior?(it) }
     * POS: { res = Anterior(it) }
     * Complejidad: O(1)
     */
    Nat ConjAcotado::const_Iterador::Anterior() const {
        assert(HayAnterior());
        return it.Anterior();
    }

    /**
     * Avanza a la posici�n siguiente del iterador.
     * Implementa la funci�n AVANZAR(in/out it: itConj)
     * PRE: { it = it_0 ^ HaySiguiente?(it) }
     * POS: { it = Avanzar(it_0) }
     * Complejidad: O(1)
     */
    void ConjAcotado::const_Iterador::Avanzar() {
        assert(HaySiguiente());
        it.Avanzar();
    }

    /**
     * Retrocede a la posici�n anterior del iterador.
     * Implementa la funci�n RETROCEDER(in/out it: itConj)
     * PRE: { it = it_0 ^ HayAnterior?(it) }
     * POS: { it = Retroceder(it_0) }
     * Complejidad: O(1)
     */
    void ConjAcotado::const_Iterador::Retroceder() {
        assert(HayAnterior());
        it.Retroceder();
    }

    /**
     * Compara c1 y c2 por igualdad.
     * Implementa la funci�n * = * (in c1: conjAcotado, in c2: conjAcotado) -> bool
     * PRE: { true }
     * POS: { res = (c1 = c2) }
     * Complejidad: O(min(#c1, #c2))
     */
    bool operator==(const ConjAcotado& c1, const ConjAcotado& c2) {
        bool res =
            c1.Infimo() == c2.Infimo() &&
            c1.Supremo() == c2.Supremo() &&
            c1.Cardinal() == c2.Cardinal();
        for (ConjAcotado::const_Iterador it = c1.CrearIt(); it.HaySiguiente() and res; it.Avanzar())
            res &= c2.Pertenece(it.Siguiente());
        return res;
    }

    /**
     * Genera una representaci�n del conjunto con un string y la escribe dentro
     * de un output stream.
     */
    std::ostream& operator<<(std::ostream& os, const ConjAcotado& c) {
        return Mostrar(os, c, '{', '}');
    }
} //namespace aed2
