/*
 * Author: ?
 *
 * Lista doblemente enlazada.
 * 
 */

#ifndef AED2_LISTA_H_
#define AED2_LISTA_H_

#include <ostream>
#include "TiposBasicos.h"

namespace aed2
{

template <typename T>
class Lista
{
  private:

    // forward declarations
    // La estructura esta definidas mas abajo
    struct Nodo;

  public:

  // forward declarations
  // Las clases estan definidas mas abajo
  class Iterador;
  class const_Iterador;

  /// Crea una lista enlazada vacía. (Operación Vacia())
  Lista();

  /// Crea por copia una lista enlazada (operación Copiar())
  Lista(const Lista& otra);

  /// Destruye la lista, incluyendo los T alojados
  ~Lista();

  /// Operacion de asignacion
  Lista<T>& operator=(const Lista<T>& otra);

  /// Opreaciones básicas
  bool EsVacia() const;
  Nat Longitud() const;

  /// Acceso al primero y último elemento (en O(1) )
  /// Versiones modificables y no modificables
  T& Primero();
  const T& Primero() const;

  T& Ultimo();
  const T& Ultimo() const;

  /// Operador "iésimo"
  T& operator[](Nat i);
  const T& operator[](Nat i) const;

  /// Elimina el primer elemento
  void Fin();
  /// Elimina el último elemento
  void Comienzo();

  /// Inserción de elementos
  Iterador AgregarAdelante(const T& elem);
  Iterador AgregarAtras(const T& elem);

  /// Creación del Iterador
  Iterador CrearIt();
  Iterador CrearItUlt();

  /// Creación del const_Iterador
  const_Iterador CrearIt() const;
  const_Iterador CrearItUlt() const;

  /**********************************
   * Iterador de Lista, modificable *
   **********************************/
  class Iterador
  {
    public:

      Iterador();

      Iterador(const typename Lista<T>::Iterador& otro);

      Iterador& operator = (const typename Lista<T>::Iterador& otro);

      bool HayAnterior() const;
      bool HaySiguiente() const;

      T& Anterior();
      T& Siguiente();
      const T& Anterior() const;
      const T& Siguiente() const;

      void Avanzar();
      void Retroceder();

      void AgregarComoAnterior(const T& elem);
      void AgregarComoSiguiente(const T& elem);

      void EliminarAnterior();
      void EliminarSiguiente();

      bool operator == (const typename Lista<T>::Iterador& otro) const;

  private:

      Lista<T>* lista_;

      typename Lista<T>::Nodo* nodo_siguiente_;

      /// El constructor es privado, necesitamos el friend.
      Iterador(Lista<T>* lista, typename Lista<T>::Nodo* proximo);

      friend typename Lista<T>::Iterador Lista<T>::CrearIt();
      friend typename Lista<T>::Iterador Lista<T>::CrearItUlt();
      friend class Lista<T>::const_Iterador;

      // devuelve el nodo siguiente en la lista circular
      typename Lista<T>::Nodo* SiguienteReal() const;
  };

  /*******************************************
   * const_Iterador de Lista, no modificable *
   *******************************************/
  class const_Iterador
  {
    public:

      const_Iterador();

      const_Iterador(const typename Lista<T>::const_Iterador& otro);

      const_Iterador(const typename Lista<T>::Iterador& otro);

      const_Iterador& operator = (const typename Lista<T>::const_Iterador& otro);

      bool HayAnterior() const;
      bool HaySiguiente() const;

      const T& Anterior() const;
      const T& Siguiente() const;

      void Avanzar();
      void Retroceder();

      bool operator == (const typename Lista<T>::const_Iterador& otro) const;

  private:

      const Lista<T>* lista_;

      const typename Lista<T>::Nodo* nodo_siguiente_;

      /// El constructor es privado, necesitamos el friend.
      const_Iterador(const Lista<T>* lista, const typename Lista<T>::Nodo* proximo);

      friend typename Lista<T>::const_Iterador Lista<T>::CrearIt() const;
      friend typename Lista<T>::const_Iterador Lista<T>::CrearItUlt() const;

      //devuelve el nodo siguiente en la lista circular
      const typename Lista<T>::Nodo* SiguienteReal() const;
  };

private:

  struct Nodo
  {
    Nodo(const T& d) : dato(d), anterior(NULL), siguiente(NULL) {};

    T dato;
    Nodo* anterior;
    Nodo* siguiente;
  };

  Nodo* primero_;
  Nat longitud_;
};

template<class T>
std::ostream& operator << (std::ostream& os, const Lista<T>& l);

template<class T>
bool operator == (const Lista<T>& l1, const Lista<T>& k2);

  //  Implementacion de Lista

template <typename T>
Lista<T>::Lista()
  : primero_(NULL), longitud_(0)
{}

template <typename T>
Lista<T>::Lista(const Lista<T>& otra)
  : primero_(NULL), longitud_(0)
{
  // TODO la idea es que haga shallow copy?
  // Pense que por convencon definiamos este operador con deep copy
  // de hecho el operador = hace deep copy
  *this = otra;
}

template <typename T>
Lista<T>& Lista<T>::operator = (const Lista<T>& otra)
{
  // Limpio mi lista
  while( !EsVacia() ) {
    Fin();
  }

  // copio todos los elementos de la otra
  for(const_Iterador it = otra.CrearIt(); it.HaySiguiente(); it.Avanzar()) {
    this->AgregarAtras( it.Siguiente() );
  }

  return *this;
}

template <typename T>
Lista<T>::~Lista()
{
  while(!EsVacia()) {
    Fin();
  }
}

template <typename T>
bool Lista<T>::EsVacia() const
{
  return primero_ == NULL;
}

template <typename T>
Nat Lista<T>::Longitud() const
{
  return longitud_;
}

template <typename T>
T& Lista<T>::Primero()
{
  #ifdef DEBUG
  assert( not EsVacia() );
  #endif

  return CrearIt().Siguiente();
}

template <typename T>
const T& Lista<T>::Primero() const
{
  #ifdef DEBUG
  assert( not EsVacia() );
  #endif

  return CrearIt().Siguiente();
}

template <typename T>
T& Lista<T>::Ultimo()
{
  #ifdef DEBUG
  assert( not EsVacia() );
  #endif

  return CrearItUlt().Anterior();
}

template <typename T>
const T& Lista<T>::Ultimo() const
{
  #ifdef DEBUG
  assert( not EsVacia() );
  #endif

  return CrearItUlt().Anterior();
}

template <typename T>
const T& Lista<T>::operator[](Nat i) const
{
  #ifdef DEBUG
  assert( i < longitud_ );
  #endif

  const_Iterador it = CrearIt();
  while( i > 0 ) {
    it.Avanzar();
    --i;
  }

  return it.Siguiente();
}

template <typename T>
T& Lista<T>::operator[](Nat i)
{
  #ifdef DEBUG
  assert( i < longitud_ );
  #endif

  Iterador it = CrearIt();
  while(i > 0)
  {
    it.Avanzar();
    --i;
  }

  return it.Siguiente();
}

template <typename T>
void Lista<T>::Fin()
{
  #ifdef DEBUG
  assert( not EsVacia() );
  #endif

  CrearIt().EliminarSiguiente();
}

template <typename T>
void Lista<T>::Comienzo()
{
  #ifdef DEBUG
  assert( not EsVacia() );
  #endif

  CrearItUlt().EliminarAnterior();
}

template <typename T>
typename Lista<T>::Iterador Lista<T>::AgregarAdelante(const T& dato)
{
  typename Lista<T>::Iterador res =  CrearIt();
  res.AgregarComoSiguiente(dato);
  return res;
}

template <typename T>
typename Lista<T>::Iterador Lista<T>::AgregarAtras(const T& dato)
{
    typename Lista<T>::Iterador res = CrearItUlt();
    res.AgregarComoSiguiente(dato);
    return res;
}

template <typename T>
typename Lista<T>::Iterador Lista<T>::CrearIt()
{
  return Iterador(this, primero_);
}

template <typename T>
typename Lista<T>::Iterador Lista<T>::CrearItUlt()
{
  return Iterador(this, NULL);
}

template <typename T>
typename Lista<T>::const_Iterador Lista<T>::CrearIt() const
{
  return const_Iterador(this, primero_);
}

template <typename T>
typename Lista<T>::const_Iterador Lista<T>::CrearItUlt() const
{
  return const_Iterador(this, NULL);
}

  // Implementacion Iterador

template <typename T>
Lista<T>::Iterador::Iterador()
  : lista_(NULL), nodo_siguiente_(NULL)
{}

template <typename T>
Lista<T>::Iterador::Iterador(const typename Lista<T>::Iterador& otro)
  : lista_(otro.lista_), nodo_siguiente_(otro.nodo_siguiente_)
{}

template <typename T>
typename Lista<T>::Iterador& Lista<T>::Iterador::operator= (const typename Lista<T>::Iterador& otro)
{
  lista_ = otro.lista_;
  nodo_siguiente_ = otro.nodo_siguiente_;

  return *this;
}

template <typename T>
bool Lista<T>::Iterador::HaySiguiente() const
{
  return nodo_siguiente_ != NULL;
}

template <typename T>
bool Lista<T>::Iterador::HayAnterior() const
{
  return nodo_siguiente_ != lista_->primero_;
}

template <typename T>
T& Lista<T>::Iterador::Siguiente()
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  return nodo_siguiente_->dato;
}

template <typename T>
T& Lista<T>::Iterador::Anterior()
{
  #ifdef DEBUG
  assert(HayAnterior());
  #endif

  return SiguienteReal()->anterior->dato;
}

template <typename T>
const T& Lista<T>::Iterador::Siguiente() const
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  return nodo_siguiente_->dato;
}

template <typename T>
const T& Lista<T>::Iterador::Anterior() const
{
  #ifdef DEBUG
  assert(HayAnterior());
  #endif

  return SiguienteReal()->anterior->dato;
}

template <typename T>
void Lista<T>::Iterador::Avanzar()
{
  #ifdef DEBUG
  assert(HaySiguiente());
  #endif

  nodo_siguiente_ = nodo_siguiente_->siguiente;

  if(nodo_siguiente_ == lista_->primero_) {
    nodo_siguiente_ = NULL;
  }
}

template <typename T>
void Lista<T>::Iterador::Retroceder()
{
  #ifdef DEBUG
  assert(HayAnterior());
  #endif

  nodo_siguiente_ = SiguienteReal()->anterior;
}

template <typename T>
void Lista<T>::Iterador::AgregarComoAnterior(const T& dato)
{
  Nodo* sig = SiguienteReal();
  Nodo* nuevo = new Nodo(dato);

  //asignamos anterior y siguiente de acuerdo a si el nodo es el primero
  //o no de la lista circular
  nuevo->anterior = sig == NULL ? nuevo : sig->anterior;
  nuevo->siguiente = sig == NULL ? nuevo : sig;

  //reencadenamos los otros nodos (notar que no hay problema cuando nuevo
  //es el primer nodo creado de la lista)
  nuevo->anterior->siguiente = nuevo;
  nuevo->siguiente->anterior = nuevo;

  //cambiamos el primero en el caso que nodo_siguiente == primero
  if(nodo_siguiente_ == lista_->primero_) {
    lista_->primero_ = nuevo;
  }

  lista_->longitud_++;
}

template <typename T>
void Lista<T>::Iterador::AgregarComoSiguiente(const T& dato)
{
  AgregarComoAnterior(dato);
  Retroceder();
}

template <typename T>
void Lista<T>::Iterador::EliminarAnterior()
{
  #ifdef DEBUG
  assert( HayAnterior() );
  #endif

  Retroceder();
  EliminarSiguiente();
}

template <typename T>
void Lista<T>::Iterador::EliminarSiguiente()
{
  #ifdef DEBUG
  assert(HaySiguiente());
  #endif

  Nodo* tmp = nodo_siguiente_;

  //reencadenamos los nodos
  tmp->siguiente->anterior = tmp->anterior;
  tmp->anterior->siguiente = tmp->siguiente;

  //borramos el unico nodo que habia?
  nodo_siguiente_ = tmp->siguiente == tmp ? NULL : tmp->siguiente;
  //borramos el último?
  nodo_siguiente_ = tmp->siguiente == lista_->primero_ ? NULL : tmp->siguiente;

  //borramos el primero?
  if(tmp == lista_->primero_) {
    lista_->primero_ = nodo_siguiente_;
  }

  delete tmp;

  lista_->longitud_--;
}

template<class T>
bool Lista<T>::Iterador::operator == (const typename Lista<T>::Iterador& otro) const {
  return lista_ == otro.lista_ && nodo_siguiente_ == otro.nodo_siguiente_;
}

template<class T>
Lista<T>::Iterador::Iterador(Lista<T>* lista, typename Lista<T>::Nodo* proximo)
  : lista_(lista), nodo_siguiente_(proximo)
{};

template <typename T>
typename Lista<T>::Nodo* Lista<T>::Iterador::SiguienteReal() const
{
  return nodo_siguiente_ == NULL ? lista_->primero_ : nodo_siguiente_;
}

  // Implementacion const_Iterador

template <typename T>
Lista<T>::const_Iterador::const_Iterador()
  : lista_(NULL), nodo_siguiente_(NULL)
{}

template <typename T>
Lista<T>::const_Iterador::const_Iterador(const typename Lista<T>::const_Iterador& otro)
  : lista_(otro.lista_), nodo_siguiente_(otro.nodo_siguiente_)
{}

template <typename T>
Lista<T>::const_Iterador::const_Iterador(const typename Lista<T>::Iterador& otro)
  : lista_(otro.lista_), nodo_siguiente_(otro.nodo_siguiente_)
{}

template <typename T>
typename Lista<T>::const_Iterador& Lista<T>::const_Iterador::operator = (const typename Lista<T>::const_Iterador& otro)
{
  lista_ = otro.lista_;
  nodo_siguiente_ = otro.nodo_siguiente_;

  return *this;
}

template <typename T>
bool Lista<T>::const_Iterador::HaySiguiente() const
{
  return nodo_siguiente_ != NULL;
}

template <typename T>
bool Lista<T>::const_Iterador::HayAnterior() const
{
  return nodo_siguiente_ != lista_->primero_;
}

template <typename T>
const T& Lista<T>::const_Iterador::Siguiente() const
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  return nodo_siguiente_->dato;
}

template <typename T>
const T& Lista<T>::const_Iterador::Anterior() const
{
  #ifdef DEBUG
  assert(HayAnterior());
  #endif

  return SiguienteReal()->anterior->dato;
}

template <typename T>
void Lista<T>::const_Iterador::Avanzar()
{
  #ifdef DEBUG
  assert(HaySiguiente());
  #endif

  nodo_siguiente_ = nodo_siguiente_->siguiente;

  if(nodo_siguiente_ == lista_->primero_) {
    nodo_siguiente_ = NULL;
  }
}

template <typename T>
void Lista<T>::const_Iterador::Retroceder()
{
  #ifdef DEBUG
  assert( HayAnterior() );
  #endif

  nodo_siguiente_ = SiguienteReal()->anterior;
}

template<class T>
bool Lista<T>::const_Iterador::operator == (const typename Lista<T>::const_Iterador& otro) const {
  return lista_ == otro.lista_ && nodo_siguiente_ == otro.nodo_siguiente_;
}

template <typename T>
Lista<T>::const_Iterador::const_Iterador(const Lista<T>* lista, const typename Lista<T>::Nodo* proximo)
  : lista_(lista), nodo_siguiente_(proximo)
{}

template <typename T>
const typename Lista<T>::Nodo* Lista<T>::const_Iterador::SiguienteReal() const
{
  return nodo_siguiente_ == NULL ? lista_->primero_ : nodo_siguiente_;
}

  // Otros

template <typename T>
std::ostream& operator<<(std::ostream& os, const Lista<T>& lista)
{
  return Mostrar(os, lista, '<', '>');
}

template <typename T>
bool operator == (const Lista<T>& l1, const Lista<T>& l2)
{
  typename Lista<T>::const_Iterador it1 = l1.CrearIt();
  typename Lista<T>::const_Iterador it2 = l2.CrearIt();

  while(it1.HaySiguiente() and it2.HaySiguiente() and it1.Siguiente() == it2.Siguiente()) {
    it1.Avanzar(); it2.Avanzar();
  }

  return not it1.HaySiguiente() and not it2.HaySiguiente();
}

}

#endif // AED2_LISTA_H_INCLUDED
