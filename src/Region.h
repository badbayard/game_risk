#ifndef REGION_H
#define REGION_H

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <cassert>
#include <vector>

using namespace std;

class Region {

	private :
		string nom_region;
		string couleur_joueur;
		unsigned int nb_unites;
		vector<Region*> frontaliers;

	public :
		/** @brief Constructeur par défaut */
		Region();

		/** @brief Constructeur de la classe Region */
		Region(const string & nom, unsigned int nb_unite = 1, const string & couleur_region = "");

		/** @brief Destructeur */
		~Region() {}



		/**
		* @brief Accesseur pour nom_region
		* @return Le nom de la region
		*/
		string getNomRegion () const;

		/** 
		* @brief Mutateur pour nom_region
		* @param nom: string
		*/
		void setNomRegion (const string & nom);



		/**
		* @brief Accesseur pour couleur_joueur
		* @return La couleur du joueur possedant la region
		*/
		string getCouleurRegion () const;

		/**
		* @brief Mutateur pour couleur_joueur
		* @param couleur_region: string
		*/
		void setCouleurRegion (const string & couleur_region);



		/**
		* @brief Accesseur pour nb_unites
		* @return Le nombre d'unites presentes dans la region
		*/
		unsigned int getNbUnite () const;

		/**
		* @brief Mutateur pour nb_unites
		* @param nb_unite : entier
		*/
		void setNbUnite (const unsigned int & nb_unite);



		/**
		* @brief Accesseur pour frontaliers
		* @return Reference sur frontaliers
		*/
		vector<Region*> & getTabFrontaliers ();

		/**
		* @brief Mutateur pour frontaliers
		* @param tab_frontalier: tableau de Region*
		*/
		void setTabFrontaliers (const vector<Region*> & tab_frontalier);



		/**
		* @brief Teste si une region donnee est voisine de l'instance consideree
		* @param voisin : Region
		* @return Booleen
		*/
		bool estFrontalier (const Region & voisin) const;



		/**
		* @brief Ajoute une Region donnee en parametre dans le tableau de frontaliers de l'instance consideree
		* @param voisin: Region
		*/
		void ajouterFrontalier (const Region & voisin);



		/** @brief Test de regression pour la classe Region */
		void testRegressionRegion();
};

#endif
