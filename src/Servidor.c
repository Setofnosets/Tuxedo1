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

void insertaLista(TPSVCINFO *rqst){
    FBFR32* fbfr = (FBFR32*)rqst->data;
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
    snprintf(buffer, sizeof(buffer), "INSERT INTO Uni Values(%d, '%s', '%s', %d, %d, '%s') ON DUPLICATE KEY UPDATE Codigo=Codigo, Grupo='%s', Materia='%s', Creditos=%d, Trimestre=%d, NombreProfesor='%s'", Uni.Codigo, Uni.Grupo, Uni.Materia, Uni.Creditos, Uni.Trimestre, Uni.NombreProfesor, Uni.Grupo, Uni.Materia, Uni.Creditos, Uni.Trimestre, Uni.NombreProfesor);
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

void imprimeLista(TPSVCINFO *rqst){
    FBFR32* fbfr = (FBFR32*)rqst->data;
    char buffer[1024];
    strcpy(buffer, "SELECT * FROM Uni");
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    fbfr = (FBFR32 *)tprealloc((char *)fbfr, 1000000);
    if((conn = mysql_init(NULL)) == NULL){
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
    if((res = mysql_store_result(conn)) == NULL){
        userlog("Error: %s", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
    Universidad Resultado;
    userlog("Enviando datos al cliente...");
    while((row = mysql_fetch_row(res)) != NULL){
        for(int i = 0; i < mysql_num_fields(res); i++){
            userlog("%s", row[i]);
            switch(i){
                case 0:
                    Resultado.Codigo = atoi(row[i]);
                    break;
                case 1:
                    strcpy(Resultado.Grupo, row[i]);
                    break;
                case 2:
                    strcpy(Resultado.Materia, row[i]);
                    break;
                case 3:
                    Resultado.Creditos = atoi(row[i]);
                    break;
                case 4:
                    Resultado.Trimestre = atoi(row[i]);
                    break;
                case 5:
                    strcpy(Resultado.NombreProfesor, row[i]);
                    break;
            }
        }
        if(Fadd32(fbfr, CODIGO, (char *)&Resultado.Codigo, 0) < 0){
            userlog("No se agrego el primer operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
        if(Fadd32(fbfr, GRUPO, (char *)&Resultado.Grupo, 0) < 0){
            userlog("No se agrego el segundo operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
        if(Fadd32(fbfr, MATERIA, (char *)&Resultado.Materia, 0) < 0){
            userlog("No se agrego el tercer operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
        if(Fadd32(fbfr, CREDITOS, (char *)&Resultado.Creditos, 0) < 0){
            userlog("No se agrego el cuarto operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
        if(Fadd32(fbfr, TRIMESTRE, (char *)&Resultado.Trimestre, 0) < 0){
            userlog("No se agrego el quinto operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
        if(Fadd32(fbfr, NOMBREPROFESOR, (char *)&Resultado.NombreProfesor, 0) < 0){
            userlog("No se agrego el sexto operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
    }
    mysql_close(conn);
    tpreturn(TPSUCCESS, 0, (char *)fbfr, 0L, 0);
}

void seleccionar(TPSVCINFO *rqst){
    FBFR32* fbfr = (FBFR32*)rqst->data;
    char buffer[1024];
    long codigo;
    //Leer codigo
    if(Fget32(fbfr, CODIGO, 0, (char *)&codigo, 0) < 0){
        userlog("No se pudo leer el codigo: %d", Ferror32);
        tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
    }
    userlog("Lectura exitosa");
    fflush(stdout);
    snprintf(buffer, sizeof(buffer), "SELECT * FROM Uni WHERE Codigo = %ld", codigo);
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    if((conn = mysql_init(NULL)) == NULL){
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
    if((res = mysql_store_result(conn)) == NULL){
        userlog("Error: %s", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
    Universidad Resultado;
    userlog("Enviando datos al cliente...");
    while((row = mysql_fetch_row(res)) != NULL){
        for(int i = 0; i < mysql_num_fields(res); i++){
            userlog("%s", row[i]);
            switch(i){
                case 0:
                    Resultado.Codigo = atoi(row[i]);
                    break;
                case 1:
                    strcpy(Resultado.Grupo, row[i]);
                    break;
                case 2:
                    strcpy(Resultado.Materia, row[i]);
                    break;
                case 3:
                    Resultado.Creditos = atoi(row[i]);
                    break;
                case 4:
                    Resultado.Trimestre = atoi(row[i]);
                    break;
                case 5:
                    strcpy(Resultado.NombreProfesor, row[i]);
                    break;
            }
        }
        if(Fadd32(fbfr, CODIGO, (char *)&Resultado.Codigo, 0) < 0){
            userlog("No se agrego el primer operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
        if(Fadd32(fbfr, GRUPO, (char *)&Resultado.Grupo, 0) < 0){
            userlog("No se agrego el segundo operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
        if(Fadd32(fbfr, MATERIA, (char *)&Resultado.Materia, 0) < 0){
            userlog("No se agrego el tercer operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
        if(Fadd32(fbfr, CREDITOS, (char *)&Resultado.Creditos, 0) < 0){
            userlog("No se agrego el cuarto operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
        if(Fadd32(fbfr, TRIMESTRE, (char *)&Resultado.Trimestre, 0) < 0){
            userlog("No se agrego el quinto operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
        if(Fadd32(fbfr, NOMBREPROFESOR, (char *)&Resultado.NombreProfesor, 0) < 0){
            userlog("No se agrego el sexto operador: %d", Ferror32);
            tpreturn (TPFAIL, 0, (char *)fbfr, 0L, 0);
        }
    }
    mysql_close(conn);
    tpreturn(TPSUCCESS, 0, (char *)fbfr, 0L, 0);
}