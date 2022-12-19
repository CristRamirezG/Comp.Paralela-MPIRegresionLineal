# Proyecto computación Paralela - MPI, modelo de regresión Lineal
Esta aplicación utiliza un modelo de regresión lineal basado en datos históricos para predecir la cantidad de accidentes esperados en una fecha futura dada. La aplicación está codificada en C++ y utiliza OpenMPI para aprovechar al máximo el rendimiento.

Para utilizar la aplicación, proporcione la fecha en formato ISO 8601 como el primer argumento de la línea de comandos. También debe proporcionar un archivo con la lista de IPs de las máquinas que se utilizarán para la ejecución en paralelo utilizando el parámetro -hostfile.


Formato de ejecución:
```
mpirun -hostfile <Archivo con ip de las maquinas> <dir Ejecutable> <YYYY-MM-DD>
```
Ejemplo de ejecución:
```
mpirun -hostfile maquinas.txt ./dist/programa 2023-12-01
```
Resultado por consola:
```
Fecha: 2023-12-01 // Cantidad de accidentes esperados: 1.74096
```
Nota: Asegúrate de tener OpenMPI instalado y configurado correctamente en todas las máquinas que se utilizarán para la ejecución en paralelo.

## Donde esta utilizando las funciones de la biblioteca MPI
Esta aplicación utiliza las funciones de MPI para la lectura del archivo CSV y el almacenamiento de los datos de este archivo en 2 vectores principales:

* vector<double> dates;
* vector<double>  accidents;

De tal forma que todos los datos queden registrados en la maquina principal (Proceso 0), para luego ser utilizados para el cálculo de la pendiente de la recta, así como el punto de intersección con el eje y.

Además de esto se utiliza la función **MPI_Barrier(MPI_COMM_WORLD)** para asegurarse que la lectura del archivo se realiza de manera completa así como los cálculos de mencionados anteriormente.

