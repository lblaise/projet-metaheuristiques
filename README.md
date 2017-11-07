# projet-metaheuristiques

Etude de cas : Couverture connexe minimum dans les réseaux de capteurs

Le fichier main.txt contient les fonctions correspondant à l'algorithme principal de résolution du problème :
 - int solvePb(int size, int Rcapt, int Rcom) correspond à notre implémentation de la méthode taboue pour résoudre une instance du problème de taille size x size, de rayon de captation Rcapt et de rayon de communication Rcom, et retourne la valeur de la meilleure grille rencontrée au cours de l'exploration.
 - void solveAll() imprime les valeurs de toutes les instances du problème à traiter.

La classe Problem (fichiers Problem.h et Problem.cpp) contient toutes les données du problème, et les méthodes utilisées dans le fichier main.cpp pour calculer une borne supérieure du problème.
 - Les attributs de la classe correspondent aux dimensions du problème, aux rayons de captation et de communication, au nombre de capteurs présents sur la grille, au nombre de cibles non captées sur la grille, au nombre de composantes connexes de capteurs dans la grille, à la répartition des capteurs sur la grille, et au nombre de fois que chaque cible de la grille est captée.
 - Les différentes méthodes permettent de calculer la fonction objectif du problème, et d'actualiser tous les attributs lorsque l'on remplace une grille par une grille voisine, en ajoutant ou en retirant des capteurs: le nombre de capteurs, de positions non couvertes, de composantes connexes etc. doivent être mises à jour.

Le fichier defines.h contient des valeurs numériques de certaines données du problème : taille de la grille, rayons de captation et de communication, les paramètres de la fonction objectif, le nombre d'itérations à réaliser dans l'algorithme tabou, la longueur de la liste taboue, le critère pour appliquer la diversification ou l'intensification dans l'algorithme.

Léa Blaise
Cyrille Vessaire
