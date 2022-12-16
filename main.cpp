#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <mpi.h>

using namespace std;

// Función para calcular la pendiente de la recta
double calculateSlope(vector<double> dates, vector<double> accidents)
{
// inicialización de las variables necesarias para la fórmula de la pendiente
int n = dates.size();
double sum_x = 0;
double sum_y = 0;
double sum_xy = 0;
double sum_x2 = 0;

// Recorremos los vectores de datos y calculamos los valores necesarios para la fórmula de la pendiente
for (int i = 0; i < n; i++)
{
    double x = dates[i];
    double y = accidents[i];

    sum_x += x;
    sum_y += y;
    sum_xy += x * y;
    sum_x2 += x * x;
}

// Calculamos la pendiente utilizando la fórmula
double slope = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);

return slope;
}

// Función para calcular el punto de intersección con el eje y (la constante)
double calculateIntercept(vector<double> dates, vector<double> accidents, double slope)
{
// Inicializamos las variables necesarias para la fórmula de la constante
int n = dates.size();
double sum_x = 0;
double sum_y = 0;


// Recorremos los vectores de datos y calculamos los valores necesarios para la fórmula de la constante
for (int i = 0; i < n; i++)
{
    double x = dates[i];
    double y = accidents[i];

    sum_x += x;
    sum_y += y;
}

// Calculamos la constante utilizando la fórmula
double intercept = (sum_y - slope * sum_x) / n;

return intercept;
}





int main(int argc, char* argv[])
{
    // Inicializamos OpenMPI
    MPI_Init(&argc, &argv);

   // Obtenemos el número total de procesos y el índice del proceso actual
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    vector<double> dates;
    vector<double> accidents;

    // Abrimos el archivo CSV
    ifstream file("datos_examen.csv");

    // Saltamos la primera línea (la cabecera) del archivo.
    string inicio;
    getline(file, inicio);

    // Leemos las líneas restantes del archivo y las procesamos
    string line;
    while (getline(file, line)){
        // Si el índice del proceso actual es 0, procesamos la línea
        if (world_rank == 0)
        {
        // Separamos la línea en dos partes, utilizando el carácter ";" como separador
        size_t pos = line.find(";");
        string date_str = line.substr(0, pos);
        string accident_str = line.substr(pos + 1);
        // Convertimos las dos partes a valores numéricos y los almacenamos en los vectores de datos
        double date = stod(date_str);
        double accident = stod(accident_str);
        dates.push_back(date);
        accidents.push_back(accident);
        }
        else{
           
        }

    }

    // Cerramos el archivo
    file.close();

    // Calculamos la pendiente de la recta que mejor se ajusta a los datos con la maquina principal
    double slope = 0;
    if (world_rank == 0)
    {
    slope = calculateSlope(dates, accidents);
    }

    // Enviamos la pendiente a los demás procesos/maquinas
    MPI_Bcast(&slope, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Calculamos el punto de intersección con el eje y (la constante)
    double intercept = calculateIntercept(dates, accidents, slope);




    // Mostramos el resultado por pantalla por el proceso 0
    if (world_rank == 0)
    {
    cout << "La pendiente de la recta que mejor se ajusta a los datos es: " << slope << endl;
    cout << "El punto de intersección con el eje y (la constante) es: " << intercept << endl;



    // Pedimos al usuario que ingrese una fecha futura para la predicción
    double date;
    cout << "Ingrese una fecha futura (en el mismo formato que los datos históricos): ";
    cin >> date;

    // Calculamos la predicción utilizando la ecuación de la recta con los valores determinados por las distintas maquinas
    double prediction = slope * date + intercept;

    cout << "Cantidad de accidentes esperados para la fecha ingresada: " << endl << prediction << endl;
    }


    // Finalizamos OpenMPI
    MPI_Finalize();
    return 0;
}