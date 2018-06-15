#include "Combat.h"
#include "Joueur.h"
#include "Region.h"


using namespace std;

	Combat::Combat(){
		
	}
	
	Combat::Combat(Joueur &joueur_a, Joueur& joueur_d, Region region_a, Region region_d){
		attaquant = joueur_a;
		defenseur = joueur_d;
		region_attaquant = region_a;
		region_defenseur = region_d;
	}
		
	Combat::~Combat(){
	
	}
	
	Joueur Combat::get_attaquant() const{
		return attaquant;
	}
	
	void Combat::set_attaquant(Joueur att){
				attaquant = att;
	}
	
	Joueur Combat::get_defenseur() const{
		return defenseur;
	}

	void Combat::set_defenseur(Joueur def){
		defenseur = def;
	}

	Region Combat::get_region_attaquant() const{
		return region_attaquant;
	}

	void Combat::set_region_attaquant(Region region_a){
		region_attaquant = region_a;
	}
	
	Region Combat::get_region_defenseur() const{
		return region_defenseur;
	}

	void Combat::set_region_defenseur(Region region_d){
		region_defenseur = region_d;
	}
	
	int unDe(int nombre){
		return rand()%nombre +1;
	}
	
	bool Combat::bataille(unsigned int nb_troupes_attaquant, unsigned int nb_troupes_defenseur){
		int res_attaquant = 0, res_defenseur = 0;
		
		for(unsigned int i = 0; i < nb_troupes_attaquant; i++)
			res_attaquant += unDe(6);
		for(unsigned int i = 0; i < nb_troupes_defenseur; i++)
			res_defenseur += unDe(6);
			
		cout << "L'attaquant a fait un jet à " << res_attaquant <<endl;
		cout << "Le défenseur a fait un jet à " << res_defenseur <<endl;
		
		if(res_attaquant > res_defenseur)
			return true;
		else
			return false;
	}

	int pos_region_tabJoueur(Joueur& j, Region& region_defenseur){
		int i =0;
		
		// On incrémente i tant qu'on ne trouve pas la region. 42 = arrêt d'urgence
		while(j.getRegionsJoueur()[i]->getNomRegion() != region_defenseur.getNomRegion() && i < 42){
					i++;
				}
		return i;
	}

	void maj_region(Joueur& jatt, Joueur& jdef, Region& region_defenseur, Region& region_attaquant, int nbTroupesMaxAttaquant, int transfert = -1){
		int a;
		string c_att = jatt.getCouleurJoueur();
		if (transfert == -1) {
			do{
				cout << "Joueur " << c_att << ", choisis un nombre d'unites a transfere dans la region conquise (entre 1 et " << nbTroupesMaxAttaquant-1 << ") : ";
				cin >> a;}
			while(a < 1 || a >= nbTroupesMaxAttaquant);
		}
		else {
			a = transfert;
		}
		// On change la couleur de la région et on ajoute le nombre de troupes indiqués par l'attaquant
	//	cout << "DEBUGmajregion AVANT => RegionDef.NbUnite = " << region_defenseur.getNbUnite() <<endl;
	//	cout << "DEBUGmajregion AVANT => RegionAtt.NbUnite = " << region_attaquant.getNbUnite() <<endl;
		region_defenseur.setCouleurRegion(c_att);
		region_defenseur.setNbUnite(a);
		region_attaquant.setNbUnite(region_attaquant.getNbUnite() - a);
		
	//	cout << "DEBUGmajregion APRES => RegionDef.NbUnite = " << region_defenseur.getNbUnite() <<endl;
	//	cout << "DEBUGmajregion APRES => RegionAtt.NbUnite = " << region_attaquant.getNbUnite() <<endl;
				
		// On cherche la position de la region dans le tableau du joueur defenseur
		int positionDef = pos_region_tabJoueur(jdef, region_defenseur);
		
		int positionAtt = pos_region_tabJoueur(jatt, region_attaquant);
	/*	cout << "IIICCCIIIII => " << jatt.getRegionsJoueur()[positionAtt]->getNomRegion() << " : " << jatt.getRegionsJoueur()[positionAtt]->getNbUnite() << " unites" <<endl;*/
		jatt.getRegionsJoueur().erase(jatt.getRegionsJoueur().begin() + positionAtt);
		jatt.getRegionsJoueur().push_back(&region_attaquant);

		jdef.getRegionsJoueur().erase(jdef.getRegionsJoueur().begin() + positionDef);
		jatt.getRegionsJoueur().push_back(&region_defenseur);
}
	
	
	void Combat::maj_troupes(Region& region_attaquant, Region& region_defenseur, Joueur& jatt, Joueur& jdef, const int & att, const int & def, const int & transfert){
		int a, d, unite_att = region_attaquant.getNbUnite(), unite_def = region_defenseur.getNbUnite();
		
		// On initialise les couleurs des joueurs dans des variables
		string c_att = jatt.getCouleurJoueur();
		string c_def = jdef.getCouleurJoueur();
		
		if (att == -1) {
			// On choisit le nombre des troupes pour l'attaque et la défense
			do{
			cout << "Joueur " << c_att << ", choisis un nombre d'unites pour l'offensive (entre 0 et " << (unite_att>3? 3:unite_att) << ") : ";
			cin >> a;}
			while((a<0) || (a>3) || (a>unite_att)); 
			cout <<endl;
			do{
			cout << "Joueur " << c_def << ", choisis un nombre d'unites pour la defense (entre 1 et " << (unite_def>2? 2:unite_def) << ") : ";
			cin >> d;}
			while((d<1) || (d>2) || (d>unite_def));
		}
		else {
			a = att;
			d = def;
		}
		// On lance la bataille et on vérifie qui gagne
		if( !bataille(a,d) ){ 		
			// Si le defenseur gagne, on diminue les troupes de l'attaquant
			cout << " Le défenseur l'emporte !" << endl <<endl;		
			region_attaquant.setNbUnite(unite_att - a +1);
			int positionAtt = pos_region_tabJoueur(jatt, region_attaquant);
			jatt.getRegionsJoueur().erase(jatt.getRegionsJoueur().begin() + positionAtt);
			jatt.getRegionsJoueur().push_back(&region_attaquant);
		}
		else {						
			// Si l'attaquant gagne, on doit changer la region du defenseur de couleur, l'enlever de ces regions et l'ajouter dans celles de l'attaquant
			cout << "Victoire !" <<endl<<endl;			
			region_defenseur.setNbUnite(unite_def - d);	//On décrémente les troupes dans la region defenseur
			int positionDef = pos_region_tabJoueur(jdef, region_defenseur);
			jdef.getRegionsJoueur().erase(jdef.getRegionsJoueur().begin() + positionDef);
			jdef.getRegionsJoueur().push_back(&region_defenseur);
			
			unsigned int limite = region_defenseur.getNbUnite();
			if(limite <= 0){	// Si le nombre de troupes est <= 0, on doit faire le changement de region
		/*	cout << "DEBUGmajtroupes AVANT => RegionDef.NbUnite = " << region_defenseur.getNbUnite() <<endl;
		cout << "DEBUGmajtroupes AVANT => RegionAtt.NbUnite = " << region_attaquant.getNbUnite() <<endl;*/
				if (att == -1) {
					maj_region(jatt, jdef, region_defenseur, region_attaquant, unite_att);
				}
				else {
					maj_region(jatt, jdef, region_defenseur, region_attaquant, unite_att, transfert);
				}

/*cout << "DEBUGmajtroupes APRES => RegionDef.NbUnite = " << region_defenseur.getNbUnite() <<endl;
		cout << "DEBUGmajtroupes APRES => RegionAtt.NbUnite = " << region_attaquant.getNbUnite() <<endl;*/

			}
		}
	}
