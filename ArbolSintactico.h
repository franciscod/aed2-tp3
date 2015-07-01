#ifndef ARBOL_SINTACTICO_H
#define ARBOL_SINTACTICO_H

/// NO MODIFICAR ESTE ARCHIVO ///

#include "Tipos.h"
#include "aed2/TiposBasicos.h"

#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>

namespace aed2 {

/**
 * @brief Arbol sintáctico de una expresión conformada por palabras (sólo letras mayúsculas/minúsculas)
 * unidas con los operadores * | & ! ( )
 * Algunos strings parseables VALIDOS:
 * (rojo | Amarillo) & !VERDE
 * azul
 * !verde
 *
 * Algunos ejemplos de strings INVALIDOS:
 * rojo123
 * (azul
 * verde rojo
 * amarillo !
 *
 * IMPORTANTE: A partir de un string VALIDO se puede obtener el ArbolSintactico que lo representa mediante
 * el método LeerDeString. Esta es la UNICA MANERA de crear un ArbolSintactico.
 */
class ArbolSintactico {
public:
    String raiz;
    ArbolSintactico* izq, *der;

    /*********************************************************************************
     * Funcion principal : lectura de una expresión
     * ArbolSintactico::LeerString lee un String bien formado y genera su ArbolSintactico
     * correspondiente.
     *
     * Notar que ArbolSintactico::LeerDeString DEVUELVE UN PUNTERO (reserva memoria nueva).
     * Para no perder memoria tendrán que destruir el ArbolSintactico cuando no sea más necesario
     * (con delete)
     */
    static ArbolSintactico* LeerDeString(const String& exp);

    // Imprime una versión textual de la expresión. Puede tener más paréntesis que la original.
    String aString() const;

    // Funcion constructora usada por el parser de expresiones
    // Combina dos arboles sintácticos con la raiz dada.
    // Hace ALIASING de los dos arboles pasados.
    static ArbolSintactico* Bin(const String& raiz, ArbolSintactico* izq, ArbolSintactico* der);

    ~ArbolSintactico();

private:
    // Constructor unico de bajo nivel. PRIVADO: No se puede llamar directamente.
    ArbolSintactico(String raiz, ArbolSintactico* izq, ArbolSintactico* der);
};

}

#endif // ARBOL_SINTACTICO_H
