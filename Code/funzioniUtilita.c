#include "funzioniUtilita.h"



bool recv_auto(int sd, char* buf){
    int len, ret;
    uint16_t lmes;

    ret = recv(sd, (void*)&lmes, sizeof(uint16_t), 0);
    if(ret <= 0){
        if (ret == 0) {
            printf("Disconnessione\n");
            
        } else {
            perror("Errore nella ricezione della lunghezza del messaggio1");
            exit(-1);
        }

        close(sd);
        return false;
    }
    len = ntohs(lmes);
    ret = recv(sd, (void*)buf, len, 0);
    if(ret <= 0){
        if (ret == 0) {
            printf("Disconnessione\n");
            
        } else {
            perror("Errore nella ricezione della lunghezza del messaggio2");
            exit(-1);
        }
        close(sd);
        return false;
    }
    return true;
}

bool send_auto(int sd, char* buf){
    int len, ret;
    uint16_t lmes;

    len = strlen(buf) + 1;
    lmes = htons(len);

    ret = send(sd, (void*)&lmes, sizeof(uint16_t), 0);
    if(ret <= 0){
        if (ret == 0) {
            printf("Disconnessione\n");
            
        } else {
            perror("Errore nell'invio della lunghezza del messaggio3");
            exit(-1);
        }
        
        close(sd);
        return false;
    }
    ret = send(sd, (void*)buf, len, 0);
    if(ret <= 0){
        if (ret == 0) {
            printf("Disconnessione\n");
            
        } else {
            perror("Errore nell'invio della lunghezza del messaggio4");
            exit(-1);
        }
        close(sd);
        return false;
        
    }

    return true;
}


void login_utente(int sd, char* user, char* psw){
    
    char cmd[10];
    int ret;


    strcpy(cmd, "login");

    ret = send_auto(sd, cmd);
    if(ret == false){
        perror("error");
        exit(0);
    }

    ret = send_auto(sd, user);
     if(ret == false){
        perror("error");
        exit(0);
    }
    

    ret = send_auto(sd, psw);
     if(ret == false){
        perror("error");
        exit(0);
    }

}

bool login_server(int sd, char* usr, char*psw){
    //la recv di cmd la faccio prima, qui devo verificare user e psw
    fflush(stdout);
    int ret;
    FILE* fd;
    int presente = 0;
   
   
    char u[30], p[30];
    
    ret = recv_auto(sd, usr);
     if(ret == false){
        perror("error");
    }
    printf("user ricevuto %s\n", usr);

    ret = recv_auto(sd, psw);
     if(ret == false){
        perror("error");
    }
    printf("psw arrivata %s\n", psw);

     
    fflush(stdout);
    //ora apro il file e controllo

    fd = fopen("utenti.txt", "r+");
    if(fd == NULL){
        perror("errore nell'apertura del file");
    }
    
    while(fscanf(fd, "%s %s\n", u, p) == 2){
        if(!strcmp(usr, u)){
            presente = 1;
            if(!strcmp(psw, p)){
                printf("accesso eseguito! Ciao %s\n", usr);
                return true;
            }else{
                printf("Password Errata!\n");
                fclose(fd);
                return false;
            
            }
        }
    }
    
    if(presente == 0){ //user non esistente, creo il profilo
        fprintf(fd, "%s %s\n", usr, psw);
        printf("registrazione effettuata! Benvenuto %s\n", usr);
    }

    fclose(fd);
    return true;
    
}


void mostra_scenari_inizio(){
    printf("****************SCEGLI UNO SCENARIO***************\n");
    printf("Benvenuto in questa escape room!\n Decidi a quale scenario vuoi giocare e digita 'start <num. scenario>'\n");
    printf("1) Scuola di Magia e Stregoneria di Hogwarts\n");
    printf("**************************************************\n\n");
}


void presentazione_scenario(){
    printf("****************PARTENZA**************************\n");
    printf("Benvenuto nella scuola di magia e stregoneria di Hogwarts!\n");
    printf("Esplora la scuola e risolvi il mistero del potere per\n diventare il mago più potente della storia!\n\n");
    printf("Sei nell'atrio. La porta alla tua sinistra porta alla +++Biblioteca+++.\n");
    printf("Davanti a te vedi due porte chiuse. Una è la ***Porta1*** lezione di Pozioni.\n");
    printf("L'altra porta ha un aspetto misterioso, sembra ben chiusa, è la\n ***Porta2*** Camera dei Segreti\n");
    printf("Potresti anche fare un giro nella +++Foresta++ proibita, è una bella giornata!\n");
    printf("Scegli cosa fare.\n");
    printf("**************************************************\n\n");
}

void inizio(){
    printf("Sei nell'atrio. La porta alla tua sinistra porta alla +++Biblioteca+++.\n");
    printf("Davanti a te vedi due porte chiuse. Una è la ***Porta1*** lezione di Pozioni.\n");
    printf("L'altra porta ha un aspetto misterioso, sembra ben chiusa, è la\n ***Porta2*** Camera dei Segreti\n");
    printf("Potresti anche fare un giro nella +++Foresta++ proibita, è una bella giornata!\n\n");
}


void info_token(int sd, int tempo_rimanente, char*info){

    int ret, token_raccolti, token_tot = 4;
    char inf[BUFSIZ];

     ret = recv_auto(sd, inf);
        if(!ret){
            exit(1);
        }

    strcpy(info, inf);
    token_raccolti = atoi(inf);
    
    if(token_raccolti == 4){
        printf("Hai vinto, Congratulazioni! Alla prossima avventura\n");
        inizializzazione_enigmi();
        inizializzazione_location();
        inizializzazione_oggetti();
        sleep(5);
        exit(0);
    }
    printf("token raccolti: %d, token mancanti: %d, tempo rimanente: %d s\n\n", token_raccolti, token_tot - token_raccolti, tempo_rimanente);

}

void gestione_partita_client(int sd){ //mi serve l'id del socket del server
    int ret, x; 
    char buf[BUFSIZ]; //buffer su cui ricevo eventuali enigmi e stringa di stdin
    char info[BUFSIZ]; //per mandare le info dopo ogni comando
    char cmd[10], oggetto1[BUFSIZ], oggetto2[BUFSIZ]; //uso nomi abbastanza lunghi
    char risposta[BUFSIZ]; //per risposta ad eventuali enigmi
    int token_tot = 4;
    uint16_t token_raccolti = 0;
    int tempo_rimanente; //variabili che mi servono per mostrare i dati dopo ogni comando


    //inizializzo la stringa info, se uso look per primo non ricevo dal server
    strcpy(info, "token raccolti: 0, token mancanti: 4, tempo rimanente: 3600");

    //la partita dura un'ora, se il tempo finisce, l'utente perde
    time_t inizio_partita, current_time;

    time(&inizio_partita);
    
    
    while(1){
        

        //ad ogni ciclo pulisco le variabili
        memset(buf, 0, sizeof(buf));
        memset(cmd, 0, sizeof(cmd));
        memset(oggetto1, 0, sizeof(oggetto1));
        memset(oggetto2, 0, sizeof(oggetto2));
        memset(risposta, 0, sizeof(risposta));


        

        //gli enigmi partono se si chiama la take sull'oggetto

        //aspetto l'inserimento 
        if(fgets(buf, sizeof(buf), stdin)!= 0){
            buf[strcspn(buf, "\n")] = '\0';


            ret = sscanf(buf, "%s %s %s", cmd, oggetto1, oggetto2);

            
            //devo sempre controllare il tempo
            time(&current_time);
            tempo_rimanente = 3600 - difftime(current_time, inizio_partita);
            if(tempo_rimanente <= 0){
                printf("Il tempo non ti è stato amico, Riprova.\n");
                sleep(5);
                break;
            }

            
            

            //comando look, con look senza argomenti gli mando le scritte di inizio partita in cui è descritto tutto in generale
            if((ret >= 1) && !strcmp(cmd, "look")){
                if(ret == 2){ //ha inserito una location o un oggetto
                    //mando prima il comando poi l'oggetto
                    
                    ret = send_auto(sd, cmd);
                    if(!ret){
                        exit(1);
                    }

                    ret = send_auto(sd, oggetto1);
                    if(!ret){
                        exit(1);
                    }

                    ret = recv_auto(sd, buf); //risposta del server al comando
                    if(!ret){
                        exit(1);
                    }

                    printf("%s\n\n", buf);

                    
                    //dopo ogni comando stampo le informazioni, me le deve mandare il server
                 
                    info_token(sd, tempo_rimanente, info);


                }else{
                    inizio();
                    token_raccolti = atoi(info);
                    printf("token raccolti: %d, token mancanti: %d, tempo rimanente: %d s\n\n", token_raccolti, token_tot - token_raccolti, tempo_rimanente); //qua stampo quelle salvate
                }
            }else if((ret == 1) && !strcmp(cmd, "obj")){ // mi mostra oggetti nello zaino
                ret = send_auto(sd, cmd);
                if(!ret){
                    exit(1);
                }

                ret = recv_auto(sd, buf); //qui mi aspetto la lista degli oggetti
                if(!ret){
                    exit(1);
                }

                printf("%s\n\n", buf);

                info_token(sd, tempo_rimanente, info);

            }else if((ret == 1) && !strcmp(cmd, "end")){ //termina la partita
                ret = send_auto(sd, cmd); //mando comando al server
                if(!ret){
                    exit(1);
                }

                ret = recv_auto(sd, buf); //qui mi aspetto la stringa di addio
                if(!ret){
                    exit(1);
                }

                inizializzazione_enigmi();
                inizializzazione_location();
                inizializzazione_oggetti();
                inizializzazione_partita();

                printf("%s\n\n", buf);
                break; //esco dal ciclo infinito, partita finita, se si esce con end non si vince 
            }else if((ret == 2) && !strcmp(cmd, "take")){
                //qui per forza due argomenti
                ret = send_auto(sd, cmd); //mando comando
                if(!ret){
                    exit(1);
                }

                ret = send_auto(sd, oggetto1); // mando argomento
                    if(!ret){
                        exit(1);
                    }

                //da qui in poi in gestione server in comando_take

                //ora potrei o dover risolvere un enigma, buffer risposta,  o ricevere direttamente l'esito della take
                //o prendo l'oggetto o lo zaino è pieno e non posso
                //se è bloccato mi faccio mandare la stringa "enigma"
                ret = recv_auto(sd, buf); //stringa per capire se è bloccato o no
                if(!ret){
                    exit(1);
                }

                if(!strcmp(buf, "enigma")){
                    //ricevo il testo dell'enigma
                    ret = recv_auto(sd, buf);
                    if(!ret){
                        exit(1);
                    }

                    printf("%s\n\n", buf);

                    //devo mandare la risposta
                    if(fgets(risposta, sizeof(risposta), stdin)!= 0){
                        risposta[strcspn(risposta, "\n")] = '\0';
                    }

                    ret = send_auto(sd, risposta);
                    if(!ret){
                        exit(1);
                    }

                    //ricevo esito
                    ret = recv_auto(sd, buf);
                    if(!ret){
                        exit(1);
                    }

                    printf("%s\n\n", buf);

                }else if(!strcmp(buf, "libero")){
                    //oggetto già libero
                    //ricevo esito
                    ret = recv_auto(sd, buf);
                    if(!ret){
                        exit(1);
                    }

                    printf("%s\n\n", buf);                    

                }else{
                //se non è ne libero ne' enigma ho sbagliato a scrivere o è Porta1
                ret = recv_auto(sd, buf);
                    if(!ret){
                        exit(1);
                    }

                    printf("%s\n\n", buf); 
                }

               
                info_token(sd, tempo_rimanente, info);


            }else if((ret > 1) && !strcmp(cmd, "use")){ //comando use
                
                x = send_auto(sd, cmd); //mando comando al server
                if(!x){
                    exit(1);
                }

                if(ret == 2){ //un argomento
                //mando stringa 'uno' per dirgli che è un solo argomento
                    strcpy(buf, "uno");
                    ret = send_auto(sd, buf);
                    if(!ret){
                        exit(1);
                    }

                //mando argomento
                    ret = send_auto(sd, oggetto1);
                    if(!ret){
                        exit(1);
                    }

                //ricevo esito
                ret = recv_auto(sd, buf);
                if(!ret){
                    exit(1);
                }

                printf("%s\n", buf);

                }else if(ret == 3){ // due argomenti
                //mando stringa 'due' per dirgli che ho due argomenti
                    strcpy(buf, "due");
                    ret = send_auto(sd, buf);
                    if(!ret){
                        exit(1);
                    }

                    ret = send_auto(sd, oggetto1); //mando primo arg
                    if(!ret){
                        exit(1);
                    }

                    ret = send_auto(sd, oggetto2); //mando secondo arg
                    if(!ret){
                        exit(1);
                    }

                    //esito
                    ret = recv_auto(sd, buf);
                    if(!ret){
                        exit(1);
                    }

                    printf("%s\n\n", buf);
        


                }else{ //mando più di tre argomenti
                    strcpy(buf, "furbo");
                    ret = send_auto(sd, buf);
                    if(!ret){
                        exit(1);
                    }

                    //ricevo e stampo esito
                    //proviamo cosi
                    mostra_comandi_possibili();
                    
                }

                //solita stampa dei token
               
                info_token(sd, tempo_rimanente, info);

            }else if((ret == 2) && !strcmp(cmd, "special")){
                //voglio usare funzione speciale, gestico la cosa su server.c
                //invio il comando
                ret = send_auto(sd, cmd);
                if(!ret){
                    exit(1);
                }


                //esito chiamata
                ret = recv_auto(sd, buf); //
                if(!ret){
                    exit(1);
                }

                    //se la chiamata è andata a buon fine continuo
                if(!strcmp(buf, "ok")){
                        
                        // invio oggetto
                        ret = send_auto(sd, oggetto1);
                        if(!ret){
                            exit(-1);
                        }

                        //se trova l'oggetto o no
                        ret = recv_auto(sd, info);
                        if(!ret){
                            perror("non ho ricevuto esito");
                            exit(-1);
                        }


                        if(!strcmp(info, "si")){
                        //leggo risposta enigma
                        
                            ret = recv_auto(sd, buf);
                            if(!ret){
                                exit(-1);
                            }

                            printf("Aiuto arrivato! Il tuo amico pensa che la risposta sia:\n%s\nVerifica tu stesso!\n\nHai usato la tua unica possibilità d'aiuto\n\n", buf);
                        }else{
                            //leggo problema
                            printf("%s\n\n", info);
                        }
                    }else{
                    //se non c'è nessuno online o non posso usare la funzione
                        //mando esito
                        printf("%s\n\n", buf);
                    }


                info_token(sd, tempo_rimanente, info);
            }
            else{
                mostra_comandi_possibili();
                token_raccolti = atoi(info);
                printf("token raccolti: %d, token mancanti: %d, tempo rimanente: %d s\n\n", token_raccolti, token_tot - token_raccolti, tempo_rimanente); //qua stampo quelle salvate
    
            }


    
            
        }


        
    }


}


bool gestione_partita_server(int sd, struct partita* p, char* cmd){ //se il client si disconnette restituisco false

    int ret;
    //uint16_t tk;
    char buf[DIMBUFF]; //qui gestisco quello che gli devo mandare
    char oggetto1[DIMBUFF], oggetto2[DIMBUFF];
    char risposta[DIMBUFF];

    //pulizia
    memset(oggetto1, 0, sizeof(oggetto1));
    memset(oggetto2, 0, sizeof(oggetto2));
    memset(risposta, 0, sizeof(risposta));
    memset(buf, 0, sizeof(buf));

    //il comando lo ricevo in server.c, qui mi occupo della gestione

    if(!strcmp(cmd, "look")){
        //ricevo l'argomento
        ret = recv_auto(sd, oggetto1);
        if(!ret){
            return false;
        }
        
        comando_look(sd,oggetto1, p);
        //printf("sto mandando le informazioni\n");
        
    }else if(!strcmp(cmd, "obj")){

        comando_obj(sd, p);
        
    }else if(!strcmp(cmd, "end")){

        strcpy(buf, "Spero che il gioco sia stato di tuo gradimento!\n");
        ret = send_auto(sd, buf);
        //non controllo tanto mando false a prescindere
        return false;
    }else if(!strcmp(cmd, "take")){

        //ricevo argomento
        ret = recv_auto(sd, oggetto1);
        if(!ret){
            return false;
        }

       

        comando_take(sd, p, oggetto1);

    }else if(!strcmp(cmd, "use")){

        comando_use(sd, p);

    }

    //devo inviare token raccolti
    //il tempo è gestito nel client quindi per stamparlo se la vede lui, anche i token mancanti si calcola
    printf("sto mandando le informazioni\n");
    sprintf(buf, "%d", p->token);
    ret = send_auto(sd, buf);
    if(!ret){
        return false;
    }

    return true;
}


void comando_look(int sd,char* ogg, struct partita* p){

    printf("comando_look\n");
    

    char buf[DIMBUFF];
    int i,j;
    int ret;
    
    //a seconda di cos'è l'oggetto devo stamparne una descrizione
    for(i = 0; i < NOBJ; i++){

        if(!strcmp(ogg, oggetti[i].nome)){
            //oggetto trovato

            if(oggetti[i].bloccato == true){
                strcpy(buf, oggetti[i].descr_bloccato);
            }else{
                //devo controllare se ho sbloccato anche le stanze, ho Camera e Pozioni Bloccate
                for(j = 0; j < 4; j++){
                    if(!strcmp(oggetti[i].luogo, loc[j].nome)){
                        //locazione
                        if(loc[j].bloccata == true){
                            strcpy(buf, "\nPer visualizzare questo oggetto devi prima\n sbloccare la stanza in cui si trova\n\n");
                        }else{
                            strcpy(buf, oggetti[i].descrizione);
                            //strcpy(p->oggettiSbloccati[i].nome, oggetti[i].nome);
                        }
                    }
                }

                
            }
            
            ret = send_auto(sd, buf);
            if(!ret){
                perror("error");
            }

            return;
        }
    }

    for(i = 0; i < 4; i++){
        if(!strcmp(ogg, loc[i].nome)){

            if(loc[i].bloccata == true){
                strcpy(buf, loc[i].descr_blocc);
            }else{
                strcpy(buf, loc[i].descrizione);
            }
            ret = send_auto(sd, buf);
            if(!ret){
                perror("error");
            }
            return;
        }
    }

    //se sono arrivato fin qui significa che non ho trovato niente
    strcpy(buf, "forse hai scritto male, riprova!");
    send_auto(sd, buf);

}


void comando_obj(int sd, struct partita* p){

    char buf[BUFSIZ];
    int ret, i, pos = 0;

    printf("comando obj\n");

    //devo mandare al client tutti gli oggetti che ha nello zaino
    //ho massimo 4 oggetti
    for(i = 0; i < OBJPTB; i++){
        if(strcmp(p->oggettiMiei[i].nome,"")){
            pos += sprintf(buf + pos, "%s\n", p->oggettiMiei[i].nome);
        }
    }

    if(strlen(buf) == 0){
        strcpy(buf, "Nessun oggetto nello zaino");
    }

    printf("%s\n", buf);

    ret = send_auto(sd, buf);
    if(!ret){
        perror("error");
    }


}


void comando_take(int sd, struct partita* p, char* ogg){

    printf("comando_take\n");

    char buf[BUFSIZ], enig[BUFSIZ], risposta[BUFSIZ];
    //bool bloccato;
    int ret, i, j;
    int mod;

     if(!strcmp(ogg, "Porta1")){ //è bloccata ma si sblocca con la use
         strcpy(buf, "nono"); //per la receive di libero o enigma
         ret = send_auto(sd, buf);

         if(oggetti[8].bloccato == true){
             strcpy(buf, "La porta è bloccata in modo strano. Non c'è un indovinello, prova in un altro modo...\n\n");
         }else{
             strcpy(buf, "Non puoi prendere quest'oggetto!\n\n");
         }

         ret = send_auto(sd, buf);
         if(!ret){
             perror("error");
         }
         return;
     }

    //anche qui aggiungo in oggetti sbloccati

    //per prima cosa devo cercare l'oggetto corrispondente
    // se è bloccato mando la stringa 'enigma'
    for(i = 0; i < NOBJ; i++){
        if(!strcmp(ogg, oggetti[i].nome)){
            //trovo l'oggetto
            //devo controllare che l'oggetto sia in un luogo non bloccato
        if(strcmp(oggetti[i].nome, "Porta2")){
            for(j = 0; j < 4; j++){
                if(!strcmp(oggetti[i].luogo, loc[j].nome)){
                    if(loc[j].bloccata == true){
                        strcpy(buf, "nono"); //per la receive di libero o enigma
                        ret = send_auto(sd, buf);

                        
                        strcpy(buf, "Sblocca prima la stanza in cui si trova\n\n");
                        send_auto(sd, buf);
                        return;
                    }
                }
            }
        }

            if(oggetti[i].bloccato == true){
                
                if(i == 9 && oggetti[i].token == 1){ // se ho fatto la use inserisco 2 token, è la Porta2
                    strcpy(buf, "nono"); //per la receive di libero o enigma
                    ret = send_auto(sd, buf);

                    
                    strcpy(buf, "Hai già risolto quest'indovinello...prova altro, è ancora bloccata!\n\n");
                   

                    ret = send_auto(sd, buf);
                    if(!ret){
                    perror("error");
                    }
                    return;
                }


                //mando stringa 'enigma' per fargli capire al client che l'oggetto è bloccato
                strcpy(buf, "enigma");
                ret = send_auto(sd, buf);
                if(!ret){
                    perror("error");
                    //break;
                }

                //mando enigma
                for(j = 0; j < NEG; j++){
                    if(!strcmp(enigmi[j].oggettoAssociato, ogg)){
                        strcpy(enig, enigmi[j].descr);
                        strcpy(risposta, enigmi[j].risposta);
                    }
                }
                ret = send_auto(sd, enig);
                if(!ret){
                    perror("error");
                }

                //risposta enigma
                ret = recv_auto(sd, buf);
                if(!ret){
                    perror("error");
                }
                //esito
                if(!strcmp(buf, risposta)){
                    if(i != 9){ //per la porta camera oltre all'indovinello devo anche usare la pozione polisucco
                        oggetti[i].bloccato = false;
                        p->token += oggetti[i].token; // tanto la porta non da token comunque
                        
                    }
                    
                    //la porta della camera non da token, quindi se il token è = 1 significa che ho risolto l'indovinello ma non ho ancora fatto
                    //la use sulla porta, se è > 1 allora ho fatto la use e posso sbloccarla definitivamnte
                    if(i == 9){
                        oggetti[i].token++;
                        if(oggetti[i].token > 1){ // ho già fatto la use
                            oggetti[i].bloccato = false;
                            loc[3].bloccata = false;
                            
                        }else{
                            strcpy(oggetti[i].descr_bloccato, "Hai risolto l'indovinello ma non basta, \ntrova un modo per entrare senza farti riconoscere\n\n");
                        }
                        
                    }


                    strcpy(buf, "Risposta Corretta! \n\n");

                    if(p->token == 1){
                        p->funzione = true;
                        strcpy(buf, "Risposta Corretta! Hai sbloccato la funzionalità speciale:\nusa il comando 'special' <oggetto> per chiedere una mano con l'enigma\nche non riesci a risolvere!\n\n");
                    }
                }else{
                    strcpy(buf, "Risposta Sbagliata!\n\n");
                }

                ret = send_auto(sd, buf);
                if(!ret){
                    perror("error");
                }

                return;

            }else{
                //oggetto libero
                //mando stringa libero e aggiungo allo zaino, se lo zaino è pieno sostituisco dall'inizio
                strcpy(buf, "libero");
                ret = send_auto(sd, buf);
                if(!ret){
                    perror("error");
                    //break;
                }
                
                //aggiungo allo zaino, var glob zaino che mi tiene conto degli oggetti, ricordarsi modulo 4
                //aggiungo allo zaino solo se non è Porta1 o Porta2 o Basilisco
                if( i >= 7 ){
                    
                    strcpy(buf, "non puoi prendere quest'oggetto!\n\n");

                }else{
                    
                    zaino += 1;
                    mod = zaino % 4;

                strcpy(p->oggettiMiei[mod].nome, oggetti[i].nome);

                //esito
                strcpy(buf, "Oggetto aggiunto allo zaino!\n\n");

                }

                
                ret = send_auto(sd, buf);
                if(!ret){
                    perror("error");
                    //break;
                }

                return;
            }
        }
    }

    strcpy(buf, "nono"); //per la receive di libero o enigma
    ret = send_auto(sd, buf);

    //se arrivo qui non ho avuto corrispondenza
    strcpy(buf, "forse hai scritto male, riprova!");
    send_auto(sd, buf);

}


void comando_use(int sd, struct partita* p){

    printf("comando_use\n");

    char buf[BUFSIZ];
    int ret, i, j, k;
    char ogg1[BUFSIZ], ogg2[BUFSIZ];

    //per prima cosa ricevo la stringa che mi dice il num di argomenti
    ret = recv_auto(sd, buf);
    if(!ret){
        perror("error");
    }

    //se ho un solo argomento, ha effetto solo use Polisucco, si sblocca porta2, ricordarsi gestione token, spiegata nella take
    if(!strcmp(buf, "uno")){ 
        //ricevo argomento 1
        ret = recv_auto(sd, ogg1);
        if(!ret){
            perror("error");
        }

        if(!strcmp(ogg1, "Polisucco")){
            //controllo se era già sbloccata, magari sto ripetendo il comando
            //prima controllo di averlo nello zaino
            for(k = 0; k < OBJPTB; k++){
                if(!strcmp(p->oggettiMiei[k].nome, ogg1)){
                if(oggetti[9].bloccato == false || oggetti[9].token == 2){
                    strcpy(buf, "Hai già usato quest'oggetto\n\n");
                    }else{
                //sblocco porta Camera, indice 9, se ha 1 token, 
                //se ha 0 token aggiungo 2
                    if(oggetti[9].token == 1){
                        oggetti[9].bloccato = false;
                        loc[3].bloccata = false;
                        strcpy(buf, "Oggetto usato con successo, hai sbloccato ***Porta2***\n\n");
                        }else{
                        oggetti[9].token = 2;
                        strcpy(buf, "Hai modificato il tuo aspetto ma non basta! Prova altro\n\n");
                    }
                    break;
                }
                }else{
                    strcpy(buf, "Per usare un oggetto devi averlo nello zaino");
                }
            }
        }else{
            //se non trovo corrispondenze ho sbagliato a scrivere
            strcpy(buf, "Non hai scatenato nessun effetto, prova altro\n\n");
        }
        //esito
        ret = send_auto(sd, buf);
        if(!ret){
            perror("error");
        }

        return;
    }else if(!strcmp(buf, "due")){ //se ho due argomenti devo usare Libro2 su Porta1
        
        //ricevo primo
        ret = recv_auto(sd, ogg1);
        if(!ret){
            perror("error");
        }

        //ricevo secondo
        ret = recv_auto(sd, ogg2);
        if(!ret){
            perror("error");
        }

        //se non trovo corrispondenze ho sbagliato a scrivere
        if((!strcmp(ogg1, "Libro2") && !strcmp(ogg2, "Porta1") ) || (!strcmp(ogg1, "Porta1") && !strcmp(ogg2, "Libro2"))){
            //sblocco Porta1
            //controllo di avere Libro2 nello zaino
            for(k = 0; k < OBJPTB; k++){
                if(!strcmp("Libro2", p->oggettiMiei[k].nome)){
                    for(i = 0; i < NOBJ; i++){
                    if(!strcmp(oggetti[i].nome, "Porta1")){
                        oggetti[i].bloccato = false;
                        for(j = 0; j < 4; j++){
                            if(!strcmp(oggetti[i].luogo, loc[j].nome)){
                                loc[j].bloccata = false;
                            }
                        }
                        strcpy(buf, "Porta sbloccata!");
                    }
                    }
                    break;
                }else{
                    strcpy(buf, "Per usare un oggetto devi averlo nello zaino");
                }
            }

        }else{
            strcpy(buf, "Non hai scatenato nessun effetto, prova altro\n\n");
        }

        //esito
        ret = send_auto(sd, buf);
        if(!ret){
            perror("error");
        }

        return;
    }

}



void mostra_comandi_possibili(){
    printf("Puoi utilizzare uno di questi comandi:\n");
    printf("- look [location | object], per ottenere una desrizione del soggetto\n");
    printf("- take object, per mettere un oggetto nello zaino\n");
    printf("- use object1 [object2], per utilizzare un oggetto\n   o utilizzare un oggetto su un'altro");
    printf("- obj, per visualizzare gli oggetti in tuo possesso\n");
    printf("- end, per terminare il gioco\n\n");

}



