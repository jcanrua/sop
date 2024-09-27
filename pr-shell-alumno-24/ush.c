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
          if (pipeline_profe(m_n,fich_entrada(),fich_salida(),es_append(),es_background())==OK)
                    ejecutar_profe(m_n,m_num_arg,m_ordenes,m_argumentos,es_background());
        }

       visualizar_profe();
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
	printf("%s - %s->", getcwd(NULL, 0), PROMPT);
	
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
	int num = num_ordenes();
	printf("Número de ordenes: %d\n", num);
	
	char **ordenes = get_ordenes();
	int *numero_argumentos = num_argumentos();
	char ***get_argumentos = get_argumentos();	

	for(int i = 0; i < num_ordenes; i++){
  		printf("ORDEN %d\n -> %s", i, ordenes[i]);
  		printf("Número de argumentos -> %d\n", numero_argumentos[i]);
		for(int j = 0; j < numero_argumentos[i]; j++){
			printf("Argumento %d -> %s\n", i, get_argumentos[i][j];
		}
	} 
} // Fin de "visualizar"



