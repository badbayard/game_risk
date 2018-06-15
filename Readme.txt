

Qu'est ce que c'est ?
---------------------

	C'est un programme réalisé dans le cadre de l'UE de LIFAP4 qui est de concevoir un jeux vidéo.
	Le jeu est une variante du jeu de plateau Risk. Le jeu est basé sur la stratégie, le but du jeu et de conquérir les régions et les pays,
	et le but principal  du jeu est basée sur la règle de la domination (conquérir toute la carte ou être le joueur le plus puissants ou influant)


Liste des fichiers nécessaires à la compilation :
-------------------------------------------------

	Jeu.h
	Terrain.h
	Combat.h
	JeuSDL.h
	Joueur.h
	Region.h
	Pays.h
	main.cpp
	mainSDL.cpp
	Jeu.cpp
	Terrain.cpp
	Combat.cpp
	JeuSDL.cpp
	Joueur.cpp
	Region.cpp
	Pays.cpp

	il faut disposer de la SDL 2.0 et de libsdl2-mixer-2.0-0 sur l'ordinateur pour pouvoir compiler le programme.
	
Compilation :
-------------

	Pour compiler le programme, il faut utiliser le Makefile qui est à la racine du dossier.
	
	~$ cd p1512256-projet-ap4
	make -f risk.make

	La commande make clean permet d'enlever les .o pour permettre de relancer une bonne compilation
	
	
Exécution :
-----------

	Une fois la compilation effectuée, cela crée 2 exécutables dans le dossier bin : test.out testSDL.out
	- "test.out" exécute le code du fichier main.cpp qui lance le jeu en mode texte.
	- "testSDL.out" exécute le code du fichier mainSDL.cpp qui lance le jeu en mode graphique.
	
	Pour les exécuter, il suffit de taper la commande pour le mode texte :
		~/p1512256-projet-ap4 ./bin/test.out
		
	Puis, pour "testSDL.out" :
			~/p1512256-projet-ap4 ./bin/testSDL.out
			

Documentation :
---------------

Un fichier de génération de documentation est fourni dans le dossier doc.

	Après avoir installé doxygen, taper la commande :
		~/p1512256-projet-ap4 doxygen doc/image.doxy
		
	Pour afficher la documentation sur un navigateur :
	~/p1512256-projet-ap4  [nom_du_navigateur] doc/html/index.html
		
	
	Un diagramme des modules réalisé sous Dia est consultable dans le dossier doc sous le nom "diagramme_classes.dia"

On trouve aussi dans le dossier doc le cahier des charges ainsi que un diagramme de Gantt, ainsi que l'image du diagramme des classes avant et après, ainsi que le
pdf de la présentation du projet.



Contacts :
----------

	Pour tout renseignements, nous contacter :
	Julien CADIER (11510421) : julien.cadier@etu.univ-lyon1.fr
	Yannis HUTT (11408376)	 : yannis.hutt@etu.univ-lyon1.fr
	Randy ANDRIAMARO (11512256) : randy.andriamaro@etu.univ-lyon1.fr

Forge :
-------
nom : [p1512256] Projet AP4
https://forge.univ-lyon1.fr/hg/p1512256-projet-ap4 
Yannis Hutt: p11408376
Julier Cadier: Julien Cadier
Randy Andriamaro: Randy Andriamaro


