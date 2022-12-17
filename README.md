# Comp.Paralela-MPIRegresionLineal
Esta aplicación utiliza un modelo de regresión lineal basado en datos históricos para predecir la cantidad de accidentes esperados en una fecha futura dada. La aplicación está codificada en C++ y utiliza OpenMPI para aprovechar al máximo el rendimiento.

Para utilizar la aplicación, proporcione la fecha en formato ISO 8601 como el primer argumento de la línea de comandos. También debe proporcionar un archivo con la lista de IPs de las máquinas que se utilizarán para la ejecución en paralelo utilizando el parámetro -hostfile.


Formato de ejecución:
```
mpirun -hostfile <Archivo con ip de las maquinas> <dir Ejecutable> <DD-MM-YYYY>
```
Ejemplo de ejecución:
```
mpirun -hostfile maquinas.txt ./dist/programa 01-09-2024
```
Nota: Asegúrate de tener OpenMPI instalado y configurado correctamente en todas las máquinas que se utilizarán para la ejecución en paralelo.
