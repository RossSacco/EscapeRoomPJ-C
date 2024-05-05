#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include "funzioniUtilita.h"

#define DIMBF 1024



//giocatore principale

int main(int argc, char* argv[]){

    int ret, sd, porta;
    char buffer[DIMBF];
    int scenario = 0; 
    char user[20], pasw[20];
    char cmd[10];
    struct sockaddr_in srv_addr;
    bool login_eff = false;
    int x, i;

    if(argc>1){
        porta = atoi(argv[1]);
    }

  

    sd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(4242);
    inet_pton(AF_INET, "127.0.0.1", &srv_addr.sin_addr);

    
    for(i = 0; i < 5; i++){
        ret = connect(sd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
        if(ret > 0){
            break;
        }
        sleep(1);
    }
    


    //invio "client" per fagli capire chi sono al server
    strcpy(buffer, "client");
    ret = send_auto(sd, buffer);
    if(!ret){
        exit(-1);
    }

    printf("Connessione con il server stabilita! %d\n", porta);

    //devo fare il login, variabile definita in funzioniUtilita
    do{
        printf("Inserisci username e password per loggarti ed iniziare a giocare!\n");
        printf("Username: ");
        if (scanf("%19s", user) != 1) {
            perror("errore nell'inserimento dell'user\n");
            close(sd);
            exit(-1);
        }
        printf("Password: ");
        if (scanf("%19s", pasw) != 1) {
            perror("errore inserimento password\n");
            close(sd);
            exit(-1);
        }

        login_utente(sd, user, pasw);
        
        ret = recv_auto(sd, buffer);
        if(!ret){
            perror("Error");
            exit(-1); //se il server si è disconnesso mentre facevo il login poi mi devo chiudere
        }
        
        if(strcmp(buffer, "ok") == 0){
            login_eff = true;
        }
        
    }while(login_eff == false);

    printf("\nCiao %s!\n\n", user);

    //pulisco stdin
    while ((x = getchar()) != '\n' && x != EOF);

    while(1){
        mostra_scenari_inizio();        

        if(fgets(buffer, sizeof(buffer), stdin) != NULL){
            buffer[strcspn(buffer, "\n")] = '\0';

            sscanf(buffer, "%5s %d", cmd, &scenario);
            if(!strcmp(cmd, "start") && scenario == 1){
                break;
            }else{
                printf("Comando non trovato, riprova\n");
            }
        }
    }

    printf("\nHai selezionato la stanza %d\nIl gioco sta per iniziare...\n\n", scenario);

    if(scenario == 1){
        presentazione_scenario();
        gestione_partita_client(sd);
    }

    //arrivo qui solo a fine partita
    close(sd);
    exit(0);
    
}