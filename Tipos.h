#ifndef AED2_TIPOS_H_
#define AED2_TIPOS_H_

#include "aed2/TiposBasicos.h"
#include "aed2/Conj.h"

namespace aed2{

typedef Nat Prioridad;
typedef Nat Interfaz;
typedef Nat ID;
typedef String Computadora;
typedef Nat Paquete;
/***
 * Las restricciones son Strings con expresiones conformadas por operadores,
 * paréntesis y nombres de caracteristicas los cuales sólo son LETRAS del alfabeto
 * que pueden ser mayusculas o minusculas y SIN NUMEROS NI OTROS CARACTERES.
 * Puede haber cualquier cantidad de espacios entre nombres de caracteristicas y operadores
 * Los operadores son:
 * OR:  |
 * AND: &
 * NOT: !
 * Ejemplos (las comillas no forman parte de la restriccion):
 * "(tieneTurbo & ! tieneCincoPuertas) | tieneVTV"
 * "!estaPintadoDeRojo"
 * "!(tieneEscapeLibre & !esPolicia  )"
 *
 * Para no tener que parsear a mano la restriccion del string se provee una clase "ArbolSintactico" que a partir
 * de un string bien formado obtiene el arbol correspondiente.
 * Ver ArbolSintactico.h para mas datos.
 */

} // namespace aed2

#endif // AED2_TIPOS_H_
