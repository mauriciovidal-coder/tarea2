#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

typedef struct{
    long conteo;
    int error;
} Resultado;
typedef struct{
    const char *palabra;
    char **archivos;
    int num_archivos;
    int siguiente;
    Resultado *resultados;
    pthread_mutex_t mutex_indice;
} contexto;
static long contador_en_archivo(const char *ruta, const char *palabra, int *hubo_error){
    FILE *archivo = fopen(ruta,"r");
    if (archivo == NULL){
        fprintf(sdterr,"hgrep:  %s:  %s\n",ruta,strerror(errno));
        *hubo_error = 1;
        return 0;
    }
}
char *linea = NULL;
size_t capacidad = 0;
long contador = 0;

while (getline(&linea,&capacidad,archivo)!= -1){
    if(strstr(linea, palabra)!= NULL){
        contador++
    }
    free(linea);
    fclose(archivo);
    *hubo_error = 0;
    return contador;

}
static void *trabajador(void *arg){
    contexto *ctx = (contexto*)arg;
    while(1){
        pthread_mutex_lock(&ctx->mutex_indice);
        int idx = ctx->siguiente;
        if (idx < ctx ->num_archivos){
            ctx -> siguiente++;
         }
    }





/*
int main( int argc, char *argv[]){
    if(argc !=2){
        fprintf(stderr,"error: deber escribir un argumento valido maximo 1 \n");
        return 1;  //retorna el error del prgrama si hay mas de un argumentos 

    }

char *palabra = argv[1];
char linea[Maxima_lineas];
int contador = 0;  // se crea una variable contador para contar las lineas que contiene la palabra 
    while (fgets(linea, Maxima_lineas, stdin) != NULL){
        if (strstr(linea, palabra) != NULL){ // strstr busca la palabra dentro de la linea 
            contador++;

        }
    }
printf("%d\n",contador); // imprime el numero de lineas que tiene la palabra 
    return 0; // retorna la operacion del programa de manera correcta 



}*/