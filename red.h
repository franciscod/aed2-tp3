#include "Conj.h"
#include "Dicc.h"
#include "dicc_trie.h"
#include "Lista.h"

class Red{

  private:

 	Conj<Compu> compus;
 	dicc_trie dns <NodoRed>

 	void inicializarConjCaminos(Red r,  const Compu c);

  public:

	/*
	 * Crea una Red carrera
	 */	
	Red();

	void agregarComputadora( Red r,  const Compu c);

	void conectar(Red r, const Compu c1, const Compu c2, const int i1, const int i2);

	Conj<Compu> computadoras(const Red r);

	bool conectadas?(const Red r, const Compu c1, const Compu c2);

	int interfazUsada(const Red r, const Compu c1, const Compu c2);

	Conj<Compu> vecinos(const Red r, const Compu c);

	bool usaInterfaz(const Red r, const Compu c, const int i);

	Conj< Lista < Compu > > caminosMinimos(const Red r, const Compu c1, const Compu c2);

	bool hayCamino(const Red r, const Compu c1, const Compu c2); 

	Red copiar(const Red r);

	bool operator==(const Red) const;
}