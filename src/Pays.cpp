#include "Pays.h"

using namespace std;

Pays::Pays()
{
	nb_regions = 0;
	nom_pays = "";
	regiments_supp = 0;
}

Pays::Pays(const string & nom, unsigned int nbregions, unsigned int nbregiments)
{
	assert(nbregions >= 0);
	assert(nbregiments >= 0);
	nb_regions = nbregions;
	nom_pays = nom;
	regiments_supp = nbregiments;
}

Pays::~Pays()
{
	for (unsigned int i = 0; i < tab_region.size(); i++) {
		delete tab_region[i];
	}
}




string Pays::getNomPays () const
{
	return nom_pays;
}

void Pays::setNomPays (const string & nom)
{
	nom_pays = nom;
}



vector<Region*> & Pays::getTabRegions ()
{
	return tab_region;
}

void Pays::setTabRegions (const vector<Region*> & tabRegion)
{
	tab_region = tabRegion;
}



unsigned int Pays::getNbRegions () const
{
	return nb_regions;
}

void Pays::setNbRegions (unsigned int nbregions)
{
	nb_regions = nbregions;
}



unsigned int Pays::getRegimentsSupp() const
{
	return regiments_supp;
}



bool Pays::controlePays (const string & couleur_joueur) const
{
	for (unsigned int i = 0; i < getNbRegions(); i++)
	{
		if (!(tab_region[i]->getCouleurRegion() == couleur_joueur))
		{
			return false;
		}
	}
	return true;
}



void Pays::ajouterRegion (const Region & reg)
{
	tab_region.push_back( (Region*) &reg );
}



void Pays::testRegressionPays()
{
	//test des get et du constructeur
	Pays();
	assert(getNbRegions()==0);
	assert(getNomPays()=="");
	assert(getRegimentsSupp()==0);

	Pays("essai",5,4);
	assert(getNbRegions()==5);
	assert(getNomPays()=="essai");
	assert(getRegimentsSupp()==4);
}
