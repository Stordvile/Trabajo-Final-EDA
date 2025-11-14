#ifndef TRABAJOPROGRAMA_H
#define TRABAJOPROGRAMA_H
#include <cstring>
class TrabajoPrograma{
public:
	void UsoDelPrograma(TablaHash &listado){
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
				listado.ActualizacionDatos();
				break;
			case 1:{
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
				int indice = listado.calcularIndiceHash(DNI_Busqueda);
				listado.cargarDatos(indice);
				Persona* p = listado.buscar(DNI_Busqueda);
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
				listado.mostrarValoresClave();
				break;
			}
			case 3:{
				listado.agregarPersona();
				break;
			}
			case 4:{
				listado.eliminarPersona();
				break;
			}
			case 5:{
				listado.MostrarInserciones();
				break;
			}
			case 6:{
				listado.MostrarEliminaciones();
				break;
			}
			case 7:{
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
