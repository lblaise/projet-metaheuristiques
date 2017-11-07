# projet-metaheuristiques

Etude de cas : Couverture connexe minimum dans les r�seaux de capteurs

Le fichier main.txt contient les fonctions correspondant � l'algorithme principal de r�solution du probl�me :
 - int solvePb(int size, int Rcapt, int Rcom) correspond � notre impl�mentation de la m�thode taboue pour r�soudre une instance du probl�me de taille size x size, de rayon de captation Rcapt et de rayon de communication Rcom, et retourne la valeur de la meilleure grille rencontr�e au cours de l'exploration.
 - void solveAll() imprime les valeurs de toutes les instances du probl�me � traiter.

La classe Problem (fichiers Problem.h et Problem.cpp) contient toutes les donn�es du probl�me, et les m�thodes utilis�es dans le fichier main.cpp pour calculer une borne sup�rieure du probl�me.
 - Les attributs de la classe correspondent aux dimensions du probl�me, aux rayons de captation et de communication, au nombre de capteurs pr�sents sur la grille, au nombre de cibles non capt�es sur la grille, au nombre de composantes connexes de capteurs dans la grille, � la r�partition des capteurs sur la grille, et au nombre de fois que chaque cible de la grille est capt�e.
 - Les diff�rentes m�thodes permettent de calculer la fonction objectif du probl�me, et d'actualiser tous les attributs lorsque l'on remplace une grille par une grille voisine, en ajoutant ou en retirant des capteurs: le nombre de capteurs, de positions non couvertes, de composantes connexes etc. doivent �tre mises � jour.

Le fichier defines.h contient des valeurs num�riques de certaines donn�es du probl�me : taille de la grille, rayons de captation et de communication, les param�tres de la fonction objectif, le nombre d'it�rations � r�aliser dans l'algorithme tabou, la longueur de la liste taboue, le crit�re pour appliquer la diversification ou l'intensification dans l'algorithme.

L�a Blaise
Cyrille Vessaire
