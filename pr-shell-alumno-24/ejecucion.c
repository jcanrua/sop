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
#include "profe.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int ejecutar (int nordenes , int *nargs , char **ordenes , char ***args , int bgnd)
{
	for(int i = 0; i < nordenes; i++){
		pid_t val = fork();
		
		if(val == -1){
			printf("ERROR al crear un hijo\n");
			break;
		}

		if(val == 0){
			redirigir_entrada(i);
			redirigir_salida(i);
			if (execvp(ordenes[i],args[i])==-1){
				printf("Error en exec\n");
				printf("Error: Comando no encontrado: %s\n", ordenes[i]);
				return 0;
			}	
		}
		else{
			cerrar_fd();
			wait(NULL);
		}
	}
} // Fin de la funcion "ejecutar"
