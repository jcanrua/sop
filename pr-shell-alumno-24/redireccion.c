/*-----------------------------------------------------+      
 |     R E D I R E C C I O N . C                       |
 +-----------------------------------------------------+
 |     Version    :                        
 |     Autor :   
 |     Asignatura :  SOP-GIIROB                                                       
 |     Descripcion: 
 +-----------------------------------------------------*/
#include "defines.h"
#include "analizador.h"
#include "redireccion.h"
#include <unistd.h>

CMDFD cmdfd;


void redireccion_ini(void){
	for (int i = 0; i < PIPELINE; i++){
		cmdfd[i].infd = 0;
		cmdfd[i].outfd = 1;
	}
}//Inicializar los valores de la estructura cmdfd



int pipeline(int ncmd, char * infile, char * outfile, int append, int bgnd){
	redireccion_ini();
		
	if (infile[0] != '\0'){
		int fd_in = open(infile, O_RDONLY);
		if (fd_in < 0){
			return 0;
		}
		cmdfd[0].infd = fd_in;
	}

	if (outfile[0] != '\0' ){
		int flags = O_WRONLY | O_CREAT;
		
		if (append){
			flags |= O_APPEND; 
		}
		else{
			flags |= O_TRUNC;
		}

		int fd_out = open(outfile, flags, 0644);
		if (fd_out < 0){
			return 0;
		}
		cmdfd[ncmd-1].outfd = fd_out;
	}


	for (int i = 0; i < ncmd - 1; i++){
		int fds[2];
		if(pipe(fds) == -1){
			return 0;
		}
		cmdfd[i+1].infd = fds[0];
		cmdfd[i].outfd = fds[1];
	}



	if (bgnd) {
    	int null_fd = open("/dev/null", O_RDONLY);
    	if (null_fd < 0) {
        	return 0;
    	}
    	cmdfd[0].infd = null_fd;  // Redirigir la entrada estándar al dispositivo nulo

	}
return 1;
} // Fin de la función "pipeline"



int redirigir_entrada(int i) {
    int control;
    if (i < 0) {
        perror("Posicion incorrecta");
        return 0;
    } else if (cmdfd[i].infd != 0) {
        control = dup2(cmdfd[i].infd, 0);
        if (control == -1) {
            perror("Error en DUP");
            return 0;
        }
    }
    close(cmdfd[i].infd);
    return 1;
} // Fin de la función "redirigir_entrada"

int redirigir_salida(int i) {
    int control;
    if (i < 0) {
        perror("Posición incorrecta");
        return 0;
    } else if (cmdfd[i].outfd != 1) {
        control = dup2(cmdfd[i].outfd, 1);
        if (control == -1) {
            perror("Error en DUP");
            return 0;
        }
        close(cmdfd[i].outfd);
    }
    return 1;
} // Fin de la función "redirigir_salida"


int cerrar_fd(){
	int control;
	for (int i = 0; i < PIPELINE; i++){
		if(cmdfd[i].infd > 2){
			control = close(cmdfd[i].infd);
			if(control == -1){
				perror("Error en close");
				return 0;
			}
		}
		if(cmdfd[i].outfd > 2){
			control = close(cmdfd[i].outfd);
			if(control == -1){
				perror("Error en close");
				return 0;
			}
		}
	}
	return 1;
} // Fin de la función "cerrar_fd"


