//---------------------------------------------------------------------------------BIBLIOTECAS---------------------------------------------------------------------
#include <stdio.h>
#include <vector>
#include <string>
#include <stdlib.h>     
#include <time.h> 
#include <ctime> 
#include <iostream>
#include <fstream> 
#include <algorithm>
#include <random>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <cctype>
#include <regex>
#include <sstream>
#include <cmath>
#include <chrono>
#include "xlsxwriter.h"

using namespace std;
//----------------------------------------------------------------------------------FUNCIONES------------------------------
bool randomPerteneceAdigitosRandom(int& random,vector<int> digitosRandom){
	bool flag = false;
	for (int i = 0; i < digitosRandom.size(); ++i){
		if(random == digitosRandom.at(i)){
			flag = true;
			break;
		}else{
			flag = false;
		}	
	}
	return flag;
}
int escogoRandom(int& min,int& max, vector<int>& digitosRandom){
	int digitoAleatorio;
	bool fstop = true;
	do{
		digitoAleatorio = min + rand()%(max-min+1);
		fstop = randomPerteneceAdigitosRandom(digitoAleatorio,digitosRandom);
	}while(fstop);
    return digitoAleatorio;
}
void orden_aleatorio(vector<string>& vector) {
    random_device rd;
    mt19937 g(rd());
    shuffle(vector.begin(), vector.end(), g); //Funcion que desordena un vector con la planificacion diaria
}
vector<int> definirDigitosSiguientes(vector<int>& vectorDiaAnterior, int tam) {
    vector<int> vec1;

    // Si el vectorDiaAnterior está vacío, comienza desde 0. 
    // De lo contrario, comienza desde el número que sigue al último número en vectorDiaAnterior.
    int start = vectorDiaAnterior.empty() ? 0 : (vectorDiaAnterior.back() + 1) % 10;
    
    while(vec1.size() < tam) {
        vec1.push_back(start);
        start = (start + 1) % 10;  // Asegura que los números estén en el rango de 0 a 9.
    }

    /*for(int i = 0; i < vec1.size(); ++i){
        cout<<vec1.at(i)<<" ";
    }
    cout<<endl;*/

    return vec1;
}
vector<string> obtenerListado( vector<int>& digitos,vector<string>& patentes){ 
	unordered_set<string> result_set; // usamos unordered_set para chequear duplicados
    vector<string> result; // vector para devolver el resultado
    
    for (const auto& patente : patentes) {
        if (patente.size() >= 1) {
            string ultimoDigito = patente.substr(patente.size() - 1);
            for (const auto& digito : digitos) {
                if (ultimoDigito.find(to_string(digito)) != string::npos) {
                    result_set.insert(patente); // insertamos en el set
                    break;
                }
            }
        }
    }
    
    // convertimos el set a un vector para devolver el resultado
    for (const auto& patente : result_set) {
        result.push_back(patente);
    }
    
    return result;
}
unordered_map<string, int> iniciarMapa(vector<string>& patentes) {
    unordered_map<string, int> mapa;
    for (int i = 0; i < patentes.size(); i++) {
    	string patente = patentes[i];
    	mapa[patente] = 0;
	}
    return mapa;
}
void contarPatentes(vector<string>& lista, unordered_map<string, int>& mapa) {
    for (string patente : lista) {
        if (mapa.count(patente)) {
            mapa[patente]++;
        }
    } // Función para aumentar el valor en 1 por cada patente encontrada en otra lista
}
vector<int> leerArchivoDigitos(string nombreArchivo) {
    vector<int> digitos;
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            for (char c : linea) {
                if (c >= 48 && c <= 57) {
                    digitos.push_back(c - 48);
                }
            }
        }
        archivo.close();
    } else {
        cout << "Error al abrir el archivo" << endl;
    }
    return digitos; //lee solo los caracteres que sean numeros 
}
vector<string> leerFolios(string nombreArchivo) {
   vector<string> resultado;
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            string digitos;
            for (char c : linea) {
                if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90)) {
                    digitos.push_back(c);
                }
            }
            resultado.push_back(digitos);
        }
        archivo.close();
    }
    return resultado;
}
vector<string> leerArchivoPatentes(string nombreArchivo) {
    vector<string> resultado;
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            string digitos;
            for (char c : linea) {
                if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90)) {
                    digitos.push_back(c);
                }
            }
            resultado.push_back(digitos);
        }
        archivo.close();
    }
    return resultado;
}
double calcularVarianza(unordered_map<string, int>& myMap) {
    int sum = 0;
    int count = 0;
    double mean = 0.0;
    double variance = 0.0;

    // Compute the mean
    for (auto it = myMap.begin(); it != myMap.end(); it++) {
        sum += it->second;
        count++;
    }
    mean = (double)sum / count;

    // Compute the variance
    for (auto it = myMap.begin(); it != myMap.end(); it++) {
        variance += pow((double)it->second - mean, 2);
    }
    variance /= count;

    return variance;
}
void guardar_map_en_archivo(unordered_map<string, int>& mapa, const string& archivo, const string& archivo2) {
    lxw_workbook* workbook = workbook_new("1-Estadisticas.xlsx");
	if (!workbook) {
	    cout << "No se pudo crear el archivo de salida." << endl;
	}
	// Crear hoja de Excel
	lxw_worksheet* worksheet = workbook_add_worksheet(workbook, NULL);
    ofstream ofs(archivo);
    ofstream ofs2(archivo2);
    int fila =0;
    if (ofs.is_open()) {
        for (const auto& par : mapa) {
         	ofs<< par.first << "\n";
        }
        ofs.close();
        //cout << "Estadistica de buses guardada en  " << archivo << endl;
    } else {
        cerr << "No se pudo abrir el archivo " << archivo << endl;
    }
    string nombre;
    ifstream inFile3(archivo);
    if (!inFile3) {
        cout << "No se pudo abrir el archivo de entrada." << endl;      
    }
	while (getline(inFile3, nombre)) {
	    worksheet_write_string(worksheet, fila, 0, nombre.c_str(), NULL);
   		fila++;
    }	   
    fila =0;
	// Cerrar archivos y liberar memoria
	inFile3.close();
    

    if (ofs2.is_open()) {
        for (const auto& par : mapa) {
            ofs2 << par.second << "\n";
        }
    
        ofs2.close();
        //cout << "Estadistica de buses guardada en  " << archivo2 << endl;
    } else {
        cerr << "No se pudo abrir el archivo " << archivo2 << endl;
    }
    ifstream inFile2(archivo2);
    if (!inFile2) {
        cout << "No se pudo abrir el archivo de entrada." << endl;      
    }
	while (getline(inFile2, nombre)) {
	    worksheet_write_string(worksheet, fila, 1, nombre.c_str(), NULL);
   		fila++;
    }	   
    fila = 0;
	// Cerrar archivos y liberar memoria
	inFile2.close();
	string col = "Varianza";
	double varianza = calcularVarianza(mapa);
	string str = to_string(varianza);
	worksheet_write_string(worksheet, 0, 3, col.c_str(), NULL);
	worksheet_write_string(worksheet, 1, 3, str.c_str(), NULL);
	workbook_close(workbook);
	cout<<"Estadisticas de planificacion guardada en 1-Estadisticas.xlsx"<<endl;
}
void guardarDias( string& nombreArchivo,vector<string>& datos){
	ofstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        for (auto dato : datos) {
            archivo << dato << endl;
        }
        archivo.close();
        //cout << "Vector guardado en archivo exitosamente." << endl;
    }
    else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}
void imprimirUnorderedMap(unordered_map<string, int>& mapa) {
    for (auto it = mapa.begin(); it != mapa.end(); ++it) {
        cout << "Llave: " << it->first << ", Valor: " << it->second << endl;
    }
}
void imprimirEntrada(vector<int>& cantidadDeDigitosXDia,vector<string>& dias,int cantDias){
    int count = 0;
    for (int i = 0; i < cantDias; ++i){
    	string aux = "Dia: ";
		int numero = i;
    	stringstream ss;
    	ss << numero;
    	string numero_str = ss.str();
    	string name = aux + numero_str;
    	dias.push_back(name);
    }
    /*cout<<"Cantidad de digitos por dia"<<endl;
    for (int i = 0; i < cantDias; i++) {
    	cout << dias.at(i) <<": ";
        cout << cantidadDeDigitosXDia.at(count) << " ";
        cout << endl;
        count++;
        if (count>6){
        	count = 0;
        }
    }*/
    cout<<endl;
    cout<<"Entrada leida correctamente"<<endl;
    cout<<endl;
}
void imprimirSorteo(vector<vector<int>>& digitosXDiasSorteados){
	cout<<"Sorteo obtenido"<<endl;
	for (int i = 0; i < digitosXDiasSorteados.size(); ++i){
		//cout<<dias.at(i)<<": ";
		for (int j = 0; j < digitosXDiasSorteados.at(i).size(); ++j){
			cout<<digitosXDiasSorteados.at(i).at(j)<<" ";
		}
		cout<<endl;
	}
}
vector<vector<string>> cambiarFoliosANombres(vector<vector<string>> folios) {
    // Definimos los valores a reemplazar
    string fSot = "500766";
    string fAlce = "500768";
    string fExprc = "500767";
    string fPlaya = "500770";
    
    string sot = "Sotral";
    string alce = "Alce";
    string exprc = "ExpDelC";
    string playa = "PlayaBlanca";
    
    // Recorremos la matriz de folios
    for (int i = 0; i < folios.size(); i++) {
        for (int j = 0; j < folios[i].size(); j++) {
            // Reemplazamos los valores correspondientes
            if (folios[i][j] == fSot) {
                folios[i][j] = sot;
            } else if (folios[i][j] == fAlce) {
                folios[i][j] = alce;
            } else if (folios[i][j] == fExprc) {
                folios[i][j] = exprc;
            } else if (folios[i][j] == fPlaya) {
                folios[i][j] = playa;
            }
        }
    }
    
    // Devolvemos la matriz modificada
    return folios;
}
void guardarPlanificacionObtenidaEnArchivo(int& cantDias, vector<vector<string>>& planificacionObtenida,vector<vector<string>>&  planificacionObtenidaConFolios,vector<vector<string>>& planificacionObtenidaConNinternos,vector<vector<int>>& digitosXDiasSorteados,int dia,int mes,int ano){
	vector<string> nombreArchivos;
	for (int i = 0; i < cantDias; ++i){
		string aux = "Dia_";
		string aux2 = ".txt";
		int numero = i;
    	stringstream ss;
    	ss << numero;
    	string numero_str = ss.str();
    	string name = aux + numero_str;
		name = name + aux2;
		nombreArchivos.push_back(name);
	}
	for (int i = 0; i < planificacionObtenida.size(); ++i){
		guardarDias(nombreArchivos.at(i),planificacionObtenida.at(i));
	}
	vector<string> nombreArchivos5;
	for (int i = 0; i < cantDias; ++i){
		string aux = "numero_";
		string aux2 = ".txt";
		int numero = i;
    	stringstream ss;
    	ss << numero;
    	string numero_str = ss.str();
    	string name = aux + numero_str;
		name = name + aux2;
		nombreArchivos5.push_back(name);
	}
	for (int i = 0; i < planificacionObtenidaConNinternos.size(); ++i){
		guardarDias(nombreArchivos5.at(i),planificacionObtenidaConNinternos.at(i));
	}
	cout<<"Planificacion guardada en archivo excel con nombre: 1-Planificacion.xlsx"<<endl;
	vector<vector<string>> nombresFolios = cambiarFoliosANombres(planificacionObtenidaConFolios);
	vector<string> nombreArchivos2;
	for (int i = 0; i < cantDias; ++i){
		string aux = "FoliosDia_";
		string aux2 = ".txt";
		int numero = i;
    	stringstream ss;
    	ss << numero;
    	string numero_str = ss.str();
    	string name = aux + numero_str;
    	name = name + aux2;
		nombreArchivos2.push_back(name);
	}
	for (int i = 0; i < nombresFolios.size(); ++i){
		guardarDias(nombreArchivos2.at(i),nombresFolios.at(i));	
	}
 	// Crear archivo de Excel
	lxw_workbook* workbook = workbook_new("1-Planificacion.xlsx");
	if (!workbook) {
	    cout << "No se pudo crear el archivo de salida." << endl;
	}
	// Crear hoja de Excel
	lxw_worksheet* worksheet = workbook_add_worksheet(workbook, NULL);
	int count = 0;
	int count2 = 1;
	int count3 = 2;
	int count4 = 3;
	tm fecha;
    fecha.tm_year = ano - 1900; // Años desde 1900, por lo que 123 corresponde a 2023
    fecha.tm_mon = mes-1;    // Abril, pero empezando desde 0
    fecha.tm_mday = dia;  // Día del mes
    fecha.tm_hour = 0;    // Asegurándose de que la hora sea 0, para evitar interferencias.
    fecha.tm_min = 0;     // Asegurándose de que los minutos sean 0, para evitar interferencias.
    fecha.tm_sec = 0;     // Asegurándose de que los segundos sean 0, para evitar interferencias.
    fecha.tm_isdst = -1;  // Dejar que mktime detecte si el horario de verano está en vigor.

	for (int i = 0; i < cantDias; ++i){
		string col1 = "Patentes";
		string col2 = "Linea";	
		string col3 = "nºInterno";	
		string col4 = "Hora";		
		string aux3 = "FoliosDia_";
		string aux = "Dia_";
		string aux6 = "numero_";
		string aux2 = ".txt";
		int numero = i;
		int fila = 0;
    	stringstream ss;
    	ss << numero;
    	string numero_str = ss.str();
    	char buffer[80];
   	 	strftime(buffer, 80, "%d/%m/%Y", &fecha); // Convertir la estructura tm a una cadena de caracteres
    	string fecha_string(buffer);
    	worksheet_write_string(worksheet, fila, i+count, numero_str.c_str(), NULL);
    	worksheet_write_string(worksheet, fila, i+count+3, fecha_string.c_str(), NULL);
    	fecha.tm_mday += 1;
    	 mktime(&fecha); 
    	string name = aux + numero_str;
    	string name2 = aux3 + numero_str;
    	string name6 = aux6 + numero_str;
		name = name + aux2;
		name2 = name2 + aux2;
		name6 = name6 + aux2;
		
		worksheet_write_string(worksheet, 1+fila, i+count, col1.c_str(), NULL);
		worksheet_write_string(worksheet, 1+fila, 1+i+count, col2.c_str(), NULL);
		worksheet_write_string(worksheet, 1+fila, 2+i+count, col3.c_str(), NULL);
		worksheet_write_string(worksheet, 1+fila, 3+i+count, col4.c_str(), NULL);


		ifstream inFile(name);
	    if (!inFile) {
	        cout << "No se pudo abrir el archivo de entrada." << endl;
	      
	    }
	    // Leer nombres de archivo de texto y escribir en hoja de Excel
	    string nombre;
	    
	    while (getline(inFile, nombre)) {
	        worksheet_write_string(worksheet, 2+fila, i+count, nombre.c_str(), NULL);
	        fila++;
	    }
	    count = count + 4;
	    // Cerrar archivos y liberar memoria
	    inFile.close();
	    ifstream inFile2(name2);
	    if (!inFile2) {
	        cout << "No se pudo abrir el archivo de entrada." << endl;
	      
	    }
	    // Leer nombres de archivo de texto y escribir en hoja de Excel
	    fila = 0;
	    while (getline(inFile2, nombre)) {
	        worksheet_write_string(worksheet, 2+fila, i+count2, nombre.c_str(), NULL);
	        fila++;
	    }
	    count2 = count2 + 4;
	    inFile2.close();
	    ifstream inFile6(name6);
	    if (!inFile6) {
	        cout << "No se pudo abrir el archivo de entrada." << endl;
	    }
	    fila = 0;
 		vector<string> horarios;
		tm horario = {};
		horario.tm_hour = 5;
		horario.tm_min = 0;
		horario.tm_sec = 0;
		time_t tiempo = mktime(&horario); // Convierte la estructura "tm" en un valor de tiempo "time_t"
		tm* horario_actual = localtime(&tiempo); // Convierte el valor de tiempo "time_t" en una estructura "tm"
		
		strftime(buffer, sizeof(buffer), "%H:%M:%S", horario_actual); // Formatea la hora en "HH:MM:SS"
		int flag = 0;
		bool flag2 = true;
		bool flagNum6 = true;
		int countNum6 = 0;
		bool flagNum5 = false;
		int countNum5 = 0;
		bool flagNum4 = false;
		int countNum4 = 0;
	    while (getline(inFile6, nombre)) {
	        worksheet_write_string(worksheet, 2+fila, i+count3, nombre.c_str(), NULL);
	        /*if (horario_actual->tm_hour == 12 && horario_actual->tm_min == 0) {
		        horario_actual->tm_hour = 7;
		        horario_actual->tm_min = 5;
		        flag2 = false;
		    }
	        if(flag2) horario_actual->tm_min += 5; // Sumarle 5 minutos al horario actual
			else horario_actual->tm_min += 15;*/
	        if(i%6 == 0 && i!=0){
	        		if(flagNum6){
		        	 horario_actual->tm_min += 10;
		        	 countNum6++;
		        	 if(countNum6 == 6){
		        	 	flagNum5 = true;
		        	 	flagNum6 = false;
		        	 }
		        }
		        else {
		        	if(flagNum5){
		        	 horario_actual->tm_min += 7;
		        	 countNum5++;
		        	 	if(countNum5 == 10){
		        	 	flagNum4 = true;
		        	 	flagNum5 = false;
		        		 }
		        	}
		        	else horario_actual->tm_min += 5;
		    	}
	        }else{
		        	if(flagNum6){
		        	 horario_actual->tm_min += 6;
		        	 countNum6++;
		        	 if(countNum6 == 5){
		        	 	flagNum5 = true;
		        	 	flagNum6 = false;
		        	 }
		        }
		        else {
		        	if(flagNum5){
		        	 horario_actual->tm_min += 5;
		        	 countNum5++;
		        	 	if(countNum5 == 6){
		        	 	flagNum4 = true;
		        	 	flagNum5 = false;
		        		 }
		        	}
		        	else horario_actual->tm_min += 4;
		    	}
	        }
	   
			time_t tiempo_actualizado = mktime(horario_actual);
			strftime(buffer, sizeof(buffer), "%H:%M:%S", horario_actual); // Formatea la hora en "HH:MM:SS"
			horarios.push_back(buffer);
			//cout << buffer << endl; 
	        worksheet_write_string(worksheet, 2+fila, i+count4, horarios.at(flag).c_str(), NULL);
	        flag++;
	        fila++;
	    }
	    count3 = count3 + 4;
		count4 = count4 + 4;
		inFile6.close();
	}
	string nombre;
	vector<string> sorteo;
	for (int i = 0; i < digitosXDiasSorteados.size(); ++i){
		int second = 0;
		string key;
		for (int j = 0; j < digitosXDiasSorteados.at(i).size(); ++j){
			key = key +to_string(digitosXDiasSorteados.at(i).at(j));	
		}
		sorteo.push_back(key);
	}
	string archivoSorteo = "Sorteo.txt";
	ofstream archivoS(archivoSorteo);
	if (archivoS.is_open()) {
	    for (const auto& str : sorteo) {
	        archivoS << str << endl;
	    }
        archivoS.close();
    }
    ifstream inFile3(archivoSorteo);
    if (!inFile3) {
        cout << "No se pudo abrir el archivo de entrada." << endl;      
    }
	int var =0;
	while (getline(inFile3, nombre)) {
	    worksheet_write_string(worksheet, 0, 1+var, nombre.c_str(), NULL);
	 	var = var + 5;     
    }	   
	// Cerrar archivos y liberar memoria
	inFile3.close();
	vector<int> cantBusesSorteo;
	for (int i = 0; i < cantDias; ++i){
		cantBusesSorteo.push_back(planificacionObtenida.at(i).size());
	}
	string archivoFrecSort = "FrecuenciaSorteo.txt";
	ofstream archivoFS(archivoFrecSort);
    if (archivoFS.is_open()) {
        for (const auto& entero : cantBusesSorteo) {
            archivoFS << entero <<endl;
        }
        archivoFS.close();
    }
    ifstream inFile4(archivoFrecSort);
    if (!inFile4) {
        cout << "No se pudo abrir el archivo de entrada." << endl;      
    }
	var =0;
	while (getline(inFile4, nombre)) {
	    worksheet_write_string(worksheet, 0, 2+var,nombre.c_str(), NULL);
	 	var = var + 5;     
    }	   
	// Cerrar archivos y liberar memoria
	inFile4.close();
	workbook_close(workbook);
}
unordered_map<string, string> relacionar_patentes_con_folios(const vector<string>& folios, const vector<string>& patentes) {
  unordered_map<string, string> patentes_con_folios;
  for (int i = 0; i < patentes.size(); i++) {
    patentes_con_folios[patentes[i]] = folios[i];
  }
  return patentes_con_folios;
}
string encontrarFolio(string patenteBuscada, unordered_map<string,string> patentes) {
     auto iter = patentes.find(patenteBuscada);
    if (iter != patentes.end()) {
        return iter->second;
    }
    else {
        return "Patente no encontrada";
    }
}
unordered_map<string, vector<string>>  separarPatentesPorFolios(unordered_map<string, string> patentes_con_folios) {
    unordered_map<string, vector<string>> folios_con_patentes;
    
    // Agrupar las patentes por folios
    for (auto const& patente : patentes_con_folios) {
        string folio = patente.second;
        string patente_num = patente.first;
        
        folios_con_patentes[folio].push_back(patente_num);
    }
    
    
    
    return folios_con_patentes;
}
vector<string> obtenerNombresExcels(unordered_map<string, vector<string>> folios_con_patentes) {
    vector<string> nombres_excels;
    
    for (auto const& folio : folios_con_patentes) {
        string nombre_excel = folio.first;
        
        nombres_excels.push_back(nombre_excel);
    }
    
    return nombres_excels;
}
void cambiarFoliosANombreLinea(vector<string>& patentes) {
    unordered_map<string, string> folios_a_nombres = {
        {"500766", "Sotral"},
        {"500768", "Alce"},
        {"500767", "ExpDelCarbon"},
        {"500770", "PlayaBlanca"}
    };
    
    for (auto& patente : patentes) {
        string folio = patente.substr(0, 6);
        
        if (folios_a_nombres.find(folio) != folios_a_nombres.end()) {
            string nombre = folios_a_nombres[folio];
            patente.replace(0, 6, nombre);
        }
    }
}
//--------------------------------------------------------INICIO PROGRAMA---------------------------------------------------
int main() { //-lxlsxwriter
	//-------------------------------------------------INICIALIZACION DE VARIABLES------------------------------------------
	int min = 0;
	int max = 9;
	int cantDias; // cantidad de dias a planificar
	vector<string> patentes;
	vector<int> fechadiayear;
	vector<int> cantidadDeDigitosXDia;
	vector<string> folios;
	vector<string> nMaquinas;
	vector<string> horarios;
	vector<vector<string>> planificacionObtenida;
	vector<vector<string>> planificacionObtenidaConFolios;
	vector<vector<string>> planificacionObtenidaConNinternos;
	vector<vector<string>> horasPlanificacion;
	vector<vector<string>> PatentesXDigito;
   	vector<int> frecuenciaBusesXDigito; //cantidad de buses por digito del total existentes {150,124, ... , 100}
    vector<int> digitosPrimerDia;                                    
    vector<int> digitosRandom; //digitos dia inicial
	vector<int> digitosAnteriores;
    vector<int> digitosNuevoDia;
    vector<string> planificacionDia; 
    vector<string> planificacionDiaConFolio; 
    vector<string> planificacionDiaConNinterno; 
    vector<string> dias;
	vector<vector<int>> digitosXDiasSorteados; //lunes a domingo {1-2-3-4-5-6-7, 8-9-0-2-4-5, ... , 1,2,3,4,5} son 7
	vector<vector<int>> unDigitoXDia;
	unordered_map<string, int> mapaPatentes;
	unordered_map<string, int> mapaFrecXdigitoPat;
	unordered_map<string, string> mapaFolios;
	unordered_map<string, string> mapanMaquinas;
	int random; //numero aleatorio entre 0 y 9 por los numeros en las patentes
    //-------------------------------------------------INICIO DE ALGORITMO---------------------------------------------------
    //digitosPrimerDia.push_back(6);
    //digitosPrimerDia.push_back(7);
    //digitosPrimerDia.push_back(8);
    //digitosPrimerDia.push_back(9);
    //digitosPrimerDia.push_back(0);
	 int numero;
    vector<int> numeros;

    // Pedir al usuario que ingrese un número entre 1 y 30
    // Pedir número entre 1 y 30
    do {
        cout << "Ingresa un número entre 1 y 30 correspondiente a los dias a simular: ";
        cin >> numero;

        if (cin.fail()) { // Si el usuario ingresa un valor no numérico
            cin.clear(); // Limpiar el estado de error de cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar todo el input hasta el siguiente salto de línea
            cout << "Error: ingresa un número válido." << endl;
        }
        else if (numero < 1 || numero > 30) { // Si el usuario ingresa un número fuera del rango
            cout << "Error: el número debe estar entre 1 y 30." << endl;
        }
    } while (numero < 1 || numero > 30 || cin.fail());

    // Llenar el vector con números del 0 al 9 sin repetir
    int contador = 0;
    int digult = 0;
    int cantUltDia = 0;
    do {
        cout << "Ingrese la cantidad de digitos del ultimo dia: ";
        cin >> cantUltDia;

        if (cin.fail()) { // Si el usuario ingresa un valor no numérico
            cin.clear(); // Limpiar el estado de error de cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar todo el input hasta el siguiente salto de línea
            cout << "Error: ingresa un número válido." << endl;
        }
        else if (cantUltDia < 1 || cantUltDia > 9) { // Si el usuario ingresa un número fuera del rango
            cout << "Error: el número debe estar entre 1 y 9." << endl;
        }
    } while (cantUltDia < 1 || cantUltDia > 9 || cin.fail());
    
    cout<<endl;
    cout<<"Ingrese los numeros del 0 al 9 correspondiente a los digitos del ultimo dia: "<<endl;
    
    while (contador < cantUltDia) {
        int numero_ingresado;
        cout << "Ingrese Digito"<<contador+1<<": ";
        cin >> numero_ingresado;
        cout<<endl;
        if (cin.fail()) { // Si el usuario ingresa un valor no numérico
            cin.clear(); // Limpiar el estado de error de cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar todo el input hasta el siguiente salto de línea
            cout << "Error: ingresa un número válido." << endl;
        }else if (numero_ingresado < 0 || numero_ingresado > 9) { // Si el usuario ingresa un número fuera del rango
            cout << "Error: el número debe estar entre 0 y 9." << endl;
        }
        else if (find(numeros.begin(), numeros.end(), numero_ingresado) != numeros.end()) {
            cout << "El numero ya fue ingresado. Por favor, ingrese otro.\n";
        } else {
            numeros.push_back(numero_ingresado);
            contador++;
        }
    }
    cout<<endl;
    int dia, mes, ano;
    do {
        cout << "Ingresa un día entre 1 y 31: ";
        cin >> dia;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: ingresa un número válido." << endl;
        }
        else if (dia < 1 || dia > 31) {
            cout << "Error: el día debe estar entre 1 y 31." << endl;
        }
    } while (dia < 1 || dia > 31 || cin.fail());

    do {
        cout << "Ingresa un mes entre 1 y 12: ";
        cin >> mes;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: ingresa un número válido." << endl;
        }
        else if (mes < 1 || mes > 12) {
            cout << "Error: el mes debe estar entre 1 y 12." << endl;
        }
    } while (mes < 1 || mes > 12 || cin.fail());
     do {
        cout << "Ingresa un año mayor o igual a 1900: ";
        cin >> ano;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: ingresa un número válido." << endl;
        }
        else if (ano < 1900) {
            cout << "Error: el año debe ser mayor o igual a 1900." << endl;
        }
    } while (ano < 1900 || cin.fail());
    // Imprimir los números ingresados
    
    cout << "El usuario SIMULARA " << numero << " dias y el ultimo dia hubo los siguientes numeros:\n";
    for (auto const& numero : numeros) {
        cout << numero << " ";
    }
    digitosPrimerDia = numeros;
    cantDias = numero;
  

    srand (time(NULL));
    patentes = leerArchivoPatentes("0-patentes.txt");
    nMaquinas = leerFolios("0-nMaquinas.txt");
    cantidadDeDigitosXDia = leerArchivoDigitos("0-digitosDia.txt");
    folios = leerFolios("0-folios.txt");
    mapaFolios = relacionar_patentes_con_folios(folios,patentes);
    mapanMaquinas = relacionar_patentes_con_folios(nMaquinas,patentes);
    mapaPatentes = iniciarMapa(patentes);
    /*for (int i = 0; i < cantidadDeDigitosXDia.at(0); ++i){   //o definir digitos primer dia 
		random = escogoRandom(min, max,digitosRandom);
		digitosRandom.push_back(random); //guardo un digito en un vector con mi semilla completamente aleatoria para el lunes
	}*/
	for (int i = 0; i < 10; ++i){
		vector<int> vector;
		vector.push_back(i);
		unDigitoXDia.push_back(vector);
	}
   	imprimirEntrada(cantidadDeDigitosXDia,dias,cantDias);
   	digitosAnteriores = definirDigitosSiguientes(digitosPrimerDia,cantidadDeDigitosXDia.at(0)); // ingresar dias anteriores a un vector
	digitosNuevoDia = digitosAnteriores; //los anteriore0s que no salieron sorteados
	int x = cantidadDeDigitosXDia.at(0);
	int y = digitosAnteriores.size();
	for (int j = 0 ; j < x - y ; ++j){ //completar sorteo del dia 
		int aux = escogoRandom(min,max,digitosNuevoDia);
		digitosNuevoDia.push_back(aux);
	}
	digitosXDiasSorteados.push_back(digitosNuevoDia); 
	planificacionDia = obtenerListado(digitosXDiasSorteados.at(0),patentes);
	orden_aleatorio(planificacionDia);
	contarPatentes(planificacionDia,mapaPatentes);
	for (int i = 0; i < planificacionDia.size(); ++i){
		string folio;
		string nInterno;
		nInterno = encontrarFolio(planificacionDia.at(i),mapanMaquinas);
		folio = encontrarFolio(planificacionDia.at(i),mapaFolios);
		planificacionDiaConFolio.push_back(folio);
		planificacionDiaConNinterno.push_back(nInterno);
	}
	planificacionObtenidaConFolios.push_back(planificacionDiaConFolio);
	planificacionObtenidaConNinternos.push_back(planificacionDiaConNinterno);
	planificacionObtenida.push_back(planificacionDia);
	planificacionDiaConNinterno.clear();
	planificacionDiaConFolio.clear();
	contarPatentes(planificacionDia,mapaPatentes);
	planificacionDia.clear();
	digitosAnteriores.clear();
	digitosNuevoDia.clear();
	int lim = 1;
	for (int i = 1; i < cantDias; ++i){
		digitosAnteriores = definirDigitosSiguientes(digitosXDiasSorteados.at(i-1),cantidadDeDigitosXDia.at(lim)); // ingresar dias anteriores a un vector
		digitosNuevoDia = digitosAnteriores; //los anteriore0s que no salieron sorteados
		int a =cantidadDeDigitosXDia.at(lim);
		lim++;
		if (lim>6) lim = 0;
		int b = digitosAnteriores.size();
		for (int j = 0 ; j < a - b ; ++j){ //completar sorteo del dia 
			int aux = escogoRandom(min,max,digitosNuevoDia);
			digitosNuevoDia.push_back(aux);
		}
		digitosXDiasSorteados.push_back(digitosNuevoDia); 
		planificacionDia = obtenerListado(digitosXDiasSorteados.at(i),patentes);
		orden_aleatorio(planificacionDia);
		for (int i = 0; i < planificacionDia.size(); ++i){
			string folio;
			string nInterno;
			nInterno = encontrarFolio(planificacionDia.at(i),mapanMaquinas);
			folio = encontrarFolio(planificacionDia.at(i),mapaFolios);
			planificacionDiaConFolio.push_back(folio);
			planificacionDiaConNinterno.push_back(nInterno);	
		}
		planificacionObtenidaConFolios.push_back(planificacionDiaConFolio);
		planificacionObtenidaConNinternos.push_back(planificacionDiaConNinterno);
		planificacionObtenida.push_back(planificacionDia);
		planificacionDiaConNinterno.clear();
		planificacionDiaConFolio.clear();
		contarPatentes(planificacionDia,mapaPatentes);
		planificacionDia.clear();
		digitosAnteriores.clear();
		digitosNuevoDia.clear();
	}
	mapaPatentes.erase("");
	mapaFrecXdigitoPat.erase("");
	guardarPlanificacionObtenidaEnArchivo(cantDias,planificacionObtenida,planificacionObtenidaConFolios,planificacionObtenidaConNinternos,digitosXDiasSorteados,dia,mes,ano);
	guardar_map_en_archivo(mapaPatentes, "ResultadosObtenidos.txt","FrecuenciaBusesObtenida.txt");
    unordered_map<string, vector<string>> folios_con_patentes = separarPatentesPorFolios(mapaFolios);
    vector<vector<string>> patentes_separadas;

    // Convertir el unordered_map de folios y patentes en un vector de vectores
    vector<string> nombres_excels = obtenerNombresExcels(folios_con_patentes);
    cambiarFoliosANombreLinea(nombres_excels);
    for (auto const& folio : folios_con_patentes) {
        vector<string> patentes_con_folio = folio.second;
        patentes_separadas.push_back(patentes_con_folio);
    }
    for (int j = 0; j < patentes_separadas.size(); ++j){
		mapaFrecXdigitoPat = iniciarMapa(patentes_separadas.at(j));
		for (int i = 0; i < 10; ++i){
			vector<string> vec = obtenerListado(unDigitoXDia.at(i),patentes_separadas.at(j));
			PatentesXDigito.push_back(vec);
			contarPatentes(vec,mapaFrecXdigitoPat);
		}
		vector<string> nombreArchivos3;
		for (int i = 0; i < 10; ++i){
			string aux = "PatentesCon_";
			string aux2 = ".txt";
			string aux3 = "_";
			int numero = i;
			int numero2 = j;
	    	stringstream ss;
	    	stringstream ss2;
	    	ss2 << numero2;	
	    	ss << numero;
	    	string numero_str = ss.str();
	    	string numero_str2 = ss2.str();
	    	string name = aux + numero_str;
	    	name = name + aux3;
	    	name = name + numero_str2;
	    	name = name + aux2;
			nombreArchivos3.push_back(name);
		}
		for (int i = 0; i < PatentesXDigito.size(); ++i){
			guardarDias(nombreArchivos3.at(i),PatentesXDigito.at(i));
		}		
		int numero2 = j;
		stringstream ss2;
		ss2 << numero2;
		string numero_str2 = ss2.str();
		string nombreExcel = "1-Patentes";
		nombreExcel = nombreExcel + nombres_excels.at(j);
		string aux2 = ".xlsx";
		nombreExcel = nombreExcel + numero_str2;
		nombreExcel = nombreExcel + aux2;
		string nombreFinal = nombreExcel;
		lxw_workbook* workbook = workbook_new(nombreFinal.c_str());
		if (!workbook) {
		    cout << "No se pudo crear el archivo de salida." << endl;
		}
		// Crear hoja de Excel
		lxw_worksheet* worksheet = workbook_add_worksheet(workbook, NULL);
		int filaI=0;
		for (int i = 0; i < nombreArchivos3.size(); ++i){
			ifstream inFile4(nombreArchivos3.at(i));
			string nombre;
		    if (!inFile4) {
		        cout << "No se pudo abrir el archivo de entrada." << endl;      
		    }
			while (getline(inFile4, nombre)) {
			    worksheet_write_string(worksheet, filaI, 0+i,nombre.c_str(), NULL);     
		    	filaI++;
		    }	   
		    filaI=0;
			inFile4.close();
		}
		PatentesXDigito.clear();
		mapaFrecXdigitoPat.clear();
		workbook_close(workbook);
	}
	cout<<"Buses ordenados por digito guardados en 1-Patentes + 'nombre empresa' + .xlsx"<<endl;
	double varianza = calcularVarianza(mapaPatentes);
	cout<<endl;
	cout<<"Varianza obtenida: "<<varianza<<endl;
	cout<<endl;
	return 0;
}
