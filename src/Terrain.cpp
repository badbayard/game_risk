#include "Terrain.h"
#include "Joueur.h"
#include "Pays.h"
#include "Region.h"

using namespace std;

Terrain::Terrain()
{
	dim_x = 0;
 	dim_y = 0;
}

Terrain::Terrain(unsigned int dimensionX, unsigned int dimensionY)
{
	assert(dimensionX >= 0);
	assert(dimensionY >= 0);
	dim_x = dimensionX;
 	dim_y = dimensionY;
	unsigned int i, j;
/*
	for(i=0;i<=dim_x;i++)
	{
		for(j=0;j<=dim_y;j++)
		{
			cout << "*";
		}
		cout << endl;
	}
*/

//terrain
 const char carte[18][80]={ " *******************   *********  *********************************          ",
	                        " ******************     ******     **    **************************          ",
							"  ***************        * *      **    ***************************          ",
							"   ***************           *   **     ****************************         ",
							"     ************           **        ******************************         ",
							"       *********  **         ********************************************    ",
							"	       *****           *************************************************  ",
							"          ***           **************************************************   ",
							"           *                     ***    ***********************************  ",
							"          *****                   **	   ********************************** ",
							"         *******                   *     *********************************** ",
							"        *********                         ********************************** ",
							"        **********        ***************************************************",
							"        **********     *******************            *******          ****  ",
							"        *********     ********************             ******            **  ",
							"         ********      *****************               ******            **  ",
							"         *******         **************                 ****             **  ",
							"           ****            ***********                   **               *  ",
 };

 for (i=0;i<18;i++)
		{
			for(j=0;j<80;j++)
			{
			cout<<carte[i][j];
		  }
			cout <<endl;
		}
}

Terrain::~Terrain()
{
	dim_x = 0;
	dim_y = 0;
	for (unsigned int i = 0; i < tab_pays.size(); i++) {
		delete tab_pays[i];
	}
}

unsigned int Terrain::getdim_x() const
{
	return dim_x;
}

void Terrain::setdim_x(unsigned int dimensionX)
{
	assert(dimensionX >= 0);
	dim_x = dimensionX;
}

unsigned int Terrain::getdim_y() const
{
	return dim_y;
}

void Terrain::setdim_y(unsigned int dimensionY)
{
	assert(dimensionY >= 0);
	dim_y = dimensionY;
}

vector<Pays*> & Terrain::getTabPays ()
{
	return tab_pays;
}

void Terrain::initTerrain ()
{
	// Amerique du Nord
	Pays *north_america = new Pays("Amerique du Nord",9,5);
	Region *alaska = new Region("Alaska");
	Region *alberta = new Region("Alberta");
	Region *central_america = new Region("Amerique centrale");
	Region *eastern_us = new Region("Etats de l'Est");
	Region *greenland = new Region("Groenland");
	Region *northwest_territory = new Region("Territoires du Nord-Ouest");
	Region *ontario = new Region("Ontario");
	Region *quebec = new Region("Quebec");
	Region *western_us = new Region("Etats de l'Ouest");
	(*north_america).ajouterRegion(*alaska);
	(*north_america).ajouterRegion(*alberta);
	(*north_america).ajouterRegion(*central_america);
	(*north_america).ajouterRegion(*eastern_us);
	(*north_america).ajouterRegion(*greenland);
	(*north_america).ajouterRegion(*northwest_territory);
	(*north_america).ajouterRegion(*ontario);
	(*north_america).ajouterRegion(*quebec);
	(*north_america).ajouterRegion(*western_us);

	// Asie
	Pays *asia = new Pays("Asie",12,7);
	Region *afghanistan = new Region("Afghanistan");
	Region *china = new Region("Chine");
	Region *india = new Region("Inde");
	Region *irkutsk = new Region("Tchita");
	Region *japan = new Region("Japon");
	Region *kamchatka = new Region("Kamtchatka");
	Region *middle_east = new Region("Moyen-Orient");
	Region *mongolia = new Region("Mongolie");
	Region *siam = new Region("Siam");
	Region *siberia = new Region("Siberie");
	Region *ural = new Region("Oural");
	Region *yakutsk = new Region("Yakoutie");
	(*asia).ajouterRegion(*afghanistan);
	(*asia).ajouterRegion(*china);
	(*asia).ajouterRegion(*india);
	(*asia).ajouterRegion(*irkutsk);
	(*asia).ajouterRegion(*japan);
	(*asia).ajouterRegion(*kamchatka);
	(*asia).ajouterRegion(*middle_east);
	(*asia).ajouterRegion(*mongolia);
	(*asia).ajouterRegion(*siam);
	(*asia).ajouterRegion(*siberia);
	(*asia).ajouterRegion(*ural);
	(*asia).ajouterRegion(*yakutsk);

	// Amerique du Sud
	Pays *south_america = new Pays("Amerique du Sud",4,2);
	Region *argentina = new Region("Argentine");
	Region *brazil = new Region("Bresil");
	Region *peru = new Region("Perou");
	Region *venezuela = new Region("Venezuela");
	(*south_america).ajouterRegion(*argentina);
	(*south_america).ajouterRegion(*brazil);
	(*south_america).ajouterRegion(*peru);
	(*south_america).ajouterRegion(*venezuela);

	// Europe
	Pays *europe = new Pays("Europe",7,5);
	Region *great_britain = new Region("Grande-Bretagne");
	Region *iceland = new Region("Islande");
	Region *northern_eu = new Region("Europe du Nord");
	Region *scandinavia = new Region("Scandinavie");
	Region *southern_eu = new Region("Europe du Sud");
	Region *ukraine = new Region("Ukraine");
	Region *western_eu = new Region("Europe occidentale");
	(*europe).ajouterRegion(*great_britain);
	(*europe).ajouterRegion(*iceland);
	(*europe).ajouterRegion(*northern_eu);
	(*europe).ajouterRegion(*scandinavia);
	(*europe).ajouterRegion(*southern_eu);
	(*europe).ajouterRegion(*ukraine);
	(*europe).ajouterRegion(*western_eu);

	// Afrique
	Pays *africa = new Pays("Afrique",6,3);
	Region *congo = new Region("Congo");
	Region *east_africa = new Region("Afrique de l'Est");
	Region *egypt = new Region("Egypte");
	Region *madagascar = new Region("Madagascar");
	Region *north_africa = new Region("Afrique du Nord");
	Region *south_africa = new Region("Afrique du Sud");
	(*africa).ajouterRegion(*congo);
	(*africa).ajouterRegion(*east_africa);
	(*africa).ajouterRegion(*egypt);
	(*africa).ajouterRegion(*madagascar);
	(*africa).ajouterRegion(*north_africa);
	(*africa).ajouterRegion(*south_africa);

	// Oceanie
	Pays *australia = new Pays("Oceanie",4,2);
	Region *eastern_au = new Region("Australie Orientale");
	Region *indonesia = new Region("Indonesie");
	Region *new_guinea = new Region("Nouvelle-Guinee");
	Region *western_au = new Region("Australie Occidentale");
	(*australia).ajouterRegion(*eastern_au);
	(*australia).ajouterRegion(*indonesia);
	(*australia).ajouterRegion(*new_guinea);
	(*australia).ajouterRegion(*western_au);




	// Frontieres Amerique du Nord
	(*alaska).ajouterFrontalier(*northwest_territory);
	(*alaska).ajouterFrontalier(*kamchatka);
	(*alberta).ajouterFrontalier(*northwest_territory);
	(*alberta).ajouterFrontalier(*ontario);
	(*alberta).ajouterFrontalier(*western_us);
	(*central_america).ajouterFrontalier(*western_us);
	(*central_america).ajouterFrontalier(*eastern_us);
	(*central_america).ajouterFrontalier(*venezuela);
	(*eastern_us).ajouterFrontalier(*central_america);
	(*eastern_us).ajouterFrontalier(*western_us);
	(*eastern_us).ajouterFrontalier(*ontario);
	(*eastern_us).ajouterFrontalier(*quebec);
	(*greenland).ajouterFrontalier(*northwest_territory);
	(*greenland).ajouterFrontalier(*ontario);
	(*greenland).ajouterFrontalier(*quebec);
	(*greenland).ajouterFrontalier(*iceland);
	(*northwest_territory).ajouterFrontalier(*alaska);
	(*northwest_territory).ajouterFrontalier(*alberta);
	(*northwest_territory).ajouterFrontalier(*ontario);
	(*northwest_territory).ajouterFrontalier(*greenland);
	(*ontario).ajouterFrontalier(*northwest_territory);
	(*ontario).ajouterFrontalier(*alberta);
	(*ontario).ajouterFrontalier(*western_us);
	(*ontario).ajouterFrontalier(*eastern_us);
	(*ontario).ajouterFrontalier(*quebec);
	(*ontario).ajouterFrontalier(*greenland);
	(*quebec).ajouterFrontalier(*ontario);
	(*quebec).ajouterFrontalier(*eastern_us);
	(*quebec).ajouterFrontalier(*greenland);
	(*western_us).ajouterFrontalier(*alberta);
	(*western_us).ajouterFrontalier(*ontario);
	(*western_us).ajouterFrontalier(*eastern_us);
	(*western_us).ajouterFrontalier(*central_america);

	// Frontieres Asie
	(*afghanistan).ajouterFrontalier(*india);
	(*afghanistan).ajouterFrontalier(*middle_east);
	(*afghanistan).ajouterFrontalier(*ural);
	(*afghanistan).ajouterFrontalier(*china);
	(*afghanistan).ajouterFrontalier(*ukraine);
	(*china).ajouterFrontalier(*afghanistan);
	(*china).ajouterFrontalier(*india);
	(*china).ajouterFrontalier(*mongolia);
	(*china).ajouterFrontalier(*siam);
	(*china).ajouterFrontalier(*siberia);
	(*china).ajouterFrontalier(*ural);
	(*india).ajouterFrontalier(*afghanistan);
	(*india).ajouterFrontalier(*china);
	(*india).ajouterFrontalier(*middle_east);
	(*india).ajouterFrontalier(*siam);
	(*irkutsk).ajouterFrontalier(*kamchatka);
	(*irkutsk).ajouterFrontalier(*mongolia);
	(*irkutsk).ajouterFrontalier(*siberia);
	(*irkutsk).ajouterFrontalier(*yakutsk);
	(*japan).ajouterFrontalier(*kamchatka);
	(*japan).ajouterFrontalier(*mongolia);
	(*kamchatka).ajouterFrontalier(*alaska);
	(*kamchatka).ajouterFrontalier(*irkutsk);
	(*kamchatka).ajouterFrontalier(*japan);
	(*kamchatka).ajouterFrontalier(*mongolia);
	(*kamchatka).ajouterFrontalier(*yakutsk);
	(*middle_east).ajouterFrontalier(*southern_eu);
	(*middle_east).ajouterFrontalier(*egypt);
	(*middle_east).ajouterFrontalier(*east_africa);
	(*middle_east).ajouterFrontalier(*india);
	(*middle_east).ajouterFrontalier(*afghanistan);
	(*middle_east).ajouterFrontalier(*ukraine);
	(*mongolia).ajouterFrontalier(*china);
	(*mongolia).ajouterFrontalier(*irkutsk);
	(*mongolia).ajouterFrontalier(*japan);
	(*mongolia).ajouterFrontalier(*kamchatka);
	(*mongolia).ajouterFrontalier(*siberia);
	(*siam).ajouterFrontalier(*indonesia);
	(*siam).ajouterFrontalier(*china);
	(*siam).ajouterFrontalier(*india);
	(*siberia).ajouterFrontalier(*china);
	(*siberia).ajouterFrontalier(*irkutsk);
	(*siberia).ajouterFrontalier(*mongolia);
	(*siberia).ajouterFrontalier(*ural);
	(*siberia).ajouterFrontalier(*yakutsk);
	(*ural).ajouterFrontalier(*ukraine);
	(*ural).ajouterFrontalier(*afghanistan);
	(*ural).ajouterFrontalier(*china);
	(*ural).ajouterFrontalier(*siberia);
	(*yakutsk).ajouterFrontalier(*irkutsk);
	(*yakutsk).ajouterFrontalier(*kamchatka);
	(*yakutsk).ajouterFrontalier(*siberia);

	// Frontieres Europe
	(*great_britain).ajouterFrontalier(*iceland);
	(*great_britain).ajouterFrontalier(*northern_eu);
	(*great_britain).ajouterFrontalier(*scandinavia);
	(*great_britain).ajouterFrontalier(*western_eu);
	(*iceland).ajouterFrontalier(*great_britain);
	(*iceland).ajouterFrontalier(*scandinavia);
	(*iceland).ajouterFrontalier(*greenland);
	(*northern_eu).ajouterFrontalier(*scandinavia);
	(*northern_eu).ajouterFrontalier(*great_britain);
	(*northern_eu).ajouterFrontalier(*western_eu);
	(*northern_eu).ajouterFrontalier(*southern_eu);
	(*northern_eu).ajouterFrontalier(*ukraine);
	(*scandinavia).ajouterFrontalier(*great_britain);
	(*scandinavia).ajouterFrontalier(*iceland);
	(*scandinavia).ajouterFrontalier(*northern_eu);
	(*scandinavia).ajouterFrontalier(*ukraine);
	(*southern_eu).ajouterFrontalier(*egypt);
	(*southern_eu).ajouterFrontalier(*north_africa);
	(*southern_eu).ajouterFrontalier(*middle_east);
	(*southern_eu).ajouterFrontalier(*northern_eu);
	(*southern_eu).ajouterFrontalier(*ukraine);
	(*southern_eu).ajouterFrontalier(*western_eu);
	(*ukraine).ajouterFrontalier(*afghanistan);
	(*ukraine).ajouterFrontalier(*middle_east);
	(*ukraine).ajouterFrontalier(*ural);
	(*ukraine).ajouterFrontalier(*northern_eu);
	(*ukraine).ajouterFrontalier(*scandinavia);
	(*ukraine).ajouterFrontalier(*southern_eu);
	(*western_eu).ajouterFrontalier(*north_africa);
	(*western_eu).ajouterFrontalier(*great_britain);
	(*western_eu).ajouterFrontalier(*northern_eu);
	(*western_eu).ajouterFrontalier(*southern_eu);

	// Frontieres Amerique du Sud
	(*argentina).ajouterFrontalier(*brazil);
	(*argentina).ajouterFrontalier(*peru);
	(*brazil).ajouterFrontalier(*argentina);
	(*brazil).ajouterFrontalier(*peru);
	(*brazil).ajouterFrontalier(*venezuela);
	(*brazil).ajouterFrontalier(*north_africa);
	(*peru).ajouterFrontalier(*argentina);
	(*peru).ajouterFrontalier(*brazil);
	(*peru).ajouterFrontalier(*venezuela);
	(*venezuela).ajouterFrontalier(*brazil);
	(*venezuela).ajouterFrontalier(*peru);
	(*venezuela).ajouterFrontalier(*central_america);

	// Frontieres Afrique
	(*congo).ajouterFrontalier(*east_africa);
	(*congo).ajouterFrontalier(*north_africa);
	(*congo).ajouterFrontalier(*south_africa);
	(*east_africa).ajouterFrontalier(*middle_east);
	(*east_africa).ajouterFrontalier(*congo);
	(*east_africa).ajouterFrontalier(*egypt);
	(*east_africa).ajouterFrontalier(*madagascar);
	(*east_africa).ajouterFrontalier(*north_africa);
	(*east_africa).ajouterFrontalier(*south_africa);
	(*egypt).ajouterFrontalier(*southern_eu);
	(*egypt).ajouterFrontalier(*middle_east);
	(*egypt).ajouterFrontalier(*east_africa);
	(*egypt).ajouterFrontalier(*north_africa);
	(*madagascar).ajouterFrontalier(*east_africa);
	(*madagascar).ajouterFrontalier(*south_africa);
	(*north_africa).ajouterFrontalier(*brazil);
	(*north_africa).ajouterFrontalier(*southern_eu);
	(*north_africa).ajouterFrontalier(*western_eu);
	(*north_africa).ajouterFrontalier(*congo);
	(*north_africa).ajouterFrontalier(*east_africa);
	(*north_africa).ajouterFrontalier(*egypt);
	(*south_africa).ajouterFrontalier(*congo);
	(*south_africa).ajouterFrontalier(*east_africa);
	(*south_africa).ajouterFrontalier(*madagascar);

	// Frontieres Oceanie
	(*eastern_au).ajouterFrontalier(*new_guinea);
	(*eastern_au).ajouterFrontalier(*western_au);
	(*indonesia).ajouterFrontalier(*siam);
	(*indonesia).ajouterFrontalier(*new_guinea);
	(*indonesia).ajouterFrontalier(*western_au);
	(*new_guinea).ajouterFrontalier(*eastern_au);
	(*new_guinea).ajouterFrontalier(*indonesia);
	(*new_guinea).ajouterFrontalier(*western_au);
	(*western_au).ajouterFrontalier(*eastern_au);
	(*western_au).ajouterFrontalier(*indonesia);
	(*western_au).ajouterFrontalier(*new_guinea);

	ajouterPays(*north_america);
	ajouterPays(*asia);
	ajouterPays(*south_america);
	ajouterPays(*europe);
	ajouterPays(*africa);
	ajouterPays(*australia);

}

void Terrain::ajouterPays (const Pays & pays)
{
	tab_pays.push_back( (Pays*) &pays );
}


void Terrain::testRegressionTerrain()
{
	assert( dim_x >= 0);
	assert( dim_y >= 0);

	Terrain(50,55);
	//test du constructeur de la classe avec aussi les get
	assert(getdim_x() == 50);
	assert(getdim_y() == 50);

	//test des set
	setdim_x(20);
	setdim_y(60);

	assert(getdim_x() == 20);
	assert(getdim_y() == 60);

	//test pour les pays


}
