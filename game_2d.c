#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <windows.h>
#include <mmsystem.h>

#define NB_LIGNES_HEROS 5
#define NB_COLONNES_HEROS 7

#define NB_LIGNES_UNITES 12
#define NB_COLONNES_UNITES 8

#define NB_LIGNES_CARTE 10
#define NB_COLONNES_CARTE 10

#define NB_LIGNES_JOUEURS 5
#define NB_COLONNES_JOUEURS 16

#define NB_LIGNES_HEROSJOUEURS 25
#define NB_COLONNES_HEROSJOUEURS 11

#define NB_LIGNES_UNITESJOUEURS 60
#define NB_COLONNES_UNITESJOUEURS 11

#define NB_LIGNES_UNITESBATAILLE 12
#define NB_COLONNES_UNITESBATAILLE 13

#define NB_LIGNES_PLATEAUBATAILLE 12
#define NB_COLONNES_PLATEAUBATAILLE 12

#define JOUEUR_HUMAIN 0
#define LARGEUR_ECRAN_EN_CARACTERES 151

#define nbr_emplacement 6

#define TAILLE_BLOC_hauteur    60
#define TAILLE_BLOC_widht      50



typedef enum {NUM_JOUEUR_J, LIGNE_J, COLONNE_J, OR, BOIS, PIERRE, FER, SOUFFRE,
SULFURE, GEMME_ROUGE, GEMME_BLEU, GEMME_JAUNE, NB_UNITES_TUEES,
NB_BATAILLE_GAGNES, NB_UNITES_PERDUES, NB_BATAILLE_PERDUES} E_JOUEURS;

typedef enum { NUM_HERO_H, BONUS_ATTAQUE_H, BONUS_DEFENSE_H,
CHANCE_CRITIQUE_H, MORAL_H, POINTS_ATTAQUE_SPECIALE_H, PRIX_H } E_HEROS;

typedef enum { NUM_UNITE_U, TYPE_U, SANTE_U, ATTAQUE_U, DEFENSE_U, DEGATS_U,
MOUVEMENTS_U, PRIX_U } E_UNITES;

typedef enum { NUM_JOUEUR_HJ, NUM_HERO_HJ, HERO_PRINCIPAL, POINTS_EXPERIENCE,
NIVEAU_HJ, POINTS_COMPETENCE_HJ, BONUS_ATTAQUE_HJ, BONUS_DEFENSE_HJ,
CHANCE_CRITIQUE_HJ, MORAL_HJ, POINTS_ATTAQUE_SPECIALE_HJ } E_HEROSJOUEURS;

typedef enum { NUM_JOUEUR_UJ, NUM_UNITE_UJ, TYPE_UJ, SANTE_UJ, ATTAQUE_UJ,
DEFENSE_UJ, DEGATS_UJ, MOUVEMENTS_UJ, NOMBRE_UJ, NIVEAU_UJ, UNITE_ACTIVE }
E_UNITESJOUEURS;

typedef enum { NUM_JOUEUR_UB, NUM_UNITE_UB, TYPE_UB, SANTE_RESTANTE_UB,
ATTAQUE_UB, DEFENSE_UB, DEGATS_UB, MOUVEMENTS_UB, NOMBRE_UNITES_UB,
LIGNE_UB, COLONNE_UB, FINTOUR_UB, SANTE_UNITAIRE_UB} E_UNITESBATAILLE;


void chargerFichierVersTableau (int nb_lignes,int nb_colonnes,int tab[nb_lignes][nb_colonnes],char* nomFichier){
    int i,j;
    FILE*fic=fopen(nomFichier,"r");
if (fic == NULL){
    exit(1);}
    else{
      for(i=0;i<nb_lignes;i++){
      for(j=0;j<nb_colonnes;j++)
     {
        fscanf(fic,"%d",&tab[i][j]);
     }
       }}
     fclose(fic);
    }


void sauvegarderTableauDansFichier (int nb_lignes, int nb_colonnes, int tab[nb_lignes][nb_colonnes],char* nomFichier){
   int i,j;
   FILE *fic=fopen(nomFichier,"w");
    if(fic==NULL){
         exit(1);
    }
    else{
      for(i=0;i<nb_lignes;i++){
        for(j=0;j<nb_colonnes;j++){
           if(j!=nb_colonnes-1){
            fprintf(fic,"%d ",tab[i][j]);
           }
           else{
            fprintf(fic,"%d",tab[i][j]);
           }
         }
         if(i!=nb_lignes-1){
            fprintf(fic,"\n");
         }
        }
       }
 fclose(fic);
}

int chercherIndiceAvecUneCondition ( int colonneConcernee, int valeur, int nb_lignes, int
nb_colonnes, int tab[nb_lignes][nb_colonnes] ){
    int trouve;
    int i;
    int pos;
    pos=-1;
       i=0;
        trouve=0;
        while(trouve==0 && i<nb_lignes){
            if(valeur==tab[i][colonneConcernee]){
                trouve=1;
                pos=i;
            }
            else{
                 i++;
            }
        }
         return pos;
}


int chercherIndiceAvecDeuxConditions ( int colonneConcernee1, int valeur1, int colonneConcernee2,
int valeur2, int nb_lignes, int nb_colonnes, int tab[nb_lignes][nb_colonnes] ){

    int trouve;
    int i;
    int pos;
        pos=-1;
       i=0;
        trouve=0;
        while(trouve==0 && i<nb_lignes){
            if((valeur1==tab[i][colonneConcernee1]) &&(valeur2==tab[i][colonneConcernee2])){
                trouve=1;
                pos=i;
            }
            else{
                 i++;
            }
        }
         return pos;
}

void chargerJeuComplet(
int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){
       chargerFichierVersTableau( NB_LIGNES_CARTE, NB_COLONNES_CARTE,carte,"carte_sauvegarde.txt");
       chargerFichierVersTableau( NB_LIGNES_JOUEURS, NB_COLONNES_JOUEURS,joueurs,"joueurs_sauvegarde.txt");
       chargerFichierVersTableau( NB_LIGNES_HEROS, NB_COLONNES_HEROS,heros ,"heros_sauvegarde.txt");
       chargerFichierVersTableau( NB_LIGNES_HEROSJOUEURS, NB_COLONNES_HEROSJOUEURS,herosJoueurs ,"herosJoueurs_sauvegarde.txt");
       chargerFichierVersTableau( NB_LIGNES_UNITES, NB_COLONNES_UNITES,unites,"unites_sauvegarde.txt");
       chargerFichierVersTableau( NB_LIGNES_UNITESJOUEURS, NB_COLONNES_UNITESJOUEURS,unitesJoueurs,"unitesJoueurs_sauvegarde.txt");
}

void sauvegarderJeuComplet (
int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){
    sauvegarderTableauDansFichier(NB_LIGNES_CARTE,NB_COLONNES_CARTE,carte,"carte_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs,"joueurs_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros,"heros_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS, herosJoueurs,"herosJoueurs_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites,"unites_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_UNITESJOUEURS, NB_COLONNES_UNITESJOUEURS,unitesJoueurs,"unitesJoueurs_sauvegarde.txt");
}


void initialiserNouveauJeu (
int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] )
{      chargerFichierVersTableau( NB_LIGNES_CARTE,NB_COLONNES_CARTE,carte,"carte_original.txt");
       chargerFichierVersTableau( NB_LIGNES_JOUEURS, NB_COLONNES_JOUEURS,joueurs,"joueurs_original.txt");
       chargerFichierVersTableau( NB_LIGNES_HEROS,NB_COLONNES_HEROS ,heros,"heros_original.txt");
       chargerFichierVersTableau( NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs,"herosJoueurs_original.txt");
       chargerFichierVersTableau( NB_LIGNES_UNITES, NB_COLONNES_UNITES,unites,"unites_original.txt");
       chargerFichierVersTableau( NB_LIGNES_UNITESJOUEURS, NB_COLONNES_UNITESJOUEURS,unitesJoueurs ,"unitesJoueurs_original.txt");
}

void degagerNuages ( int lig, int col, int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE] ){
    int ligne_max,ligne_min,colonne_min,colonne_max,i,j,unitaire;

     ligne_min=lig-2;
     ligne_max=lig+2;
     colonne_min=col-2;
     colonne_max=col+2;

      if(ligne_min<0){
        ligne_min=0;
      }
      if(ligne_max>=NB_LIGNES_CARTE-1){
            ligne_max=NB_LIGNES_CARTE-1;
      }
      if(colonne_min<=0){
            colonne_min=0;
      }
      if(colonne_max>=NB_COLONNES_CARTE-1){
        colonne_max=NB_COLONNES_CARTE-1;
      }
        for(i=ligne_min;i<=ligne_max;i++){
            for(j=colonne_min;j<=colonne_max;j++){
                unitaire=carte[i][j]%10;
                if(unitaire==1){
                    carte[i][j]-=1;
                }

            }
        }
}

void avancer ( char action,
int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS] ){

int ligne_corrent,colonne_corrent, nouvelle_ligne,nouvelle_colonne;

  ligne_corrent=joueurs[JOUEUR_HUMAIN][LIGNE_J];
  colonne_corrent=joueurs[JOUEUR_HUMAIN][COLONNE_J];
    nouvelle_ligne=ligne_corrent;
    nouvelle_colonne=colonne_corrent;

    if(action=='h'){
          if(ligne_corrent > 0){
            nouvelle_ligne=ligne_corrent-1;
            carte[ligne_corrent][colonne_corrent]-=2;
     }
  }

    else
      if(action=='b'){
        if(ligne_corrent < NB_LIGNES_CARTE-1){
           nouvelle_ligne=ligne_corrent+1;
           carte[ligne_corrent][colonne_corrent]-=2;
      }
  }

   else if(action=='g'){
        if(colonne_corrent >0){
                 nouvelle_colonne=colonne_corrent-1;
                 carte[ligne_corrent][colonne_corrent]-=2;
        }
    }

  else
     if(action=='d'){
        if(colonne_corrent <NB_COLONNES_CARTE-1){
           nouvelle_colonne=colonne_corrent+1;
           carte[ligne_corrent][colonne_corrent]-=2;
        }
    }

 if(nouvelle_ligne!=ligne_corrent || nouvelle_colonne!=colonne_corrent){
    joueurs[JOUEUR_HUMAIN][LIGNE_J]=nouvelle_ligne;
    joueurs[JOUEUR_HUMAIN][COLONNE_J]= nouvelle_colonne;
    carte[nouvelle_ligne][nouvelle_colonne]+=2;
    degagerNuages(nouvelle_ligne,nouvelle_colonne,carte);
}
}

void deroulerJeu (SDL_Surface* ecran, int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){

    int continuer = 1;
    SDL_Event event;
    while (continuer==1)
    {
        afficherCarte(ecran,carte,joueurs);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            case SDLK_DOWN:
                  avancer('b',carte,joueurs);
                  executerAction(carte,joueurs,herosJoueurs,unitesJoueurs);
                break;
            case SDLK_UP:
                  avancer('h',carte,joueurs);
                  executerAction(carte,joueurs,herosJoueurs,unitesJoueurs);
                break;
            case SDLK_RIGHT:
                 avancer('d',carte,joueurs);
                 executerAction(carte,joueurs,herosJoueurs,unitesJoueurs);
                break;
            case SDLK_LEFT:
                  avancer('g',carte,joueurs);
                  executerAction(carte,joueurs,herosJoueurs,unitesJoueurs);
                break;
            }
            break;
        }
    }
}

void congrat(SDL_Surface* ecran,int or_humaine,int tresor_montant){

    SDL_Surface  *menu = NULL;
    SDL_Rect positionMenu;
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;
    SDL_Surface* texte2 = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    ecran = SDL_SetVideoMode(540, 540, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Treasor",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
    TTF_Init();
     font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
      font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",40);
        SDL_Color or_tresor={255, 179, 0};
        SDL_Color or={252, 211, 128};
        SDL_Rect positionText;
        //menu = IMG_Load("images/tee.gif");
        menu = IMG_Load("images/congrat.png");
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, &positionMenu);
            char* ch[50];
            char* ch1[50];
                sprintf(ch,"%d",tresor_montant);
                sprintf(ch1,"%d",or_humaine);
                texte2 = TTF_RenderText_Blended(font2,ch,or_tresor);
                positionText.x =340;
                positionText.y =310;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);
                texte2 = TTF_RenderText_Blended(font1,ch1,or);
                positionText.x =380;
                positionText.y =28;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

    SDL_Flip(ecran);
    SDL_FreeSurface(texte2);
    TTF_CloseFont(font1);
    SDL_FreeSurface(menu);
    SDL_Delay(4000);
}


void executerAction (
int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS])
{
     int ind_joueur,ligne,colonne,lig_hero_principal,choix,lig_hero;
     ind_joueur=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,JOUEUR_HUMAIN,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
     ligne=joueurs[ind_joueur][LIGNE_J];
     colonne=joueurs[ind_joueur][COLONNE_J];
    if(carte[ligne][colonne]%100/10==1){

    SDL_Surface *ecran = NULL, *menu = NULL;
    SDL_Rect positionMenu;
    ecran = SDL_SetVideoMode(540, 540, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    positionMenu.y = 0;
    SDL_WM_SetCaption("Attack",NULL);
    menu = IMG_Load("images/NON ENCORE PROGRAMEe.png");
    SDL_BlitSurface(menu, NULL, ecran, &positionMenu);
    SDL_Flip(ecran);
    SDL_Delay(1000);

    }
  else
     if(carte[ligne][colonne]%100/10==2){
      if(carte[ligne][colonne]/100!=0){
    int nombre_phrase=2;
    SDL_Surface *ecran = NULL, *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte = NULL;
    SDL_Surface* texte2 = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    ecran = SDL_SetVideoMode(540, 540, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Experience",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
    TTF_Init();
    font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",21);
    font = TTF_OpenFont("Aloevera-OVoWO.ttf",25);
        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Color or={255, 179, 0};
        SDL_Color green={4, 127, 0};
        SDL_Rect positionText;
        SDL_Rect positionText1;
        menu = IMG_Load("images/achat_tresor.png");

    while (continuer==1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(indiceOption==0)
                {
                int indice=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,JOUEUR_HUMAIN,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
                int reste=carte[ligne][colonne]/100;
                joueurs[ind_joueur][OR]+=(carte[ligne][colonne])/100;
                congrat(ecran,joueurs[indice][OR],reste);
                carte[ligne][colonne]=carte[ligne][colonne]%100;
                deroulerJeu(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                }
                else
                {
                deroulerJeu(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                }
                break;
            case SDLK_RIGHT:
                if(indiceOption>0)
                {
                    indiceOption--;
                }
                break;
            case SDLK_LEFT:
                if(indiceOption<1)
                {
                    indiceOption++;
                }
                break;
            }
            break;
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, &positionMenu);


            char* phrase[1];
            char* ch[50];
            int x=carte[ligne][colonne]/100;
            char ch1[50]="Confirmez vous l'ajout de piece d'or ?";
            sprintf(ch,"%d",x);
            phrase[0]= "OUI";
            phrase[1]= "NON";
            int i, y_depart=435;
            SDL_Color couleur_a_utiliser;
                 if(1==indiceOption)
                {
                texte = TTF_RenderText_Blended(font, phrase[1],green);
                positionText.x =90;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], blanc);
                positionText.x =380;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                }
                else
                {
                texte = TTF_RenderText_Blended(font, phrase[1], blanc);
                positionText.x =90;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], green);
                positionText.x =380;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                }

                texte2 = TTF_RenderText_Blended(font1,ch,or);
                positionText.x =240;
                positionText.y =304;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

                texte2 = TTF_RenderText_Blended(font1,ch1,or);
                positionText.x =51;
                positionText.y =56;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

    SDL_Flip(ecran);
    }
    SDL_FreeSurface(texte);
    SDL_FreeSurface(texte2);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}

     }
else
    if(carte[ligne][colonne]%100/10==3){
     if(carte[ligne][colonne]/100!=0){
    int nombre_phrase=2;

    SDL_Surface *ecran = NULL, *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte = NULL;
    SDL_Surface* texte2 = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    ecran = SDL_SetVideoMode(540, 540, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Experience",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
    TTF_Init();
    font = TTF_OpenFont("Aloevera-OVoWO.ttf",25);
    font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",22);
        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Color or={255, 179, 0};
        SDL_Color green={4, 127, 0};
        SDL_Rect positionText;
         SDL_Rect positionText1;
        menu = IMG_Load("images/achat_exp.png");

    while (continuer==1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(indiceOption==0)
                {
                //herosJoueurs[lig_hero_principal][POINTS_EXPERIENCE]+=carte[ligne][colonne]/100;
                carte[ligne][colonne]=carte[ligne][colonne]%100;
                deroulerJeu(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                }
                else
                {
                deroulerJeu(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                }
                break;
            case SDLK_RIGHT:
                if(indiceOption>0)
                {
                    indiceOption--;
                }
                break;
            case SDLK_LEFT:
                if(indiceOption<1)
                {
                    indiceOption++;
                }
                break;
            }
            break;
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, &positionMenu);

        if(font != 0)
        {
            char* phrase[1];
            char* ch[50];
            int x=carte[ligne][colonne]/100;
            char ch1[50]="Confirmez vous l'ajout de points d'experinece ?";
            sprintf(ch,"%d",x);
            phrase[0]= "OUI";
            phrase[1]= "NON";
            int i, y_depart=439;
            SDL_Color couleur_a_utiliser;

                if(1==indiceOption)
                {
                texte = TTF_RenderText_Blended(font, phrase[1],green);
                positionText.x =93;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], blanc);
                positionText.x =358;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                }
                else
                {
                texte = TTF_RenderText_Blended(font, phrase[1], blanc);
                positionText.x =93;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], green);
                positionText.x =358;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                }
                texte2 = TTF_RenderText_Blended(font1,ch,or);
                positionText.x =240;
                positionText.y =310;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

                texte2 = TTF_RenderText_Blended(font1,ch1,or);
                positionText.x =10;
                positionText.y =48;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);
        }
    SDL_Flip(ecran);


    }
    SDL_FreeSurface(texte);
    SDL_FreeSurface(texte2);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);

}
         }
    }




void  afficherCarte(SDL_Surface* ecran,int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS]){
   SDL_Surface *cache = NULL, *tresor = NULL,*xpinf=NULL,*xpsup=NULL,*exp_fermer=NULL,*tresor_fermer=NULL, *exp = NULL,*tresor_vide = NULL, *joueur_humaine = NULL,*gazon=NULL, *Enemie1 = NULL,*Enemie2 = NULL, *Enemie3 = NULL, *Enemie4 = NULL;
   SDL_Rect position, positionJoueur;
   SDL_WM_SetCaption("Carte",NULL);
   int  i = 0, j = 0;
   xpinf=IMG_Load("images/xpinf.png");
   xpsup= IMG_Load("images/xpsup.png");
   cache = IMG_Load("images/cahe.png");
   tresor = IMG_Load("images/tresor_plein.png");
   tresor_vide=IMG_Load("images/treasor_vide.png");
   tresor_fermer=IMG_Load("images/tresor_fermer.png");
   exp = IMG_Load("images/objectif.png");
   exp_fermer=IMG_Load("images/objectif_fermer.png");
   Enemie1 = IMG_Load("images/homm_PNG33.png");
   Enemie2 = IMG_Load("images/homm_PNG2.png");
   Enemie3 = IMG_Load("images/homm_PNG17.png");
   Enemie4 = IMG_Load("images/homm_PNG16.png");
   joueur_humaine = IMG_Load("images/homm_PNG14.png");
   gazon=IMG_Load("images/gzon1.png");
    SDL_Color blanc = { 255, 255, 255};
     SDL_Color or_color={255, 179, 0};
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;
    SDL_Surface* texte1,*texte2,*texte3,*texte4,*texte5,*texte6,*texte7,*texte8,*texte9 = NULL;
    SDL_Rect positionText;
    font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",19);
    font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",19);

    for (i = 0 ; i < NB_LIGNES_CARTE ; i++)
    {
        for (j = 0 ; j <  NB_COLONNES_CARTE ; j++)
        {
            if (carte[i][j] == 2)
            {
                positionJoueur.x = i;
                positionJoueur.y = j;
            }
        }
    }

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

    for (i =0 ; i < NB_LIGNES_CARTE ; i++)
        {
            for (j = 0 ; j <   NB_COLONNES_CARTE; j++)
            {
                 int chiffre_unite=carte[i][j]%100%10;
                 int chiffre_dizaine=carte[i][j]%100/10;
                position.y = i * 45;
                position.x = j * 54;
                if(carte[i][j]==2){
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(joueur_humaine, NULL, ecran, &position);
                }
                else if(carte[i][j]==0){
                     SDL_BlitSurface(gazon, NULL, ecran, &position);
                }
                else if(carte[i][j]==1){
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(cache, NULL, ecran, &position);
                }
                 else if(chiffre_unite==1){
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(cache, NULL, ecran, &position);
                }
                else if(carte[i][j]%10==0){
                if(chiffre_dizaine==2){
                    if(carte[i][j]/100==0){
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(tresor_vide, NULL, ecran, &position);
                    }else{
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(tresor_fermer, NULL, ecran, &position);
                    }
                }
                else
                     if(chiffre_dizaine==3){
                      if(carte[i][j]/100==0){
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    }else{
                    if(carte[i][j]>500000){
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(xpsup, NULL, ecran, &position);
                    }
                    else{
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(xpinf, NULL, ecran, &position);
                    }
                    }
                }
                 else if(chiffre_dizaine==1){
                switch(carte[i][j]/100)
                {
                case 1:
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(Enemie1, NULL, ecran, &position);
                    break;
                case 2:
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(Enemie2, NULL, ecran, &position);
                    break;
                case 3:
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(Enemie3, NULL, ecran, &position);
                    break;
                case 4:
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(Enemie4, NULL, ecran, &position);
                    break;
                }
            }
        }else if(carte[i][j]%10==2){
            switch(chiffre_dizaine){
             {
                case 1:
                    switch (carte[i][j]/100)
                    {
             case 1:SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(Enemie1, NULL, ecran, &position);
                    SDL_BlitSurface(joueur_humaine, NULL, ecran, &position);
                    break;
             case 2:SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(Enemie2, NULL, ecran, &position);
                    SDL_BlitSurface(joueur_humaine, NULL, ecran, &position);
                    break;
             case 3:SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(Enemie3, NULL, ecran, &position);
                    SDL_BlitSurface(joueur_humaine, NULL, ecran, &position);
                    break;
             case 4:
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(Enemie4, NULL, ecran, &position);
                    SDL_BlitSurface(joueur_humaine, NULL, ecran, &position);
                    break;
                    }
                    break;
                case 2:
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(tresor, NULL, ecran, &position);
                    SDL_BlitSurface(joueur_humaine, NULL, ecran, &position);
                    break;
                case 3:
                    if(carte[i][j]/100==0){
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    SDL_BlitSurface(joueur_humaine, NULL, ecran, &position);
                    }else{
                    SDL_BlitSurface(gazon, NULL, ecran, &position);
                    if(carte[i][j]/500000){
                    SDL_BlitSurface(xpsup, NULL, ecran, &position);
                    }else{
                    SDL_BlitSurface(xpinf, NULL, ecran, &position);
                    }
                    SDL_BlitSurface(joueur_humaine, NULL, ecran, &position);
                    }
                    break;
                }
            }

        }
      }
    }
            SDL_Surface *menu = NULL;
            SDL_Rect positionMenu;
            positionMenu.x = 0;
            positionMenu.y =274;
            menu = IMG_Load("images/Sans_titre-removebg-preview.png");
            SDL_BlitSurface(menu, NULL, ecran, &positionMenu);

                int y_depart=474;
                int indice=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,JOUEUR_HUMAIN,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
                char ch[50];
                char ch1[50];
                char ch2[50];
                char ch3[50];
                char ch4[50];
                char ch5[50];
                char ch6[50];
                char ch7[50];
                char ch8[50];
                int or=joueurs[indice][OR];
                int pierre=joueurs[indice][PIERRE];
                int bois=joueurs[indice][BOIS];
                int fer=joueurs[indice][FER];
                int souffre=joueurs[indice][SOUFFRE];
                int sulfure=joueurs[indice][SULFURE];
                int jaune=joueurs[indice][GEMME_JAUNE];
                int bluee=joueurs[indice][GEMME_BLEU];
                int rouge=joueurs[indice][GEMME_ROUGE];
                sprintf(ch,"%d",or);
                sprintf(ch1,"%d",bois);
                sprintf(ch2,"%d",fer);
                sprintf(ch3,"%d",pierre);
                sprintf(ch4,"%d",souffre);
                sprintf(ch5,"%d",sulfure);
                sprintf(ch6,"%d",jaune);
                sprintf(ch7,"%d",bluee);
                sprintf(ch8,"%d",rouge);


                texte1 = TTF_RenderText_Blended(font2,ch,or_color);
                positionText.x =480;
                positionText.y =y_depart;
                SDL_BlitSurface(texte1, NULL, ecran, &positionText);

                texte2 = TTF_RenderText_Blended(font1,ch1,blanc);
                positionText.x =401;
                positionText.y =y_depart;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

                texte3 = TTF_RenderText_Blended(font2,ch2,blanc);
                positionText.x =315;
                positionText.y =y_depart;
                SDL_BlitSurface(texte3, NULL, ecran, &positionText);

                texte4 = TTF_RenderText_Blended(font2,ch3,blanc);
                positionText.x =218;
                positionText.y =y_depart;
                SDL_BlitSurface(texte4, NULL, ecran, &positionText);

                texte5 = TTF_RenderText_Blended(font2,ch4,blanc);
                positionText.x =130;
                positionText.y =y_depart;
                SDL_BlitSurface(texte5, NULL, ecran, &positionText);

                texte6 = TTF_RenderText_Blended(font2,ch5,blanc);
                positionText.x =40;
                positionText.y =y_depart;
                SDL_BlitSurface(texte6, NULL, ecran, &positionText);

                texte7 = TTF_RenderText_Blended(font2,ch6,blanc);
                positionText.x =400;
                positionText.y =y_depart+40;
                SDL_BlitSurface(texte7, NULL, ecran, &positionText);

                texte8 = TTF_RenderText_Blended(font2,ch7,blanc);
                positionText.x =315;
                positionText.y =y_depart+40;
                SDL_BlitSurface(texte8, NULL, ecran, &positionText);

                texte9 = TTF_RenderText_Blended(font2,ch8,blanc);
                positionText.x =220;
                positionText.y =y_depart+40;
                SDL_BlitSurface(texte9, NULL, ecran, &positionText);

        SDL_Flip(ecran);
        position.x = positionJoueur.x * 49;
        position.y = positionJoueur.y * 49;
        SDL_BlitSurface(joueur_humaine, NULL, ecran, &position);
        SDL_Surface *Case_a_remplir = NULL;
        Case_a_remplir = SDL_CreateRGBSurface(SDL_HWSURFACE, 49, 49, 32,100,255,255, 250);
        SDL_FillRect(Case_a_remplir, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
        SDL_SetAlpha(Case_a_remplir, SDL_SRCALPHA, 40);
        SDL_BlitSurface(Case_a_remplir, NULL, ecran, &position);

        SDL_EnableKeyRepeat(0,0);
        SDL_FreeSurface(cache);
        SDL_FreeSurface(tresor);
  	    SDL_FreeSurface(Enemie1);
  	    SDL_FreeSurface(menu);
  	    SDL_FreeSurface(gazon);
  	    SDL_FreeSurface(tresor_fermer);
  	    SDL_FreeSurface(exp_fermer);
  	    SDL_FreeSurface(Enemie1);
        SDL_FreeSurface(Enemie2);
        SDL_FreeSurface(Enemie3);
 	    SDL_FreeSurface(Enemie4);
 	    SDL_FreeSurface(texte1);
 	    SDL_FreeSurface(texte2);
}

void  config_hero(ec){
    int nombre_phrase=2;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Heros",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Rect positionText;
        menu = IMG_Load("images/config_hero.png");

    while (continuer==1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(indiceOption==0)
                {
                    //achter_hero(ec);
                }
                else if(indiceOption==1)
                {
                   continuer = 0;
                }
                break;
            case SDLK_UP:
                if(indiceOption>0)
                {
                    indiceOption--;
                }
                break;
            case SDLK_DOWN:
                if(indiceOption<nombre_phrase-1)
                {
                    indiceOption++;
                }
                break;
            }
            break;
        }
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
        if(font != 0)
        {
            char* phrase[nombre_phrase];
            phrase[0]= "  Mettre un nouveau hero comme principal   ";
            phrase[1]= "       Revenir au menu precedent           ";
            int i, y_depart=190;
            SDL_Color couleur_a_utiliser;

            for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blue;
                }
                else
                {
                    couleur_a_utiliser=blanc;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                positionText.x =80;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
            }
        }
        else
        {
        }
        SDL_Flip(ec);
    }
    SDL_FreeSurface(texte);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}

void  config_unite(ec){
int nombre_phrase=3;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("configuration Unite",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
         font = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Rect positionText;
        menu = IMG_Load("images/config_unite.png");

    while (continuer==1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {

            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(indiceOption==0)
                {
                    //(ec);
                }
                else if(indiceOption==1)
                {
                   //
                }
                else if(indiceOption==2)
                {
                   continuer = 0;
                }
                break;
            case SDLK_UP:
                if(indiceOption>0)
                {
                    indiceOption--;
                }
                break;
            case SDLK_DOWN:
                if(indiceOption<nombre_phrase-1)
                {
                    indiceOption++;
                }
                break;
            }
            break;
        }
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
        if(font != 0)
        {
            char* phrase[nombre_phrase];
            phrase[0]= "  Mettre une unite comme principal ";
            phrase[1]= "   Mettre une unite comme reserve  ";
            phrase[2]= "     Revenir au menu precedent     ";
            int i, y_depart=190;
            SDL_Color couleur_a_utiliser;

            for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blue;
                }
                else
                {
                    couleur_a_utiliser=blanc;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                positionText.x =110;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
            }
        }
        else
        {
        }
        SDL_Flip(ec);
    }
    SDL_FreeSurface(texte);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}

void  campement(ec){
int nombre_phrase=3;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Configuer",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
         font = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Rect positionText;
        menu = IMG_Load("images/campement.png");

    while (continuer==1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {

            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(indiceOption==0)
                {
                    config_hero(ec);
                }
                else if(indiceOption==1)
                {
                   config_unite(ec);
                }
                else if(indiceOption==2)
                {
                   continuer = 0;
                }
                break;
            case SDLK_UP:
                if(indiceOption>0)
                {
                    indiceOption--;
                }
                break;
            case SDLK_DOWN:
                if(indiceOption<nombre_phrase-1)
                {
                    indiceOption++;
                }
                break;
            }
            break;
        }
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
        if(font != 0)
        {
            char* phrase[nombre_phrase];
            phrase[0]= "  configuration heros         ";
            phrase[1]= "  configuration unites        ";
            phrase[2]= "  Revenir au menu precedent   ";
            int i, y_depart=190;
            SDL_Color couleur_a_utiliser;

            for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blue;
                }
                else
                {
                    couleur_a_utiliser=blanc;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                positionText.x =140;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
            }
        }
        else
        {
        }
        SDL_Flip(ec);
    }
    SDL_FreeSurface(texte);

    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}

void acheter_unite(ec){
int nombre_phrase=2;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    ec = SDL_SetVideoMode(500, 500, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Achat Unites",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;

        TTF_Init();
         font = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Rect positionText;
        menu = IMG_Load("images/achat unites.png");

    while (continuer==1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {

            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(indiceOption==0)
                {
                     acheter_unite(ec);
                }
                else if(indiceOption==1)
                {
                   continuer = 0;
                }
                break;
            case SDLK_UP:
                if(indiceOption>0)
                {
                    indiceOption--;
                }
                break;
            case SDLK_DOWN:
                if(indiceOption<nombre_phrase-1)
                {
                    indiceOption++;
                }
                break;
            }
            break;
        }
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
        if(font != 0)
        {
            char* phrase[nombre_phrase];
            phrase[0]= " acheter des nouvelles unites ";
            phrase[1]= "  Revenir au menu precedent   ";
            int i, y_depart=190;
            SDL_Color couleur_a_utiliser;

            for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blue;
                }
                else
                {
                    couleur_a_utiliser=blanc;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                positionText.x =140;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
            }
        }
        else
        {
        }
        SDL_Flip(ec);
    }
    SDL_FreeSurface(texte);

    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}

void  acheter_hero(ec){
    int nombre_phrase=2;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Achat Heros",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Rect positionText;
        menu = IMG_Load("images/achat_heros.png");

    while (continuer==1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {

            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(indiceOption==0)
                {

                }
                else if(indiceOption==1)
                {
                   continuer = 0;
                }
                break;
            case SDLK_UP:
                if(indiceOption>0)
                {
                    indiceOption--;
                }
                break;
            case SDLK_DOWN:
                if(indiceOption<nombre_phrase-1)
                {
                    indiceOption++;
                }
                break;
            }
            break;
        }
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
        if(font != 0)
        {
            char* phrase[nombre_phrase];
            phrase[0]= "  acheter un nouveau hero        ";
            phrase[1]= "Revenir au menu precedent ";
            int i, y_depart=190;
            SDL_Color couleur_a_utiliser;

            for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blue;
                }
                else
                {
                    couleur_a_utiliser=blanc;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                positionText.x =140;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
            }
        }
        else
        {
        }
        SDL_Flip(ec);
    }
    SDL_FreeSurface(texte);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}

void magasin(ec){
    int nombre_phrase=3;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Magasin",NULL);
    menu = IMG_Load("images/magasin.png");
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",25);
        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Rect positionText;

    while (continuer==1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(indiceOption==0)
                {
                    acheter_hero(ec);
                }
                else if(indiceOption==1)
                {
                    acheter_unite(ec);
                }

                else if(indiceOption==2){
                    continuer = 0;

                }
                break;
            case SDLK_UP:
                if(indiceOption>0)
                {
                    indiceOption--;
                }
                break;
            case SDLK_DOWN:
                if(indiceOption<nombre_phrase-1)
                {
                    indiceOption++;
                }
                break;
            }
            break;
        }
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
        if(font != 0)
        {
            char* phrase[nombre_phrase];
            phrase[0]= "      acheter des heros        ";
            phrase[1]= "      acheter des unites       ";
            phrase[2]= "Revenir au menu precedent ";
            int i, y_depart=190;
            SDL_Color couleur_a_utiliser;

            for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blue;
                }
                else
                {
                    couleur_a_utiliser=blanc;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                positionText.x =120;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
            }
        }
        else
        {
        }
        SDL_Flip(ec);
    }
    SDL_FreeSurface(texte);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
    }

void menu_princial( SDL_Surface  *ecran,
int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){

    int nombre_phrase=5;
    SDL_Surface  *menu = NULL,*ligne=NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL;
    ligne = IMG_Load("images/pngtree-star-horizontal-line-decoration-elements-2986140-png-image_1731077-removebg-preview.png");
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Menu Principal",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",25);
        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Rect positionText;
        menu = IMG_Load("images/Menu Principal.png");

    while (continuer==1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_KP_ENTER: case SDLK_RETURN:
                {
                if(indiceOption==0)
                {
                    magasin(ecran);
                    sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                }
                else if(indiceOption==1)
                {
                    campement(ecran);
                    sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                }
                else if(indiceOption==2){
                     deroulerJeu(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                     sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                }
                else if(indiceOption==3){
                      initialiserNouveauJeu(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                      sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                }
                else if(indiceOption==4){
                    continuer = 0;
                }
                }
                break;
            case SDLK_UP:
                if(indiceOption>0)
                {
                    indiceOption--;
                }
                break;
            case SDLK_DOWN:
                if(indiceOption<nombre_phrase-1)
                {
                    indiceOption++;
                }
                break;
            }
            break;
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, &positionMenu);
        if(font != 0)
        {
            char* phrase[nombre_phrase];
            phrase[0]= "Magasin";
            phrase[1]= "Campement";
            phrase[2]= "Reprendre le jeu en cours";
            phrase[3]= "Nouvelle Partie";
            phrase[4]= "Quitter";
            int i, y_depart=190;
            SDL_Color couleur_a_utiliser;

            for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blue;
                    positionText.x =180;
                }
                else
                {
                    couleur_a_utiliser=blanc;
                    positionText.x =160;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                //positionText.x =160;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
            }

        }
        SDL_Flip(ecran);
    }
    SDL_FreeSurface(texte);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}
void intro(ec){
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    positionMenu.y = 0;
    SDL_WM_SetCaption("Heroes magic war",NULL);
    menu = IMG_Load("images/intro.png");
    SDL_BlitSurface(menu, NULL, ec, &positionMenu);
    SDL_Flip(ec);
    SDL_Delay(3000);
}
void depart(ec){
    intro(ec);
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    positionMenu.y = 0;
    SDL_WM_SetCaption("Heroes magic war",NULL);
    menu = IMG_Load("images/background.jpg");
    SDL_BlitSurface(menu, NULL, ec, &positionMenu);
    SDL_Flip(ec);
    SDL_Delay(3000);
}

int main(int argc, char *argv[])
{
    int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE];
    int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS];
    int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS];
    int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES];
    int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS];
    int herosJoueurs[NB_LIGNES_HEROSJOUEURS ][NB_COLONNES_HEROSJOUEURS ];
    int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE];
    int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE];

    int nombre_phrase=2;
    SDL_Surface *ecran = NULL, *menu = NULL,*cache=NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    ecran = SDL_SetVideoMode(540, 540, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    depart(ecran);
    SDL_WM_SetCaption("Heroes magic war",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
    TTF_Init();
    font = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Color or={255, 179, 0};
        SDL_Color green={4, 127, 0};
        SDL_Rect positionText;
        menu = IMG_Load("images/backgriund1.png");
    while (continuer==1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(indiceOption==0)
                {
                    initialiserNouveauJeu(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                    menu_princial(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                }
                else
                {
                    chargerJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                    menu_princial(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                }
                break;
            case SDLK_UP:
                if(indiceOption>0)
                {
                    indiceOption--;
                }
                break;
            case SDLK_DOWN:
                if(indiceOption<1)
                {
                    indiceOption++;
                }
                break;
            }
            break;
        }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, &positionMenu);

        if(font != 0)
        {
            char* phrase[nombre_phrase];
            phrase[0]= "Nouvele partie";
            phrase[1]= "charger partie";
            int i, y_depart=345;
            SDL_Color couleur_a_utiliser;
          for(i=0; i<nombre_phrase; i++)
          {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=green;
                }
                else
                {
                    couleur_a_utiliser=blanc;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
                positionText.x =33;
                positionText.y =y_depart+i*105;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
          }
        }
        SDL_Flip(ecran);
    }
    SDL_FreeSurface(texte);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}

































