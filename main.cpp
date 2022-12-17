#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <mpi.h>

using namespace std;

// Función que convierte una fecha a un valor double
double dateToDouble(int day, int month, int year)
{
    // Creamos una estructura tm para representar la fecha
    tm date;
    date.tm_mday = day;
    date.tm_mon = month - 1; // Los meses van de 0 a 11
    date.tm_year = year - 1900; // Los años se cuentan a partir de 1900
    date.tm_hour = 0; // Inicializamos la hora a 0
    date.tm_min = 0; // Inicializamos los minutos a 0
    date.tm_sec = 0; // Inicializamos los segundos a 0
    

    // Convertimos la fecha a un valor double utilizando la función mktime
    time_t timestamp = mktime(&date);

    // Convertimos el timestamp a un valor double y lo devolvemos
    return static_cast<double>(timestamp);
}


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
    int day, month, year;
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

        
        //Separa la fecha en año mes y dia para luego convertir el valor resultante en un valor double
        size_t pos2 = date_str.find("-");
        string year_str = date_str.substr(0, pos2);
        year = stoi(year_str);
        pos2 += 1;

        pos2 = date_str.find("-", pos2);
        string month_str = date_str.substr(pos2 - 2);
        month = stoi(month_str);
        pos2 += 1;
        
        string day_str = date_str.substr(pos2);
        day = stoi(day_str);

        //Ajusta el valor de la fecha mediante la funcion dateToDoble
        double date_double = dateToDouble(day,month,year);
        //cout<<"dia: " <<day << "mes: " <<month <<"año: " <<year <<"valorDoble: "<<date_double<<endl;


        double accident = stod(accident_str);
        dates.push_back(date_double);
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

    //cout << "La pendiente de la recta que mejor se ajusta a los datos es: " << slope << endl;
    //cout << "El punto de intersección con el eje y (la constante) es: " << intercept << endl;
    cout << "Para obtener datos correctos la fecha debe estar en el formato ISO 8601: DD-MM-YYYY" << endl;
    //combierte el primer argumento de la linea de comandos a un string
    string arg1(argv[1]);

    //Separa la fecha en año mes y dia para luego convertir el valor resultante en un valor double
    size_t pos2 = arg1.find("-");
    string year_str = arg1.substr(0, pos2);
    year = stoi(year_str);
    pos2 += 1;

    pos2 = arg1.find("-", pos2);
    string month_str = arg1.substr(pos2 - 2);
    month = stoi(month_str);
    pos2 += 1;
        
    string day_str = arg1.substr(pos2);
    day = stoi(day_str);

    //Ajusta el valor de la fecha mediante la funcion dateToDoble 
    //validador para que los meses no sean negativos
    double date_arg1; 
        if(month >= 10){
            cout<< year << "-" << month  << "-" << day;
            date_arg1 = dateToDouble(year,month,day);
        }
        else{
            if(month > 0){
                cout<< year << "-0" << month  << "-" << day;
                date_arg1 = dateToDouble(year,month,day);
            }
            else{

                cout<<"El formato ingresado es incorrecto" << endl;
                return 1;
            }
        }


    // Calculamos la predicción utilizando la ecuación de la recta con los valores determinados por las distintas maquinas (y = mx + b)
    double prediction = slope * date_arg1 + intercept;

    cout << " // Cantidad de accidentes esperados: " << prediction << endl;
    }


    // Finalizamos OpenMPI
    MPI_Finalize();
    return 0;
}