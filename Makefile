# Compilador que será utilizado
CC=g++

# flags de compilación
CFLAGS=-g -Wall

# flags de linkeo
LDFLAGS=

# Agrear acá los directorios a incluir en la compilación
INCDIR=. #../../../../../Modulos-CPP

# Agregar acá los archivos .cpp a compilar
# TODO: Descomentar el Driver.cpp cuando esté listo
SOURCES=test.cpp Red.cpp#Driver.cpp

# Objetos que serán generados (no tocar)
OBJECTS=$(SOURCES:.cpp=.o)

# Nombre del ejecutable a generar
EXECUTABLE=test

all: $(SOURCES) tests

$(EXECUTABLE): clean $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

tests: $(EXECUTABLE)
	./$(EXECUTABLE)

valgrind: $(EXECUTABLE)
	valgrind --leak-check=full --track-origins=yes ./$(EXECUTABLE)

.cpp.o:
	$(CC) -I$(INCDIR) $(CFLAGS) $< -c -o $@
