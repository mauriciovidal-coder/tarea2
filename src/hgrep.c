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
        fprintf(stderr,"hgrep:  %s:  %s\n",ruta,strerror(errno));
        *hubo_error = 1;
        return 0;
    }
char *linea = NULL;
size_t capacidad = 0;
long contador = 0;

    while(getline(&linea,&capacidad,archivo)!= -1){
        if(strstr(linea, palabra)!= NULL){
            contador++;
       }
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
        pthread_mutex_unlock(&ctx->mutex_indice);
            if (idx >= ctx->num_archivos){
            break;
        }
         
         int err=0;
         long c = contador_en_archivo(ctx->archivos[idx], ctx->palabra, &err);
        ctx->resultados[idx].conteo = c;
        ctx->resultados[idx].error = err;
    }
    
    return NULL;
}
 static void mostrar_uso(void){
    fprintf(stderr,"Uso ./hgrep [-p |-f] patron N archivos\n");

    }
int main(int argc, char *argv[]){
    int modo_procesos = 0;
    int modo_encuentro = 0;
    int pos = 1;
    if(pos < argc &&(strcmp(argv[pos],"-f") == 0 || strcmp(argv[pos],"-p" == 0))){
        if(strcmp(argv[pos],"-f") == 0){
            modo_procesos = 1;

        }else{
            modo_encuentro = 1;

        }
        pos++;    
    }
    if (argc - pos < 3){
        mostrar_uso();
        return 2;

    }
    const char *palabra = argv[pos];
    char *fin = NULL;
    long n_long = strtol(argv[pos + 1],&fin,10);
    if(fin == argv[pos +1] || *fin != '\0'|| n_long < 1 || n_long > 64){
        mostrar_uso();
        return 2;

    }
   int n_trabajadores =(int)n_long;
   char **archivos = &argv[pos + 2];
   int num_archivos = argc - (pos + 2);
   int num_hilos = (n_trabajadores < num_archivos) ? n_trabajadores: num_archivos;
   
   (void)modo_procesos;
   (void)modo_encuentro;
   Resultado *resultados = calloc((size_t)num_archivos, sizeof(Resultado));
   pthread_t *hilos =  malloc((size_t)num_hilos * sizeof(pthread_t));
   if (resultados == NULL || hilos == NULL){
    fprintf(stderr,"hgrep: error de memoria\n");
    free(resultados);
    free(hilos);
    return 2;
   }
contexto ctx;
ctx.palabra = palabra;
ctx.archivos = archivos;
ctx.num_archivos = num_archivos;
ctx.siguiente = 0;
ctx.resultados = resultados;
pthread_mutex_init(&ctx.mutex_indice, NULL);
   for(int i = 0; i < num_hilos; i++){
    if(pthread_create(&hilos[i],NULL, trabajador, &ctx) != 0){
        fprintf(stderr,"hgrep: error al crear hilo\n");
        pthread_mutex_destroy(&ctx.mutex_indice);
        free(resultados);
        free(hilos);
        return 2;
    }
   }
   for (int i = 0; i < num_hilos; i++){
    pthread_join(hilos[i], NULL);

   }
   int hubo_error = 0;
   int hubo_coincidencia = 0;
   for(int i = 0; i < num_archivos; i++){
    if(resultados[i].error){
        hubo_error = 1;
        continue;
    }
    if (resultados[i].conteo > 0){
        hubo_coincidencia = 1;
    }
    if (num_archivos == 1){
        printf("%s: %ld\n", archivos[i], resultados[i].conteo);
    } 
    else{
        printf("%s: %ld\n", archivos[i], resultados[i].conteo);
    }
   }



pthread_mutex_destroy(&ctx.mutex_indice);
free(resultados);
free(hilos);
if(hubo_error){
    return 2;
}
if(hubo_coincidencia){
    return 0;
}
return 1;
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