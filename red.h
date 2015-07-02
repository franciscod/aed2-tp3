#include "Conj.h"
#include "Dicc.h"
#include "dicc_trie.h"
#include "Lista.h"

struct Compu{
	int ip;
	Conj <Nat> interfaces;
};

class Red{

  private:
  	struct NodoRed{
  		Compu* pc ;
		dicc_trie < Conj< lista < Compu > > > caminos: ;
		Dicc < int , *NodoRed > conexiones:;
  	};
 	Conj<Compu> compus;
 	dicc_trie dns <NodoRed>

 	void inicializarConjCaminos(const Compu* c);

  public:

	/*
	 * Crea una Red carrera
	 */	
	Red();

	void agregarComputadora( const Compu& c);

	void conectar(const Compu& c1, const Compu&c2, const int i1, const  int i2);

	Conj<Compu> computadoras();

	bool conectadas?(const Compu& c1, const Compu& c2);

	int interfazUsada( const Compu& c1, const Compu& c2);

	Conj<Compu> vecinos( const Compu& c);

	bool usaInterfaz( const Compu& c, const int i);

	Conj< Lista < Compu > > caminosMinimos( const Compu& c1, const Compu& c2);

	bool hayCamino( const Compu& c1, const Compu& c2); 

	Red copiar();

	bool operator==(const Red&) const;
};