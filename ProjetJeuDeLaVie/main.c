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
#define WAITYESNO 1
#define WAITTIME 0.5
#define MUT_APO_YESNO 1
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
            else tabtemp[j*COLONNES + i] = CELLDEAD;
        }
    }
    for(j=0;j<LIGNES;j++){
        for(i=0;i<COLONNES;i++){
            tab[j*COLONNES + i] = tabtemp[j*COLONNES + i];
        }
    }
    free(tabtemp);
}

//Transforme les cellules en CELLALIVE aléatoirement à un taux tauxmutation/100, en CELLDEAD aléatoirement à un taux tauxapoptose/100. L'apoptose l'emporte sur la mutation.
void mutApo(char *tab, int tauxMutation, int tauxApoptose){
    int i, j;
    for(j=0;j<LIGNES;j++){
        for(i=0;i<COLONNES;i++){
            if(rand()%100+1 <= tauxMutation) tab[j * COLONNES + i] = CELLALIVE;
            if(rand()%100+1 <= tauxApoptose) tab[j * COLONNES + i] = CELLDEAD;
        }
    }
}

//Fonction pour attendre un nombre de secondes données:
void myWait(void){
    time_t start, end;
    time(&start);
    do time(&end); while(difftime(end, start) <= WAITTIME);
}

void showEvolution(char *tab){
    int nbGen, i, apoptose = 0, mutation = 0, attente = 0;
    do{
        printf("Entrez le nombre de générations souhaitées: ");
        scanf("%d", &nbGen);
        if(nbGen < 0) printf("La possibilité de retour aux générations précédentes n'est pas encore disponible dans cette version.\n");
    }while(nbGen<0);
    if(MUT_APO_YESNO == 1){
        do{
            printf("Entrez le taux de mutation M souhaité (chaque cellule a M chance sur 100 d'être en vie à la prochaine génération, indépendemment des règles de base): ");
            scanf("%d", &mutation);
            if(mutation < 0 || mutation >100) printf("Impossible. Entrez une valeur entre 0 et 100\n");
        }while(mutation<0 || mutation >100);
        do{
            printf("Entrez le taux d'apoptose A souhaité (chaque cellule a A chance sur 100 d'être morte à la prochaine génération, indépendemment des règles de base et des mutations): ");
            scanf("%d", &apoptose);
            if(apoptose < 0 || apoptose >100) printf("Impossible. Entrez une valeur entre 0 et 100\n");
        }while(apoptose<0 || apoptose >100);
    }
    if(WAITYESNO == 1){
        do{
            printf("Entrez 0 si vous ne souhaitez pas de temps d'attente entre les affichages des générations, 1 si vous en souhaitez un: ");
            scanf("%d", &attente);
            if(attente < 0 || attente > 1) printf("Impossible. Entrez une valeur entre 0 et 1\n");
        }while(attente < 0 || attente > 1);
    }
    printf("-----Gen 0-----\n");
    showTab(tab);
    for(i=0;i<nbGen;i++){
        if(attente == 1) myWait();
        printf("-----Gen %d-----\n", i+1);
        tabGenPlusOne(tab);
        if(mutation > 0 || apoptose > 0) mutApo(tab, mutation, apoptose);
        showTab(tab);
    }
}

void userInput(char *tab){
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
    showEvolution(tab);
}

void randomInput(char *tab){
    int i, j;
    for(j=0;j<LIGNES;j++){
        for(i=0;i<COLONNES;i++){
            if(rand()%2+1 == 1) tab[j * COLONNES + i] = CELLDEAD;
            else tab[j * COLONNES + i] = CELLALIVE;
        }
    }
    showEvolution(tab);
}

void glider(char *tab){
    tab[0*COLONNES +1] = '@';
    tab[1*COLONNES +2] = '@';
    tab[2*COLONNES +0] = '@';
    tab[2*COLONNES +1] = '@';
    tab[2*COLONNES +2] = '@';
    showEvolution(tab);
}

void menu(char *tab){
    int a;
    do{
        printf("Entrez 1 pour un arangement de cellules aléatoire, 2 pour entrer vos propres coordonnées de cellules, 3 pour le glider: ");
        scanf("%d", &a);
    }while(a<1 || a>3);
    if(a==1) randomInput(tab);
    if(a==2) userInput(tab);
    if(a==3) glider(tab);
}

int main(){
    char *tab;
    srand((int)time(NULL));
    tab = (char*)malloc(sizeof(char)*COLONNES*LIGNES); //Possible de passer cette ligne dans la fonction iniTab?
    iniTab(tab);
    //Ne pas toucher au dessus
    
    menu(tab);
    
    free(tab);
    return 0;
}
