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
	auto start = high_resolution_clock::now();
	CreacionDatos creador;
	TablaHash listado;
	//creador.CrearDatos(listado);

	TrabajoPrograma Uso;
	Uso.UsoDelPrograma(listado);
	auto end = high_resolution_clock::now();
	auto duracion = duration_cast<milliseconds>(end-start);
	cout<<"Tiempo: "<<duracion.count()<<endl;
	return 0;
}


	

