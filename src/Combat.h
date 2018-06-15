#ifndef COMBAT_H
#define COMBAT_H

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <cassert>

#include "Joueur.h"
#include "Region.h"

using namespace std;

class Combat {

	private :
		Joueur attaquant;
		Joueur defenseur;
		Region region_attaquant;
		Region region_defenseur;

	public :
	/** @brief Constructeur par défaut */
		Combat();
	/** @brief Constructeur par copie
			@param joueur_a:Joueur
			@param joueur_d:Joueur
			@param region_a:Region
			@param region_d:Region
	 */
		Combat(Joueur &joueur_a, Joueur &joueur_d, Region region_a, Region region_d);

	/** @brief Destructeur */
		~Combat();

	/** @brief Accesseur pour le joueur attaquant
		@return Les informations concernant le joueur attaquant */
	Joueur get_attaquant() const;

	/** @brief Mutateur pour le joueur attaquant
		@param attaquant : Joueur */
	void set_attaquant(Joueur att);


	/** @brief Accesseur pour le joueur defenseur
		@return Les informations concernant le joueur defenseur */
	Joueur get_defenseur() const;

	/** @brief Mutateur pour le joueur defenseur
		@param defenseur : Joueur */
	void set_defenseur(Joueur def);


	/** @brief Accesseur pour la region region_attaquant
		@return Les informations concernant la region region_attaquant */
	Region get_region_attaquant() const;

	/** @brief Mutateur pour la region region_attaquant
		@param region_attaquant : Region */
	void set_region_attaquant(Region region_a);


	/** @brief Accesseur pour la region_defenseur
		@return Les informations concernant la region_defenseur */
	Region get_region_defenseur() const;

	/** @brief Mutateur pour la region_defenseur
		@param region_defenseur : Region */
	void set_region_defenseur(Region region_d);

	/**  @brief Renvoie le gagnant d'une bataille
		  @param nb_troupes_attaquant
			@param nb_troupes_defenseur : entier
		  @return Vrai si l'attaquant gagne, faux sinon*/
	bool bataille(unsigned int nb_troupes_attaquant, unsigned int nb_troupes_defenseur);

	/**  @brief Lance un combat et fais les modifications suite à celui-ci
		 @param region_attaquant : Region 
		 @param region_defenseur : Region
		 */
	void maj_troupes(Region& region_attaquant, Region& region_defenseur, Joueur& jatt, Joueur& jdef, const int & att = -1, const int & def = -1, const int & transfert = -1);
	
};


#endif
