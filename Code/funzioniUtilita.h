#ifndef FUNZIONIUTILITA_H
#define FUNZIONIUTILITA_H
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
#include <time.h>
#include "strutture.h"
#define DIMBUFF 1024


//funzioni per send e receive
bool send_auto(int sd, char* buf);
bool recv_auto(int sd, char* buf);

void login_utente(int sd, char* user, char* psw);
bool login_server(int sd, char*user, char*psw);

void mostra_scenari_inizio();
void presentazione_scenario();
bool gestione_partita_server(int sd, struct partita* p, char* cmd);
void gestione_partita_client(int sd);
void mostra_comandi_possibili();
void inizio();

void comando_look(int sd,char* ogg, struct partita* p);
void comando_obj(int sd, struct partita* p);
void comando_take(int sd, struct partita* p, char* ogg);
void comando_use(int sd, struct partita* p);

void info_token(int sd, int tempo_rimanente, char*buf);



#endif