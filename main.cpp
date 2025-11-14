#include <iostream>
#include <chrono>
#include <cstdlib>
#include <csignal>
#include <cstdio>
#include "CreacionDatos.h"
#include "Hash.h"
#include "TrabajoPrograma.h"
using namespace std;
using namespace std::chrono;

bitset<100000000> CreacionDatos::usadosTelefono;
	
int main (int argc, char *argv[]) {
	//se empieza a medir el tiempo
	auto start = high_resolution_clock::now();
	CreacionDatos creador;
	TablaHash listado;
	/*tenemos dos funciones aqui, CrearDatos y UsoDelPrograma
	CrearDatos sirve para crear los 33 millones de datos iniciales mientras que UsoDelPrograma llama al menu para usar el programa
	despues de la primera ejecucion se debe poner como comentario la funcion CrearDatos para que no genere 33 millones de datos
	//en cada ejecucion
	*/
	//creador.CrearDatos(listado);
	TrabajoPrograma Uso;
	Uso.UsoDelPrograma(listado);
	auto end = high_resolution_clock::now();
	auto duracion = duration_cast<milliseconds>(end-start);
	cout<<"Tiempo: "<<duracion.count()<<endl;
	return 0;
}


	

