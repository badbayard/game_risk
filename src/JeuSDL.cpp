#include "JeuSDL.h"
#include "Jeu.h"
#include "Combat.h"

using namespace std;

/*
	Classe Image
*/
Image::Image()
{
	surface = NULL;
	texture = NULL;
	font = NULL;
	a_change = false;
}

Image::~Image()
{
	if (surface != NULL) {
		SDL_FreeSurface(surface);
		surface = NULL;
	}
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
	if (font != NULL) {
		TTF_CloseFont(font);
		font = NULL;
	}
}

bool Image::loadSurface(const string & nom_image)
{
	// Chargement de l'image
	surface = IMG_Load(nom_image.c_str());
	if(surface == NULL) {
		cout << "Erreur de chargement de l'image " << nom_image << endl;
		cout << "(IMG_GetError) " << IMG_GetError() << endl;
		cout << "(SDL_GetError) " << SDL_GetError() << endl;
		return false;
	}

    SDL_Surface * surfaceCorrectPixelFormat = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_ARGB8888,0);
    SDL_FreeSurface(surface);
    surface = surfaceCorrectPixelFormat;

	return true;
}

bool Image::loadTexture(const string & nom_image, SDL_Renderer * render)
{
	// Chargement de l'image
	if (!loadSurface(nom_image)) {
		return false;
	}

	// Chargement de la texture
	texture = SDL_CreateTextureFromSurface(render,surface);
    if (texture == NULL) {
        cout << "Erreur: probleme creation texture pour " << nom_image << endl;
        return false;
    }

    return true;
}

bool Image::loadFont(const string & font_file, int ptsize)
{
	if (font != NULL) {
		TTF_CloseFont(font);
		font = NULL;
	}
	font = TTF_OpenFont(font_file.c_str(), ptsize);
	if (font == NULL) {
		cout << "TTF_OpenFont Erreur: " << SDL_GetError() << endl;
		return false;
	}
	return true;
}

bool Image::writeOnTexture(const string & message, TTF_Font * ft, SDL_Renderer * render, Uint32 wrapping, SDL_Color text_color)
{
	surface = TTF_RenderUTF8_Blended_Wrapped(ft, message.c_str(), text_color, wrapping);
	if (surface == NULL) {
		cout << "writeOnTexture Erreur: " << SDL_GetError() << endl;
		return false;
	}
	texture = SDL_CreateTextureFromSurface(render,surface);
    if (texture == NULL) {
        cout << "Erreur: probleme creation texture TTF pour " << message << endl;
        return false;
    }
	//SDL_FreeSurface(surface);
	return true;
}

void Image::draw(SDL_Renderer * render, int x, int y, int w, int h)
{
	// Positionnement
    int ok;
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = (w < 0) ? surface->w : w;
    rect.h = (w < 0) ? surface->h : h;

	// Mise a jour de la texture si elle a change
    if (a_change) {
        ok = SDL_UpdateTexture(texture,NULL,surface->pixels,surface->pitch);
        assert(ok == 0);
        a_change = false;
    }

	// Affichage de la texture
    ok = SDL_RenderCopy(render,texture,NULL,&rect);
    assert(ok == 0);
}








/*
	Classe JeuSDL
*/
JeuSDL::JeuSDL() : Jeu()
{
	fenetre = NULL;
	renderer = NULL;
	pix.w = 1;
	pix.h = 1;
	r = 0;
	g = 0;
	b = 0;
	all_ok = true;
	joueur_actuel = 0;
	fenetre_taille_x = 960;
	fenetre_taille_y = 637;
	//afficherInit();
}



JeuSDL::~JeuSDL()
{
	quitterSDL();
}




bool JeuSDL::afficherInit()
{
	// Initialisation SDL
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
		cout << "SDL_Init Erreur: " << SDL_GetError() << endl;
		return false;
	}

	// Initialisation SDL_image pour utiliser les autres formats d'image
  int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		cout << "IMG_Init Erreur: (IMG_GetError) " << IMG_GetError() << ", (SDL_GetError) " << SDL_GetError() << endl;
		SDL_Quit();
		return false;
	}

	// Creation fenetre
	fenetre = SDL_CreateWindow("A vos Risk et perils!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, fenetre_taille_x, fenetre_taille_y, SDL_WINDOW_SHOWN);
	if (fenetre == NULL) {
		cout << "SDL_CreateWindow Erreur: " << SDL_GetError() << endl;
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	// Creation renderer
	renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		cout << "SDL_CreateRenderer Erreur: " << SDL_GetError() << endl;
		SDL_DestroyWindow(fenetre);
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	//Initialisation de SDL_ttf
	if( TTF_Init() == -1 ) {
		cout << "TTF_Init Erreur: " << SDL_GetError() << endl;
		SDL_Quit();
		return false;
	}

	// On vide le renderer
	SDL_RenderClear(renderer);

	// Mise a jour buffer
	return true;
}





void JeuSDL::quitterSDL()
{
	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
	}
	if (fenetre != NULL) {
		SDL_DestroyWindow(fenetre);
	}
	IMG_Quit();
	/*
	// Ce bloc cause un core dump s'il est exécuté
	if (TTF_WasInit()) {
		TTF_Quit();
	}
	*/
	Mix_CloseAudio();
	SDL_Quit();
}





void JeuSDL::initJeu()
{
	Jeu::initJeu();
	lireDonneesCarte("data/code_RVB");
	all_ok = all_ok && carte.loadTexture(string("data/Risk_modif_reduit.png"), renderer);
	all_ok = all_ok && hover_box.loadFont("data/sample.ttf");
	all_ok = all_ok && static_box.loadFont("data/sample.ttf");

}



Region* getRegionChoisie (const vector<Region*> & territoires, const string & nom)
{
	for (unsigned int i = 0; i < territoires.size(); i++) {
		if (territoires[i]->getNomRegion() == nom) {
			return territoires[i];
		}
	}
	return NULL;
}



void JeuSDL::boucleJeu() {
	initJeu();
	if ( !all_ok ) {
		cout << "Erreur initialisation" << endl;
		exit(1);
	}
	bool debut = true;
	bool repartition = false;
	bool renfort = false;
	bool attaque = false;
	bool manoeuvre = false;
	bool quitter = false;
	SDL_RenderClear( renderer );
	carte.draw( renderer );
	static_box.writeOnTexture( "Choisissez le nombre\nde joueurs", static_box.font, renderer, 220 );
	static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
	SDL_RenderPresent( renderer );

	// Liste de tous les territoires
	vector<Region*> all_regions;
	for (unsigned int u = 0; u < terrain.getTabPays().size(); u++)
	{
		for (unsigned int j = 0; j < terrain.getTabPays()[u]->getTabRegions().size(); j++)
		{
			all_regions.push_back( terrain.getTabPays()[u]->getTabRegions()[j] );
		}
	}

	// File d'evenements : stocke toutes les donnes d'evenement
	SDL_Event evenements;
	while ( !quitter ) {

		// Recuperation d'un evenement
		SDL_WaitEvent( &evenements );

		SDL_RenderClear( renderer );
		carte.draw( renderer );
		if ( debut ) {
			static_box.writeOnTexture( "Choisissez le nombre\nde joueurs", static_box.font, renderer, 220 );
			static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
		}
		if ( repartition ) {
			string texte = "Repartition des troupes\nJoueur ";
			texte += to_string(joueur_actuel);
			texte += " (";
			texte += tab_joueur[joueur_actuel].getCouleurJoueur();
			texte += "), \nrepartissez vos\ntroupes !\n(";
			texte += to_string(tab_joueur[joueur_actuel].getNbRegiments());
			texte += " unites restantes)\n\nChoisissez une region";
			static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
			static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
		}
		SDL_RenderPresent( renderer );

		// Selon le type d'evenement
		switch ( evenements.type ) {
			// Si on apuuie sur le bouton X de la fenetre
			case SDL_QUIT:
				quitter = true;
				break;

			// Si on appuie sur une touche du clavier
			case SDL_KEYDOWN:
				// Selon la touche appuiee
				switch ( evenements.key.keysym.scancode ) {
					// Si on appuie sur la touche Escape
					case SDL_SCANCODE_ESCAPE:
						quitter = true;
						break;

					// Pour les autres touches non gerees par le switch
					default:
						break;
				}
				break;

			// Au mouvement de la souris
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState( &souris_x, &souris_y );
				break;

			case SDL_MOUSEBUTTONUP:
				// Initialisation des joueurs
				if ( debut ) {
					int n = getNombreChoisi();
					SDL_RenderClear( renderer );
					carte.draw( renderer );
					if ( n == -1 ) {
						hover_box.writeOnTexture( "Ceci n'est pas un nombre", static_box.font, renderer, 220 );
						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 350) / 425 );
					}
					else if (n < 2) {
						hover_box.writeOnTexture( "Minimum 2 joueurs", static_box.font, renderer, 220 );
						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 350) / 425 );
					}
					else if (n > 5) {
						hover_box.writeOnTexture( "Maximum 5 joueurs", static_box.font, renderer, 220 );
						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 350) / 425 );
					}
					else {
						nb_joueur = n;
						// Initialisation des choix de couleurs possibles
						vector<string> liste_couleurs;
						liste_couleurs.push_back("Bleu");
						liste_couleurs.push_back("Vert");
						liste_couleurs.push_back("Orange");
						liste_couleurs.push_back("Blanc");
						liste_couleurs.push_back("Violet");
						for (unsigned int i = 0; i < nb_joueur; i++) {
							// Ajout d'un nouveau joueur avec les valeurs saisies
							tab_joueur.push_back( Joueur( "nom", liste_couleurs[0] ) );
							// Suppression de la couleur choisie
							liste_couleurs.erase(liste_couleurs.begin());
							// Traitement du nombre de troupes et du nombre de regions
							switch(nb_joueur) {
								case 2:
									tab_joueur[i].setNbRegiments(40);
									tab_joueur[i].setNbRegionsInit(21);
									break;
								case 3:
									tab_joueur[i].setNbRegiments(35);
									tab_joueur[i].setNbRegionsInit(14);
									break;
								case 4:
									tab_joueur[i].setNbRegiments(30);
									tab_joueur[i].setNbRegionsInit(10);
									break;
								case 5:
									tab_joueur[i].setNbRegiments(25);
									tab_joueur[i].setNbRegionsInit(8);
									break;
								default: cout << "pas de joueur" << endl;
									break;
							}
						}

						srand((unsigned int) time(NULL));

						// Liste de tous les territoires
						vector<Region*> territoires;
						for (unsigned int i = 0; i < terrain.getTabPays().size(); i++) {
							for (unsigned int j = 0; j < terrain.getTabPays()[i]->getTabRegions().size(); j++) {
								territoires.push_back( terrain.getTabPays()[i]->getTabRegions()[j] );
							}
						}

						// Repartition aleatoire des territoires
						for (unsigned int i = 0; i < nb_joueur; i++) {
							for (unsigned int j = 0; j < tab_joueur[i].getNbRegionsInit(); j++) {
								unsigned int random_val = rand() % territoires.size();
								tab_joueur[i].getRegionsJoueur().push_back( territoires[ random_val ] );
								territoires[ random_val ]->setCouleurRegion( tab_joueur[i].getCouleurJoueur() );
								territoires.erase( territoires.begin() + random_val );
							}
						}
						// Il reste eventuellement des territoires non attribués pour une partie à 4 ou 5 joueurs

						for (unsigned int i = 0; i < nb_joueur; i++) {
							tab_joueur[i].setNbRegiments( tab_joueur[i].getNbRegiments() - tab_joueur[i].getNbRegions() );
						}
						debut = false;
						repartition = true;
					}
					SDL_RenderPresent( renderer );
				}

				// Repartition des troupes
				else if ( repartition ) {
					SDL_RenderClear( renderer );
					carte.draw( renderer );
					static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
					pix.x = souris_x;
					pix.y = souris_y;
					SDL_RenderReadPixels(renderer, &pix, SDL_PIXELFORMAT_ARGB8888, &current_pix, sizeof(current_pix));
					SDL_GetRGB(current_pix, carte.surface->format, &r, &g, &b);
					Region* region_choisie = getRegionChoisie( tab_joueur[joueur_actuel].getRegionsJoueur(), getNomParRGB( (int)r, (int)g, (int)b ));
					if ( region_choisie == NULL ) {
						hover_box.writeOnTexture( "Cette region ne \nvous appartient pas", static_box.font, renderer, 220 );
						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
					}
					else {
						string texte = "Choisissez le nombre\nde troupes a placer\nsur ";
						texte += region_choisie->getNomRegion();
						hover_box.writeOnTexture( texte, static_box.font, renderer, 220 );
						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
						SDL_RenderPresent( renderer );
						bool continuer = true;
						while (continuer) {
							SDL_WaitEvent( &evenements );
							switch ( evenements.type ) {
								case SDL_QUIT:
									quitter = true;
									continuer = false;
									break;
								case SDL_KEYDOWN:
									switch ( evenements.key.keysym.scancode ) {
										case SDL_SCANCODE_ESCAPE:
											quitter = true;
											continuer = false;
											break;
					
											// Pour les autres touches non gerees par le switch
										default:
											break;
									}
									break;
								case SDL_MOUSEBUTTONDOWN:
									SDL_GetMouseState( &souris_x, &souris_y );
									break;
								case SDL_MOUSEBUTTONUP:
									int nb = getNombreChoisi();
									if ( nb == -1 ) {
										SDL_RenderClear( renderer );
										carte.draw( renderer );
										hover_box.writeOnTexture( "Ceci n'est pas un nombre", static_box.font, renderer, 220 );
										static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
										hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
										SDL_RenderPresent( renderer );
										continuer = false;
									}
									else if (((unsigned int) nb) > tab_joueur[joueur_actuel].getNbRegiments()) {
										SDL_RenderClear( renderer );
										carte.draw( renderer );
										string texte = "Maximum ";
										texte += to_string(tab_joueur[joueur_actuel].getNbRegiments());
										texte += " unites";
										hover_box.writeOnTexture( texte, static_box.font, renderer, 220 );
										static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
										hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
										SDL_RenderPresent( renderer );
									}
									else {
										region_choisie->setNbUnite( region_choisie->getNbUnite() + nb );
										tab_joueur[joueur_actuel].setNbRegiments( tab_joueur[joueur_actuel].getNbRegiments() - nb );
										continuer = false;
										if (tab_joueur[joueur_actuel].getNbRegiments() == 0) {
											joueur_actuel++;
											if (((int) joueur_actuel) == ((int) nb_joueur)) {
												repartition = false;
												renfort = true;
												quitter = true;
											}
										}
									}

									break;
							}
						}
					}
					SDL_RenderPresent( renderer );
				}
				break;
		}
	}

	joueur_actuel = 0;
	bool reset_total = true;
	quitter = false;
	unsigned int total_renfort;
	SDL_RenderClear( renderer );
	carte.draw( renderer );
	static_box.writeOnTexture( "Debut de la partie", static_box.font, renderer, 220 );
	static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
	SDL_RenderPresent( renderer );
	while ( !quitter ) {

		// Recuperation d'un evenement
		SDL_WaitEvent( &evenements );

		SDL_RenderClear( renderer );
		carte.draw( renderer );
		if ( renfort ) {
			string texte = "Phase renfort\nJoueur ";
			texte += to_string(joueur_actuel);
			texte += " (";
			texte += tab_joueur[joueur_actuel].getCouleurJoueur();
			texte += "),\nchoisis une region\nou placer les renforts";
			static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
			static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );

			if (reset_total) {
				total_renfort = ( b >= 3 ) ? (tab_joueur[joueur_actuel].getNbRegions() / 3) : 3;
				reset_total = false;
			}
			tab_joueur[joueur_actuel].setNbRegiments( total_renfort );
		}
		else if ( attaque ) {
			string texte = "Phase attaque !\nJoueur ";
			texte += to_string(joueur_actuel);
			texte += " (";
			texte += tab_joueur[joueur_actuel].getCouleurJoueur();
			texte += "),\nvoulez-vous attaquer?\n";
			static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
			static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
			hover_box.writeOnTexture( "OUI\n\nNON", static_box.font, renderer, 220 );
			hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
		}
		else if ( manoeuvre ) {
			string texte = "Phase manoeuvre\nJoueur ";
			texte += to_string(joueur_actuel);
			texte += " (";
			texte += tab_joueur[joueur_actuel].getCouleurJoueur();
			texte += "),\nchoisissez la region\nde depart";
			static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
			static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
		}
		SDL_RenderPresent( renderer );

		// Selon le type d'evenement
		switch ( evenements.type ) {
			// Si on apuuie sur le bouton X de la fenetre
			case SDL_QUIT:
				quitter = true;
				break;

			// Si on appuie sur une touche du clavier
			case SDL_KEYDOWN:
				// Selon la touche appuiee
				switch ( evenements.key.keysym.scancode ) {
					// Si on appuie sur la touche Escape
					case SDL_SCANCODE_ESCAPE:
						quitter = true;
						break;

					// Pour les autres touches non gerees par le switch
					default:
						break;
				}
				break;

			// Au mouvement de la souris
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState( &souris_x, &souris_y );
				break;

			case SDL_MOUSEBUTTONUP:
				if ( renfort ) {
					pix.x = souris_x;
					pix.y = souris_y;
					SDL_RenderReadPixels(renderer, &pix, SDL_PIXELFORMAT_ARGB8888, &current_pix, sizeof(current_pix));
					SDL_GetRGB(current_pix, carte.surface->format, &r, &g, &b);
					Region* region_choisie = getRegionChoisie( tab_joueur[joueur_actuel].getRegionsJoueur(), getNomParRGB( (int)r, (int)g, (int)b ));
					if ( region_choisie == NULL ) {
						SDL_RenderClear( renderer );
						carte.draw( renderer );
						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
						hover_box.writeOnTexture( "Cette region ne\nvous appartient pas", static_box.font, renderer, 220 );
						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
						SDL_RenderPresent( renderer );
					}
					else {
						SDL_RenderClear( renderer );
						carte.draw( renderer );
						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
						string texte = "Combien de troupes a placer\nsur ";
						texte += region_choisie->getNomRegion();
						texte += "?\n(";
						texte += to_string(region_choisie->getNbUnite());
						texte += " unites)";
						hover_box.writeOnTexture( texte, static_box.font, renderer, 220 );
						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
						SDL_RenderPresent( renderer );
						bool continuer = true;
						while (continuer) {
							SDL_WaitEvent( &evenements );
							switch ( evenements.type ) {
								case SDL_QUIT:
									quitter = true;
									continuer = false;
									break;
								case SDL_KEYDOWN:
									switch ( evenements.key.keysym.scancode ) {
										case SDL_SCANCODE_ESCAPE:
											quitter = true;
											continuer = false;
											break;
					
											// Pour les autres touches non gerees par le switch
										default:
											break;
									}
									break;
								case SDL_MOUSEBUTTONDOWN:
									SDL_GetMouseState( &souris_x, &souris_y );
									break;
								case SDL_MOUSEBUTTONUP:
									int nb = getNombreChoisi();
									if ( nb == -1 ) {
										SDL_RenderClear( renderer );
										carte.draw( renderer );
										hover_box.writeOnTexture( "Ceci n'est pas un nombre", static_box.font, renderer, 220 );
										static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
										hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
										SDL_RenderPresent( renderer );
										continuer = false;
									}
									else if (((unsigned int) nb) > total_renfort) {
										SDL_RenderClear( renderer );
										carte.draw( renderer );
										string texte = "Maximum ";
										texte += to_string(total_renfort);
										texte += " troupes";
										hover_box.writeOnTexture( texte, static_box.font, renderer, 220 );
										static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
										hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
										SDL_RenderPresent( renderer );
									}
									else {
										region_choisie->setNbUnite(region_choisie->getNbUnite() + nb);
										total_renfort = total_renfort - nb;
										continuer = false;

										// Incrementation
										if (((int) total_renfort) <= 0) {
											joueur_actuel++;
											reset_total = true;
											if (((int) joueur_actuel) == ((int) nb_joueur)) {
												renfort = false;
												attaque = true;
												joueur_actuel = 0;
											}
										}
									}

									break;
							}
						}
					}
				}

				else if ( attaque ) {

					// OUI
					if ( souris_x > 29 && souris_x < 73 && souris_y > 540 && souris_y < 570 ) {
						Region region_attaquant, region_defenseur;
  						vector <Region> tab_regions_frontalieres;
						SDL_RenderClear( renderer );
						carte.draw( renderer );
						string texte = "Phase attaque !\nJoueur ";
						texte += to_string(joueur_actuel);
						texte += " (";
						texte += tab_joueur[joueur_actuel].getCouleurJoueur();
						texte += "),\nchoisissez la region\nattaquante";
						static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
						SDL_RenderPresent( renderer );

						bool continuer = true;
						while (continuer) {
							SDL_WaitEvent( &evenements );
							SDL_RenderClear( renderer );
							carte.draw( renderer );
							string texte = "Phase attaque !\nJoueur ";
							texte += to_string(joueur_actuel);
							texte += " (";
							texte += tab_joueur[joueur_actuel].getCouleurJoueur();
							texte += "),\nchoisissez la region\nattaquante";
							static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
							static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
							SDL_RenderPresent( renderer );
							switch ( evenements.type ) {
								case SDL_QUIT:
									quitter = true;
									continuer = false;
									break;
								case SDL_KEYDOWN:
									switch ( evenements.key.keysym.scancode ) {
										case SDL_SCANCODE_ESCAPE:
											quitter = true;
											continuer = false;
											break;
					
											// Pour les autres touches non gerees par le switch
										default:
											break;
									}
									break;
								case SDL_MOUSEBUTTONDOWN:
									SDL_GetMouseState( &souris_x, &souris_y );
									break;
								case SDL_MOUSEBUTTONUP:
									pix.x = souris_x;
									pix.y = souris_y;
									SDL_RenderReadPixels(renderer, &pix, SDL_PIXELFORMAT_ARGB8888, &current_pix, sizeof(current_pix));
									SDL_GetRGB(current_pix, carte.surface->format, &r, &g, &b);
									Region* region_choisie = getRegionChoisie( tab_joueur[joueur_actuel].getRegionsJoueur(), getNomParRGB( (int)r, (int)g, (int)b ));
									if ( region_choisie == NULL ) {
										SDL_RenderClear( renderer );
										carte.draw( renderer );
										static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
										hover_box.writeOnTexture( "Cette region ne \nvous appartient pas", static_box.font, renderer, 220 );
										hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
										SDL_RenderPresent( renderer );
									}
									else {
										SDL_RenderClear( renderer );
										carte.draw( renderer );
										string texte = "Phase attaque !\nJoueur ";
										texte += to_string(joueur_actuel);
										texte += " (";
										texte += tab_joueur[joueur_actuel].getCouleurJoueur();
										texte += "),\nchoisissez la region\na attaquer";
										static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
										static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
										SDL_RenderPresent( renderer );
										bool continuer2 = true;
										while (continuer2) {
											SDL_WaitEvent( &evenements );
											switch ( evenements.type ) {
												case SDL_QUIT:
													quitter = true;
													continuer = false;
													continuer2 = false;
													break;
												case SDL_KEYDOWN:
													switch ( evenements.key.keysym.scancode ) {
														case SDL_SCANCODE_ESCAPE:
															quitter = true;
															continuer = false;
															continuer2 = false;
															break;
										
															// Pour les autres touches non gerees par le switch
														default:
															break;
													}
													break;
												case SDL_MOUSEBUTTONDOWN:
													SDL_GetMouseState( &souris_x, &souris_y );
													break;
												case SDL_MOUSEBUTTONUP:
													pix.x = souris_x;
													pix.y = souris_y;
													SDL_RenderReadPixels(renderer, &pix, SDL_PIXELFORMAT_ARGB8888, &current_pix, sizeof(current_pix));
													SDL_GetRGB(current_pix, carte.surface->format, &r, &g, &b);
													Region* region_cible = getRegionChoisie( region_choisie->getTabFrontaliers(), getNomParRGB( (int)r, (int)g, (int)b ));
													if ( region_cible == NULL ) {
														SDL_RenderClear( renderer );
														carte.draw( renderer );
														static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
														hover_box.writeOnTexture( "Cette region n'est pas\nfrontaliere", static_box.font, renderer, 220 );
														hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
														SDL_RenderPresent( renderer );
														continuer2 = false;
													}
													else {
														Joueur * joueur_cible;
														for (unsigned int it = 0; it < tab_joueur.size(); it++) {
															for (unsigned int it2 = 0; it2 < tab_joueur[it].getRegionsJoueur().size(); it2++) {
																if (region_cible == tab_joueur[it].getRegionsJoueur()[it2]) {
																	joueur_cible = &(tab_joueur[it]);
																}
															}
														}
														Combat batailleEpique(tab_joueur[joueur_actuel], *joueur_cible, *region_choisie, *region_cible);
														SDL_RenderClear( renderer );
														carte.draw( renderer );
														string texte = "Phase attaque !\nJoueur ";
														texte += to_string(joueur_actuel);
														texte += " (";
														texte += tab_joueur[joueur_actuel].getCouleurJoueur();
														texte += "),\ncombien de troupes\nvont attaquer ?";
														static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
														static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
														SDL_RenderPresent( renderer );
														bool continuer3 = true;
														while (continuer3) {
															SDL_WaitEvent( &evenements );
															SDL_RenderClear( renderer );
															carte.draw( renderer );
															string texte = "Phase attaque !\nJoueur ";
															texte += to_string(joueur_actuel);
															texte += " (";
															texte += tab_joueur[joueur_actuel].getCouleurJoueur();
															texte += "),\ncombien de troupes\nvont attaquer ?";
															static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
															static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
															SDL_RenderPresent( renderer );
															switch ( evenements.type ) {
																case SDL_QUIT:
																	quitter = true;
																	continuer = false;
																	continuer2 = false;
																	continuer3 = false;
																	break;
																case SDL_KEYDOWN:
																	switch ( evenements.key.keysym.scancode ) {
																		case SDL_SCANCODE_ESCAPE:
																			quitter = true;
																			continuer = false;
																			continuer2 = false;
																			continuer3 = false;
																			break;
																			
																			// Pour les autres touches non gerees par le switch
																		default:
																			break;
																	}
																	break;
																case SDL_MOUSEBUTTONDOWN:
																	SDL_GetMouseState( &souris_x, &souris_y );
																	break;
																case SDL_MOUSEBUTTONUP:
																	int nb = getNombreChoisi();
																	if ( nb == -1 ) {
																		SDL_RenderClear( renderer );
																		carte.draw( renderer );
																		hover_box.writeOnTexture( "Ceci n'est pas un nombre", static_box.font, renderer, 220 );
																		static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
																		hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
																		SDL_RenderPresent( renderer );
																	}
																	else if (((unsigned int) nb) > 3 || ((unsigned int) nb) > region_choisie->getNbUnite()) {
																		SDL_RenderClear( renderer );
																		carte.draw( renderer );
																		string texte = "Maximum ";
																		texte += (region_choisie->getNbUnite() < 3) ? (to_string(region_choisie->getNbUnite())) : (to_string(3));
																		texte += " troupes";
																		hover_box.writeOnTexture( texte, static_box.font, renderer, 220 );
																		static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
																		hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
																		SDL_RenderPresent( renderer );
																	}
																	else {
																		SDL_RenderClear( renderer );
																		carte.draw( renderer );
																		string texte = "Phase attaque !\nJoueur ";
																		texte += joueur_cible->getCouleurJoueur();
																		texte += ",\ncombien de troupes\nen defensive ?";
																		static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
																		static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
																		SDL_RenderPresent( renderer );
																		bool continuer4 = true;
																		while (continuer4) {
																			SDL_WaitEvent( &evenements );
																			SDL_RenderClear( renderer );
																			carte.draw( renderer );
																			string texte = "Phase attaque !\nJoueur ";
																			texte += joueur_cible->getCouleurJoueur();
																			texte += ",\ncombien de troupes\nen defensive ?";
																			static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
																			static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
																			SDL_RenderPresent( renderer );
																			switch ( evenements.type ) {
																				case SDL_QUIT:
																					quitter = true;
																					continuer = false;
																					continuer2 = false;
																					continuer3 = false;
																					continuer4 = false;
																					break;
																				case SDL_KEYDOWN:
																					switch ( evenements.key.keysym.scancode ) {
																						case SDL_SCANCODE_ESCAPE:
																							quitter = true;
																							continuer = false;
																							continuer2 = false;
																							continuer3 = false;
																							continuer4 = false;
																							break;
																							
																							// Pour les autres touches non gerees par le switch
																						default:
																							break;
																					}
																					break;
																				case SDL_MOUSEBUTTONDOWN:
																					SDL_GetMouseState( &souris_x, &souris_y );
																					break;
																				case SDL_MOUSEBUTTONUP:
																					int nb2 = getNombreChoisi();
																					if ( nb2 == -1 ) {
																						SDL_RenderClear( renderer );
																						carte.draw( renderer );
																						hover_box.writeOnTexture( "Ceci n'est pas un nombre", static_box.font, renderer, 220 );
																						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
																						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
																						SDL_RenderPresent( renderer );
																					}
																					else if (((unsigned int) nb2) > 2 || ((unsigned int) nb2) > region_cible->getNbUnite()) {
																						SDL_RenderClear( renderer );
																						carte.draw( renderer );
																						string texte = "Maximum ";
																						texte += (region_cible->getNbUnite() < 2) ? (to_string(region_cible->getNbUnite())) : (to_string(2));
																						texte += " troupes";
																						hover_box.writeOnTexture( texte, static_box.font, renderer, 220 );
																						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
																						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
																						SDL_RenderPresent( renderer );
																					}
																					else {
																						SDL_RenderClear( renderer );
																						carte.draw( renderer );
																						string texte = "Phase attaque !\nJoueur ";
																						texte += to_string(joueur_actuel);
																						texte += " (";
																						texte += tab_joueur[joueur_actuel].getCouleurJoueur();
																						texte += "),\ncombien de troupes\ntransférer si victoire ?";
																						static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
																						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
																						SDL_RenderPresent( renderer );
																						bool continuer5 = true;
																						while (continuer5) {
																							SDL_WaitEvent( &evenements );
																							SDL_RenderClear( renderer );
																							carte.draw( renderer );
																							string texte = "Phase attaque !\nJoueur ";
																							texte += to_string(joueur_actuel);
																							texte += " (";
																							texte += tab_joueur[joueur_actuel].getCouleurJoueur();
																							texte += "),\ncombien de troupes\ntransférer si victoire ?";
																							static_box.writeOnTexture( texte, static_box.font, renderer, 220 );
																							static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
																							SDL_RenderPresent( renderer );
																							switch ( evenements.type ) {
																								case SDL_QUIT:
																									quitter = true;
																									continuer = false;
																									continuer2 = false;
																									continuer3 = false;
																									continuer4 = false;
																									continuer5 = false;
																									break;
																								case SDL_KEYDOWN:
																									switch ( evenements.key.keysym.scancode ) {
																										case SDL_SCANCODE_ESCAPE:
																											quitter = true;
																											continuer = false;
																											continuer2 = false;
																											continuer3 = false;
																											continuer4 = false;
																											continuer5 = false;
																											break;
																											
																											// Pour les autres touches non gerees par le switch
																										default:
																											break;
																									}
																									break;
																								case SDL_MOUSEBUTTONDOWN:
																									SDL_GetMouseState( &souris_x, &souris_y );
																									break;
																								case SDL_MOUSEBUTTONUP:
																									int nb3 = getNombreChoisi();
																									if ( nb3 == -1 ) {
																										SDL_RenderClear( renderer );
																										carte.draw( renderer );
																										hover_box.writeOnTexture( "Ceci n'est pas un nombre", static_box.font, renderer, 220 );
																										static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
																										hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
																										SDL_RenderPresent( renderer );
																									}
																									else if (((unsigned int) nb3) > region_choisie->getNbUnite()) {
																										SDL_RenderClear( renderer );
																										carte.draw( renderer );
																										string texte = "Maximum ";
																										texte += to_string(region_choisie->getNbUnite());
																										texte += " troupes";
																										hover_box.writeOnTexture( texte, static_box.font, renderer, 220 );
																										static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
																										hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
																										SDL_RenderPresent( renderer );
																									}
																									else {
																										SDL_RenderClear( renderer );
																										carte.draw( renderer );
																										static_box.writeOnTexture( "Attaque...", static_box.font, renderer, 220 );
																										static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
																										SDL_RenderPresent( renderer );
																										batailleEpique.maj_troupes(*region_choisie, *region_cible, tab_joueur[joueur_actuel], *joueur_cible, nb, nb2, nb3);
																										continuer = false;
																										continuer2 = false;
																										continuer3 = false;
																										continuer4 = false;
																										continuer5 = false;
																									}
																									break;
																							}
																						}

















																					}
																					break;
																			}
																		}

																		// Incrementation
																		// if (((int) total_renfort) <= 0) {
																		// 	joueur_actuel++;
																		// 	reset_total = true;
																		// 	if (((int) joueur_actuel) == ((int) nb_joueur)) {
																		// 		renfort = false;
																		// 		attaque = true;
																		// 		joueur_actuel = 0;
																		// 	}
																		// }
																	}
																	break;
															}
														}
													}
													break;
											}
										}







									}
									break;
							}
						}

						joueur_actuel++;
						if (((unsigned int) joueur_actuel) == nb_joueur) {
							joueur_actuel = 0;
							attaque = false;
							manoeuvre = true;
						}
					}


					// NON
					else if ( souris_x > 29 && souris_x < 73 && souris_y > 570 && souris_y < 600 ) {
						joueur_actuel++;
						if (((unsigned int) joueur_actuel) == nb_joueur) {
							joueur_actuel = 0;
							attaque = false;
							manoeuvre = true;
						}
					}

				}

				else if ( manoeuvre ) {
					pix.x = souris_x;
					pix.y = souris_y;
					SDL_RenderReadPixels(renderer, &pix, SDL_PIXELFORMAT_ARGB8888, &current_pix, sizeof(current_pix));
					SDL_GetRGB(current_pix, carte.surface->format, &r, &g, &b);
					Region* region_choisie = getRegionChoisie( tab_joueur[joueur_actuel].getRegionsJoueur(), getNomParRGB( (int)r, (int)g, (int)b ));
					if ( region_choisie == NULL ) {
						SDL_RenderClear( renderer );
						carte.draw( renderer );
						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
						hover_box.writeOnTexture( "Cette region ne\nvous appartient pas", static_box.font, renderer, 220 );
						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
						SDL_RenderPresent( renderer );
					}
					else if ( region_choisie->getNbUnite() == 1 ) {
						SDL_RenderClear( renderer );
						carte.draw( renderer );
						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
						hover_box.writeOnTexture( "Cette region n'a\nqu'une unite", static_box.font, renderer, 220 );
						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
						SDL_RenderPresent( renderer );
					}
					else {
						SDL_RenderClear( renderer );
						carte.draw( renderer );
						static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
						hover_box.writeOnTexture( "Choisissez la region\nfrontaliere d'arrivee", static_box.font, renderer, 220 );
						hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
						SDL_RenderPresent( renderer );
						bool continuer2 = true;
						while (continuer2) {
							SDL_WaitEvent( &evenements );
							switch ( evenements.type ) {
								case SDL_QUIT:
									quitter = true;
									continuer2 = false;
									break;
								case SDL_KEYDOWN:
									switch ( evenements.key.keysym.scancode ) {
										case SDL_SCANCODE_ESCAPE:
											quitter = true;
											continuer2 = false;
											break;
						
											// Pour les autres touches non gerees par le switch
										default:
											break;
									}
									break;
								case SDL_MOUSEBUTTONDOWN:
									SDL_GetMouseState( &souris_x, &souris_y );
									break;
								case SDL_MOUSEBUTTONUP:
									pix.x = souris_x;
									pix.y = souris_y;
									SDL_RenderReadPixels(renderer, &pix, SDL_PIXELFORMAT_ARGB8888, &current_pix, sizeof(current_pix));
									SDL_GetRGB(current_pix, carte.surface->format, &r, &g, &b);
									Region* region_cible = getRegionChoisie( region_choisie->getTabFrontaliers(), getNomParRGB( (int)r, (int)g, (int)b ));
									if ( region_cible == NULL ) {
										SDL_RenderClear( renderer );
										carte.draw( renderer );
										static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
										hover_box.writeOnTexture( "Cette region n'est pas\nfrontaliere", static_box.font, renderer, 220 );
										hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
										SDL_RenderPresent( renderer );
										continuer2 = false;
									}
									else {
										SDL_RenderClear( renderer );
										carte.draw( renderer );
										static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
										hover_box.writeOnTexture( "Combien de troupes\nvoulez-vous transferer ?", static_box.font, renderer, 220 );
										hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
										SDL_RenderPresent( renderer );
										bool continuer = true;
										while (continuer) {
											SDL_WaitEvent( &evenements );
											switch ( evenements.type ) {
												case SDL_QUIT:
													quitter = true;
													continuer = false;
													continuer2 = false;
													break;
												case SDL_KEYDOWN:
														switch ( evenements.key.keysym.scancode ) {
															case SDL_SCANCODE_ESCAPE:
																quitter = true;
																continuer = false;
																continuer2 = false;
																break;
											
																// Pour les autres touches non gerees par le switch
															default:
																break;
														}
														break;
												case SDL_MOUSEBUTTONDOWN:
														SDL_GetMouseState( &souris_x, &souris_y );
														break;
												case SDL_MOUSEBUTTONUP:
													int nb = getNombreChoisi();
													if ( nb == -1 ) {
														SDL_RenderClear( renderer );
														carte.draw( renderer );
														hover_box.writeOnTexture( "Ceci n'est pas un nombre", static_box.font, renderer, 220 );
														static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
														hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
														SDL_RenderPresent( renderer );
														continuer = false;
													}
													else if (((unsigned int) nb) > region_choisie->getNbUnite() - 1) {
														SDL_RenderClear( renderer );
														carte.draw( renderer );
														string texte = "Maximum ";
														texte += to_string(region_choisie->getNbUnite() - 1);
														texte += " troupes";
														hover_box.writeOnTexture( texte, static_box.font, renderer, 220 );
														static_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 307) / 425 );
														hover_box.draw( renderer, (fenetre_taille_x * 25) / 640, (fenetre_taille_y * 370) / 425 );
														SDL_RenderPresent( renderer );
													}
													else {
														region_choisie->setNbUnite(region_choisie->getNbUnite() - nb);
														region_cible->setNbUnite(region_cible->getNbUnite() + nb);
														continuer = false;
														continuer2 = false;
														joueur_actuel++;
														if (((unsigned int) joueur_actuel) == nb_joueur) {
															joueur_actuel = 0;
															manoeuvre = false;
															renfort = true;
														}
													}
													break;
												
											}
										}
									}
									break;
							}
						}
					}
				}

				break;
		}
	}

}








string JeuSDL::getNomParRGB(int R, int G, int B)
{
	for (unordered_map<string, CodeRGB>::iterator it = CodeCouleur.begin() ; it != CodeCouleur.end() ; ++it) {
		/*
		cout << it->first << endl;
		cout << "R: " << it->second.R << endl;
		cout << "G: " << it->second.G << endl;
		cout << "B: " << it->second.B << endl;
		cout << endl;
		*/
		if (it->second.R == R && it->second.G == G && it->second.B == B) {
			return it->first;
		}
	}
	return string("x");
}




void JeuSDL::lireDonneesCarte(const string & chemin)
{
	CodeCouleur[ string("Alaska") ] = CodeRGB( 223, 192, 88 );
	CodeCouleur[ string("Alberta") ] = CodeRGB( 226, 192, 90 );
	CodeCouleur[ string("Amerique centrale") ] = CodeRGB( 231, 200, 97 );
	CodeCouleur[ string("Etats de l'Est") ] = CodeRGB( 228, 200, 95 );
	CodeCouleur[ string("Groenland") ] = CodeRGB( 231, 200, 95 );
	CodeCouleur[ string("Territoires du Nord-Ouest") ] = CodeRGB( 225, 192, 88 );
	CodeCouleur[ string("Ontario") ] = CodeRGB( 225, 192, 90 );
	CodeCouleur[ string("Quebec") ] = CodeRGB( 221, 192, 88 );
	CodeCouleur[ string("Etats de l'Ouest") ] = CodeRGB( 207, 193, 84 );
	CodeCouleur[ string("Afghanistan") ] = CodeRGB( 169, 201, 94 );
	CodeCouleur[ string("Chine") ] = CodeRGB( 169, 201, 96 );
	CodeCouleur[ string("Inde") ] = CodeRGB( 173, 204, 97 );
	CodeCouleur[ string("Tchita") ] = CodeRGB(173,204,100);
	CodeCouleur[ string("Japon") ] = CodeRGB(175,204,100);
	CodeCouleur[ string("Kamtchaka") ] = CodeRGB(178,207,101);
	CodeCouleur[ string("Moyen-Orient") ] = CodeRGB(179,207,102);
	CodeCouleur[ string("Mongolie") ] = CodeRGB(179,207,104);
	CodeCouleur[ string("Siam") ] = CodeRGB(180,207,105);
	CodeCouleur[ string("Siberie") ] = CodeRGB(184,209,106);
	CodeCouleur[ string("Oural") ] = CodeRGB(187,212,108);
	CodeCouleur[ string("Yakoutie") ] = CodeRGB(190,214,109);
	CodeCouleur[ string("Argentine") ] = CodeRGB(215,72,88);
	CodeCouleur[ string("Bresil") ] = CodeRGB(217,73,89);
	CodeCouleur[ string("Perou") ] = CodeRGB(219,73,89);
	CodeCouleur[ string("Venezuela") ] = CodeRGB(222,74,90);
	CodeCouleur[ string("Grande-Bretagne") ] = CodeRGB(149,166,173);
	CodeCouleur[ string("Islande") ] = CodeRGB(149,166,175);
	CodeCouleur[ string("Europe du Nord") ] = CodeRGB(151,166,175);
	CodeCouleur[ string("Scandinavie") ] = CodeRGB(151,166,176);
	CodeCouleur[ string("Europe du Sud") ] = CodeRGB(151,166,178);
	CodeCouleur[ string("Ukraine") ] = CodeRGB(154,169,181);
	CodeCouleur[ string("Europe occidentale") ] = CodeRGB(156,169,181);
	CodeCouleur[ string("Congo") ] = CodeRGB(155,122,98);
	CodeCouleur[ string("Afrique de l'Est") ] = CodeRGB(157,122,98);
	CodeCouleur[ string("Egypte") ] = CodeRGB(161,125,100);
	CodeCouleur[ string("Madagascar") ] = CodeRGB(162,125,100);
	CodeCouleur[ string("Afrique du Nord") ] = CodeRGB(162,125,102);
	CodeCouleur[ string("Afrique du Sud") ] = CodeRGB(164,125,102);
	CodeCouleur[ string("Australie Orientale") ] = CodeRGB(166,72,161);
	CodeCouleur[ string("Indonesie") ] = CodeRGB(168,72,161);
	CodeCouleur[ string("Nouvelle-Guinee") ] = CodeRGB(170,72,161);
	CodeCouleur[ string("Australie Occidentale") ] = CodeRGB(173,72,161);
	//CodeCouleur[ string("") ] = CodeRGB(,,);

}




//610 60
//942


int JeuSDL::getNombreChoisi() {
	if ((souris_x > 564) && (souris_x < 601) && (souris_y > 0) && (souris_y < 19)) {
		return 1;
	}
	else if ((souris_x > 601) && (souris_x < 639) && (souris_y > 0) && (souris_y < 19)) {
		return 2;
	}
	else if ((souris_x > 639) && (souris_x < 676) && (souris_y > 0) && (souris_y < 19)) {
		return 3;
	}
	else if ((souris_x > 676) && (souris_x < 716) && (souris_y > 0) && (souris_y < 19)) {
		return 4;
	}
	else if ((souris_x > 716) && (souris_x < 754) && (souris_y > 0) && (souris_y < 19)) {
		return 5;
	}
	else if ((souris_x > 754) && (souris_x < 792) && (souris_y > 0) && (souris_y < 19)) {
		return 6;
	}
	else if ((souris_x > 792) && (souris_x < 830) && (souris_y > 0) && (souris_y < 19)) {
		return 7;
	}
	else if ((souris_x > 830) && (souris_x < 868) && (souris_y > 0) && (souris_y < 19)) {
		return 8;
	}
	else if ((souris_x > 868) && (souris_x < 905) && (souris_y > 0) && (souris_y < 19)) {
		return 9;
	}
	else if ((souris_x > 905) && (souris_x < 955) && (souris_y > 0) && (souris_y < 19)) {
		return 10;
	}
	else if ((souris_x > 564) && (souris_x < 601) && (souris_y > 19) && (souris_y < 36)) {
		return 11;
	}
	else if ((souris_x > 601) && (souris_x < 639) && (souris_y > 19) && (souris_y < 36)) {
		return 12;
	}
	else if ((souris_x > 639) && (souris_x < 676) && (souris_y > 19) && (souris_y < 36)) {
		return 13;
	}
	else if ((souris_x > 676) && (souris_x < 716) && (souris_y > 19) && (souris_y < 36)) {
		return 14;
	}
	else if ((souris_x > 716) && (souris_x < 754) && (souris_y > 19) && (souris_y < 36)) {
		return 15;
	}
	else if ((souris_x > 754) && (souris_x < 792) && (souris_y > 19) && (souris_y < 36)) {
		return 16;
	}
	else if ((souris_x > 792) && (souris_x < 830) && (souris_y > 19) && (souris_y < 36)) {
		return 17;
	}
	else if ((souris_x > 830) && (souris_x < 868) && (souris_y > 19) && (souris_y < 36)) {
		return 18;
	}
	else if ((souris_x > 868) && (souris_x < 905) && (souris_y > 19) && (souris_y < 36)) {
		return 19;
	}
	else if ((souris_x > 905) && (souris_x < 955) && (souris_y > 19) && (souris_y < 36)) {
		return 20;
	}
	else {
		return -1;
	}
}







void JeuSDL::MenuSDL()
{
	bool r = false;
	SDL_Event evenements;
	bool quitter = false;
	MusicSDL();


	//--------------------------------------------------




	//---------------------------------------------


	//r = afficherInit();
	r = menu.loadTexture("data/cavalerie-france.xcf",renderer);
	SDL_RenderClear(renderer);
	if(r == true)
	{
		//menu.loadTexture("data/cavalerie-france.xcf",renderer);
		menu.draw(renderer,0,0);
	}
	else
	{
		cout << "probleme avec la sdl" <<endl;
		//quitterSDL();
	}

	SDL_RenderPresent(renderer);

// Tant qu'un evenement quitter n'a pas ete declenche
while (!quitter) {
	// Tant qu'il reste des evenements a traiter dans la file d'evenement
	while (SDL_PollEvent( &evenements )) {	// Recuperation d'un evenement

		// Selon le type d'evenement
		switch (evenements.type) {
			// Si on apuuie sur le bouton X de la fenetre
			case SDL_QUIT:
				quitter = true;
				break;

			// Si on appuie sur une touche du clavier
			case SDL_KEYDOWN:
				// Selon la touche appuiee
				switch (evenements.key.keysym.scancode) {
					// Si on appuie sur la touche Escape
					case SDL_SCANCODE_ESCAPE:
						quitter = true;
						break;

					// Pour les autres touches non gerees par le switch
					default:
						break;
				}
				break;

			// Evenements de la souris
			// Au mouvement de la souris
			case SDL_MOUSEBUTTONUP:
				//SDL_GetMouseState(&souris_x, &souris_y);
				souris_x=evenements.button.x;
				souris_y=evenements.button.y;
				cout << "souris_x : " << souris_x << "	,	souris_y : " << souris_y << endl;

				if(souris_x > 41 && souris_x < 177 && souris_y > 596 && souris_y < 637) //pour le bouton quitter
				{
					quitter = true ;
				}

				if(souris_x > 41 && souris_x < 134 && souris_y > 541 && souris_y < 580)
				{
					Aide.loadTexture("data/fond-gris.xcf",renderer);
					SDL_RenderClear(renderer);
					Aide.draw(renderer,0,0);
					SDL_RenderPresent(renderer);


				}
				if(souris_x > 550 && souris_x < 770 && souris_y > 580 && souris_y < 590)
				{
					SDL_RenderClear(renderer);
					menu.draw(renderer,0,0);
					SDL_RenderPresent(renderer);
				}
				if(souris_x > 42 && souris_x < 322 && souris_y > 463 && souris_y < 509) {
					boucleJeu();
					quitter = true;
				}

			break;
		}
	}
}
}


void JeuSDL::MusicSDL()
{
		Mix_Music *musique = NULL;
	  SDL_Init(SDL_INIT_AUDIO);
  	Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024 );
		musique = Mix_LoadMUS("data/music_menu.ogg"); //Charge le son a l'adresse indiqué
		if(musique == NULL)  //Vérifie si le son est ok.
 	 	{
			cout << "Erreur lors du chargement du son"<<endl;
			cerr<< Mix_GetError()<<endl;
	 	}

		Mix_PlayMusic(musique,-1); // on joue notre son en boucle */

}