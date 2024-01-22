#include "Drone.h"


int compterLignes(char *chemin) {
    FILE *fichier;
    int nombreLignes = 0;
    char caractere;

    // Ouvrez le fichier en mode lecture
    fichier = fopen(chemin, "r");

    // Vérifiez si le fichier est ouvert avec succès
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return -1; // Code d'erreur
    }

    // Parcourez le fichier caractère par caractère
    while ((caractere = fgetc(fichier)) != EOF) {
        // Si le caractère est un saut de ligne, incrémente le compteur de lignes
        if (caractere == '\n') {
            nombreLignes++;
        }
    }

    // Fermez le fichier
    fclose(fichier);

    //pour avoir les meme obstacles, j'ai choisi cette methode car je l'appellerai une seule fois
    srand((unsigned int)time(NULL));

    return nombreLignes;
}

void LectureFichier(char *texte, Drones *flotte) {

    FILE *fichier = fopen(texte, "r");

    if (fichier == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", texte);
        return;
    }

    char dimension[256];

    // Lire les dimensions
    fgets(dimension, sizeof(dimension), fichier);
    sscanf(dimension, "%d %d", &lignes, &colonnes);
    printf("lignes : %d  colonnes : %d \n", lignes, colonnes);
    

    char ligne[500];

    MAX_DRONES = compterLignes(texte) - 1;
    // Initialiser le tableau de drones
    for (int i = 0; i < MAX_DRONES; ++i) {
        flotte->drone[i].posX = NULL;
        flotte->drone[i].posY = NULL;
        flotte->drone[i].posX_pr = NULL;
        flotte->drone[i].posY_pr = NULL;
        flotte->drone[i].orien = NULL;
        flotte->drone[i].commande = NULL;
    }

    // Lire les données des drones
    int droneIndex = 0;
    while ((droneIndex <= MAX_DRONES)) {
        fgets(ligne, sizeof(ligne), fichier);
        int posX ;
        int posY ; 
        char orientation ;
        char commande[500];
        sscanf(ligne, "%d %d %c %s", &posX, &posY, &orientation, commande);

        // = strtok(ligne + 6, "\n");

        // Allouer de la mémoire pour les membres de la structure Drone_info
        flotte->drone[droneIndex].posX = malloc(sizeof(int));
        flotte->drone[droneIndex].posY = malloc(sizeof(int));
        flotte->drone[droneIndex].posX_pr = malloc(sizeof(int));
        flotte->drone[droneIndex].posY_pr = malloc(sizeof(int));
        flotte->drone[droneIndex].orien = malloc(sizeof(char));
        flotte->drone[droneIndex].commande = malloc(strlen(commande) + 1);
        if (strlen(commande) > MAX_COMMANDES){
            MAX_COMMANDES = strlen(commande);
        }
        // Attribution des valeurs
        *(flotte->drone[droneIndex].posX) = posX;
        *(flotte->drone[droneIndex].posY) = posY;
        *(flotte->drone[droneIndex].orien) = orientation;
        strcpy(flotte->drone[droneIndex].commande, commande);
/*
        // Utilisation des données du drone
        printf("Position X du drone %i : %d\n", droneIndex+1, *(flotte->drone[droneIndex].posX));
        printf("Position Y du drone %i : %d\n", droneIndex+1, *(flotte->drone[droneIndex].posY));
        printf("Orientation du drone %i : %c\n", droneIndex+1, *(flotte->drone[droneIndex].orien));
        printf("Commande du drone %i : %s\n", droneIndex+1, (flotte->drone[droneIndex].commande));
*/
        ++droneIndex;

        // Vérifier si on a atteint la limite MAX_DRONES
        if (droneIndex >= MAX_DRONES) {
            fprintf(stderr, "Nombre maximum de drones atteint (%d)\n", MAX_DRONES);
            break;
        }
    }
    fclose(fichier);

   
    flotte->drone->carte = (int **)malloc(lignes*sizeof(int *));
    for (int i=0; i < lignes;i++){
		flotte->drone->carte[i] = (int *)malloc(colonnes*sizeof(int));
	}
    for (int i=0; i < lignes;i++){
        for (int j=0; j < colonnes; j++){
            flotte->drone->carte[i][j] = 0;
        }
    }
    // Position the drone with its index
    for (int k = 0; k < MAX_DRONES; k++){
        flotte->drone->carte[*(flotte->drone[k].posX)][*(flotte->drone[k].posY)] = k + 1;
           
        }
    
    

}

void Obstacles(Drones *flotte)
{
    
    // Place obstacles with index -1
    int k = 0;
    while (k  < nbObstacle ) {
        int i = rand() % lignes;
        int j = rand() % colonnes;
        if (!(flotte->drone->carte[i][j]>0 && flotte->drone->carte[i][j]<= MAX_DRONES)) {
            flotte->drone->carte[i][j] = id_obs;
            k++;   
        }
    }
}

void clearScreen()
{
    int n;
    for (n = 0; n < 10; n++)
        printf( "\n\n\n" );
}

void affichage(Drones flotte){
    clearScreen();

    for(int i=0;i<colonnes;i++){
        printf("=");
    }
    printf("\n");
    for(int i = 0; i<lignes; i++)
    {
        for(int j = 0; j<colonnes; j++)
        {
            if (flotte.drone->carte[i][j]== id_obs){
                printf("#");
            } else if ((flotte.drone->carte[i][j]>0 && flotte.drone->carte[i][j]<= MAX_DRONES)){
                switch (*(flotte.drone[flotte.drone->carte[i][j]-1].orien)) {
                    case 'N':
                        printf("^");
                        break;
                    case 'S':
                        printf("v");
                        break;
                    case 'W':
                        printf("<");
                        break;
                    case 'E':
                        printf(">");
                        break;
                    default:
                        printf(" ");
                }
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    for(int i=0;i<colonnes;i++){
        printf("=");
    }
    printf("\n");
/*
    for (int i = 0; i<MAX_DRONES; i++){
        printf(" Position de drone %i est x : %d  y : %d  oreintation : %c\n", i+1, *flotte.drone[i].posX, *flotte.drone[i].posY, *flotte.drone[i].orien);
    }
    */
/* 
    for (int i = 0; i<MAX_DRONES; i++){
        printf(" coord de drone %i est x : %d  y : %d  oreintation : %c\n", 
        i+1, *flotte.drone[i].posX, *flotte.drone[i].posY, *flotte.drone[i].orien);
        printf(" coord de drone %i est x_pr : %d  y_pr : %d  \n",
         i+1, *flotte.drone[i].posX_pr, *flotte.drone[i].posY_pr);
    }
*/   
        
}



void deplacer_drone_par_commande(Drones *flotte, int index, int cmnd)
{
        *flotte->drone[index].posX_pr = *flotte->drone[index].posX;
        *flotte->drone[index].posY_pr = *flotte->drone[index].posY;
        //*flotte->drone[index].orien_pr = *flotte->drone[index].orien;
     switch (flotte->drone[index].commande[cmnd]) {
        case 'L':
            switch (*flotte->drone[index].orien)
                {
                case 'N':
                    *flotte->drone[index].orien = 'W';
                    break;
                case 'W':
                    *flotte->drone[index].orien = 'S';
                    break;
                case 'S':
                    *flotte->drone[index].orien = 'E';
                    break;
                case 'E':
                    *flotte->drone[index].orien = 'N';
                    break;
                default:
                    break;
                }
            break;
        case 'R':
           switch (*flotte->drone[index].orien)
                {
                case 'N':
                    *flotte->drone[index].orien = 'E';
                    break;
                case 'W':
                    *flotte->drone[index].orien = 'N';
                    break;
                case 'S':
                    *flotte->drone[index].orien = 'W';
                    break;
                case 'E':
                    *flotte->drone[index].orien = 'S';
                    break;
                default:
                    break;
                }
            break; 
        case 'M':
            switch (*flotte->drone[index].orien)
                    {
                    case 'N':
                        if ((*(flotte->drone[index].posX) - 1 >= 0 )
                            && (flotte->drone->carte[*(flotte->drone[index].posX) - 1][*(flotte->drone[index].posY)] == 0)){
                                (*(flotte->drone[index].posX))--;
                        }else if (*(flotte->drone[index].posX) - 1 < 0){
                                *flotte->drone[index].orien = 'S';
                                deplacer_drone_par_commande(flotte, index,cmnd); //pour re-appliquer la commande en sens inverse
                            }
                        break;
                    case 'W':
                        if ((*(flotte->drone[index].posY) - 1 >= 0 ) 
                            && (flotte->drone->carte[*(flotte->drone[index].posX)][*(flotte->drone[index].posY) - 1] == 0)) {
                                (*(flotte->drone[index].posY))--;
                        }else if (*(flotte->drone[index].posY) - 1 < 0 ){
                                *flotte->drone[index].orien = 'E';
                                deplacer_drone_par_commande(flotte, index,cmnd);
                        }
                        break;
                    case 'S':
                        if ((*(flotte->drone[index].posX) + 1 < lignes )
                            && (flotte->drone->carte[*(flotte->drone[index].posX) + 1][*(flotte->drone[index].posY)] == 0)) {
                                (*(flotte->drone[index].posX))++;
                        }else if (*(flotte->drone[index].posX) + 1 >= lignes){
                                *flotte->drone[index].orien = 'N';
                                deplacer_drone_par_commande(flotte, index,cmnd);
                        }
                        break;
                    case 'E':
                        if ((*(flotte->drone[index].posY) + 1 < colonnes) 
                            && (flotte->drone->carte[*(flotte->drone[index].posX)][*(flotte->drone[index].posY) + 1] == 0)) {
                                (*(flotte->drone[index].posY))++;
                        }else if (*(flotte->drone[index].posY) + 1 >= colonnes){
                                *flotte->drone[index].orien = 'W';
                                deplacer_drone_par_commande(flotte, index,cmnd);
                        }
                        break;
                    default:
                        break;
                    }
            break;
        case 'B':
            switch (*flotte->drone[index].orien)
                    {
                    case 'N':
                        if ((*(flotte->drone[index].posX) + 1 < lignes )
                            && (flotte->drone->carte[*(flotte->drone[index].posX) + 1][*(flotte->drone[index].posY)] == 0)){
                                (*(flotte->drone[index].posX))++;
                        }else if (*(flotte->drone[index].posX) + 1 >= lignes ){
                                *flotte->drone[index].orien = 'S';
                                deplacer_drone_par_commande(flotte, index,cmnd);
                        }
                        break;
                    case 'W':
                        if ((*(flotte->drone[index].posY) + 1 < colonnes) 
                            && (flotte->drone->carte[*(flotte->drone[index].posX)][*(flotte->drone[index].posY) + 1] == 0)) {
                                (*(flotte->drone[index].posY))++;
                        } else if(*(flotte->drone[index].posY) + 1 >= colonnes){
                                *flotte->drone[index].orien = 'E';
                                deplacer_drone_par_commande(flotte, index,cmnd);
                        }
                        break;
                    case 'S':
                        if ((*(flotte->drone[index].posX) - 1 >= 0 )
                            && (flotte->drone->carte[*(flotte->drone[index].posX) - 1][*(flotte->drone[index].posY)] == 0)) {
                                (*(flotte->drone[index].posX))--;
                        }else if(*(flotte->drone[index].posX) - 1 < 0 ){
                                *flotte->drone[index].orien = 'N';
                                deplacer_drone_par_commande(flotte, index,cmnd);
                        }
                        break;
                    case 'E':
                        if((*(flotte->drone[index].posY) - 1 >= 0 ) 
                            && (flotte->drone->carte[*(flotte->drone[index].posX)][*(flotte->drone[index].posY) - 1] == 0)) {
                                (*(flotte->drone[index].posY))--;
                        }else if (*(flotte->drone[index].posY) - 1 < 0 ){
                                *flotte->drone[index].orien = 'W';
                                deplacer_drone_par_commande(flotte, index,cmnd); 
                        }
                        break;
                    default:
                        break;
                    }
            break;                
        default:
            //fprintf(stderr, "Commande inconnue pour le drone %d : %c\n", index + 1, *flotte->drone[index].commande[cmnd]);
            break;
    }

    flotte->drone->carte[*(flotte->drone[index].posX_pr)][*(flotte->drone[index].posY_pr)] = 0;
    flotte->drone->carte[*(flotte->drone[index].posX)][*(flotte->drone[index].posY)] = index + 1;
   // printf("coord de %i drone act x : %i y : %i \n", index+1, *flotte->drone[index].posX, *flotte->drone[index].posY);
    //printf("orientation act : %c\n", *flotte->drone[index].orien);
}


void deplacer0(Drones *flotte)
{
    int k = 0;
    while (k<MAX_DRONES)
    {
        for (int i = 0; flotte->drone[k].commande[i] != '\0'; i++) {
            //pthread_mutex_lock(&dmutex);

            deplacer_drone_par_commande(flotte, k, i);

            flotte->drone->carte[*(flotte->drone[k].posX_pr)][*(flotte->drone[k].posY_pr)] = 0;
            flotte->drone->carte[*(flotte->drone[k].posX)][*(flotte->drone[k].posY)] = k + 1;
            
            affichage(*flotte);

            //pthread_mutex_unlock(&dmutex);
            sleep(1);
        }
        
        k++;

    }
    
    
}

void *deplacer(void *arg)
{
    Drones * flotte = (Drones *)arg;
    
    while(1){
        for (int i = 0; i<MAX_COMMANDES; i++)
        {
            pthread_mutex_lock(&dmutex);
            for(int k=0;k<MAX_DRONES; k++){

                deplacer_drone_par_commande(flotte, k, i);

            }  
            pthread_cond_signal(&cond);
            pthread_cond_wait(&cond, &dmutex);
            pthread_mutex_unlock(&dmutex);
   
        }
        break;
 
    }
    pthread_exit(NULL);
    
}

void *displayThread(void *arg) {
    Drones * flotte = (Drones *)arg;
    while (1)
    {
        pthread_mutex_lock(&dmutex);
        for (int i = 0; i < MAX_DRONES; i++) {

            affichage(*flotte);
            //printf(" coord de drone %i est x : %d  y : %d  oreintation : %c\n", i+1, *flotte->drone[i].posX, *flotte->drone[i].posY, *flotte->drone[i].orien);
        }
        printf("\n");
     
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond, &dmutex);
        pthread_mutex_unlock(&dmutex);

        
        sleep(1);
    }
    
    pthread_exit(NULL);
}

void cleanup(Drones *flotte) {
    for (int i = 0; i < MAX_DRONES; ++i) {
        
        free(flotte->drone[i].posX);
        free(flotte->drone[i].posY);
        free(flotte->drone[i].orien);
        free(flotte->drone[i].commande);
    }
}


/*
int main() {
    Drones flotte;
    char *fichier = "commandDrone.txt";

    pthread_t threadDeplacement, threadAffichage;

    srand((unsigned int)time(NULL));

    int MAX_DRONES = compterLignes(fichier) - 1;
    LectureFichier(fichier, &flotte);

    //affichage(flotte);
    pthread_create(&threadAffichage, NULL, displayThread, &flotte);
    pthread_create(&threadDeplacement, NULL, deplacer, &flotte);
   
    pthread_join(threadAffichage, NULL);
    pthread_join(threadDeplacement, NULL);
    

    cleanup(&flotte);
    for (int i = 0; i < lignes; ++i) {
        free(flotte.drone->carte[i]);
    }
    free(flotte.drone->carte);

    return 0;
}
*/
