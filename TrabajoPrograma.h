#ifndef TRABAJOPROGRAMA_H
#define TRABAJOPROGRAMA_H
#include <cstring>
class TrabajoPrograma{
public:
	void UsoDelPrograma(TablaHash &listado){
		//menu sencillo en consola
		int opcion;
		do{
			cout<<"[0] Salir del sistema"<<endl;
			cout<<"[1] Buscar persona por DNI"<<endl;
			cout<<"[2] Mostrar valores claves"<<endl;
			cout<<"[3] Anadir Persona"<<endl;
			cout<<"[4] Eliminar Persona"<<endl;
			cout<<"[5] Mostrar todas las inserciones"<<endl;
			cout<<"[6] Mostrar todas las eliminaciones"<<endl;
			cout<<"[7] Forzar Actualizacion de Datos"<<endl;
			cout << "Seleccione una opcion: ";
			cin.clear();
			cin>>opcion;
			cin.ignore(1, '\n');
			switch(opcion){
			case 0:
				//fin de ejecucion del programa
				listado.ActualizacionDatos();
				break;
			case 1:{
				//nos aseguramos de que DNI tenga si o si 8 digitos
				char DNI_Busqueda[9];
				string input;
				do {
					cout << "Ingrese el DNI (8 digitos): ";
					cin >> input;
					
					if (input.length() != 8) {
						cout << "El DNI debe tener exactamente 8 digitos"<<endl;
					}
				} while (input.length() != 8);
				
				strncpy(DNI_Busqueda, input.c_str(), 8);
				DNI_Busqueda[8] = '\0';
				//calculamos el indice para saber en que archivo debemos empezar a buscar
				int indice = listado.calcularIndiceHash(DNI_Busqueda);
				listado.cargarDatos(indice);
				Persona* p = listado.buscar(DNI_Busqueda);
				//si no existe la Persona entonces se imprime que no existe
				if(!p){
					cout<<"Persona no encontrada"<<endl;
				}else{
					cout<<"Persona encontrada"<<endl;
					listado.MostrarPersona(*p);
				}
				listado.vaciarHash();
				break;
			}
			case 2:{
				//llamada a Mostrar Valores
				listado.mostrarValoresClave();
				break;
			}
			case 3:{
				//llamada a agregar persona
				listado.agregarPersona();
				break;
			}
			case 4:{
				//llamada a Eliminar Persona
				listado.eliminarPersona();
				break;
			}
			case 5:{
				//lamada a Mostrar Inserciones
				listado.MostrarInserciones();
				break;
			}
			case 6:{
				//lamada a Mostrar Eliminaciones
				listado.MostrarEliminaciones();
				break;
			}
			case 7:{
				//Funcion para forzar la actualizacion de datos
				listado.ActualizacionDatos();
				break;
			}
			default:
				cout<<"Opcion no valida"<<endl;
				break;
			}
		} while(opcion != 0);
	}
};
#endif
