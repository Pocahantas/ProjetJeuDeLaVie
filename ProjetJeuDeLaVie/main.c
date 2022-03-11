/*#include <stdio.h>
#include <stdlib.h>

affichage2(int *tab,int lignes,int colonnes){
int i,j,*z;
 for(i=0;i<lignes;i++){
   z=tab+i*colonnes;
   printf("\n");
      for(j=0;j<colonnes;j++){
          printf("%d ",*(z+j));
       }
  }
}

void analyse(int *tab, int ligne, int col, int tailleC){

    printf("\n Test = %d", *(tab+ligne*tailleC+col));



}


int main()
{
int lignes = 7;
int colonnes = 5;

int *tab;
int i,j;
tab = (int*)malloc(lignes*colonnes*sizeof(int));
    for(i=0; i<lignes; i++){
        for(j=0;j<colonnes;j++){
        *(tab+i*colonnes + j) = 100*j+ i;
        }
    }

affichage2(tab, 7,5);
analyse(tab, 3,3, 5);

    return EXIT_SUCCESS;
}*/




//TODO
//Apprendre interface graphique C pour meilleur affichage? -> Apprendre SDL? OpenGL?
//Possibilité d'importer un fichier texte avec une disposition déjà faite pour la voir évoluer?

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define COLONNES 100
#define LIGNES 20
#define CELLALIVE '@'
#define CELLDEAD ' '
#define RECENTDEAD '*'
#define WAITYESNO 0
#define WAITTIME 0.5
//Utilisation dans le programme d'un tableau 1D de telle sorte que les valeurs soient ordonnées de la sorte sur la même ligne à la suite: (x1y1), (x2y1), ... , (x1y2), (x2y2) ...

//Initialise le tableau avec des espaces:
void iniTab(char *tab){
    int i, j;
    for(j=0;j<LIGNES;j++){
        for(i=0;i<COLONNES;i++){
            tab[j * COLONNES + i] = CELLDEAD;
        }
    }
}

//Affiche le tableau:
void showTab(char *tab){
    int i, j;
    for(j=0;j<LIGNES;j++){
        for(i=0;i<COLONNES;i++){
            printf("%c", tab[j * COLONNES + i]);
        }
        printf("\n");
    }
}

//Compte le nombre de voisins, l'espace n'est pas plat mais celui d'un tore:
int nombreVoisins(char *tab, int x, int y){
    int cpt=0;
    if(tab[y * COLONNES + (x + 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin de droite
    if(tab[y * COLONNES + (x - 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin de gauche
    if(tab[((y + 1 + LIGNES)%LIGNES) * COLONNES + x] == CELLALIVE) cpt++; //voisin du bas
    if(tab[((y - 1 + LIGNES)%LIGNES) * COLONNES + x] == CELLALIVE) cpt++; //voisin du haut
    if(tab[((y + 1 + LIGNES)%LIGNES) * COLONNES + (x + 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin bas droite
    if(tab[((y - 1 + LIGNES)%LIGNES) * COLONNES + (x + 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin haut droite
    if(tab[((y + 1 + LIGNES)%LIGNES) * COLONNES + (x - 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin bas gauche
    if(tab[((y - 1 + LIGNES)%LIGNES) * COLONNES + (x - 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin haut gauche
    return cpt;
}


//Transforme le tableau en tableau génération + 1 (utilisation de tableau temporaire pour d'abord calculer toutes les cellules):
void tabGenPlusOne(char *tab){
    char *tabtemp;
    int i, j, n;
    tabtemp = (char*)malloc(sizeof(char)*COLONNES*LIGNES);
    for(j=0;j<LIGNES;j++){
        for(i=0;i<COLONNES;i++){
            n = nombreVoisins(tab, i, j);
            if(n==3) tabtemp[j*COLONNES + i] = CELLALIVE;
            else if(n==2) tabtemp[j*COLONNES + i] = tab[j*COLONNES + i]; //nécessaire de checker si la cellule reste identique ou pas? a voir plus tard
            else {
                if (tab[j*COLONNES + i] == CELLALIVE){ // Permet d'afficher les cellules récemment mortes
                    tabtemp[j*COLONNES + i]= RECENTDEAD;
                    }
                 else tabtemp[j*COLONNES + i] = CELLDEAD;
                }
        }
    }
    for(j=0;j<LIGNES;j++){
        for(i=0;i<COLONNES;i++){
            tab[j*COLONNES + i] = tabtemp[j*COLONNES + i];
        }
    }
    free(tabtemp);
}
//Nouvelles règles
void tabGenPlusTwo(char *tab){
    char *tabtemp;
    int i, j, n;
    tabtemp = (char*)malloc(sizeof(char)*COLONNES*LIGNES);
    for(j=0;j<LIGNES;j++){
        for(i=0;i<COLONNES;i++){
            n = nombreVoisins(tab, i, j);
            if(n>7) tabtemp[j*COLONNES + i] = CELLDEAD;
            else if(n==2) tabtemp[j*COLONNES + i] = CELLALIVE; //nécessaire de checker si la cellule reste identique ou pas? a voir plus tard
            else {
                if (tab[j*COLONNES + i] == CELLALIVE){
                    tabtemp[j*COLONNES + i]= RECENTDEAD;
                    }
                 else tabtemp[j*COLONNES + i] = CELLDEAD;
                }
        }
    }
    for(j=0;j<LIGNES;j++){
        for(i=0;i<COLONNES;i++){
            tab[j*COLONNES + i] = tabtemp[j*COLONNES + i];
        }
    }
    free(tabtemp);
}


//Fonction pour attendre un nombre de secondes données:
void myWait(void){
    time_t start, end;
    time(&start);
    do time(&end); while(difftime(end, start) <= WAITTIME);
}

void showEvolution(char *tab, int ruleset){
    int nbGen, i;
    do{
        printf("Entrez le nombre de générations souhaitées: ");
        scanf("%d", &nbGen);
        if(nbGen < 0) printf("La possibilité de retour aux générations précédentes n'est pas encore disponible dans cette version.\n"); //de toute façon c'est pas possible de retoruner en arrière mathématiquement
    }while(nbGen<0);
    printf("-----Gen 0-----\n");
    showTab(tab);
    for(i=0;i<nbGen;i++){
        if(WAITYESNO == 1) myWait();
        printf("-----Gen %d-----\n", i+1);
        if (ruleset == 0) tabGenPlusOne(tab);
        else tabGenPlusTwo(tab);
        showTab(tab);
    }
}

void userInput(char *tab, int ruleset){
    int x=-2, y=-2;
    printf("Entrez les coordonnées des cellules vivantes souhaitées. Entrez -1 pour l'axe des x lorsque vous avez terminé. Les coordonées valides vont de 0 à %d pour l'axe des x, de 0 à %d pour l'axe des y.\n", COLONNES-1, LIGNES-1);
    //L'utilisation du while est bancale, plus simple et rapide avec un do-while
    while(x!=-1){
        y = -2;
        printf("Entrez la coordonnée de l'axe x: ");
        scanf("%d", &x);
        if(x<-1 || x>COLONNES-1) printf("Erreur, veuillez entrer une valeur valide entre 0 et %d\n", COLONNES-1);
        else if(x == -1) printf("Saisie des cellules terminée.\n");
        else{
            while(y<0 || y>LIGNES-1){
                printf("Entrez la coordonée de l'axe y: ");
                scanf("%d", &y);
                if(y<-1 || y>LIGNES-1) printf("Erreur, veuillez entrer une valeur valide entre 0 et %d\n", LIGNES-1);
                if(y==-1) printf("Bien tenté mais je ne vous laisserai pas vous arrêter en si bon chemin, vous sembliez si enthousiaste à ajouter une nouvelle cellule.\n");
            }
        }
        if(x>=0 && x<COLONNES && y>= 0 && y<LIGNES){
            tab[y*COLONNES + x] = CELLALIVE;
            printf("Cellule ajoutée.\n");
        }
    }
    showEvolution(tab, ruleset);
}

void randomInput(char *tab, int ruleset){
    int i, j;
    for(j=0;j<LIGNES;j++){
        for(i=0;i<COLONNES;i++){
            if(rand()%2+1 == 1) tab[j * COLONNES + i] = CELLDEAD;
            else tab[j * COLONNES + i] = CELLALIVE;
        }
    }
    showEvolution(tab, ruleset);
}

void glider(char *tab, int ruleset){
    tab[0*COLONNES +1] = '@';
    tab[1*COLONNES +2] = '@';
    tab[2*COLONNES +0] = '@';
    tab[2*COLONNES +1] = '@';
    tab[2*COLONNES +2] = '@';
    showEvolution(tab, ruleset);
}

void menu(char *tab){
    int a, ruleset;
    do{
        printf("Entrez 1 pour un arangement de cellules aléatoire, 2 pour entrer vos propres coordonnées de cellules, 3 pour le glider: ");
        scanf("%d", &a);
        printf("Quelle règles de jeu désirez-vous ? 0 pour Conway (Classique) ou 1 pour Overpopulation \n Attention ! Le glider ne marche que en classique ! ");
        scanf("%d", &ruleset);
    }while(a<1 || a>3);
    if(a==1) randomInput(tab, ruleset);
    if(a==2) userInput(tab, ruleset);
    if(a==3) glider(tab, ruleset);
}

int main(){
    char *tab;
    srand((int)time(NULL));
    tab = (char*)malloc(sizeof(char)*COLONNES*LIGNES);
    iniTab(tab);
    //Ne pas toucher au dessus

    menu(tab);

    free(tab);
    return 0;
}
