/*-----------------------------------------------------+
 |     defines . H                                     |
 +-----------------------------------------------------+
 |     Version    :                                    |
 |     Autor      :                                    |
 |     Asignatura :  SOP-GIIROB                        |
 |     Descripcion:                                    |
 +-----------------------------------------------------*/

#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define max(a,b) (((a)>(b)) ? (a): (b))

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0

#define LOGO "MICRO SHELL (c)2023 Jorge"
#define PROMPT "jorge"

#define MAXLINE 200
#define MAXARG 20
#define MAXNAME 100
#define PIPELINE 10


//COLORES
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define RED "\033[31m"
#define WHITE "\033[37m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"  
#define ORANGE "\033[38;5;214m"  

typedef struct
{
  int infd;
  int outfd;
}CMDFD[PIPELINE-1];


#endif

