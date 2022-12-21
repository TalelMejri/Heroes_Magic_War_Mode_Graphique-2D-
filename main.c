#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <windows.h>
#include <MMsystem.h>

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

#define DOUBLE_CLICK_MAX_DELAY 1000

#define max 1000

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


enum {heros1, heros2, heros3, heros4, heros5};

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
                  afficherCarte(ecran,carte,joueurs);
                  SDL_Delay(400);
                  executerAction(carte,joueurs,heros,unites,herosJoueurs,unitesJoueurs);
                break;
            case SDLK_UP:
                    avancer('h',carte,joueurs);
                    afficherCarte(ecran,carte,joueurs);
                    SDL_Delay(400);
                    executerAction(carte,joueurs,heros,unites,herosJoueurs,unitesJoueurs);
                break;
            case SDLK_RIGHT:
                 avancer('d',carte,joueurs);
                 afficherCarte(ecran,carte,joueurs);
                 SDL_Delay(400);
                 executerAction(carte,joueurs,heros,unites,herosJoueurs,unitesJoueurs);
                break;
            case SDLK_LEFT:
                  avancer('g',carte,joueurs);
                  afficherCarte(ecran,carte,joueurs);
                  SDL_Delay(400);
                  executerAction(carte,joueurs,heros,unites,herosJoueurs,unitesJoueurs);
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
    ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Treasor",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
    TTF_Init();
     font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
      font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
        SDL_Color or_tresor={255, 179, 0};
        SDL_Color red={255, 0, 0};
        SDL_Color blanc={255,255,255};
        SDL_Rect positionText;
        menu = IMG_Load("images/congrat_treasor.png");
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, &positionMenu);
            char* ch_congrat[80];
            sprintf(ch_congrat,"Un tr�sor de montant << %d >> a �t� ajout� ",tresor_montant);
            char ch_congrat_suv[40]="a votre fortune!";
            char* ch1[50];
                sprintf(ch1,"%d",or_humaine);
                texte2 = TTF_RenderText_Blended(font1,ch1,or_tresor);
                positionText.x =353;
                positionText.y =370;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

                texte2 = TTF_RenderText_Blended(font2,ch_congrat,blanc);
                positionText.x =130;
                positionText.y =460;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

                texte2 = TTF_RenderText_Blended(font2,ch_congrat_suv,blanc);
                positionText.x =280;
                positionText.y =490;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

    SDL_Flip(ecran);
    SDL_FreeSurface(texte2);
    TTF_CloseFont(font1);
    SDL_FreeSurface(menu);
    SDL_Delay(3000);
}

void congrat_exp(SDL_Surface* ecran,int exp_montant){
    SDL_Surface  *menu = NULL;
    SDL_Rect positionMenu;
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;
    SDL_Surface* texte2 = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Experience",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
    TTF_Init();
      font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
      font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
        SDL_Color or_tresor={255, 179, 0};
        SDL_Color red={255, 0, 0};
        SDL_Color blanc={255, 255, 255};
        SDL_Rect positionText;
        menu = IMG_Load("images/congrat_exp.png");
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, &positionMenu);
            char* ch_congrat[80];
            sprintf(ch_congrat,"<< %d >> points d'experience a �t� ajout� � votre h�ro principal",exp_montant);
            char ch_congrat_suiv[40]="a h�ro principal !";
                char* ch1[50];
                sprintf(ch1,"%d",exp_montant);

                texte2 = TTF_RenderText_Blended(font1,ch1,or_tresor);
                positionText.x =354;
                positionText.y =370;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

                texte2 = TTF_RenderText_Blended(font2,ch_congrat,blanc);
                positionText.x =28;
                positionText.y =460;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

                texte2 = TTF_RenderText_Blended(font2,ch_congrat_suiv,blanc);
                positionText.x =290;
                positionText.y =490;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

    SDL_Flip(ecran);
    SDL_FreeSurface(texte2);
    TTF_CloseFont(font1);
    SDL_FreeSurface(menu);
    SDL_Delay(3000);
}

void confirmation_experience (SDL_Surface *ecran ,int ligne ,int colonne,int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){
   int nombre_phrase=2;
    SDL_Surface  *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte = NULL;
    SDL_Surface* texte2 = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
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
                int reste=carte[ligne][colonne]/100;
                congrat_exp(ecran,reste);
                carte[ligne][colonne]=carte[ligne][colonne]%100;
                continuer = 0;
                }
                else
                {
                continuer = 0;
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
            char ch1[50]="Confirmez vous l'ajout de points d'�xperinece ?";
            sprintf(ch,"%d",x);
            phrase[0]= "OUI";
            phrase[1]= "NON";
          int i, y_depart=488;
            SDL_Color couleur_a_utiliser;
                 if(1==indiceOption)
                {
                texte = TTF_RenderText_Blended(font, phrase[1],green);
                positionText.x =170;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], blanc);
                positionText.x =550;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                }
                else
                {
                texte = TTF_RenderText_Blended(font, phrase[1], blanc);
                positionText.x =170;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], green);
                positionText.x =550;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                }

                texte2 = TTF_RenderText_Blended(font1,ch,or);
                positionText.x =350;
                positionText.y =340;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

                texte2 = TTF_RenderText_Blended(font1,ch1,or);
                positionText.x =170;
                positionText.y =62;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);
        }
    SDL_Flip(ecran);
    }
    SDL_FreeSurface(texte);
    SDL_FreeSurface(texte2);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}


void confirmation_tresor ( SDL_Surface *ecran ,int ind_joueur ,int ligne,int colonne,
int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){
    int nombre_phrase=2;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte = NULL;
    SDL_Surface* texte2 = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
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
                continuer = 0;
                }
                else
                {
                continuer = 0;
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
            char ch1[50]="Confirmez vous l'ajout de pi�ce d'or ?";
            sprintf(ch,"%d",x);
            phrase[0]= "OUI";
            phrase[1]= "NON";
            int i, y_depart=488;
            SDL_Color couleur_a_utiliser;
                 if(1==indiceOption)
                {
                texte = TTF_RenderText_Blended(font, phrase[1],green);
                positionText.x =170;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], blanc);
                positionText.x =550;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                }
                else
                {
                texte = TTF_RenderText_Blended(font, phrase[1], blanc);
                positionText.x =170;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], green);
                positionText.x =550;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
                }

                texte2 = TTF_RenderText_Blended(font1,ch,or);
                positionText.x =350;
                positionText.y =340;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

                texte2 = TTF_RenderText_Blended(font1,ch1,or);
                positionText.x =170;
                positionText.y =62;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

    SDL_Flip(ecran);
    }
    SDL_FreeSurface(texte);
    SDL_FreeSurface(texte2);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}

void warning_hero(SDL_Surface *ec){
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Event event;
    positionMenu.y = 0;
    int continuer=1;
    SDL_WM_SetCaption("Annuler",NULL);
    menu = IMG_Load("images/warning_hero.png");
    while (continuer==1)
    {   SDL_BlitSurface(menu, NULL, ec, &positionMenu);
       SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
        case SDLK_ESCAPE:
            continuer = 0;
            break;
        }
        }
    SDL_Flip(ec);
    }
SDL_FreeSurface(menu);
}

void warning_hero_principal(SDL_Surface *ec){
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Event event;
    positionMenu.y = 0;
    int continuer=1;
    SDL_WM_SetCaption("Annuler",NULL);
    menu = IMG_Load("images/hero_principal.png");
    while (continuer==1)
    {   SDL_BlitSurface(menu, NULL, ec, &positionMenu);
       SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
        case SDLK_ESCAPE:
            continuer = 0;
            break;
        }
        }
    SDL_Flip(ec);
    }
SDL_FreeSurface(menu);
}
int choixAutomatiqueUniteEnnemie ( int numJoueurEnnemi, int codeAttaque,
int ligne_courante, int colonne_courante,
int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE])
{
    int num_unite_attaque;
    int droite=0,gauche=0,haut=0,bas=0;
     if(codeAttaque==1){
            int chercher_indice=chercherIndiceAvecUneCondition(NUM_JOUEUR_UB,numJoueurEnnemi,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
            num_unite_attaque=unitesBataille[chercher_indice][NUM_UNITE_UB];
            return num_unite_attaque;
     }
   else
      if(codeAttaque>1){

       haut=(codeAttaque/10000)%10;
       bas=(codeAttaque/1000)%10;
       droite=(codeAttaque/10)%10;
       gauche=(codeAttaque/100)%10;

     if(gauche==1){
        num_unite_attaque=plateauBataille[ligne_courante][colonne_courante-1];
        return num_unite_attaque/1000;
     }
     else if(droite==1){
        num_unite_attaque=plateauBataille[ligne_courante][colonne_courante+1];
        return num_unite_attaque/1000;
     }
     else if(haut==1){
        num_unite_attaque=plateauBataille[ligne_courante-1][colonne_courante];
        return num_unite_attaque/1000;
     }
     else if(bas==1){
        num_unite_attaque= plateauBataille[ligne_courante+1][colonne_courante];
        return num_unite_attaque/1000;
     }
    }
}

void deplacementAutomatiqueUnite( int numJoueur, int typeUnite,
 int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
 int ligne_courante, int colonne_courante,
 int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE],
 int indiceUnite)
 {
     int num_unite=unitesBataille[indiceUnite][NUM_UNITE_UB];
     int codeattaque,trouve=0,i=0,j=0;
     int nouvelle_ligne=-1,nouvelle_colonne=-1;
        while(i<=NB_LIGNES_PLATEAUBATAILLE-1 && trouve==0){
            while(j<=NB_COLONNES_PLATEAUBATAILLE-1 && trouve==0){
                    if(plateauBataille[i][j]%10==1){
                        nouvelle_ligne=i;
                        nouvelle_colonne=j;
                        codeattaque=codeAttaqueSansDeplacement(numJoueur,typeUnite,nouvelle_ligne,nouvelle_colonne,plateauBataille);
                       if(codeattaque>1 && plateauBataille[nouvelle_ligne][nouvelle_colonne]/100%10!=numJoueur){
                        trouve=1;
                    }
                }
                j++;
         }
         if(j>=NB_COLONNES_PLATEAUBATAILLE-1){
            j=0;
            i++;
          }
}
         if(nouvelle_ligne!=-1 && nouvelle_colonne!=-1 ){
         deplacerUnite(numJoueur,indiceUnite,num_unite,ligne_courante,colonne_courante,nouvelle_ligne,nouvelle_colonne,plateauBataille,unitesBataille);
     }
}

void marquerDeplacementsPossibles (
int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
int ligneCourante,int colonneCourante,int mouvements){

  int i,j;
  int somme=0;
  int ligne_min=ligneCourante-mouvements;
  int ligne_max=ligneCourante+mouvements;
  int colonne_min=colonneCourante-mouvements;
  int colonne_max=colonneCourante+mouvements;
  int differ_ligne=0;
  int differ_col=0;

   if(ligne_max>=NB_LIGNES_PLATEAUBATAILLE-1){
        ligne_max=NB_LIGNES_PLATEAUBATAILLE-1;
   }
   if(ligne_min<0){
        ligne_min=0;
   }
   if(colonne_max>=NB_COLONNES_PLATEAUBATAILLE-1){
        colonne_max=NB_COLONNES_PLATEAUBATAILLE-1;
   }
   if(colonne_min<0){
        colonne_min=0;
   }

    for(i=ligne_min;i<=ligne_max;i++){
        for(j=colonne_min;j<=colonne_max;j++){
             differ_ligne=abs(ligneCourante-i);
             differ_col=abs(colonneCourante-j);
             somme=differ_col+differ_ligne;
             if(somme<mouvements+1){
                if(plateauBataille[i][j]==0)
                    plateauBataille[i][j]=1;

            }
        }
    }
}
void effacerDeplacementsPossibles (
int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE] )
{
        int i,j;
         for(i=0;i<NB_LIGNES_PLATEAUBATAILLE;i++){
            for(j=0;j<NB_COLONNES_PLATEAUBATAILLE;j++){
                if(plateauBataille[i][j]==1){
                    plateauBataille[i][j]=0;
                }
            }
         }
}

void deplacerUnite (int numJoueur, int indiceDansUniteEnBataille,
int numUnite, int ligne_courante, int colonne_courante,int nouvelle_ligne, int nouvelle_colonne,
int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE] ){

if(plateauBataille[nouvelle_ligne][nouvelle_colonne]==1){
    unitesBataille[indiceDansUniteEnBataille][LIGNE_UB]= nouvelle_ligne;
     unitesBataille[indiceDansUniteEnBataille][COLONNE_UB]= nouvelle_colonne;
       plateauBataille[nouvelle_ligne][nouvelle_colonne]= plateauBataille[ligne_courante][colonne_courante];
       plateauBataille[ligne_courante][colonne_courante]=0;
    }
}

int codeAttaqueSansDeplacement(
int numJoueur, int typeUnite,int ligne_courante, int colonne_courante,
int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE] ){
 int premier=0,deuxieme=0,troiseme=0,qutreieme=0;
 int droite=plateauBataille[ligne_courante][colonne_courante+1];
 int gauche=plateauBataille[ligne_courante][colonne_courante-1];
 int bas=plateauBataille[ligne_courante+1][colonne_courante];
 int haut=plateauBataille[ligne_courante-1][colonne_courante];

    if(typeUnite==1){
      return 1;
   }
    else if(typeUnite==0 || typeUnite==2)
        {
        if(droite/1000!=0 && droite/100%10!=numJoueur){
            premier=10;
        }
         if(gauche/1000!=0 && gauche/100%10!=numJoueur){
            deuxieme=100;
        }
         if(bas/1000!=0 && bas/100%10!=numJoueur){
            troiseme=1000;
        }
         if(haut/1000!=0 && haut/100%10!=numJoueur){
            qutreieme=10000;
        }
         if(premier!=0 || deuxieme!=0 || troiseme!=0 || qutreieme!=0  ){
            return (premier+deuxieme+troiseme+qutreieme)+2;
        }
   }
   return 0;
}

int uniteAttaquable( int codeAttaque, int ligne_courante, int colonne_courante,
int ligne_ennemie, int colonne_ennemie ){
    int differ_ligne=abs(ligne_courante-ligne_ennemie);
    int differ_colonne=abs(colonne_courante-colonne_ennemie);
      int haut=codeAttaque/10000%10;
      int bas=codeAttaque/1000%10;
      int droite=codeAttaque/10%10;
      int gauche=codeAttaque/100%10;

    if(codeAttaque%10==1){
        return 1;
    }
    else if(codeAttaque%10==2){
      if(differ_ligne==0 && differ_colonne==1 && droite==1){
        return 1;
    }
      if(differ_ligne==0 && differ_colonne==1 && gauche==1){
       return 1;
     }
      if(differ_ligne==1 && differ_colonne==0 && haut==1){
      return 1;
    }
      if(differ_ligne==1 && differ_colonne==0 && bas==1){
      return 1;
    }
}
    return 0;
}

void attaque(int indiceDansUniteEnBataille, int indiceEnnemiDansUnitesBataille,
int numJoueur, int numEnnemi, int ligne, int colonne,
int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS] )
{
    int perte_totale_de_sante;
    float bonus_attaque;
    int rest_sante_totale;
    int sante_totale_restant;
    int nombre_unite_restant;
    int sante_totale_unite_attaque;
    int indice_joueur,attaque_unite_attauqant,defense_enemie,nombre_unite_attaquant,degat_attaquant;
    int sante_unitaire_enemie,indice_enemie,nombre_unite_enemi;
    sante_unitaire_enemie=unitesBataille[indiceEnnemiDansUnitesBataille][SANTE_UNITAIRE_UB];
    nombre_unite_enemi=unitesBataille[indiceEnnemiDansUnitesBataille][NOMBRE_UNITES_UB];
    degat_attaquant=unitesBataille[indiceDansUniteEnBataille][DEGATS_UB];
    nombre_unite_attaquant=unitesBataille[indiceDansUniteEnBataille][NOMBRE_UNITES_UB];
    defense_enemie=unitesBataille[indiceEnnemiDansUnitesBataille][DEFENSE_UB];
    attaque_unite_attauqant=unitesBataille[indiceDansUniteEnBataille][ATTAQUE_UB];
    sante_totale_unite_attaque=(nombre_unite_enemi*sante_unitaire_enemie)+(sante_totale_unite_attaque%sante_unitaire_enemie);
    bonus_attaque=(attaque_unite_attauqant-defense_enemie)/200.0;
    perte_totale_de_sante=nombre_unite_attaquant*degat_attaquant+nombre_unite_attaquant*degat_attaquant*bonus_attaque;
    sante_totale_restant=sante_totale_unite_attaque-perte_totale_de_sante;
    if(sante_totale_restant<=0){
        indice_joueur=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,numJoueur,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
        indice_enemie=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,numEnnemi,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
        joueurs[indice_enemie][NB_UNITES_PERDUES]+=1;
        joueurs[indice_joueur][NB_UNITES_TUEES]+=1;
        plateauBataille[ligne][colonne]=0;
        for(int i=0;i<=SANTE_UNITAIRE_UB;i++){
        unitesBataille[indiceEnnemiDansUnitesBataille][i]=-1;
        }
    }
    else{
        nombre_unite_restant=sante_totale_restant/sante_unitaire_enemie;
        unitesBataille[indiceEnnemiDansUnitesBataille][NOMBRE_UNITES_UB]=nombre_unite_restant;
        rest_sante_totale=sante_totale_restant%sante_unitaire_enemie;
        if(rest_sante_totale>=0){
            unitesBataille[indiceEnnemiDansUnitesBataille][NOMBRE_UNITES_UB]+=1;
            unitesBataille[indiceEnnemiDansUnitesBataille][SANTE_RESTANTE_UB]=rest_sante_totale;
        }
        else{
            unitesBataille[indiceEnnemiDansUnitesBataille][SANTE_RESTANTE_UB]=sante_unitaire_enemie;
        }
    }
}

void reinitialiserToursBataille (
int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE] )
{
    for(int i=0;i<NB_LIGNES_UNITESBATAILLE;i++){
        unitesBataille[i][FINTOUR_UB]=0;
    }
}

int verificationFinTours ( int numJoueur,int
unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE] )
{
 int nombre_unite_fin_tour=0;
  for(int i=0;i<NB_LIGNES_UNITESBATAILLE;i++){
        if(unitesBataille[i][NUM_JOUEUR_UB]==numJoueur && unitesBataille[i][FINTOUR_UB]==0){
            nombre_unite_fin_tour++;
        }
  }
  if(nombre_unite_fin_tour > 0){
    return 0;
  }
  else{
    return 1;
  }
}

int verificationGagnant ( int numJoueur, int numJoueurEnnemi, int
unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE] )
{
    int nbr_joueur=0;
    int nbr_enemie=0;
     for(int i=0;i<NB_LIGNES_UNITESBATAILLE-1;i++){
        if(unitesBataille[i][NUM_JOUEUR_UB]==numJoueur){
            nbr_joueur++;
        }
        if(unitesBataille[i][NUM_JOUEUR_UB]==numJoueurEnnemi){
            nbr_enemie++;
        }
    }

    if(nbr_enemie>0 && nbr_joueur==0){
        return numJoueurEnnemi;
    }

    else if(nbr_joueur>0 && nbr_enemie==0){
        return numJoueur;
    }
    else{
        return -1;
    }
}

int DoubleClickDetected (void)
{
    static Uint32 LastClickTicks;
    Uint32 CurrentClickTicks;
    if (! LastClickTicks)
    {
        LastClickTicks = SDL_GetTicks ();
        return 0;
    }
    else
    {
        CurrentClickTicks = SDL_GetTicks ();
        if (CurrentClickTicks - LastClickTicks <= DOUBLE_CLICK_MAX_DELAY)
        {
            LastClickTicks = CurrentClickTicks;
            return 1;
        }
        else
        {
            LastClickTicks = CurrentClickTicks;
            return 0;
        }
    }
}

void info_unite_bataille(SDL_Surface *ec, int x,int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE]){
     SDL_Surface *unite4 = NULL, *unite5 = NULL, *unite8 = NULL, *unite9 = NULL,*unite10 = NULL,*unite12 = NULL, *unite14 = NULL,*unite15= NULL,*unite18 = NULL, *unite19 = NULL,*unite22= NULL,*unite23=NULL;
     unite4 = IMG_Load("images/info_unite4.png");
     unite5 = IMG_Load("images/info_unite5.png");
     unite8 = IMG_Load("images/info_unite8.png");
     unite9 = IMG_Load("images/info_unite9.png");
     unite10 = IMG_Load("images/info_unite10.png");
     unite12 = IMG_Load("images/info_unite12.png");
     unite14 = IMG_Load("images/info_unite14.png");
     unite15 = IMG_Load("images/info_unite15.png");
     unite18= IMG_Load("images/info_unite18.png");
     unite19 = IMG_Load("images/info_unite19.png");
     unite22 = IMG_Load("images/info_unite22.png");
     unite23 = IMG_Load("images/info_unite23.png");
            int liste_unite=12;
            int numero[liste_unite];
                numero[0]=10;
                numero[1]=12;
                numero[2]=15;
                numero[3]=14;
                numero[4]=18;
                numero[5]=19;
                numero[6]=22;
                numero[7]=23;
                numero[8]=4;
                numero[9]=5;
                numero[10]=8;
                numero[11]=9;
        int i;
         int tab[50];
         for(i=0;i<12;i++){
           int indice_humaine=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,0,NUM_UNITE_UB,numero[i],NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
           int nbr_soldats=unitesBataille[indice_humaine][NOMBRE_UNITES_UB];
           tab[i]=nbr_soldats;
         }

         char* ch_10[50];
         char* ch_12[50];
         char* ch_15[50];
         char* ch_14[50];
         char* ch_18[50];
         char* ch_19[50];
         char* ch_22[50];
         char* ch_23[50];
         char* ch_4[50];
         char* ch_5[50];
         char* ch_8[50];
         char* ch_9[50];

        sprintf(ch_10,"%d",tab[0]);
        sprintf(ch_12,"%d",tab[1]);
        sprintf(ch_15,"%d",tab[2]);
        sprintf(ch_14,"%d",tab[3]);
        sprintf(ch_18,"%d",tab[4]);
        sprintf(ch_19,"%d",tab[5]);
        sprintf(ch_22,"%d",tab[6]);
        sprintf(ch_23,"%d",tab[7]);
        sprintf(ch_4,"%d",tab[8]);
        sprintf(ch_5,"%d",tab[9]);
        sprintf(ch_8,"%d",tab[10]);
        sprintf(ch_9,"%d",tab[11]);

            char* tous_nbr_solsats_humiane[liste_unite];
                tous_nbr_solsats_humiane[0]=ch_10;
                tous_nbr_solsats_humiane[1]=ch_12;
                tous_nbr_solsats_humiane[2]=ch_15;
                tous_nbr_solsats_humiane[3]=ch_14;
                tous_nbr_solsats_humiane[4]=ch_18;
                tous_nbr_solsats_humiane[5]=ch_19;
                tous_nbr_solsats_humiane[6]=ch_22;
                tous_nbr_solsats_humiane[7]=ch_23;
                tous_nbr_solsats_humiane[8]=ch_4;
                tous_nbr_solsats_humiane[9]=ch_5;
                tous_nbr_solsats_humiane[10]=ch_8;
                tous_nbr_solsats_humiane[11]=ch_9;

        SDL_Event event;
        SDL_Surface *menu = NULL;
        SDL_Surface* texte = NULL;
        SDL_Rect positionMenu;
        SDL_Rect positionText;
        SDL_Color blanc = { 255, 255, 255};
        TTF_Font* font = NULL;
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",25);
        SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
        positionMenu.x = 0;
        SDL_Rect position;
        positionMenu.y = 0;
        SDL_WM_SetCaption("Heroes magic war",NULL);
        TTF_Init();
        int continuer=1;
while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int y_clicked=event.button.y;
                int x_clicked=event.button.x;
                  int xmin=710;
                  int xmax=750;
                  int ymin=40;
                  int ymax=91;
                    if(x_clicked>xmin && x_clicked<xmax){
                    if(y_clicked>ymin && y_clicked<ymax){
                        continuer=0;
                    }
                  }

            }break;
        }
        char* phrase[liste_unite];
        phrase[0]= unite10;
        phrase[1]= unite12;
        phrase[2]= unite15;
        phrase[3]= unite14;

        phrase[4]= unite18;
        phrase[5]= unite19;
        phrase[6]= unite22;
        phrase[7]= unite23;

        phrase[8]= unite4;
        phrase[9]= unite5;
        phrase[10]= unite8;
        phrase[11]= unite9;
        for( i=0;i<liste_unite;i++){
            if(numero[i]==x){
                  SDL_BlitSurface(phrase[i], NULL, ec, &positionMenu);
                  position.x=580;
                  position.y=135;
                  texte = TTF_RenderText_Blended(font,tous_nbr_solsats_humiane[i],blanc);
                  SDL_BlitSurface(texte, NULL, ec, &position);
            }
        }
    SDL_Flip(ec);
    }
    SDL_FreeSurface(unite10);
    SDL_FreeSurface(unite12);
    SDL_FreeSurface(unite15);
    SDL_FreeSurface(unite14);
    SDL_FreeSurface(unite18);
    SDL_FreeSurface(unite19);
    SDL_FreeSurface(unite22);
    SDL_FreeSurface(unite23);
    SDL_FreeSurface(unite4);
    SDL_FreeSurface(unite5);
    SDL_FreeSurface(unite8);
    SDL_FreeSurface(unite9);
}

void affiche_attaque_humaine(SDL_Surface *ec,int num_unite,int numero_unite_enemie,int numjoueur,int num_enemie,int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE],int attaquer,int contre){
      SDL_Surface  *unite4 = NULL, *unite5 = NULL, *unite8 = NULL, *unite9 = NULL,*unite10 = NULL, *unite12 = NULL, *unite14 = NULL, *unite18 = NULL,*unite19 = NULL,
        *unite22 = NULL, *unite23 = NULL,*unite15 = NULL;
      SDL_Surface *enmie1=NULL,*enmie2=NULL, *enmie3=NULL, *enmie4 = NULL,*heros1=NULL, *heros2 = NULL, *heros3 = NULL, *heros4 = NULL,*heros5 = NULL,
      *unite1_enemie1 = NULL, *unite2_enemie1 = NULL, *unite1_enemie2 = NULL,*unite2_enemie2 = NULL,
       *unite1_enemie3 = NULL, *unite2_enemie3 = NULL, *unite1_enemie4 = NULL,*unite2_enemie4 = NULL;
    SDL_Rect position;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("UNITE",NULL);
    menu = IMG_Load("images/afficher_attaquer.png");
    int indiceOption=0;
    positionMenu.x = 0;
    int j;
    int liste_unite=12;
    positionMenu.y = 0;
    SDL_BlitSurface(menu, NULL, ec, &positionMenu);
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",21);
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",16);
        font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",13);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
        SDL_Rect positionText;

     unite1_enemie1= IMG_Load("images/unite_enemie1.png");
     unite2_enemie1= IMG_Load("images/unite1_enemie1.png");
     unite1_enemie2= IMG_Load("images/unite_enemie2.png");
     unite2_enemie2= IMG_Load("images/unite1_enemie2.png");
     unite1_enemie3= IMG_Load("images/unite_enemie3.png");
     unite2_enemie3= IMG_Load("images/unite1_enemie3.png");
     unite1_enemie4= IMG_Load("images/unite_enemie4.png");
     unite2_enemie4= IMG_Load("images/unite1_enemie4.png");
      int numero_unite[8];
       numero_unite[0]=5;
       numero_unite[1]=19;
       numero_unite[2]=9;
       numero_unite[3]=10;
       numero_unite[4]=22;
       numero_unite[5]=23;
       numero_unite[6]=14;
       numero_unite[7]=15;
       char *unite_enemie[50];
        unite_enemie[0]=unite1_enemie1;
        unite_enemie[1]=unite2_enemie1;
        unite_enemie[2]=unite1_enemie2;
        unite_enemie[3]=unite2_enemie2;
        unite_enemie[4]=unite1_enemie3;
        unite_enemie[5]=unite2_enemie3;
        unite_enemie[6]=unite1_enemie4;
        unite_enemie[7]=unite2_enemie4;

            int numero[liste_unite];
                numero[0]=10;
                numero[1]=12;
                numero[2]=15;
                numero[3]=14;
                numero[4]=18;
                numero[5]=19;
                numero[6]=22;
                numero[7]=23;
                numero[8]=4;
                numero[9]=5;
                numero[10]=8;
                numero[11]=9;

     unite4 = IMG_Load("images/unite__4.png");
     unite15 = IMG_Load("images/unite__15.png");
     unite5 = IMG_Load("images/unite__5.png");
     unite8 = IMG_Load("images/unite__8.png");
     unite9 = IMG_Load("images/unite__9.png");
     unite14 = IMG_Load("images/unite__14.png");
     unite10 = IMG_Load("images/unite__10.png");
     unite12= IMG_Load("images/unite__12.png");
     unite18 = IMG_Load("images/unite__18.png");
     unite19 = IMG_Load("images/unite__19.png");
     unite22= IMG_Load("images/unite__22.png");
     unite23 = IMG_Load("images/unite__23.png");
         char* tous_unites[liste_unite];
            tous_unites[0]= unite10;
            tous_unites[1]= unite12;
            tous_unites[2]= unite15;
            tous_unites[3]= unite14;
            tous_unites[4]= unite18;
            tous_unites[5]= unite19;
            tous_unites[6]= unite22;
            tous_unites[7]= unite23;
            tous_unites[8]= unite4;
            tous_unites[9]= unite5;
            tous_unites[10]= unite8;
            tous_unites[11]= unite9;

            int indice_humaine=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,numjoueur,NUM_UNITE_UB,num_unite,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
            int indice_enemie=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,num_enemie,NUM_UNITE_UB,numero_unite_enemie,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
            int nbr_soldats_humaine=unitesBataille[indice_humaine][NOMBRE_UNITES_UB];
            int nbr_soldats_enemie=unitesBataille[indice_enemie][NOMBRE_UNITES_UB];
            int sante_humaine=unitesBataille[indice_humaine][SANTE_UNITAIRE_UB];
            int sante_enemie=unitesBataille[indice_enemie][SANTE_UNITAIRE_UB];

                 char *ch[50];
                 sprintf(ch,"l'unit� %d ",num_unite);
                 texte = TTF_RenderText_Blended(font,ch,blanc);
                 positionText.x =260;
                 positionText.y =30;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);
                 char *ch1[50];
                 if(contre==0){
                 sprintf(ch1,"attaque l'unit� %d ",numero_unite_enemie);
                 }else{
                 sprintf(ch1,"contre attaque l'unite %d ",numero_unite_enemie);
                 }
                 texte = TTF_RenderText_Blended(font,ch1,blanc);
                 positionText.x =365;
                 positionText.y =30;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);
                 char *ch2[50];
                 sprintf(ch2,"nombre soldats %d ",nbr_soldats_humaine);
                 texte = TTF_RenderText_Blended(font,ch2,blanc);
                 positionText.x =90;
                 positionText.y =320;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);
                 char *ch4[50];
                 sprintf(ch4,"Sante Unitaire  %d ",sante_humaine);
                 texte = TTF_RenderText_Blended(font,ch4,blanc);
                 positionText.x =90;
                 positionText.y =340;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);

                 char *ch3[50];
                 if(indice_enemie==-1){
                 sprintf(ch3,"nombre soldats %d ",0);
                 }else{
                 sprintf(ch3,"nombre soldats %d ",nbr_soldats_enemie);
                 }
                 texte = TTF_RenderText_Blended(font,ch3,blanc);
                 positionText.x =520;
                 positionText.y =320;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);

                 char *ch5[50];
                 if(indice_enemie==-1){
                 sprintf(ch5,"Sante Unitaire  %d ",0);
                 }else{
                 sprintf(ch5,"Sante Unitaire %d ",sante_enemie);
                 }
                 texte = TTF_RenderText_Blended(font,ch5,blanc);
                 positionText.x =520;
                 positionText.y =350;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);

           for(j=0;j<liste_unite;j++){
                if(numero[j]==num_unite){
                      position.x=180;
                      position.y=120;
                    SDL_BlitSurface(tous_unites[j], NULL, ec, &position);
                }
            }

             for(j=0;j<8;j++){
                if(numero_unite[j]==numero_unite_enemie ){

                     position.x=580;
                     position.y=120;
                    SDL_BlitSurface(unite_enemie[j], NULL, ec, &position);
                }
             }
    SDL_Flip(ec);
    SDL_FreeSurface(menu);
    SDL_Delay(2000);
}

void affiche_attaque_ennemie(SDL_Surface *ec,int num_unite,int numero_unite_enemie,int numjoueur,int num_enemie,int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE],int attaquer,int contre){
      SDL_Surface  *unite4 = NULL, *unite5 = NULL, *unite8 = NULL, *unite9 = NULL,*unite10 = NULL, *unite12 = NULL, *unite14 = NULL, *unite18 = NULL,*unite19 = NULL,
        *unite22 = NULL, *unite23 = NULL,*unite15 = NULL;
      SDL_Surface *enmie1=NULL,*enmie2=NULL, *enmie3=NULL, *enmie4 = NULL,*heros1=NULL, *heros2 = NULL, *heros3 = NULL, *heros4 = NULL,*heros5 = NULL,
      *unite1_enemie1 = NULL, *unite2_enemie1 = NULL, *unite1_enemie2 = NULL,*unite2_enemie2 = NULL,
       *unite1_enemie3 = NULL, *unite2_enemie3 = NULL, *unite1_enemie4 = NULL,*unite2_enemie4 = NULL;
    SDL_Rect position;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("UNITE",NULL);
    menu = IMG_Load("images/afficher_attaquer.png");
    int indiceOption=0;
    positionMenu.x = 0;
    int j;
    int liste_unite=12;
    positionMenu.y = 0;
    SDL_BlitSurface(menu, NULL, ec, &positionMenu);
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",21);
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",16);
        font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",13);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
        SDL_Rect positionText;

     unite1_enemie1= IMG_Load("images/unite_enemie1.png");
     unite2_enemie1= IMG_Load("images/unite1_enemie1.png");
     unite1_enemie2= IMG_Load("images/unite_enemie2.png");
     unite2_enemie2= IMG_Load("images/unite1_enemie2.png");
     unite1_enemie3= IMG_Load("images/unite_enemie3.png");
     unite2_enemie3= IMG_Load("images/unite1_enemie3.png");
     unite1_enemie4= IMG_Load("images/unite_enemie4.png");
     unite2_enemie4= IMG_Load("images/unite1_enemie4.png");
      int numero_unite[8];
       numero_unite[0]=5;
       numero_unite[1]=19;
       numero_unite[2]=9;
       numero_unite[3]=10;
       numero_unite[4]=22;
       numero_unite[5]=23;
       numero_unite[6]=14;
       numero_unite[7]=15;
       char *unite_enemie[50];
        unite_enemie[0]=unite1_enemie1;
        unite_enemie[1]=unite2_enemie1;
        unite_enemie[2]=unite1_enemie2;
        unite_enemie[3]=unite2_enemie2;
        unite_enemie[4]=unite1_enemie3;
        unite_enemie[5]=unite2_enemie3;
        unite_enemie[6]=unite1_enemie4;
        unite_enemie[7]=unite2_enemie4;

            int numero[liste_unite];
                numero[0]=10;
                numero[1]=12;
                numero[2]=15;
                numero[3]=14;
                numero[4]=18;
                numero[5]=19;
                numero[6]=22;
                numero[7]=23;
                numero[8]=4;
                numero[9]=5;
                numero[10]=8;
                numero[11]=9;

     unite4 = IMG_Load("images/unite__4.png");
     unite15 = IMG_Load("images/unite__15.png");
     unite5 = IMG_Load("images/unite__5.png");
     unite8 = IMG_Load("images/unite__8.png");
     unite9 = IMG_Load("images/unite__9.png");
     unite14 = IMG_Load("images/unite__14.png");
     unite10 = IMG_Load("images/unite__10.png");
     unite12= IMG_Load("images/unite__12.png");
     unite18 = IMG_Load("images/unite__18.png");
     unite19 = IMG_Load("images/unite__19.png");
     unite22= IMG_Load("images/unite__22.png");
     unite23 = IMG_Load("images/unite__23.png");
         char* tous_unites[liste_unite];
            tous_unites[0]= unite10;
            tous_unites[1]= unite12;
            tous_unites[2]= unite15;
            tous_unites[3]= unite14;
            tous_unites[4]= unite18;
            tous_unites[5]= unite19;
            tous_unites[6]= unite22;
            tous_unites[7]= unite23;
            tous_unites[8]= unite4;
            tous_unites[9]= unite5;
            tous_unites[10]= unite8;
            tous_unites[11]= unite9;

            int indice_humaine=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,numjoueur,NUM_UNITE_UB,num_unite,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
            int indice_enemie=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,num_enemie,NUM_UNITE_UB,numero_unite_enemie,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
            int nbr_soldats_humaine=unitesBataille[indice_humaine][NOMBRE_UNITES_UB];
            int nbr_soldats_enemie=unitesBataille[indice_enemie][NOMBRE_UNITES_UB];
            int sante_humaine=unitesBataille[indice_humaine][SANTE_UNITAIRE_UB];
            int sante_enemie=unitesBataille[indice_enemie][SANTE_UNITAIRE_UB];

                 char *ch[50];
                 sprintf(ch,"l'unit� ennemie  %d ",num_unite);
                 texte = TTF_RenderText_Blended(font,ch,blanc);
                 positionText.x =195;
                 positionText.y =30;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);
                 char *ch1[50];
                 if(contre==0){
                 sprintf(ch1,"attaque l'unit� %d ",numero_unite_enemie);
                 }else{
                 sprintf(ch1,"contre attaque l'unit� %d ",numero_unite_enemie);
                 }
                 texte = TTF_RenderText_Blended(font,ch1,blanc);
                 positionText.x =410;
                 positionText.y =30;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);
                 char *ch2[50];
                 sprintf(ch2,"nombre soldats %d ",nbr_soldats_humaine);
                 texte = TTF_RenderText_Blended(font,ch2,blanc);
                 positionText.x =90;
                 positionText.y =320;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);
                 char *ch4[50];
                 sprintf(ch4,"Sante Unitaire  %d ",sante_humaine);
                 texte = TTF_RenderText_Blended(font,ch4,blanc);
                 positionText.x =90;
                 positionText.y =340;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);

                 char *ch3[50];
                 if(indice_enemie==-1){
                 sprintf(ch3,"nombre soldats %d ",0);
                 }else{
                 sprintf(ch3,"nombre soldats %d ",nbr_soldats_enemie);
                 }
                 texte = TTF_RenderText_Blended(font,ch3,blanc);
                 positionText.x =520;
                 positionText.y =320;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);

                 char *ch5[50];
                 if(indice_enemie==-1){
                 sprintf(ch5,"Sante Unitaire  %d ",0);
                 }else{
                 sprintf(ch5,"Sante Unitaire %d ",sante_enemie);
                 }
                 texte = TTF_RenderText_Blended(font,ch5,blanc);
                 positionText.x =520;
                 positionText.y =350;
                 SDL_BlitSurface(texte, NULL, ec, &positionText);


           for(j=0;j<liste_unite;j++){
                if(numero[j]==numero_unite_enemie){
                      position.x=580;
                      position.y=120;
                    SDL_BlitSurface(tous_unites[j], NULL, ec, &position);
                }
            }


            for(j=0;j<8;j++){
                if(numero_unite[j]==num_unite ){
                     position.x=180;
                     position.y=120;
                    SDL_BlitSurface(unite_enemie[j], NULL, ec, &position);
                }
             }

    SDL_Flip(ec);
    SDL_FreeSurface(menu);
    SDL_Delay(2000);
}


int jouerTourHumainBataille (SDL_Surface *ec, int numJoueur, int numJoueurEnnemi,
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE],int nbr_tour,int type)
{
    SDL_Rect positionText;
    SDL_Rect position;
    int liste_heros=5;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL,*font1=NULL;
    SDL_Surface *suivante=NULL;
    suivante= IMG_Load("images/suivante.png");
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Tour Humaine",NULL);
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",16);
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",14);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
    int verife_attauqe=0,num_unite_attaquer;
    int verife_contre=0;
    int indice_unite_ennemie;
    int numero_unite_enemie_sauvegarde;
    int num_unite=-1,type_unite,ligne_unite,colonne_unite,fin_tour_unite, ligne_unite_attaquant,colonne_unite_attaquant;
    int ligne_unite_bataille,colonne_unite_bataille,mouvement_unite_bataille;
    int codeattaque_contre,chiffre_unite,codeattaque,type_unite_attaquant;
    int indice_unite_battaile,indice_unite,chercher_enemie,chercher_unite_attaque;
    int nouvelle_ligne,nouvelle_colonne,ligne_enemie,colonne_enemie, numero_unite_enemie;
    int choix_attaquer,choix_deplacer,num_unite_sauvegarder;
    int fin_tour_forcer=0,verifier_gagnant=-1,verifier_fintour=0;
    int continuer=1;
    int i;
    positionMenu.x =610;
    positionMenu.y =0;
    menu = IMG_Load("images/game_info.png");

    while((continuer) && (verifier_fintour==0) && (verifier_gagnant==-1)){
    afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille,type);
    SDL_BlitSurface(menu, NULL, ec, &positionMenu);
            char ch8[50]="Passer au tour ennemie";
            texte = TTF_RenderText_Blended(font,ch8,blanc);
            positionText.x =614;
            positionText.y =490;
            SDL_BlitSurface(texte, NULL, ec, &positionText);
            position.x=660;
            position.y=520;
            SDL_BlitSurface(suivante, NULL, ec, &position);
            char ast[50]="astuce :";
            texte = TTF_RenderText_Blended(font,ast,red);
            positionText.x =620;
            positionText.y =410;
            SDL_BlitSurface(texte, NULL, ec, &positionText);
            char ch0[50]="cliquer droite sur l'unit� ";
            texte = TTF_RenderText_Blended(font,ch0,blanc);
            positionText.x =617;
            positionText.y =430;
            SDL_BlitSurface(texte, NULL, ec, &positionText);
            char ch00[50]="pour consulter l'info";
            texte = TTF_RenderText_Blended(font,ch00,blanc);
            positionText.x =617;
            positionText.y =444;
            SDL_BlitSurface(texte, NULL, ec, &positionText);
            char ch2[50]="Tour Humain";
            texte = TTF_RenderText_Blended(font,ch2,blanc);
            positionText.x =640;
            positionText.y =150;
            SDL_BlitSurface(texte, NULL, ec, &positionText);
            char *nbr[50];
            sprintf(nbr,"Tour %d",nbr_tour);
            texte = TTF_RenderText_Blended(font,nbr,blanc);
            positionText.x =670;
            positionText.y =180;
            SDL_BlitSurface(texte, NULL, ec, &positionText);
            char ch9[50]="Historique de joueur :";
            texte = TTF_RenderText_Blended(font1,ch9,or_color);
            positionText.x =615;
            positionText.y =220;
            SDL_BlitSurface(texte, NULL, ec, &positionText);
      SDL_WaitEvent(&event);
        int tab[50][50];
        tab[0][0]=668;
        tab[0][1]=690;
        tab[1][0]=540;
        tab[1][1]=592;
        switch(event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
          if (event.button.button == SDL_BUTTON_LEFT)
            {
            int x_clicked=event.button.x;
              int y_clicked=event.button.y;

                int indice_j=(x_clicked/50);
                int indice_i=(y_clicked/50);
                    if((x_clicked>tab[0][0])&&(x_clicked<tab[0][1])){
                         if((y_clicked>tab[1][0])&&(y_clicked<tab[1][1])){
                                continuer=0;
                            }
                        }
                    if(indice_i<=NB_LIGNES_PLATEAUBATAILLE-1 && indice_j<=NB_COLONNES_PLATEAUBATAILLE-1){
                     if(num_unite==-1){
                    num_unite=plateauBataille[indice_i][indice_j]/1000;
                    indice_unite_battaile=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,JOUEUR_HUMAIN,NUM_UNITE_UB,num_unite,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
                 if(indice_unite_battaile!=-1 && plateauBataille[indice_i][indice_j]/100%10==numJoueur){
                  if(unitesBataille[indice_unite_battaile][FINTOUR_UB]==0){
                    char *ch1[50];
                    sprintf(ch1,"selection de l'unit� %d",num_unite);
                    texte = TTF_RenderText_Blended(font,ch1,blanc);
                    positionText.x=615;
                    positionText.y=240;
                    SDL_BlitSurface(texte, NULL, ec, &positionText);
                    if(unitesBataille[indice_unite_battaile][TYPE_UB]==1){
                    char *ch2[50];
                    sprintf(ch2,"<< cette unit� peut ");
                    texte = TTF_RenderText_Blended(font,ch2,yellow);
                    positionText.x=615;
                    positionText.y=260;
                    SDL_BlitSurface(texte, NULL, ec, &positionText);
                    char *ch22[50];
                    sprintf(ch22,"attaquer � distance >>");
                    texte = TTF_RenderText_Blended(font,ch22,yellow);
                    positionText.x=617;
                    positionText.y=270;
                    SDL_BlitSurface(texte, NULL, ec, &positionText);
                    }else{
                    char *ch3[50];
                    sprintf(ch3,"<< cette unit� ne peut");
                    texte = TTF_RenderText_Blended(font,ch3,yellow);
                    positionText.x=615;
                    positionText.y=260;
                    SDL_BlitSurface(texte, NULL, ec, &positionText);
                    char *ch33[50];
                    sprintf(ch33,"pas attaquer � distance>>");
                    texte = TTF_RenderText_Blended(font,ch33,yellow);
                    positionText.x=617;
                    positionText.y=270;
                    SDL_BlitSurface(texte, NULL, ec, &positionText);
                    }
                   afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille,type);
                    SDL_BlitSurface(menu, NULL, ec, &positionMenu);
                    SDL_Delay(2000);
                    ligne_unite_bataille=unitesBataille[indice_unite_battaile][LIGNE_UB];
                    colonne_unite_bataille=unitesBataille[indice_unite_battaile][COLONNE_UB];
                    mouvement_unite_bataille=unitesBataille[indice_unite_battaile][MOUVEMENTS_UB];
                    marquerDeplacementsPossibles(plateauBataille,ligne_unite_bataille,colonne_unite_bataille,mouvement_unite_bataille);
                    afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille);
                 }
                }
                  }else{
                       if(DoubleClickDetected() == 1){
                         indice_unite_battaile=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,numJoueur,NUM_UNITE_UB,num_unite,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
                         nouvelle_colonne=(event.button.x/50);
                         nouvelle_ligne=(event.button.y/50);
                         if(plateauBataille[nouvelle_ligne][nouvelle_colonne]==1){
                          deplacerUnite(numJoueur,indice_unite_battaile,num_unite,ligne_unite_bataille,colonne_unite_bataille,nouvelle_ligne,nouvelle_colonne,plateauBataille,unitesBataille);
                          effacerDeplacementsPossibles(plateauBataille);
                          //afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille);
                          unitesBataille[indice_unite_battaile][FINTOUR_UB]=1;
                        char *ch4[50];
                        sprintf(ch4,"l'unit�:%d deplace � (%d,%d)",num_unite,nouvelle_ligne,nouvelle_colonne);
                        texte = TTF_RenderText_Blended(font,ch4,blanc);
                        positionText.x=613;
                        positionText.y=240;
                        SDL_BlitSurface(texte, NULL, ec, &positionText);
                       afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille,type);
                        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
                        SDL_Delay(2000);
                          num_unite=-1;
                        }else
                            if(plateauBataille[nouvelle_ligne][nouvelle_colonne]/100%10==numJoueurEnnemi){
                            type_unite=unitesBataille[indice_unite_battaile][TYPE_UB];
                            ligne_unite=unitesBataille[indice_unite_battaile][LIGNE_UB];
                            colonne_unite=unitesBataille[indice_unite_battaile][COLONNE_UB];
                            fin_tour_unite=unitesBataille[indice_unite_battaile][FINTOUR_UB];
                            codeattaque=codeAttaqueSansDeplacement(JOUEUR_HUMAIN,type_unite,ligne_unite,colonne_unite,plateauBataille);
                        if(codeattaque==0){
                                 if(plateauBataille[nouvelle_ligne+1][nouvelle_colonne]==1){
                                     deplacerUnite(numJoueur,indice_unite_battaile,num_unite,ligne_unite_bataille,colonne_unite_bataille,nouvelle_ligne+1,nouvelle_colonne,plateauBataille,unitesBataille);
                                     codeattaque=codeAttaqueSansDeplacement(JOUEUR_HUMAIN,type_unite,nouvelle_ligne+1,nouvelle_colonne,plateauBataille);
                                 }else  if(plateauBataille[nouvelle_ligne-1][nouvelle_colonne]==1){
                                     deplacerUnite(numJoueur,indice_unite_battaile,num_unite,ligne_unite_bataille,colonne_unite_bataille,nouvelle_ligne-1,nouvelle_colonne,plateauBataille,unitesBataille);
                                     codeattaque=codeAttaqueSansDeplacement(JOUEUR_HUMAIN,type_unite,nouvelle_ligne-1,nouvelle_colonne,plateauBataille);
                                 }else  if(plateauBataille[nouvelle_ligne][nouvelle_colonne+1]==1){
                                     deplacerUnite(numJoueur,indice_unite_battaile,num_unite,ligne_unite_bataille,colonne_unite_bataille,nouvelle_ligne,nouvelle_colonne+1,plateauBataille,unitesBataille);
                                     codeattaque=codeAttaqueSansDeplacement(JOUEUR_HUMAIN,type_unite,nouvelle_ligne,nouvelle_colonne+1,plateauBataille);
                                 }else  if(plateauBataille[nouvelle_ligne][nouvelle_colonne-1]==1){
                                     deplacerUnite(numJoueur,indice_unite_battaile,num_unite,ligne_unite_bataille,colonne_unite_bataille,nouvelle_ligne,nouvelle_colonne-1,plateauBataille,unitesBataille);
                                     codeattaque=codeAttaqueSansDeplacement(JOUEUR_HUMAIN,type_unite,nouvelle_ligne,nouvelle_colonne-1,plateauBataille);
                                 }
                        }
                        if(codeattaque>0){
                              numero_unite_enemie=plateauBataille[nouvelle_ligne][nouvelle_colonne]/1000;
                              indice_unite_ennemie=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,numJoueurEnnemi,NUM_UNITE_UB,numero_unite_enemie,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
                              type_unite_attaquant=unitesBataille[indice_unite_ennemie][TYPE_UB];
                              ligne_unite_attaquant=unitesBataille[indice_unite_ennemie][LIGNE_UB];
                              colonne_unite_attaquant=unitesBataille[indice_unite_ennemie][COLONNE_UB];
                            if(uniteAttaquable(codeattaque,ligne_unite,colonne_unite,ligne_unite_attaquant,colonne_unite_attaquant)==1){
                                char *ch5[50];
                                sprintf(ch5,"l'unit� %d attaque  ",num_unite);
                                texte = TTF_RenderText_Blended(font,ch5,blanc);
                                positionText.x=620;
                                positionText.y=240;
                                SDL_BlitSurface(texte, NULL, ec, &positionText);
                                char *ch55[50];
                                sprintf(ch55,"l'unit� %d ",numero_unite_enemie);
                                texte = TTF_RenderText_Blended(font,ch55,blanc);
                                positionText.x=623;
                                positionText.y=255;
                                SDL_BlitSurface(texte, NULL, ec, &positionText);
                                afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille,type);
                                SDL_BlitSurface(menu, NULL, ec, &positionMenu);
                                SDL_Delay(2000);
                                int attaquer=numJoueur;
                                int contre=0;
                                affiche_attaque_humaine(ec,num_unite,numero_unite_enemie,numJoueur,numJoueurEnnemi,unitesBataille,attaquer,contre);
                                fin_bataille(ec);
                                attaque(indice_unite_battaile,indice_unite_ennemie,JOUEUR_HUMAIN,numJoueurEnnemi,ligne_unite_attaquant,colonne_unite_attaquant,plateauBataille,unitesBataille,joueurs);
                             //   affiche_attaque_humaine(ec,num_unite,numero_unite_enemie,numJoueur,numJoueurEnnemi,unitesBataille,attaquer,contre);
                              afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille,type);
                                chiffre_unite=codeattaque%10;
                                if(chiffre_unite>1 && indice_unite_ennemie!=-1){
                                codeattaque_contre=codeAttaqueSansDeplacement(numJoueurEnnemi,type_unite_attaquant,ligne_unite_attaquant,colonne_unite_attaquant,plateauBataille);
                               if(codeattaque_contre>1){
                                 int contre=1;
                                 int attaquer=numJoueurEnnemi;
                                 affiche_attaque_ennemie(ec,numero_unite_enemie,num_unite,numJoueurEnnemi,numJoueur,unitesBataille,attaquer,contre);
                                 fin_bataille(ec);
                                 attaque(indice_unite_ennemie,indice_unite_battaile,numJoueurEnnemi,JOUEUR_HUMAIN,ligne_unite,colonne_unite,plateauBataille,unitesBataille,joueurs);
                                 //affiche_attaque_ennemie(ec,numero_unite_enemie,num_unite,numJoueurEnnemi,numJoueur,unitesBataille,attaquer,contre);
                                 afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille);
                             }
                            }
                        }
                    }
                       effacerDeplacementsPossibles(plateauBataille);
                       // afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille);
                       unitesBataille[indice_unite_battaile][FINTOUR_UB]=1;
                       num_unite=-1;
                        }else if(plateauBataille[nouvelle_ligne][nouvelle_colonne]==0){
                            num_unite=-1;
                        }
                       }else{
                             int case_plateau=plateauBataille[indice_i][indice_j];
                                if(case_plateau==0 || case_plateau==1){
                                 effacerDeplacementsPossibles(plateauBataille);
                                 afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille,type);
                                 num_unite=-1;
                                }else{
                                int num_unite2=plateauBataille[indice_i][indice_j]/1000;
                                indice_unite_battaile=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,JOUEUR_HUMAIN,NUM_UNITE_UB,num_unite2,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
                                if(indice_unite_battaile!=-1){
                                    if(unitesBataille[indice_unite_battaile][FINTOUR_UB]==0){
                                    num_unite=num_unite2;
                                    ligne_unite_bataille=unitesBataille[indice_unite_battaile][LIGNE_UB];
                                    colonne_unite_bataille=unitesBataille[indice_unite_battaile][COLONNE_UB];
                                    mouvement_unite_bataille=unitesBataille[indice_unite_battaile][MOUVEMENTS_UB];
                                    marquerDeplacementsPossibles(plateauBataille,ligne_unite_bataille,colonne_unite_bataille,mouvement_unite_bataille);
                                    afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille,type);
                                    }
                                }
                                }
                       }
                 }
        }
         } else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                   int info_colonne=(event.button.x/50);
                   int info_ligne=(event.button.y/50);
                   if(info_ligne<=NB_LIGNES_PLATEAUBATAILLE-1 && info_colonne<=NB_COLONNES_PLATEAUBATAILLE-1){
                            int x=plateauBataille[info_ligne][info_colonne]/1000;
                            indice_unite_battaile=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,JOUEUR_HUMAIN,NUM_UNITE_UB,x,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
                            if(indice_unite_battaile!=-1){
                                info_unite_bataille(ec,x,unitesBataille);
                                afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille,type);
                            }
                   }
            }
            break;
         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            }
        }
            verifier_fintour=verificationFinTours(numJoueur,unitesBataille);
            verifier_gagnant=verificationGagnant(numJoueur,numJoueurEnnemi,unitesBataille);
           SDL_Flip(ec);
         }
    SDL_FreeSurface(menu);
    return verifier_gagnant;
}

int jouerTourEnnemiBataille (SDL_Surface *ec, int numJoueur, int numJoueurEnnemi,
    int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
    int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
    int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
    int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE],int tour_enemi,int type){
    SDL_Rect positionText;
    SDL_Rect position;
    int liste_heros=5;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Tour Enemie",NULL);
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",16);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
     int verifier_fintour=0,verifier_gagnant=-1;
     int verifier;
     int num_unite_attaque,num_unite_attaquante,num_unite_attaquante_nouvelle;
     int mouvement_unite_attaque;
     int type_unite_attaque,type_unite_joueur ,type_unite_enemei;
     int codeattaque_contre,codeattaque,nouvelle_codeattaque;
     int indice_unite_attaque,indice_unite_attaquante,nouvelle_indice_unite_attaque,indice_enemi,indice_enemi_apres_attaque;
     int ligne_unite_attaquante,colonne_unite_attaquante,ligne_corrent,colonne_corrent;
     int nouvelle_ligne_joueur,nouvelle_colonne_joueur;
     int ligne_enemie_nouvelle,colonne_enemie_nouvelle;
     int ligne_ennemi,colonne_ennemi;
     int continuer=1;
     int i;
    positionMenu.x =610;
    positionMenu.y =0;
    menu = IMG_Load("images/game_info.png");
    afficherPlateauBataille(ec,numJoueurEnnemi,numJoueur,plateauBataille,unitesBataille,type);
    SDL_BlitSurface(menu, NULL, ec, &positionMenu);
    while((continuer) && (verifier_gagnant==-1) && (verifier_fintour==0)){
        afficherPlateauBataille(ec,numJoueurEnnemi,numJoueur,plateauBataille,unitesBataille);
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
            char ch5[50]="Tour Enemie";
            texte = TTF_RenderText_Blended(font,ch5,blanc);
            positionText.x =630;
            positionText.y =150;
            SDL_BlitSurface(texte, NULL, ec, &positionText);
            char *nbr2[50];
            sprintf(nbr2,"Tour %d",tour_enemi);
            texte = TTF_RenderText_Blended(font,nbr2,blanc);
            positionText.x =660;
            positionText.y =180;
            SDL_BlitSurface(texte, NULL, ec, &positionText);
            char ch10[50]="Historique de l'ennemie :";
            texte = TTF_RenderText_Blended(font,ch10,red);
            positionText.x =615;
            positionText.y =220;
            SDL_BlitSurface(texte, NULL, ec, &positionText);
        indice_unite_attaque=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,numJoueur,FINTOUR_UB,0,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
        num_unite_attaque=unitesBataille[indice_unite_attaque][NUM_UNITE_UB];
        char *ch[50];
        sprintf(ch,"selection de l'unit� %d",num_unite_attaque);
        texte = TTF_RenderText_Blended(font,ch,blanc);
        positionText.x=615;
        positionText.y=240;
        SDL_BlitSurface(texte, NULL, ec, &positionText);
        SDL_Delay(1000);
        ligne_corrent=unitesBataille[indice_unite_attaque][LIGNE_UB];
        colonne_corrent=unitesBataille[indice_unite_attaque][COLONNE_UB];
        type_unite_attaque=unitesBataille[indice_unite_attaque][TYPE_UB];
        codeattaque=codeAttaqueSansDeplacement(numJoueur,type_unite_attaque,ligne_corrent,colonne_corrent,plateauBataille);
        num_unite_attaquante=choixAutomatiqueUniteEnnemie(numJoueurEnnemi,codeattaque,ligne_corrent,colonne_corrent,plateauBataille,unitesBataille);
        indice_unite_attaquante=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,numJoueurEnnemi,NUM_UNITE_UB,num_unite_attaquante,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
          if(codeattaque==0){
                mouvement_unite_attaque=unitesBataille[indice_unite_attaque][MOUVEMENTS_UB];
                marquerDeplacementsPossibles(plateauBataille,ligne_corrent,colonne_corrent,mouvement_unite_attaque);
                afficherPlateauBataille(ec,numJoueurEnnemi,numJoueur,plateauBataille,unitesBataille,type);
                SDL_BlitSurface(menu, NULL, ec, &positionMenu);
                char ch2[50]="Tour Enemie";
                texte = TTF_RenderText_Blended(font,ch2,blanc);
                positionText.x =630;
                positionText.y =150;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                char *nbr[50];
                sprintf(nbr,"Tour %d",tour_enemi);
                texte = TTF_RenderText_Blended(font,nbr,blanc);
                positionText.x =660;
                positionText.y =180;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                SDL_Delay(700);
                char ch10[50]="Historique de l'ennemie :";
                texte = TTF_RenderText_Blended(font,ch10,red);
                positionText.x =615;
                positionText.y =220;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                deplacementAutomatiqueUnite(numJoueur,type_unite_attaque,plateauBataille,ligne_corrent,colonne_corrent,unitesBataille,indice_unite_attaque);
                afficherPlateauBataille(ec,numJoueurEnnemi,numJoueur,plateauBataille,unitesBataille,type);
                SDL_BlitSurface(menu, NULL, ec, &positionMenu);
                char ch22[50]="Tour Ennemie";
                texte = TTF_RenderText_Blended(font,ch22,blanc);
                positionText.x =630;
                positionText.y =150;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                char *nbr11[50];
                sprintf(nbr11,"Tour %d",tour_enemi);
                texte = TTF_RenderText_Blended(font,nbr11,blanc);
                positionText.x =660;
                positionText.y =180;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                SDL_Delay(700);
                char ch100[50]="Historique de l'ennemie :";
                texte = TTF_RenderText_Blended(font,ch100,red);
                positionText.x =615;
                positionText.y =220;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                    char *ch1[50];
                    sprintf(ch1,"l'unit� %d deplace ",num_unite_attaque);
                    texte = TTF_RenderText_Blended(font,ch1,blanc);
                    positionText.x=615;
                    positionText.y=240;
                    SDL_BlitSurface(texte, NULL, ec, &positionText);
                    SDL_Delay(1000);
                    effacerDeplacementsPossibles(plateauBataille);
                    afficherPlateauBataille(ec,numJoueurEnnemi,numJoueur,plateauBataille,unitesBataille,type);
                    SDL_BlitSurface(menu, NULL, ec, &positionMenu);
                    char ch4[50]="Tour Ennemie";
                    texte = TTF_RenderText_Blended(font,ch4,blanc);
                    positionText.x =630;
                    positionText.y =150;
                    SDL_BlitSurface(texte, NULL, ec, &positionText);
                    char *nbr1[50];
                    sprintf(nbr,"Tour %d",tour_enemi);
                    texte = TTF_RenderText_Blended(font,nbr1,blanc);
                    positionText.x =660;
                    positionText.y =180;
                    SDL_BlitSurface(texte, NULL, ec, &positionText);
                    SDL_Delay(700);
                    ch10[50]="Historique de l'ennemie :";
                    texte = TTF_RenderText_Blended(font,ch10,red);
                    positionText.x =615;
                    positionText.y =220;
                    SDL_BlitSurface(texte, NULL, ec, &positionText);
                    unitesBataille[indice_unite_attaque][FINTOUR_UB]=1;
            }else if(codeattaque>1){
                afficherPlateauBataille(ec,numJoueurEnnemi,numJoueur,plateauBataille,unitesBataille);
                SDL_BlitSurface(menu, NULL, ec, &positionMenu);
                ligne_corrent=unitesBataille[indice_unite_attaque][LIGNE_UB];
                colonne_corrent=unitesBataille[indice_unite_attaque][COLONNE_UB];
                num_unite_attaquante_nouvelle=choixAutomatiqueUniteEnnemie(numJoueurEnnemi,codeattaque,ligne_corrent,colonne_corrent,plateauBataille,unitesBataille);
                indice_enemi=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,numJoueurEnnemi,NUM_UNITE_UB,num_unite_attaquante_nouvelle,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
                ligne_ennemi=unitesBataille[indice_enemi][LIGNE_UB];
                colonne_ennemi=unitesBataille[indice_enemi][COLONNE_UB];
                type_unite_enemei=unitesBataille[indice_enemi][TYPE_UB];
                verifier=uniteAttaquable(codeattaque,ligne_corrent,colonne_corrent,ligne_ennemi,colonne_ennemi);
                    if(verifier==1 && indice_enemi!=-1){
                      int attaquer=numJoueur;
                      int contre=0;
                       afficherPlateauBataille(ec,numJoueurEnnemi,numJoueur,plateauBataille,unitesBataille,type);
                        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
                        char *ch3[50];
                        sprintf(ch3,"l'unit� %d attaque l'unit� %d",num_unite_attaque,num_unite_attaquante_nouvelle);
                        texte = TTF_RenderText_Blended(font,ch3,blanc);
                        positionText.x=615;
                        positionText.y=240;
                        SDL_BlitSurface(texte, NULL, ec, &positionText);
                        SDL_Delay(1000);
                        affiche_attaque_ennemie(ec,num_unite_attaque,num_unite_attaquante_nouvelle,numJoueur,numJoueurEnnemi,unitesBataille,attaquer,contre);
                        //fin_bataille(ec);
                        attaque(indice_unite_attaque,indice_enemi,numJoueur,numJoueurEnnemi,ligne_ennemi,colonne_ennemi,plateauBataille,unitesBataille,joueurs);
                      //affiche_attaque_ennemie(ec,num_unite_attaque,num_unite_attaquante_nouvelle,numJoueur,numJoueurEnnemi,unitesBataille,attaquer,contre);
                      indice_enemi_apres_attaque=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,numJoueurEnnemi,NUM_UNITE_UB,num_unite_attaquante_nouvelle,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
                      codeattaque_contre=codeAttaqueSansDeplacement(numJoueurEnnemi,type_unite_enemei,ligne_ennemi,colonne_ennemi,plateauBataille);
                        if(codeattaque_contre>1 && indice_enemi_apres_attaque!=-1){
                            contre=1;
                            attaquer=numJoueurEnnemi;
                            affiche_attaque_humaine(ec,num_unite_attaquante_nouvelle,num_unite_attaque,numJoueurEnnemi,numJoueur,unitesBataille,attaquer,contre);
                           // fin_bataille(ec);
                            attaque(indice_enemi_apres_attaque,indice_unite_attaque,numJoueurEnnemi,numJoueur,nouvelle_ligne_joueur,nouvelle_colonne_joueur,plateauBataille,unitesBataille,joueurs);
                            //afficherPlateauBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,plateauBataille,unitesBataille);
                           //affiche_attaque_humaine(ec,num_unite_attaquante_nouvelle,num_unite_attaque,numJoueurEnnemi,numJoueur,unitesBataille,attaquer,contre);
                    }
                    unitesBataille[indice_unite_attaque][FINTOUR_UB]=1;
           }
        }else if(codeattaque==1){
             num_unite_attaquante=choixAutomatiqueUniteEnnemie(numJoueurEnnemi,codeattaque,ligne_corrent,colonne_corrent,plateauBataille,unitesBataille);
            // afficherPlateauBataille(ec,numJoueurEnnemi,numJoueur,plateauBataille,unitesBataille,type);
             SDL_BlitSurface(menu, NULL, ec, &positionMenu);
             char *ch6[50];
             sprintf(ch6,"selection de l'unit� � attaquer %d",num_unite_attaquante);
             texte = TTF_RenderText_Blended(font,ch6,blanc);
             positionText.x=615;
             positionText.y=240;
             SDL_BlitSurface(texte, NULL, ec, &positionText);
             SDL_Delay(1000);
          indice_unite_attaquante=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,numJoueurEnnemi,NUM_UNITE_UB,num_unite_attaquante,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
          ligne_enemie_nouvelle=unitesBataille[indice_unite_attaquante][LIGNE_UB];
          colonne_enemie_nouvelle=unitesBataille[indice_unite_attaquante][COLONNE_UB];
          int contre=0;
          int attaquer=0;
          affiche_attaque_ennemie(ec,num_unite_attaque,num_unite_attaquante,numJoueur,numJoueurEnnemi,unitesBataille,attaquer,contre);
          //fin_bataille(ec);
          attaque(indice_unite_attaque,indice_unite_attaquante,numJoueur,numJoueurEnnemi,ligne_enemie_nouvelle,colonne_enemie_nouvelle,plateauBataille,unitesBataille,joueurs);
         // affiche_attaque_ennemie(ec,num_unite_attaque,num_unite_attaquante,numJoueur,numJoueurEnnemi,unitesBataille,attaquer,contre);
            //afficherPlateauBataille(ec,numJoueurEnnemi,numJoueur,plateauBataille,unitesBataille,type);
          SDL_BlitSurface(menu, NULL, ec, &positionMenu);
          char *ch7[50];
          sprintf(ch7,"l'unit� %d attaque l'unit� %d",num_unite_attaque,num_unite_attaquante);
          texte = TTF_RenderText_Blended(font,ch7,blanc);
          positionText.x=615;
          positionText.y=240;
          SDL_BlitSurface(texte, NULL, ec, &positionText);
          SDL_Delay(1000);
          unitesBataille[indice_unite_attaque][FINTOUR_UB]=1;
        }
            verifier_fintour=verificationFinTours(numJoueur,unitesBataille);
            verifier_gagnant=verificationGagnant(numJoueur,numJoueurEnnemi,unitesBataille);
            SDL_BlitSurface(menu, NULL, ec, &positionMenu);
            SDL_Flip(ec);
         }
    SDL_FreeSurface(menu);
    return verifier_gagnant;
}

void afficherPlateauBataille (SDL_Surface* ec, int numJoueur, int numJoueurEnnemi,
    int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
    int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE],int type){
    SDL_Surface *cadre = NULL, *cadre_placer = NULL,*cadre_enemie=NULL;
    SDL_WM_SetCaption("Plateau",NULL);
    int  i = 0, j = 0;
    if(type==0){
    cadre=IMG_Load("images/glace.png");
    cadre_placer= IMG_Load("images/placer_glace.png");
    }else if(type==1){
    cadre=IMG_Load("images/image.png");
    cadre_placer= IMG_Load("images/image_1.png");
    }
    cadre_enemie= IMG_Load("images/cadre_enemie.png");
    SDL_Rect position;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    SDL_Color blanc = { 255, 255, 255};
    SDL_Color or_color={255, 179, 0};
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;
    SDL_Surface* texte1;
    SDL_Rect positionText;
    font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",19);
    font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",19);
    SDL_Surface  *unite4 = NULL, *unite5 = NULL, *unite8 = NULL, *unite9 = NULL,*unite10 = NULL, *unite12 = NULL, *unite14 = NULL, *unite18 = NULL,*unite19 = NULL,
        *unite22 = NULL, *unite23 = NULL,*unite15 = NULL,*joueur_humaine=NULL;
    SDL_Surface *enmie1=NULL,*enmie2=NULL, *enmie3=NULL, *enmie4 = NULL,*heros1=NULL, *heros2 = NULL, *heros3 = NULL, *heros4 = NULL,*heros5 = NULL,
      *unite1_enemie1 = NULL, *unite2_enemie1 = NULL, *unite1_enemie2 = NULL,*unite2_enemie2 = NULL,
       *unite1_enemie3 = NULL, *unite2_enemie3 = NULL, *unite1_enemie4 = NULL,*unite2_enemie4 = NULL;
     enmie1= IMG_Load("images/enemie1.png");
     enmie2= IMG_Load("images/enemie2.png");
     enmie3= IMG_Load("images/enemie3.png");
     enmie4= IMG_Load("images/enemie4.png");
     heros1= IMG_Load("images/unite8.png");
     heros2= IMG_Load("images/unite9.png");
     heros3= IMG_Load("images/unite14.png");
     heros4= IMG_Load("images/unite10.png");
     heros5= IMG_Load("images/unite12.png");

     unite1_enemie1= IMG_Load("images/unite_enemie1.png");
     unite2_enemie1= IMG_Load("images/unite1_enemie1.png");
     unite1_enemie2= IMG_Load("images/unite_enemie2.png");
     unite2_enemie2= IMG_Load("images/unite1_enemie2.png");
     unite1_enemie3= IMG_Load("images/unite_enemie3.png");
     unite2_enemie3= IMG_Load("images/unite1_enemie3.png");
     unite1_enemie4= IMG_Load("images/unite_enemie4.png");
     unite2_enemie4= IMG_Load("images/unite1_enemie4.png");

     unite4 = IMG_Load("images/unite__4.png");
     unite15 = IMG_Load("images/unite__15.png");
     unite5 = IMG_Load("images/unite__5.png");
     unite8 = IMG_Load("images/unite__8.png");
     unite9 = IMG_Load("images/unite__9.png");
     unite14 = IMG_Load("images/unite__14.png");
     unite10 = IMG_Load("images/unite__10.png");
     unite12= IMG_Load("images/unite__12.png");
     unite18 = IMG_Load("images/unite__18.png");
     unite19 = IMG_Load("images/unite__19.png");
     unite22= IMG_Load("images/unite__22.png");
     unite23 = IMG_Load("images/unite__23.png");
     joueur_humaine=IMG_Load("images/humaine.png");
     int liste_unite=12;
     int compte=0;
         char* phrase[5];
             phrase[0]= heros1;
             phrase[1]= heros2;
             phrase[2]= heros3;
             phrase[3]= heros4;
             phrase[4]= heros5;
           char *tous_enemie[8];
              tous_enemie[0]=unite1_enemie1;
              tous_enemie[1]=unite2_enemie1;
              tous_enemie[2]=unite1_enemie2;
              tous_enemie[3]=unite2_enemie2;
              tous_enemie[4]=unite1_enemie3;
              tous_enemie[5]=unite2_enemie3;
              tous_enemie[6]=unite1_enemie4;
              tous_enemie[7]=unite2_enemie4;
        int  numero_unite_enemeie[8];
            numero_unite_enemeie[0]=5;
            numero_unite_enemeie[1]=19;
            numero_unite_enemeie[2]=9;
            numero_unite_enemeie[3]=10;
            numero_unite_enemeie[4]=22;
            numero_unite_enemeie[5]=23;
            numero_unite_enemeie[6]=14;
            numero_unite_enemeie[7]=15;

        int numero[liste_unite];
                numero[0]=10;
                numero[1]=12;
                numero[2]=15;
                numero[3]=14;
                numero[4]=18;
                numero[5]=19;
                numero[6]=22;
                numero[7]=23;
                numero[8]=4;
                numero[9]=5;
                numero[10]=8;
                numero[11]=9;
         int tab[50];
         for(i=0;i<12;i++){
           int indice_humaine=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,0,NUM_UNITE_UB,numero[i],NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
           int nbr_soldats=unitesBataille[indice_humaine][NOMBRE_UNITES_UB];
           tab[i]=nbr_soldats;
         }

         char* ch_10[50];
         char* ch_12[50];
         char* ch_15[50];
         char* ch_14[50];
         char* ch_18[50];
         char* ch_19[50];
         char* ch_22[50];
         char* ch_23[50];
         char* ch_4[50];
         char* ch_5[50];
         char* ch_8[50];
         char* ch_9[50];
        sprintf(ch_10,"%d",tab[0]);
        sprintf(ch_12,"%d",tab[1]);
        sprintf(ch_15,"%d",tab[2]);
        sprintf(ch_14,"%d",tab[3]);
        sprintf(ch_18,"%d",tab[4]);
        sprintf(ch_19,"%d",tab[5]);
        sprintf(ch_22,"%d",tab[6]);
        sprintf(ch_23,"%d",tab[7]);
        sprintf(ch_4,"%d",tab[8]);
        sprintf(ch_5,"%d",tab[9]);
        sprintf(ch_8,"%d",tab[10]);
        sprintf(ch_9,"%d",tab[11]);
            char* tous_nbr_solsats_humiane[liste_unite];
                tous_nbr_solsats_humiane[0]=ch_10;
                tous_nbr_solsats_humiane[1]=ch_12;
                tous_nbr_solsats_humiane[2]=ch_15;
                tous_nbr_solsats_humiane[3]=ch_14;
                tous_nbr_solsats_humiane[4]=ch_18;
                tous_nbr_solsats_humiane[5]=ch_19;
                tous_nbr_solsats_humiane[6]=ch_22;
                tous_nbr_solsats_humiane[7]=ch_23;
                tous_nbr_solsats_humiane[8]=ch_4;
                tous_nbr_solsats_humiane[9]=ch_5;
                tous_nbr_solsats_humiane[10]=ch_8;
                tous_nbr_solsats_humiane[11]=ch_9;

         int tab_ennemie[50];
         for(i=0;i<12;i++){
           int indice_enemie=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UB,numJoueurEnnemi,NUM_UNITE_UB,numero_unite_enemeie[i],NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
           int nbr_soldats=unitesBataille[indice_enemie][NOMBRE_UNITES_UB];
           tab_ennemie[i]=nbr_soldats;
         }
         char* ch_enemie_8[50];
         char* ch_enemie_19[50];
         char* ch_enemie_9[50];
         char* ch_enemie_10[50];
         char* ch_enemie_22[50];
         char* ch_enemie_23[50];
         char* ch_enemie_14[50];
         char* ch_enemie_15[50];
        sprintf(ch_enemie_8,"%d",tab_ennemie[0]);
        sprintf(ch_enemie_19,"%d",tab_ennemie[1]);
        sprintf(ch_enemie_9,"%d",tab_ennemie[2]);
        sprintf(ch_enemie_10,"%d",tab_ennemie[3]);
        sprintf(ch_enemie_22,"%d",tab_ennemie[4]);
        sprintf(ch_enemie_23,"%d",tab_ennemie[5]);
        sprintf(ch_enemie_14,"%d",tab_ennemie[6]);
        sprintf(ch_enemie_15,"%d",tab_ennemie[7]);
            char* tous_nbr_solsats_enemie[8];
                tous_nbr_solsats_enemie[0]=ch_enemie_8;
                tous_nbr_solsats_enemie[1]=ch_enemie_19;
                tous_nbr_solsats_enemie[2]=ch_enemie_9;
                tous_nbr_solsats_enemie[3]=ch_enemie_10;
                tous_nbr_solsats_enemie[4]=ch_enemie_22;
                tous_nbr_solsats_enemie[5]=ch_enemie_23;
                tous_nbr_solsats_enemie[6]=ch_enemie_14;
                tous_nbr_solsats_enemie[7]=ch_enemie_15;


       char* tous_unites[liste_unite];
            tous_unites[0]= unite10;
            tous_unites[1]= unite12;
            tous_unites[2]= unite15;
            tous_unites[3]= unite14;
            tous_unites[4]= unite18;
            tous_unites[5]= unite19;
            tous_unites[6]= unite22;
            tous_unites[7]= unite23;
            tous_unites[8]= unite4;
            tous_unites[9]= unite5;
            tous_unites[10]= unite8;
            tous_unites[11]= unite9;

    for (i =0 ; i < NB_LIGNES_PLATEAUBATAILLE ; i++)
        {
            for (j = 0 ; j <   NB_COLONNES_PLATEAUBATAILLE; j++)
            {
                int chiffre_dizaine=plateauBataille[i][j]/100%10;
                int chiffre=plateauBataille[i][j]/1000;
                position.y =i*49;
                position.x=j*50;
              	if(plateauBataille[i][j]==0){
                    SDL_BlitSurface(cadre, NULL, ec, &position);
                }
                else if(plateauBataille[i][j]==1){
                    SDL_BlitSurface(cadre_placer, NULL, ec, &position);
                }
                else if(plateauBataille[i][j]%100==10){
                    if(chiffre_dizaine==0){
                        for(int compt=0;compt<liste_unite;compt++){
                            if(numero[compt]==chiffre){
                                 SDL_BlitSurface(cadre, NULL, ec, &position);
                                 SDL_BlitSurface(tous_unites[compt], NULL, ec, &position);
                                 texte = TTF_RenderText_Blended(font1,tous_nbr_solsats_humiane[compt],or_color);
                                 SDL_BlitSurface(texte, NULL, ec, &position);
                            }
                        }
                   }
                   else{
                    for(int compt=0;compt<8;compt++){
                        if(numero_unite_enemeie[compt]==chiffre){
                                 SDL_BlitSurface(cadre_enemie, NULL, ec, &position);
                                 SDL_BlitSurface(tous_enemie[compt], NULL, ec, &position);
                                 texte = TTF_RenderText_Blended(font1,tous_nbr_solsats_enemie[compt],or_color);
                                 SDL_BlitSurface(texte, NULL, ec, &position);
                        }

                    }
                   }
                        }
                    }
                }
        SDL_Flip(ec);
        SDL_EnableKeyRepeat(0,0);
        SDL_FreeSurface(cadre);
        SDL_FreeSurface(cadre_placer);
}

 void initialiserBataille ( int numJoueurHumain, int numJoueurEnnemi,
		int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
		int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS],
		int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
		int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE])
{
	    int j,i;
			int vide_joueur_humaine,vide_enemei,colonne_joueur_humaine=0,colonne_enmei=0;

 				for(i=0;i<=NB_LIGNES_UNITESBATAILLE-1;i++){
					for(j=0;j<=NB_COLONNES_UNITESBATAILLE-1;j++){
						unitesBataille[i][j]=-1;

                    }
                }

                for(i=0;i<=NB_LIGNES_PLATEAUBATAILLE-1;i++){
					for(j=0;j<=NB_COLONNES_PLATEAUBATAILLE-1;j++){
						plateauBataille[i][j]=0;

                    }
                }

    for(i=0;i<NB_LIGNES_UNITESJOUEURS;i++){
		if(unitesJoueurs[i][NUM_JOUEUR_UJ]==numJoueurHumain && unitesJoueurs[i][UNITE_ACTIVE]==1){
			 vide_joueur_humaine=chercherIndiceAvecUneCondition(NUM_JOUEUR_UB,-1,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
				if(vide_joueur_humaine<NB_LIGNES_UNITESBATAILLE-1){
			    for( j=0;j<=NOMBRE_UJ;j++){
				 unitesBataille[vide_joueur_humaine][j]=unitesJoueurs[i][j];
		    	    }
				unitesBataille[vide_joueur_humaine][LIGNE_UB]=NB_LIGNES_UNITESBATAILLE-1;
				unitesBataille[vide_joueur_humaine][COLONNE_UB]=colonne_joueur_humaine;
				unitesBataille[vide_joueur_humaine][FINTOUR_UB]=0;
				unitesBataille[vide_joueur_humaine][SANTE_UNITAIRE_UB]= unitesBataille[vide_joueur_humaine][SANTE_RESTANTE_UB];
				plateauBataille[NB_LIGNES_PLATEAUBATAILLE-1][colonne_joueur_humaine]=(unitesBataille[vide_joueur_humaine][NUM_UNITE_UB]*1000)+(0*100)+10;
				colonne_joueur_humaine+=2;
				}
		}
			if(unitesJoueurs[i][NUM_JOUEUR_UJ]==numJoueurEnnemi && unitesJoueurs[i][UNITE_ACTIVE]==1){
				vide_enemei=chercherIndiceAvecUneCondition (NUM_JOUEUR_UB,-1,NB_LIGNES_UNITESBATAILLE,NB_COLONNES_UNITESBATAILLE,unitesBataille);
				if(vide_enemei<NB_LIGNES_UNITESBATAILLE/2){
			    for( j=0;j<=NOMBRE_UJ;j++){
                unitesBataille[vide_enemei][j]=unitesJoueurs[i][j];
                }
				unitesBataille[vide_enemei][LIGNE_UB]=0;
				unitesBataille[vide_enemei][COLONNE_UB]=colonne_enmei;
				unitesBataille[vide_enemei][FINTOUR_UB]=0;
				unitesBataille[vide_enemei][SANTE_UNITAIRE_UB]= unitesBataille[vide_enemei][SANTE_RESTANTE_UB];
				plateauBataille[0][colonne_enmei]=(unitesBataille[vide_enemei][NUM_UNITE_UB]*1000)+(numJoueurEnnemi*100)+10;
				colonne_enmei+=2;
				}
		}
    }
}

void victory(SDL_Surface* ec,int indice_joueur_enemie,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS]){
    SDL_Rect position;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Victory",NULL);
    menu = IMG_Load("images/winner.png");
    int indiceOption=0;
    positionMenu.x = 0;
    int j;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",18);
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",16);
        font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",13);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
        SDL_Rect positionText;

    while (continuer)
    {
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
            int i=0;
            int y_depart=110,y_depart_titre=90;
            int x_depart=10,x_depart_titre=80;
            int claim[12][12];

                claim[0][0]=264;
                claim[0][1]=515;
                claim[0][2]=502;
                claim[0][3]=583;

        SDL_WaitEvent(&event);
        switch(event.type)
        {
       case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            { int x_clicked=event.button.x;
              int y_clicked=event.button.y;

                for(i=0;i<4;i++){
                    if((x_clicked>claim[i][0])&&(x_clicked<claim[i][1])){
                         if((y_clicked>claim[i][2])&&(y_clicked<claim[i][3])){
                                    continuer=0;

                            }
                                }
                }
                char *ch[50];
                sprintf(ch,"%d",x_clicked);
                texte = TTF_RenderText_Blended(font,ch,pink);
                positionText.x =10;
                positionText.y =48;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                char *ch1[50];
                sprintf(ch1,"%d",y_clicked);
                texte = TTF_RenderText_Blended(font,ch1,pink);
                positionText.x =80;
                positionText.y =50;
                SDL_BlitSurface(texte, NULL, ec, &positionText);

            }break;
        }
                char *or[50];
                sprintf(or,"+%d",joueurs[indice_joueur_enemie][OR]);
                char*fer[50];
                sprintf(fer,"+%d",joueurs[indice_joueur_enemie][FER]);
                char*pieree[50];
                sprintf(pieree,"+%d",joueurs[indice_joueur_enemie][PIERRE]);
                char*bois[50];
                sprintf(bois,"+%d",joueurs[indice_joueur_enemie][BOIS]);
                char*gemme_rouge[50];
                sprintf(gemme_rouge,"+%d",joueurs[indice_joueur_enemie][GEMME_ROUGE]);
                char *fortune[40];
                fortune[0]=or;
                fortune[1]=fer;
                fortune[2]=pieree;
                fortune[3]=bois;
                fortune[4]=gemme_rouge;
                for(int i=0;i<5;i++){
                    texte = TTF_RenderText_Blended(font,fortune[i],blanc);
                    position.x=150*i+75;
                    position.y=413;
                    SDL_BlitSurface(texte, NULL, ec, &position);
                }
                SDL_Flip(ec);
         }
   SDL_FreeSurface(menu);
}

void lose(SDL_Surface* ec,int joueur_enemie){
    SDL_Rect position;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Victory",NULL);
    menu = IMG_Load("images/lose.png");
    int indiceOption=0;
    positionMenu.x = 0;
    int j;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",28);
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",16);
        font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",13);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
        SDL_Rect positionText;

    while (continuer)
    {
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
            int i=0;
            int y_depart=110,y_depart_titre=90;
            int x_depart=10,x_depart_titre=80;
            int ok[12][12];

                ok[0][0]=312;
                ok[0][1]=481;
                ok[0][2]=521;
                ok[0][3]=584;

        SDL_WaitEvent(&event);
        switch(event.type)
        {
       case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            { int x_clicked=event.button.x;
              int y_clicked=event.button.y;
                for(i=0;i<4;i++){
                    if((x_clicked>ok[i][0])&&(x_clicked<ok[i][1])){
                         if((y_clicked>ok[i][2])&&(y_clicked<ok[i][3])){
                                    continuer=0;
                            }
                                }
                }

                char *ch[50];
                sprintf(ch,"%d",x_clicked);
                texte = TTF_RenderText_Blended(font,ch,pink);
                positionText.x =10;
                positionText.y =48;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                char *ch1[50];
                sprintf(ch1,"%d",y_clicked);
                texte = TTF_RenderText_Blended(font,ch1,pink);
                positionText.x =80;
                positionText.y =50;
                SDL_BlitSurface(texte, NULL, ec, &positionText);

            }break;
        }
                char *ch2[50];
                sprintf(ch2,"Le joueur %d a gagn� la bataille",joueur_enemie);
                texte = TTF_RenderText_Blended(font,ch2,blanc);
                positionText.x =190;
                positionText.y =350;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                SDL_Flip(ec);
         }
   SDL_FreeSurface(menu);
}

void deroulerBataille (SDL_Surface *ec, int numJoueur, int numJoueurEnnemi,
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS],
int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE],int type
){
int i,tour_humaine=1,tour_enemi=1;
int gagant_humaine,gagant_enemi;
int indice_joueur_humaine=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,numJoueur,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
int indice_joueur_enemie=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,numJoueurEnnemi,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);

            do{
                    reinitialiserToursBataille(unitesBataille);
                    gagant_humaine=jouerTourHumainBataille(ec,numJoueur,numJoueurEnnemi,joueurs,herosJoueurs,plateauBataille,unitesBataille,tour_humaine,type);
                    tour_humaine++;
                    if(gagant_humaine==-1){
                    gagant_enemi=jouerTourEnnemiBataille(ec,numJoueurEnnemi,numJoueur,joueurs,herosJoueurs,plateauBataille,unitesBataille,tour_enemi,type);
                    tour_enemi++;
                    }
            }while(gagant_enemi==-1 && gagant_humaine==-1);

            if(gagant_humaine==numJoueur || gagant_enemi==numJoueur){
                victory(ec,indice_joueur_enemie,joueurs);
                for(i=OR;i<=GEMME_JAUNE;i++){
                    joueurs[indice_joueur_humaine][i]+=joueurs[indice_joueur_enemie][i];
                    joueurs[indice_joueur_enemie][i]=0;
                }
                joueurs[indice_joueur_humaine][NB_BATAILLE_GAGNES]+=1;
                joueurs[indice_joueur_enemie][NB_BATAILLE_PERDUES]+=1;
            }

            else if(gagant_enemi==numJoueurEnnemi || gagant_humaine==numJoueurEnnemi){
                 lose(ec,numJoueurEnnemi);
                /*for(i=OR;i<=GEMME_JAUNE;i++){
                    joueurs[indice_joueur_enemie][i]+=joueurs[indice_joueur_humaine][i];
                    joueurs[indice_joueur_humaine][i]=0;
                  }*/
                joueurs[indice_joueur_humaine][NB_BATAILLE_PERDUES]+=1;
                joueurs[indice_joueur_enemie][NB_BATAILLE_GAGNES]+=1;
            }
    //SDL_Flip(ec);
}

void choisir_bataille(SDL_Surface *ec, int numJoueur, int numJoueurEnnemi,
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS],
int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE]){
    SDL_Rect position;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Battaille",NULL);
    menu = IMG_Load("images/choisir_votre_plateau_bataille.png");
    int indiceOption=0;
    positionMenu.x = 0;
    int j;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",21);
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",16);
        font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",13);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color noir = {0, 0, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
        SDL_Rect positionText;

    while (continuer)
    {
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
            int i=0;
            int y_depart=110,y_depart_titre=90;
            int x_depart=10,x_depart_titre=80;
            int glace[12][12];
            int fire[12][12];
            int quitter[12][12];

                fire[0][0]=431;
                fire[0][1]=788;
                fire[0][2]=313;
                fire[0][3]=494;

                glace[1][0]=16;
                glace[1][1]=366;
                glace[1][2]=313;
                glace[1][3]=494;

        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
       case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            { int x_clicked=event.button.x;
              int y_clicked=event.button.y;

               int i=0;
                int trouve=0;
                  while(trouve==0 && i<4){
                    if((x_clicked>fire[i][0])&&(x_clicked<fire[i][1])){
                         if((y_clicked>fire[i][2])&&(y_clicked<fire[i][3])){
                                 int type=1;
                                 deroulerBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,joueurs,herosJoueurs,unitesJoueurs,plateauBataille,unitesBataille,type);
                                 trouve=1;
                                 continuer=0;
                            }
                    }
                        i++;
                  }

                i=0;
                 trouve=0;
                  while(trouve==0 && i<4){
                    if((x_clicked>glace[i][0])&&(x_clicked<glace[i][1])){
                         if((y_clicked>glace[i][2])&&(y_clicked<glace[i][3])){
                                 int type=0;
                                 deroulerBataille(ec,JOUEUR_HUMAIN,numJoueurEnnemi,joueurs,herosJoueurs,unitesJoueurs,plateauBataille,unitesBataille,type);
                                 trouve=1;
                                 continuer=0;
                            }
                    }
                        i++;
                  }

            }break;

         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            }
        }
                SDL_Flip(ec);
         }
   SDL_FreeSurface(menu);
}

void confirme_bataille(SDL_Surface *ec,int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS],
int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE],
int num_enemie){
    SDL_Surface  *unite4 = NULL, *unite5 = NULL, *unite8 = NULL, *unite9 = NULL,*unite10 = NULL, *unite12 = NULL, *unite14 = NULL, *unite18 = NULL,*unite19 = NULL,
        *unite22 = NULL, *unite23 = NULL,*unite15 = NULL,*joueur_humaine=NULL;
    SDL_Surface *enmie1=NULL,*enmie2=NULL, *enmie3=NULL, *enmie4 = NULL,*heros1=NULL, *heros2 = NULL, *heros3 = NULL, *heros4 = NULL,*heros5 = NULL,
      *unite1_enemie1 = NULL, *unite2_enemie1 = NULL, *unite1_enemie2 = NULL,*unite2_enemie2 = NULL,
       *unite1_enemie3 = NULL, *unite2_enemie3 = NULL, *unite1_enemie4 = NULL,*unite2_enemie4 = NULL;
     enmie1= IMG_Load("images/enemie1.png");
     enmie2= IMG_Load("images/enemie2.png");
     enmie3= IMG_Load("images/enemie3.png");
     enmie4= IMG_Load("images/enemie4.png");
     heros1= IMG_Load("images/unite8.png");
     heros2= IMG_Load("images/unite9.png");
     heros3= IMG_Load("images/unite14.png");
     heros4= IMG_Load("images/unite10.png");
     heros5= IMG_Load("images/unite12.png");
     unite1_enemie1= IMG_Load("images/unite_enemie1.png");
     unite2_enemie1= IMG_Load("images/unite1_enemie1.png");
     unite1_enemie2= IMG_Load("images/unite_enemie2.png");
     unite2_enemie2= IMG_Load("images/unite1_enemie2.png");
     unite1_enemie3= IMG_Load("images/unite_enemie3.png");
     unite2_enemie3= IMG_Load("images/unite1_enemie3.png");
     unite1_enemie4= IMG_Load("images/unite_enemie4.png");
     unite2_enemie4= IMG_Load("images/unite1_enemie4.png");
     unite4 = IMG_Load("images/unite__4.png");
     unite15 = IMG_Load("images/unite__15.png");
     unite5 = IMG_Load("images/unite__5.png");
     unite8 = IMG_Load("images/unite__8.png");
     unite9 = IMG_Load("images/unite__9.png");
     unite14 = IMG_Load("images/unite__14.png");
     unite10 = IMG_Load("images/unite__10.png");
     unite12= IMG_Load("images/unite__12.png");
     unite18 = IMG_Load("images/unite__18.png");
     unite19 = IMG_Load("images/unite__19.png");
     unite22= IMG_Load("images/unite__22.png");
     unite23 = IMG_Load("images/unite__23.png");
     joueur_humaine=IMG_Load("images/humaine.png");
     int liste_unite=12;

         char* phrase[5];
             phrase[0]= heros1;
             phrase[1]= heros2;
             phrase[2]= heros3;
             phrase[3]= heros4;
             phrase[4]= heros5;
        char* enemie[4];
             enemie[1]= enmie1;
             enemie[2]= enmie2;
             enemie[3]= enmie3;
             enemie[4]= enmie4;
    SDL_Rect position;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Battaille",NULL);
    menu = IMG_Load("images/battle.png");
    int indiceOption=0;
    positionMenu.x = 0;
    int j;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",21);
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",16);
        font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",13);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
        SDL_Rect positionText;

    while (continuer)
    {
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);

            char* tous_unites[liste_unite];
            tous_unites[0]= unite10;
            tous_unites[1]= unite12;
            tous_unites[2]= unite15;
            tous_unites[3]= unite14;
            tous_unites[4]= unite18;
            tous_unites[5]= unite19;
            tous_unites[6]= unite22;
            tous_unites[7]= unite23;
            tous_unites[8]= unite4;
            tous_unites[9]= unite5;
            tous_unites[10]= unite8;
            tous_unites[11]= unite9;
                int numero[liste_unite];
                numero[0]=10;
                numero[1]=12;
                numero[2]=15;
                numero[3]=14;
                numero[4]=18;
                numero[5]=19;
                numero[6]=22;
                numero[7]=23;
                numero[8]=4;
                numero[9]=5;
                numero[10]=8;
                numero[11]=9;
            int i=0;
            int y_depart=110,y_depart_titre=90;
            int x_depart=10,x_depart_titre=80;
            int fight[12][12];
            int cancel[12][12];

                cancel[0][0]=766;
                cancel[0][1]=792;
                cancel[0][2]=14;
                cancel[0][3]=57;

                fight[1][0]=280;
                fight[1][1]=523;
                fight[1][2]=479;
                fight[1][3]=538;

        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
       case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            { int x_clicked=event.button.x;
              int y_clicked=event.button.y;

                for(i=0;i<liste_unite;i++){
                    if((x_clicked>cancel[i][0])&&(x_clicked<cancel[i][1])){
                         if((y_clicked>cancel[i][2])&&(y_clicked<cancel[i][3])){
                                    continuer=0;

                            }
                                }
                }

                int i=0;
                int trouve=0;
                  while(trouve==0 && i<4){
                    if((x_clicked>fight[i][0])&&(x_clicked<fight[i][1])){
                         if((y_clicked>fight[i][2])&&(y_clicked<fight[i][3])){
                                 //deroulerBataille(ec,JOUEUR_HUMAIN,num_enemie,joueurs,herosJoueurs,unitesJoueurs,plateauBataille,unitesBataille);
                                 choisir_bataille(ec,JOUEUR_HUMAIN,num_enemie,joueurs,herosJoueurs,unitesJoueurs,plateauBataille,unitesBataille);
                                 trouve=1;
                                 continuer=0;
                            }
                    }
                        i++;
                  }

            }break;

         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            }
        }

        for(i=1;i<5;i++){
            if(i==num_enemie){
                position.x=495;
                position.y=180;
                SDL_BlitSurface(enemie[i], NULL, ec, &position);
                if(num_enemie==1){
                position.x=620;
                position.y=200;
                SDL_BlitSurface(unite1_enemie1, NULL, ec, &position);
                position.x=690;
                position.y=200;
                SDL_BlitSurface(unite2_enemie1, NULL, ec, &position);
                }else if(num_enemie==2){
                position.x=630;
                position.y=230;
                SDL_BlitSurface(unite1_enemie2, NULL, ec, &position);
                position.x=690;
                position.y=230;
                SDL_BlitSurface(unite2_enemie2, NULL, ec, &position);
                }else if(num_enemie==3){
                position.x=620;
                position.y=199;
                SDL_BlitSurface(unite1_enemie3, NULL, ec, &position);
                position.x=670;
                position.y=199;
                SDL_BlitSurface(unite2_enemie3, NULL, ec, &position);
                }else if(num_enemie==4){
                position.x=630;
                position.y=200;
                SDL_BlitSurface(unite1_enemie4, NULL, ec, &position);
                position.x=700;
                position.y=200;
                SDL_BlitSurface(unite2_enemie4, NULL, ec, &position);
                }
            }
        }
                position.x=230;
                position.y=180;
                SDL_BlitSurface(joueur_humaine, NULL, ec, &position);

          int nbr_unite=0;
            while(nbr_unite!=liste_unite)
            {
                int ligne_unite=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UJ,JOUEUR_HUMAIN,NUM_UNITE_UJ,numero[nbr_unite],NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
                 if(ligne_unite!=-1){
                 if(unitesJoueurs[ligne_unite][UNITE_ACTIVE]==1){
                    position.x=55*(nbr_unite%3)+35;
                    position.y=50*(nbr_unite/3)+180;
                    SDL_BlitSurface(tous_unites[nbr_unite], NULL, ec, &position);
                 }
              }
              nbr_unite++;
            }
                SDL_Flip(ec);
         }
   SDL_FreeSurface(menu);
   SDL_FreeSurface(joueur_humaine);
   SDL_FreeSurface(unite1_enemie3);
   SDL_FreeSurface(unite2_enemie3);
}

void besoin_unite_active(SDL_Surface *ec){
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Event event;
    positionMenu.y = 0;
    int continuer=1;
    SDL_WM_SetCaption("Annuler",NULL);
    menu = IMG_Load("images/unite_active.png");
    while (continuer==1)
    {   SDL_BlitSurface(menu, NULL, ec, &positionMenu);
       SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
        case SDLK_ESCAPE:
            continuer = 0;
            break;
        }
        }
    SDL_Flip(ec);
    }
SDL_FreeSurface(menu);
}

void aucune_fortune(SDL_Surface *ec,int num_enemie){
    SDL_Rect position;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL;
    TTF_Init();
    SDL_Color noir = {0, 0, 0};
    font = TTF_OpenFont("Aloevera-OVoWO.ttf",25);
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    positionMenu.y = 0;
    int continuer=1;
    SDL_WM_SetCaption("Annuler",NULL);
    menu = IMG_Load("images/fortune personnle.png");
    while (continuer==1)
    {   SDL_BlitSurface(menu, NULL, ec, &positionMenu);
       SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
        case SDLK_ESCAPE:
            continuer = 0;
            break;
        }
        }
                char *ch[50];
                sprintf(ch,"L'ennemie %d � �t� d�ja attaqu�",num_enemie);
                texte = TTF_RenderText_Blended(font,ch,noir);
                position.x =170;
                position.y =350;
                SDL_BlitSurface(texte, NULL, ec, &position);
    SDL_Flip(ec);
    }
SDL_FreeSurface(menu);
}

void verifier(SDL_Surface*ec,int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS],
int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE],
int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE],int x){
 int i;
        int indice_enemie=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,x,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
        int verifier_unite_active=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UJ,JOUEUR_HUMAIN,UNITE_ACTIVE,1,NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
        int bataille_perdu_enemie=joueurs[indice_enemie][NB_BATAILLE_PERDUES];

    if(verifier_unite_active!=-1 &&  bataille_perdu_enemie==0){
          confirme_bataille(ec,carte,joueurs,heros,unites,herosJoueurs,unitesJoueurs,plateauBataille,unitesBataille,x);
        }
    else if(verifier_unite_active==-1){
           besoin_unite_active(ec);
    }
    else  if(bataille_perdu_enemie>0){
            aucune_fortune(ec,x);
    }
}

void executerAction (
int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS])
{
     int ind_joueur,ligne,colonne,choix;
     ind_joueur=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,JOUEUR_HUMAIN,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
     ligne=joueurs[ind_joueur][LIGNE_J];
     colonne=joueurs[ind_joueur][COLONNE_J];
        int plateauBataille[NB_LIGNES_PLATEAUBATAILLE][NB_COLONNES_PLATEAUBATAILLE];
        int unitesBataille[NB_LIGNES_UNITESBATAILLE][NB_COLONNES_UNITESBATAILLE];
     SDL_Surface *ecran = NULL, *menu = NULL;
     SDL_Rect positionMenu;
     ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if(carte[ligne][colonne]%100/10==1){
            int num_enemie=carte[ligne][colonne]/100;
            initialiserBataille(JOUEUR_HUMAIN,num_enemie,herosJoueurs,unitesJoueurs,plateauBataille,unitesBataille);
            //deroulerBataille(ecran,JOUEUR_HUMAIN,num_enemie,joueurs,herosJoueurs,unitesJoueurs,plateauBataille,unitesBataille);
            verifier(ecran,carte,joueurs,heros,unites,herosJoueurs,unitesJoueurs,plateauBataille,unitesBataille,num_enemie);
    }
  else
     if(carte[ligne][colonne]%100/10==2){
     if(carte[ligne][colonne]/100!=0){
        confirmation_tresor(ecran,ind_joueur,ligne,colonne,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
     }
 }
  else
    if(carte[ligne][colonne]%100/10==3){
            int lig_hero=chercherIndiceAvecUneCondition(NUM_JOUEUR_HJ,0,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
            int lig_hero_principal=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,0,HERO_PRINCIPAL,1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
    if(carte[ligne][colonne]/100!=0){
        if(lig_hero==-1){
            warning_hero(ecran);
        }
        else if(lig_hero_principal==-1){
            warning_hero_principal(ecran);
         }
       else{
            confirmation_experience(ecran,ligne,colonne,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
         }
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
                position.y = i * 51;
                position.x = j * 82;
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
            positionMenu.y =340;
            menu = IMG_Load("images/Sans_titre-removebg-preview.png");
            SDL_BlitSurface(menu, NULL, ecran, &positionMenu);

                int y_depart=540;
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
                positionText.x =680;
                positionText.y =y_depart+16;
                SDL_BlitSurface(texte1, NULL, ecran, &positionText);

                texte2 = TTF_RenderText_Blended(font1,ch1,blanc);
                positionText.x =520;
                positionText.y =y_depart;
                SDL_BlitSurface(texte2, NULL, ecran, &positionText);

                texte3 = TTF_RenderText_Blended(font2,ch2,blanc);
                positionText.x =350;
                positionText.y =y_depart;
                SDL_BlitSurface(texte3, NULL, ecran, &positionText);

                texte4 = TTF_RenderText_Blended(font2,ch3,blanc);
                positionText.x =218;
                positionText.y =y_depart;
                SDL_BlitSurface(texte4, NULL, ecran, &positionText);

                texte5 = TTF_RenderText_Blended(font2,ch4,blanc);
                positionText.x =60;
                positionText.y =y_depart;
                SDL_BlitSurface(texte5, NULL, ecran, &positionText);

                texte6 = TTF_RenderText_Blended(font2,ch5,blanc);
                positionText.x =60;
                positionText.y =y_depart+40;
                SDL_BlitSurface(texte6, NULL, ecran, &positionText);

                texte7 = TTF_RenderText_Blended(font2,ch6,blanc);
                positionText.x =520;
                positionText.y =y_depart+40;
                SDL_BlitSurface(texte7, NULL, ecran, &positionText);

                texte8 = TTF_RenderText_Blended(font2,ch7,blanc);
                positionText.x =350;
                positionText.y =y_depart+40;
                SDL_BlitSurface(texte8, NULL, ecran, &positionText);

                texte9 = TTF_RenderText_Blended(font2,ch8,blanc);
                positionText.x =218;
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

void mettre_principal(SDL_Surface*ec,int x, int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
 int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
 int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS]){
     SDL_Surface *heros1 = NULL, *heros2 = NULL, *heros3 = NULL, *heros4 = NULL,*heros5 = NULL;
     heros1 = IMG_Load("images/confirme_principal_heros1.png");
     heros2 = IMG_Load("images/confirme_principal_heros2.png");
     heros3 = IMG_Load("images/confirme_principal_heros3.png");
     heros4 = IMG_Load("images/confirme_principal_heros4.png");
     heros5 = IMG_Load("images/confirme_principal_heros5.png");
        int liste_heros=5;
        int indiceOption=0;
    SDL_Event event;
    SDL_Surface *menu = NULL;
    SDL_Surface* texte = NULL;
    SDL_Rect positionMenu;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte2 = NULL;
    SDL_Rect positionText;
    SDL_Color blanc = { 255, 255, 255};
    SDL_Color red = {255, 0, 0};
    SDL_Color or_color = {240, 205, 115};
    SDL_Color green={4, 127, 0};
     SDL_Color or={255, 179, 0};
    TTF_Font* font = NULL;
    TTF_Init();
    font = TTF_OpenFont("Aloevera-OVoWO.ttf",25);
    font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",22);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Rect position;
    positionMenu.y = 0;
    SDL_WM_SetCaption("Heroes magic war",NULL);
    int continuer=1;
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
               int ligne_hero=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,JOUEUR_HUMAIN,NUM_HERO_HJ,x+1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
            int hero_principal=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,JOUEUR_HUMAIN,HERO_PRINCIPAL,1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
            if(hero_principal!=-1){
            herosJoueurs[hero_principal][HERO_PRINCIPAL]=0;
            herosJoueurs[ligne_hero][HERO_PRINCIPAL]=1;
            }else{
            herosJoueurs[ligne_hero][HERO_PRINCIPAL]=1;
            }
                continuer = 0;
                }
                else
                {
                continuer = 0;
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

        char* phrase[liste_heros];
        phrase[0]= heros1;
        phrase[1]= heros2;
        phrase[2]= heros3;
        phrase[3]= heros4;
        phrase[4]= heros5;
        for(int i=0;i<liste_heros;i++){
            if(i==x){
                  SDL_BlitSurface(phrase[i], NULL, ec, &positionMenu);
            }
        }

        if(font != 0)
        {
            char* phrase[1];
            char *ch[10];
            sprintf(ch,"Mettre L'h�ro %d principal",x+1);
            phrase[0]= "OUI";
            phrase[1]= "NON";
            int i, y_depart=489;
            SDL_Color couleur_a_utiliser;

                if(1==indiceOption)
                {
                texte = TTF_RenderText_Blended(font, phrase[1],green);
                positionText.x =175;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], blanc);
                positionText.x =550;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                }
                else
                {
                texte = TTF_RenderText_Blended(font, phrase[1], blanc);
                positionText.x =175;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], green);
                positionText.x =550;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                }


                texte2 = TTF_RenderText_Blended(font,ch,or_color);
                positionText.x =255;
                positionText.y =58;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);
        }
    SDL_Flip(ec);
    }
    SDL_FreeSurface(heros1);
    SDL_FreeSurface(heros2);
    SDL_FreeSurface(heros3);
    SDL_FreeSurface(heros4);
    SDL_FreeSurface(heros5);
}

void  lancer_config_heros(SDL_Surface*ec, int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
 int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
 int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS]){
     SDL_Rect position;
     SDL_Surface *heros1 = NULL, *heros2 = NULL, *heros3 = NULL, *heros4 = NULL,*heros5 = NULL;
     SDL_Surface *heros1_principal = NULL, *heros2_principal = NULL, *heros3_principal = NULL, *heros4_principal = NULL,*heros5_principal = NULL;
     heros1 = IMG_Load("images/nonheros1.png");
     heros2 = IMG_Load("images/nonheros2.png");
     heros3 = IMG_Load("images/nonheros3.png");
     heros4 = IMG_Load("images/nonheros4.png");
     heros5 = IMG_Load("images/nonheros5.png");
     heros1_principal = IMG_Load("images/principal_heros1.png");
     heros2_principal = IMG_Load("images/principal_heros2.png");
     heros3_principal = IMG_Load("images/principal_heros3.png");
     heros4_principal = IMG_Load("images/principal_heros4.png");
     heros5_principal = IMG_Load("images/principal_heros5.png");

    int liste_heros=5;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL,*font1=NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Heros",NULL);
    menu = IMG_Load("images/back_config.png");
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",30);
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
        SDL_Rect positionText;

    while (continuer)
    {
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
            char* phrase[liste_heros];
            phrase[0]= heros1;
            phrase[1]= heros2;
            phrase[2]= heros3;
            phrase[3]= heros4;
            phrase[4]= heros5;
            char* heros_principal[liste_heros];
            heros_principal[0]= heros1_principal;
            heros_principal[1]= heros2_principal;
            heros_principal[2]= heros3_principal;
            heros_principal[3]= heros4_principal;
            heros_principal[4]= heros5_principal;
            int i=0;
            int y_depart=110,y_depart_titre=90;
            int x_depart=2,x_depart_titre=80;
            int tab_info[5][5];

                tab_info[0][0]=24;
                tab_info[0][1]=180;
                tab_info[0][2]=286;
                tab_info[0][3]=325;

                tab_info[1][0]= 323;
                tab_info[1][1]=477;
                tab_info[1][2]=286;
                tab_info[1][3]=325;

                tab_info[2][0]= 622;
                tab_info[2][1]= 781;
                tab_info[2][2]=286;
                tab_info[2][3]=325;

                tab_info[3][0]= 24;
                tab_info[3][1]= 180;
                tab_info[3][2]=546;
                tab_info[3][3]=587;

                tab_info[4][0]= 323;
                tab_info[4][1]=477;
                tab_info[4][2]=546;
                tab_info[4][3]=587;

        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            { int x_clicked=event.button.x;
              int y_clicked=event.button.y;
                for(i=0;i<5;i++){
                    if((x_clicked>tab_info[i][0])&&(x_clicked<tab_info[i][1])){
                         if((y_clicked>tab_info[i][2])&&(y_clicked<tab_info[i][3])){
                                 mettre_principal(ec,i,joueurs,heros,herosJoueurs);
                            }
                                }
                }

                char *ch[50];
                sprintf(ch,"%d",x_clicked);
                texte = TTF_RenderText_Blended(font,ch,pink);
                positionText.x =10;
                positionText.y =48;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                char *ch1[50];
                sprintf(ch1,"%d",y_clicked);
                texte = TTF_RenderText_Blended(font,ch1,pink);
                positionText.x =80;
                positionText.y =50;
                SDL_BlitSurface(texte, NULL, ec, &positionText);

            }break;

         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            }
        }
                int nbr=0;
                for(i=0;i<liste_heros;i++){
                      int lign_herojoueur=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,JOUEUR_HUMAIN,NUM_HERO_HJ,i+1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
                      if(lign_herojoueur!=-1){
                        nbr++;
                      }
                }
                if(nbr==0){
                char ch[100]="Acune H�ro";
                texte_titre = TTF_RenderText_Blended(font1,ch,blanc);
                positionText.x =350;
                positionText.y =250;
                SDL_BlitSurface(texte_titre, NULL, ec, &positionText);
                }else{
                int nbr_heros=0;
                while(nbr_heros!=liste_heros)
            {
                int ligne_hero=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,JOUEUR_HUMAIN,NUM_HERO_HJ,nbr_heros+1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
                int lign_herojoueur=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,JOUEUR_HUMAIN,NUM_HERO_HJ,nbr_heros+1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
                 if(nbr_heros>2){
                 position.y =y_depart+280;
                 position.x =((x_depart+nbr_heros)-5)*300;
                }else{
                  position.y =(y_depart-20)+40;
                  position.x =x_depart+nbr_heros*300;
                }
                if(lign_herojoueur!=-1){
                        if(herosJoueurs[ligne_hero][HERO_PRINCIPAL]==1){
                            SDL_BlitSurface(heros_principal[nbr_heros], NULL, ec, &position);
                        }else{
                            SDL_BlitSurface(phrase[nbr_heros], NULL, ec, &position);
                        }
                }
                 nbr_heros++;
            }
                }

                char ch[100]="Configuration H�ros";
                texte_titre = TTF_RenderText_Blended(font,ch,blanc);
                positionText.x =250;
                positionText.y =25;
                SDL_BlitSurface(texte_titre, NULL, ec, &positionText);
                SDL_Flip(ec);
         }

 SDL_FreeSurface(menu);

}


void  config_hero(SDL_Surface*ec, int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
 int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
 int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS]){
    int nombre_phrase=2;
    SDL_Surface *menu = NULL,*select=NULL;
    SDL_Rect positionMenu,position;
    SDL_Event event;
    TTF_Font* font = NULL,*font1=NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Heros",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
         font = TTF_OpenFont("Aloevera-OVoWO.ttf",24);
         font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",27);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255 ,0 ,127};
        SDL_Color pink = {255 ,0 ,0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Rect positionText;
        menu = IMG_Load("images/menu1.png");
        select=IMG_Load("images/select.png");
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
                    lancer_config_heros(ec,joueurs,heros,herosJoueurs);
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
            char ch[50]="configuration H�ros";
            char* phrase[nombre_phrase];
            phrase[0]= "  Mettre un nouveau h�ro comme principal   ";
            phrase[1]= "  Revenir au menu pr�c�dent                ";
            int i, y_depart=230, y_depart_titre=80;
            SDL_Color couleur_a_utiliser;

            for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blanc;
                    position.x =220;
                    position.y=y_depart-10+i*40;
                    SDL_BlitSurface(select, NULL, ec, &position);
                }
                else
                {
                    couleur_a_utiliser=red;
                }
                texte = TTF_RenderText_Blended(font1, phrase[i], couleur_a_utiliser) ;
                positionText.x =230;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
            }
                texte_titre = TTF_RenderText_Blended(font,ch,yellow);
                positionText.x =303;
                positionText.y =y_depart_titre;
                SDL_BlitSurface(texte_titre, NULL, ec, &positionText);
        }
        SDL_Flip(ec);
    }
    SDL_FreeSurface(texte);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}


void confirme_unite_active(SDL_Surface *ec,int x,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
 int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
 int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){
    SDL_Surface *Confirme=NULL, *unite4 = NULL, *unite5 = NULL, *unite8 = NULL, *unite9 = NULL,*unite10 = NULL, *unite12 = NULL, *unite14 = NULL, *unite18 = NULL,*unite19 = NULL,
     *unite22 = NULL, *unite23 = NULL,*unite15 = NULL;
     unite4 = IMG_Load("images/unite_4.png");
     unite15 = IMG_Load("images/unite_15.png");
     unite5 = IMG_Load("images/unite_5.png");
     unite8 = IMG_Load("images/unite_8.png");
     unite9 = IMG_Load("images/unite_9.png");
     unite14 = IMG_Load("images/unite_14.png");
     unite10 = IMG_Load("images/unite_10.png");
     unite12= IMG_Load("images/unite_12.png");
     unite18 = IMG_Load("images/unite_18.png");
     unite19 = IMG_Load("images/unite_19.png");
     unite22= IMG_Load("images/unite_22.png");
     unite23 = IMG_Load("images/unite_23.png");
     Confirme = IMG_Load("images/confirme_active_unite.png");
      int liste_unite=12;
           char* name[liste_unite];
            name[0]="Bubo Bubo";
            name[1]="Hyena Rider";
            name[2]="Spirit";
            name[3]="Mindslaver";
            name[4]="Cerberus";
            name[5]="Swordsman";
            name[6]="Frost mage";
            name[7]="Sister";
            name[8]="Skeleton";
            name[9]="Goblin thief";
            name[10]="Zombie";
            name[11]="Fire Elemental";

      int numero[liste_unite];
                numero[0]=10;
                numero[1]=12;
                numero[2]=15;
                numero[3]=14;
                numero[4]=18;
                numero[5]=19;
                numero[6]=22;
                numero[7]=23;
                numero[8]=4;
                numero[9]=5;
                numero[10]=8;
                numero[11]=9;

        int indiceOption=0;
    SDL_Event event;
    SDL_Surface *menu = NULL;
    SDL_Surface* texte = NULL;
    SDL_Rect positionMenu;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte2 = NULL;
    SDL_Rect positionText;
    SDL_Color blanc = { 255, 255, 255};
    SDL_Color red = {255, 0, 0};
    SDL_Color or_color = {240, 205, 115};
    SDL_Color green={4, 127, 0};
     SDL_Color or={255, 179, 0};
    TTF_Font* font = NULL;
    TTF_Init();
    font = TTF_OpenFont("Aloevera-OVoWO.ttf",25);
    font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",22);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Rect position;
    positionMenu.y = 0;
    SDL_WM_SetCaption("Heroes magic war",NULL);
    int continuer=1;
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
                 int ligne_unite=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UJ,JOUEUR_HUMAIN,NUM_UNITE_UJ,numero[x],NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
                 unitesJoueurs[ligne_unite][UNITE_ACTIVE]=1;
                 continuer = 0;
                }
                else
                {
                continuer = 0;
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

      char* phrase[liste_unite];
            phrase[0]= unite10;
            phrase[1]= unite12;
            phrase[2]= unite15;
            phrase[3]= unite14;
            phrase[4]= unite18;
            phrase[5]= unite19;
            phrase[6]= unite22;
            phrase[7]= unite23;
            phrase[8]= unite4;
            phrase[9]= unite5;
            phrase[10]= unite8;
            phrase[11]= unite9;
        for(int i=0;i<liste_unite;i++){
            if(i==x){
                  SDL_BlitSurface(Confirme, NULL, ec, &positionMenu);
                  position.x=350;
                  position.y=170;
                  SDL_BlitSurface(phrase[i], NULL, ec, &position);
            }
        }

        if(font != 0)
        {
            char* phrase[1];
            char ch1[50]="Activation de l'unit�";
            phrase[0]= "OUI";
            phrase[1]= "NON";
            int i, y_depart=485;
            SDL_Color couleur_a_utiliser;

                if(1==indiceOption)
                {
                texte = TTF_RenderText_Blended(font, phrase[1],green);
                positionText.x =180;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], blanc);
                positionText.x =580;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                }
                else
                {
                texte = TTF_RenderText_Blended(font, phrase[1], blanc);
                positionText.x =180;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], green);
                positionText.x =580;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                }

                texte2 = TTF_RenderText_Blended(font,ch1,or_color);
                positionText.x =310;
                positionText.y =56;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);

                texte2 = TTF_RenderText_Blended(font1,name[x],blanc);
                positionText.x =350;
                positionText.y =123;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);

        }
    SDL_Flip(ec);
    }
     SDL_FreeSurface(menu);
     SDL_FreeSurface(Confirme);
 }

void confirme_unite_reserve(SDL_Surface *ec,int x,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
 int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
 int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){
 SDL_Surface *Confirme=NULL, *unite4 = NULL, *unite5 = NULL, *unite8 = NULL, *unite9 = NULL,*unite10 = NULL, *unite12 = NULL, *unite14 = NULL, *unite18 = NULL,*unite19 = NULL,
     *unite22 = NULL, *unite23 = NULL,*unite15 = NULL;
     unite4 = IMG_Load("images/unite_4.png");
     unite15 = IMG_Load("images/unite_15.png");
     unite5 = IMG_Load("images/unite_5.png");
     unite8 = IMG_Load("images/unite_8.png");
     unite9 = IMG_Load("images/unite_9.png");
     unite14 = IMG_Load("images/unite_14.png");
     unite10 = IMG_Load("images/unite_10.png");
     unite12= IMG_Load("images/unite_12.png");
     unite18 = IMG_Load("images/unite_18.png");
     unite19 = IMG_Load("images/unite_19.png");
     unite22= IMG_Load("images/unite_22.png");
     unite23 = IMG_Load("images/unite_23.png");
     Confirme = IMG_Load("images/confirme_active_unite.png");
      int liste_unite=12;
          char* name[liste_unite];
            name[0]="Bubo Bubo";
            name[1]="Hyena Rider";
            name[2]="Spirit";
            name[3]="Mindslaver";
            name[4]="Cerberus";
            name[5]="Swordsman";
            name[6]="Frost mage";
            name[7]="Sister";
            name[8]="Skeleton";
            name[9]="Goblin thief";
            name[10]="Zombie";
            name[11]="Fire Elemental";
      int numero[liste_unite];
                numero[0]=10;
                numero[1]=12;
                numero[2]=15;
                numero[3]=14;
                numero[4]=18;
                numero[5]=19;
                numero[6]=22;
                numero[7]=23;
                numero[8]=4;
                numero[9]=5;
                numero[10]=8;
                numero[11]=9;

        int indiceOption=0;
    SDL_Event event;
    SDL_Surface *menu = NULL;
    SDL_Surface* texte = NULL;
    SDL_Rect positionMenu;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte2 = NULL;
    SDL_Rect positionText;
    SDL_Color blanc = { 255, 255, 255};
    SDL_Color red = {255, 0, 0};
    SDL_Color or_color = {240, 205, 115};
    SDL_Color green={4, 127, 0};
     SDL_Color or={255, 179, 0};
    TTF_Font* font = NULL;
    TTF_Init();
    font = TTF_OpenFont("Aloevera-OVoWO.ttf",25);
    font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",22);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Rect position;
    positionMenu.y = 0;
    SDL_WM_SetCaption("Heroes magic war",NULL);
    int continuer=1;
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
                 int ligne_reserve=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UJ,JOUEUR_HUMAIN,NUM_UNITE_UJ,numero[x],NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
                 unitesJoueurs[ligne_reserve][UNITE_ACTIVE]=0;
                 continuer = 0;
                }
                else
                {
                continuer = 0;
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

      char* phrase[liste_unite];
            phrase[0]= unite10;
            phrase[1]= unite12;
            phrase[2]= unite15;
            phrase[3]= unite14;
            phrase[4]= unite18;
            phrase[5]= unite19;
            phrase[6]= unite22;
            phrase[7]= unite23;
            phrase[8]= unite4;
            phrase[9]= unite5;
            phrase[10]= unite8;
            phrase[11]= unite9;
        for(int i=0;i<liste_unite;i++){
            if(i==x){
                  SDL_BlitSurface(Confirme, NULL, ec, &positionMenu);
                  position.x=350;
                  position.y=170;
                  SDL_BlitSurface(phrase[i], NULL, ec, &position);
            }
        }

         if(font != 0)
        {
            char* phrase[1];
            char ch1[50]="Mettre l'unit� en reserve ";
            phrase[0]= "OUI";
            phrase[1]= "NON";
            int i, y_depart=485;
            SDL_Color couleur_a_utiliser;

                if(1==indiceOption)
                {
                texte = TTF_RenderText_Blended(font, phrase[1],green);
                positionText.x =180;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], blanc);
                positionText.x =580;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                }
                else
                {
                texte = TTF_RenderText_Blended(font, phrase[1], blanc);
                positionText.x =180;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                texte = TTF_RenderText_Blended(font, phrase[0], green);
                positionText.x =580;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                }

                texte2 = TTF_RenderText_Blended(font,ch1,or_color);
                positionText.x =310;
                positionText.y =56;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);

                texte2 = TTF_RenderText_Blended(font1,name[x],blanc);
                positionText.x =350;
                positionText.y =123;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);

        }
    SDL_Flip(ec);
    }
     SDL_FreeSurface(menu);
     SDL_FreeSurface(Confirme);


 }

 void warning_emplacement(SDL_Surface *ec,int x){
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Rect position;
    SDL_Event event;
    SDL_Surface* texte = NULL;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte2 = NULL;
    SDL_Rect positionText;
    font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",22);
    SDL_Color blanc = { 255, 255, 255};
    SDL_Color red = {255, 0, 0};
    SDL_Color or_color = {240, 205, 115};
    SDL_Color green={4, 127, 0};
    SDL_Color or={255, 179, 0};
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    positionMenu.y = 0;
    int continuer=1;
    SDL_WM_SetCaption("Annuler",NULL);
    menu = IMG_Load("images/warning_emplacement.png");
    while (continuer==1)
    {   SDL_BlitSurface(menu, NULL, ec, &positionMenu);
       SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
        case SDLK_ESCAPE:
            continuer = 0;
            break;
            }
        }

          char *ch[50];
          sprintf(ch,"Activation de l'unit� %d annul�e",x);
          char ch1[50]="Tous les emplacements sont occup�s !";
          texte2 = TTF_RenderText_Blended(font1,ch,blanc);
          position.x =150;
          position.y =250;
          SDL_BlitSurface(texte2, NULL, ec, &position);

          texte2 = TTF_RenderText_Blended(font1,ch1,blanc);
          position.x =120;
          position.y =320;
          SDL_BlitSurface(texte2, NULL, ec, &position);

    SDL_Flip(ec);
    }
SDL_FreeSurface(menu);


 }

void verifie_unite(SDL_Surface *ec,int x,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
 int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
 int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){
     int liste_unite=12;
     int i;
        int numero[liste_unite];
            numero[0]=10;
            numero[1]=12;
            numero[2]=15;
            numero[3]=14;
            numero[4]=18;
            numero[5]=19;
            numero[6]=22;
            numero[7]=23;
            numero[8]=4;
            numero[9]=5;
            numero[10]=8;
            numero[11]=9;
    int active=0;
    for(i=0;i<NB_LIGNES_UNITESJOUEURS;i++){
        if(unitesJoueurs[i][NUM_JOUEUR_UJ]==0 && unitesJoueurs[i][UNITE_ACTIVE]==1){
            active+=unitesJoueurs[i][UNITE_ACTIVE];
        }
    }
     int ligne_unite=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UJ,JOUEUR_HUMAIN,NUM_UNITE_UJ,numero[x],NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
     if(unitesJoueurs[ligne_unite][UNITE_ACTIVE]==1)
     {
      confirme_unite_reserve(ec,x,joueurs,unites,unitesJoueurs);
     }else
     if(active==nbr_emplacement){
            warning_emplacement(ec,x);
     }else
        if(unitesJoueurs[ligne_unite][UNITE_ACTIVE]==0){
            confirme_unite_active(ec,x,joueurs,unites,unitesJoueurs);
          }
 }

void lancer_config_unite(SDL_Surface *ec,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
 int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
 int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){
    SDL_Surface *active=NULL,*reserve=NULL, *info_cadre=NULL, *unite4 = NULL, *unite5 = NULL, *unite8 = NULL, *unite9 = NULL,*unite10 = NULL, *unite12 = NULL, *unite14 = NULL, *unite18 = NULL,*unite19 = NULL,
        *unite22 = NULL, *unite23 = NULL,*unite15 = NULL,*cadre_nbr_sodlats=NULL;

    SDL_Surface *unite4_reserve=NULL, *unite15_reserve = NULL, *unite5_reserve = NULL, *unite8_reserve = NULL,
        *unite9_reserve = NULL, *unite14_reserve = NULL, *unite10_reserve = NULL, *unite12_reserve = NULL,*unite18_reserve = NULL,
        *unite19_reserve = NULL, *unite22_reserve = NULL,*unite23_reserve = NULL;

     unite4 = IMG_Load("images/unite_4.png");
     unite15 = IMG_Load("images/unite_15.png");
     unite5 = IMG_Load("images/unite_5.png");
     unite8 = IMG_Load("images/unite_8.png");
     unite9 = IMG_Load("images/unite_9.png");
     unite14 = IMG_Load("images/unite_14.png");
     unite10 = IMG_Load("images/unite_10.png");
     unite12= IMG_Load("images/unite_12.png");
     unite18 = IMG_Load("images/unite_18.png");
     unite19 = IMG_Load("images/unite_19.png");
     unite22= IMG_Load("images/unite_22.png");
     unite23 = IMG_Load("images/unite_23.png");

     unite4_reserve = IMG_Load("images/unite_4_reserve.png");
     unite15_reserve = IMG_Load("images/unite_15_reserve.png");
     unite5_reserve = IMG_Load("images/unite_5_reserve.png");
     unite8_reserve = IMG_Load("images/unite_8_reserve.png");
     unite9_reserve = IMG_Load("images/unite_9_reserve.png");
     unite14_reserve = IMG_Load("images/unite_14_reserve.png");
     unite10_reserve = IMG_Load("images/unite_10_reserve.png");
     unite12_reserve= IMG_Load("images/unite_12_reserve.png");
     unite18_reserve = IMG_Load("images/unite_18_reserve.png");
     unite19_reserve = IMG_Load("images/unite_19_reserve.png");
     unite22_reserve= IMG_Load("images/unite_22_reserve.png");
     unite23_reserve = IMG_Load("images/unite_23_reserve.png");

    active=IMG_Load("images/active.png");
    reserve=IMG_Load("images/reserve.png");
    cadre_nbr_sodlats=IMG_Load("images/soldtas.png");

    int liste_unite=12;
    SDL_Rect position;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL,*font1=NULL,*font2=NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Heros",NULL);
    menu = IMG_Load("images/back_config.png");
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
         font = TTF_OpenFont("Aloevera-OVoWO.ttf",21);
         font1= TTF_OpenFont("Aloevera-OVoWO.ttf",14);
         font2= TTF_OpenFont("Aloevera-OVoWO.ttf",26);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color noir ={0, 0, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
        SDL_Rect positionText;


        while (continuer)
    {
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
            char* phrase[liste_unite];
            phrase[0]= unite10;
            phrase[1]= unite12;
            phrase[2]= unite15;
            phrase[3]= unite14;
            phrase[4]= unite18;
            phrase[5]= unite19;
            phrase[6]= unite22;
            phrase[7]= unite23;
            phrase[8]= unite4;
            phrase[9]= unite5;
            phrase[10]= unite8;
            phrase[11]= unite9;
            char* unite_reserve[liste_unite];
            unite_reserve[0]= unite10_reserve;
            unite_reserve[1]= unite12_reserve;
            unite_reserve[2]= unite15_reserve;
            unite_reserve[3]= unite14_reserve;
            unite_reserve[4]= unite18_reserve;
            unite_reserve[5]= unite19_reserve;
            unite_reserve[6]= unite22_reserve;
            unite_reserve[7]= unite23_reserve;
            unite_reserve[8]= unite4_reserve;
            unite_reserve[9]= unite5_reserve;
            unite_reserve[10]= unite8_reserve;
            unite_reserve[11]= unite9_reserve;
             char* active_cadre[liste_unite];
            active_cadre[0]=active;
            active_cadre[1]=active;
            active_cadre[2]=active;
            active_cadre[3]=active;
            active_cadre[4]=active;
            active_cadre[5]=active;
            active_cadre[6]=active;
            active_cadre[7]=active;
            active_cadre[8]=active;
            active_cadre[9]=active;
            active_cadre[10]=active;
            active_cadre[11]=active;
            char* reserve_cadre[liste_unite];
            reserve_cadre[0]=reserve;
            reserve_cadre[1]=reserve;
            reserve_cadre[2]=reserve;
            reserve_cadre[3]=reserve;
            reserve_cadre[4]=reserve;
            reserve_cadre[5]=reserve;
            reserve_cadre[6]=reserve;
            reserve_cadre[7]=reserve;
            reserve_cadre[8]=reserve;
            reserve_cadre[9]=reserve;
            reserve_cadre[10]=reserve;
            reserve_cadre[11]=reserve;
        char* cadre_soldats[liste_unite];
            cadre_soldats[0]= cadre_nbr_sodlats;
            cadre_soldats[1]= cadre_nbr_sodlats;
            cadre_soldats[2]= cadre_nbr_sodlats;
            cadre_soldats[3]= cadre_nbr_sodlats;
            cadre_soldats[4]= cadre_nbr_sodlats;
            cadre_soldats[5]= cadre_nbr_sodlats;
            cadre_soldats[6]= cadre_nbr_sodlats;
            cadre_soldats[7]= cadre_nbr_sodlats;
            cadre_soldats[8]= cadre_nbr_sodlats;
            cadre_soldats[9]= cadre_nbr_sodlats;
            cadre_soldats[10]= cadre_nbr_sodlats;
            cadre_soldats[11]= cadre_nbr_sodlats;
             int numero[liste_unite];
                numero[0]=10;
                numero[1]=12;
                numero[2]=15;
                numero[3]=14;
                numero[4]=18;
                numero[5]=19;
                numero[6]=22;
                numero[7]=23;
                numero[8]=4;
                numero[9]=5;
                numero[10]=8;
                numero[11]=9;
                int i=0;
                int nbr_sodalts[liste_unite];
                for( i=0;i<liste_unite;i++){
                    int chercher=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UJ,JOUEUR_HUMAIN,NUM_UNITE_UJ,numero[i],NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
                    nbr_sodalts[i]=unitesJoueurs[chercher][NOMBRE_UJ];
                }

            int y_depart=110,y_depart_titre=90;
            int x_depart=2,x_depart_titre=80;
            int clique[12][12];

                clique[0][0]=15;
                clique[0][1]=113;
                clique[0][2]=212;
                clique[0][3]=236;

                clique[1][0]= 231;
                clique[1][1]=328;
                clique[1][2]=212;
                clique[1][3]=236;

                clique[2][0]= 442;
                clique[2][1]= 543;
                clique[2][2]=212;
                clique[2][3]=236;

                clique[3][0]= 659;
                clique[3][1]= 759;
                clique[3][2]=212;
                clique[3][3]=236;

                clique[4][0]= 15;
                clique[4][1]=113;
                clique[4][2]=378;
                clique[4][3]=412;


                clique[5][0]=231 ;
                clique[5][1]= 328;
                clique[5][2]=378;
                clique[5][3]=412;

                clique[6][0]= 442;
                clique[6][1]=543;
                clique[6][2]=378;
                clique[6][3]=412;

                clique[7][0]= 659;
                clique[7][1]= 759;
                clique[7][2]=378;
                clique[7][3]=412;

                clique[8][0]=15;
                clique[8][1]=113;
                clique[8][2]=565;
                clique[8][3]=587;

                clique[9][0]=231;
                clique[9][1]=328;
                clique[9][2]=565;
                clique[9][3]=587;

                clique[10][0]=442;
                clique[10][1]=543;
                clique[10][2]=565;
                clique[10][3]=587;

                clique[11][0]=659;
                clique[11][1]=759;
                clique[11][2]=565;
                clique[11][3]=587;

        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            { int x_clicked=event.button.x;
              int y_clicked=event.button.y;
                for(i=0;i<liste_unite;i++){
                    if((x_clicked>clique[i][0])&&(x_clicked<clique[i][1])){
                         if((y_clicked>clique[i][2])&&(y_clicked<clique[i][3])){
                                 verifie_unite(ec,i,joueurs,unites,unitesJoueurs);
                            }
                                }
                }



            }break;

         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            }
        }

            int nbr=0;
            for(i=0;i<liste_unite;i++){
                int verife=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UJ,JOUEUR_HUMAIN,NUM_UNITE_UJ,numero[i],NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
                    if(verife!=-1){
                            nbr++;
                    }
            }
            if(nbr==0){
                    char ch[50]="Acune Unite";
                    texte2 = TTF_RenderText_Blended(font2,ch,blanc);
                    position.x=250;
                    position.y=250;
                    SDL_BlitSurface(texte2, NULL, ec, &position);
            }else{
          int nbr_unite=0;
            while(nbr_unite!=liste_unite)
            {
                int ligne_unite=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UJ,JOUEUR_HUMAIN,NUM_UNITE_UJ,numero[nbr_unite],NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
                 if(ligne_unite!=-1){
                 if(unitesJoueurs[ligne_unite][UNITE_ACTIVE]==0){
                    position.x=215*(nbr_unite%4)+13;
                    position.y=176*(nbr_unite/4)+85;
                    SDL_BlitSurface(unite_reserve[nbr_unite], NULL, ec, &position);
                    position.x=215*(nbr_unite%4);
                    position.y=176*(nbr_unite/4)+210;
                    SDL_BlitSurface(active_cadre[nbr_unite], NULL, ec, &position);
                    position.x=215*(nbr_unite%4)+30;
                    position.y=176*(nbr_unite/4)+160;
                    SDL_BlitSurface(cadre_soldats[nbr_unite], NULL, ec, &position);
                    char ch[50]="Activer";
                    texte2 = TTF_RenderText_Blended(font1,ch,blanc);
                    position.x=215*(nbr_unite%4)+45;
                    position.y=176*(nbr_unite/4)+215;
                    SDL_BlitSurface(texte2, NULL, ec, &position);
                 }else{

                    position.x=215*(nbr_unite%4)+13;
                    position.y=176*(nbr_unite/4)+85;
                    SDL_BlitSurface(phrase[nbr_unite], NULL, ec, &position);
                    position.x=215*(nbr_unite%4);
                    position.y=176*(nbr_unite/4)+210;
                    SDL_BlitSurface(reserve_cadre[nbr_unite], NULL, ec, &position);
                    position.x=215*(nbr_unite%4)+30;
                    position.y=176*(nbr_unite/4)+160;
                    SDL_BlitSurface(cadre_soldats[nbr_unite], NULL, ec, &position);
                    char ch[50]="D�sactiver";
                    texte2 = TTF_RenderText_Blended(font1,ch,blanc);
                    position.x=215*(nbr_unite%4)+33;
                    position.y=176*(nbr_unite/4)+223;
                    SDL_BlitSurface(texte2, NULL, ec, &position);
                 }
                char *ch2[50];
                sprintf(ch2,"%d",nbr_sodalts[nbr_unite]);
                texte2 = TTF_RenderText_Blended(font,ch2,blanc);
                position.x=215*(nbr_unite%4)+53;
                position.y=176*(nbr_unite/4)+176;
                SDL_BlitSurface(texte2, NULL, ec, &position);
                }
                nbr_unite++;
             }
        }

                char ch[100]="Configuration Unites";
                texte_titre = TTF_RenderText_Blended(font,ch,blanc);
                positionText.x =258;
                positionText.y =25;
                SDL_BlitSurface(texte_titre, NULL, ec, &positionText);
                SDL_Flip(ec);
         }

 SDL_FreeSurface(menu);
}

void  campement(SDL_Surface *ec,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){
int nombre_phrase=3;
    SDL_Surface *menu = NULL,*select=NULL;
    SDL_Rect positionMenu,position;
    SDL_Event event;
    TTF_Font* font = NULL,*font1=NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Configuer",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
         font = TTF_OpenFont("Aloevera-OVoWO.ttf",27);
         font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",32);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255 ,0 ,127};
           SDL_Color blanc = {255 ,255 ,255};
        SDL_Color pink = {255 ,0 ,0};
        SDL_Color blue = {0, 109, 255};
        SDL_Rect positionText;
        menu = IMG_Load("images/menu1.png");
        select = IMG_Load("images/select.png");

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
                    config_hero(ec,joueurs,heros,herosJoueurs);
                }
                else if(indiceOption==1)
                {
                   lancer_config_unite(ec,joueurs,unites,unitesJoueurs);
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
            char ch[50]="Configuer";
            char* phrase[nombre_phrase];
            phrase[0]= "  configuration h�ros         ";
            phrase[1]= "  configuration unit�s        ";
            phrase[2]= "  Revenir au menu pr�c�dent   ";
            int i, y_depart=230, y_depart_titre=80;
            SDL_Color couleur_a_utiliser;
            for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blanc;
                    position.x =270;
                    position.y=y_depart-10+i*40;
                    SDL_BlitSurface(select, NULL, ec, &position);
                }
                else
                {
                    couleur_a_utiliser=red;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                 positionText.x =280;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
            }
                texte_titre = TTF_RenderText_Blended(font1,ch,yellow);
                positionText.x =360;
                positionText.y =y_depart_titre;
                SDL_BlitSurface(texte_titre, NULL, ec, &positionText);
        }
        SDL_Flip(ec);
    }
    SDL_FreeSurface(texte);

    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}

void deja_existe_unite(SDL_Surface *ec,int x){
    SDL_Surface  *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte = NULL;
    SDL_Surface* texte2 = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Ajouter",NULL);
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
        menu = IMG_Load("images/deja_exite.png");
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

            }
            break;
        }
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);

        if(font != 0)
        {
            char *ch[10];
            char ch1[50]=" Achat annule ,car hero ";
            char ch2[50]="   est deja acheter !";
            sprintf(ch,"%d",x+1);
            int i, y_depart=439;
            SDL_Color couleur_a_utiliser;

                texte2 = TTF_RenderText_Blended(font,ch,blanc);
                positionText.x =400;
                positionText.y =260;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);

                texte2 = TTF_RenderText_Blended(font,ch1,noir);
                positionText.x =100;
                positionText.y =260;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);

                texte2 = TTF_RenderText_Blended(font,ch2,noir);
                positionText.x =150;
                positionText.y =310;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);
        }
    SDL_Flip(ec);
    }
    SDL_FreeSurface(texte);
    SDL_FreeSurface(texte2);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}

void warning_achats_unite(SDL_Surface *ec){
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Event event;
    positionMenu.y = 0;
    int continuer=1;
    SDL_WM_SetCaption("Annuler",NULL);
    menu = IMG_Load("images/warning_unite.png");
    while (continuer==1)
    {   SDL_BlitSurface(menu, NULL, ec, &positionMenu);
       SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
        case SDLK_ESCAPE:
            continuer = 0;
            break;
        }
        }
    SDL_Flip(ec);
    }
SDL_FreeSurface(menu);
}

void confirmation_achats_unite(SDL_Surface *ec,int soldat,int x,int or,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS],int prix_unite10,int prix_unite12,
int prix_unite15,int prix_unite14,int prix_unite18,int prix_unite19,int prix_unite22,int prix_unite23,
int prix_unite4,int prix_unite5,int prix_unite8,int prix_unite9){

 SDL_Surface  *unite4 = NULL, *unite5 = NULL, *unite8 = NULL, *unite9 = NULL,*unite10 = NULL, *unite12 = NULL, *unite14 = NULL, *unite18 = NULL,*unite19 = NULL,
     *unite22 = NULL, *unite23 = NULL,*unite15 = NULL;
     unite4 = IMG_Load("images/achats_unite_4.png");
     unite15 = IMG_Load("images/achats_unite_15.png");
     unite5 = IMG_Load("images/achats_unite_5.png");
     unite8 = IMG_Load("images/achats_unite_8.png");
     unite9 = IMG_Load("images/achats_unite_9.png");
     unite14 = IMG_Load("images/achats_unite_14.png");
     unite10 = IMG_Load("images/achats_unite_10.png");
     unite12= IMG_Load("images/achats_unite_12.png");
     unite18 = IMG_Load("images/achats_unite_18.png");
     unite19 = IMG_Load("images/achats_unite_19.png");
     unite22= IMG_Load("images/achats_unite_22.png");
     unite23 = IMG_Load("images/achats_unite_23.png");
        int liste_unite=12;
        int indiceOption=0;
         int numero[liste_unite];
                numero[0]=10;
                numero[1]=12;
                numero[2]=15;
                numero[3]=14;
                numero[4]=18;
                numero[5]=19;
                numero[6]=22;
                numero[7]=23;
                numero[8]=4;
                numero[9]=5;
                numero[10]=8;
                numero[11]=9;
              int prix_tous_unite[liste_unite];
                prix_tous_unite[0]=prix_unite10;
                prix_tous_unite[1]=prix_unite12;
                prix_tous_unite[2]=prix_unite15;
                prix_tous_unite[3]=prix_unite14;
                prix_tous_unite[4]=prix_unite18;
                prix_tous_unite[5]=prix_unite19;
                prix_tous_unite[6]=prix_unite22;
                prix_tous_unite[7]=prix_unite23;
                prix_tous_unite[8]=prix_unite4;
                prix_tous_unite[9]=prix_unite5;
                prix_tous_unite[10]=prix_unite8;
                prix_tous_unite[11]=prix_unite9;
    SDL_Event event;
    SDL_Surface *menu = NULL;
    SDL_Surface* texte = NULL;
    SDL_Rect positionMenu;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte2 = NULL;
    SDL_Rect positionText;
    SDL_Color blanc = { 255, 255, 255};
    SDL_Color red = {255, 0, 0};
    SDL_Color or_color = {240, 205, 115};
    TTF_Font* font = NULL;
    TTF_Init();
    font = TTF_OpenFont("Aloevera-OVoWO.ttf",25);
    font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",22);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Rect position;
    positionMenu.y = 0;
    SDL_WM_SetCaption("Heroes magic war",NULL);
    int continuer=1;
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
                    int ligne_unitejoueur=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UJ,JOUEUR_HUMAIN,NUM_UNITE_UJ,numero[x],NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
                     if(ligne_unitejoueur!=-1){
                        unitesJoueurs[ligne_unitejoueur][NOMBRE_UJ]=unitesJoueurs[ligne_unitejoueur][NOMBRE_UJ]+soldat;
                     }
               int ligne_unite=chercherIndiceAvecUneCondition(NUM_UNITE_U,numero[x],NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);
               int ligne_joueur=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,0,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
               int ligne_moins=chercherIndiceAvecUneCondition(NUM_JOUEUR_UJ,-1,NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
                        unitesJoueurs[ligne_moins][NUM_JOUEUR_UJ]=0;
                        unitesJoueurs[ligne_moins][NUM_UNITE_UJ]=numero[x];
                        unitesJoueurs[ligne_moins][TYPE_UJ]=unites[ligne_unite][TYPE_U];
                        unitesJoueurs[ligne_moins][SANTE_UJ]=unites[ligne_unite][SANTE_U];
                        unitesJoueurs[ligne_moins][ATTAQUE_UJ]=unites[ligne_unite][ATTAQUE_U];
                        unitesJoueurs[ligne_moins][DEFENSE_UJ]=unites[ligne_unite][DEFENSE_U];
                        unitesJoueurs[ligne_moins][DEGATS_UJ]=unites[ligne_unite][DEGATS_U];
                        unitesJoueurs[ligne_moins][MOUVEMENTS_UJ]=unites[ligne_unite][MOUVEMENTS_U];
                        unitesJoueurs[ligne_moins][NOMBRE_UJ]=soldat;
                        unitesJoueurs[ligne_moins][NIVEAU_UJ]=0;
                        unitesJoueurs[ligne_moins][UNITE_ACTIVE]=0;
                        joueurs[ligne_joueur][OR]=(joueurs[ligne_joueur][OR]-(prix_tous_unite[x]));
                        continuer = 0;
                    }
                else
                {
                continuer = 0;
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

          char* phrase[liste_unite];
            phrase[0]= unite10;
            phrase[1]= unite12;
            phrase[2]= unite15;
            phrase[3]= unite14;
            phrase[4]= unite18;
            phrase[5]= unite19;
            phrase[6]= unite22;
            phrase[7]= unite23;
            phrase[8]= unite4;
            phrase[9]= unite5;
            phrase[10]= unite8;
            phrase[11]= unite9;

        for(int i=0;i<liste_unite;i++){
            if(i==x){


                SDL_BlitSurface(phrase[i], NULL, ec, &positionMenu);
            }
        }
           if(font != 0)
        {
            char* conf[1];
            char* ch[50];
            sprintf(ch,"%d",soldat);
            conf[0]= "OUI";
            conf[1]= "NON";
            int i, y_depart=535;
            SDL_Color couleur_a_utiliser;

                if(1==indiceOption)
                {
                texte = TTF_RenderText_Blended(font, conf[1],red);
                positionText.x =180;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                texte = TTF_RenderText_Blended(font, conf[0], blanc);
                positionText.x =560;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                }
                else
                {
                texte = TTF_RenderText_Blended(font, conf[1], blanc);
                positionText.x =180;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                texte = TTF_RenderText_Blended(font, conf[0], red);
                positionText.x =560;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                }

                texte2 = TTF_RenderText_Blended(font1,ch,or_color);
                positionText.x =387;
                positionText.y =325;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);


        }
    SDL_Flip(ec);
    }
}

void payer_unite(SDL_Surface *ec,int nbr,int x,int or,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS],int prix_unite10,int prix_unite12,
int prix_unite15,int prix_unite14,int prix_unite18,int prix_unite19,int prix_unite22,int prix_unite23,
int prix_unite4,int prix_unite5,int prix_unite8,int prix_unite9){
     int liste_unite=12;
      int phrase[liste_unite];
        phrase[0]= prix_unite10;
        phrase[1]= prix_unite12;
        phrase[2]= prix_unite15;
        phrase[3]= prix_unite14;
        phrase[4]= prix_unite18;
        phrase[5]= prix_unite19;
        phrase[6]= prix_unite22;
        phrase[7]= prix_unite23;
        phrase[8]= prix_unite4;
        phrase[9]= prix_unite5;
        phrase[10]= prix_unite8;
        phrase[11]= prix_unite9;

           int numero[liste_unite];
                numero[0]=10;
                numero[1]=12;
                numero[2]=15;
                numero[3]=14;
                numero[4]=18;
                numero[5]=19;
                numero[6]=22;
                numero[7]=23;
                numero[8]=4;
                numero[9]=5;
                numero[10]=8;
                numero[11]=9;

    int ligne_unitejoueur=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UJ,JOUEUR_HUMAIN,NUM_UNITE_UJ,numero[x],NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
     for(int i=0;i<=liste_unite;i++){
        if(i==x){
              if(phrase[i]>or){
                     warning_achats_unite(ec);
                }else{
                      confirmation_achats_unite(ec,nbr,i,or,joueurs,unites,unitesJoueurs,prix_unite10,prix_unite12,prix_unite15,prix_unite14,prix_unite18,prix_unite19,prix_unite22,prix_unite23,prix_unite4,prix_unite5,prix_unite8,prix_unite9);
                }
            }
        }

}

void info_unite(SDL_Surface *ec,int x){
     SDL_Surface *unite4 = NULL, *unite5 = NULL, *unite8 = NULL, *unite9 = NULL,*unite10 = NULL,*unite12 = NULL, *unite14 = NULL,*unite15= NULL,*unite18 = NULL, *unite19 = NULL,*unite22= NULL,*unite23=NULL;
     unite4 = IMG_Load("images/info_unite4.png");
     unite5 = IMG_Load("images/info_unite5.png");
     unite8 = IMG_Load("images/info_unite8.png");
     unite9 = IMG_Load("images/info_unite9.png");
     unite10 = IMG_Load("images/info_unite10.png");
     unite12 = IMG_Load("images/info_unite12.png");
     unite14 = IMG_Load("images/info_unite14.png");
     unite15 = IMG_Load("images/info_unite15.png");
     unite18= IMG_Load("images/info_unite18.png");
     unite19 = IMG_Load("images/info_unite19.png");
     unite22 = IMG_Load("images/info_unite22.png");
     unite23 = IMG_Load("images/info_unite23.png");
     int liste_unite=12;
    SDL_Event event;
    SDL_Surface *menu = NULL;
    SDL_Surface* texte = NULL;
    SDL_Rect positionMenu;
    SDL_Rect positionText;
    SDL_Color blanc = { 255, 255, 255};
    TTF_Font* font = NULL;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Rect position;
    positionMenu.y = 0;
   SDL_WM_SetCaption("Heroes magic war",NULL);
    int continuer=1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int y_clicked=event.button.y;
                int x_clicked=event.button.x;
                  int xmin=710;
                  int xmax=750;
                  int ymin=40;
                  int ymax=91;
                    if(x_clicked>xmin && x_clicked<xmax){
                    if(y_clicked>ymin && y_clicked<ymax){
                        continuer=0;
                    }
                  }

            }break;
        }
        char* phrase[liste_unite];
        phrase[0]= unite10;
        phrase[1]= unite12;
        phrase[2]= unite15;
        phrase[3]= unite14;

        phrase[4]= unite18;
        phrase[5]= unite19;
        phrase[6]= unite22;
        phrase[7]= unite23;

        phrase[8]= unite4;
        phrase[9]= unite5;
        phrase[10]= unite8;
        phrase[11]= unite9;
        for(int i=0;i<liste_unite;i++){
            if(i==x){
                  SDL_BlitSurface(phrase[i], NULL, ec, &positionMenu);
            }
        }
    SDL_Flip(ec);
    }
}

void LancerAcheterUnite(SDL_Surface *ec,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
 int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
 int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){

     SDL_Surface *icon_seul_unite=NULL,*icon_dix_unite=NULL, *info_cadre=NULL, *unite4 = NULL, *unite5 = NULL, *unite8 = NULL, *unite9 = NULL,*unite10 = NULL, *unite12 = NULL, *unite14 = NULL, *unite18 = NULL,*unite19 = NULL,
     *unite22 = NULL, *unite23 = NULL,*unite15 = NULL,*circle=NULL;
     circle = IMG_Load("images/circle.png");
     unite4 = IMG_Load("images/unite4.png");
     unite15 = IMG_Load("images/unites15.png");
     unite5 = IMG_Load("images/unite5.png");
     unite8 = IMG_Load("images/unite8.png");
     unite9 = IMG_Load("images/unite9.png");
     unite14 = IMG_Load("images/unite14.png");
     unite10 = IMG_Load("images/unite10.png");
     unite12= IMG_Load("images/unite12.png");
     unite18 = IMG_Load("images/unite18.png");
     unite19 = IMG_Load("images/unite19.png");
     unite22= IMG_Load("images/unite22.png");
     unite23 = IMG_Load("images/unite23.png");
     info_cadre=IMG_Load("images/info.png");
     icon_seul_unite=IMG_Load("images/final.png");
     icon_dix_unite=IMG_Load("images/10unite-removebg-preview.png");

    int liste_unite=12;
        char* name[liste_unite];
            name[0]=" Bubo Bubo  ";
            name[1]=" Hyena Rider";
            name[2]="    Spirit    ";
            name[3]=" Mindslaver ";
            name[4]=" Cerberus   ";
            name[5]=" Swordsman  ";
            name[6]=" Frost Mage ";
            name[7]="  Sister    ";
            name[8]=" Skeleton   ";
            name[9]="Goblin Thief";
            name[10]="   Zombie  ";
            name[11]="Fire Elemental";
              int numero[liste_unite];
                numero[0]=10;
                numero[1]=12;
                numero[2]=15;
                numero[3]=14;
                numero[4]=18;
                numero[5]=19;
                numero[6]=22;
                numero[7]=23;
                numero[8]=4;
                numero[9]=5;
                numero[10]=8;
                numero[11]=9;
     SDL_Rect position;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    TTF_Font* font2 = NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("UNITE",NULL);
    menu = IMG_Load("images/back_unite.png");
    int indiceOption=0;
    positionMenu.x = 0;
    int j;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",21);
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",16);
        font2 = TTF_OpenFont("Aloevera-OVoWO.ttf",13);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
        SDL_Rect positionText;

    while (continuer)
    {
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
            char* info[liste_unite];
            info[0]=info_cadre;
            info[1]=info_cadre;
            info[2]=info_cadre;
            info[3]=info_cadre;
            info[4]=info_cadre;
            info[5]=info_cadre;
            info[6]=info_cadre;
            info[7]=info_cadre;
            info[8]=info_cadre;
            info[9]=info_cadre;
            info[10]=info_cadre;
            info[11]=info_cadre;
            char* seul_unite[liste_unite];
            seul_unite[0]=icon_seul_unite;
            seul_unite[1]=icon_seul_unite;
            seul_unite[2]=icon_seul_unite;
            seul_unite[3]=icon_seul_unite;
            seul_unite[4]=icon_seul_unite;
            seul_unite[5]=icon_seul_unite;
            seul_unite[6]=icon_seul_unite;
            seul_unite[7]=icon_seul_unite;
            seul_unite[8]=icon_seul_unite;
            seul_unite[9]=icon_seul_unite;
            seul_unite[10]=icon_seul_unite;
            seul_unite[11]=icon_seul_unite;
            char* dix_unite[liste_unite];
            dix_unite[0]=icon_dix_unite;
            dix_unite[1]=icon_dix_unite;
            dix_unite[2]=icon_dix_unite;
            dix_unite[3]=icon_dix_unite;
            dix_unite[4]=icon_dix_unite;
            dix_unite[5]=icon_dix_unite;
            dix_unite[6]=icon_dix_unite;
            dix_unite[7]=icon_dix_unite;
            dix_unite[8]=icon_dix_unite;
            dix_unite[9]=icon_dix_unite;
            dix_unite[10]=icon_dix_unite;
            dix_unite[11]=icon_dix_unite;
            char* phrase[liste_unite];
            phrase[0]= unite10;
            phrase[1]= unite12;
            phrase[2]= unite15;
            phrase[3]= unite14;
            phrase[4]= unite18;
            phrase[5]= unite19;
            phrase[6]= unite22;
            phrase[7]= unite23;
            phrase[8]= unite4;
            phrase[9]= unite5;
            phrase[10]= unite8;
            phrase[11]= unite9;

            int i=0;
                int lign_numjoueur=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,JOUEUR_HUMAIN,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
                int or=joueurs[lign_numjoueur][OR];
                char *or_humaine[10];
                sprintf(or_humaine,"%d",or);
                int lign_unite4=chercherIndiceAvecUneCondition(NUM_UNITE_U,4,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);
                int lign_unite5=chercherIndiceAvecUneCondition(NUM_UNITE_U,5,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);
                int lign_unite8=chercherIndiceAvecUneCondition(NUM_UNITE_U,8,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);
                int lign_unite9=chercherIndiceAvecUneCondition(NUM_UNITE_U,9,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);

                int lign_unite10=chercherIndiceAvecUneCondition(NUM_UNITE_U,10,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);
                int lign_unite12_=chercherIndiceAvecUneCondition(NUM_UNITE_U,12,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);
                int lign_unite14=chercherIndiceAvecUneCondition(NUM_UNITE_U,14,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);
                int lign_unite15=chercherIndiceAvecUneCondition(NUM_UNITE_U,15,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);

                int lign_unite18=chercherIndiceAvecUneCondition(NUM_UNITE_U,18,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);
                int lign_unite19_=chercherIndiceAvecUneCondition(NUM_UNITE_U,19,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);
                int lign_unite22=chercherIndiceAvecUneCondition(NUM_UNITE_U,22,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);
                int lign_unite23=chercherIndiceAvecUneCondition(NUM_UNITE_U,23,NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites);

                int prix_unite4=unites[lign_unite4][PRIX_U];
                int prix_unite5=unites[lign_unite5][PRIX_U];
                int prix_unite8=unites[lign_unite8][PRIX_U];
                int prix_unite9=unites[lign_unite9][PRIX_U];

                int prix_unite10=unites[lign_unite10][PRIX_U];
                int prix_unite12=unites[lign_unite12_][PRIX_U];
                int prix_unite14=unites[lign_unite14][PRIX_U];
                int prix_unite15=unites[lign_unite15][PRIX_U];

                int prix_unite18=unites[lign_unite18][PRIX_U];
                int prix_unite19=unites[lign_unite19_][PRIX_U];
                int prix_unite22=unites[lign_unite22][PRIX_U];
                int prix_unite23=unites[lign_unite23][PRIX_U];

                int prix_tous_unite[liste_unite];
                prix_tous_unite[0]=prix_unite10;
                prix_tous_unite[1]=prix_unite12;
                prix_tous_unite[2]=prix_unite15;
                prix_tous_unite[3]=prix_unite14;
                prix_tous_unite[4]=prix_unite18;
                prix_tous_unite[5]=prix_unite19;
                prix_tous_unite[6]=prix_unite22;
                prix_tous_unite[7]=prix_unite23;
                prix_tous_unite[8]=prix_unite4;
                prix_tous_unite[9]=prix_unite5;
                prix_tous_unite[10]=prix_unite8;
                prix_tous_unite[11]=prix_unite9;

                char* afficher_prix_unite10[10];
                char* afficher_prix_unite12[10];
                char* afficher_prix_unite14[10];
                char* afficher_prix_unite15[10];
                char* afficher_prix_unite5[10];
                char* afficher_prix_unite18[10];
                char* afficher_prix_unite8[10];
                char* afficher_prix_unite9[10];
                char* afficher_prix_unite4[10];
                char* afficher_prix_unite19[10];
                char* afficher_prix_unite22[10];
                char* afficher_prix_unite23[10];
                    sprintf(afficher_prix_unite8,"%d",prix_unite8);
                    sprintf(afficher_prix_unite5,"%d",prix_unite5);
                    sprintf(afficher_prix_unite9,"%d",prix_unite9);
                    sprintf(afficher_prix_unite4,"%d",prix_unite4);
                    sprintf(afficher_prix_unite19,"%d",prix_unite19);
                    sprintf(afficher_prix_unite22,"%d",prix_unite22);
                    sprintf(afficher_prix_unite23,"%d",prix_unite23);
                    sprintf(afficher_prix_unite10,"%d",prix_unite10);
                    sprintf(afficher_prix_unite12,"%d",prix_unite12);
                    sprintf(afficher_prix_unite14,"%d",prix_unite14);
                    sprintf(afficher_prix_unite15,"%d",prix_unite15);
                    sprintf(afficher_prix_unite18,"%d",prix_unite18);

               char* afficher_tous_prix[liste_unite];
               afficher_tous_prix[0]=afficher_prix_unite10;
               afficher_tous_prix[1]=afficher_prix_unite12;
               afficher_tous_prix[2]=afficher_prix_unite15;
               afficher_tous_prix[3]=afficher_prix_unite14;
               afficher_tous_prix[4]=afficher_prix_unite18;
               afficher_tous_prix[5]=afficher_prix_unite19;
               afficher_tous_prix[6]=afficher_prix_unite22;
               afficher_tous_prix[7]=afficher_prix_unite23;
               afficher_tous_prix[8]=afficher_prix_unite4;
               afficher_tous_prix[9]=afficher_prix_unite5;
               afficher_tous_prix[10]=afficher_prix_unite8;
               afficher_tous_prix[11]=afficher_prix_unite9;

            int y_depart=110,y_depart_titre=90;
            int x_depart=10,x_depart_titre=80;
            int tab_info[12][12];
            int tab_prix[12][12];
            int tab_prix_dix[12][12];

                tab_info[0][0]=14;
                tab_info[0][1]=106;
                tab_info[0][2]=170;
                tab_info[0][3]=195;

                tab_info[1][0]=229;
                tab_info[1][1]=319;
                tab_info[1][2]=170;
                tab_info[1][3]=195;

                tab_info[2][0]= 442;
                tab_info[2][1]= 536;
                tab_info[2][2]=170;
                tab_info[2][3]=195;

                tab_info[3][0]= 659;
                tab_info[3][1]= 752;
                tab_info[3][2]=170;
                tab_info[3][3]=195;

                tab_info[4][0]= 14;
                tab_info[4][1]=106;
                tab_info[4][2]=348;
                tab_info[4][3]=371;

                tab_info[5][0]= 229;
                tab_info[5][1]=319;
                tab_info[5][2]=348;
                tab_info[5][3]=371;

                tab_info[6][0]= 442;
                tab_info[6][1]=536;
                tab_info[6][2]=348;
                tab_info[6][3]=371;


                tab_info[7][0]= 659;
                tab_info[7][1]=752;
                tab_info[7][2]=348;
                tab_info[7][3]=371;


                tab_info[8][0]=14;
                tab_info[8][1]=106;
                tab_info[8][2]=522;
                tab_info[8][3]=543;


                tab_info[9][0]=229;
                tab_info[9][1]=319;
                tab_info[9][2]=522;
                tab_info[9][3]=543;


                tab_info[10][0]=442;
                tab_info[10][1]=536;
                tab_info[10][2]=522;
                tab_info[10][3]=543;


                tab_info[11][0]=659;
                tab_info[11][1]=752;
                tab_info[11][2]=522;
                tab_info[11][3]=543;

                tab_prix[0][0]=11;
                tab_prix[0][1]=53;
                tab_prix[0][2]=201;
                tab_prix[0][3]=244;

                tab_prix[1][0]=226;
                tab_prix[1][1]=267;
                tab_prix[1][2]=201;
                tab_prix[1][3]=244;

                tab_prix[2][0]=443;
                tab_prix[2][1]=483;
                tab_prix[2][2]=201;
                tab_prix[2][3]=244;

                tab_prix[3][0]=656;
                tab_prix[3][1]=696;
                tab_prix[3][2]=201;
                tab_prix[3][3]=244;

                tab_prix[4][0]=11;
                tab_prix[4][1]=53;
                tab_prix[4][2]=378;
                tab_prix[4][3]=421;

                tab_prix[5][0]=226;
                tab_prix[5][1]=267;
                tab_prix[5][2]=378;
                tab_prix[5][3]=421;

                tab_prix[6][0]=443;
                tab_prix[6][1]=483;
                tab_prix[6][2]=378;
                tab_prix[6][3]=421;

                tab_prix[7][0]=656;
                tab_prix[7][1]=696;
                tab_prix[7][2]=378;
                tab_prix[7][3]=421;

                tab_prix[8][0]=11;
                tab_prix[8][1]=53;
                tab_prix[8][2]=551;
                tab_prix[8][3]=595;

                tab_prix[9][0]=226;
                tab_prix[9][1]=267;
                tab_prix[9][2]=551;
                tab_prix[9][3]=595;


                tab_prix[10][0]=443;
                tab_prix[10][1]=483;
                tab_prix[10][2]=551;
                tab_prix[10][3]=595;

                tab_prix[11][0]=656;
                tab_prix[11][1]=696;
                tab_prix[11][2]=551;
                tab_prix[11][3]=595;


                tab_prix_dix[0][0]=65;
                tab_prix_dix[0][1]=107;
                tab_prix_dix[0][2]=201;
                tab_prix_dix[0][3]=245;


                tab_prix_dix[1][0]=279;
                tab_prix_dix[1][1]=321;
                tab_prix_dix[1][2]=201;
                tab_prix_dix[1][3]=245;


                tab_prix_dix[2][0]=493;
                tab_prix_dix[2][1]=538;
                tab_prix_dix[2][2]=201;
                tab_prix_dix[2][3]=245;


                tab_prix_dix[3][0]=709;
                tab_prix_dix[3][1]=751;
                tab_prix_dix[3][2]=201;
                tab_prix_dix[3][3]=245;


                tab_prix_dix[4][0]=65;
                tab_prix_dix[4][1]=107;
                tab_prix_dix[4][2]=378;
                tab_prix_dix[4][3]=422;


                tab_prix_dix[5][0]=279;
                tab_prix_dix[5][1]=321;
                tab_prix_dix[5][2]=378;
                tab_prix_dix[5][3]=422;


                tab_prix_dix[6][0]=493;
                tab_prix_dix[6][1]=538;
                tab_prix_dix[6][2]=378;
                tab_prix_dix[6][3]=422;


                tab_prix_dix[7][0]=709;
                tab_prix_dix[7][1]=751;
                tab_prix_dix[7][2]=378;
                tab_prix_dix[7][3]=422;


                tab_prix_dix[8][0]=65;
                tab_prix_dix[8][1]=107;
                tab_prix_dix[8][2]=553;
                tab_prix_dix[8][3]=596;


                tab_prix_dix[9][0]=279;
                tab_prix_dix[9][1]=321;
                tab_prix_dix[9][2]=553;
                tab_prix_dix[9][3]=596;


                tab_prix_dix[10][0]=493;
                tab_prix_dix[10][1]=538;
                tab_prix_dix[10][2]=553;
                tab_prix_dix[10][3]=596;


                tab_prix_dix[11][0]=709;
                tab_prix_dix[11][1]=751;
                tab_prix_dix[11][2]=553;
                tab_prix_dix[11][3]=596;

        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            { int x_clicked=event.button.x;
              int y_clicked=event.button.y;

                for(i=0;i<liste_unite;i++){
                    if((x_clicked>tab_prix[i][0])&&(x_clicked<tab_prix[i][1])){
                         if((y_clicked>tab_prix[i][2])&&(y_clicked<tab_prix[i][3])){
                                  int nbr_soldat=1;
                                 payer_unite(ec,nbr_soldat,i,or,joueurs,unites,unitesJoueurs,prix_unite10,prix_unite12,prix_unite15,prix_unite14,prix_unite18,prix_unite19,prix_unite22,prix_unite23,prix_unite4,prix_unite5,prix_unite8,prix_unite9);
                            }
                                }
                     }

                for(i=0;i<liste_unite;i++){
                    if((x_clicked>tab_prix_dix[i][0])&&(x_clicked<tab_prix_dix[i][1])){
                         if((y_clicked>tab_prix_dix[i][2])&&(y_clicked<tab_prix_dix[i][3])){
                                     int nbr_soldat=10;
                                    payer_unite(ec,nbr_soldat,i,or,joueurs,unites,unitesJoueurs,prix_unite10*10,prix_unite12*10,prix_unite15*10,prix_unite14*10,prix_unite18*10,prix_unite19*10,prix_unite22*10,prix_unite23*10,prix_unite4*10,prix_unite5*10,prix_unite8*10,prix_unite9*10);
                            }
                                }
                }

                for(i=0;i<liste_unite;i++){
                    if((x_clicked>tab_info[i][0])&&(x_clicked<tab_info[i][1])){
                         if((y_clicked>tab_info[i][2])&&(y_clicked<tab_info[i][3])){
                                    info_unite(ec,i);
                            }
                                }
                }

                char *ch[50];
                sprintf(ch,"%d",x_clicked);
                texte = TTF_RenderText_Blended(font,ch,pink);
                positionText.x =10;
                positionText.y =48;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                char *ch1[50];
                sprintf(ch1,"%d",y_clicked);
                texte = TTF_RenderText_Blended(font,ch1,pink);
                positionText.x =80;
                positionText.y =50;
                SDL_BlitSurface(texte, NULL, ec, &positionText);

            }break;

         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            }
        }

          int nbr_unite=0;
            while(nbr_unite!=liste_unite)
            {
                position.x=215*(nbr_unite%4)+10;
                position.y=176*(nbr_unite/4)+75;
                SDL_BlitSurface(phrase[nbr_unite], NULL, ec, &position);
                nbr_unite++;
            }

            for(i=0;i<liste_unite;i++){
                position.x=215*(i%4)+10;
                position.y=176*(i/4)+198;
                SDL_BlitSurface(seul_unite[i], NULL, ec, &position);
            }

            for(i=0;i<liste_unite;i++){
                 position.x=215*(i%4)+100;
                 position.y=176*(i/4)+100;
                 SDL_BlitSurface(circle, NULL, ec, &position);
            }

            for(i=0;i<liste_unite;i++){
                 position.x=215*(i%4)+60;
                 position.y=176*(i/4)+198;
                 SDL_BlitSurface(dix_unite[i], NULL, ec, &position);
            }

             for(i=0;i<liste_unite;i++){
                 texte = TTF_RenderText_Blended(font2,name[i],blanc);
                 position.x=215*(i%4)+20;
                 position.y=176*(i/4)+80;
                 SDL_BlitSurface(texte, NULL, ec, &position);
            }

           for(i=0;i<liste_unite;i++){
                position.x=215*(i%4)+10;
                position.y=176*(i/4)+169;
                SDL_BlitSurface(info[i], NULL, ec, &position);
            }

         for(i=0;i<liste_unite;i++){
                if(prix_tous_unite[i]>or){
                texte = TTF_RenderText_Blended(font,afficher_tous_prix[i],red);
                }else{
                texte = TTF_RenderText_Blended(font,afficher_tous_prix[i],or_color);
                }
                position.x=215*(i%4)+33;
                position.y=176*(i/4)+228;
                SDL_BlitSurface(texte, NULL, ec, &position);
            }

             for(i=0;i<liste_unite;i++){
                char* ch_prix[12];
                sprintf(ch_prix,"%d",prix_tous_unite[i]*10);
                if(prix_tous_unite[i]*10>or){
                texte = TTF_RenderText_Blended(font1,ch_prix,red);
                }else{
                texte = TTF_RenderText_Blended(font1,ch_prix,or_color);
                }
                 position.x=215*(i%4)+84;
                 position.y=176*(i/4)+228;
                 SDL_BlitSurface(texte, NULL, ec, &position);
            }

            for(i=0;i<liste_unite;i++){
                int chercher=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_UJ,JOUEUR_HUMAIN,NUM_UNITE_UJ,numero[i],NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs);
                if(chercher==-1){
                char *ch2[20];
                sprintf(ch2,"%d",0);
                texte2 = TTF_RenderText_Blended(font1,ch2,blanc);
                }else{
                char *ch2[20];
                sprintf(ch2,"%d",unitesJoueurs[chercher][NOMBRE_UJ]);
                texte2 = TTF_RenderText_Blended(font1,ch2,blanc);
                }
                 position.x=215*(i%4)+120;
                 position.y=176*(i/4)+115;
                 SDL_BlitSurface(texte2, NULL, ec, &position);
            }

                texte_titre = TTF_RenderText_Blended(font,or_humaine,or_color);
                positionText.x =650;
                positionText.y =22;
                SDL_BlitSurface(texte_titre, NULL, ec, &positionText);

                SDL_Flip(ec);
         }

 SDL_FreeSurface(menu);


}

void acheter_unite(SDL_Surface *ec,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){
int nombre_phrase=2;
    SDL_Surface *menu = NULL,*select=NULL;
    SDL_Rect positionMenu;
    SDL_Rect  position;
    SDL_Event event;
    TTF_Font* font = NULL,*font1=NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Achat Unites",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
         font = TTF_OpenFont("Aloevera-OVoWO.ttf",27);
         font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",32);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color red = {255 ,0 ,127};
        SDL_Color pink = {255 ,0 ,0};
        SDL_Rect positionText;
        menu = IMG_Load("images/menu1.png");
        select = IMG_Load("images/select.png");

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
                     LancerAcheterUnite(ec,joueurs,unites,unitesJoueurs);
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
            char ch[50]="Achat Unit�s";
            char* phrase[nombre_phrase];
            phrase[0]= "  Acheter des nouvelles unit�s ";
            phrase[1]= "  Revenir au menu pr�c�dent    ";
            int i, y_depart=230, y_depart_titre=80;
            SDL_Color couleur_a_utiliser;
              for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blanc;
                    position.x =273;
                    position.y=y_depart-10+i*40;
                    SDL_BlitSurface(select, NULL, ec, &position);
                }
                else
                {
                   couleur_a_utiliser=red;
                    //positionText.x =320;

                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                positionText.x =280;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
            }
                texte_titre = TTF_RenderText_Blended(font1,ch,yellow);
                positionText.x =320;
                positionText.y =y_depart_titre;
                SDL_BlitSurface(texte_titre, NULL, ec, &positionText);
        }
        SDL_Flip(ec);
    }
    SDL_FreeSurface(texte);

    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}


void  acheter_hero( SDL_Surface *ec,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
    int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
    int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS]){

    int nombre_phrase=2;
    SDL_Surface *menu = NULL,*select=NULL;
    SDL_Rect positionMenu;
    SDL_Rect position;
    SDL_Event event;
    TTF_Font* font = NULL,*font1=NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Achat Heros",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",27);
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",32);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255 ,0 ,127};
         SDL_Color blanc = {255 ,255 ,255};
        SDL_Color pink = {255 ,0 ,0};
        SDL_Color blue = {0, 109, 255};
        SDL_Rect positionText;
        menu = IMG_Load("images/menu1.png");
        select = IMG_Load("images/select.png");

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

                     lancerAchatHeros(ec,JOUEUR_HUMAIN,joueurs,heros,herosJoueurs);
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
            char ch[50]="Achat H�ros";
            char* phrase[nombre_phrase];
            phrase[0]= "  Acheter un nouveau h�ro        ";
            phrase[1]= "  Revenir au menu pr�c�dent      ";
            int i, y_depart=230, y_depart_titre=80;
            SDL_Color couleur_a_utiliser;
                for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blanc;

                    position.x =290;
                    position.y=y_depart-10+i*40;
                    SDL_BlitSurface(select, NULL, ec, &position);
                }
                else
                {
                    couleur_a_utiliser=red;

                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                positionText.x =320;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
            }
                texte_titre = TTF_RenderText_Blended(font1,ch,yellow);
                positionText.x =330;
                positionText.y =y_depart_titre;
                SDL_BlitSurface(texte_titre, NULL, ec, &positionText);
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


void info_hero(SDL_Surface *ec,int x){
     SDL_Surface *heros1 = NULL, *heros2 = NULL, *heros3 = NULL, *heros4 = NULL,*heros5 = NULL;
     heros1 = IMG_Load("images/info_heros1.png");
     heros2 = IMG_Load("images/info_heros2.png");
     heros3 = IMG_Load("images/info_heros3.png");
     heros4 = IMG_Load("images/info_heros4.png");
     heros5 = IMG_Load("images/info_heros5.png");
     int liste_heros=5;
    SDL_Event event;
    SDL_Surface *menu = NULL;
    SDL_Surface* texte = NULL;
    SDL_Rect positionMenu;
    SDL_Rect positionText;
    SDL_Color blanc = { 255, 255, 255};
    TTF_Font* font = NULL;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Rect position;
    positionMenu.y = 0;
   SDL_WM_SetCaption("Heroes magic war",NULL);
    int continuer=1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int y_clicked=event.button.y;
                int x_clicked=event.button.x;
                 int xmin=748;
                 int xmax=797;
                 int ymin=23;
                 int ymax=76;

                  if(x_clicked>xmin && x_clicked<xmax){
                    if(y_clicked>ymin && y_clicked<ymax){
                        continuer=0;
                    }
                  }
                  else{
                  xmin=710;
                  xmax=753;
                  ymin=43;
                  ymax=92;
                    if(x_clicked>xmin && x_clicked<xmax){
                    if(y_clicked>ymin && y_clicked<ymax){
                       continuer=0;
                    }
                  }

                  }

            }break;
        }
        char* phrase[liste_heros];
        phrase[0]= heros1;
        phrase[1]= heros2;
        phrase[2]= heros3;
        phrase[3]= heros4;
        phrase[4]= heros5;
        for(int i=0;i<liste_heros;i++){
            if(i==x){
                  SDL_BlitSurface(phrase[i], NULL, ec, &positionMenu);
            }
        }
    SDL_Flip(ec);
    }
    SDL_FreeSurface(heros1);
    SDL_FreeSurface(heros2);
    SDL_FreeSurface(heros3);
    SDL_FreeSurface(heros4);
    SDL_FreeSurface(heros5);
}

void warning_achats(SDL_Surface *ec){
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Event event;
    positionMenu.y = 0;
    int continuer=1;
    SDL_WM_SetCaption("Annuler",NULL);
    menu = IMG_Load("images/Achat annule ,le joueur ne dispose pas du solde suffisant pour acheter cet hero! (1).png");
    while (continuer==1)
    {   SDL_BlitSurface(menu, NULL, ec, &positionMenu);
       SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
        case SDLK_ESCAPE:
            continuer = 0;
            break;
        }
        }
    SDL_Flip(ec);
    }
SDL_FreeSurface(menu);
}

void confirmation_achats_heros(SDL_Surface *ec,int x,int or,
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],int prix_heros1,
int prix_heros2,int prix_heros3,int prix_heros4,int prix_heros5){
     SDL_Surface *heros1 = NULL, *heros2 = NULL, *heros3 = NULL, *heros4 = NULL,*heros5 = NULL;
     heros1 = IMG_Load("images/achat_heros1.png");
     heros2 = IMG_Load("images/achat_heros2.png");
     heros3 = IMG_Load("images/achat_heros3.png");
     heros4 = IMG_Load("images/achat_heros4.png");
     heros5 = IMG_Load("images/achat_heros5.png");
        int liste_heros=5;
        int indiceOption=0;
        int  prix[liste_heros];
        prix[0]= prix_heros1;
        prix[1]= prix_heros2;
        prix[2]= prix_heros3;
        prix[3]= prix_heros4;
        prix[4]= prix_heros5;
    SDL_Event event;
    SDL_Surface *menu = NULL;
    SDL_Surface* texte = NULL;
    SDL_Rect positionMenu;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte2 = NULL;
    SDL_Rect positionText;
    SDL_Color blanc = { 255, 255, 255};
    SDL_Color red = {255, 0, 0};
    SDL_Color or_color = {240, 205, 115};
    TTF_Font* font = NULL;
    TTF_Init();
    font = TTF_OpenFont("Aloevera-OVoWO.ttf",25);
    font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",22);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    SDL_Rect position;
    positionMenu.y = 0;
    SDL_WM_SetCaption("Heroes magic war",NULL);
    int continuer=1;
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
                int lign_numhero=chercherIndiceAvecUneCondition(NUM_HERO_H,x+1,NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros);
                int lign_numjoueur=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,JOUEUR_HUMAIN,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
                int lign_herojoueur=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,JOUEUR_HUMAIN,NUM_HERO_HJ,x,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
                int lign_moins=chercherIndiceAvecUneCondition(NUM_JOUEUR_HJ,-1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
                herosJoueurs[lign_moins][NUM_JOUEUR_HJ]=0;
                herosJoueurs[lign_moins][NUM_HERO_HJ]=x+1;
                herosJoueurs[lign_moins][HERO_PRINCIPAL]=0;
                herosJoueurs[lign_moins][POINTS_EXPERIENCE]=0;
                herosJoueurs[lign_moins][NIVEAU_HJ]=0;
                herosJoueurs[lign_moins][POINTS_COMPETENCE_HJ]=0;
                herosJoueurs[lign_moins][BONUS_ATTAQUE_HJ]=heros[lign_numhero][BONUS_ATTAQUE_H];
                herosJoueurs[lign_moins][BONUS_DEFENSE_HJ]=heros[lign_numhero][BONUS_DEFENSE_H];
                herosJoueurs[lign_moins][CHANCE_CRITIQUE_HJ]=heros[lign_numhero][CHANCE_CRITIQUE_H];
                herosJoueurs[lign_moins][MORAL_HJ]=heros[lign_numhero][MORAL_H];
                herosJoueurs[lign_moins][POINTS_ATTAQUE_SPECIALE_HJ]=heros[lign_numhero][POINTS_ATTAQUE_SPECIALE_H];
                joueurs[lign_numjoueur][OR]=joueurs[lign_numjoueur][OR]-prix[x];
                continuer = 0;
                }
                else
                {
                continuer = 0;
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

        char* phrase[liste_heros];
        phrase[0]= heros1;
        phrase[1]= heros2;
        phrase[2]= heros3;
        phrase[3]= heros4;
        phrase[4]= heros5;
        for(int i=0;i<liste_heros;i++){
            if(i==x){
                  SDL_BlitSurface(phrase[i], NULL, ec, &positionMenu);
            }
        }
           if(font != 0)
        {
            char* conf[1];
            char* ch1[50];
            sprintf(ch1,"L'achat h�ro  %d",x+1);
            conf[0]= "OUI";
            conf[1]= "NON";
            int i, y_depart=510;
            SDL_Color couleur_a_utiliser;

                if(1==indiceOption)
                {
                texte = TTF_RenderText_Blended(font, conf[1],red);
                positionText.x =260;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                texte = TTF_RenderText_Blended(font, conf[0], blanc);
                positionText.x =510;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                }
                else
                {
                texte = TTF_RenderText_Blended(font, conf[1], blanc);
                positionText.x =260;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                texte = TTF_RenderText_Blended(font, conf[0], red);
                positionText.x =510;
                positionText.y =y_depart;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
                }

                texte2 = TTF_RenderText_Blended(font1,ch1,or_color);
                positionText.x =290;
                positionText.y =51;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);
        }
    SDL_Flip(ec);
    }
    SDL_FreeSurface(heros1);
    SDL_FreeSurface(heros2);
    SDL_FreeSurface(heros3);
    SDL_FreeSurface(heros4);
    SDL_FreeSurface(heros5);

}

void deja_existe(SDL_Surface *ec,int x){
    SDL_Surface  *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    TTF_Font* font1 = NULL;
    SDL_Surface* texte = NULL;
    SDL_Surface* texte2 = NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("ANNULER",NULL);
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
        menu = IMG_Load("images/deja_exite.png");
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

            }
            break;
        }
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);

        if(font != 0)
        {
            char *ch[10];
            char ch1[50]=" Achat annule ,car hero ";
            char ch2[50]="   est deja acheter !";
            sprintf(ch,"%d",x+1);
            int i, y_depart=439;
            SDL_Color couleur_a_utiliser;

                texte2 = TTF_RenderText_Blended(font,ch,blanc);
                positionText.x =400;
                positionText.y =260;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);

                texte2 = TTF_RenderText_Blended(font,ch1,noir);
                positionText.x =100;
                positionText.y =260;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);

                texte2 = TTF_RenderText_Blended(font,ch2,noir);
                positionText.x =150;
                positionText.y =310;
                SDL_BlitSurface(texte2, NULL, ec, &positionText);

        }
    SDL_Flip(ec);
    }
    SDL_FreeSurface(texte);
    SDL_FreeSurface(texte2);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
}

void payer_heros(SDL_Surface *ec,int x,int or,
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],int prix_heros1,
int prix_heros2,int prix_heros3,int prix_heros4,int prix_heros5){
      int liste_heros=5;
      char* phrase[liste_heros];
        phrase[0]= prix_heros1;
        phrase[1]= prix_heros2;
        phrase[2]= prix_heros3;
        phrase[3]= prix_heros4;
        phrase[4]= prix_heros5;
     int lign_herojoueur=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,JOUEUR_HUMAIN,NUM_HERO_HJ,x+1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
     for(int i=0;i<=liste_heros;i++){
        if(i==x){
                if(lign_herojoueur!=-1){
                     deja_existe(ec,x);
                }else if(phrase[i]>or){
                     warning_achats(ec);
                }else{
                     confirmation_achats_heros(ec,x,or,joueurs,heros,herosJoueurs,prix_heros1,prix_heros2,prix_heros3,prix_heros4,prix_heros5);
                }
            }
        }
}

void lancerAchatHeros ( SDL_Surface *ec,int numJoueur,
int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS] ){
     SDL_Rect position;
     SDL_Surface *heros1 = NULL, *heros2 = NULL, *heros3 = NULL, *heros4 = NULL,*heros5 = NULL;
     SDL_Surface *heros1_acheter = NULL, *heros2_acheter = NULL, *heros3_acheter = NULL, *heros4_acheter = NULL,*heros5_acheter = NULL;
     heros1 = IMG_Load("images/logo_heros.png");
     heros2 = IMG_Load("images/logo_heros2.png");
     heros3 = IMG_Load("images/logo_heros3.png");
     heros4 = IMG_Load("images/logo_heros4.png");
     heros5 = IMG_Load("images/logo_heros5.png");
     heros1_acheter = IMG_Load("images/logo_heros_gris1.png");
     heros2_acheter = IMG_Load("images/logo_heros_gris2.png");
     heros3_acheter = IMG_Load("images/logo_heros_gris3.png");
     heros4_acheter = IMG_Load("images/logo_heros_gris4.png");
     heros5_acheter = IMG_Load("images/logo_heros_gris5.png");
    int liste_heros=5;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL,*texte2=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Heros",NULL);
    menu = IMG_Load("images/back_heros.png");
    int indiceOption=0;
    positionMenu.x = 0;
    int j;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",19);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255, 0, 0};
        SDL_Color pink = {255, 105, 180};
        SDL_Color blue = {0, 109, 255};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color or_color={255, 179, 0};
        SDL_Rect positionText;

    while (continuer)
    {
        SDL_BlitSurface(menu, NULL, ec, &positionMenu);
        char* name[liste_heros];
            name[0]="SIR KULLICH";
            name[1]="GALTHROM";
            name[2]="IZZY THE GREAT";
            name[3]="GABO THE OLD";
            name[4]="BASILEUS";
            char* info[liste_heros];
            info[0]="INFO";
            info[1]="INFO";
            info[2]="INFO";
            info[3]="INFO";
            info[4]="INFO";
            char* phrase[liste_heros];
            phrase[0]= heros1;
            phrase[1]= heros2;
            phrase[2]= heros3;
            phrase[3]= heros4;
            phrase[4]= heros5;
            char* heros_acheter[liste_heros];
            heros_acheter[0]= heros1_acheter;
            heros_acheter[1]= heros2_acheter;
            heros_acheter[2]= heros3_acheter;
            heros_acheter[3]= heros4_acheter;
            heros_acheter[4]= heros5_acheter;
            int i=0;
                int lign_numjoueur=chercherIndiceAvecUneCondition(NUM_JOUEUR_J,JOUEUR_HUMAIN,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
                int or=joueurs[lign_numjoueur][OR];
                char *or_humaine[10];
                sprintf(or_humaine,"%d",or);
                int lign_numhero1=chercherIndiceAvecUneCondition(NUM_HERO_H,1,NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros);
                int lign_numhero2=chercherIndiceAvecUneCondition(NUM_HERO_H,2,NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros);
                int lign_numhero3=chercherIndiceAvecUneCondition(NUM_HERO_H,3,NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros);
                int lign_numhero4=chercherIndiceAvecUneCondition(NUM_HERO_H,4,NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros);
                int lign_numhero5=chercherIndiceAvecUneCondition(NUM_HERO_H,5,NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros);
                int prix_heros1=heros[lign_numhero1][PRIX_H];
                int prix_heros2=heros[lign_numhero2][PRIX_H];
                int prix_heros3=heros[lign_numhero3][PRIX_H];
                int prix_heros4=heros[lign_numhero4][PRIX_H];
                int prix_heros5=heros[lign_numhero5][PRIX_H];
                char* afficher_heros1[10];
                char* afficher_heros2[10];
                char* afficher_heros3[10];
                char* afficher_heros4[10];
                char* afficher_heros5[10];
                    sprintf(afficher_heros1,"%d",prix_heros1);
                    sprintf(afficher_heros2,"%d",prix_heros2);
                    sprintf(afficher_heros3,"%d",prix_heros3);
                    sprintf(afficher_heros4,"%d",prix_heros4);
                    sprintf(afficher_heros5,"%d",prix_heros5);
            int y_depart=110,y_depart_titre=90;
            int x_depart=2,x_depart_titre=80;
            int tab_info[5][5];
            int tab_prix[5][5];

                tab_info[0][0]=41;
                tab_info[0][1]=162;
                tab_info[0][2]=254;
                tab_info[0][3]=276;

                tab_info[1][0]= 342;
                tab_info[1][1]=463;
                tab_info[1][2]=254;
                tab_info[1][3]=276;

                tab_info[2][0]= 439;
                tab_info[2][1]= 764;
                tab_info[2][2]=254;
                tab_info[2][3]=276;

                tab_info[3][0]= 41;
                tab_info[3][1]= 162;
                tab_info[3][2]=514;
                tab_info[3][3]=538;

                tab_info[4][0]= 342;
                tab_info[4][1]=463;
                tab_info[4][2]=514;
                tab_info[4][3]=538;

                tab_prix[0][0]=37;
                tab_prix[0][1]=169;
                tab_prix[0][2]=289;
                tab_prix[0][3]=314;

                tab_prix[1][0]=338;
                tab_prix[1][1]=466;
                tab_prix[1][2]=289;
                tab_prix[1][3]=314;

                tab_prix[2][0]=637;
                tab_prix[2][1]=764;
                tab_prix[2][2]=289;
                tab_prix[2][3]=314;

                tab_prix[3][0]=37;
                tab_prix[3][1]=169;
                tab_prix[3][2]=548;
                tab_prix[3][3]=577;

                tab_prix[4][0]=388;
                tab_prix[4][1]=466;
                tab_prix[4][2]=548;
                tab_prix[4][3]=577;

        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            { int x_clicked=event.button.x;
              int y_clicked=event.button.y;
                for(i=0;i<liste_heros;i++){
                    if((x_clicked>tab_prix[i][0])&&(x_clicked<tab_prix[i][1])){
                         if((y_clicked>tab_prix[i][2])&&(y_clicked<tab_prix[i][3])){
                                    payer_heros(ec,i,or,joueurs,heros,herosJoueurs,prix_heros1,prix_heros2,prix_heros3,prix_heros4,prix_heros5);
                            }
                                }
                     }

                for(i=0;i<liste_heros;i++){
                    if((x_clicked>tab_info[i][0])&&(x_clicked<tab_info[i][1])){
                         if((y_clicked>tab_info[i][2])&&(y_clicked<tab_info[i][3])){
                                    info_hero(ec,i);
                            }
                                }
                }



            }break;

         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            }
        }

          int nbr_heros=0;
            while(nbr_heros!=liste_heros)
            {
               int lign_herojoueur=chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,JOUEUR_HUMAIN,NUM_HERO_HJ,nbr_heros+1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
               if(nbr_heros>2){
                 position.y =y_depart+280;
                 position.x =((x_depart+nbr_heros)-5)*300;
                }else{
                  position.y =(y_depart-20)+40;
                  position.x =x_depart+nbr_heros*300;
                }
                 if(lign_herojoueur==-1){
                    SDL_BlitSurface(phrase[nbr_heros], NULL, ec, &position);
                 }else{
                    SDL_BlitSurface(heros_acheter[nbr_heros], NULL, ec, &position);
                 }
                 nbr_heros++;
            }

            int x_depart_1=40;
            for(i=0;i<liste_heros;i++){
                texte = TTF_RenderText_Blended(font, name[i], blanc);
              if(i>2){
                 position.y =y_depart+260;
                 position.x =x_depart_1+(i-3)*296;
                }else{
                  position.y =(y_depart-20);
                  position.x =x_depart_1+i*296;
                }
                SDL_BlitSurface(texte, NULL, ec, &position);
            }

            for(i=0;i<liste_heros;i++){
                texte = TTF_RenderText_Blended(font, info[i], blanc);
               if(i>2){
                 position.y =y_depart+410;
                 position.x =x_depart_1+40+(i-3)*296;
                }else{
                  position.y =y_depart+150;
                  position.x =x_depart_1+40+i*296;
                }
                SDL_BlitSurface(texte, NULL, ec, &position);
            }

                if(prix_heros1>or){
                texte = TTF_RenderText_Blended(font,afficher_heros1,red);
                }else{
                texte = TTF_RenderText_Blended(font,afficher_heros1,blanc);
                }
                positionText.x =106;
                positionText.y =y_depart+185;
                SDL_BlitSurface(texte, NULL, ec, &positionText);

                if(prix_heros2>or){
                texte = TTF_RenderText_Blended(font,afficher_heros2,red);
                }else{
                texte = TTF_RenderText_Blended(font,afficher_heros2,blanc);
                }
                positionText.x =410;
                positionText.y =y_depart+185;
                SDL_BlitSurface(texte, NULL, ec, &positionText);

                if(prix_heros3>or){
                texte = TTF_RenderText_Blended(font,afficher_heros3,red);
                }else{
                texte = TTF_RenderText_Blended(font,afficher_heros3,blanc);
                }
                positionText.x =700;
                positionText.y =y_depart+185;
                SDL_BlitSurface(texte, NULL, ec, &positionText);

                if(prix_heros4>or){
                texte = TTF_RenderText_Blended(font,afficher_heros4,red);
                }else{
                texte = TTF_RenderText_Blended(font,afficher_heros4,blanc);
                }
                positionText.x =106;
                positionText.y =y_depart+446;
                SDL_BlitSurface(texte, NULL, ec, &positionText);

                if(prix_heros5>or){
                texte = TTF_RenderText_Blended(font,afficher_heros5,red);
                }else{
                texte = TTF_RenderText_Blended(font,afficher_heros5,blanc);
                }
                positionText.x =410;
                positionText.y =y_depart+446;
                SDL_BlitSurface(texte, NULL, ec, &positionText);

                texte_titre = TTF_RenderText_Blended(font,or_humaine,or_color);
                positionText.x =630;
                positionText.y =25;
                SDL_BlitSurface(texte_titre, NULL, ec, &positionText);

                SDL_Flip(ec);
         }

 SDL_FreeSurface(menu);
}

void magasin(SDL_Surface *ec,int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){
    int nombre_phrase=3;
    SDL_Rect position;
    SDL_Surface *menu = NULL,*select=NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font* font = NULL,*font1=NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Magasin",NULL);
    menu = IMG_Load("images/menu1.png");
    select = IMG_Load("images/select.png");
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",27);
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",32);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255 ,0 ,127};
        SDL_Color blanc = {255 ,255 ,255};
        SDL_Color pink = {255 ,0 ,0};
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
                    acheter_hero(ec,joueurs,heros,herosJoueurs);
                }
                else if(indiceOption==1)
                {
                    acheter_unite(ec,joueurs,unites,unitesJoueurs);
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
            char ch[50]="Magasin";
            char* phrase[nombre_phrase];
            phrase[0]= "      Acheter des h�ros         ";
            phrase[1]= "      Acheter des unit�s        ";
            phrase[2]= "      Revenir au menu pr�c�dent ";
            int i, y_depart=260, y_depart_titre=80;
            SDL_Color couleur_a_utiliser;
               for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blanc;

                    position.x =290;
                    position.y=y_depart-10+i*40;
                    SDL_BlitSurface(select, NULL, ec, &position);
                }
                else
                {
                    couleur_a_utiliser=red;

                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
                positionText.x =280;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ec, &positionText);
            }
                texte_titre = TTF_RenderText_Blended(font1,ch,yellow);
                positionText.x =360;
                positionText.y =y_depart_titre;
                SDL_BlitSurface(texte_titre, NULL, ec, &positionText);
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
    int indice_heros=chercherIndiceAvecUneCondition(NUM_HERO_H,1,NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros);
    int nombre_phrase=5;
    SDL_Surface  *menu = NULL,*select=NULL;
    SDL_Rect positionMenu;
    SDL_Rect position;
    SDL_Event event;
    TTF_Font* font = NULL,*font1=NULL;
    SDL_Surface* texte = NULL,*texte_titre=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    SDL_WM_SetCaption("Menu Principal",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
        TTF_Init();
        font = TTF_OpenFont("Aloevera-OVoWO.ttf",27);
        font1 = TTF_OpenFont("Aloevera-OVoWO.ttf",29);
        SDL_Color yellow ={255, 231, 0};
        SDL_Color red = {255 ,0 ,127};
        SDL_Color blanc = {255 ,255 ,255};
        SDL_Color pink = {255 ,0 ,0};
        SDL_Color blue = {0, 109, 255};
        SDL_Rect positionText;
        select= IMG_Load("images/select.png");
        menu = IMG_Load("images/menu1.png");

    while (continuer)
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
                    magasin(ecran,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                    sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                }
                else if(indiceOption==1)
                {
                    campement(ecran,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
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
            char ch[50]="Menu Principal";
            char* phrase[nombre_phrase];
            phrase[0]= "Magasin";
            phrase[1]= "Campement";
            phrase[2]= "Reprendre le jeu en cours";
            phrase[3]= "Nouvelle Partie";
            phrase[4]= "Quitter";

            int i, y_depart=230, y_depart_titre=80;
            SDL_Color couleur_a_utiliser;
            for(i=0; i<nombre_phrase; i++)
            {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=blanc;
                    positionText.x =320;
                    position.x =285;
                    position.y=y_depart-10+i*40;
                    SDL_BlitSurface(select, NULL, ecran, &position);
                }
                else
                {
                    couleur_a_utiliser=red;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                positionText.y =y_depart+i*40;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
            }
                texte_titre = TTF_RenderText_Blended(font1,ch,yellow);
                positionText.x =310;
                positionText.y =y_depart_titre;
                SDL_BlitSurface(texte_titre, NULL, ecran, &positionText);
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

void fin_bataille(ec){
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    positionMenu.y = 0;
    SDL_WM_SetCaption("Heroes magic war",NULL);
    menu = IMG_Load("images/Fin_bataille.png");
    SDL_BlitSurface(menu, NULL, ec, &positionMenu);
    SDL_Flip(ec);
    SDL_Delay(600);
}

void depart(ec){
    //intro(ec);
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    positionMenu.x = 0;
    positionMenu.y = 0;
    SDL_WM_SetCaption("Heroes magic war",NULL);
    menu = IMG_Load("images/background.png");
    SDL_BlitSurface(menu, NULL, ec, &positionMenu);
    SDL_Flip(ec);
    //PlaySound(TEXT("Imagine Dragons x J.I.D - Enemy (from the series Arcane League of Legends).wav"),NULL,SND_ASYNC);
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
    SDL_Surface* texte = NULL,*texte_titre=NULL;
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logoo.png"), NULL);
    ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    depart(ecran);
    SDL_WM_SetCaption("Heroes magic war",NULL);
    int indiceOption=0;
    positionMenu.x = 0;
    positionMenu.y = 0;
    TTF_Init();
    font = TTF_OpenFont("Aloevera-OVoWO.ttf",26);
        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255};
        SDL_Color blue = {0, 109, 255};
        SDL_Color or={255, 179, 0};
        SDL_Color fire={88, 13, 34};
        SDL_Rect positionText;
        menu = IMG_Load("images/backgriund_intro.png");

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
                    continuer = 0;
                }
                else
                {
                    chargerJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
                    menu_princial(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
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
            char ch[50]="Heroes Magic War";
            char* phrase[nombre_phrase];
            phrase[0]= "Nouvelle partie";
            phrase[1]= "Charger partie";
            int i, y_depart=360;
            int y_depart_titre=145;
            SDL_Color couleur_a_utiliser;
          for(i=0; i<nombre_phrase; i++)
          {
                if(i==indiceOption)
                {
                    couleur_a_utiliser=fire;
                }
                else
                {
                    couleur_a_utiliser=blanc;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
                positionText.x =115;
                positionText.y =y_depart+i*95;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);
          }
                texte_titre = TTF_RenderText_Blended(font, ch, or);
                positionText.x =270;
                positionText.y =y_depart_titre+20;
                SDL_BlitSurface(texte_titre, NULL, ecran, &positionText);
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
































