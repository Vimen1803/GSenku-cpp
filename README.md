# GSenku-cpp
Famoso juego chino recreado en C++. Con distintos tipos de tablero y movimientos habilitados, se manaeja a través de la terminal o un compilador de C++

Se describen a continuacion la entrada, el objetivo y la salida esperada de GSenku.

# Entrada: 
El programa leerá de la línea de comandos los siguientes parámetros:
ficheroTablero ficheroMovimientos retardo [ficheroSalida]
donde:
- ficheroTablero es el nombre del fichero que contiene la definición del tablero a utilizar así como el estado del mismo.
- ficheroMovimientos es el nombre del fichero que contiene los movimientos permitidos.
- Retardo es el núumero de milisegundos que se debe esperar tras mostrar el estado
del tablero, antes de mostrarlo en su estado siguiente.
- ficheroSalida es un parámetro opcional para almacenar los movimientos de la solución, o un -1 si no ha conseguido encontrar ninguna. En dicho fichero se almacenará un movimiento por línea, y cada movimiento simplemente se especificará con la posición inicial y final de la pieza movida. Si no se da un nombre de fichero de salida, se tendrá que utilizar por defecto resultado.txt.

# Objetivo: 
- El objetivo es solucionar un tablero que nos dan en un estado determinado utilizando únicamente los movimientos permitidos. Como se ha comentado anteriormente, este solitario se soluciona cuando se deja una sola pieza en el tablero. La solución (la lista de movimientos válidos que se han seguido) se almacenará en el fichero de resultados (resultado.txt por defecto, pero puede ser definido por el parámetro opcional de la línea de comandos).
- Si retardo> 0 entonces se deberá mostrar por la pantalla el estado del tablero tras cada movimiento. Después de mostrar el estado del tablero, el programa deberá hacer una pausa de retardo milisegundos. Para acelerar la ejecución y pruebas, si retardo ≤ 0, no se mostrará el tablero, almacenando únicamente la solución en el fichero.
- El programa acabará cuando se alcance la solución o cuando se determine que no hay
solución.

# Ejecucion
```
./mainGsenku [tablero.txt] [movimientos.txt] [retardo(1)] [fichero-de-salida.txt]
```
