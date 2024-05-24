Comment utiliser les commandes GIT pour les branches :

Branche 'main' : Branche principale ou se trouve le code a jour
qui a ete merge lorsque l'on est sur que notre code fonctionne.

Branche 'develop' : Contient le code en developpement. A partir
de la branche 'develop' on retrouve toute les sous branches en
fonction de la tache.

SOUS-BRANCHES :
Branche 'config_parse' : Elie
Branche 'CGI' : Imane
Branche 'req_res' : Alex


**ENTRER DANS UNE BRANCHE**
Si on est dans le main :
	git checkout develop
Si on est dans 'develop' :
	git checkout feature/NOM BRANCHE (config_parse/CGI/req_res)

		SORTIR D UNE BRANCHE
	git checkout NOM BRANCHE
