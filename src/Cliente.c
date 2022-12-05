#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fml32.h>
#include "../inc/biblioOper.fml.h"
#include "atmi.h"

typedef struct Universidad{
    int Codigo;
    char Grupo[10];
    char Materia[50];
    int Creditos;
    int Trimestre;
    char NombreProfesor[50];
}Universidad;

int leerArchivo(char* nombreArchivo, Universidad* Universidad);
int insertarUniversidad(Universidad* Universidad, int cantidad);
int numeroLineas(char* nombreArchivo);
int imprimeLista();

int main() {
    Universidad Universidad[numeroLineas("Universidad.csv")];
    leerArchivo("Universidad.csv", Universidad);
    imprimeLista();
    return 0;
}

int leerArchivo(char* nombreArchivo, Universidad* Universidad){
    FILE *archivo;
    char *token;
    char *linea = NULL;
    size_t buffer = 0;
    linea = malloc(sizeof(char)*buffer);
    int i = 0;
    int lineaActual = 0;
    archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error de apertura del archivo. \n\n");
        return -1;
    } else {
        while (getline(&linea, &buffer, archivo) != -1) {
            token = strtok(linea, ",");
            while (token != NULL) {
                switch (i) {
                    case 0:
                        Universidad[lineaActual].Codigo = atoi(token);
                        break;
                    case 1:
                        strcpy(Universidad[lineaActual].Grupo, token);
                        break;
                    case 2:
                        strcpy(Universidad[lineaActual].Materia, token);
                        break;
                    case 3:
                        Universidad[lineaActual].Creditos = atoi(token);
                        break;
                    case 4:
                        Universidad[lineaActual].Trimestre = atoi(token);
                        break;
                    case 5:
                        token[strcspn(token, "\r\n")] = 0;
                        strcpy(Universidad[lineaActual].NombreProfesor, token);
                        break;
                }
                token = strtok(NULL, ",");
                i++;
            }
            i = 0;
            lineaActual++;
        }
    }
    fclose(archivo);
    insertarUniversidad(Universidad, numeroLineas("Universidad.csv"));
    return 0;
}

int numeroLineas(char* nombreArchivo){
    FILE *archivo;
    char *linea = NULL;
    size_t buffer = 0;
    int numeroLineas = 0;
    archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error de apertura del archivo. \n\n");
        return -1;
    } else {
        while (getline(&linea, &buffer, archivo) != -1) {
            numeroLineas++;
        }
    }
    fclose(archivo);
    return numeroLineas;
}

int imprimeLista(){
    FBFR32 *fbfr;
    FBFR32 *recv;
    FLDLEN32 flen;
    char msgbuf[1024];
    printf("Contectando con el servidor...\n");
    if(tpinit((TPINIT *)NULL) == -1){
        printf("Error en la conexion con el servidor, tperrno = %d\n", tperrno);
        return 1;
    }

    //Reservacion de memoria del buffer
    if ((fbfr = (FBFR32 *) tpalloc("FML32", NULL, 1024)) == NULL){
        printf("Error Reservando espacio para Buffer fbfr\n");
        tpterm();
        return(1);
    }

    if ((recv = (FBFR32 *) tpalloc("FML32", NULL, 1024)) == NULL){
        printf("Error Reservando espacio para Buffer recv\n");
        tpterm();
        return(1);
    }

    if (tpcall("LISTA", (char *)fbfr, 0, (char **)&recv, &flen, 0) == -1){
        printf("Error en la llamada al servicio LISTA, tperrno = %d\n", tperrno);
        tpfree((char *)fbfr);
        tpfree((char *)recv);
        tpterm();
        return(1);
    }

    flen = sizeof(msgbuf);
    Fget32(recv, OUTPUT, 0, (char *)msgbuf, &flen);
    printf("Respuesta del servidor: %s\n", msgbuf);

    tpfree((char *)fbfr);
    tpfree((char *)recv);
    tpterm();
    return 0;
}

int insertarUniversidad(Universidad* Universidad, int cantidad){
    FBFR32 *fbfr;
    FBFR32 *recv;
    FLDLEN32 flen;
    char msgbuf[1024];
    int i = 0;
    printf("Contectando con el servidor...\n");
    if(tpinit((TPINIT *)NULL) == -1){
        printf("Error en la conexion con el servidor, tperrno = %d\n", tperrno);
        return 1;
    }

    //Reservacion de memoria del buffer
    if ((fbfr = (FBFR32 *) tpalloc("FML32", NULL, 1024)) == NULL){
        printf("Error Reservando espacio para Buffer fbfr\n");
        tpterm();
        return(1);
    }

    if ((recv = (FBFR32 *) tpalloc("FML32", NULL, 1024)) == NULL){
        printf("Error Reservando espacio para Buffer recv\n");
        tpterm();
        return(1);
    }

    printf("Enviando datos al servidor...\n");
    while (i < cantidad){
        if(Fadd32 (fbfr, CODIGO, (char *) &Universidad[i].Codigo, 0) == -1){
            printf("Error agregando el campo CODIGO al buffer\n");
            tpfree((char *)fbfr);
            tpterm();
            return(1);
        }
        if(Fadd32 (fbfr, GRUPO, (char *) &Universidad[i].Grupo, 0) == -1){
            printf("Error agregando el campo GRUPO al buffer\n");
            tpfree((char *)fbfr);
            tpterm();
            return(1);
        }
        if(Fadd32 (fbfr, MATERIA, (char *) &Universidad[i].Materia, 0) == -1){
            printf("Error agregando el campo MATERIA al buffer\n");
            tpfree((char *)fbfr);
            tpterm();
            return(1);
        }
        if(Fadd32 (fbfr, CREDITOS, (char *) &Universidad[i].Creditos, 0) == -1){
            printf("Error agregando el campo CREDITOS al buffer\n");
            tpfree((char *)fbfr);
            tpterm();
            return(1);
        }
        if(Fadd32 (fbfr, TRIMESTRE, (char *) &Universidad[i].Trimestre, 0) == -1){
            printf("Error agregando el campo TRIMESTRE al buffer\n");
            tpfree((char *)fbfr);
            tpterm();
            return(1);
        }
        if(Fadd32 (fbfr, NOMBREPROFESOR, (char *) &Universidad[i].NombreProfesor, 0) == -1){
            printf("Error agregando el campo NOMBREPROFESOR al buffer\n");
            tpfree((char *)fbfr);
            tpterm();
            return(1);
        }
        i++;
    }
    //Invocar al servidor
    if (tpcall("INSERTAR", (char *)fbfr, 0, (char **)&recv, &flen, 0) == -1){
        printf("Error en la llamada al servicio INSERTAR, tperrno = %d\n", tperrno);
        tpfree((char *)fbfr);
        tpfree((char *)recv);
        tpterm();
        return(1);
    }

    flen = sizeof(msgbuf);
    Fget32(recv, OUTPUT, 0, (char *)msgbuf, &flen);
    printf("Respuesta del servidor: %s\n", msgbuf);

    tpfree((char *)fbfr);
    tpfree((char *)recv);
    tpterm();
    return 0;
}