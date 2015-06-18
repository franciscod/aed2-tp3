
#ifndef AED2_VECTOR_H_INCLUDED
#define AED2_VECTOR_H_INCLUDED

#include "TiposBasicos.h"
#include "Arreglo.h"

namespace aed2 {

    template<class T>
    class Vector {
        public:
            class const_Iterador;

            Vector();
            Vector(const Vector<T>& otro);
            Vector<T>& operator=(const Vector<T>& otro);

            void AgregarAtras(const T& elem);
            bool EsVacio() const;
            void Comienzo();
            void TomarPrimeros(Nat n);
            void TirarUltimos(Nat n);
            const T& Ultimo() const;
            T& Ultimo();
            Nat Longitud() const;
            const T& operator[](Nat pos) const;
            T& operator[](Nat pos);

            void Agregar(Nat pos, const T& elem);
            void Eliminar(Nat pos);

            //funciones para implementar sorting y esas cosas eficientemente
            void Mover(Nat posThis, Vector<T>& otro, Nat posOtro);
            void Mover(Nat destino, Nat origen);
            void Swap(Nat posThis, Vector<T>& otro, Nat posOtro);
            void Swap(Nat pos1, Nat pos2);

            //iterador para funciones genericas.  En particular, Mostrar :)
            const_Iterador CrearIt() const;

            class const_Iterador{
                public:
                    bool HaySiguiente() const {return pos < vector->Longitud();}
                    bool HayAnterior() const {return pos != 0;}
                    void Avanzar() {assert(HaySiguiente()); ++pos;}
                    void Retroceder() {assert(HayAnterior()); --pos;}
                    const T& Siguiente() const {assert(HaySiguiente()); return (*vector)[pos];}
                    const T& Anterior() const {assert(HayAnterior()); return (*vector)[pos-1];};
                private:
                    const Vector<T>* vector;
                    Nat pos;
                    const_Iterador(const Vector<T>* v, Nat p = 0) : vector(v), pos(p) {}
                    friend typename Vector<T>::const_Iterador Vector<T>::CrearIt() const;
            };
        private:
            Arreglo<T> elementos;
            Nat longitud;
    };

    template<class T>
    std::ostream& operator<<(std::ostream& os, const Vector<T>& v);

    template<class T>
    bool operator==(const Vector<T>& v, const Vector<T>& w);


template<class T>
Vector<T>::Vector() : longitud(0) {}

template<class T>
Vector<T>::Vector(const Vector<T>& otro) : longitud(0) {
    for(Nat p = 0; p < otro.longitud; ++p) AgregarAtras(otro[p]);
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& otro) {
    if(this != &otro) {
        //esto no es lo mas rapido, pero bueh
        longitud = 0;
        elementos.Redimensionar(0);
        for(Nat p = 0; p < otro.longitud; ++p) AgregarAtras(otro[p]);
    }
    return *this;
}

template<class T>
void Vector<T>::AgregarAtras(const T& elem) {
    Agregar(longitud, elem);
}

template<class T>
bool Vector<T>::EsVacio() const {
    return longitud == 0;
}

template<class T>
void Vector<T>::Comienzo() {
    assert(not EsVacio());
    longitud -= 1;
}

template<class T>
void Vector<T>::TomarPrimeros(Nat n) {
    assert(n <= longitud);
    longitud = n;
}

template<class T>
void Vector<T>::TirarUltimos(Nat n) {
    assert(n <= longitud);
    longitud -= n;
}

template<class T>
const T& Vector<T>::Ultimo() const {
    assert(not EsVacio());
    return elementos[longitud - 1];
}

template<class T>
T& Vector<T>::Ultimo() {
    assert(not EsVacio());
    return elementos[longitud - 1];
}

template<class T>
Nat Vector<T>::Longitud() const {
    return longitud;
}

template<class T>
const T& Vector<T>::operator[](Nat pos) const {
    assert(pos < longitud);
    return elementos[pos];
}

template<class T>
T& Vector<T>::operator[](Nat pos) {
    assert(pos < longitud);
    return elementos[pos];
}

template<class T>
void Vector<T>::Agregar(Nat pos, const T& elem) {
    assert(pos <= longitud);

    if(longitud == elementos.Tamanho())
        elementos.Redimensionar(longitud == 0 ? 1 : longitud << 1);
    longitud += 1;

    for(Nat p = longitud-1; p > pos; --p) Mover(p, p-1);
    elementos.Definir(pos, elem);
}

template<class T>
void Vector<T>::Eliminar(Nat pos) {
    assert(pos < longitud and not EsVacio());
    for(Nat p = pos+1; p < longitud; ++p) Mover(p-1, p);
    longitud -= 1;
}

template<class T>
void Vector<T>::Mover(Nat posThis, Vector<T>& otro, Nat posOtro) {
    assert(posThis < longitud and posOtro < otro.longitud);
    elementos.Mover(posThis, otro.elementos, posOtro);
}

template<class T>
void Vector<T>::Mover(Nat destino, Nat origen) {
    assert(destino < longitud and origen < longitud);
    Mover(destino, *this, origen);
}

template<class T>
void Vector<T>::Swap(Nat posThis, Vector<T>& otro, Nat posOtro) {
    assert(posThis < longitud and posOtro < otro.longitud);
    elementos.Swap(posThis, otro.elementos, posOtro);
}

template<class T>
void Vector<T>::Swap(Nat pos1, Nat pos2) {
    assert(pos1 < longitud and pos2 < longitud);
    Swap(pos1, *this, pos2);
}

template<class T>
typename Vector<T>::const_Iterador Vector<T>::CrearIt() const {
    return const_Iterador(this);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& v) {
    return Mostrar(os, v, '<', '>');
}

template<class T>
bool operator==(const Vector<T>& v, const Vector<T>& w) {
    for(Nat p = 0; p < v.Tamanho() and p < w.Tamanho(); ++p)
        if(v[p] != w[p]) return false;
    return true;
}

}

#endif //AED2_VECTOR_H_INCLUDED
