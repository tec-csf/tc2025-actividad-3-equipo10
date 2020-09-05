/*
Autór: Sergio Hernandez Castillo
Matrícula: A01025210
Descripción: Actividad 3 - Administración de un SO - Ejercicio 1

NOTA: Mi compañero Antonio Junco se encargó del ejericio 2, y trabaje junto con Daniel Roa
      ya que a el le tocó hacer este ejercicio también.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int id;
    int padre;
    int tiempoDeEjecucion;
} Subproceso;

typedef struct Nodo{
    Subproceso * s;
    struct Nodo * next;
} Nodo_t;

int conteo = 0;
Nodo_t * head = NULL;

void enqueueRR(int id, int padre, int tiempoDeEjecucion){
    Nodo_t * actual = head;

    if (conteo == 0){
        Subproceso * sp = (Subproceso *) malloc(sizeof(Subproceso));

        sp->id = id;
        sp->padre = padre;
        sp->tiempoDeEjecucion = tiempoDeEjecucion;

        head->s = sp;
    }

    else {
        while (actual->next != NULL){
            actual = actual->next;
        }

        actual->next = malloc(sizeof(Nodo_t));
        Subproceso * sp = (Subproceso *) malloc(sizeof(Subproceso));

        sp->id = id;
        sp->padre = padre;
        sp->tiempoDeEjecucion = tiempoDeEjecucion;

        actual->next->s = sp;
        actual->next->next = NULL;
    }

    ++conteo;
}

Subproceso * dequeueRR(){
    Subproceso * valorDeRegreso;

    if (conteo == 0){
        valorDeRegreso = NULL; 
    }

    else if (conteo == 1){
        valorDeRegreso = head->s;
    }

    else {
        Nodo_t * siguienteProceso = head->next;
        valorDeRegreso = head->s;
        free(head);
        head = siguienteProceso;
    }

    --conteo;

    return valorDeRegreso;
}

int main(){
    head = malloc(sizeof(Nodo_t));
    head->next = NULL;
    int cantidadDeProcesos = 0;
    int cantidadDeSubprocesos = 0;
    int totalDeSubprocesos = 0;
    int contadorDeSubprocesos = -1;
    int contadorDeProcesos = 0;
    int quantum = 5;
    int cpus = 0;
    int tiempoFake = 0;
    
    printf("El quantum dura %d segundos.", quantum);
    printf("\nProporcione la cantidad de CPUs: ");
    scanf("%d", &cpus);

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

    int a = 0;
    i = arr;
    j = 0;
    int k = 0;

    while (a < totalDeSubprocesos){
        if (k == *i){
            contadorDeSubprocesos = 0;
            ++j;
            ++k;
            ++i;
        }

        else {
            ++contadorDeSubprocesos;
            ++k;
        }

        printf("\nProporcione el tiempo de ejecución del subproceso %d-%d: ", j, contadorDeSubprocesos);
        scanf("%d", &tiempoFake);

        printf("Añadiendo proceso a la cola de Round Robin.");
        enqueueRR(contadorDeSubprocesos, j, tiempoFake);

        printf("\n");

        ++a;
    }

    while (conteo > 0){
        for (int b = 0; b < cpus; ++b){
            if (conteo > 0){
                Subproceso * sp = dequeueRR();
                printf("\nSacando al subproceso %d-%d de la cola y insertándolo al CPU %d \nTiempo de ejecución: %d\n", sp->padre, sp->id, b, sp->tiempoDeEjecucion);

                sp->tiempoDeEjecucion -= quantum;

                if (sp->tiempoDeEjecucion <= 0){
                    free(sp);
                }

                else if (sp->tiempoDeEjecucion > 0){
                    enqueueRR(sp->id, sp->padre, sp->tiempoDeEjecucion);
                }

                else {
                    printf("El subproceso %d-%d en CPU %d ha acabado.\n", sp->padre, sp->id, b);
                }
            }
        }

        sleep(quantum);
        printf("\nEl quantum ha finalizado.\n");
    }

    free(arr);
    free(head);
    
    return 0;
}
