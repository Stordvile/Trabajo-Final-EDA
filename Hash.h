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

struct Lugar{
	char Departamento[13];
	char Provincia[11] ;
	char Ciudad[16];
	char Distrito[20];
	char Ubicacion[15];
};
//prueba vector o lista
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

const int TAM_TABLA = 12;

class TablaHash{
private:
	list<pair<char[9], Persona>> Tabla_Creacion[TAM_TABLA];
	list<pair<char[9], Persona>> Tabla_Insercion[TAM_TABLA];
	list<pair<char[9], Persona>> Tabla_Eliminacion[TAM_TABLA];
	vector<Persona> cacheInicio;
	vector<Persona> cacheMedio;
	vector<Persona> cacheFinal;
	

public:
	void convertirNumeroAChar(int numero, char* destino, size_t tam) {
		for (int i = tam - 2; i >= 0; --i) {
			destino[i] = '0' + (numero % 10);
			numero /= 10;
		}
		destino[tam - 1] = '\0';
	}
	
	void vaciarHash() {
		for (auto &lista : Tabla_Creacion) { 
			lista.clear();
		}
	}
	
	int calcularIndiceHash(const char dni[9]) {
		int numero = atoi(dni);
		const int Bloque_DNI = (99999999 - 10000000 + 1) / 12;
		return (numero - 10000000) / Bloque_DNI;
	}
	
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
	
	Persona* buscar(const char dni[9]) {
		int indice = calcularIndiceHash(dni);
		
		for (auto &elemento : Tabla_Creacion[indice]) {
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

		for (auto &elemento : Tabla_Insercion[indice]) {
			if (strcmp(elemento.first, dni) == 0)
				return &elemento.second;
		}
		
		return nullptr;
	}
	
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
		
		for(const auto& buffer : buffers){
			for(const auto& elem : buffer){
				InsertarCreacion(elem);
			}
		}
		cout<<"Se han cargado "<<tamRegistros<<" de "<<archivos[indice]<<endl;
	}
		
	void ActualizacionDatos(){
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
		#pragma omp parallel
		{
			int tid = omp_get_thread_num();
			FILE* archivo = fopen(archivos[tid].c_str(), "rb");
			Persona p;
			Tabla_Creacion[tid].clear();
			while(!Tabla_Eliminacion[tid].empty()){
				while(fread(&p, sizeof(Persona), 1, archivo)==1){
					bool estaEliminado = false;
					for (auto &elemento_eliminado : Tabla_Eliminacion[tid]) {
						if (strcmp(elemento_eliminado.first, p.DNI) == 0) {
							Tabla_Eliminacion[tid].remove_if([&p](const pair<char[9], Persona>& elem) {
								return strcmp(elem.first, p.DNI) == 0;
							});
							estaEliminado = true;
							break;
						}
					}
					if (!estaEliminado) {
						InsertarCreacion(p);
					}
				}
				fclose(archivo);
				fs::remove(archivos[tid].c_str());
			}
			if(!Tabla_Insercion[tid].empty()){
				for(auto &elemento_insertado : Tabla_Insercion[tid]){
					InsertarCreacion(elemento_insertado.second);
				}
			}
			for(auto &elemento : Tabla_Creacion[tid]){
				guardarPersona(tid, elemento.second);
			}
		};
	}
};
#endif
