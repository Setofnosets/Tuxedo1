#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fml32.h>
#include "../inc/biblioOper.fml.h"
#include "atmi.h"

typedef struct Universidad{
    long Codigo;
    char Grupo[10];
    char Materia[50];
    long Creditos;
    long Trimestre;
    char NombreProfesor[50];
}Universidad;

int leerArchivo(char* nombreArchivo, Universidad* Universidad);
int insertarUniversidad(Universidad Universidad);
int numeroLineas(char* nombreArchivo);
int imprimeLista();

int main() {
    int numLineas = numeroLineas("Universidad.csv");
    Universidad Universidad[numLineas];
    leerArchivo("Universidad.csv", Universidad);
    //imprimeLista();
    int i = 0;
    while(i < numLineas){
        insertarUniversidad(Universidad[i]);
        i++;
    }
    imprimeLista();
    /*Universidad Uni;
    Uni.Codigo = 1;
    strcpy(Uni.Grupo, "aaaa");
    strcpy(Uni.Materia, "bbbbb");
    Uni.Creditos = 2;
    Uni.Trimestre=5;
    strcpy(Uni.NombreProfesor, "Juan");
    insertarUniversidad(Uni, 1);*/
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
    long longitud;
    char msgbuf[1024];
    printf("Conectando con el servidor...\n");
    if(tpinit((TPINIT *)NULL) == -1){
        printf("Error en la conexion con el servidor, tperrno = %d\n", tperrno);
        return 1;
    }

    //Reservacion de memoria del buffer
    if ((fbfr = (FBFR32 *) tpalloc("FML32", NULL, 2048)) == NULL){
        printf("Error Reservando espacio para Buffer fbfr\n");
        tpterm();
        return(1);
    }

    if ((recv = (FBFR32 *) tpalloc("FML32", NULL, 2048)) == NULL){
        printf("Error Reservando espacio para Buffer recv\n");
        tpterm();
        return(1);
    }

    if (tpcall("imprimeLista", (char *)fbfr, 0, (char **)&recv, &longitud, 0) == -1){
        printf("Error en la llamada al servicio LISTA, tperrno = %d\n", tperrno);
        tpfree((char *)fbfr);
        tpfree((char *)recv);
        tpterm();
        return(1);
    }

    flen = sizeof(Universidad);
    /*Fget32(recv, OUTPUT, 0, (char *)msgbuf, &flen);
    printf("Respuesta del servidor: %s\n", msgbuf);*/
    //Respuesta del servidor:
    Universidad Uni;
    int i = 0;
    while(Fget32(recv, CODIGO, i, (char *)&Uni.Codigo, 0) != -1){
        Fget32(recv, GRUPO, i, (char *)&Uni.Grupo, 0);
        Fget32(recv, MATERIA, i, (char *)&Uni.Materia, 0);
        Fget32(recv, CREDITOS, i, (char *)&Uni.Creditos, 0);
        Fget32(recv, TRIMESTRE, i, (char *)&Uni.Trimestre, 0);
        Fget32(recv, NOMBREPROFESOR, i, (char *)&Uni.NombreProfesor, 0);
        printf("Codigo: %d, Grupo: %s, Materia: %s, Creditos: %d, Trimestre: %d, NombreProfesor: %s\n", Uni.Codigo, Uni.Grupo, Uni.Materia, Uni.Creditos, Uni.Trimestre, Uni.NombreProfesor);
        i++;
    }
    if(Ferror32 != FNOTPRES){
        printf("Error en la lectura del buffer, Ferror32 = %d\n", Ferror32);
    }
    tpfree((char *)fbfr);
    tpfree((char *)recv);
    tpterm();
    return 0;
}

int insertarUniversidad(Universidad Universidad){
    FBFR32 *fbfr, *recv;
    long flen;
    char msgbuf[1024];
    int i = 0;
    printf("Conectando con el servidor...\n");
    if(tpinit((TPINIT *)NULL) == -1){
        printf("Error en la conexion con el servidor, tperrno = %d\n", tperrno);
        return 1;
    }

    //Reservacion de memoria del buffer
    if ((fbfr = (FBFR32 *) tpalloc("FML32", NULL, 2048)) == NULL){
        printf("Error Reservando espacio para Buffer fbfr\n");
        tpterm();
        return(1);
    }
    if ((recv = (FBFR32 *) tpalloc("FML32", NULL, 2048)) == NULL){
        printf("Error Reservando espacio para Buffer recv\n");
        tpterm();
        return(1);
    }
    printf("Enviando datos al servidor...\n");
    if(Fadd32 (fbfr, CODIGO, (char *) &Universidad.Codigo, 0) == -1){
        printf("Error agregando el campo CODIGO al buffer: %d\n", Ferror32);
        tpfree((char *)fbfr);
        tpterm();
        return(1);
    }
    if(Fadd32 (fbfr, GRUPO, (char *) &Universidad.Grupo, 0) == -1){
        printf("Error agregando el campo GRUPO al buffer: %d\n", Ferror32);
        tpfree((char *)fbfr);
        tpterm();
        return(1);
    }
    if(Fadd32 (fbfr, MATERIA, (char *) &Universidad.Materia, 0) == -1){
        printf("Error agregando el campo MATERIA al buffer: %d\n", Ferror32);
        tpfree((char *)fbfr);
        tpterm();
        return(1);
    }
    if(Fadd32 (fbfr, CREDITOS, (char *) &Universidad.Creditos, 0) == -1){
        printf("Error agregando el campo CREDITOS al buffer: %d\n", Ferror32);
        tpfree((char *)fbfr);
        tpterm();
        return(1);
    }
    if(Fadd32 (fbfr, TRIMESTRE, (char *) &Universidad.Trimestre, 0) == -1){
        printf("Error agregando el campo TRIMESTRE al buffer: %d\n", Ferror32);
        tpfree((char *)fbfr);
        tpterm();
        return(1);
    }
    if(Fadd32 (fbfr, NOMBREPROFESOR, (char *) &Universidad.NombreProfesor, 0) == -1){
        printf("Error agregando el campo NOMBREPROFESOR al buffer: %d\n", Ferror32);
        tpfree((char *)fbfr);
        tpterm();
        return(1);
    }
    //Invocar al servidor
    printf("Invocando al servidor....\n");
    fflush(stdout);
    if (tpcall("insertaLista", (char *)fbfr, 0, (char **)&recv, &flen, (long)0) == -1){
        printf("Error en la llamada al servicio INSERTAR, tperrno = %d\n", tperrno);
        tpfree((char *)fbfr);
        tpterm();
        return(1);
    }

   /* flen = sizeof(msgbuf);
    Fget32(recv, OUTPUT, 0, (char *)msgbuf, &flen);
    printf("Respuesta del servidor: %s\n", msgbuf);*/

    tpfree((char *)fbfr);
    tpterm();
    return 0;
}