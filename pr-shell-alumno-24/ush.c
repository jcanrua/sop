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
  int error;

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
	     if(m_n>0){
         	if (pipeline(m_n,fich_entrada(),fich_salida(),es_append(),es_background())==1){
				error = ejecutar(m_n,m_num_arg,m_ordenes,m_argumentos,es_background());
			}
        }
		if(error != 0) {
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
int leerLinea( char *linea, int tamanyLinea )
{
	printf(ORANGE"%s" RESET " - " BOLD ORANGE "%s" RESET " -> ", getcwd(NULL, 0), PROMPT);
	
	int total = 0;
	char i;
	do{
		i = getchar();
		linea[total] = i;
		total++;
		if(i == EOF){
			return -2;
		}
		else if(total>tamanyLinea){
			printf(RED "\nError: se ha excedido el tamaño máximo de la linea\n" RESET);
			return -1;
		}
	} while(i != '\n');
	return total;
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
void visualizar( void )
{  
	void analizador_ini(void);
	int numero_ordenes = num_ordenes();
	printf("\n");
	printf(BOLD GREEN "Número de ordenes:" RESET " " BOLD ORANGE "%d\n" RESET, numero_ordenes);
	
	char **ordenes = get_ordenes();
	int *numero_argumentos = num_argumentos();
	char ***argumentos = get_argumentos();	
	

	for(int i = 0; i < numero_ordenes; i++){
  		printf(BOLD GREEN "\n        --- ORDEN "ORANGE" %d "RESET"---\n", i);
  		printf(PURPLE"    Número de argumentos ->"ORANGE" %d\n" RESET, numero_argumentos[i]);
		for(int j = 0; j < numero_argumentos[i]; j++){
			if(argumentos[i][j] == NULL){
				break;
			}
			printf(CYAN"        Argumento %d -> " ORANGE "%s\n" RESET, j, argumentos[i][j]);
		}
	}
	
	char *entrada = fich_entrada();
	char *salida = fich_salida();

	if(entrada[0] != '\0'){
		printf( PURPLE"    El fichero de entrada es: " ORANGE" %s\n" RESET, entrada);
	}
	
	if(salida[0] != '\0'){	
		printf(PURPLE"    El fichero de salida es: "ORANGE" %s\n" RESET, salida);
		if(es_append() != 0){
			printf(CYAN"        La salida debe ser añadida al fichero (APPEND)\n" RESET);
		}
		else{
			printf(CYAN"        La salida NO debe ser añadida al fichero (TRUNK)\n"RESET);
		}
	}
    printf("\n");	
	if(es_background() != 0){
		printf(BLUE"    La orden se ha ejecutado en BACKGROUND\n" RESET);
	}
	else{
		printf(BLUE"    La orden se ha ejecutado en FOREGROUND\n"RESET);
	}
	
	printf("\n");
} // Fin de "visualizar"
