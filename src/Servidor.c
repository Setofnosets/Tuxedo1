#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fml32.h>
#include "../inc/biblioOper.fml.h"
#include "atmi.h"
#include <userlog.h>
#include <mysql.h>

typedef struct Universidad{
    long Codigo;
    char Grupo[10];
    char Materia[50];
    long Creditos;
    long Trimestre;
    char NombreProfesor[50];
}Universidad;

int tpsvrinit(int argc, char *argv []){
    userlog("Iniciando servidor");
    return(0);
}

void imprimeLista(TPSVCINFO *rqst){
    FBFR32* fbfr = (FBFR32*)rqst->data;
    userlog("%d", Fielded32(fbfr));
    char buffer[1024];
    Universidad Uni;
    strcpy(buffer, "INSERT INTO Uni VALUES('");
    int len;
    //Recuperar Datos
    long* codo = (long*)Ffind32(fbfr, CODIGO, 0, &len);
    userlog("%d",*codo);
    if(Fget32(fbfr, CODIGO, 0, (char *)&Uni.Codigo, 0) < 0){
        userlog("No se leyo el primer operador: %d", Ferror32);
        userlog("Codigo: %d",Uni.Codigo);
        tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
    }
    if(Fget32(fbfr, GRUPO, 0, (char *)&Uni.Grupo, 0) < 0){
        userlog("No se leyo el segundo operador: %d", Ferror32);
        tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
    }
    if(Fget32(fbfr, MATERIA, 0, (char *)&Uni.Materia, 0) < 0){
        userlog("No se leyo el tercer operador: %d", Ferror32);
        tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
    }
    if(Fget32(fbfr, CREDITOS, 0, (char *)&Uni.Creditos, 0) < 0){
        userlog("No se leyo el cuarto operador: %d", Ferror32);
        tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
    }
    if(Fget32(fbfr, TRIMESTRE, 0, (char *)&Uni.Trimestre, 0) < 0){
        userlog("No se leyo el quinto operador: %d", Ferror32);
        tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
    }
    if(Fget32(fbfr, NOMBREPROFESOR, 0, (char *)&Uni.NombreProfesor, 0) < 0){
        userlog("No se leyo el sexto operador: %d", Ferror32);
        tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
    }
    userlog("Lectura exitosa");
    fflush(stdout);
    snprintf(buffer, sizeof(buffer), "INSERT INTO Uni Values(%d, '%s', '%s', %d, %d, '%s')",Uni.Codigo,Uni.Grupo,Uni.Materia,Uni.Creditos,Uni.Trimestre,Uni.NombreProfesor);
    userlog("%s",buffer);
    MYSQL *conn;
    if ((conn = mysql_init(NULL)) == NULL) {
        userlog("Error Inicializacion: %s", mysql_error(conn));
        exit(1);
    }
    if(mysql_real_connect(conn, "127.0.0.1", "root", "A5r!87T0U1Ro", "Universidad", 3306, NULL, 0) == NULL){
        userlog("Error conexion: %s", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
    if(mysql_query(conn, buffer) != 0){
        userlog("Error: %s", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
    mysql_close(conn);
    userlog("Insercion Exitosa");
    tpreturn(TPSUCCESS, 0, rqst->data, 0L, 0);
}