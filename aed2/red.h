class Red{

  public:

	/*
	 * Crea una Red carrera
	 */	
	IniciarRed();

	void agregarComputadora( Red r,  const compu c);

	void conectar(Red r, const compu c1, const compu c2, const int i1, const int i2);

	Conj<compu> computadoras(const Red r);

	bool conectadas?(const Red r, const compu c1, const compu c2);

	int interfazUsada(const Red r, const compu c1, const compu c2);

	Conj <compu> vecinos(const Red r, const compu c);

	bool usaInterfaz?(const Red r, const compu c, const int i);

	Conj< Lista < compu > > caminosMinimos(const Red r, const compu c1, const compu c2);

	bool hayCamino?(const Red r, const compu c1, const compu c2); 

	Red copiar(const Red r);

	bool operator==(const Red) const;
}