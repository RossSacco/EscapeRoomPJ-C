#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "funzioniUtilita.h"
#include "strutture.h"


//host in cui implemento la funzionalità speciale.
//Potrà essere chiamato solo dopo che il giocatore principale avrà risolto un enigma.
//a seconda del luogo dove verrà chiamato risolverà l'enigma per cui il giocatore ha chiesto aiuto ed uscirà.


int main(int argc, char* argv[]){

    int ret, sd, porta, x;
    char buf[DIMBUFF];
    char enigma[DIMBUFF], risposta[DIMBUFF];
    struct sockaddr_in srv_addr;


    if(argc>1){
        porta = atoi(argv[1]);
    }

    sd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(4242);
    inet_pton(AF_INET, "127.0.0.1", &srv_addr.sin_addr);

    
    for(x = 0; x < 5; x++){
    ret = connect(sd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if(ret > 0){
        break;
    }
    sleep(1);
    }
    
    

    //invio "client" per fagli capire chi sono
    strcpy(buf, "other");
    ret = send_auto(sd, buf);
    if(!ret){
        exit(-1);
    }

    printf("Connessione con il server stabilita! %d\n\n", porta);


    //mi metto in attesa che il giocatore abbia bisogno di me.
    printf("Attendi il tuo momento!\n\n");

    //ricevo l'enigma
    ret = recv_auto(sd, buf);
    if(!ret){
        exit(1);
    }

    while ((x = getchar()) != '\n' && x != EOF);
    printf("\nTocca a te! Fai del tuo meglio\n\n");
    printf("%s\n\n", buf);

    if(fgets(risposta, sizeof(risposta), stdin)!= 0){
        risposta[strcspn(risposta, "\n")] = '\0';
    }

    ret = send_auto(sd, risposta);
    if(!ret){
        exit(1);
    }

    printf("\nGrazie del tuo contributo, sarà stato abbastanza?\n\n");

    close(sd);

}