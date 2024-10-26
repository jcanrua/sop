/*-----------------------------------------------------+      
 |     R E D I R E C C I O N . C                       |
 +-----------------------------------------------------+
 |     Version    :                                    |                      
 |     Autor :                                         |
 |     Asignatura :  SOP-GIIROB                        |
 |     Descripcion:                                    |
 +-----------------------------------------------------*/
#include "defines.h"
#include "redireccion.h"
#include "ejecucion.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "profe.h"


int ejecutar (int nordenes , int *nargs , char **ordenes , char ***args , int bgnd){
    int status;
    for (int i = 0; i < nordenes; i++){
        pid_t control_situacion = fork(); //controlar si es el hijo o el padre
        if(control_situacion == 0){ //hijo
            redirigir_entrada(i);
            redirigir_salida(i);
            cerrar_fd();
            if(execvp(ordenes[i], args[i]) == -1){
                printf("Error en exec");
                return 0;
            }
        }
    }
    cerrar_fd();
    while(wait(&status) != -1);
    return 1;
} // Fin de la funcion "ejecutar"
