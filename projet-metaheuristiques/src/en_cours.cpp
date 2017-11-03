int checkconnexitechange(int x, int y, bool suppr, int cc){ //x, y coordonnées du point ajouté / supprimé, suppr = vrai si suppression, faux si ajout d'un capteur
// cc représente le nombre de composante connexe
    int8_t Dcc = 0; // Delta nbr de composantes connexes
    int8_t nbSensorsInRange = 0; // contiendra le nbr de capteur a portée de captation
    vector<pair<int8_t, int8_t>> liste; // liste contient l'ensemble des voisins du point (x,y)

// Suppression d'un capteur
    if (suppr){
        getneighbour(liste, nbSensorsInRange); // modifie liste et nbSensorsInRange
        if (nbSensorsInRange == 0) // suppression d'un capteur sans voisin à portée de communication
        {
            Dcc = -1;
            return Dcc;
        }
        if (nbSensorsInRange > 1) // suppression d'un capteur avec plusieurs voisins à portée de communication
        {
            bool Verif[nbSensorsInRange] = {false}; // tableau vérifiant le fait d'avoir pris en compte tous les voisins
            bool verificationtotale = false; // bool représentant le nombre
            int i = 0;
            while(!verificationtotale)
            {
                checkconnex(liste, Verif, verificationtotale, Dcc, i, nbSensorsInRange); // donne le nombre de composante connexe lorsqu'il n'y a pas (x,y)
            }
            return Dcc;
        }
        else // suppression d'un capteur avecc un seul voisin à portée de communication
        {
           Dcc = 0;
           return Dcc;
        }
    }

//ajout d'un capteur
    if (cc>1 && !suppr)
    {
        getneighbour(liste, nbSensorsInRange);

        if (nbSensorsInRange == 0) //ajout d'un capteur sans voisin
        {
            Dcc = 1;
            return Dcc;
        }
        if (nbSensorsInRange > 1) // suppression d'un capteur avec plusieurs voisins à portée de communication
        {
            bool Verif[nbSensorsInRange] = {false}; // tableau vérifiant le fait d'avoir pris en compte les voisins (Verif[i]= true si liste[i] a été parcouru)
            bool verificationtotale = true; // bool vérfiant que l'on ait bien pris en compte tous les voisins
            int i = 0;
            while(verificationtotale)
            {
                checkconnex(liste, *Verif, &verificationtotale, &Dcc, &i, nbSensorsInRange); // donne le nombre de composante connexe concerner lorsqu'il n'y a pas le point (x,y)
            }
            Dcc = -1 * Dcc; // opposé du cas suppression de capteur : on a diminué le nombre de composantes connexes
            return Dcc;
        }
        else //ajout d'un capteur avec un seul voisin
        {
            Dcc = 0;
            return Dcc;
        }

    }
    if (cc == 1 && !suppr)
    {
      isthereneighbour(&nbSensorsInRange); // s'arrête dès quelle trouve un voisin
      if (nbSensorsInRange == 0) //ajout d'un capteur sans voisin
      {
          Dcc = 1;
          return Dcc;
      }
      else //ajout d'un capteur avec des voisins
      {
          Dcc = 0;
          return Dcc;
      }
    }
    else // cas ou il n'y a pas de capteur (i.e. cc = 0)
    {
       Dcc = 1;
    }
};

//
void Problem::checkconnex(vector<pair<int, int>> liste, bool *Verif, bool &verificationtotale, int &Dcc, int &i, int nbSensorsInRange)
{
    vector<vector<bool> > visited(dimensions.first, vector<bool>(dimensions.second, false));

    bool verification = true;
    for (int j = 0; j < nbSensorsInRange, j++)
    {
        verification = verification && Verif[j];
    }

    for (int j = 0; j < nbSensorsInRange, j++)
    {
        if (!Verif[j])
        {
            if(!browseconnexcomponant(liste, &visited, j, *Verif)) //retourne vrai si il existe une composante connexe prenant en compte l'ensemble des points non encore visités de la liste depuis la j-ième à la dernière composante
            {
                Dcc += 1;
            }
        }
    }

};

bool browseconnexcomponant(vector<pair<int, int>> liste, vector<vector<bool> > &visited, int j, bool *Verif) //retourne vrai si il existe une composante connexe prenant en compte l'ensemble des points non encore visités de la liste depuis la j-ième à la dernière composante
{
    queue<pair<int, int> > Q;
    Q.push(liste[j]);

    bool verification = true;

    for (int i = j; i < nbSensorsInRange, i++)
    {
        verification = verification && Verif[i];
    }

    //A la fin, le booléen verification vaudra true si il existe une composante connexe prenant en compte l'ensemble des points non encore visités de la liste depuis la j-ième à la dernière composante

    while (!Q.empty() && !verification)
    {
        int rr = Q.front().first;
        int cc = Q.front().second;
        Q.pop();
        if (!visited[rr][cc])
        {
            visited[rr][cc] = true;
            for (int i = j; i < nbSensorsInRange i++)
            {
                if(rr == liste[i].first && cc == liste[i].second)
                {
                    Verif[i] = true;
                    verification = true;

                    for (int k = j; k < nbSensorsInRange, k++)
                    {
                        verification = verification && Verif[k];
                    }
                }
            }

            for (int i = max(0, rr - Rcom); i < min(dimensions.first, rr + Rcom + 1); i++)
            {
                for (int j = max(0, cc - Rcom); j < min(dimensions.second, cc + Rcom + 1); j++)
                {
                    if (((rr - i)*(rr - i) + (cc - j)*(cc - j) <= Rcom*Rcom) && grid[i][j]) {

                        Q.push(make_pair(i, j));
                    }
                }
            }
        }
    }

    return verification;

};

void getneighbour(vector<pair<int, int>> &liste, int &nbSensorsInRange)
{
    for (int i = 0; i<= Rcom; i++) // boucle sur un carré de taille 2 Rcom pour vérifier la présence de voisins
    {
        for(int j = min(y-i, 0); j <= max(y+i, getDimensions().second), j++)
        {
            for (int k = min(x - Rcom, 0); k <= max(x + Rcom, getDimensions().first ), k++)
            {
                if(grid[j][k] && (j-y)*(j-y)+(k-x)*(k-x)<= Rcom*Rcom)
                {
                    nbSensorsInRange +=1;
                    currentpair = make_pair(j,k);
                    liste.append(currentpair);
                    }
                }
              }
          }
    }
};

void isthereneighbour(int &nbSensorsInRange) 
{
    for (int i = 0; i<= Rcom; i++)  // boucle sur un carré de taille 2 Rcom pour vérifier la présence de voisins
    {
        for(int j = min(y-i, 0); j <= max(y+i, getDimensions().second), j++)
        {
            for (int k = min(x - Rcom, 0); k <= max(x + Rcom, getDimensions().first ), k++)
            {
                if(grid[j][k] && (j-y)*(j-y)+(k-x)*(k-x)<= Rcom*Rcom)
                {
                    nbSensorsInRange +=1;
                    return;
                }
            }
        }
    }
};
