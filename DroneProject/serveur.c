/*
To run this code : 
gcc serveur.c -o serveur
./serveur
*/

#include "Drone.h"


int main() {
    Drones flotte;
    char *fichier = "commandDrone.txt";
    int version;
    printf("Tapez la version (1, 2 ou 3) : ");
    scanf("%d", &version);
    if(version == 1 ){
/////////// version 1    
   
    char *fichier0 = "infoDrone.txt";

    srand((unsigned int)time(NULL));

    int MAX_DRONES = compterLignes(fichier0) - 1;
    LectureFichier(fichier0, &flotte);
    Obstacles(&flotte);
    for (int i = 0; i<MAX_DRONES; i++){
        printf(" Position de drone %i est x : %d  y : %d  oreintation : %c  commande : %s\n", 
        i+1, *flotte.drone[i].posX, *flotte.drone[i].posY, *flotte.drone[i].orien, (flotte.drone[i].commande));
    }

    deplacer0(&flotte);
    
    for (int i = 0; i<MAX_DRONES; i++){
        printf(" Position finale de drone %i est x : %d  y : %d  oreintation : %c\n", 
        i+1, *flotte.drone[i].posX, *flotte.drone[i].posY, *flotte.drone[i].orien);
    }
/////////// version 1 
    }else if(version == 2){
/////////// version 2
    
    char *fichier0 = "infoDrone.txt";

    pthread_t threadDeplacement, threadAffichage;

    srand((unsigned int)time(NULL));

    int MAX_DRONES = compterLignes(fichier0) - 1;
    LectureFichier(fichier0, &flotte);
    Obstacles(&flotte);
    //affichage(flotte);
    pthread_create(&threadAffichage, NULL, displayThread, &flotte);
    pthread_create(&threadDeplacement, NULL, deplacer, &flotte);
   
    pthread_join(threadAffichage, NULL);
    pthread_join(threadDeplacement, NULL);
/////////// version 2
    }else{
/////////// version 3
/////////////////////////
    int s = 0, taille, bd, error_number, lg = 40;
    char msg[BUFFER_SIZE];
    char msg1[40] = "message bien recu ";
    
    
    char msg4[BUFFER_SIZE];
    struct sockaddr_in *padin; //pointeur adresse internet locale
    struct sockaddr_in *p_exp; //pointeur adresse internet expediteur (recuperée de l'entete paquet UDP recu)
    
    s = socket(AF_INET, SOCK_DGRAM, AF_UNSPEC);
    if (s == -1) {
        perror("erreur creation socket");
        exit(EXIT_FAILURE);
    }
    printf("le socket est identifie par : %d \n", s);

    taille = sizeof(struct sockaddr_in);
    padin = (struct sockaddr_in *)(malloc(taille));
    bzero((char *)padin, taille);

    padin->sin_family = AF_INET;
    padin->sin_port = htons(LEPORT);

    bd = bind(s, (struct sockaddr *)padin, taille);
    if (bd == -1) {
        perror("Erreur d'attachement");
        exit(EXIT_FAILURE);
    }

    p_exp = (struct sockaddr_in *)(malloc(sizeof(struct sockaddr_in)));
    socklen_t p_lgexp = sizeof(struct sockaddr_in);
//////////////////////////////
    
    MAX_DRONES = compterLignes(fichier) - 1;
    pthread_t threadDeplacement, threadAffichage;
    srand((unsigned int)time(NULL));

    while (1) {
        ////received
        bd = recvfrom(s, msg, BUFFER_SIZE, 0, (struct sockaddr *)p_exp, &p_lgexp);
        if (bd == -1) {
            perror("Erreur receive");
            exit(EXIT_FAILURE);
        }
        msg[bd] = '\0';
        //printf("Client : '%s'\n", msg);
        FILE *file = fopen("commandDrone.txt", "wb");
        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            continue;  // Continue to the next iteration of the loop
        }
        fwrite(msg, 1, strlen(msg), file);
        fclose(file);
        ////affichage de la carte 
        

        
        LectureFichier(fichier, &flotte);

        int k = 0;
        while (k  < nbObstacle ) {
            int i = rand() % lignes;
            int j = rand() % colonnes;
            if (!(flotte.drone->carte[i][j]>0 && flotte.drone->carte[i][j]<= MAX_DRONES)) {
                flotte.drone->carte[i][j] = id_obs;
                k++;   
            }
        }

        pthread_create(&threadAffichage, NULL, displayThread, &flotte);
        //pthread_create(&threadDeplacement, NULL, deplacer, &flotte);
            
            while(1){
               
                for (int i = 0; i<MAX_COMMANDES; i++)
                {   
                    
                    pthread_mutex_lock(&dmutex);

                    char Positions[BUFFER_SIZE];
                    sprintf(Positions, "%d %d\n", lignes, colonnes);
                    
                    for (int k = 0; k < MAX_DRONES; k++) {
                        char droneMSG[100];
                        //strcpy(msg3, ""); // Clear msg3

                        // Assuming deplacer_drone_par_commande updates drone positions
                        deplacer_drone_par_commande(&flotte, k, i);

                        // Append drone information to msg3
                        sprintf(droneMSG, "%d %d %c\n", 
                          *flotte.drone[k].posX, *flotte.drone[k].posY, *flotte.drone[k].orien);
                        //printf("drone %i msg : '%s'\n", k+1, droneMSG);
                        strcat(Positions, droneMSG);   
                                         
                    }

                    // Send positions
                    ssize_t bd = sendto(s, Positions, BUFFER_SIZE, 0, (struct sockaddr*)p_exp, sizeof(*p_exp));
                    if (bd == -1) {
                        perror("Erreur send");
                        exit(EXIT_FAILURE);
                    }

                    //printf("Message envoyé : '%s'\n", Positions);
                    
                    pthread_cond_signal(&cond);
                    pthread_cond_wait(&cond, &dmutex);
                    pthread_mutex_unlock(&dmutex);
        
                }
                for (int i = 0; i<MAX_DRONES; i++){
                    printf(" Position finale de drone %i est x : %d  y : %d  oreintation : %c\n", i+1, *flotte.drone[i].posX, *flotte.drone[i].posY, *flotte.drone[i].orien);
                }
                break;
 
            }
            pthread_exit(NULL);
        
        pthread_join(threadAffichage, NULL);
        //pthread_join(threadDeplacement, NULL);
        
    }
    close(s);
        // Free allocated memory
    free(padin);
    free(p_exp);
    cleanup(&flotte);
    for (int i = 0; i < lignes; ++i) {
        free(flotte.drone->carte[i]);
    }
    free(flotte.drone->carte);
    exit(EXIT_FAILURE);
/////////// version 3
    }

 
    
    
    return 0;
}