#include "ArbolSintactico.h"
#include "aed2/Lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/// NO MODIFICAR ESTE ARCHIVO ///

using namespace aed2;

////////////////////////////////////////////////////////////////
/// Metodos de Arbol Sintactico

ArbolSintactico::ArbolSintactico(String _raiz, ArbolSintactico *i, ArbolSintactico *d) :
    raiz(_raiz), izq(i), der(d) {}

ArbolSintactico::~ArbolSintactico() {
    if (izq) delete izq;
    if (der) delete der;
}

ArbolSintactico *ArbolSintactico::Bin(const String &raiz, ArbolSintactico *izq, ArbolSintactico *der) { return new ArbolSintactico(raiz,izq,der); }

String ArbolSintactico::aString() const
{
    if (!izq && !der)
        return raiz;

    if (izq && der)
        return "("+izq->aString()+raiz+der->aString()+")";

    return raiz + "("+izq->aString()+")";
}

////////////////////////////////////////////////////////////////
// CODIGO PARSER

/***
 * Parser adaptado de
 * http://en.literateprograms.org/Shunting_yard_algorithm_(C)
 */

// hooks de parsing que generan objetos ArbolSintactico

String hoja(const char *c) {
    String c1 = "";
    // parsea caracteristica mientras lea letras del alfabeto
    while (isalpha(*c)) c1+=*(c++);
    return c1;
}


ArbolSintactico* eval_not(ArbolSintactico * e1, ArbolSintactico * e2) {
    return ArbolSintactico::Bin("!",e1,0);
}

ArbolSintactico* eval_and(ArbolSintactico* e1, ArbolSintactico* e2) {
    return ArbolSintactico::Bin("&",e1,e2);
}

ArbolSintactico* eval_or(ArbolSintactico* e1, ArbolSintactico* e2) {
    return ArbolSintactico::Bin("|",e1,e2);
}

enum {ASSOC_NONE=0, ASSOC_LEFT, ASSOC_RIGHT};

struct op_s {
    char op;
    int prec;
    int assoc;
    int unary;
    ArbolSintactico* (*eval)(ArbolSintactico* a1, ArbolSintactico* a2);
};

// operadores aceptados por nuestras restricciones
// declaracion de los hooks a usar
op_s ops[]={
    {'!', 10, ASSOC_RIGHT, 1, eval_not},
    {'&', 9, ASSOC_LEFT, 0, eval_and},
    {'|', 9, ASSOC_LEFT, 0, eval_or},
    {'(', 0, ASSOC_NONE, 0, NULL},
    {')', 0, ASSOC_NONE, 0, NULL}
};

// El algoritmo de parsing usa dos pilas: una de operandos y otra de subArbolSintacticoes ya parseados
Lista<op_s *> opstack;
Lista<ArbolSintactico*> exprstack;

struct op_s *getop(char ch) {
    unsigned int i;
    for(i=0; i<sizeof ops/sizeof ops[0]; ++i) {
        if(ops[i].op==ch) return ops+i;
    }
    return NULL;
}

void push_opstack(struct op_s *op) {
    opstack.AgregarAtras(op);
}

struct op_s *pop_opstack() {
    if(opstack.EsVacia()) {
        std::cerr << "ERROR: pila de operadores vacía" << std::endl;
        exit(EXIT_FAILURE);
    }
    op_s* ret = opstack.Ultimo();
    opstack.Comienzo();
    return ret;
}

void push_exprstack(ArbolSintactico* num) {
    exprstack.AgregarAtras(num);
}

ArbolSintactico* pop_exprstack() {
    if(exprstack.EsVacia()) {
        std::cerr << "ERROR: Pila de arboles vacía" << std::endl;
        exit(EXIT_FAILURE);
    }
    ArbolSintactico* ret = exprstack.Ultimo();
    exprstack.Comienzo();
    return ret;
}

void shunt_op(struct op_s *op)
{
    struct op_s *pop;
    ArbolSintactico *n1, *n2;
    if(op->op=='(') {
        push_opstack(op);
        return;
    } else if(op->op==')') {
        while(!opstack.EsVacia() && opstack.Ultimo()->op!='(') {
            pop=pop_opstack();
            n1=pop_exprstack();
            if(pop->unary) push_exprstack(pop->eval(n1, 0));
            else {
                n2=pop_exprstack();
                push_exprstack(pop->eval(n2, n1));
            }
        }
        if(!(pop=pop_opstack()) || pop->op!='(') {
            std::cerr << "ERROR: Error de pila. Falta paréntesis \'(\'" << std::endl;
            exit(EXIT_FAILURE);
        }
        return;
    }

    if(op->assoc==ASSOC_RIGHT) {
        while(!opstack.EsVacia() && op->prec<opstack.Ultimo()->prec) {
            pop=pop_opstack();
            n1=pop_exprstack();
            if(pop->unary) push_exprstack(pop->eval(n1, 0));
            else {
                n2=pop_exprstack();
                push_exprstack(pop->eval(n2, n1));
            }
        }
    } else {
        while(!opstack.EsVacia() && op->prec<=opstack.Ultimo()->prec) {
            pop=pop_opstack();
            n1=pop_exprstack();
            if(pop->unary) push_exprstack(pop->eval(n1, 0));
            else {
                n2=pop_exprstack();
                push_exprstack(pop->eval(n2, n1));
            }
        }
    }
    push_opstack(op);
}

ArbolSintactico* ArbolSintactico::LeerDeString(const String& exp)
{
    const char *expr;
    const char *tstart=NULL;
    exprstack = Lista<ArbolSintactico*>();
    opstack = Lista<op_s*>();
    struct op_s startop={'X', 0, ASSOC_NONE, 0, NULL};	/* Dummy operator to mark start */
    struct op_s *op=NULL;
    ArbolSintactico *n1, *n2;
    struct op_s *lastop=&startop;

    for(expr=exp.c_str(); *expr; ++expr) {
        if(!tstart) {

            if((op=getop(*expr))) {
                if(lastop && (lastop==&startop || lastop->op!=')')) {
                    if(op->op=='!') op=getop('!');
                    else if(op->op!='(') {
                        std::cerr << "ERROR: Uso ilegal del operador binario ("<< op->op << ")";
                        exit(EXIT_FAILURE);
                    }
                }
                shunt_op(op);
                lastop=op;
            } else if(isalpha(*expr)) tstart=expr;
            else if(!isspace(*expr)) {
                std::cerr << "ERROR DE SINTAXIS." << std::endl;
                return 0;
            }
        } else {
            if(isspace(*expr)) {
                push_exprstack(ArbolSintactico::Bin(hoja(tstart),0,0));
                tstart=NULL;
                lastop=NULL;
            } else if((op=getop(*expr))) {
                push_exprstack(ArbolSintactico::Bin(hoja(tstart),0,0));
                tstart=NULL;
                shunt_op(op);
                lastop=op;
            } else if(!isalpha(*expr)) {
                std::cerr << "ERROR DE SINTAXIS." << std::endl;
                return 0;
            }
        }
    }
    if(tstart) push_exprstack(ArbolSintactico::Bin(hoja(tstart),0,0));

    while(!opstack.EsVacia()) {
        op=pop_opstack();
        n1=pop_exprstack();
        if(op->unary) push_exprstack(op->eval(n1, 0));
        else {
            n2=pop_exprstack();
            push_exprstack(op->eval(n2, n1));
        }
    }
    if(exprstack.Longitud()!=1) {
        std::cerr << "ERROR: la pila de árboles tiene " << exprstack.Longitud() << " elementos luego de parsear. Debería tener 1." << std::endl;
        return 0;
    }

    return exprstack[0];
}

