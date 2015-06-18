
#ifndef AED2_ARREGLO_H_INCLUDED
#define AED2_ARREGLO_H_INCLUDED

#include "TiposBasicos.h"

namespace aed2 {

    template<class T>
    class Arreglo{
        public:

            /**
             * Crea un arreglo de tamaño 0.
             */
            Arreglo();

            /**
             * Crea un arreglo de tamaño tam, donde todas las posiciones
             * son nulas.
             */
            Arreglo(Nat tam);

            /**
             * Constructor por copia.  Los elementos de otro se copian
             * en el mismo orden a this, y pasan a ser arreglos independientes
             */
            Arreglo(const Arreglo<T>& otro);

            /**
             * Operacion de asignacion.  Borra lo que se que habia en this
             * y copia los elementos de otro en el mismo orden.
             */
            Arreglo<T>& operator=(const Arreglo<T>& otro);

            /**
             * Destructor.  Borra lo que hubiera en el arreglo.
             */
            ~Arreglo();

            /**
             * Devuelve el elemento en la posicion pos.
             * Requiere: Definido(pos)
             */
            const T& operator[](Nat pos) const;

            /**
             * Devuelve el elemento en la posicion pos.
             * Requiere: Definido(pos)
             */
            T& operator[](Nat pos);

            /**
             * Devuelve true si en la posicion pos fue definido algun
             * elemento.
             */
            bool Definido(Nat pos) const;

            /**
             * Define valor en la posicion pos.  Devuelve this
             */
            Arreglo<T>& Definir(Nat pos, const T& valor);

            /**
             * Indefine el elemento en pos.
             * No requiere que el elemento este definido.
             * Devuelve this.
             */
            Arreglo<T>& Borrar(Nat pos);

            /**
             * Devuelve el tamaño del arreglo
             */
            Nat Tamanho() const;

            /**
             * Redimensiona el arreglo, sin copiar los items de nuevo.
             * Si el arreglo es mas chico, se borran los datos sobrantes.
             * Sino, se definen items nuevos en null.
             */
            Arreglo<T>& Redimensionar(Nat tam);

            /**
             * Mueve el dato de otro[posOtro] a this[posThis] en O(1).
             * Si ya habia un dato en this[posThis], el mismo se elimina.
             * Despues de esto, no esta definido el valor de posOtro en
             * otro.
             * Obviamente, this puede ser igual a otro, para lograr mover
             * las cosas de un lugar a otro.  Aunque se recomienda utilizar
             * la otra version de mover.
             */
            Arreglo<T>& Mover(Nat posThis, Arreglo<T>& otro, Nat posOtro);

            /**
             * Equivalente a Mover(destino, this, origen)
             */
            Arreglo<T>& Mover(Nat destino, Nat origen);

            /**
             * Intercambia los valores de this[posThis] y otro[posOtro]
             * sin realizar copias nuevas de los elementos.
             * Obviamente, this puede ser igual a otro.
             */
            Arreglo<T>& Swap(Nat posThis, Arreglo<T>& otro, Nat posOtro);

            /**
             * Equivalente a Swap(posA, this, posB)
             */
            Arreglo<T>& Swap(Nat posA, Nat posB);

        private:
            //HACK PARA QUE AUTOMAGICAMENTE SE ASIGNEN LAS POSICIONES EN NULL
            struct Ptr {
                public:
                    Ptr(T* t = NULL) {ptr = t;}
                    operator T*() {return ptr;}
                    void Delete() {if(ptr!=NULL) delete ptr; ptr = NULL;}
                    Ptr Copiar() {return ptr == NULL ? NULL : Ptr(new T(*ptr));}
                    T* ptr;
            };

            Ptr* array;
            Nat size;

            void Destruir();
            void Asignar(const Arreglo<T>& otro);

    };

    /**
     * Funciones globales de comparacion.  Con definir el == obtenemos
     * el != gratis :)
     */
    template<class T>
    bool operator==(const Arreglo<T>&, const Arreglo<T>&);

    template<class T>
    std::ostream& operator<<(std::ostream& os, const Arreglo<T>&);


template<class T>
Arreglo<T>::Arreglo() : array(NULL), size(0) {}

template<class T>
Arreglo<T>::Arreglo(Nat tamanio) : size(tamanio) {
    array = new Ptr[size];
}

template<class T>
Arreglo<T>::Arreglo(const Arreglo<T>& otro) {
    Asignar(otro);
}

template<class T>
Arreglo<T>& Arreglo<T>::operator=(const Arreglo<T>& otro) {
    if(this != &otro) {
        Destruir();
        Asignar(otro);
    }
    return *this;
}

template<class T>
Arreglo<T>::~Arreglo() {
    Destruir();
}

template<class T>
const T& Arreglo<T>::operator[](Nat pos) const {
    assert(Definido(pos));
    return *array[pos];
}

template<class T>
T& Arreglo<T>::operator[](Nat pos) {
    assert(Definido(pos));
    return *array[pos];
}

template<class T>
bool Arreglo<T>::Definido(Nat pos) const {
    return pos < size and array[pos] != NULL;
}

template<class T>
Arreglo<T>& Arreglo<T>::Definir(Nat pos, const T& valor) {
    assert(pos < size);
    Borrar(pos);
    array[pos] = new T(valor);
    return *this;
}

template<class T>
Arreglo<T>& Arreglo<T>::Borrar(Nat pos) {
    assert(pos < size);
    array[pos].Delete();
    return *this;
}

template<class T>
Nat Arreglo<T>::Tamanho() const {
    return size;
}

template<class T>
Arreglo<T>& Arreglo<T>::Redimensionar(Nat tam) {
    Ptr* oldarray = array;
    array = tam == 0 ? NULL : new Ptr[tam];

    for(Nat p = 0; p < size && p < tam; ++p)
        array[p] = oldarray[p];     //no copia, simplemente mueve los punteros

    //borramos los elementos viejos
    for(Nat p = tam; p < size; ++p)
        oldarray[p].Delete();
    delete [] oldarray;

    size = tam;
    return *this;
}

template<class T>
Arreglo<T>& Arreglo<T>::Mover(Nat posThis, Arreglo<T>& otro, Nat posOtro) {
    if(this != &otro or posThis != posOtro) {
        Borrar(posThis);
        array[posThis] = otro.array[posOtro];
        otro.array[posOtro] = NULL;
    }
    return *this;
}

template<class T>
Arreglo<T>& Arreglo<T>::Mover(Nat destino, Nat origen) {
    return Mover(destino, *this, origen);
}

template<class T>
Arreglo<T>& Arreglo<T>::Swap(Nat posThis, Arreglo<T>& otro, Nat posOtro) {
    Ptr tmp = array[posThis];
    array[posThis] = otro.array[posOtro];
    otro.array[posOtro] = tmp;
    return *this;
}

template<class T>
Arreglo<T>& Arreglo<T>::Swap(Nat posA, Nat posB) {
    return Swap(posA, *this, posB);
}

template<class T>
void Arreglo<T>::Destruir() {
    for(Nat p = 0; p < size; ++p) Borrar(p);
    delete[] array;
    array = NULL;
}

template<class T>
void Arreglo<T>::Asignar(const Arreglo<T>& otro) {
    size = otro.size;
    array = new Ptr[size];
    for(Nat p = 0; p < size; ++p)
        array[p] = otro.array[p].Copiar();
}


template<class T>
std::ostream& operator<<(std::ostream& os, const Arreglo<T>& a) {
    os << "[";
    for(Nat p = 0; p < a.Tamanho() - 1; ++p) {
        if(a.Definido(p)) os << a[p];
        os << ",";
    }
    if(a.Definido(a.Tamanho()-1)) os << a[a.Tamanho()-1];
    return os << "]";
}

template<class T>
bool operator==(const Arreglo<T>& a, const Arreglo<T>& b) {
    bool retval = a.Tamanho() == b.Tamanho();
    for(Nat p = 0; p < a.Tamanho() && retval; ++p) {
        retval = a.Definido(p) == b.Definido(p);
        if(retval and a.Definido(p)) retval = a[p] == b[p];
    }
    return retval;
}

}



#endif
