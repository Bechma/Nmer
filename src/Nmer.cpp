// BRUNO CHENOLL MATIENZO

#include "Nmer.h"

#include <fstream>
#include <cmath>
#include <iostream>
/*
template<typename T, int K>
extern void recorrido_preorden(typename ktree<T,K>::const_node n);
*/
std::ostream&   operator<<(ostream & s, const pair<char,int> & par){
	s << par.first << " " << par.second;
	return s;
}

template<typename T, int K>
void recorrido_preorden(const ktree<T,K> & arbol){
	recorrido_preorden<T,K>(arbol.root());
}

template<typename T, int K>
void recorrido_preorden(typename ktree<T,K>::const_node   n){

	if (!n.null()){
	  cout << "( " << (*n) <<"); " <<endl;
	  typename ktree<T,K>::const_node::child_iterator ini = n.begin();
	  typename ktree<T,K>::const_node::child_iterator fin = n.end();
	  while (ini!=fin){
		 recorrido_preorden<T,K>(*ini);
		 ++ini;
	   }
	 }
}









Nmer::Nmer() {
  max_long = 0;
  el_Nmer = ktree<pair<char,int>,4>(pair<char,int>('-',0));

}

Nmer::Nmer(const Nmer & a){
   this->el_Nmer = a.el_Nmer;
   this->max_long = a.max_long;
}

Nmer & Nmer::operator= (const Nmer & a){
   if(this != &a){
      this->el_Nmer = a.el_Nmer;
      this->max_long = a.max_long;
   }
   return *this;
}

int Nmer::identificar_letra(char letra) const{
	if(letra == 'A' or letra == 'a'){
		return 0;
	}
	else if (letra == 'G' or letra == 'g') {
		return 1;
	}
	else if (letra == 'C' or letra == 'c') {
		return 2;
	}
	else if (letra == 'T' or letra == 't'){
		return 3;
	}
	else
		return -1;
}

char Nmer::identificar_numero(int i) const{
	if (i == 0)			return 'A';
	else if (i == 1)	return 'G';
	else if (i == 2)	return 'C';
	else					return 'T';
}

void Nmer::insertar_cadena(const string & cadena){
	ktree<pair<char,int>,4>::node n_act = el_Nmer.root();

	for(char c : cadena){
		int numero = identificar_letra(c);

		if( n_act.k_child(numero).null() )
			el_Nmer.insert_k_child(n_act, numero, pair<char,int>(c,1));
		else
			(*(n_act.k_child(numero))).second++;

		n_act = n_act.k_child(numero);
	}

}

Nmer Nmer::Prefix(const string & adn){
	Nmer devolver, copia_original(*this);
	ktree<pair<char, int>, 4>::node original = copia_original.el_Nmer.root();
	bool existe_adn = true;

	for(unsigned i = 0; i < adn.size() && existe_adn; ++i){
		int numero = identificar_letra(adn[i]);
		if( ! original[numero].null() )
			original = original[numero];
		else
			existe_adn = false;
	}

	if( existe_adn ){
		copia_original.el_Nmer.prune_k_child(original.parent(), original.child_number(), devolver.el_Nmer);
		*(devolver.el_Nmer.root()) = pair<char,int>('-',0);
		devolver.max_long = copia_original.max_long - adn.size();
	}

	return devolver;
}

void Nmer::insertar_rama(const ktree<pair<char,int>,4>::const_node & extrae, ktree<pair<char,int>,4>::node & inserta, ktree<pair<char,int>,4> & arbol_insertar) {
	for(unsigned i = 0; i < 4; ++i){
		if( !extrae[i].null() ){
			arbol_insertar.insert_k_child(inserta, i, *extrae);
			ktree<pair<char,int>,4>::node asdf = inserta[i];
			insertar_rama(extrae[i], asdf, arbol_insertar);
		}

	}
}

void Nmer::Union_recursivo(const ktree<pair<char,int>,4>::const_node & izq,
									const ktree<pair<char,int>,4>::const_node & der, ktree<pair<char,int>,4>::node & incluir, ktree<pair<char,int>,4> & arbol_incluir) {
	for(unsigned i = 0; i < 4; ++i){
		if( !izq[i].null() && !der[i].null() ){
			arbol_incluir.insert_k_child(incluir, i, pair<char,int>(identificar_numero(i),(*izq[i]).second+(*der[i]).second));
			ktree<pair<char,int>,4>::node asdf = incluir[i];
			Union_recursivo(izq[i], der[i], asdf, arbol_incluir);
		}
		else if( !izq[i].null() && der[i].null() ){
			ktree<pair<char,int>,4>::node asdf = incluir[i];
			insertar_rama(izq[i], asdf, arbol_incluir);
		}
		else if( izq[i].null() && !der[i].null() ){
			ktree<pair<char,int>,4>::node asdf = incluir[i];
			insertar_rama(der[i], asdf, arbol_incluir);
		}
		else if( izq[i].null() && der[i].null() ){

		}
	}
}

Nmer Nmer::Union (const Nmer & reference) {
	Nmer devolver;
	ktree<pair<char,int>,4>::node asdf = devolver.el_Nmer.root();
	Union_recursivo(el_Nmer.root(), reference.el_Nmer.root(), asdf, devolver.el_Nmer);
	devolver.max_long = (max_long > reference.max_long) ? max_long : reference.max_long;
   return devolver;
}

bool Nmer::containsString(const string & adn) const{
	bool devolver = true;
	ktree<pair<char, int>, 4>::const_node nodo = el_Nmer.root();
	nodo = nodo[identificar_letra(adn[0])];
	for(unsigned i = 0; i < adn.size() && devolver; ){
		if(nodo.null()){
			devolver = false;
		}
		else{
			nodo = nodo[identificar_letra(adn[++i])];
		}
	}
	return devolver;
}

bool Nmer::included_recursivo(const ktree<pair<char,int>,4>::const_node & actual, const ktree<pair<char,int>,4>::const_node & incluido) const{
	bool devolver = true;
	for(unsigned i = 0; i < 4 && devolver; ++i){
		if( !actual[i].null() && !incluido[i].null() ){
			devolver = included_recursivo(actual[i], incluido[i]);
		}
		else if( !actual[i].null() && incluido[i].null() ){

		}
		else if( actual[i].null() && !incluido[i].null() ){
			devolver = false;
		}
		else if( actual[i].null() && incluido[i].null() ){

		}
	}

	return devolver;
}

// AGT
bool Nmer::included(const Nmer & reference) const{
	return included_recursivo(el_Nmer.root(), reference.el_Nmer.root());
}

void Nmer::sequenceADN(unsigned int l, const string & adn){
	*this = Nmer();
	max_long = l;
	int i = 0;
	string aux = adn.substr(i,l);

	while(aux.size() > 0){
		insertar_cadena(aux);
		i++;
		aux = adn.substr(i,l);
	}
}

void Nmer::Distance_recursivo_una_rama(const ktree<pair<char,int>,4>::const_node & nodo, float & distancia){
	for(unsigned i = 0; i < 4; ++i){
		if( !nodo[i].null() ){
			++distancia;
			Distance_recursivo_una_rama(nodo[i], distancia);
		}
	}
}

void Nmer::Distance_recursivo(const ktree<pair<char,int>,4>::const_node & Y, const ktree<pair<char,int>,4>::const_node & X, float & distancia, const float & max){

	for(unsigned i = 0; i < 4; ++i){
		if( !Y[i].null() && !X[i].null() ){
			Distance_recursivo(Y[i], X[i], distancia, max);
			distancia += abs( (*Y[i]).second - (*X[i]).second )/max;
		}
		else if( !Y[i].null() && X[i].null() ){
			Distance_recursivo_una_rama(Y[i], distancia);
		}
		else if( Y[i].null() && !X[i].null() ){

		}
		else if( Y[i].null() && X[i].null() ){

		}
	}

}

float Nmer::Distance(const Nmer & x){
	float max = (this->size() > x.size()) ? this->size() : x.size();
	float dist = 0;

	Distance_recursivo(el_Nmer.root(), x.el_Nmer.root(), dist, max);

	return dist/size();
}



bool Nmer::loadSerialized(const string & fichero) {
 string cadena;
 ifstream fe;
 pair<char,int> nulo('x',-1);
 Nmer::String2Base stb;

 fe.open(fichero.c_str(), ifstream::in);
 if (fe.fail()){
    cerr << "Error al abrir el fichero " << fichero << endl;
 }
 else {
    //leo la cabecera del fichero (líneas que comienzan con #)
    do{
      getline(fe,cadena,'\n');
    } while (cadena.find("#")==0 && !fe.eof());
    // leemos Nmer_length
     max_long = std::stoi(cadena);
     // leemos cadena serializada
    getline(fe,cadena,'\n');
    el_Nmer.deserialize(cadena,nulo,';',stb);
    fe.close();
    return true;
 } // else
 fe.close();
 return false;
}

void Nmer::rareNmer_recursivo(const ktree<pair<char,int>,4>::const_node & nodo, 			const int & nveces, set< pair<string, int>, ordenCreciente > & conjunto, string s){
	bool tiene_hijos = false;
	string aux = s+'x';
	unsigned tama = aux.size();

	for(unsigned i = 0; i < 4; ++i){
		if(!nodo[i].null()){
			tiene_hijos = true;
			aux[tama-1] = identificar_numero(i);
			rareNmer_recursivo(nodo[i], nveces, conjunto, aux);
		}
	}

	if( !tiene_hijos && (*nodo).second <= nveces ){
		conjunto.insert(pair<string, int>(s, (*nodo).second));
	}

}

set<pair<string,int>,ordenCreciente> Nmer::rareNmer(int threshold){
	set< pair<string, int>, ordenCreciente > devolver;

	rareNmer_recursivo(el_Nmer.root(), threshold, devolver, "");

	return devolver;
}

void Nmer::commonNmer_recursivo(const ktree<pair<char,int>,4>::const_node & nodo, 		const int & nveces, set< pair<string, int>, ordenDecreciente > & conjunto, string s){

	bool es_mayor = false;
	string aux = s+'x';
	unsigned tama = aux.size();

	for(unsigned i = 0; i < 4; ++i){
		if(!nodo[i].null()){
			aux[tama-1] = identificar_numero(i);
			if( (*nodo[i]).second > nveces){
				es_mayor = true;
				commonNmer_recursivo(nodo[i], nveces, conjunto, aux);
			}
		}
	}

	if( !es_mayor && (*nodo).second > nveces ){
		conjunto.insert(pair<string, int>(s, (*nodo).second));
	}

}

set<pair<string,int>,ordenDecreciente> Nmer::commonNmer(int threshold){
	set< pair<string, int>, ordenDecreciente > devolver;

	commonNmer_recursivo(el_Nmer.root(), threshold, devolver, "");

	return devolver;
}

void Nmer::level_recursivo(const ktree<pair<char,int>,4>::const_node & nodo, 			const int & l, int nivel_anterior, set< pair<string, int>, ordenCreciente > & conjunto, string s){
	string aux = s+'x';
	unsigned tama = aux.size();
	int nivel_actual = nivel_anterior+1;

	for(unsigned i = 0; i < 4; ++i){
		if(!nodo[i].null()){
			aux[tama-1] = identificar_numero(i);

			if( nivel_actual == l )
				conjunto.insert(pair<string, int>(aux, (*nodo[i]).second));
			else
				level_recursivo(nodo[i], l, nivel_actual, conjunto, aux);

		}
	}
}

set<pair<string,int>,ordenCreciente> Nmer::level(int l){
	set< pair<string, int>, ordenCreciente > devolver;
	int nactual = 0;

	level_recursivo(el_Nmer.root(), l, nactual, devolver, "");

	return devolver;
}

void Nmer::list_Nmer_recursivo(const ktree<pair<char,int>,4>::const_node & nodo, string s) const{
	string aux = s+'x';
	unsigned tama = aux.size();
	for(unsigned i = 0; i < 4; i++){
		if( !nodo[i].null() ){
			aux[tama-1]= identificar_numero(i);
			cout << "(" << aux << " " << (*nodo[i]).second << "); " <<endl;
			list_Nmer_recursivo(nodo[i], aux);
		}
	}
}

void Nmer::list_Nmer() const {
   list_Nmer_recursivo(el_Nmer.root(), "");
}

unsigned int Nmer::length() const {
   return max_long;
}

Nmer::size_type Nmer::size() const{
  return el_Nmer.size()-1;
}
