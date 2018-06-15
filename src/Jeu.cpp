#include "Jeu.h"
#include "Combat.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <fstream>
#include <cassert>
#include <vector>

using namespace std;


Jeu::Jeu()
{
	nb_joueur=0;
//	terrain = NULL;
}


// Regarde si l'entier a est présent dans le tableau Tabou de taille NbJoueur
bool estValide(int a, int tabou[], int nbJoueur){
	for(int i = 0; i < nbJoueur; i++)
			if( tabou[i] == a)
				return true;
	return false;
}


unsigned int Jeu::getNbJoueur(){
	return nb_joueur;
}

void Jeu::setNbJoueur(unsigned int nbj){
	nb_joueur = nbj;
}

vector<Joueur>& Jeu::getTabJoueur(){
	return tab_joueur;
}

void Jeu::setTabJoueur( vector<Joueur> tabj){
	tab_joueur = tabj;
}


void Jeu::lancerJeu()
{
	initJeu();
	afficheTerrainTXT();

		// Initialisation des choix de couleurs possibles
	vector<string> liste_couleurs;
	liste_couleurs.push_back("Rouge");
	liste_couleurs.push_back("Bleu");
	liste_couleurs.push_back("Vert");
	liste_couleurs.push_back("Orange");
	liste_couleurs.push_back("Blanc");
	liste_couleurs.push_back("Violet");
	// liste_couleurs = {"Rouge","Bleu","Vert","Orange","Blanc","Violet"};

		// Initialisation des joueurs
	unsigned int n;
	cout << "Mettre le nombre de joueurs : ";
	do
	{
		cin >> n;
		if (n < 2)
		{
			cout << "/!\\ Minimum 2 joueurs : ";
		}
		if (n > 5)
		{
			cout << "/!\\ Maximum 5 joueurs : ";
		}
	} while (n < 2 || n > 5);
	nb_joueur = n;
	cout << endl;

	// Saisie d'un nouveau joueur
	for (unsigned int i = 0; i < nb_joueur; i++)
	{
		string nom;
		unsigned int num_couleur;

		cout << "Joueur " << i+1 << ", entrez votre nom : ";
		cin >> nom;

		cout << "Choisissez une couleur..." << endl;
		for (unsigned int j = 0; j < liste_couleurs.size(); j++)
		{
			cout << liste_couleurs[j] << "	" << j+1 << endl;
		}
		do
		{
			cin >> num_couleur;
			if (num_couleur < 1 || num_couleur > liste_couleurs.size())
			{
				cout << "/!\\ Donnez une valeur entre 1 et " << liste_couleurs.size() << " : ";
			}
		} while (num_couleur < 1 || num_couleur > liste_couleurs.size());
		num_couleur--;

		// Ajout d'un nouveau joueur avec les valeurs saisies
		tab_joueur.push_back( Joueur( nom, liste_couleurs[num_couleur] ) );

		// Traitement du nombre de troupes et du nombre de regions
		switch(nb_joueur)
		{
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
		cout << tab_joueur[i].getnom_joueur() << " (Joueur " << i+1 << ") dispose de " << tab_joueur[i].getNbRegiments() << " troupes et " << tab_joueur[i].getNbRegionsInit() << " regions" << endl;

		// Suppression de la couleur choisie dans la liste des choix de couleur
		liste_couleurs.erase(liste_couleurs.begin() + num_couleur);

		cout << endl;
	}

	srand((unsigned int) time(NULL));

		// Liste de tous les territoires
	vector<Region*> territoires;
	for (unsigned int i = 0; i < terrain.getTabPays().size(); i++)
	{
		for (unsigned int j = 0; j < terrain.getTabPays()[i]->getTabRegions().size(); j++)
		{
			territoires.push_back( terrain.getTabPays()[i]->getTabRegions()[j] );
		}
	}

		// Repartition aleatoire des territoires
	for (unsigned int i = 0; i < nb_joueur; i++)
	{
		for (unsigned int j = 0; j < tab_joueur[i].getNbRegionsInit(); j++)
		{
			unsigned int random_val = rand() % territoires.size();
			tab_joueur[i].getRegionsJoueur().push_back( territoires[ random_val ] );
			territoires[ random_val ]->setCouleurRegion( tab_joueur[i].getCouleurJoueur() );
			territoires.erase( territoires.begin() + random_val );
		}
	}
	// Il reste eventuellement des territoires non attribués pour une partie à 4 ou 5 joueurs

		// Repartition des regiments pour chaque joueur
	for (unsigned int i = 0; i < nb_joueur; i++)
	{

		tab_joueur[i].setNbRegiments( tab_joueur[i].getNbRegiments() - tab_joueur[i].getNbRegions() );
		cout << "	" << tab_joueur[i].getnom_joueur() << " (Joueur " << i+1 << "), repartissez vos troupes ! (" << tab_joueur[i].getNbRegiments() << " unites restantes)" << endl<<endl;

		bool continuer = true;
		unsigned int nb, num_territoire;
		while (continuer)
		{
			cout << "Choisissez parmi les territoires suivants :" << endl;
			for (unsigned int j = 0; j < tab_joueur[i].getNbRegions(); j++)
			{
				cout << "	" << j+1 << " " << tab_joueur[i].getRegionsJoueur()[j]->getNomRegion() << " (" << tab_joueur[i].getRegionsJoueur()[j]->getNbUnite() << " unites)" << endl;
			}
			do
			{
				cin >> num_territoire;
				if (num_territoire < 1 || num_territoire > tab_joueur[i].getNbRegions())
				{
					cout << "/!\\ Erreur, recommencez : ";
				}
			} while (num_territoire < 1 || num_territoire > tab_joueur[i].getNbRegions());
			cout << "Combien de regiments voulez-vous ajouter sur " << tab_joueur[i].getRegionsJoueur()[num_territoire-1]->getNomRegion() << " ? (" << tab_joueur[i].getNbRegiments() << " restants)" << endl;
			do
			{
				cin >> nb;
				if (nb > tab_joueur[i].getNbRegiments())
				{
					cout << "/!\\ Maximum " << tab_joueur[i].getNbRegiments() << " unites : ";
				}
			} while (nb > tab_joueur[i].getNbRegiments());
			tab_joueur[i].getRegionsJoueur()[num_territoire-1]->setNbUnite( tab_joueur[i].getRegionsJoueur()[num_territoire-1]->getNbUnite() + nb );
			if ((((int) tab_joueur[i].getNbRegiments()) - ((int) nb)) <= 0)
			{
				continuer = false;
			}
			tab_joueur[i].setNbRegiments( tab_joueur[i].getNbRegiments() - nb );
		}
		cout << endl;
	}

		// Affichage recap pour chaque joueur
	for (unsigned int i = 0; i < nb_joueur; i++)
	{
		cout << tab_joueur[i].getnom_joueur() << " occupe les territoires suivants :" << endl;
		for (unsigned int j = 0; j < tab_joueur[i].getNbRegions(); j++)
		{
			cout << "	" << tab_joueur[i].getRegionsJoueur()[j]->getNomRegion() << " (" << tab_joueur[i].getRegionsJoueur()[j]->getNbUnite() << " unites)" << endl;
		}
		cout << endl << endl;
	}


	while( !finPartie() ){
		cout <<endl<< "Phase Renfort !" <<endl;
		phaseRenfort();
		cout <<endl<< "Phase Attaque !" <<endl;
		for(unsigned int i = 0; i < nb_joueur; i++){

			phaseAttaque(tab_joueur[i]);
		}
		cout <<endl<< "Phase Manoeuvre !" << endl;
		for(unsigned int i = 0; i < nb_joueur; i++){
			phaseManoeuvre(tab_joueur[i]);
		}
	}
}


bool Jeu::finPartie()
{
	unsigned int i;

	for(i=0;i<nb_joueur;i++)
	{
		if(tab_joueur[i].getNbRegions() == 42)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}


void Jeu::afficherAide()
{
	int a;
	cout <<"Aide du jeu"<<endl;

	do
	{
		cout <<"Les phases de jeu : 1 " <<endl;
		cout <<"Les renforts : 2 " <<endl;
		cout <<"Les combats : 3 " <<endl;
		cout <<"Les manoeuvres : 4"<<endl;
		cout <<"La condition de victoire : 5 " <<endl;
		cin >> a;
	}
	while((a<1)||(a>4));

	switch(a)
	{
		case 1 : cout <<"Un joueur peut proceder à 3 actions distinctes dans l'ordre suivant"<<endl;
						 cout <<"1-Phase de renfort "<<endl;
						 cout <<"2-Phase de combat " <<endl;
						 cout <<"3-Phase de manoeuvre " <<endl;
		break;

		case 2 : cout <<"La phase de renfort permet d'avoir des renforts en fonction du nombre de regions controles et de pays controle en entier " <<endl;
		break;

		case 3 : cout <<"La phase de combat permet d'engager le combat contre un autre joueur qui se situe a votre frontiere " <<endl;
		break;

		case 4 : cout <<"La phase de manoeuvre permet de faire un déplacement de troupe d'une region a une autre region frontaliere" <<endl;
		break;

		case 5 : cout <<"La condition de victoire est le contrôle totale de la map"<<endl;
		break;

		default : exit(1);
		break;
	}
}


//--------------------------------------------------------------------------------------
//Phase Renfort

void Jeu::phaseRenfort()
{
	unsigned int num, i, k, total_renfort;
	//unsigned int bonus_pays;

	for(i=0;i<nb_joueur;i++)
	{
		unsigned int b = (tab_joueur[i].getNbRegions()/3);
		if( b >= 3 )
			total_renfort = b;
		else
			total_renfort = 3;
		//rajouter les renforts en fonction du pays
	tab_joueur[i].setNbRegiments(total_renfort);

	while(total_renfort > 0){
		cout << " Joueur " << tab_joueur[i].getCouleurJoueur() << ", choisis une region où placer les renforts (" << total_renfort << " restants) " <<endl;
		for(k =0; k < tab_joueur[i].getNbRegions(); k++){
		cout << k+1 << " : " << tab_joueur[i].getRegionsJoueur()[k]->getNomRegion() << " (" << tab_joueur[i].getRegionsJoueur()[k]->getNbUnite() << " unites)" <<endl;
		}
		cin >> k;
		while( k <=0 || k > tab_joueur[i].getNbRegions()+1){
			cout << "Saisie incorrecte, recommencez : ";
			cin >> k;
		}
		do{
			cout << "Choisis le nombre de troupes à placer entre 0 et " << total_renfort << " : ";
			cin >> num;}
		while(num < 0 || num > total_renfort);

		tab_joueur[i].getRegionsJoueur()[k-1]->setNbUnite(tab_joueur[i].getRegionsJoueur()[k-1]->getNbUnite() + num);
		total_renfort = total_renfort - num;

		cout << endl;
	}
	cout <<endl;
	}
}


//--------------------------------------------------------------------------------------
//Phase Attaque

void Jeu::phaseAttaque(Joueur& j)
{
	// On laisse le choix pour savoir si le joueur veut attaquer
	int a ;
	cout<<"Joueur " << j.getCouleurJoueur()<< ", voulez-vous attaquer une region ? " <<endl;
	do
	{
		cout <<"oui : 1"<<endl;
		cout <<"non : 2" <<endl;
		cin >> a ;
	}while((a<1)||(a>2));


	if(a == 1)
	{
		Region region_attaquant, region_defenseur;
  		vector <Region> tab_regions_frontalieres;
		unsigned int num;

		// On choisit la region d'attaque
		do{
    		cout << "Choisis la region depuis laquelle tu veux attaquer (tu dois avoir au moins 2 troupes dans la region) :" << endl;
    		for( unsigned int i = 0; i < j.getNbRegions(); i++)
      		{
				cout << i+1 << " : " << j.getRegionsJoueur()[i]->getNomRegion() << " (" << j.getRegionsJoueur()[i]->getNbUnite() << " unites)" <<endl;
     		}
    		cin >> num;}
  		while((num < 1) || (num > j.getNbRegions()) || (j.getRegionsJoueur()[num-1]->getNbUnite() < 2));

		region_attaquant = *(j.getRegionsJoueur()[num-1]);

		// On crée le tableau des regions frontalieres qui ne sont pas de la même couleur que celle du joueur
  		for (unsigned int i = 0; i < region_attaquant.getTabFrontaliers().size(); i++)
  		{
  			tab_regions_frontalieres.push_back(*(region_attaquant.getTabFrontaliers()[i]));
  		}
  		for( unsigned int i = 0; i < tab_regions_frontalieres.size(); i++)
   	 		if( tab_regions_frontalieres[i].getCouleurRegion() == j.getCouleurJoueur())
      			tab_regions_frontalieres.erase( tab_regions_frontalieres.begin() + i );

		// Parmi ce tableau, on demande au joueur, quelle region il veut attaquer
		do{
    		cout << " Choisis la region frontaliere que tu souhaites attaquer : " <<endl;
    		for( unsigned int i = 0; i < tab_regions_frontalieres.size(); i++)
      		{
				cout << i+1 << " : " << tab_regions_frontalieres[i].getNomRegion() << " (" << tab_regions_frontalieres[i].getNbUnite() << " unites)" << endl;
      		}
    		cin >> num;}
  		while( num < 1 || num > tab_regions_frontalieres.size() );

  		region_defenseur = tab_regions_frontalieres[num-1];

		// On récupère le joueur qui va être attaqué via la couleur de la région attaquée
		Joueur *j2;
		for(unsigned int i = 0; i < nb_joueur; i++)
			if(region_defenseur.getCouleurRegion() == tab_joueur[i].getCouleurJoueur())
				j2 = &(tab_joueur[i]);

		// On lance le combat !
		Combat batailleEpique(j, *j2, region_attaquant, region_defenseur);
		batailleEpique.maj_troupes(region_attaquant, region_defenseur, j, *j2);
	}

	cout <<endl << "Affichage des regions après la bataille : " <<endl;
	for( unsigned int i =0; i < j.getNbRegions(); i++){
		cout << i+1 << " : " << j.getRegionsJoueur()[i]->getNomRegion() << " (" << j.getRegionsJoueur()[i]->getNbUnite() << " unites)" <<endl;
	}
	cout <<endl;
}


//--------------------------------------------------------------------------------------
//Phase Manoeuvre

void Jeu::phaseManoeuvre(Joueur& j){

  Region region_depart, region_arrivee;
  unsigned int num;
  vector <Region> tab_regions_frontalieres;


	// Debug => cout << "GetNbRegions : " << j.getNbRegions() <<endl;
  do{
    cout << "Joueur " << j.getCouleurJoueur() << ", choisis la region de depart : " << endl;
    for( unsigned int i = 0; i < j.getNbRegions(); i++)
      {
      //Problème incompréhensible avec getNomRegion ... Car c'est la même qu'à la fin de l'attaque 
	cout << i+1 << " : " << j.getRegionsJoueur()[i]->getNomRegion() << " (" << j.getRegionsJoueur()[i]->getNbUnite() << " unites)" <<endl;
      }
    cin >> num;}
  while(num < 1 || num > j.getNbRegions() );

  region_depart = *(j.getRegionsJoueur()[num-1]);
  for (unsigned int i = 0; i < region_depart.getTabFrontaliers().size(); i++)
  {
  	tab_regions_frontalieres.push_back(*(region_depart.getTabFrontaliers()[i]));
  }
  for( unsigned int i = 0; i < tab_regions_frontalieres.size(); i++)
    if( tab_regions_frontalieres[i].getCouleurRegion() != j.getCouleurJoueur())
      tab_regions_frontalieres.erase( tab_regions_frontalieres.begin() + i );

  do{
    cout << " Choisis la region frontaliere ou doivent se rendre les troupes : " <<endl;
    for( unsigned int i = 0; i < tab_regions_frontalieres.size(); i++)
      {
	cout << i+1 << " : " << tab_regions_frontalieres[i].getNomRegion()  << " (" << tab_regions_frontalieres[i].getNbUnite() << " unites)" <<endl;
      }
    cin >> num;}
  while( num < 1 || num > tab_regions_frontalieres.size() );

  region_arrivee = tab_regions_frontalieres[num-1];

  unsigned int max_troupes = region_depart.getNbUnite()-1;
  do{
    cout << "Choisis le nombre de troupes à déplacer entre 0 et " << max_troupes << " : ";
    cin >> num;}
  while(num < 0 || num > max_troupes);

  region_depart.setNbUnite(region_depart.getNbUnite() - num);
  region_arrivee.setNbUnite(region_arrivee.getNbUnite() + num);

  cout <<endl << "Affichage des regions après la manoeuvre : " <<endl;
	for( unsigned int i =0; i < j.getNbRegions(); i++){
		cout << i+1 << " : " << j.getRegionsJoueur()[i]->getNomRegion() << " (" << j.getRegionsJoueur()[i]->getNbUnite() << " unites)" <<endl;
	}
	cout <<endl;

//	cout << endl << "Nom Region 21 => " << j.getRegionsJoueur()[20]->getNomRegion() << endl << "Nb unites 21 => " << j.getRegionsJoueur()[20]->getNbUnite() <<endl;
}


//--------------------------------------------------------------------------------------


void Jeu::initJeu()
{
	terrain.initTerrain();
}

void Jeu::afficheTerrainTXT()
{
	for (unsigned int i = 0; i < terrain.getTabPays().size(); i++)
	{
		cout << terrain.getTabPays()[i]->getNomPays() << " (" << terrain.getTabPays()[i]->getRegimentsSupp() << ")" <<  endl;
		for (unsigned int j = 0; j < terrain.getTabPays()[i]->getTabRegions().size(); j++)
		{
			cout << "	" << terrain.getTabPays()[i]->getTabRegions()[j]->getNomRegion() << endl;
			cout << "	(";
			for (unsigned int k = 0; k < terrain.getTabPays()[i]->getTabRegions()[j]->getTabFrontaliers().size(); k++)
			{
				cout << terrain.getTabPays()[i]->getTabRegions()[j]->getTabFrontaliers()[k]->getNomRegion();
				if (k != terrain.getTabPays()[i]->getTabRegions()[j]->getTabFrontaliers().size()-1)
				{
					cout << ", ";
				}
			}
			cout << ")" << endl << endl;
		}
		cout << endl << endl;
	}
}
