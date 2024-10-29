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
	if (cmdfd == NULL) {
        fprintf(stderr, RED"Error: cmdfd no está inicializado.\n" RESET);
        return; 
    }
    if (PIPELINE <= 0) {
        fprintf(stderr, RED"Error: PIPELINE debe ser mayor que 0.\n"RESET);
        return;
    }
	for (int i = 0; i < PIPELINE; i++){
		cmdfd[i].infd = 0;
		cmdfd[i].outfd = 1;
	}
}//Inicializar los valores de la estructura cmdfd

int pipeline(int ncmd, char * infile, char * outfile, int append, int bgnd){
    redireccion_ini();
    if(ncmd >= 1){
        int fd, fds[2];
        if(bgnd){
            fd = open("/dev/null", O_RDONLY);
            if(fd == -1){
                perror(RED"Error en Open"RESET);
                return 0;
            }
            cmdfd[0].infd = fd;
        }
        for (int i = 0; i < ncmd -1; i++){
            pipe(fds);
            cmdfd[i+1].infd = fds[0];
            cmdfd[i].outfd = fds[1];
        }
        if(strlen(infile) != 0){
            fd = open(infile, O_RDONLY | O_CREAT);
            if(fd == -1){
                perror(RED"Error en Open"RESET);
                return 0;
            }
            cmdfd[0].infd = fd;
        }
        if(strlen(outfile) != 0){
            if(append){
                fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if(fd == -1){
                    perror(RED"Error en Open"RESET);
                    return 0;
                }
                cmdfd[ncmd-1].outfd = fd;
            }
            else{
                fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if(fd == -1){
                    perror(RED"Error en Open"RESET);
                    return 0;
                }
                cmdfd[ncmd-1].outfd = fd;
            }
        }
    }
    return 1;
} // Fin de la función "pipeline"
  
int redirigir_entrada(int i) {
    if (i < 0) {
        perror(RED"Posición incorrecta"RESET);
        return 0;
    } else if (cmdfd[i].infd != 0) {
        if (dup2(cmdfd[i].infd, 0) == -1) {
            perror(RED"Error en DUP entrada"RESET);
            return 0;
        }
    }
    return 1;
}

int redirigir_salida(int i) {
    if (i < 0){
		perror(RED"Posición incorrecta"RESET);
		return 0;
	} 
    else if (cmdfd[i].outfd != 1) {
        if (dup2(cmdfd[i].outfd, 1) == -1) {
            perror(RED"Error en DUP salida"RESET);
            return 0;
        }
    }
    return 1;
}




int cerrar_fd(){
	int control;
	for (int i = 0; i < PIPELINE; i++){
		if(cmdfd[i].infd > 2){
			control = close(cmdfd[i].infd);
			if(control == -1){
				perror(RED"Error al cerrar entrada"RESET);;
				return 0;
			}
		}
		if(cmdfd[i].outfd > 2){
			control = close(cmdfd[i].outfd);
			if(control == -1){
				perror(RED"Error al cerrar salida"RESET);
				return 0;
			}
		}
	}
	return 1;
} // Fin de la función "cerrar_fd"


