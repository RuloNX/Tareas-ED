#include "tdas/extra.h"
#include "tdas/list.h" // Agrega el archivo de encabezado que contiene la declaración de list_append
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tdas/list.h"
#define MAX_LINEA 256
typedef struct {
    char id[100];
    char titulo[100];
    List *generos;
    char director[300];
    float rating;
    int anio;
} Pelicula;

typedef struct {
    double limite_inferior;
    double limite_superior;
} RangoCalificacion;

void mostrarMenuPrincipal() {
    limpiarPantalla();
    puts("========================================");
    puts("     Base de Datos de Películas");
    puts("========================================");

    puts("1) Cargar Películas");
    puts("2) Buscar por id");
    puts("3) Buscar por director");
    puts("4) Buscar por género");
    puts("5) Buscar por década");
    puts("6) Buscar por rango de calificaciones");
    puts("7) Buscar por década y género");
    puts("8) Salir");
}

int is_equal_str(void *key1, void *key2) {
    return strcmp((char *)key1, (char *)key2) == 0;
}

void cargar_peliculas(Map *pelis_byid) {
    FILE *archivo = fopen("peliculas.csv", "r");
    if (archivo == NULL) return;

    char linea[MAX_LINEA];
    fgets(linea, MAX_LINEA, archivo);

    char id[100], titulo[100], director[300], rating_str[20], anio_str[10], *genero;
    float rating;
    int anio;

    while (fgets(linea, MAX_LINEA, archivo) != NULL) {
        if (sscanf(linea, "%99[^,],%99[^,],%299[^,],%19[^,],%9[^,],", id, titulo, director, rating_str, anio_str) != 5) continue;
        rating = atof(rating_str);
        anio = atoi(anio_str);

        Pelicula *pelicula = (Pelicula *)malloc(sizeof(Pelicula));
        if (pelicula == NULL) break;

        strcpy(pelicula->id, id);
        strcpy(pelicula->titulo, titulo);
        strcpy(pelicula->director, director);
        pelicula->rating = rating;
        pelicula->anio = anio;

        pelicula->generos = list_create();
        if (pelicula->generos == NULL) {
            free(pelicula);
            break;
        }

        char *ptr = linea;
        while ((genero = strsep(&ptr, ",")) != NULL) {
            if (*genero != '\0') {
                list_append(pelicula->generos, strdup(genero));
            }
        }

        map_insert(pelis_byid, pelicula->id, pelicula);
    }

    fclose(archivo);
}

void buscar_por_id(Map *pelis_byid, const char *id) {
    printf("Buscando película por ID: %s\n", id);
    Pelicula *pelicula = map_get(pelis_byid, id);
    if (pelicula != NULL) {
        printf("Título: %s, Director: %s, Año: %d\n",
               pelicula->titulo, pelicula->director, pelicula->anio);
    } else {
        printf("No se encontró ninguna película con el ID: %s\n", id);
    }
}

void buscar_por_genero(Map *pelis_byid, const char *genero) {
    MapPair *pair = map_first(pelis_byid);
    bool encontrado = false;

    while (pair != NULL) {
        Pelicula *pelicula = pair->value;
        List *generos = pelicula->generos;

        for (int i = 0; i < list_size(generos); i++) {
            char *genre = list_get(generos, i);
            if (strcmp(genre, genero) == 0) {
                printf("ID: %s, Título: %s, Director: %s, Año: %d\n",
                       pelicula->id, pelicula->titulo, pelicula->director, pelicula->anio);
                encontrado = true;
                break; 
            }
        }
        pair = map_next(pelis_byid);
    }

    if (!encontrado) {
        printf("No se encontraron películas del género '%s'\n", genero);
    }
}

void buscar_por_decada(Map *pelis_byid, char *decada) {
    MapPair *pair = map_first(pelis_byid);
    while (pair != NULL) {
        Pelicula *pelicula = pair->value;
        int inicio_decada, fin_decada;

        if (strcmp(decada, "1920s") == 0) {
            inicio_decada = 1920;
            fin_decada = 1929;
        } else if (strcmp(decada, "1930s") == 0) {
            inicio_decada = 1930;
            fin_decada = 1939;
        } else if (strcmp(decada, "1940s") == 0) {
            inicio_decada = 1940;
            fin_decada = 1949;
        } else if (strcmp(decada, "1950s") == 0) {
            inicio_decada = 1950;
            fin_decada = 1959;
        } else if (strcmp(decada, "1960s") == 0) {
            inicio_decada = 1960;
            fin_decada = 1969;
        } else if (strcmp(decada, "1970s") == 0) {
            inicio_decada = 1970;
            fin_decada = 1979;
        } else if (strcmp(decada, "1980s") == 0) {
            inicio_decada = 1980;
            fin_decada = 1989;
        } else if (strcmp(decada, "1990s") == 0) {
            inicio_decada = 1990;
            fin_decada = 1999;
        } else if (strcmp(decada, "2000s") == 0) {
            inicio_decada = 2000;
            fin_decada = 2009;
        } else if (strcmp(decada, "2010s") == 0) {
            inicio_decada = 2010;
            fin_decada = 2019;
        } else if (strcmp(decada, "2020s") == 0) {
            inicio_decada = 2020;
            fin_decada = 2029;
        } else {
            printf("La década ingresada no es válida.\n");
            return;
        }

        if (pelicula->anio >= inicio_decada && pelicula->anio <= fin_decada) {
            printf("Título: %s, Año: %d\n", pelicula->titulo, pelicula->anio);
        }

        pair = map_next(pelis_byid);
    }
}

bool is_in_range(double calificacion, RangoCalificacion range) {
    return calificacion >= range.limite_inferior && calificacion <= range.limite_superior;
}

void buscar_por_calificacion(Map *pelis_byid, RangoCalificacion range) {
    MapPair *pair = map_first(pelis_byid);
    bool encontrado = false;

    while (pair != NULL) {
        Pelicula *pelicula = pair->value;

        if (is_in_range(pelicula->rating, range)) {
            printf("ID: %s, Título: %s, Director: %s, Año: %d, Calificación: %.2f\n",
                   pelicula->id, pelicula->titulo, pelicula->director, pelicula->anio, pelicula->rating);
            encontrado = true;
        }

        pair = map_next(pelis_byid);
    }

    if (!encontrado) {
        printf("No se encontraron películas en el rango de calificaciones %.2f - %.2f\n", range.limite_inferior, range.limite_superior);
    }
}

void buscar_por_decada_y_genero(Map *pelis_byid, char *decada, const char *genero) {
    int inicio_decada, fin_decada;

    if (strcmp(decada, "1920s") == 0) {
        inicio_decada = 1920;
        fin_decada = 1929;
    } else if (strcmp(decada, "1930s") == 0) {
        inicio_decada = 1930;
        fin_decada = 1939;
    } else if (strcmp(decada, "1940s") == 0) {
        inicio_decada = 1940;
        fin_decada = 1949;
    } else if (strcmp(decada, "1950s") == 0) {
        inicio_decada = 1950;
        fin_decada = 1959;
    } else if (strcmp(decada, "1960s") == 0) {
        inicio_decada = 1960;
        fin_decada = 1969;
    } else if (strcmp(decada, "1970s") == 0) {
        inicio_decada = 1970;
        fin_decada = 1979;
    } else if (strcmp(decada, "1980s") == 0) {
        inicio_decada = 1980;
        fin_decada = 1989;
    } else if (strcmp(decada, "1990s") == 0) {
        inicio_decada = 1990;
        fin_decada = 1999;
    } else if (strcmp(decada, "2000s") == 0) {
        inicio_decada = 2000;
        fin_decada = 2009;
    } else if (strcmp(decada, "2010s") == 0) {
        inicio_decada = 2010;
        fin_decada = 2019;
    } else if (strcmp(decada, "2020s") == 0) {
        inicio_decada = 2020;
        fin_decada = 2029;
    } else {
        printf("La década ingresada no es válida.\n");
        return;
    }

    printf("Películas del género '%s' lanzadas en la década de los %s:\n", genero, decada);

    MapPair *pair = map_first(pelis_byid);
    while (pair != NULL) {
        Pelicula *pelicula = pair->value;

        if (pelicula->anio >= inicio_decada && pelicula->anio <= fin_decada) {
            List *generos = pelicula->generos;
            for (int i = 0; i < list_size(generos); i++) {
                char *genre = list_get(generos, i);
                if (strcmp(genre, genero) == 0) {
                    printf("Título: %s, Año: %d\n", pelicula->titulo, pelicula->anio);
                    break;
                }
            }
        }
        pair = map_next(pelis_byid);
    }
}

void ejecutarPrograma() {
    char opcion;
    Map *pelis_byid = map_create(is_equal_str);
    RangoCalificacion rango_calificaciones;

    do {
        mostrarMenuPrincipal();
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);
        char decada[10];
        char genero[100];
        char id[100];
        switch (opcion) {
            case '1':
                cargar_peliculas(pelis_byid);
                break;
            case '2':
                printf("Ingrese el ID de la película: ");
                scanf("%s", id);
                buscar_por_id(pelis_byid, id);
                break;
            case '3':
                buscar_por_genero(pelis_byid, "Drama");
                break;
            case '4':
                printf("Ingrese la década (por ejemplo, '1980s'): ");
                scanf("%s", decada);
                buscar_por_decada(pelis_byid, decada);
                break;
            case '5':
                printf("Ingrese el límite inferior del rango de calificaciones: ");
                scanf("%lf", &rango_calificaciones.limite_inferior);
                printf("Ingrese el límite superior del rango de calificaciones: ");
                scanf("%lf", &rango_calificaciones.limite_superior);
                buscar_por_calificacion(pelis_byid, rango_calificaciones);
                break;
            case '6':
                printf("Ingrese la década (por ejemplo, '1980s'): ");
                scanf("%s", decada);
                printf("Ingrese el género: ");
                scanf("%s", genero);
                buscar_por_decada_y_genero(pelis_byid, decada, genero);
                break;
            case '7':
                break;
            default:
                break;
        }
    } while (opcion != '8');

    map_destroy(pelis_byid);
}

int main() {
    ejecutarPrograma();
    return 0;
}
