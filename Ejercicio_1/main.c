/*
Autór: Sergio Hernandez Castillo
Matrícula: A01025210
Descripción: Actividad 3 - Administración de un SO - Ejercicio 1
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int padre;
    int tiempoDeEjecucion;
} Subproceso;

int main(){
    int cantidadDeProcesos = 0;
    int cantidadDeSubprocesos = 0;
    int totalDeSubprocesos = 0;
    int contadorDeSubprocesos = -1;
    int contadorDeProcesos = 0;
    
    printf("\nProporcione la cantidad de procesos (no subprocesos): ");
    scanf("%d", &cantidadDeProcesos);
    printf("\n");

    int * arr = malloc(sizeof(int) * cantidadDeProcesos);
    int * final = arr + cantidadDeProcesos;

    int * i = arr;
    int j = 0;

    while ((i < final) && (j < cantidadDeProcesos)){
        printf("Proporcione la cantidad de subprocesos dentro del proceso %d: ", j);
        scanf("%d", &cantidadDeSubprocesos);

        totalDeSubprocesos += cantidadDeSubprocesos;
        *i = totalDeSubprocesos;

        ++i;
        ++j;
    }

    Subproceso * subprocesos = (Subproceso *) malloc(sizeof(Subproceso) * totalDeSubprocesos);
    Subproceso * finSubprocesos = subprocesos + totalDeSubprocesos;

    Subproceso * s = subprocesos;
    i = arr;
    j = 0;
    int k = 0;

    while (s < finSubprocesos){
        if (k == *i){
            contadorDeSubprocesos = 0;
            s->id = contadorDeSubprocesos;

            ++j;
            s->padre = j;

            ++k;
            ++i;

            printf("\nProporcione el tiempo de ejecución del subproceso %d-%d: ", s->padre, s->id);
            scanf("%d", &s->tiempoDeEjecucion);

            ++s;
        }

        else {
            ++contadorDeSubprocesos;
            s->id = contadorDeSubprocesos;

            s->padre = j;
            ++k;

            printf("\nProporcione el tiempo de ejecución del subproceso %d-%d: ", s->padre, s->id);
            scanf("%d", &s->tiempoDeEjecucion);

            ++s;
        }
    }

    for (s = subprocesos; s < finSubprocesos; ++s){
        printf("\nSubproceso %d-%d \nTiempo de Ejecución: %d", s->padre, s->id, s->tiempoDeEjecucion);
    }

    printf("\n");

    free(subprocesos);
    free(arr);

    return 0;
}