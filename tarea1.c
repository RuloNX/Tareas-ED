#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define MAX_NOMBRE 50
#define MAX_SINTOMA 100

typedef struct {
    char nombre[MAX_NOMBRE];
    int edad;
    char sintoma[MAX_SINTOMA];
    time_t hora_registro;
} Paciente;

typedef struct Nodo {
    Paciente paciente;
    struct Nodo *siguiente;
} Nodo;

typedef struct {
    Nodo *inicio;
} ListaPacientes;

typedef struct {
    ListaPacientes alta;
    ListaPacientes media;
    ListaPacientes baja;
} ListaEspera;

void inicializarLista(ListaPacientes *lista);
void registrarPaciente(ListaEspera *lista);
void mostrarPacientesEnEspera(ListaEspera *lista);
void cambiarPrioridadPaciente(ListaEspera *lista);
void mostrarMenuPrincipal(ListaEspera *lista);
void mostrarListaEspera(ListaEspera *lista);
void mostrarLista(ListaPacientes *lista);
int validarNombre(const char *nombre);
int validarEdad(int edad);
void agregarPaciente(ListaPacientes *lista, Paciente paciente);
void eliminarPaciente(ListaPacientes *lista, const char *nombre);
void ordenarPorHora(ListaPacientes *lista);
int buscarPacienteEnLista(ListaPacientes *lista, const char *nombre, Nodo **encontrado, Nodo **anterior);
void atenderSiguientePaciente(ListaEspera *lista);

int main() {
    ListaEspera lista_espera;
    inicializarLista(&lista_espera.alta);
    inicializarLista(&lista_espera.media);
    inicializarLista(&lista_espera.baja);

    mostrarMenuPrincipal(&lista_espera);

    return 0;
}

void inicializarLista(ListaPacientes *lista) {
    lista->inicio = NULL;
}

void agregarPaciente(ListaPacientes *lista, Paciente paciente) {
    Nodo *nuevo_nodo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevo_nodo == NULL) {
        printf("Error al asignar memoria para el paciente.\n");
        return;
    }
    nuevo_nodo->paciente = paciente;
    nuevo_nodo->siguiente = lista->inicio;
    lista->inicio = nuevo_nodo;
}

void ordenarPorHora(ListaPacientes *lista) {
    if (lista->inicio == NULL || lista->inicio->siguiente == NULL) {
        return; // No se necesita ordenar si la lista está vacía o tiene solo un elemento
    }
    int swapped;
    Nodo *ptr1;
    Nodo *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = lista->inicio;

        while (ptr1->siguiente != lptr) {
            if (ptr1->paciente.hora_registro > ptr1->siguiente->paciente.hora_registro) {
                // Intercambiar los nodos si están en el orden incorrecto
                Paciente temp = ptr1->paciente;
                ptr1->paciente = ptr1->siguiente->paciente;
                ptr1->siguiente->paciente = temp;
                swapped = 1;
            }
            ptr1 = ptr1->siguiente;
        }
        lptr = ptr1;
    } while (swapped);
}

void registrarPaciente(ListaEspera *lista) {
    Paciente nuevo_paciente;
    char entrada[MAX_NOMBRE]; // Para almacenar la entrada del usuario
    system(CLEAR);
    printf("Ingrese el nombre del paciente: ");
    if (scanf("%s", nuevo_paciente.nombre) != 1 || !validarNombre(nuevo_paciente.nombre)) {
        system(CLEAR);
        printf("Error: Ingrese un Nombre Valido.\n");
        sleep(2);
        registrarPaciente(lista);
        return;
    }
    printf("Ingrese la edad del paciente: ");
    if (scanf("%s", entrada) != 1) { // Leer como cadena
        system(CLEAR);
        printf("Error: Ingrese una Edad Valida.\n");
        sleep(2);
        registrarPaciente(lista);
        return;
    }
    // Verificar si la entrada contiene un punto decimal
    if (strchr(entrada, '.') != NULL) {
        system(CLEAR);
        printf("Error: Ingrese una Edad Valida.\n");
        sleep(2);
        registrarPaciente(lista);
        return;
    }
    // Convertir la entrada a entero
    nuevo_paciente.edad = atoi(entrada);
    if (!validarEdad(nuevo_paciente.edad)) {
        system(CLEAR);
        printf("Error: Ingrese una Edad Valida.\n");
        sleep(2);
        registrarPaciente(lista);
        return;
    }
    printf("Ingrese el síntoma del paciente: ");
    scanf(" %99[^\n]", nuevo_paciente.sintoma); // Leer el síntoma sin restricciones
    nuevo_paciente.hora_registro = time(NULL);
    agregarPaciente(&lista->baja, nuevo_paciente);
    ordenarPorHora(&lista->baja); // Ordenar la lista baja prioridad por hora
    system(CLEAR);
    printf("*** Paciente registrado exitosamente ***\n");
    sleep(2);
    system(CLEAR);
}

void mostrarPacientesEnEspera(ListaEspera *lista) {
    system(CLEAR);
    mostrarListaEspera(lista);
    int opcion;
    printf("\n1. Eliminar Paciente\n");
    printf("2. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    if (scanf("%d", &opcion) != 1) {
        system(CLEAR);
        printf("Error al leer la entrada. Inténtelo de nuevo.\n");
        sleep(2);
        system(CLEAR);
        while (getchar() != '\n');
        mostrarPacientesEnEspera(lista);
        return;
    }
    switch (opcion) {
        case 1: {
            system(CLEAR);
            mostrarListaEspera(lista);
            printf("\nIngrese el nombre del paciente a eliminar: ");
            char nombre[MAX_NOMBRE];
            if (scanf("%s", nombre) != 1) {
                system(CLEAR);
                printf("Error al leer el nombre del paciente.\n");
                sleep(2);
                system(CLEAR);
                while (getchar() != '\n');
                return;
            }
            ListaPacientes *listas_prioridad[3] = {&lista->alta, &lista->media, &lista->baja};
            for (int i = 0; i < 3; ++i) {
                Nodo *actual = listas_prioridad[i]->inicio;
                Nodo *anterior = NULL;
                while (actual != NULL && strcmp(actual->paciente.nombre, nombre) != 0) {
                    anterior = actual;
                    actual = actual->siguiente;
                }
                if (actual != NULL) {
                    if (anterior != NULL) {
                        anterior->siguiente = actual->siguiente;
                    } else {
                        listas_prioridad[i]->inicio = actual->siguiente;
                    }
                    free(actual);
                    system(CLEAR);
                    printf("*** Paciente eliminado exitosamente ***\n");
                    sleep(2);
                    system(CLEAR);
                    mostrarPacientesEnEspera(lista);
                    return;
                }
            }
            system(CLEAR);
            printf("El paciente no se encontró en la lista.\n");
            sleep(2);
            system(CLEAR);
            mostrarPacientesEnEspera(lista);
            break;
        }
        case 2:
            mostrarMenuPrincipal(lista);
            break;
        default:
            system(CLEAR);
            printf("Opción no válida. Por favor, seleccione 1 o 2.\n");
            sleep(2);
            system(CLEAR);
            mostrarPacientesEnEspera(lista);
            break;
    }
}

void mostrarListaEspera(ListaEspera *lista) {
    system(CLEAR);
    printf("Lista de pacientes en espera:\n\n");
    printf("==== Alta prioridad ====\n");
    mostrarLista(&lista->alta);
    printf("\n==== Media prioridad ====\n");
    mostrarLista(&lista->media);
    printf("\n==== Baja prioridad ====\n");
    mostrarLista(&lista->baja);
}

void mostrarLista(ListaPacientes *lista) {
    if (lista->inicio == NULL) {
        printf("*** Lista vacía ***\n");
        return;
    }
    Nodo *actual = lista->inicio;
    while (actual != NULL) {
        printf("Nombre: %s, Edad: %d, Síntoma: %s\n", actual->paciente.nombre, actual->paciente.edad, actual->paciente.sintoma);
        actual = actual->siguiente;
    }
}

void mostrarMenuPrincipal(ListaEspera *lista) {
    system(CLEAR);
    int opcion;
    do {
        printf("\n ============= Menú Principal ==============\n");
        printf("|                                            |\n");
        printf("| 1. Registrar nuevo paciente                |\n");
        printf("| 2. Ver lista de pacientes en espera        |\n");
        printf("| 3. Cambiar prioridad de un paciente        |\n");
        printf("| 4. Atender al siguiente paciente           |\n");
        printf("| 5. Salir                                   |\n");
        printf(" ============================================\n");
        printf("\nSeleccione una opción: ");

        if (scanf("%d", &opcion) != 1) {
            system(CLEAR);
            printf("Error al leer la entrada. Inténtelo de nuevo.\n");
            sleep(2);
            system(CLEAR);
            while (getchar() != '\n');
            continue;
        }
        switch (opcion) {
            case 1:
                registrarPaciente(lista);
                break;
            case 2:
                mostrarPacientesEnEspera(lista);
                break;
            case 3:
                cambiarPrioridadPaciente(lista);
                break;
            case 4:
                atenderSiguientePaciente(lista);
                break;
            case 5:
                system(CLEAR);
                printf("Saliendo del programa...\n");
                sleep(2);
                system(CLEAR);
                exit(0);
            default:
                system(CLEAR);
                printf("Opción no válida. Por favor, seleccione una opción del 1 al 5.\n");
                sleep(2);
                system(CLEAR);
                break;
        }
    } while (opcion != 5);
}

void cambiarPrioridadPaciente(ListaEspera *lista) {
    system(CLEAR);
    mostrarListaEspera(lista);
    int opcion;
    printf("\n1. Cambiar prioridad de un paciente\n");
    printf("2. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    if (scanf("%d", &opcion) != 1) {
        system(CLEAR);
        printf("Error al leer la entrada. Inténtelo de nuevo.\n");
        sleep(2);
        system(CLEAR);
        while (getchar() != '\n');
        cambiarPrioridadPaciente(lista);
        return;
    }
    switch (opcion) {
        case 1: {
            system(CLEAR);
            mostrarListaEspera(lista);
            printf("\nIngrese el nombre del paciente: ");
            char nombre[MAX_NOMBRE];
            if (scanf("%s", nombre) != 1) {
                system(CLEAR);
                printf("Error al leer el nombre del paciente.\n");
                while (getchar() != '\n');
                sleep(2);
                system(CLEAR);
                cambiarPrioridadPaciente(lista);
                return;
            }
            printf("Ingrese la nueva prioridad (a - alta, m - media, b - baja): ");
            char prioridad;
            if (scanf(" %c", &prioridad) != 1 || (prioridad != 'a' && prioridad != 'm' && prioridad != 'b')) {
                system(CLEAR);
                printf("Error: Ingrese una prioridad válida (a, m, b).\n");
                while (getchar() != '\n');
                sleep(2);
                system(CLEAR);
                cambiarPrioridadPaciente(lista);
                return;
            }

            ListaPacientes *lista_origen = NULL;
            ListaPacientes *lista_destino = NULL;

            switch (prioridad) {
                case 'a':
                    lista_destino = &lista->alta;
                    break;
                case 'm':
                    lista_destino = &lista->media;
                    break;
                case 'b':
                    lista_destino = &lista->baja;
                    break;
            }

            Nodo *actual = NULL;
            Nodo *anterior = NULL;

            // Buscar al paciente en todas las listas de prioridad
            ListaPacientes *todas_listas[] = {&lista->alta, &lista->media, &lista->baja};
            for (int i = 0; i < 3; i++) {
                if (buscarPacienteEnLista(todas_listas[i], nombre, &actual, &anterior)) {
                    lista_origen = todas_listas[i];
                    break;
                }
            }

            if (lista_origen == NULL) {
                system(CLEAR);
                printf("Error: El paciente no se encontró en ninguna lista.\n");
                sleep(2);
                system(CLEAR);
                cambiarPrioridadPaciente(lista);
                return;
            }

            // Remover al paciente de la lista de origen
            if (anterior != NULL) {
                anterior->siguiente = actual->siguiente;
            } else {
                lista_origen->inicio = actual->siguiente;
            }

            // Agregar al paciente a la nueva lista de prioridad
            agregarPaciente(lista_destino, actual->paciente);
            free(actual);

            // Ordenar la nueva lista por hora de llegada
            ordenarPorHora(lista_destino);

            system(CLEAR);
            printf("*** Cambio de prioridad exitoso ***\n");
            sleep(2);
            system(CLEAR);
            cambiarPrioridadPaciente(lista);
            return;
        }
        case 2:
            mostrarMenuPrincipal(lista);
            break;
        default:
            system(CLEAR);
            printf("Opción no válida. Por favor, seleccione 1 o 2.\n");
            sleep(2);
            system(CLEAR);
            cambiarPrioridadPaciente(lista);
            break;
    }
}

void atenderSiguientePaciente(ListaEspera *lista) {
    system(CLEAR);
    ListaPacientes *listas_prioridad[] = {&lista->alta, &lista->media, &lista->baja};
    int i = 0;
    while (i < 3) {
        if (listas_prioridad[i]->inicio != NULL) {
            Nodo *atendido = listas_prioridad[i]->inicio;
            printf("Atendiendo al siguiente paciente:\n\n");
            printf("Nombre: %s, Edad: %d, Síntoma: %s\n", atendido->paciente.nombre, atendido->paciente.edad, atendido->paciente.sintoma);
            listas_prioridad[i]->inicio = atendido->siguiente;
            free(atendido);
            int opcion;
            printf("\n1. Atender al siguiente paciente\n");
            printf("2. Volver al menú principal\n");
            printf("Seleccione una opción: ");
            if (scanf("%d", &opcion) != 1) {
                printf("Error al leer la entrada. Inténtelo de nuevo.\n");
                while (getchar() != '\n');
                continue;
            }
            switch (opcion) {
                case 1:
                    atenderSiguientePaciente(lista);
                    break;
                case 2:
                    mostrarMenuPrincipal(lista);
                    break;
                default:
                    printf("Opción no válida. Por favor, seleccione 1 o 2.\n");
                    break;
            }
            return;
        }
        i++;
    }
    printf("No hay pacientes en espera.\n");
    sleep(2);
    system(CLEAR);
    mostrarMenuPrincipal(lista);
}


int validarNombre(const char *nombre) {
    while (*nombre) {
        if (!isalpha(*nombre)) {
            return 0;
        }
        nombre++;
    }
    return 1;
}

int validarEdad(int edad) {
    return (edad > 0 && edad <= 179);
}

int buscarPacienteEnLista(ListaPacientes *lista, const char *nombre, Nodo **encontrado, Nodo **anterior) {
    Nodo *actual = lista->inicio;
    *anterior = NULL;
    while (actual != NULL && strcmp(actual->paciente.nombre, nombre) != 0) {
        *anterior = actual;
        actual = actual->siguiente;
    }
    if (actual != NULL) {
        *encontrado = actual;
        return 1;
    }
    return 0;
}