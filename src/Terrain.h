#ifndef TERRAIN_H
#define TERRAIN_H

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <cassert>
#include <vector>

#include "Region.h"
#include "Pays.h"

using namespace std;

class Terrain {

	private:

		unsigned int dim_x;
		unsigned int dim_y;
 		vector<Pays*> tab_pays;

 		/**
 		* @brief Ajoute un pays donné en paramètre dans la liste des pays de l'instance Terrain considérée
 		* @param pays : Pays
 		*/
 		void ajouterPays (const Pays & pays);

	public:
	/** @brief Constructeur par défault */
		Terrain();

	/** @brief Constructeur de la classe
	 * @param dimensionX dimension Y : entier */
		Terrain(unsigned int dimensionX, unsigned int dimensionY);

	/** @brief destructeur de la classe */
		~Terrain();

	/** @brief récupére la taille dim_x
			@return entier*/
		unsigned int getdim_x() const;

	/** @brief modifie dim_x
	 *  @param dimensionX : entier */
		void setdim_x(unsigned int dimensionX);

		/** @brief récupére la taille dim_y
				@return entier*/
		unsigned int getdim_y() const;

		/** @brief modifie dim_y
		 *  @param dimensionY : entier */
		void setdim_y(unsigned int dimensionY);


		/**
		* @brief Accesseur pour tab_pays
		* @return Reference sur tab_pays, directement manipulable
		*/
		vector<Pays*> & getTabPays ();

		/** @brief Initialise le terrain */
		void initTerrain ();

		/** @brief permet de faire le test de regression */
		void testRegressionTerrain();
};


#endif
