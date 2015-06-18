/*
 * Author: Fede
 * Maintainer: tfischer
 * 
 * Conjunto (lineal) imlementado sobre Diccionario (Dicc.h),
 * que está implementado sobre listas enlazadas (Lista.h).
 * 
 * Created on 30 de octubre de 2010
 */

#ifndef AED2_CONJ_H_INCLUDED
#define	AED2_CONJ_H_INCLUDED

#include <iostream>
#include "Dicc.h"

namespace aed2
{

template<class T>
class Conj
{
  public:

    //forward declarations
    class Iterador;
    class const_Iterador;

    // Generadores

    Conj();
    Conj(const Conj<T>& otro);

    Iterador Agregar(const T& e);
    Iterador AgregarRapido(const T& e);

    void Eliminar(const T& e);

    // Observadores

    bool EsVacio() const;

    bool Pertenece(const T& e) const;

    Nat Cardinal() const;

    Iterador CrearIt();
    const_Iterador CrearIt() const;

    //esta funcion la agregamos aca, porque nos conviene acceder a
    //la representación.  Sino, no haria falta.
    bool operator == (const Conj<T>& otro) const;

    /************************************
    * Iterador de Conjunto, modificable *
    ************************************/
    class Iterador
    {
      public:

        Iterador();

        Iterador(const typename Conj<T>::Iterador& otra);

        Iterador& operator = (const typename Conj<T>::Iterador& otra);

        bool operator == (const typename Conj<T>::Iterador& otro) const;

        bool HaySiguiente() const;
        bool HayAnterior() const;

        const T& Siguiente() const;
        const T& Anterior() const;

        void Avanzar();
        void Retroceder();

        void EliminarAnterior();
        void EliminarSiguiente();

      private:
      
        typename Dicc<T,bool>::Iterador it_dicc_;

        Iterador(Conj<T>& c);

        Iterador(const typename Dicc<T,bool>::Iterador& itDict);

        friend class Conj<T>::const_Iterador;

        friend typename Conj<T>::Iterador Conj<T>::CrearIt();
        friend typename Conj<T>::Iterador Conj<T>::Agregar(const T&);
        friend typename Conj<T>::Iterador Conj<T>::AgregarRapido(const T&);
    };

    /*********************************************
    * const_Iterador de Conjunto, no modificable *
    *********************************************/
    class const_Iterador
    {
      public:

        const_Iterador();

        const_Iterador(const typename Conj<T>::Iterador& otra);

        const_Iterador(const typename Conj<T>::const_Iterador& otra);

        const_Iterador& operator = (const typename Conj<T>::const_Iterador& otra);

        bool operator==(const typename Conj<T>::const_Iterador& otro) const;

        bool HaySiguiente()const;
        bool HayAnterior()const;

        const T& Siguiente()const;
        const T& Anterior()const;

        void Avanzar();
        void Retroceder();

      private:

        typename Dicc<T,bool>::const_Iterador it_dicc_;

        const_Iterador(const Conj<T>& c);

        friend typename Conj<T>::const_Iterador Conj<T>::CrearIt() const;

    };

  private:

    Dicc<T,bool> dicc_;
};

template<class T>
std::ostream& operator<<(std::ostream&, const Conj<T>&);

template<class T>
bool operator==(const Conj<T>&, const Conj<T>&);

  //  Implementacion de Conjunto

template<class T>
Conj<T>::Conj()
{}

template<class T>
Conj<T>::Conj(const Conj<T>& otro)
  : dicc_( otro.dicc_ )
{}

template<class T>
typename Conj<T>::Iterador Conj<T>::Agregar(const T& e){
    return Iterador(dicc_.Definir(e, true));
}

template<class T>
typename Conj<T>::Iterador Conj<T>::AgregarRapido(const T& e){
    return Iterador(dicc_.DefinirRapido(e, true));
}

template<class T>
void Conj<T>::Eliminar(const T& e){
    if(Pertenece(e)) dicc_.Borrar(e);
}

template<class T>
bool Conj<T>::EsVacio() const{
    return dicc_.CantClaves()==0;
}

template<class T>
bool Conj<T>::Pertenece(const T& e) const{
    return dicc_.Definido(e);
}

template<class T>
Nat Conj<T>::Cardinal() const{
    return dicc_.CantClaves();
}

template<class T>
bool Conj<T>::operator==(const Conj<T>& otro) const {
    return dicc_ == otro.dicc_;
}

template<class T>
typename Conj<T>::Iterador Conj<T>::CrearIt(){
    return Iterador(*this);
}

template<class T>
typename Conj<T>::const_Iterador Conj<T>::CrearIt() const{
    return const_Iterador(*this);
}

  // Implementacion Iterador

template<class T>
Conj<T>::Iterador::Iterador(const typename Conj<T>::Iterador& otra)
  : it_dicc_(otra.it_dicc_)
{}

template<class T>
typename Conj<T>::Iterador& Conj<T>::Iterador::operator = (const typename Conj<T>::Iterador& otra)
{
  it_dicc_ = otra.it_dicc_;
  return *this;
}

template<class T>
Conj<T>::Iterador::Iterador()
{}

template<class T>
bool Conj<T>::Iterador::operator == (const typename Conj<T>::Iterador& otro) const
{
  return it_dicc_ == otro.it_dicc_;
}

template<class T>
bool Conj<T>::Iterador::HaySiguiente() const
{
  return it_dicc_.HaySiguiente();
}

template<class T>
bool Conj<T>::Iterador::HayAnterior() const
{
  return it_dicc_.HayAnterior();
}

template<class T>
const T& Conj<T>::Iterador::Siguiente() const
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  return it_dicc_.SiguienteClave();
}

template<class T>
const T& Conj<T>::Iterador::Anterior() const
{
  #ifdef DEBUG
  assert( HayAnterior() );
  #endif

  return it_dicc_.AnteriorClave();
}

template<class T>
void Conj<T>::Iterador::Avanzar()
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  it_dicc_.Avanzar();
}

template<class T>
void Conj<T>::Iterador::Retroceder()
{
  #ifdef DEBUG
  assert( HayAnterior() );
  #endif

  it_dicc_.Retroceder();
}

template<class T>
void Conj<T>::Iterador::EliminarAnterior()
{
  #ifdef DEBUG
  assert( HayAnterior() );
  #endif

  it_dicc_.EliminarAnterior();
}

template<class T>
void Conj<T>::Iterador::EliminarSiguiente()
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  it_dicc_.EliminarSiguiente();
}

template<class T>
Conj<T>::Iterador::Iterador(Conj<T>& c)
  : it_dicc_( c.dicc_.CrearIt() )
{}

template<class T>
Conj<T>::Iterador::Iterador(const typename Dicc<T,bool>::Iterador& iter)
  : it_dicc_(iter)
{}

  // Implementacion const_Iterador

template<class T>
Conj<T>::const_Iterador::const_Iterador()
{}

template<class T>
Conj<T>::const_Iterador::const_Iterador(const typename Conj<T>::Iterador& otra)
  : it_dicc_( otra.it_dicc_ )
{}

template<class T>
Conj<T>::const_Iterador::const_Iterador(const typename Conj<T>::const_Iterador& otra)
  : it_dicc_( otra.it_dicc_ )
{}

template<class T>
typename Conj<T>::const_Iterador& Conj<T>::const_Iterador::operator = (const typename Conj<T>::const_Iterador& otra)
{
  it_dicc_ = otra.it_dicc_;

  return *this;
}

template<class T>
bool Conj<T>::const_Iterador::operator == (const typename Conj<T>::const_Iterador& otro) const
{
  return it_dicc_ == otro.it_dicc_;
}

template<class T>
bool Conj<T>::const_Iterador::HaySiguiente() const
{
  return it_dicc_.HaySiguiente();
}

template<class T>
bool Conj<T>::const_Iterador::HayAnterior() const
{
  return it_dicc_.HayAnterior();
}

template<class T>
const T& Conj<T>::const_Iterador::Siguiente() const
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  return it_dicc_.SiguienteClave();
}

template<class T>
const T& Conj<T>::const_Iterador::Anterior() const
{
  #ifdef DEBUG
  assert( HayAnterior() );
  #endif

  return it_dicc_.AnteriorClave();
}

template<class T>
void Conj<T>::const_Iterador::Avanzar()
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  it_dicc_.Avanzar();
}

template<class T>
void Conj<T>::const_Iterador::Retroceder()
{
  #ifdef DEBUG
  assert( HayAnterior() );
  #endif

  it_dicc_.Retroceder();
}

template<class T>
Conj<T>::const_Iterador::const_Iterador(const Conj<T>& c)
  : it_dicc_( c.dicc_.CrearIt() )
{}

  // Otras implementaciones

template<class T>
bool operator==(const Conj<T>& c1, const Conj<T>& c2)
{
  return c1.operator==(c2);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Conj<T>& c)
{
  return Mostrar(os, c, '{', '}');
}

}

#endif	//AED2_CONJ_H_INCLUDED
