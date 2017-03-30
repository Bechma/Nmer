// BRUNO CHENOLL MATIENZO

#include<iostream>
#include<fstream>
#include<sstream>
#include<Nmer.h>
using namespace std;

int main(void){

	Nmer prueba;
	ifstream archivo("datos/simple");
	string adn;
	getline(archivo, adn);
	prueba.sequenceADN(3, adn);

	cout << "Prueba de inserción de la cadenaSimple.srl(archivo datos/simple)\n";
	prueba.list_Nmer();
	cout << prueba.size() << " " << prueba.length() << endl << endl;

	Nmer prueba_prefijo;
	prueba_prefijo.sequenceADN(3, "TCA");
	cout << "¿Esta incluido el Nmer \"TCA\" en el Nmer anterior?\n";

	if(prueba.included(prueba_prefijo))
		cout << "SIIIIIIIIIIIII\n";
	else
		cout << "NOOOOOOOOOOOOO\n";

	cout << "¿Contiene la cadena ATA el Nmer anterior?\n";
	if(prueba.containsString("ATA"))
		cout << "Lo contiene" << endl;
	else
		cout << "No está" << endl;

	cout << "Extraigo un Nmer a partir de T:\n";
	Nmer prefijo = prueba.Prefix("T");
	prefijo.list_Nmer();
	cout << prefijo.size() << " " << prueba.length() << endl;

	Nmer launion(prueba);
	Nmer resultado = prueba.Union(launion);
	cout << "\nUno 2 cadenas simples, debería de salir el mismo resultado con los Nmers +1\n";
	resultado.list_Nmer();
	cout << resultado.size() << " " << resultado.length() << endl << endl;

	auto tresomenos = resultado.rareNmer(3);

	cout << "En el resultado obtenido, imprimo los Nmers con 3 o menos repeticiones\n";
	for(auto it : tresomenos){
		cout << "( " << it.first << " " << it.second << " )" << endl;
	}

	tresomenos = resultado.level(2);
	cout << "Los Nmers de longitud 2\n";
	for(auto it : tresomenos){
		cout << "( " << it.first << " " << it.second << " )" << endl;
	}

	cout << "Los Nmer que se repiten mas de 1 vez son:\n";
	auto tresomas = resultado.commonNmer(1);
	for(auto it : tresomas){
		cout << "( " << it.first << " " << it.second << " )" << endl;
	}

	cout << "La distancia que hay entre el Nmer \"TCA\" y el resultado de la union anterior es de: " << prueba_prefijo.Distance(resultado) << endl;

	cout << "\nLas distancias entre el Nmer 1.txt y el resto con longitud 8 es de:\n";

	fstream cadena_humano("datos/1.txt");
	Nmer humano;
	getline(cadena_humano, adn);
	humano.sequenceADN(8, adn);
	cadena_humano.close();

	for(int i = 2; i <= 10; i++){
		ostringstream archivos;
		archivos << "datos/" << i << ".txt";

		cadena_humano.open( archivos.str() );
		getline(cadena_humano, adn);
		prueba.sequenceADN(8, adn);
		cout << "Distancia entre " << i << ".txt y 1.txt es de: " << humano.Distance(prueba) << endl;
		cadena_humano.close();
	}



	return 0;
}
