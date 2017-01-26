#include <iostream>
#include <ctype.h>
#include <vector>
#include <stdlib.h>
#include <ctime>

using namespace std;

//Définition des variables NDIMia et grilles pour le fichier ia.cpp
const int NDIMia = 8;
typedef int Grille[NDIMia][NDIMia];

//Défintion de la structure CaseIa pour le fichier ia.cpp
struct CaseIa
{
  int x;
  int y;
};

// Valeurs minimum et maximum pour l'evaluation
const int ValMin = -30000;
const int ValMax = 30000;

// profondeur de l'évaluation
int profondeur = 0;
// profondeur maximum, 3 est le meilleur compromis
int profondeurMax = 3;

/**
  Détecte les cases jouables sur le tableau
  @param[in, out] grtmp - la grille temporaire du jeu
*/
void casesJouablesIa(Grille &grtmp)
{
  int i, j, k, l;

  for(i=0; i<NDIMia; i++)
    for(j=0; j<NDIMia; j++)
      if( grtmp[i][j]==1 || grtmp[i][j]==2 )
        for(k=-1; k<2; k++)
          for(l=-1; l<2; l++)
            if(grtmp[i+k][j+l]==0)
              if(j+l!=-1 && j+l!=8)
                grtmp[i+k][j+l] = 3;
}


/**
  Test si un carré est présent sur la grille
	@param[in] gr - Une grille
	@param[out] player - Un player
*/
int carre(Grille &grtmp, const int player)
{
  int i, j, gagnant=0, adversaire=3;

  for(i=0; i<NDIMia; i++)
    for(j=0; j<NDIMia; j++)
      if(grtmp[i][j]==1 || grtmp[i][j]==2)
        if(grtmp[i][j]==grtmp[i][j-1] && grtmp[i][j]==grtmp[i+1][j] && grtmp[i][j]==grtmp[i+1][j-1])
        {
          gagnant = grtmp[i][j];
        }

  if(player==1)
    adversaire=2;
  if(player==2)
    adversaire=1;

  if(gagnant == player)
    return player;
  else if(gagnant == adversaire)
    return adversaire;
  else
    return 0;
}

int ligne(Grille &grtmp, const int player)
{
  int x, y, aux, naux, gagnant=0, adversaire = 0;

  if(player==1)
    adversaire=2;
  if(player==2)
    adversaire=1;

//test de victoire sur les colones
  for(x=0; x<4; x++)
    for(y=0; y<NDIMia; y++)
    {
      if(grtmp[x][y]==1 || grtmp[x][y]==2)
      {
        aux = grtmp[x][y];

        switch(aux)
        {
          case 1 :
            naux = 2;
            break;
          case 2 :
            naux = 1;
            break;
          default :
            break;
        }

        if(grtmp[x+1][y]==naux && grtmp[x+2][y]==naux && grtmp[x+3][y]==naux && grtmp[x+4][y]==aux)
          gagnant = aux;
      }
    }

//test de victoire sur les lignes
    for(x=0; x<NDIMia; x++)
      for(y=0; y<4; y++)
      {
        if(grtmp[x][y]==1 || grtmp[x][y]==2)
        {
          aux = grtmp[x][y];

          switch(aux)
          {
            case 1 :
              naux = 2;
              break;
            case 2 :
              naux = 1;
              break;
            default :
              break;
          }

          if(grtmp[x][y+1]==naux && grtmp[x][y+2]==naux && grtmp[x][y+3]==naux && grtmp[x][y+4]==aux)
            gagnant = aux;
        }
      }

//test victoire diagonale droite
    for(x=0; x<4; x++)
      for(y=0; y<4; y++)
      {
        if(grtmp[x][y]==1 || grtmp[x][y]==2)
        {
          aux = grtmp[x][y];

          switch(aux)
          {
            case 1 :
              naux = 2;
              break;
            case 2 :
              naux = 1;
              break;
            default :
              break;
          }

          if(grtmp[x+1][y+1]==naux && grtmp[x+2][y+2]==naux && grtmp[x+3][y+3]==naux && grtmp[x+4][y+4]==aux)
            gagnant = aux;
        }
      }

//test victoire diagonale gauche
    for(x=0; x<4; x++)
      for(y=4; y<NDIMia; y++)
      {
        if(grtmp[x][y]==1 || grtmp[x][y]==2)
        {
          aux = grtmp[x][y];

          switch(aux)
          {
            case 1 :
              naux = 2;
              break;
            case 2 :
              naux = 1;
              break;
            default :
              break;
          }

          if(grtmp[x+1][y-1]==naux && grtmp[x+2][y-2]==naux && grtmp[x+3][y-3]==naux && grtmp[x+4][y-4]==aux)
            gagnant = aux;
        }
      }

    if(gagnant == player)
      return player;
    else if(gagnant == adversaire)
      return adversaire;
    else
      return 0;
}

/**
  Détecte une victoire
  @param[in] grtmp - la grille du jeu
  @param[in] player - le player pour lequel il faut tester la victoire
  @return score - le score découlant du nombres de victoires sur ce tableau
*/
int victoire(Grille &grtmp, const int player)
{
  int score = 0, adversaire, tmpc=0, tmpl=0;

  if(player==1)
    adversaire=2;
  if(player==2)
    adversaire=1;

  tmpc = carre(grtmp, player);
  tmpl = ligne(grtmp, player);

  if(tmpc==player)
    score+=1;
  if(tmpl==player)
    score+=1;
  if(tmpc==adversaire)
    score-=1;
  if(tmpl==adversaire)
    score-=1;

  return score;
}

/**
  Détection des carrés presque formés
  @param[in] grtmp - la grille du jeu
  @param[in] player - le player pour lequel il faut rechercher un "presque carré"
  @return score 2 - le score découlant de la meilleure combinaison de carrés sur la grille
*/
int presqueCarre(Grille &grtmp, const int player)
{
  int i, j, gagnant=0, adversaire, cptplayer=0, cptadversaire=0, score=0, score2=ValMin;
  int k[4]={0, 0, 1, 1};
  int l[4]={0, 1, 0, 1};

  if(player==1)
    adversaire==2;
  if(player==2)
    adversaire==1;

  for(i=0; i<NDIMia-1; i++)
    for(j=0; j<NDIMia-1; j++)
    {
      for(int r=0; r<4; r++)
      {
      if(grtmp[i+k[r]][j+l[r]]==player)
        cptplayer++;
      if(grtmp[i+k[r]][j+l[r]]==adversaire)
        cptadversaire++;
      }

      score = cptplayer*200 - cptadversaire*250;

      if(score>score2)
        score2 = score;

      cptplayer=0;
      cptadversaire=0;
    }

  return score2;
}

char symbole(int j)
{
	const char symboles[] = {'.','x','o', '@'};
	return symboles[j];
}

/**
  Détermine un score en fonction des victoires ou potentielles victoires sur la Grille
  @param[in] grtmp - la grille du jeu
  @param[in] player - le player pour lequel il faut déterminer un score
  @return score - le score obtenu par le player
*/
int eval(Grille &grtmp, const int player)
{
  int score = 0;
  int tmp = 0;

  tmp = victoire(grtmp, player);
  score = score + tmp*1000;

  score = score + presqueCarre(grtmp, player);

  return score;
}

/**
  Créé un vecteur contenant toutes les coordonnées jouables
  @param[in] grtmp - la grille du jeu
  @param[in, out] at - vecteur contenant les coordonnées jouables
*/
void coordsCoupsJouables(Grille &grtmp, vector <CaseIa> &at)
{
  int k=0;
  at.resize(0);

  //vecteur at contenant toutes les positions jouables
  for(int i=0; i<NDIMia; i++)
    for(int j=0; j<NDIMia; j++)
      if(grtmp[i][j]==3)
      {
        at.push_back(CaseIa());
        at[k].x = i;
        at[k].y = j;
        k++;
      }
}

/**
  Copie les valeurs d'une grille dans une deuxième
  @param[in] gr1 - grille a copier
  @param[in, out] gr2 - grille a remplir
*/
void copieGrille(Grille &gr1, Grille &gr2)
{
  //gr1 grille a copier
  //gr2 la copie

  for(int j=0; j<NDIMia; j++)
    for(int k=0; k<NDIMia; k++)
      gr2[j][k]=gr1[j][k];
}

/**
  Rempli la grille grtmp avec les probabilités de coups des prochains tours pour l'évaluation
  @param[in] grtmp - grille du jeu
  @param[out] x,y - les coordonnées optimale a jouer au prochain tours
  @param[in] player, adversaire - le player qui doit jouer le prochain tour et son adversaire
*/
void calcIA(Grille &grtmp, int &x, int &y, const int player, const int adversaire)
{
  vector <CaseIa> P1;
  vector <CaseIa> P2;
  vector <CaseIa> P3;
  Grille cp1;
  Grille cp2;
  Grille cp3;
  int i, j, k, l, r, score=0, max=ValMin;

  coordsCoupsJouables(grtmp, P1);
  copieGrille(grtmp, cp1);
  for(i = 0; i < P1.size(); i++)
  {
    copieGrille(cp1, grtmp);
    grtmp[(P1[i].x)][(P1[i].y)] = player;

    //bonus si victoire au prochain tour
    if(victoire(grtmp, player) >= 1)
    {
      score = ValMax;
    }

    //bonus si empèche victoire adversaire au prochain tour
    grtmp[P1[i].x][P1[i].y] = adversaire;
    if(victoire(grtmp, adversaire) >= 1)
    {
      score += 4000;
    }
    grtmp[(P1[i].x)][(P1[i].y)] = player;

    casesJouablesIa(grtmp);
    coordsCoupsJouables(grtmp, P2);

    copieGrille(grtmp, cp2);
    for(j = 0; j < P2.size(); j++)
    {
      copieGrille(cp2, grtmp);
      grtmp[(P2[j].x)][(P2[j].y)] = adversaire;

      casesJouablesIa(grtmp);
      coordsCoupsJouables(grtmp, P3);

      copieGrille(grtmp, cp3);
      for(k = 0; k < P3.size(); k++)
      {
        copieGrille(cp3, grtmp);
        grtmp[(P3[k].x)][(P3[k].y)] = player;

        score = score + eval(grtmp, player);

        if(score > max)
        {
          max = score;
          x = (P1[i].x);
          y = (P1[i].y);
        }

        score = 0;
      }
    }
  }
}

/*
  Appel la fonction calcIA
  param[out] x,y - les coordonnées à renvoyer au fichier neomix.cpp
  param[in] gr - la grille du jeu
  param[in] quiJoue - le player qui doit player le prochain tour
*/
void ia(int &x, int &y, const Grille &gr, const int &quiJoue)
{
  Grille grtmp;

  for(int j=0; j<NDIMia; j++)
    for(int k=0; k<NDIMia; k++)
      grtmp[j][k]=gr[j][k];

  int player = quiJoue;
  int adversaire;
  if(quiJoue==1)
    adversaire=2;
  if(quiJoue==2)
    adversaire=1;

  calcIA(grtmp, x, y, player, adversaire);

  std::cout << x << y << std::endl;
}
