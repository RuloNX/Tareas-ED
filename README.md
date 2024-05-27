# Resolución del 8-Puzzle

Este proyecto consiste en la implementación de un algoritmo para resolver el famoso juego de rompecabezas 8-Puzzle utilizando diferentes algoritmos de búsqueda. El objetivo es modelar el problema como un grafo implícito y luego aplicar los siguientes algoritmos de búsqueda:

1. Búsqueda en profundidad (DFS).
2. Búsqueda en anchura (BFS).
3. Búsqueda mejor-primero.

## Descripción del Problema

El 8-Puzzle es un juego de rompecabezas que consiste en un marco de 3x3 (nueve casillas) donde ocho de las casillas contienen un bloque numerado del 1 al 8, y una casilla está vacía. El objetivo del juego es mover los bloques alrededor de la casilla vacía para alcanzar una configuración específica, generalmente ordenando los números en un patrón secuencial.

### Funcionamiento del 8-Puzzle

- **Configuración inicial:** El puzzle comienza con los bloques en una disposición aleatoria o predefinida que no sigue el orden secuencial.
- **Movimientos permitidos:** Puedes deslizar un bloque adyacente a la casilla vacía hacia esta, efectivamente moviendo la casilla vacía a la posición del bloque y dejando la posición anterior del bloque ahora vacía.
- **Objetivo:** El objetivo final es organizar los bloques de manera que los números estén ordenados del 1 al 8, empezando por la casilla superior central y siguiendo de izquierda a derecha y de arriba a abajo. La casilla en la esquina superior izquierda debe quedar vacía.

## Estructura del Proyecto

El proyecto está estructurado de la siguiente manera:

- **`8puzzle.c`**: Archivo principal que contiene la implementación de los algoritmos de búsqueda y las funciones auxiliares.
- **`8puzzle.h`**: Archivo de encabezado que contiene las definiciones de estructuras y funciones utilizadas en `8puzzle.c`.
- **`README.md`**: Este archivo, que proporciona información detallada sobre el proyecto, su estructura, y cómo ejecutarlo.

## Compilación y Ejecución

Para compilar el programa, puedes utilizar cualquier compilador de C estándar. Por ejemplo, en sistemas Unix/Linux puedes usar gcc:

```
gcc 8puzzle.c -o 8puzzle
```

Para ejecutar el programa:

```
./8puzzle
```

## Funcionalidades

- El programa resuelve el 8-Puzzle utilizando los algoritmos de búsqueda en profundidad, búsqueda en anchura y búsqueda mejor-primero.
- Muestra la secuencia de estados desde el estado inicial hasta la solución.
- Informa la cantidad de iteraciones que realizó cada algoritmo para encontrar la solución.

## Contribuciones

- **Raul Gonzalez**: Desarrollo del código, revisión de errores, y otras tareas relacionadas con la implementación del proyecto.
- **Ignacio Calderon**: Desarrollo del código, revisión de errores, y otras tareas relacionadas con la implementación del proyecto.

## Notas

- Se ha utilizado la distancia Manhattan (L1) como heurística para la búsqueda mejor-primero.
- Se recomienda leer detenidamente el código fuente para comprender la implementación de cada algoritmo y las estructuras de datos utilizadas.

## Referencias

- [Wikipedia - 8-Puzzle](https://en.wikipedia.org/wiki/15_puzzle)
- [Distance Metrics in AI: When to Use Each](https://builtin.com/data-science/distance-metrics-ai)

¡Diviértete resolviendo el 8-Puzzle! 🧩🔍
