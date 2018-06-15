#ifndef JEU_H
#define JEU_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "Terrain.h"
#include "Joueur.h"

using namespace std;

class Jeu {

	protected:

		unsigned int nb_joueur;
		Terrain terrain;
  		vector<Joueur> tab_joueur;

  		/** @brief Initialise la carte a utiliser */
  		void initJeu ();

  		/** @brief Affichage mode texte des regions et pays */
  		void afficheTerrainTXT();

	public:

		/** @brief : constructeur par default */
		Jeu();

		/** @brief : Accesseur pour le nombre de joueurs
				@return un entier
		 */
		unsigned int getNbJoueur();

		/** @brief : Mutateur du nombre de joueurs */
		void setNbJoueur(unsigned int nbj);

		/** @brief : Accesseur pour le tableau de joueurs
				@ return Tableau dynamique de joueur
		*/
		vector<Joueur>& getTabJoueur();

		/** @brief : Mutateur pour le tableau de joueurs */
		void setTabJoueur( vector<Joueur> tabj );

		/** @brief : lancer le jeu */
		void lancerJeu();

		/** @brief : met fin la partie */
		bool finPartie();

		/** @brief : afficher du texte d'aide pour les utilisateurs */
		void afficherAide();

		/** @brief : permet d'entrer en phase de manoeuvre */
		void phaseManoeuvre (Joueur &j);

		/** @brief : permet d'entrer en phase de renfort */
		void phaseRenfort();

		/** @brief : permet d'entrer en phase d'attaque */
		void phaseAttaque(Joueur &j);

		/** @brief : permet de mettre fin au tour */
		bool fintour();
};




#endif
