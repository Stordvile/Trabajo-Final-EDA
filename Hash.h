#ifndef HASH_H
#define HASH_H
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <omp.h>
#include <vector>


using namespace std;

//estructura Lugar para la direccion de vivienda de una persona
struct Lugar{
	char Departamento[13];
	char Provincia[11] ;
	char Ciudad[16];
	char Distrito[20];
	char Ubicacion[15];
};
//estructura Persona para las caracteristicas de una persona
struct Persona{
	char DNI[9];
	char Nombres[20];
	char Apellidos[25];
	char Nacionalidad[16];
	char LugarNacimiento[15];
	Lugar Direccion;
	char Telefono[10];
	char Correo[32];
	char EstadoCivil[11];
};
//definimos el tamano contante para las tablas
const int TAM_TABLA = 12;

class TablaHash{
private:
	/*
	van a haber 3 listas, Creacion para los datos de los archivos, insercion para los datos insertados durante el uso del programa y
	eliminacion para las eliminaciones registradas, se los guarda en tablas distintas para luego poder actualizar los datos y que se refleje
	en los archivos
	*/
	list<pair<char[9], Persona>> Tabla_Creacion[TAM_TABLA];
	list<pair<char[9], Persona>> Tabla_Insercion[TAM_TABLA];
	list<pair<char[9], Persona>> Tabla_Eliminacion[TAM_TABLA];
	vector<Persona> cacheInicio;
	vector<Persona> cacheMedio;
	vector<Persona> cacheFinal;
	

public:
	//funcion para convertir numeros en char, usada en la creacion de datos
	void convertirNumeroAChar(int numero, char* destino, size_t tam) {
		for (int i = tam - 2; i >= 0; --i) {
			destino[i] = '0' + (numero % 10);
			numero /= 10;
		}
		destino[tam - 1] = '\0';
	}
	//funcion para vaciar la tabla creacion, se usa al momento de buscar y en la creacion de datos
	void vaciarHash() {
		for (auto &lista : Tabla_Creacion) { 
			lista.clear();
		}
	}
	//funcion para calcular el Indice, se usa en varios momentos del programa
	int calcularIndiceHash(const char dni[9]) {
		int numero = atoi(dni);
		const int Bloque_DNI = (99999999 - 10000000 + 1) / 12;
		return (numero - 10000000) / Bloque_DNI;
	}
	//funciones para insertar en una tabla, las 3 InsertarCreacion, InsertarEliminacion e InsertarInsercion son iguales cambiando la tabla
	void InsertarCreacion(const Persona &p) {
		int indice = calcularIndiceHash(p.DNI);
		pair<char[9], Persona> elemento;
		strcpy(elemento.first, p.DNI);
		elemento.second = p;
		Tabla_Creacion[indice].push_back(elemento);
	}
	
	void InsertarInsercion(const Persona &p) {
		int indice = calcularIndiceHash(p.DNI);
		pair<char[9], Persona> elemento;
		strcpy(elemento.first, p.DNI);
		elemento.second = p;
		Tabla_Insercion[indice].push_back(elemento);
	}
	
	void InsertarEliminacion(const Persona &p) {
		int indice = calcularIndiceHash(p.DNI);
		pair<char[9], Persona> elemento;
		strcpy(elemento.first, p.DNI);
		elemento.second = p;
		Tabla_Eliminacion[indice].push_back(elemento);
	}
	//funcion buscar, devuelve un puntero a la persona que se encuentre, en caso de no encontrarla devuelve nullptr
	Persona* buscar(const char dni[9]) {
		int indice = calcularIndiceHash(dni);
		
		for (auto &elemento : Tabla_Creacion[indice]) {
			//revisa que el elemento este en Tabla_Creacion pero no en Tabla_Eliminacion
			bool estaEliminado = false;
			for (auto &elemento_eliminado : Tabla_Eliminacion[indice]) {
				if (strcmp(elemento_eliminado.first, dni) == 0) {
					estaEliminado = true;
					break;
				}
			}
			if (estaEliminado)
				continue;
			if (strcmp(elemento.first, dni) == 0)
				return &elemento.second;
		}
		//luego revisa que este en Tabla_Insercion
		for (auto &elemento : Tabla_Insercion[indice]) {
			if (strcmp(elemento.first, dni) == 0)
				return &elemento.second;
		}
		
		return nullptr;
	}
	//funcion para agregar persona, pide todos los datos y luego lo guarda en Inserciones.bin y en Tabla_Insercion
	void agregarPersona() {
		Persona p;
		cout << "Ingrese los datos de la persona" << endl;
		cout << "-------------------------------" << endl;
		
		cout << "DNI: ";
		string Entrada_DNI;
		do {
			cin >> Entrada_DNI;
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpia el salto de línea
			if (Entrada_DNI.length() != 8) {
				cout << "El DNI debe tener exactamente 8 digitos" << endl;
			}
		} while (Entrada_DNI.length() != 8);
		strncpy(p.DNI, Entrada_DNI.c_str(), 8);
		p.DNI[8] = '\0';
		
		cout << "Nombres: ";
		cin.getline(p.Nombres, 20);
		cout << "Apellidos: ";
		cin.getline(p.Apellidos, 25);
		cout << "Nacionalidad: ";
		cin.getline(p.Nacionalidad, 16);
		cout << "Lugar de Nacimiento: ";
		cin.getline(p.LugarNacimiento, 15);
		cout << "Departamento: ";
		cin.getline(p.Direccion.Departamento, 13);
		cout << "Provincia: ";
		cin.getline(p.Direccion.Provincia, 11);
		cout << "Ciudad: ";
		cin.getline(p.Direccion.Ciudad, 16);
		cout << "Distrito: ";
		cin.getline(p.Direccion.Distrito, 20);
		cout << "Ubicacion: ";
		cin.getline(p.Direccion.Ubicacion, 15);
		
		cout << "Telefono: ";
		string Entrada_Telefono;
		do {
			cin >> Entrada_Telefono;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (Entrada_Telefono.length() != 9) {
				cout << "El telefono debe tener exactamente 9 digitos" << endl;
			}
		} while (Entrada_Telefono.length() != 9);
		strncpy(p.Telefono, Entrada_Telefono.c_str(), 9);
		p.Telefono[9] = '\0';
		
		cout << "Correo: ";
		cin.getline(p.Correo, 32);
		char* arroba = strchr(p.Correo, '@');
		char* puntoCom = strstr(p.Correo, ".com");
		if (!arroba || !puntoCom) {
			cout << "Correo invalido: debe contener '@' y terminar en '.com'\n";
			cin.getline(p.Correo, 32);
		}
		
		cout << "Estado Civil: ";
		cin.getline(p.EstadoCivil, 11);
		
		namespace fs = filesystem;
		string nombreArchivo = "./Archivos/Inserciones.bin";
		FILE *archivo = fopen(nombreArchivo.c_str(), "ab");
		if(archivo){
			fwrite(&p, sizeof(Persona), 1, archivo);
			cout << "Persona Insertada correctamente." << endl;
		}
		fclose(archivo);
		InsertarInsercion(p);
	}
	//funcion para eliminar persona, pide el DNI y si la persona esta en Tabla_Creacion y no esta eliminada ya entonces
	//se guarda en Eliminaciones.bin y en Tabla_Eliminacion
	void eliminarPersona(){
		cout << "Ingrese los datos de la persona" << endl;
		cout << "-------------------------------" << endl;
		cout << "DNI: ";
		
		string Entrada_DNI;
		do {
			cin >> Entrada_DNI;
			if (Entrada_DNI.length() != 8)
				cout << "El DNI debe tener exactamente 8 digitos" << endl;
		} while (Entrada_DNI.length() != 8);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		char dni[9];
		strncpy(dni, Entrada_DNI.c_str(), 8);
		dni[8] = '\0';
		int indice = calcularIndiceHash(dni);
		cargarDatos(indice);
		Persona* p = buscar(dni);
		if(!p){
			cout<<"Persona no encontrada"<<endl;
			return;
		}
		
		InsertarEliminacion(*p);
		Persona Encontrada = *p;
		namespace fs = filesystem;
		string nombreArchivo = "./Archivos/Eliminaciones.bin";
		FILE *archivo = fopen(nombreArchivo.c_str(), "ab");
		if(archivo){
			fwrite(&Encontrada, sizeof(Persona), 1, archivo);
			cout << "Persona eliminada correctamente." << endl;
		}
		fclose(archivo);
		vaciarHash();
	}
	//Funciones para guardar los datos clave
	void guardarEnCacheInicio(const Persona &p) {
		namespace fs = std::filesystem;
		fs::path carpeta = "./Archivos";
		
		if (!fs::exists(carpeta))
			fs::create_directory(carpeta);
		
		std::string nombreArchivo = (carpeta / "Cache_Inicio.bin").string();
		FILE *archivo = fopen(nombreArchivo.c_str(), "ab");
		if (archivo) {
			fwrite(&p, sizeof(Persona), 1, archivo);
			fclose(archivo);
		}
	}
	
	void guardarEnCacheMedio(const Persona &p) {
		namespace fs = std::filesystem;
		fs::path carpeta = "./Archivos";
		
		if (!fs::exists(carpeta))
			fs::create_directory(carpeta);
		
		std::string nombreArchivo = (carpeta / "Cache_Medio.bin").string();
		FILE *archivo = fopen(nombreArchivo.c_str(), "ab");
		if (archivo) {
			fwrite(&p, sizeof(Persona), 1, archivo);
			fclose(archivo);
		}
	}
	
	void guardarEnCacheFinal(const Persona &p) {
		namespace fs = std::filesystem;
		fs::path carpeta = "./Archivos";
		
		if (!fs::exists(carpeta))
			fs::create_directory(carpeta);
		
		std::string nombreArchivo = (carpeta / "Cache_Final.bin").string();
		FILE *archivo = fopen(nombreArchivo.c_str(), "ab");
		if (archivo) {
			fwrite(&p, sizeof(Persona), 1, archivo);
			fclose(archivo);
		}
	}
	//Funcion para guardar una persona en uno de los archivos Datos
	void guardarPersona(int idHilo, const Persona &p){
		namespace fs = filesystem;
		fs::path carpeta = "./Archivos";
		if (!fs::exists(carpeta)) fs::create_directory(carpeta);
		
		string nombreArchivo = carpeta.string() + "/Datos" + to_string(idHilo + 1) + ".bin";
		FILE *archivo = fopen(nombreArchivo.c_str(), "ab"); // append binario
		if (archivo) {
			fwrite(&p, sizeof(Persona), 1, archivo);
			fclose(archivo);
		}
	}
	//funcion para ver todas las inserciones
	void MostrarInserciones(){
		string ruta = "./Archivos/Inserciones.bin";
		
		FILE *archivo = fopen(ruta.c_str(), "rb");
		if(!archivo){
			cout<<"No se pudo abrir el archivo: "<<ruta<<endl;
			return;
		}
		
		fseek(archivo, 0, SEEK_END);
		int tamArchivo = ftell(archivo);
		fclose(archivo);
		int tamPersona = sizeof(Persona);
		int tamRegistros = tamArchivo/tamPersona;
		bool errorEnHilo = false;
		//paralelizamos la busqueda para leer el archivo Inserciones lo mas rapido posible
		#pragma omp parallel
		{
			int tid = omp_get_thread_num();
			int inicio = (tamRegistros/TAM_TABLA) * tid;
			int fin = (tid == TAM_TABLA-1) ? tamRegistros : inicio + (tamRegistros/TAM_TABLA);
			int cantidad = fin - inicio;
			
			FILE *pf = fopen(ruta.c_str(), "rb");
			if(!pf){
#pragma omp atomic write
				errorEnHilo = true;
			}
			
			fseek(pf, inicio * tamPersona, SEEK_SET);
			Persona p;
			for(int i = 0; i < cantidad; i++){
				size_t leido = fread(&p, tamPersona, 1, pf);
				if(leido == 1){
					#pragma omp critical
					{
						MostrarPersona(p);
					}
				}
			}
			fclose(pf);
		};
		//en caso haya un hilo que no pudo acceder se notifica
		if(errorEnHilo){
			cout << "Un hilo no pudo abrir el archivo." << endl;
			return;
		}
	}
	//Misma logica que las Inserciones solo que para mostrar las Eliminaciones registradas
	void MostrarEliminaciones(){
		string ruta = "./Archivos/Eliminaciones.bin";
		
		FILE *archivo = fopen(ruta.c_str(), "rb");
		if(!archivo){
			cout<<"No se pudo abrir el archivo: "<<ruta<<endl;
			return;
		}
		
		fseek(archivo, 0, SEEK_END);
		int tamArchivo = ftell(archivo);
		fclose(archivo);
		int tamPersona = sizeof(Persona);
		int tamRegistros = tamArchivo/tamPersona;
		bool errorEnHilo = false;

		#pragma omp parallel
		{
			int tid = omp_get_thread_num();
			int inicio = (tamRegistros/TAM_TABLA) * tid;
			int fin = (tid == TAM_TABLA-1) ? tamRegistros : inicio + (tamRegistros/TAM_TABLA);
			int cantidad = fin - inicio;
			
			FILE *pf = fopen(ruta.c_str(), "rb");
			if(!pf){
				#pragma omp atomic write
				errorEnHilo = true;
			}
			
			fseek(pf, inicio * tamPersona, SEEK_SET);
			Persona p;
			for(int i = 0; i < cantidad; i++){
				size_t leido = fread(&p, tamPersona, 1, pf);
				if(leido == 1){
					#pragma omp critical
					{
					MostrarPersona(p);
					}
				}
			}
			fclose(pf);
		};
			
		if(errorEnHilo){
			cout << "Un hilo no pudo abrir el archivo." << endl;
			return;
		}
	}
	//Funcion para mostrar los valores clave, ya que siempre seran 10 valores en cada archivo no es necesario paralelizarlo
	//no se ganaria casi nada de tiempo
	void mostrarValoresClave() {
		const vector<string> archivos = {
			"./Archivos/Cache_Inicio.bin",
			"./Archivos/Cache_Medio.bin",
			"./Archivos/Cache_Final.bin"
		};
		
		const vector<string> titulos = {
			"\n===== 10 PRIMEROS REGISTROS =====",
			"\n===== 10 REGISTROS DEL MEDIO =====",
			"\n===== 10 ULTIMOS REGISTROS ====="
		};
		
		for (size_t i = 0; i < archivos.size(); ++i) {
			cout << titulos[i] << endl;
			
			FILE* archivo = fopen(archivos[i].c_str(), "rb");
			if (!archivo) {
				cerr << "No se pudo abrir el archivo: " << archivos[i] << endl;
				continue;
			}
			
			Persona p;
			int contador = 1;
			while (fread(&p, sizeof(Persona), 1, archivo) == 1) {
				cout << "Registro #" << contador++ << endl;
				MostrarPersona(p);
			}
			cout.flush();
			fclose(archivo);
		}
	}
	//Funcion para mostrar los datos de una persona
	void MostrarPersona(Persona &p){
		cout << "------------------------------"<<endl;
		cout << "DNI: " << p.DNI << endl;
		cout << "Nombres: " << p.Nombres << endl;
		cout << "Apellidos: " << p.Apellidos << endl;
		cout << "Nacionalidad: " << p.Nacionalidad << endl;
		cout << "Lugar de Nacimiento: " << p.LugarNacimiento << endl;
		cout << "Departamento: " << p.Direccion.Departamento << endl;
		cout << "Provincia: " << p.Direccion.Provincia << endl;
		cout << "Ciudad: " << p.Direccion.Ciudad << endl;
		cout << "Distrito: " << p.Direccion.Distrito << endl;
		cout << "Ubicacion: " << p.Direccion.Ubicacion << endl;
		cout << "Telefono: " << p.Telefono << endl;
		cout << "Correo: " << p.Correo << endl;
		cout << "Estado Civil: " << p.EstadoCivil << endl;
		cout << "-----------------------------" << endl;
	}
	//Funcion para cargar los datos, recibe como parametro el Indice del archivo que se quiere cargar
	void cargarDatos(int indice){
		const vector<string> archivos = {
			"./Archivos/Datos1.bin",
			"./Archivos/Datos2.bin",
			"./Archivos/Datos3.bin",
			"./Archivos/Datos5.bin",
			"./Archivos/Datos5.bin",
			"./Archivos/Datos6.bin",
			"./Archivos/Datos7.bin",
			"./Archivos/Datos8.bin",
			"./Archivos/Datos9.bin",
			"./Archivos/Datos10.bin",
			"./Archivos/Datos11.bin",
			"./Archivos/Datos12.bin",
		};
		
		FILE* archivo = fopen(archivos[indice].c_str(), "rb");
		if (!archivo) {
			cout << "No se pudo abrir el archivo: " << archivos[indice] << endl;
			return;
		}
		
		fseek(archivo, 0, SEEK_END);
		long tamArchivo = ftell(archivo);
		fclose(archivo);
		
		long tamPersona = sizeof(Persona);
		long tamRegistros = tamArchivo / tamPersona;
		
		//paralelizamos la lectura, para esto lo guardamos en un vector vectores de personas
		vector<vector<Persona>> buffers(omp_get_max_threads());
		
		#pragma omp parallel
		{
			Persona p;
			int tid = omp_get_thread_num();
			int inicio = (tamRegistros/TAM_TABLA) * tid;
			int fin = (tid == TAM_TABLA-1) ? tamRegistros : inicio + (tamRegistros/TAM_TABLA);
			int cantidad = fin - inicio;
			FILE *pf = fopen(archivos[indice].c_str(), "rb");
			fseek(pf, inicio * tamPersona, SEEK_SET);
			
			for(long i = 0; i < cantidad; i++){
				if(fread(&p, tamPersona, 1, pf) == 1){
					buffers[tid].push_back(p);
				}
			}
			fclose(pf);
		};
		//no se puede paralelizar la insercion en la Lista Hash sin embargo lo que mas demora es la lectura del archivo
		//por lo que paralelizar en este caso si representa un gran beneficio
		for(const auto& buffer : buffers){
			for(const auto& elem : buffer){
				InsertarCreacion(elem);
			}
		}
		cout<<"Se han cargado "<<tamRegistros<<" de "<<archivos[indice]<<endl;
	}
	//funcion para Actualizar los datos guardados en archivos
	void ActualizacionDatos(){
		//el motivo de porque no se llama a cargarDatos para la parte en que debemos cargarlos es que aqui ya vamos a paralelizar
		//y cada hilo trabajara un archivo entonces no podemos llamar a otra funcion que tambien realiza paralelizacion
		namespace fs = std::filesystem;
		const vector<string> archivos = {
			"./Archivos/Datos1.bin",
			"./Archivos/Datos2.bin",
			"./Archivos/Datos3.bin",
			"./Archivos/Datos4.bin",
			"./Archivos/Datos5.bin",
			"./Archivos/Datos6.bin",
			"./Archivos/Datos7.bin",
			"./Archivos/Datos8.bin",
			"./Archivos/Datos9.bin",
			"./Archivos/Datos10.bin",
			"./Archivos/Datos11.bin",
			"./Archivos/Datos12.bin",
		};
		//vaciamos el Hash por si acaso antes de cualquier cosa
		vaciarHash();
		#pragma omp parallel
		{
			//la paralelizacion aqui funcionara de tal forma que cada hilo se encargara de un indice de la Tabla_Creacion, Tabla_Eliminacion y Tabla_Insercion
			int tid = omp_get_thread_num();
			FILE* archivo = fopen(archivos[tid].c_str(), "rb");
			Persona p;
			while(!Tabla_Eliminacion[tid].empty()){
				//los hilos verifican que hayan indices en sus respectivos hilos, en caso no los haya simplemente
				//no entran aqui ya no no deben de hacer ninguna revision
				while(fread(&p, sizeof(Persona), 1, archivo)==1){
					bool estaEliminado = false;
					for (auto &elemento_eliminado : Tabla_Eliminacion[tid]) {
						if (strcmp(elemento_eliminado.first, p.DNI) == 0) {
							//en caso se encuentre un elemento de la Tabla_Eliminacion se lo retira, esto porque como ya se encontro para nosotros
							//ya no es relevante y es una iteracion que seria inutil en futuros bucles
							//lo sacamos y este proceso se repite hasta que Tabla_Eliminacion[tid] esye vacio
							Tabla_Eliminacion[tid].remove_if([&p](const pair<char[9], Persona>& elem) {
								return strcmp(elem.first, p.DNI) == 0;
							});
							estaEliminado = true;
							break;
						}
					}
					//si un elemento no esta eliminado se lo manda a la Tabla_Creacion, aqui esta parte si se puede paralelizar
					//porque cada hilo maneja Indices de la tabla distintos de tal forma que no habra cruces en la memoria
					if (!estaEliminado) {
						InsertarCreacion(p);
					}
				}
				//aqui se da la orden de borrar los archivos, solo se da si hay eliminaciones porque para guardar las inserciones en los archivos no
				//es necesario borrarlos
				fclose(archivo);
				fs::remove(archivos[tid].c_str());
			}
			//ahora vemos que la tabla Tabla_Insercion no este vacia,
			//aqui se hace If en lugar del while anterior porque aca no vamos a usar bucles anidados que tengan que buscar algo en cada ciclo
			if(!Tabla_Insercion[tid].empty()){
				for(auto &elemento_insertado : Tabla_Insercion[tid]){
					InsertarCreacion(elemento_insertado.second);
				}
			}
			//una vez que nuestra Tabla_Creacion tenga sus elementos actualizados empezamos a guardarla en nuevos archivos
			//el porque esto se puede paralelizar es que cada hilo trabaja con un archivo distinto
			for(auto &elemento : Tabla_Creacion[tid]){
				guardarPersona(tid, elemento.second);
			}
		};
	}
};
#endif
