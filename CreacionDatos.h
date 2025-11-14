#ifndef CREACIONDATOS_H
#define CREACIONDATOS_H
#include <limits>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <chrono>
#include <unordered_set>
#include <bitset>
#include <omp.h>
#include <list>
#include "Hash.h"
using namespace std;
using namespace std::chrono;


class CreacionDatos{
private:
	//sets de datos para la creacion de personas
	vector<string> Nombres = {"Jaime", "Manuel", "Martin", "Augusto", "Edinson", "Alex", "Cesar", "Ivan", "Daniela", "Marta", "Sandra", "Rosario", "Ariana", "Luisa"};
	vector<string> Apellidos = {"Rodriguez", "Valenzuela", "Gil", "Celeste", "Quispe", "Dominguez", "Torres", "Campo", "Zabaleta", 
		"Barcos", "Caicedo", "Rosales", "Concha", "Flores"};
	vector<string> Nacionalidad = {"Peruana", "Ecuador"};
	vector<string> dominios = {"@gmail.com","@hotmail.com"};
	vector<string> LugarNacimiento = {"Tumbes", "Zarumilla", "San Ignacio", "Jaen", "San Jose", "Santa Rosa", "Sullana", "Piura", "Paita",
		"Catacocha", "Loja"};
	vector<string> EstadoCivil = {"Solter@", "Casad@", "Divorciad@", "Viud@", "Separad@", "Conviviente"};
	vector<string> Departamento = {"Amazonas", "Ancash", "Apurimac", "Arequipa", "Ayacucho", "Cajamarca", "Cusco", "Huancavelica",
		"Huanuco", "Ica", "Junin", "La Libertad", "Lambayeque", "Lima", "Loreto", "Madre de Dios", "Moquegua", "Pasco", "Piura",
		"Puno", "San Martin", "Tacna", "TUmbes", "Ucayali"};
	vector<string> Provincia = {"Trujillo", "Huancayo", "Cusco", "Piura", "Maynas", "Puno", "Arequipa", "Cajamarca", "Chiclayo", "Tacna",
		"Huaraz", "Huanuco", "Ica", "Chachapoyas", "Abancay", "Tumbes", "Moyobamba", "Cerro de Pasco", "Sullana", "Ayacucho"};
	vector<string> Ciudad = {"Talara", "Juliaca", "Huaral", "Pisco", "Tarapoto", "Moquegua", "Sicuani", "Jaen", "Tingo Maria", "Pucallpa",
		"Ilo", "Chulucanas", "Huanta", "Lima", "Huacho", "Chimbote", "Satipo", "Puerto Maldonado", "Chincha Alta", "Andahuaylas", "Yurimaguas",
		"Cajamarca", "Tarma"};
	vector<string> Distrito = {"Castilla", "La Banda de Shilcayo", "La Molina", "Ate", "Cercado de Lima", "El Tambo", "Morales", "Pocollay", "Yarinacocha",
		"Manatay", "Amarilis", "La Victoria", "Surco", "Ate", "Cerro Colorado", "Moche", "Simbal", "Gregorio Albarracin", "Mariano Melgar",
		"Yanahuara", "Sapallanga", "San Miguel", "Banos del Inca"};
	vector<string> Ubicacion = {"Calle 1", "Calle Principal", "Calle 2", "Calle 3", "Calle 4", "Calle 5", "Calle 6", "Calle 7"};
	//set para los telefonos
	static bitset<100000000> usadosTelefono;
	//distribuciones para poder sacar los datos aleatorios de los sets de datos
	uniform_int_distribution<int> distNombre;
	uniform_int_distribution<int> distApellido;
	uniform_int_distribution<int> distNacionalidad;
	uniform_int_distribution<int> distDominio;
	uniform_int_distribution<int> distTelefono;
	uniform_int_distribution<int> distLugar;
	uniform_int_distribution<int> distEstadoCivil;
	uniform_int_distribution<int> distDepartamento;
	uniform_int_distribution<int> distProvincia;
	uniform_int_distribution<int> distCiudad;
	uniform_int_distribution<int> distDistrito;
	uniform_int_distribution<int> distUbicacion;
	
	//funcion para conseguir la primera palabra completa de un char
	string obtenerPrimerToken(const char* s) {
		string str(s);
		size_t pos = str.find(' ');
		if (pos == string::npos) return str;
		return str.substr(0, pos);
	}
	
	//funcion para conseguir los DNI aleatorios, debido a que funciona con rangos debemos enviar el bitset con los parametros para generar el rango
	int GenerarDNI(mt19937 &rng, bitset<(99999999 - 10000000 + 1) / 12> &UsadosDNI, int inicio, int fin) {
		uniform_int_distribution<int> distDNI(inicio, fin);
		int nuevoDNI;
		while (true) {
			nuevoDNI = distDNI(rng);
			int idx = nuevoDNI - inicio;
			
			if (!UsadosDNI[idx]) {
				UsadosDNI[idx] = true;
				break; 
			}
		}
		return nuevoDNI;
	}
	
	//conseguir telefono de forma aleatoria
	int generarTelefono(mt19937 &rng) {
		int tel;
		while (true) {
			tel = distTelefono(rng);
			#pragma omp critical
			{
				if (!usadosTelefono[tel - 900000000]) {
					usadosTelefono[tel - 900000000] = true;
				} else {
					tel = -1; // reintentar
				}
			}
			if (tel != -1) break;
		}
		return tel;
	}
	
	//funcion para generar dos nombres distintso, ambos se escriben en el destino que es el nombre de la persona
	void generarNombres(mt19937 &rng, char* destino, size_t tam) {
		int idx1, idx2;
		do {
			idx1 = distNombre(rng);
			idx2 = distNombre(rng);
		} while (idx1 == idx2);
		
		snprintf(destino, tam, "%s %s",
				 Nombres[idx1].c_str(),
				 Nombres[idx2].c_str());
	}
	
	//funcion para generar dos apellidos distintos, ambos se escriben en el destino que es el apellido de la persona
	void generarApellidos(mt19937 &rng, char* destino, size_t tam) {
		int idx1, idx2;
		do {
			idx1 = distApellido(rng);
			idx2 = distApellido(rng);
		} while (idx1 == idx2);
		
		snprintf(destino, tam, "%s %s",
				 Apellidos[idx1].c_str(),
				 Apellidos[idx2].c_str());
	}
	
	//funcion para la nacionalidad
	void generarNacionalidad(mt19937 &rng, char* destino, size_t tam) {
		snprintf(destino, tam, "%s",
				 Nacionalidad[distNacionalidad(rng)].c_str());
	}
	
	//funcion para el lugar de nacimiento
	void generarLugarNacimiento(mt19937 &rng, char* destino, size_t tam) {
		snprintf(destino, tam, "%s",
				 LugarNacimiento[distLugar(rng)].c_str());
	}
	
	//funcion para el estado civil
	void generarEstadoCivil(mt19937 &rng, char* destino, size_t tam) {
		snprintf(destino, tam, "%s",
				 EstadoCivil[distEstadoCivil(rng)].c_str());
	}
	
	//funcion para la direccion, al ser todos miembros de una misma estructura dentro de persona lo podemos generar en una sola funcion
	void generarDireccion(mt19937 &rng, Lugar &u) {
		snprintf(u.Departamento, sizeof(u.Departamento), "%s",
				 Departamento[distDepartamento(rng)].c_str());
		snprintf(u.Provincia, sizeof(u.Provincia), "%s",
				 Provincia[distProvincia(rng)].c_str());
		snprintf(u.Ciudad, sizeof(u.Ciudad), "%s",
				 Ciudad[distCiudad(rng)].c_str());
		snprintf(u.Distrito, sizeof(u.Distrito), "%s",
				 Distrito[distDistrito(rng)].c_str());
		snprintf(u.Ubicacion, sizeof(u.Ubicacion), "%s",
				 Ubicacion[distUbicacion(rng)].c_str());
	}
	
	// Generar correo: primer nombre + primer apellido mas el dominio
	void generarCorreo(mt19937 &rng, const char* nombres, const char* apellidos, char* destino, size_t tam) {
		string n1 = obtenerPrimerToken(nombres);
		string a1 = obtenerPrimerToken(apellidos);
		snprintf(destino, tam, "%s.%s%s", n1.c_str(), a1.c_str(), dominios[distDominio(rng)].c_str());
	}
	
public:
	//declaramos el constructor con los rangos de las distribuciones
	CreacionDatos()
		: distNombre(0, Nombres.size() - 1),
		distApellido(0, Apellidos.size() - 1),
		distNacionalidad(0, Nacionalidad.size() - 1),
		distDominio(0, dominios.size() - 1),
		distTelefono(900000000, 999999999),
		distLugar(0, LugarNacimiento.size() - 1),
		distEstadoCivil(0, EstadoCivil.size() - 1),
		distDepartamento(0, Departamento.size() - 1),
		distProvincia(0, Provincia.size() - 1),
		distCiudad(0, Ciudad.size() - 1),
		distDistrito(0, Distrito.size() - 1),
		distUbicacion(0, Ubicacion.size() - 1)
	{}
	//funcion principal de la creacion de rangos
	void CrearDatos(TablaHash &listado){
		const int Bloque_DNI = (99999999 - 10000000 + 1) / 12;
		//vector con el numero de rangos que habra
		vector<pair<int,int>> rangos(12);
		//iniciamos la paralelizacion del programa para aumentar la velocidad
		#pragma omp parallel
		{
			int tid = omp_get_thread_num();
			mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count() + tid);
			int inicio = 10000000 + tid * Bloque_DNI;
			int fin = (tid == 12 - 1) ? 99999999 : inicio + Bloque_DNI - 1;
			rangos[tid] = {inicio, fin};
			bitset<Bloque_DNI> usadosDNI;
			#pragma omp for schedule(dynamic)
			/*este pragma se usa para dividir el for entre todos los hilos si no fuese por esto el codigo se ejecutaria por cada hilo
			osea no serian 33 millones de datos, serian 396 millones esa solo es la parte de for, el schedule(dynamic)
			sirve para controlar las iteraciones del for, con esta forma no se hace de forma 100% igual pero eso no nos importa porque lo
			que importa son los rangos del DNI
			*/
			for (int i = 0; i < 33000000; i++) {
				Persona p;
				array<char, 9> arr{};
				int dni, telefono;
				dni = GenerarDNI(rng, usadosDNI, inicio, fin);
				listado.convertirNumeroAChar(dni, p.DNI, sizeof(p.DNI));
				memcpy(arr.data(), p.DNI, 9);
				telefono = generarTelefono(rng);
				listado.convertirNumeroAChar(telefono, p.Telefono, sizeof(p.Telefono));
				generarNombres(rng, p.Nombres, sizeof(p.Nombres));
				generarApellidos(rng, p.Apellidos, sizeof(p.Apellidos));
				generarNacionalidad(rng, p.Nacionalidad, sizeof(p.Nacionalidad));
				generarLugarNacimiento(rng, p.LugarNacimiento, sizeof(p.LugarNacimiento));
				generarDireccion(rng, p.Direccion);
				generarEstadoCivil(rng, p.EstadoCivil, sizeof(p.EstadoCivil));
				generarCorreo(rng, p.Nombres, p.Apellidos, p.Correo, sizeof(p.Correo));
				listado.InsertarCreacion(p);
				listado.guardarPersona(tid,p);
				//funcion para guardados clave dentro del sistema, los guardados claves son los 10 primeros, los del medio y los ultimos
				if(i<10){ 
					listado.guardarEnCacheInicio(p); 
				}else if(i>=(33000000/2)-5 && i < (33000000/2)+5){ 
					listado.guardarEnCacheMedio(p); 
				}else if(i>=33000000-10){ 
					listado.guardarEnCacheFinal(p); 
				}
			}
		}
	}
};
#endif
