OBJ = ./obj
SRC = ./src
HDR = ./headers
APP = ./app

all: libed myapps

libed:
		gcc -ggdb -c $(SRC)/functions.c -I $(HDR) -o $(OBJ)/functions.o
		gcc -ggdb -c $(SRC)/auxiliar.c -I $(HDR) -o $(OBJ)/auxiliar.o
		gcc -ggdb -c $(SRC)/utilidades.c -I $(HDR) -o $(OBJ)/utilidades.o

myapps:
		gcc -ggdb $(APP)/main.c $(OBJ)/*.o -I $(HDR) -o ./trabPratico

run:
		./trabPratico

clean:
		rm $(OBJ)/*
		rm ./trabPratico
		
cleanbin:
		rm ./*.bin

cleanzip:
		rm ./*.zip

cleanall: clean cleanbin cleanzip