#include "Combat.h"
#include "Joueur.h"
#include "Pays.h"
#include "Region.h"
#include "Terrain.h"
#include "Jeu.h"
#include "JeuSDL.h"

#include <iostream>

using namespace std;

int main ()
{
	JeuSDL jeu;
	if ( !jeu.afficherInit() ) {
		jeu.quitterSDL();
		exit(1);
	}
	jeu.MenuSDL();
	jeu.quitterSDL();
	return 0;
}
