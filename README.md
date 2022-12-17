# Comp.Paralela-MPIRegresionLineal
Una aplicación que, dada una fecha futura, pueda predecir la cantidad de accidentes esperados para el día de entrada. 
Esta aplicación utiliza un modelo de regresión lineal, que utiliza los datos históricos, Codificada en C++ y utiliza OpenMPI para maximizar el rendimiento.

La aplicación lee la fecha como el primer argumento de la línea de comandos, el formato
de fecha corresponde al ISO 8601.

formato de ejecucion:
```
mpirun -hostfile <Archivo con ip de las maquinas> <dir Ejecutable> <DD-MM-YYYY>
```
Ejemplo de ejecucion:
```
mpirun -hostfile maquinas ./dist/programa 01-09-2024
```
