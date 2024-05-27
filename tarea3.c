#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"

// Definición de la estructura para el estado del puzzle
typedef struct {
    int square[3][3]; // Matriz 3x3 que representa el tablero
    int x;    // Posición x del espacio vacío
    int y;    // Posición y del espacio vacío
    List* actions; // Secuencia de movimientos para llegar al estado
} State;

int distancia_L1(State* state) {
    int ev = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int val = state->square[i][j];
            if (val == 0) continue;
            int ii = (val - 1) / 3;
            int jj = (val - 1) % 3;
            ev += abs(ii - i) + abs(jj - j);
        }
    }
    return ev;
}

bool is_final_state(State* state) {
    int goal[3][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (state->square[i][j] != goal[i][j]) {
                return false;
            }
        }
    }
    return true;
}

State* copy_state(State* state) {
    State* new_state = (State*)malloc(sizeof(State));
    if (!new_state) {
        fprintf(stderr, "Error al asignar memoria para new_state\n");
        exit(1);
    }
    memcpy(new_state->square, state->square, sizeof(int) * 9);
    new_state->x = state->x;
    new_state->y = state->y;
    new_state->actions = list_create();
    void* node = list_first(state->actions);
    while (node != NULL) {
        list_pushBack(new_state->actions, node);
        node = list_next(state->actions);
    }
    return new_state;
}

void free_state(State* state) {
    if (state) {
        list_clean(state->actions);
        free(state->actions);
        free(state);
    }
}

State* transition(State* state, int dx, int dy) {
    int new_x = state->x + dx;
    int new_y = state->y + dy;
    if (new_x < 0 || new_x >= 3 || new_y < 0 || new_y >= 3) {
        return NULL;
    }

    State* new_state = copy_state(state);
    new_state->square[state->x][state->y] = state->square[new_x][new_y];
    new_state->square[new_x][new_y] = 0;
    new_state->x = new_x;
    new_state->y = new_y;
    list_pushBack(new_state->actions, (void*)((intptr_t)((dx * 3) + dy)));
    return new_state;
}

List* get_neighbors(State* state) {
    List* neighbors = list_create();
    State* new_state;

    new_state = transition(state, -1, 0); // mover arriba
    if (new_state) list_pushBack(neighbors, new_state);

    new_state = transition(state, 1, 0); // mover abajo
    if (new_state) list_pushBack(neighbors, new_state);

    new_state = transition(state, 0, -1); // mover izquierda
    if (new_state) list_pushBack(neighbors, new_state);

    new_state = transition(state, 0, 1); // mover derecha
    if (new_state) list_pushBack(neighbors, new_state);

    return neighbors;
}

// Función para imprimir el estado del puzzle
void imprimirEstado(const State *estado) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (estado->square[i][j] == 0)
                printf("x "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("%d ", estado->square[i][j]);
        }
        printf("\n");
    }
}

void imprimirSecuencia(List* actions) {
    printf("Secuencia de movimientos realizados:\n");
    void* current = list_first(actions);
    while (current != NULL) {
        int action = (intptr_t)current;
        switch (action) {
            case -3: printf("Arriba\n"); break;
            case 3: printf("Abajo\n"); break;
            case -1: printf("Izquierda\n"); break;
            case 1: printf("Derecha\n"); break;
            default: printf("Movimiento desconocido: %d\n", action); break;
        }
        current = list_next(actions);
    }
}

State* dfs(State* initial_state) {
    List* stack = list_create();
    list_pushBack(stack, initial_state);
    int iterations = 0;

    while (list_size(stack) > 0) {
        State* state = list_popBack(stack);
        iterations++;

        if (is_final_state(state)) {
            printf("Solución encontrada en %d iteraciones\n", iterations);
            imprimirEstado(state);
            imprimirSecuencia(state->actions);
            printf("Total de movimientos: %d\n", list_size(state->actions));
            list_clean(stack);
            free(stack);
            return state;
        }

        List* neighbors = get_neighbors(state);
        void* neighbor;
        while ((neighbor = list_popFront(neighbors)) != NULL) {
            list_pushBack(stack, neighbor);
        }

        list_clean(neighbors);
        free(neighbors);
        break;
    }

    printf("No se encontró solución\n");
    list_clean(stack);
    free(stack);
    return NULL;
}

State* bfs(State* initial_state) {
    List* queue = list_create();
    list_pushBack(queue, initial_state);
    int iterations = 0;

    while (list_size(queue) > 0) {
        State* state = list_popFront(queue);
        iterations++;

        if (is_final_state(state)) {

            imprimirEstado(state);
            imprimirSecuencia(state->actions);
            printf("Total de movimientos: %d\n", list_size(state->actions));
            list_clean(queue);
            free(queue);
            return state;
        }

        List* neighbors = get_neighbors(state);
        void* neighbor;
        while ((neighbor = list_popFront(neighbors)) != NULL) {
            list_pushBack(queue, neighbor);
        }
        list_clean(neighbors);
        free(neighbors);
    }

    printf("No se encontró solución\n");
    list_clean(queue);
    free(queue);
    return NULL;
}

State* best_first(State* initial_state) {
    Heap* priority_queue = heap_create();
    heap_push(priority_queue, initial_state, distancia_L1(initial_state));
    int iterations = 0;
    const int MAX_ITERATIONS = 1000; // Límite máximo de iteraciones

    while (heap_top(priority_queue) != NULL && iterations < MAX_ITERATIONS) {
        State* state = heap_top(priority_queue);
        heap_pop(priority_queue);
        iterations++;

        if (is_final_state(state)) {
            printf("Solución encontrada en %d iteraciones\n", iterations);
            imprimirEstado(state);
            imprimirSecuencia(state->actions);
            printf("Total de movimientos: %d\n", list_size(state->actions));
            heap_pop(priority_queue);
            free(priority_queue);
            return state;
        }

        List* neighbors = get_neighbors(state);
        void* current = list_first(neighbors);
        while (current != NULL) {
            State* neighbor = (State*)current;
            heap_push(priority_queue, neighbor, distancia_L1(neighbor));
            current = list_next(neighbors);
        }
        list_clean(neighbors);
        free(neighbors);
    }

    printf("No se encontró solución después de %d iteraciones\n", MAX_ITERATIONS);
    return NULL;
}

void imprimirComparacion(State* estado_actual) {
    printf("Estado actual del puzzle:\n");
    imprimirEstado(estado_actual);

    if (is_final_state(estado_actual)) {
        printf("\n¡El estado actual coincide con el estado final!\n");
    } else {
        printf("\nEl estado actual NO coincide con el estado final.\n");
    }
}

State* reset_estado_inicial() {
    State* estado_inicial = (State*)malloc(sizeof(State));
    if (!estado_inicial) {
        fprintf(stderr, "Error al asignar memoria para estado_inicial\n");
        exit(1);
    }
    int initial_square[3][3] = {
        {0, 2, 8},
        {1, 3, 4},
        {6, 5, 7}
    };
    memcpy(estado_inicial->square, initial_square, sizeof(int) * 9);
    estado_inicial->x = 0;
    estado_inicial->y = 0;
    estado_inicial->actions = list_create();
    return estado_inicial;
}

int main() {
    State* estado_inicial = reset_estado_inicial();

    // Imprime el estado inicial
    printf("Estado inicial del puzzle:\n");
    imprimirEstado(estado_inicial);

    printf("Distancia L1: %d\n", distancia_L1(estado_inicial));

    int opcion;
    State* resultado = NULL;
    do {
        printf("\n***** EJEMPLO MENU ******\n");
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");

        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");

        printf("Ingrese su opción: ");
        scanf(" %d", &opcion);
        getchar(); // Limpiar el buffer de entrada

        // Reiniciar el estado inicial
        free_state(estado_inicial); // Liberar el estado inicial anterior
        estado_inicial = reset_estado_inicial(); // Reiniciar el estado inicial

        switch (opcion) {
            case 1:
                resultado = dfs(estado_inicial);
                if (resultado != NULL) {
                    printf("Estado final del puzzle:\n");
                    imprimirEstado(resultado);
                    printf("Total de movimientos: %d\n", list_size(resultado->actions));
                    free_state(resultado);
                }
                break;
            case 2:
                resultado = bfs(estado_inicial);
                if (resultado != NULL) {
                    printf("Estado final del puzzle:\n");
                    imprimirEstado(resultado);
                    printf("Total de movimientos: %d\n", list_size(resultado->actions));
                    free_state(resultado);
                }
                break;
            case 3:
                resultado = best_first(estado_inicial);
                imprimirComparacion(estado_inicial);
                if (resultado != NULL) {
                    printf("Estado final del puzzle:\n");
                    imprimirEstado(resultado);
                    printf("Total de movimientos: %d\n", list_size(resultado->actions));
                    free_state(resultado);
                }
                break;
            case 4:
                break;
            default:
                printf("Opción no válida. Por favor, intente nuevamente.\n");
                break;
        }

    } while (opcion != 4);

    free_state(estado_inicial); // Liberar el estado inicial al salir

    return 0;
}