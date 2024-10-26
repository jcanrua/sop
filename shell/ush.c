/*-----------------------------------------------------+
 |     U S H. C                                        
 +-----------------------------------------------------+
 |     Versión :                                       |                      
 |     Autor :                                         |
 |     Asignatura :  SOP-GIIROB                        |                               
 |     Descripción :                                   |
 +-----------------------------------------------------*/
#include "defines.h"
#include "analizador.h"
#include "redireccion.h"
#include "ejecucion.h"
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "profe.h"

#include <stdio.h>


//
// Declaraciones de funciones locales
//
void visualizar( void );
int leerLinea( char *linea, int tamanyLinea );


//
// Prog. ppal.
// 
int main(int argc, char * argv[])
{
    
  char line[255];
  int res;
  char **m_ordenes;
  char ***m_argumentos;
  int *m_num_arg;
  int m_n;
  int no_error_ej = 0, no_error_pipe = 0;

  while(1)
  {
    
    do
    {
        res=leerLinea(line,MAXLINE);    
        if (res==-2) {
      		fprintf(stdout,"logout\n");
		      exit(0);
        }
	
    	  if (res==-1){
		      fprintf(stdout,"linea muy larga\n");
	      }
     }while(res<=0);

     if (analizar(line)==OK){
       m_n=num_ordenes();
	     m_num_arg=num_argumentos();
	     m_ordenes=get_ordenes();
	     m_argumentos=get_argumentos();
	     if(m_n>0)
	     {
          if (pipeline(m_n,fich_entrada(),fich_salida(),es_append(),es_background())==1)
                    no_error_ej = ejecutar(m_n,m_num_arg,m_ordenes,m_argumentos,es_background());
                    no_error_pipe = 1;
        }

       if(no_error_ej && no_error_pipe){
          visualizar();
       }
     }
 }    
  return 0;
}





/****************************************************************/
/*                       leerLinea                             
  --------------------------------------------------------------
                                                               
   DESCRIPCIÓN:                                                 
   Obtiene la línea de órdenes para el mShell.    
   Util para depuracion.                                        
                                                                
   ENTRADA:                                                 
    linea - puntero a un vector de carácteres donde se almancenan los caracteres 
   leídos del teclado
    tamanyLinea - tamaño máximo del vector anterior

   SALIDA:
    -- linea - si termina bien, contiene como último carácter el retorno de carro.
    -- leerLinea -  Entero que representa el motivo de finalización de la función:
     > 0 - terminación correcta, número de caracteres leídos, incluído '\n'
     -1 - termina por haber alcanzado el número máximo de caracteres que se 
    espera leer de teclado, sin encontrar '\n'.
     -2 - termina por haber leído fin de fichero (EOF).
*/
/****************************************************************/
//char * getline(void)
int leerLinea( char *linea, int tamanyLinea ){
  printf("\n%s %s ", PROMPT, getcwd(NULL, 0));
  char c;
  int control_tam=0;
  while(control_tam<=tamanyLinea){
    if(control_tam==tamanyLinea){
      return -1;
    }
    else{
      c=getchar();
      if(c==EOF){
        return -2;
      }
      else{
        linea[control_tam]=c;
        if(c=='\n'){
          return control_tam;
        }
      }
    }
    control_tam++;
  }
  return OK; 
}




/****************************************************************/
/*                       visualizar                             */
/*--------------------------------------------------------------*/
/*                                                              */
/* DESCRIPCIÓN:                                                 */
/* Visualiza los distintos argumentos de la orden analizada.    */
/* Util para depuracion.                                        */
/*                                                              */
/* ENTRADA: void                                                */
/*                                                              */
/* SALIDA: void                                                 */
/*                                                              */
/****************************************************************/
void visualizar( void ){
char ***argumentos = get_argumentos(); // Obtener la matriz de ordenes y argumentos
int numero_ordenes = num_ordenes();
printf("\n  \033[0;32m Se han introducido %d ordenes.\033[0m \n", numero_ordenes);
int control_ord=0;
    if (argumentos[control_ord] == NULL) {
        printf("\n  No se han introducido ordenes.\n");
        return;
    }
    do{
      printf("  Orden %d: \033[;34m%s\033[m \n", control_ord+1, argumentos[control_ord][0]);
      if(es_background() == TRUE){
        printf("  Esta orden se ha lanzado en \033[1;31m BACKGROUND \033[m. \n");
      }
      else{
        printf("  Esta orden esta lanzada en \033[1;31m FOREGROUND \033[m. \n");
      }
      if(es_append() && control_ord == numero_ordenes){
        printf("  Esta orden esta lanzada en modo \033[1;33m APPEND \033[m. \n");
      }
      else if(!es_append() && control_ord == numero_ordenes){
        printf("  Esta orden esta lanzada en modo \033[1;33m TRUNK \033[m. \n");
      }
      if(strlen(fich_entrada())!=0 && control_ord == 0){
        printf("  Redirección de entrada a --> %s. \n", fich_entrada());
      }
      if(strlen(fich_salida())!=0 && control_ord == numero_ordenes){
        printf("  Redirección de salida a --> %s. \n", fich_salida());
      }
      int control_arg=1;
      if(argumentos[control_ord][control_arg] == NULL){
        printf("    No se han introducido argumentos para esta orden. \n");
      }
      else{
        printf("      Argumentos de la orden: \n");
        while(argumentos[control_ord][control_arg]!=NULL){
          printf("        Argumento %d --> \033[0;34m%s\033[m \n", control_arg, argumentos[control_ord][control_arg]);
          control_arg++;
        }
      }
      control_ord++;
      numero_ordenes--;
    }while(numero_ordenes!=0);
    
}
    
   
 // Fin de "visualizar"