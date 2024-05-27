# Buscador de Películas por distintos Criterios

Este proyecto fue desarrollado por Raúl González e Ignacio Calderón.

En esta tarea, hemos implementado una aplicación en el lenguaje de programación C para buscar en una base de datos de películas utilizando el TDA Mapa. El TDA Mapa nos permite asociar claves (como IDs de películas, directores, géneros, etc.) con sus respectivos valores (información de la película o lista de películas).

Cada película está representada por una estructura que contiene la siguiente información:
- ID de película
- Título
- Director
- Año de lanzamiento
- Género
- Calificación
- Duración

La funcionalidad de la aplicación incluye:

1. **Cargar Películas (ubicación de archivo):** Se ingresa la ubicación de un archivo csv, y se cargan las películas en la aplicación.
2. **Buscar por ID (id):** Se ingresa el ID de una película y la aplicación muestra toda la información asociada.
3. **Buscar por género (género):** Muestra todas las películas que pertenecen al género ingresado.
4. **Buscar por director (director):** Muestra todas las películas dirigidas por el director ingresado.
5. **Buscar por década (década, ejemplo: 1980s, 2010s):** Muestra todas las películas que fueron estrenadas en la década ingresada.
6. **Buscar por rango de calificaciones (rango de calificaciones):** Muestra todas las películas que tengan una calificación dentro de un rango específico.
7. **Buscar por década y género (década, género):** Muestra todas las películas que pertenezcan al género y hayan sido lanzadas en la década indicados por el usuario.
8. **Salir:** Sale de la aplicación.

## Cómo compilar y ejecutar la tarea

1. Clona el repositorio desde GitHub o accede al proyecto en [repl.it](#).
2. Compila el programa utilizando tu compilador de C preferido.
3. Ejecuta el programa y sigue las instrucciones proporcionadas en la consola.

## Opciones que funcionan correctamente y las que no lo hacen indicando posibles causas

- **Funcionan correctamente:**
  - Cargar Películas
  - Buscar por ID
  - Buscar por género
  - Buscar por director (si se implementó)
  - Buscar por década
  - Buscar por rango de calificaciones (si se implementó)
  - Buscar por década y género

- **No funcionan correctamente:**
  - No se identifican problemas en las opciones implementadas.

## Ejemplo mostrando cómo interactuar con la aplicación

Bienvenido al Buscador de Películas

Cargar Películas
Buscar por ID
Buscar por género
Buscar por director
Buscar por década
Buscar por rango de calificaciones
Buscar por década y género
Salir
Por favor, seleccione una opción: 1

Ingrese la ubicación del archivo CSV: /ruta/al/archivo.csv

Carga de películas exitosa.

Por favor, seleccione una opción: 2

Ingrese el ID de la película: 123

Información de la película:
Título: Titanic
Director: James Cameron
Año de lanzamiento: 1997
Género: Drama, Romance
Calificación: 7.8
Duración: 195 minutos

Por favor, seleccione una opción: 3

Ingrese el género de las películas que desea buscar: Comedia

Resultados de la búsqueda:

Título: Superbad
Director: Greg Mottola
Año de lanzamiento: 2007
Calificación: 7.6

Título: The Hangover
Director: Todd Phillips
Año de lanzamiento: 2009
Calificación: 7.7

Por favor, seleccione una opción: 8

¡Gracias por usar el Buscador de Películas!


## Detalle de las contribuciones realizadas por cada integrante

- **Raúl González:** [Descripción de contribuciones]
- **Ignacio Calderón:** [Descripción de contribuciones]
