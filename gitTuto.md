**Comment utiliser les commandes GIT pour les branches :**

Branche 'main' : Branche principale ou se trouve le code a jour
qui a ete merge lorsque l'on est sur que notre code fonctionne.

Branche 'develop' : Contient le code en developpement. A partir
de la branche 'develop' on retrouve toute les sous branches en
fonction de la tache.

**SOUS-BRANCHES :**
Branche 'config_parse' : Elie
Branche 'CGI' : Imane
Branche 'req_res' : Alex


**ENTRER DANS UNE BRANCHE**
	git checkout NOM BRANCHE (develop ou main)

**ENTRER DANS UNE SOUS-BRANCHE**
	git checkout feature/NOM BRANCHE (config_parse/CGI/req_res)

**SORTIR D'UNE BRANCHE OU D'UNE SOUS-BRANCHE**
	git checkout NOM BRANCHE

**PUSH TON TRAVAIL DANS DEVELOP QUAND TU ES DANS TA SOUS-BRANCHE** >> Push dans ta sous-branche avant!!
	git checkout develop
	git merge feature/NOM BRANCHE(config_parse/CGI/req_res)

**PULL LE TRAVAIL QUI A ETE PUSH DANS DEVELOP**
	git pull origin develop

**SUPPRIMER UNE BANCHE**
	git branch -d feature/NOM BRANCHE(config_parse/CGI/req_res)
