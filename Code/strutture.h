#ifndef STRUTTURE_H
#define STRUTTURE_H
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
#define NOBJ 10 // 8 oggetti + le 2 porte
#define OBJPTB 4
#define DIMBUFF 1024
#define NEG 5 // 4 oggetti + 1 porte

struct obj{
    char nome[DIMBUFF];
    char descrizione[DIMBUFF];
    char descr_bloccato[DIMBUFF];
    bool bloccato; //se bloccato avrà un enigma associato
    char luogo[20];
    int token; //1 se bloccato e poi sbloccato, sennò 0
};

extern struct obj oggetti[NOBJ];

struct enigma{
    char descr[DIMBUFF];
    char risposta[DIMBUFF];
    char oggettoAssociato[DIMBUFF];
};

extern struct enigma enigmi[NEG];

struct location{
    char nome[DIMBUFF];
    char descrizione[DIMBUFF];
    char descr_blocc[DIMBUFF];
    bool bloccata;
};

extern struct location loc[4];

extern int zaino;

//per ogni partita devo tener conto degli oggetti raccolti, enigmi risolti, token,  funzione speciale sbloccata.
//anche qui tengo una struttura
struct partita{
    
    int token; //per vincere devi raccoglierli tutti, inizializzati a 0, li ottengo risolvendo gli enigmi
    struct obj oggettiMiei[OBJPTB]; //posso portare fino a 4 oggetti
    //ogni oggetto è identificato da un numero
    //struct obj oggettiSbloccati[NOBJ]; 
    //0--> Libro d'incantesimi  - Biblio - 1 (libero)
    //1--> Libro di Storia della Magia   - Biblio - 0 (enigma)
    //2--> Scopa Volante *inutile*       - Foresta - 1 (libero)
    //3--> Strana scritta su un tronco   - Foresta - 0 (enigma) (non è proprio un oggetto ma portarlo con se indica la conoscenza di esso)
    //4--> Pozione Polisucco             - Pozioni - 0 (enigma) (devo prima sbloccare la stanza)
    //5--> Veritaserum                   - Pozioni - 1 (libero) ('')
    //6--> AlgaBranchia                  - Pozioni - 1 (libero) ('')
    //7--> Basilisco                     - Camera dei Segreti (ultimo enigma per la vittoria)
    // -----> per risolvere l'enigma finale e vincere serve 1 oltre che gli oggetti per entrare nella stanza

    
    //8 --> porta Lezione di Pozioni - 0 bloccata
    //9 --> Camera dei segreti - 0 bloccata 

    bool funzione; //se posso usare la funzione speciale
};


void inizializzazione_oggetti();
void inizializzazione_enigmi();
void inizializzazione_location();
struct partita* inizializzazione_partita();



#endif