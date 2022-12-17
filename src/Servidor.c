#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fml32.h>
#include "../inc/biblioOper.fml.h"
#include "atmi.h"
#include <mysql.h>

typedef struct Universidad{
    int Codigo;
    char Grupo[10];
    char Materia[50];
    int Creditos;
    int Trimestre;
    char NombreProfesor[50];
}Universidad;

tpsvrinit(int argc, char *argv []){
    int iRetValue = 0;
    userlog ("Iniciando servidor");
    return(iRetValue);
}

void imprimeLista(TPSVCINFO *rqst){
    FBFR32* fbfr = (FBFR32*)rqst->data;
    char buffer[500];
    Universidad Uni;
    strcpy(buffer, "INSERT INTO Uni VALUES('");
    MYSQL *conn;
    if ((conn = mysql_init(NULL)) == NULL) {
        userlog("Error: %s", mysql_error(conn));
        exit(1);
    }
    if(mysql_real_connect(conn, "localhost", "root", "A5r!87T0U1Ro", "Universidad", 0, NULL, 0) == NULL){
        userlog("Error: %s", mysql_error(conn));
        exit(1);
    }
    if(Fget32(fbfr, OPERUN, 0, (char *)Uni.Codigo, 0) < 0){
        userlog("No se leyo el primer operador");
        tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
    }
    strcat(buffer)
    if(mysql_query(conn, "INSERT INTO Uni VALUES('')") != 0){
        printf("Error: %s", mysql_error(conn));
        exit(1);
    }
}