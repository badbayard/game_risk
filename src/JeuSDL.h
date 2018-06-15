#ifndef JEUSDL_H
#define JEUSDL_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL/SDL_mixer.h>

#include "Jeu.h"

using namespace std;

/*
	Classe Image
*/
class Image
{
	private:
		SDL_Texture * texture;
		bool a_change;

	public:
		SDL_Surface * surface;
		TTF_Font * font;

		/** @brief constructeur  par défault*/
		Image();

		/** @brief Destructeur */
		~Image();

		/** @brief charge la surface
				@param nom_image : chaine de caractère
				@return booléen
		*/
		bool loadSurface(const string & nom_image);

		/** @brief charge la texture
				@param nom_image : chaine de caractère
				@param render : SDL_Renderer
				@return : booléen
		*/
		bool loadTexture(const string & nom_image, SDL_Renderer * render);

		/** @brief charge le fond
				@param font_file :chaine de caractère
				@param ptsize : entier
				@return booléen
		*/
		bool loadFont(const string & font_file, int ptsize = 12);

		/** @brief écris sur la texture
		 		@param message : chaine de caractère
				@param ft : TTF_Font
				@param render : SDL_Renderer
				@param text_color : SDL_Color
				@param background : SDL_Color
				@param wrapping : Uint32
				@return booléen*/
		bool writeOnTexture(const string & message, TTF_Font * ft, SDL_Renderer * render, Uint32 wrapping = 500, SDL_Color text_color = {0,0,0} );

		/** @brief dessine dans le renderer
		 		@param render : SDL_Renderer
				@param x : entier
				@param y ; entier
				@param w : entier
				@param h : entier*/
		void draw(SDL_Renderer * render, int x = 0, int y = 0, int w = -1, int h = -1);
};

class CodeRGB
{
	public:
		int R, G, B;

		CodeRGB(int red = 0, int green = 0, int blue = 0) {
			R = red;	G = green;	B = blue;
		}
};


/*
	Classe JeuSDL
*/
class JeuSDL : public Jeu
{

	protected:
		Image carte;
		int fenetre_taille_x, fenetre_taille_y;
		SDL_Window * fenetre;
		SDL_Renderer * renderer;
		int souris_x, souris_y;
		Uint32 current_pix;
		SDL_Rect pix;
		Uint8 r, g, b;
		Image menu;
		Image Aide;
		Image hover_box;
		Image static_box;
		unordered_map<string, CodeRGB> CodeCouleur;
		bool all_ok;
		int joueur_actuel;


  		/** @brief Initialise la carte a utiliser */
  		void initJeu ();

  		/**
  		* @brief Charge les donnees de la carte contenues dans le fichier chemin
  		* @param chemin (string) : nom du fichier contenant les codes couleur sur la carte
  		*/
  		void lireDonneesCarte (const string & chemin);

		/** @brief Recupere la valeur du nombre choisi a la souris
				@return int
		*/
		int getNombreChoisi();

	public:

		/** @brief constructeur par défault */
		JeuSDL();
		/** @brief Destructeur de la classe */
		~JeuSDL();


		/**
		* @brief Initialise les elements SDL necessaire a l'affichage graphique du jeu
		* @return true si tout s'est bien passé, false s'il y a eu une erreur
		*/
		bool afficherInit();

		/**
		* @brief Lance la partie
		*/
		void boucleJeu();

		/**
		* @brief Quitte la SDL proprement en fin de programme
		*/
		void quitterSDL();

		/** @brief crée le menu*/
		void MenuSDL();

		/** @brief récupére le nom de la region par le code RGB
		 		@param R : entier
				@param G : entier
				@param B : entier
				@return chaine de caractère*/
		string getNomParRGB(int R, int G, int B);

		/** @brief permet d'initialiser la music et de lancer la music du jeu */
		void MusicSDL();

};

#endif
