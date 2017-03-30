
/**
 * @file Nmer.h
 * @brief TDA Nmer
 *		Representa un conjunto de Nmer subsecuencias de tamaño 1 hasta N que
 *		se pueden obtener a partir de una cadena de ADN
 * @author alumno
 * @bug Por espeficicar
 */
#ifndef __NMER_H
#define __NMER_H

#include "ktree.h"
#include <string>
#include <set>

using namespace std;

class ordenCreciente{
public:
	bool operator() (const pair<string, int> & izq, const pair<string, int> & der){
		if(izq.second < der.second)
			return true;
		else
			if(izq.second == der.second)
				return izq.first < der.first;
			else
				return false;
	}
};

class ordenDecreciente{
public:
	bool operator() (const pair<string, int> & izq, const pair<string, int> & der){
		if(izq.second > der.second)
			return true;
		else
			if(izq.second == der.second)
				return izq.first > der.first;
			else
				return false;
	}
};



class Nmer {
public:
  typedef unsigned int size_type;

  /** @brief Constructor primitivo .
      Crea un Nmer de longitud maxima 0, con el valor ('-',0) en la raíz
  */
	Nmer();

  /** @brief Constructor de copia .
  */
  Nmer(const Nmer & a);

	/**	@brief Operador de asignación.
	*/
  Nmer & operator=(const Nmer & a);



  /**
	*	@brief Si adn es "ACT", devuelve el Nmer que representa todas
	*	las subcadenas que empiezan por "ACT" (ACT*)
	*	@return Devuelve el Nmer (subarbol) asociado a un prefijo. Si no existe, devuelve la raiz de Nmer por defecto (- 0)
	*/
  Nmer Prefix(const string & adn);

  /**
	*	@brief Se devuelve un Nmer donde para cada nodo (representa una secuencia).
	*	@brief Computa la suma de las frecuencias en *this y en referencia,
	*/
  Nmer Union(const Nmer & reference) ;

	/**
	*	@brief Devuelve true si la cadena adn está representada en el árbol.
	*
	*/
  bool containsString(const string & adn) const;

  /**
  *	@brief Devuelve true si cada nodo de *this está también representado en reference
  *	es decir, si todas las secuencias representadas en el árbol de *this están
  *	también incluidas en reference. False en otro caso.
  */
  bool included(const Nmer & reference) const;

	/**
	*	@brief Construir Nmer a partir de cadena de ADN
	*	@param l: maxima longitud del Nmer
	*	@param adn: cadena ADN
	*/
  void sequenceADN(unsigned int l, const string & adn);

	/**
	*	@brief Calcula la distancia entre x y *this
	*	@param x: Nmer a comparar la distancia
	*/
  float Distance(const Nmer & x);

  /**
	*	@brief Devuelve la lista de todas las subcadenas
	*	(no prefijo) que aparecen menos de threshold veces en el Nmer ordenadas en orden *	creciente de frecuencia
	*/
  set<pair<string,int>,ordenCreciente> rareNmer(int threshold);

  /**
  	*	@brief Devuelve la lista de las cadenas de
	*	longitud mayor posible (no prefijo) que aparecen más de threshold veces
	*	en el Nmer, ordenadas en orden
	*	decreciente de frecuencia
	*/
  set<pair<string,int>,ordenDecreciente> commonNmer(int threshold);

	/**
	*	@brief Devuelve el conjunto de Nmers de longitud exacta l.
	*/
  set<pair<string,int>,ordenCreciente> level(int l);


  /** @brief lectura fichero serializado
   * @param nombre_fichero fichero serializado con extension .srl
   *
   * Genera a partir del fichero el ktree que contiene todos los kmers asociados a una cadena de ADN
   *
   * La cadena original viene descrita en el fichero serializado
   */
  bool loadSerialized(const string & nombre_fichero);


  /** @brief Imprime los Nmers
   * @todo Debemos de implementar este método de forma que nos garantizemos que se imprimen todos los Nmers.
   */
   void list_Nmer() const;


  /** @brief Máxima longitud de los Nmers almacenados
   */
   unsigned int length()const;


  /** @brief Número de Nmers almacenados
   */
   size_type size() const;

private:
  ktree<pair<char,int>,4> el_Nmer; // subsecuencias
  unsigned int max_long; // Mayor longitud de la cadena representada, esto es, el nivel máximo del árbol


  /**
		@brief Función auxiliar recursiva de included
		@param actual: nodo del árbol donde queremos comprobar si está incluido
		@param incluido: nodo del árbol que recorremos para identificarlo y comprobar si está incluido en actual.
		@return si incluido está contenido en actual, devuelve true. En caso contrario false.
  */
  bool included_recursivo(const ktree<pair<char,int>,4>::const_node & actual, const ktree<pair<char,int>,4>::const_node & incluido) const;

  /**
		@brief Función auxiliar recursiva de Union
		@param izq: nodo del arbol del Nmer que llama
		@param der: nodo del arbol del Nmer pasado a Union
		@param incluir: nodo del arbol que se forma
		@param arbol_incluir: arbol que se forma
  */
  void Union_recursivo(const ktree<pair<char,int>,4>::const_node & izq, const ktree<pair<char,int>,4>::const_node & der, ktree<pair<char,int>,4>::node & incluir, ktree<pair<char,int>,4> & arbol_incluir) ;

  /**
		@brief Inserta una rama entera a partir de un nodo
		@param extrae: nodos de la rama a copia_original
		@param inserta: nodo del arbol que se esta formando
		@param arbol_insertar: arbol que se esta formando
  */
  void insertar_rama(const ktree<pair<char,int>,4>::const_node & extrae, ktree<pair<char,int>,4>::node & inserta, ktree<pair<char,int>,4> & arbol_insertar) ;

  /**
	 @brief Funcion auxiliar recursiva de rareNmer
	 @param nodo: Nodo del Nmer actual
	 @param nveces: Que la cadena se repita N o menos veces
	 @param conjunto: Conjunto a devolver
	 @param s: String completo del Nmer actual
  */
  void rareNmer_recursivo(const ktree<pair<char,int>,4>::const_node & nodo, const int & nveces, set< pair<string, int>, ordenCreciente > & conjunto, string s);


  /**
	 @brief Funcion auxiliar recursiva de commonNmer
	 @param nodo: Nodo del Nmer actual
	 @param nveces: Que la cadena se repita mas de N veces
	 @param conjunto: Conjunto a devolver
	 @param s: String completo del Nmer actual
  */
  void commonNmer_recursivo(const ktree<pair<char,int>,4>::const_node & nodo, const int & nveces, set< pair<string, int>, ordenDecreciente > & conjunto, string s);

  /**
	  @brief Funcion auxiliar recursiva de level
	  @param nodo: Nodo del Nmer actual
	  @param conjunto: Conjunto a devolver
	  @param s: String completo del Nmer actual
  */
  void level_recursivo(const ktree<pair<char,int>,4>::const_node & nodo, const int & l, int nivel_anterior, set< pair<string, int>, ordenCreciente > & conjunto, string s);

  /**
		@brief Suma 1 a la distancia por las ramas que no se encuentren en X
		@param nodo: Nodo de Y que no pertenece a X
		@param distancia: acumulacion de las distancias entre nodos.
  */
  void Distance_recursivo_una_rama(const ktree<pair<char,int>,4>::const_node & nodo, float & distancia);

  /**
		@brief Funcion auxiliar recursiva Distance
		@param Y: nodo del arbol actual
		@param X: nodo del arbol del Nmer pasado a Distance
		@param max: el valor maximo(rankingX.size(),rankingY.size())
		@param distancia: acumulacion de las distancias entre nodos.
  */
  void Distance_recursivo(const ktree<pair<char,int>,4>::const_node & Y, const ktree<pair<char,int>,4>::const_node & X, float & distancia, const float & max);

  /**
	  @brief Función auxiliar recursiva de list_Nmer
	  @param nodo: nodo del Nmer que llama
	  @param s: cadena de Nmers acumulados
  */
  void list_Nmer_recursivo(const ktree<pair<char,int>,4>::const_node & nodo, string s) const;

  /**
	  @brief Identifica el nucleótido pasado como argumento para saber el nº de hijo en el ktree.
	  @param letra: nucleótido
	  @return devuelve el nº de hijo que se corresponde con el carácter. Devuelve -1 en caso de fallo.
  */
 int identificar_letra(char letra) const;

 /**
	 @brief Identifica el numero de hijo con su nucleotido correspondiente
	 @param i: numero hijo
	 @return devuelve el nucleotido correspondiente con el numero introducido.
 */
 char identificar_numero(int i) const;

  /**
  *	@brief avanzar por la cadena a la vez que descendemos por el árbol para actualizar los valores del Nmer que representa.
  */
 void insertar_cadena(const string & cadena);

  /** @brief Functor para convertir un string en un pair<char,int>
   * se utiliza en loadSerialized
   */
  class String2Base {
   public:
    pair<char,int> operator()( const string & cad) {
	    pair<char,int> salida;
	    salida.first = cad[0];
	    salida.second = std::stoi(cad.substr(1));
	    return salida;
    }
  };

   /** @brief Functor para convertir un pair<char,int> en un string
    * Necesario para serializar un Nmer.
   */
  class Base2String {
   public:
     string operator()( const pair<char,int> & x) {
       string salida = string(1,x.first) + " " +std::to_string(x.second);
       return salida;
     }
  };
};


#endif
