#include "Region.h"

using namespace std;

Region::Region()
{
	nom_region = "";
	couleur_joueur = "";
	nb_unites = 1;
}

Region::Region(const string & nom, unsigned int nb_unite, const string & couleur_region)
{
	assert(nb_unite >= 0);
	nom_region = nom;
	couleur_joueur = couleur_region;
	nb_unites = nb_unite;
}


string Region::getNomRegion () const
{
	return nom_region;
}

void Region::setNomRegion (const string & nom)
{
	nom_region = nom;
}


string Region::getCouleurRegion () const
{
	return couleur_joueur;
}

void Region::setCouleurRegion (const string & couleur_region)
{
	couleur_joueur = couleur_region;
}



unsigned int Region::getNbUnite () const
{
	return nb_unites;
}

void Region::setNbUnite (const unsigned int & nb_unite)
{
	assert(nb_unite >= 0);
	nb_unites = nb_unite;
}

vector<Region*> & Region::getTabFrontaliers ()
{
	return frontaliers;
}

void Region::setTabFrontaliers (const vector<Region*> & tab_frontalier)
{
	frontaliers = tab_frontalier;
}

bool Region::estFrontalier (const Region & voisin) const
{
	for (unsigned int i = 0; i < frontaliers.size(); i++) {
		if (frontaliers[i]->getNomRegion() == voisin.getNomRegion())
		{
			return true;
		}
	}
	return false;
}



void Region::ajouterFrontalier (const Region & voisin)
{
	frontaliers.push_back( (Region*) &voisin );
}



void Region::testRegressionRegion()
{
	//test du constructeur et des get
	Region();
	assert(getNomRegion()=="");
	assert(getNbUnite()==1);
	assert(getCouleurRegion()=="");

	Region("essai",5,"Rouge");
	assert(getNomRegion()=="essai");
	assert(getNbUnite()==5);
	assert(getCouleurRegion()=="Rouge");

	//test des set
	setNbUnite(50);
	setCouleurRegion("Bleu");
	setNomRegion("essai2");

	assert(getNomRegion()=="essai2");
	assert(getNbUnite()==50);
	assert(getCouleurRegion()=="Bleu");
}
