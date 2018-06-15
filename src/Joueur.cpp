#include "Joueur.h"



using namespace std;

Joueur::Joueur(const string & nom, const string & couleur_joueur)
{
	nom_joueur = nom;
	couleur = couleur_joueur;
}

string Joueur::getCouleurJoueur() const
{
	return couleur;
}

void Joueur::setCouleurJoueur(string couleur_joueur)
{
	couleur = couleur_joueur;
}

unsigned int Joueur::getNbRegions()const
{
	return tab_region.size();
}

unsigned int Joueur::getNbRegionsInit(){
	return nb_regions_initial;
}

void Joueur::setNbRegionsInit(unsigned int nb_Regions)
{
	assert (nb_Regions >= 0);
	nb_regions_initial = nb_Regions;
}

unsigned int Joueur::getNbRegiments() const
{
	return nb_regiments;
}

void Joueur::setNbRegiments (unsigned int nb_Regiments)
{
	assert (nb_Regiments >= 0);
	nb_regiments = nb_Regiments;
}

void Joueur::setnom_joueur(string nom)
{
	nom_joueur=nom;
}

string Joueur::getnom_joueur()
{
	return nom_joueur;
}

vector<Region*> & Joueur::getRegionsJoueur()
{
	return tab_region;
}

void Joueur::testRegressionJoueur()
{
	assert(getCouleurJoueur()=="");
	assert(getNbRegions()== 0);
	assert(getNbRegiments()==0);
	assert(getnom_joueur()=="");

	setCouleurJoueur("Rouge");
	setNbRegionsInit(3);
	setNbRegiments(40);
	setnom_joueur("jessai");

	assert(getCouleurJoueur()=="Rouge");
	assert(getNbRegions()==3);
	assert(getNbRegiments()==40);
	assert(getnom_joueur()=="jessai");

}
