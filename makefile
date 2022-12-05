PROC	= $(ORACLE_HOME)/bin/PROC
PROC_CFLAGS = code=ANSI_C
PROCINCLUDES = include=include include=/usr/include sys_include/include include=../inc
PROCFLAGS = $(PROC_CFLAGS) $(PROCINCLUDES)

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