#ifndef PAYS_H
#define PAYS_H

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <cassert>
#include <vector>

#include "Region.h"

using namespace std;

class Pays {

	private :

		string nom_pays;
		unsigned int nb_regions;
		unsigned int regiments_supp;
		vector<Region*> tab_region;

	public :

		/** @brief Constructeur par défault */
		Pays();

		/** @brief Constructeur de la classe
		 		@param nom : chaine de caractère
				@param nbregions : entier
				@param nbregiments : entier*/
		Pays(const string & nom, unsigned int nbregions, unsigned int nbregiments);

		/** @brief Destructeur pour la classe Pays */
		~Pays();


		/**
		* @brief Accesseur pour nom_pays
		* @return string
		*/
		string getNomPays () const;

		/**
		* @brief Mutateur pout nom_pays
		* @param nom : string
		*/
		void setNomPays (const string & nom);



		/**
		* @brief Accesseur pour nb_regions
		* @return Le nombre de regions dans le Pays
		*/
		unsigned int getNbRegions () const;

		/**
		* @brief Mutateur pout nb_regions
		* @param nbregions : entier
		*/
		void setNbRegions (unsigned int nbregions);



		/**
		* @brief Accesseur pour tab_region
		* @return Reference sur tab_region pour le manipuler directement
		*/
		vector<Region*> & getTabRegions ();

		/**
		* @brief Mutateur pout tab_region
		* @param tabRegion : tableau de Region
		*/
		void setTabRegions (const vector<Region*> & tabRegion);



		/**
		* @brief Accesseur pour regiments_supp
		* @return Le bonus de régiments supplémentaires associé à une région
		*/
		unsigned int getRegimentsSupp() const;



		/**
		* @brief Teste si un joueur controle toutes les regions du Pays
		* @param couleur_joueur : string (couleur associee au joueur)
		* @return Vrai si le joueur controle toute la region, faux sinon
		*/
		bool controlePays (const string & couleur_joueur) const;



		/**
		* @brief Ajoute une région donnée en paramètre dans le tableau des régions de l'instance Pays considérée
		* @param reg : Region
		*/
		void ajouterRegion (const Region & reg);



		/** @brief test de regression pour la classe pays */
		void testRegressionPays();
};

#endif
