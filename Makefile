all: clean build tests

clean:
	rm -f tests

build:
	g++ -g -o tests -Wall -fmessage-length=0 tests.cpp

tests: build
	./tests

valgrind: build
	valgrind --leak-check=full --track-origins=yes ./tests

# Compilador que será utilizado
CC=g++

# flags de compilación
CFLAGS=-g -Wall

# flags de linkeo
LDFLAGS=

# Agrear acá los directorios a incluir en la compilación
INCDIR=. #../../../../../Modulos-CPP

# Agregar acá los archivos .cpp a compilar
SOURCES=test.cpp Driver.cpp ArbolSintactico.cpp 

# Objetos que serán generados (no tocar)
OBJECTS=$(SOURCES:.cpp=.o)

# Nombre del ejecutable a generar
EXECUTABLE=test

#DESCOMENTAME all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

#DESCOMENTAME clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

.cpp.o:
	$(CC) -I$(INCDIR) $(CFLAGS) $< -c -o $@
