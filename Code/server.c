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
#include <stdbool.h>
#include <signal.h>
#include "strutture.h"
#include "funzioniUtilita.h"

#define MAX_HOST 2

#define DIMBUFF 1024
#define DIMCMD 10



int server_avviato = 0;

void mostra_comandi_serverStart();
void input_server();



    int ret;
    int porta;
    //uso I/O multiplexing
    fd_set master, read_fds;
    int fdmax;

    struct sockaddr_in srv_addr, clt_addr;
    int listener;
    int newfd;

int main(int argc, char* argv[]){

    int i, len, new_sd, j;
    char cmd[10];
    char usr[20], psw[20];
    char buf[DIMBUFF];
    char enigma[DIMBUFF];
    char risposta[DIMBUFF];
    char ogg[DIMBUFF];
    struct partita *p;
    bool trovato = false; //per funzione speciale
    
    

     if(argc > 1){
         porta = atoi(argv[1]);
     }else{
         porta = 4242;
     }

    listener = socket(AF_INET, SOCK_STREAM, 0);
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(porta);
    srv_addr.sin_addr.s_addr = INADDR_ANY; //mette il server in ascolto su tutte le interfacce

    ret = bind(listener, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if(ret < 0){
        perror("errore nella bind del server");
        exit(-1);
    }

    //prima di mettere in ascolto aspetto l'avvio
    mostra_comandi_serverStart();


    while (server_avviato == 0)
    {
        //devo controllare l'input 
        input_server();
    }

    

    if(server_avviato == 1)
    {
        inizializzazione_oggetti();
        inizializzazione_enigmi();
        inizializzazione_location();

        int giocatore_principale = -1;
        int giocatore_speciale = -1;
        int cmd1;
        int cmd2;

        FD_ZERO(&master);
        FD_ZERO(&read_fds);
        FD_SET(listener, &master);
        FD_SET(STDIN_FILENO, &master);
        fdmax = listener > STDIN_FILENO ? listener : STDIN_FILENO;

        
        

        printf("Server avviato!\n");
        //devo mettere in ascolto 
        ret = listen(listener, MAX_HOST);
        if(ret < 0){
            perror("errore nella listen");
            exit(-1);
        }

        

        while(1){
        
         
        memset(cmd, 0, sizeof(cmd));
        //dopo averlo avviato lo metto in ascolto, posso avere massimo due host
        //dopo il login inizializzo la partita e invio i comandi
        read_fds = master;

        ret = select(fdmax+1, &read_fds, NULL, NULL, NULL);
        if(ret < 0 ){
                        perror("errore nella select");
                        exit(-1);
                    }

        for(i = 0; i <= fdmax; i++){  
            
            if(FD_ISSET(i, &read_fds)){ //o stdin server, o listener o scambio di dati
                
                  if(i == STDIN_FILENO){
                      
                     input_server();

                  }else if(i == listener){ //richiesta di connessione
                    
                    len = sizeof(clt_addr);
                
                    new_sd = accept(listener, (struct sockaddr*)&clt_addr, &len);
                    
                    //handshake per capire che giocatore è
                    ret = recv_auto(new_sd, buf);
                    if(!ret){
                        perror("error");
                        continue;
                    }
                    if(!strcmp(buf, "client")){
                        giocatore_principale = new_sd;
                    }else if(!strcmp(buf, "other")){
                        giocatore_speciale = new_sd;
                    }
                    
                    printf("nuova connessione %d\n", new_sd);

                    if(new_sd < 0){
                        perror("errore nella accept");
                        close(new_sd);
                        continue;
                    }
                    
                    FD_SET(new_sd, &master);
                    if(new_sd > fdmax){
                        fdmax = new_sd;
                    }

                }else{ //inserimento comando del client

                    
                    
                    //a seconda del comando che ricevo mi comporto di conseguenza, ho fatto in modo che quando il client si rivolge al server inizi sempre col comando
                    ret = recv_auto(i, cmd);
                    
                    if(ret == false){ //giocatore disconnesso
                        FD_CLR(i, &master);
                        if(i == giocatore_speciale){
                            giocatore_speciale = -1;
                        }
                        continue;
                    }
                    
                    if(!strcmp(cmd, "login")){
                        ret = login_server(i, usr, psw);
                        if(ret){
                        strcpy(buf, "ok");
                        send_auto(i, buf);
                        
                        p = inizializzazione_partita();
                        }else{
                            strcpy(buf, "no");
                            send_auto(i, buf);
                        }
                    }else if(!strcmp(cmd, "special")){
                        //gestione funzione speciale
                        //controllo che possa usarla
                        if(p->funzione == false){
                           
                            //glielo dico
                            strcpy(buf, "Non puoi usare la funzione speciale.\n\n");
                            ret = send_auto(i, buf);
                            if(!ret){
                                perror("errore invio esito");
                            }
                        }else{
                            //qui controllo prima che other sia online, gli mando ok o no
                            if(giocatore_speciale == -1){
                                
                                //nessun other online
                                strcpy(buf, "Nessun giocatore online ad aiutarti.\n\n");
                                ret = send_auto(i, buf);
                                if(!ret){
                                    perror("errore invio giocatore offline");
                                }
                                
                            }else{
                                //gli dico ok
                                strcpy(buf, "ok");
                                ret = send_auto(i, buf);
                                
                                //ricevo l'argomento
                                ret = recv_auto(i, ogg);
                                
                                
                                
                                for(j = 0; j < NOBJ; j++){
                                    if(!strcmp(ogg, enigmi[j].oggettoAssociato)){
                                        trovato = true;
                                        strcpy(enigma, enigmi[j].descr);
                                    }
                                }

                                if(trovato == true){
                                    
                                    //gli mando si
                                    strcpy(buf, "si");
                                    ret = send_auto(i, buf);
                                    if(!ret){
                                        perror("errore invio trovato");
                                    }

                                    //mando l'enigma a other se è connesso
                                    if(giocatore_speciale != -1){
                                        ret = send_auto(giocatore_speciale, enigma);
                                        if(!ret){
                                             perror("error");
                                             continue;
                                        }

                                        //ricevo la risposta
                                        ret = recv_auto(giocatore_speciale, risposta);
                                        if(!ret){
                                            perror("error");
                                            continue;
                                        }
                                    }else{ //se entro qui il giocatore non è connesso

                                        strcpy(risposta, "Nessun giocatore online che può aiutarti!\n Riprova più tardi\n\n");


                                    }
                                    
                                    //la invio al giocatore principale
                                    ret = send_auto(i, risposta);
                                    if(!ret){
                                        perror("errore invio risposta");
                                        continue;
                                    }

                                    if(giocatore_speciale != -1){
                                        p->funzione = false;
                                        FD_CLR(giocatore_speciale, &master);
                                        giocatore_speciale = -1;
                                    }

                                }else{ //qua se non ho trovato l'enigma
                                    //hai mandato l'oggetto sbagliato
                                    
                                    strcpy(buf, "Forse hai scritto male, riprova! Ricorda, puoi usare la\nfunzione speciale solo su oggetti bloccati da un enigma\n\n");
                                    ret = send_auto(i, buf);
                                    if(!ret){
                                        perror("errore invio scritto male");
                                    }

                                }
                            
                                
                            }
                        }

                        
                        sprintf(buf, "%d", p->token);
                        ret = send_auto(i, buf);
                        if(!ret){
                            perror("error");
                        }

                    }else{
                        ret = gestione_partita_server(i, p, cmd);
                        if(!ret){
                            printf("Disconnessione\n");
                            if(i == giocatore_speciale){
                                giocatore_speciale = -1;
                            }
                            FD_CLR(i, &master);
                            
                        }
                    }   
                    
                }

            }

        }

        }
    }
    
        
}

void mostra_comandi_serverStart(){

    printf("****************SERVER STARTED******************\n");
    printf("Digita un comando:\n");
    printf("1) start ----> avvia il server di gioco\n");
    printf("2) stop ----> termina il server\n");
    printf("************************************************\n");

}

void input_server(){
    //o stop o start
    char buf[DIMBUFF];
    char command[DIMCMD];
    

    if(fgets(buf, sizeof(buf), stdin) != NULL){
        
        buf[strcspn(buf, "\n\r")] = '\0'; //rimuovo il ritorno carrello se presente

        if(sscanf(buf, "%s", command) == 1){
            if(strcmp(command, "stop") == 0){
                //gestione stop
                if(server_avviato == 1){
                    //sono qui se sto richiedendo di chiudere il server dopo aver avviato una partita
                    close(listener);
                    // Chiudi le connessioni esistenti
                    for (int i = 0; i <= fdmax; i++) {
                        if (FD_ISSET(i, &master)) {
                            if (i != listener) {
                                
                                close(i);
                                FD_CLR(i, &master);
                            }
                        }
                    }
                    

                    fflush(stdout);
                    
                    exit(1);
                }else{
                    //sono qui se ricevo stop ancor prima dell'avvio
                    printf("Server non in funzione!\n");
                    mostra_comandi_serverStart();
                }
                
                return;
            }else if(strcmp(command, "start") == 0){
                //gestione start
                if(server_avviato == 1){
                    printf("Server già in funzione\n");
                    return;
                }
                server_avviato = 1;
                printf("Avviamento server...\n");
                
                
                
            }else{
                mostra_comandi_serverStart();
                
            }
        }
    }
}

