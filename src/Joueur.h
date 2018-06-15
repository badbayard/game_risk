#ifndef JOUEUR_H
#define JOUEUR_H

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <cassert>
#include <vector>

#include "Region.h"

using namespace std ;

class Joueur {

	private :

		string couleur;
		string nom_joueur;
		unsigned int nb_regions_initial;
		unsigned int nb_regiments;
		vector<Region*> tab_region ;

	public:
		/**
		* @brief Constructeur de la classe Joueur
		* @param nom : chaine de caractère
			@param couleur_joueur
			@return Un Joueur
		*/
		Joueur(const string & nom = "", const string & couleur_joueur = "");

		/** @brief : Recupére la couleur du joueur
		 		@return une chaine de caractère*/
		string  getCouleurJoueur() const;

		/** @brief : modifie la couleur du joueur
		 * @param : couleur_joueur chaine de caractère
		 * */
		void setCouleurJoueur(string couleur_joueur);

		/** @brief : Recupére le nombre de region du joueur
				@return entier*/
		unsigned int getNbRegions() const;

		/** @brief : Recupère le nombre de region initial du joueur
				@return entier
		*/
		unsigned int getNbRegionsInit();

		 /** @brief : modifie le nombre de region initial du joueur
		 * @param : nb_Regions entier
		 * */
		void setNbRegionsInit(unsigned int nb_Regions);

		/** @brief : Recupére le nombre de regiments du joueur
				@return : entier*/
		unsigned int getNbRegiments () const;

		/** @brief : modifie le nombre de regiments
		 * @param : nb_Regiments entier */
		void setNbRegiments (unsigned int nb_Regiments);

		/** @brief : modifie le nom du joueur
		 *  @param : nom_joueur chaine de caractere */
		void setnom_joueur (string nom);

		/** @brief:prend le nom du jouer
				@return chaine de caractère*/
		string getnom_joueur();

		/** @brief Accesseur pour tab_region 
				@return Tableau dynamique de Region*/
		vector<Region*> & getRegionsJoueur();

		/** @brief: test de regression de la classe joueur */
		void testRegressionJoueur();



};

#endif
