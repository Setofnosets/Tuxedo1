# Path: makefile
BIN=./bin
SRC=./src
INC=./inc
FML=./fml
CC=gcc

all: campos_fml cliTuxedo servTuxedo

#Campos fml
campos_fml: $(FML)/biblioOper.fml
	mkfldhdr32 $(FML)/biblioOper.fml
	mv biblioOper.fml.h $(INC)

#Generacion de clientes
cliTuxedo: $(SRC)/Cliente.c
	@echo "Generando clientes Tuxedo"
	buildclient -o $(BIN)/Cliente -f $(SRC)/Cliente.c

#Generacion del Servidor
servTuxedo: $(SRC)/Servidor.c
	@echo "Generando servidor Tuxedo"
	buildserver -o $(BIN)/Servidor -f $(SRC)/Servidor.c -s imprimeLista -f -lmysqlclient