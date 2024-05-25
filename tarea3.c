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
    List* actions; //Secuencia de movimientos para llegar al estado
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
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
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
        }
        current = list_next(actions);
    }
}

void dfs(State* initial_state) {
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
            return;
        }

        List* neighbors = get_neighbors(state);
        void* neighbor;
        while ((neighbor = list_popFront(neighbors)) != NULL) {
            list_pushBack(stack, neighbor);
        }
        list_clean(neighbors);
    }

    printf("No se encontró solución\n");
}

void bfs(State* initial_state) {
    List* queue = list_create();
    list_pushBack(queue, initial_state);
    int iterations = 0;

    while (list_size(queue) > 0) {
        State* state = list_popFront(queue);
        iterations++;

        if (is_final_state(state)) {
            printf("Solución encontrada en %d iteraciones\n", iterations);
            imprimirEstado(state);
            imprimirSecuencia(state->actions);
            printf("Total de movimientos: %d\n", list_size(state->actions));
            return;
        }

        List* neighbors = get_neighbors(state);
        void* neighbor;
        while ((neighbor = list_popFront(neighbors)) != NULL) {
            list_pushBack(queue, neighbor);
        }
        list_clean(neighbors);
    }

    printf("No se encontró solución\n");
}

void best_first(State* initial_state) {
    Heap* heap = heap_create();
    heap_push(heap, initial_state, distancia_L1(initial_state));
    int iterations = 0;

    while (heap_top(heap) != NULL) {
        State* state = heap_top(heap);
        heap_pop(heap);
        iterations++;

        if (is_final_state(state)) {
            printf("Solución encontrada en %d iteraciones\n", iterations);
            imprimirEstado(state);
            imprimirSecuencia(state->actions);
            printf("Total de movimientos: %d\n", list_size(state->actions));
            return;
        }

        List* neighbors = get_neighbors(state);
        void* neighbor;
        while ((neighbor = list_popFront(neighbors)) != NULL) {
            heap_push(heap, neighbor, distancia_L1(neighbor));
        }
        list_clean(neighbors);
    }

    printf("No se encontró solución\n");
}

int main() {
    // Estado inicial del puzzle
    State estado_inicial = {
        {{0, 2, 8}, // Primera fila (0 representa espacio vacío)
         {1, 3, 4}, // Segunda fila
         {6, 5, 7}}, // Tercera fila
        0, 1   // Posición del espacio vacío (fila 0, columna 1)
    };
    estado_inicial.actions = list_create();

    // Imprime el estado inicial
    printf("Estado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    printf("Distancia L1:%d\n", distancia_L1(&estado_inicial));

    int opcion;
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

        switch (opcion) {
            case 1:
                dfs(&estado_inicial);
                break;
            case 2:
                bfs(&estado_inicial);
                break;
            case 3:
                best_first(&estado_inicial);
                break;
            case 4:
                break;
            default:
                printf("Opción no válida. Por favor, intente nuevamente.\n");
                break;
        }
        if (opcion != 4) {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }

    } while (opcion != 4);

    return 0;
}
