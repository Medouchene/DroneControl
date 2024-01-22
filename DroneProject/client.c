/*
After running serveur, to run this code : 
gcc client.c -o client
./client

tape le nom du fichier : infoDrone
*/

#include "Drone.h"


int main() {
/////////////////////
    int s = 0, bd;
    struct sockaddr_in padin;
    socklen_t p_lgexp = sizeof(struct sockaddr_in);
    char *name = "localhost";
    char *adr_serv = "127.0.0.1";
    char msg[40];
    char msg1[BUFFER_SIZE];
    struct hostent *host;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1) {
        perror("Erreur création socket");
        exit(EXIT_FAILURE);
    }

    bzero((char *)&padin, sizeof(struct sockaddr_in));

    if ((host = gethostbyname(name)) == NULL) {
        perror("Nom de machine");
        exit(EXIT_FAILURE);
    }
    bcopy(host->h_addr_list[0], &padin.sin_addr, host->h_length);

    padin.sin_family = AF_INET;
    padin.sin_port = htons(LEPORT);
////////////////////////////////
    Drones flotte;
    int droneIndex;
    int posX ;
    int posY ; 
    char orientation ;
    char *fichier = "file.txt";
    
    //mettre les obs
    int compteur = 0;
    int obs[nbObstacle][2];
    srand((unsigned int)time(NULL));
////send
        printf("Quel est votre fichier ?  ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = '\0';

        //printf("Votre message est : '%s'\n", msg);

        char filename[50];
        sprintf(filename, "%s.txt", msg);
        FILE *file = fopen(filename, "rb");
        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            exit(EXIT_FAILURE);  // Continue to the next iteration of the loop
        }
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);
        char *file_content = (char *)malloc(file_size + 1);
        if (file_content == NULL) {
            perror("Erreur allocation mémoire");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        fread(file_content, 1, file_size, file);
        file_content[file_size] = '\0';  // Null-terminate the string
        //printf("contenu '%s'\n", file_content);
        fclose(file);

        bd = sendto(s, file_content, file_size, 0, (struct sockaddr *)&padin, sizeof(padin));
        if (bd == -1) {
            perror("Erreur send");
            free(file_content);
            exit(EXIT_FAILURE);
        }
        
    while (1) {

        
        ////received
        bd = recvfrom(s, msg1, sizeof(msg1), 0, (struct sockaddr *)&padin, &p_lgexp);
        if (bd == -1) {
            perror("Erreur receive");
            exit(EXIT_FAILURE);
        }
        msg1[bd] = '\0';
        //printf("Serveur : '%s'\n", msg1);

        FILE *file = fopen("file.txt", "wb");
        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            continue;  // Continue to the next iteration of the loop
        }
        fwrite(msg1, 1, strlen(msg1), file);
        fclose(file);
        
        ////Affichage de la carte      
        MAX_DRONES = compterLignes(fichier)-1;  
        LectureFichier(fichier, &flotte);
        
        int k = 0;
        while (k  < nbObstacle ) {
            if (compteur < 1){
                obs[k][0] = rand() % lignes;
                obs[k][1] = rand() % colonnes;
            }
            
            if (!(flotte.drone->carte[obs[k][0]][obs[k][1]]>0 && flotte.drone->carte[obs[k][0]][obs[k][1]]<= MAX_DRONES)) {
                flotte.drone->carte[obs[k][0]][obs[k][1]] = id_obs;
                k++; 
  
            }
        }
        compteur++;
        
        affichage(flotte);
        sleep(1);
        
           
        
    }
    
    cleanup(&flotte);
    for (int i = 0; i < lignes; ++i) {
        free(flotte.drone->carte[i]);
    }
    free(flotte.drone->carte);
    close(s);
    free(file_content);
   
    return 0;
}
