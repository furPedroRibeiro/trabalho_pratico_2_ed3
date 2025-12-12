OBJ = ./obj
SRC = ./src
HDR = ./headers
APP = ./app

all: libed myapps

libed:
		gcc -ggdb -c $(SRC)/funcionalidade_1.c -I $(HDR) -o $(OBJ)/funcionalidade_1.o
		gcc -ggdb -c $(SRC)/funcionalidade_2.c -I $(HDR) -o $(OBJ)/funcionalidade_2.o
		gcc -ggdb -c $(SRC)/funcionalidade_3.c -I $(HDR) -o $(OBJ)/funcionalidade_3.o
		gcc -ggdb -c $(SRC)/funcionalidade_4.c -I $(HDR) -o $(OBJ)/funcionalidade_4.o
		gcc -ggdb -c $(SRC)/funcionalidade_5.c -I $(HDR) -o $(OBJ)/funcionalidade_5.o
		gcc -ggdb -c $(SRC)/funcionalidade_6.c -I $(HDR) -o $(OBJ)/funcionalidade_6.o
		gcc -ggdb -c $(SRC)/funcionalidade_7.c -I $(HDR) -o $(OBJ)/funcionalidade_7.o
		gcc -ggdb -c $(SRC)/funcionalidade_8.c -I $(HDR) -o $(OBJ)/funcionalidade_8.o
		gcc -ggdb -c $(SRC)/funcionalidade_9.c -I $(HDR) -o $(OBJ)/funcionalidade_9.o
		gcc -ggdb -c $(SRC)/funcionalidade_10.c -I $(HDR) -o $(OBJ)/funcionalidade_10.o
		gcc -ggdb -c $(SRC)/geral.c -I $(HDR) -o $(OBJ)/geral.o
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

zip: 
		zip -r trabPratico.zip src/ headers/ app/ Makefile