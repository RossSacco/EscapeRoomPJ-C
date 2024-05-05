#include "strutture.h"


struct obj oggetti[NOBJ];
struct enigma enigmi[NEG];
struct location loc[4];
int zaino = 0; // tengo conto di quanti oggetti ho nello zaino, se è pieno faccio modulo 4

void inizializzazione_oggetti(){

    strcpy(oggetti[0].nome, "Libro1"); // libro storia della magia
    strcpy(oggetti[0].descrizione, "Sei riuscito ad aprire il libro, molto bene.\n Sfogliando le pagine cattura la tua attenzione il paragrafo\n riguardate una creatura mitologica: il Basilisco.\n Descrive la sua grandezza e la sua fatalità, capace di uccidere con uno sguardo...\n\n"); 
    strcpy(oggetti[0].descr_bloccato, "E' un libro molto grande e polveroso. Non riesci ad aprirlo,\n forse ci vuole una magia! Usa il comando 'take' sull'oggetto\n per tentare di sbloccarlo\n\n");
    strcpy(oggetti[0].luogo, "Biblioteca");
    oggetti[0].bloccato = true;
    oggetti[0].token = 1;


    strcpy(oggetti[1].nome, "Libro2"); //libro d'incantesimi
    strcpy(oggetti[1].descrizione, "All'interno del libro ci sono varie tipologie di incantesimi,\n cattura la tua attenzione la sezione di Trasfigurazione, sai che scherzi!\n Ma leggendo più affondo leggi un incantesimo interessante chiamato\n Alohomora (apre tutte le porte)!\n\n");
    strcpy(oggetti[1].descr_bloccato, "");
    strcpy(oggetti[1].luogo, "Biblioteca");
    oggetti[1].bloccato = false;
    oggetti[1].token = 0;

    strcpy(oggetti[2].nome, "Firebolt");
    strcpy(oggetti[2].descrizione, "Una Firebolt abbandonata! Si tratta del modello di Scopa\n più popolare del mondo! Veloce ed elegante\n Chissà cosa ci fa qui...\n\n");
    strcpy(oggetti[2].descr_bloccato, "");
    strcpy(oggetti[2].luogo, "Foresta");
    oggetti[2].bloccato = false;
    oggetti[2].token = 0;

    strcpy(oggetti[3].nome, "Codice");
    strcpy(oggetti[3].descrizione, "La patina da cui era coperto il codice è sparita!\n E' chiaramente leggibile la sequenza 62442!!!\n\n");
    strcpy(oggetti[3].descr_bloccato, "C'è inciso qualcosa qui. Serve un incantesimo per riverarlo!\n Usa il comando 'take' sull'oggetto\n per tentare di sbloccarlo\n\n");
    strcpy(oggetti[3].luogo, "Foresta");
    oggetti[3].bloccato = true;
    oggetti[3].token = 1;

    strcpy(oggetti[4].nome, "Polisucco"); //pozione polisucco
    strcpy(oggetti[4].descrizione, "Ecco la pozione Polisucco. Basta un capello e prenderai\n le sembianze di chiunque vorrai. Se decidi di prenderla, usala con cautela\n\n");
    strcpy(oggetti[4].descr_bloccato, "Il professor Piton ci tiene alle sue pozioni.\n Serve un incantesimo per ingannarlo! Usa il comando\n 'take' sull'oggetto per tentare di sbloccarlo\n\n" );
    strcpy(oggetti[4].luogo, "Pozioni");
    oggetti[4].bloccato = true;
    oggetti[4].token = 1;

    strcpy(oggetti[5].nome, "Veritaserum");
    strcpy(oggetti[5].descrizione, "Con la pozione Veritaserum basterebbe una goccia ed anche\n il tuo peggior nemico ti rivelerebbe i suoi segreti più intimi, se solo glielo chiederai!\n\n");
    strcpy(oggetti[5].descr_bloccato, "");
    strcpy(oggetti[5].luogo, "Pozioni");
    oggetti[5].bloccato = false;
    oggetti[5].token = 0;

    strcpy(oggetti[6].nome, "AlgaBranchia");
    strcpy(oggetti[6].descrizione, "Studiata ad erbologia, mangiandola potrai respirare sott'acqua per circa un'ora. Fai attenzione!\n\n");
    strcpy(oggetti[6].descr_bloccato, "");
    strcpy(oggetti[6].luogo, "Pozioni");
    oggetti[6].bloccato = false;
    oggetti[6].token = 0;

    strcpy(oggetti[7].nome, "Basilisco"); //oggetto finale, prendendo questo token vinci.
    strcpy(oggetti[7].descrizione, "Ce l'hai fatta. Hai vinto. Il basilisco ti dona\n la sua sapienza, il segreto del potere è...\n...Coming Soon");
    strcpy(oggetti[7].descr_bloccato, "Sei arrivato fin qui, davanti al Basilisco.\n Riuscirai a conquistare il potere?\n Usa il comando 'take'\n\n");
    strcpy(oggetti[7].luogo, "Camera"); //camera dei segreti
    oggetti[7].bloccato = true;
    oggetti[7].token = 1;

    strcpy(oggetti[8].nome, "Porta1"); //porta lezione di Pozioni
    strcpy(oggetti[8].descrizione, "La porta è aperta, puoi entrare nell'aula di +++Pozioni+++\n\n");
    strcpy(oggetti[8].descr_bloccato, "Il professor Piton l'ha sigillata! Chissà cosa nasconde...\n Usa il comando 'take' sull'oggetto per tentare di sbloccarlo\n\n");
    strcpy(oggetti[8].luogo, "Pozioni");
    oggetti[8].bloccato = true;
    oggetti[8].token = 0;

    strcpy(oggetti[9].nome, "Porta2"); //porta camera dei segreti
    strcpy(oggetti[9].descrizione, "La porta è aperta, puoi entrare nella +++Camera+++ dei segreti\n\n");
    strcpy(oggetti[9].descr_bloccato, "Questa porta dev'essere importante, è ben protetta.\n Usa il comando 'take' sull'oggetto per tentare di sbloccarlo\n\n");
    strcpy(oggetti[9].luogo, "Camera"); //camera dei segreti
    oggetti[9].bloccato = true; 
    oggetti[9].token = 0;
}

void inizializzazione_enigmi(){
    //6 enigmi
    strcpy(enigmi[0].descr, "Qual è la maledizione-senza-perdono tra le seguenti?\n - Expelliarmus\n - Lumos\n - Imperio\n");
    strcpy(enigmi[0].risposta, "Imperio");
    strcpy(enigmi[0].oggettoAssociato, "Libro1");

    strcpy(enigmi[1].descr, "Completa la citazione: \n 'E' inutile rifugiarsi nei sogni,\n Harry, e dimenticarsi di ------'");
    strcpy(enigmi[1].risposta, "vivere");
    strcpy(enigmi[1].oggettoAssociato, "Codice");

    strcpy(enigmi[2].descr, "Qual è il nome completo del professor Silente?\n Albus Percival Wulfric ----- Silente\n - Brian\n - Merlino\n - Michael\n");
    strcpy(enigmi[2].risposta, "Brian");
    strcpy(enigmi[2].oggettoAssociato, "Polisucco");

    strcpy(enigmi[3].descr, "Qual è il codice per poter accedere\n al ministero della magia tramite cabina telefonica\n a Londra?\n");
    strcpy(enigmi[3].risposta, "62442");
    strcpy(enigmi[3].oggettoAssociato, "Porta2"); // porta camera dei segreti

    strcpy(enigmi[4].descr, "Cosa permette al Basilisco di uccidere all'istante?\n");
    strcpy(enigmi[4].risposta, "Occhi");
    strcpy(enigmi[4].oggettoAssociato, "Basilisco");
}

void inizializzazione_location(){
    //4 location
    strcpy(loc[0].nome, "Biblioteca");
    strcpy(loc[0].descrizione, "Sei nella Biblioteca. Davanti a te hai immensi scaffali pieni di libri.\n Vedi su un tavolo il ***Libro2*** d'incantesimi, sembra interessante.\n Noti un libro nascosto in fondo allo scaffale, sarà difficile prenderlo,\n è il ***Libro1*** di Storia della Magia");
    loc[0].bloccata = false;
    strcpy(loc[0].descr_blocc, "");

    strcpy(loc[1].nome, "Pozioni");
    strcpy(loc[1].descrizione, "Ti dirigi subito verso lo scaffale.\n In esso trovi diverse pozioni, tutte potenti.\n Leggi sulle etichette ***Veritaserum***, ***Polisucco***, ***AlgaBranchia***...\n Qualcuna mi sarà utile?\n\n");
    loc[1].bloccata = true;
    strcpy(loc[1].descr_blocc, "Non puoi ancora entrare. Apri prima la porta\n\n");

    strcpy(loc[2].nome, "Foresta");
    strcpy(loc[2].descrizione, "Segui il sentiero formatasi nel terreno.\n Poco più avanti rispetto a te noti qualcosa\n adagiato in terra, sembra una ***Firebolt***,\n ti stupisce vederla lì abbandonata, considerandone il valore.\n Accanto a te scorgi una scritta sul tronco di un albero\n, sembra un ***Codice***, cosa sarà?\n\n");
    loc[2].bloccata = false;
    strcpy(loc[2].descr_blocc, "");

    strcpy(loc[3].nome, "Camera"); //camera dei segreti
    strcpy(loc[3].descrizione, "C'è una grande sala davanti a te. Scorgi in fondo un'immensa statua.\n Ti avvicini quando ti accorgi che la statua si muove!\n Davanti a te hai il ***Basilisco***");
    loc[3].bloccata = true;
    strcpy(loc[3].descr_blocc, "Non puoi ancora entrare. Apri prima la Porta\n\n");


}


struct partita* inizializzazione_partita(){

    struct partita *partita;
    partita = (struct partita*)malloc(sizeof(struct partita));

    partita->token = 0;

    partita->funzione = false;

 

    //inizialmente il giocatore non ha con se nessun oggetto
 

    for (int i = 0; i < OBJPTB; i++) {
        strcpy(partita->oggettiMiei[i].nome, "");
        strcpy(partita->oggettiMiei[i].descrizione, "");
        partita->oggettiMiei[i].bloccato = false;
        strcpy(partita->oggettiMiei[i].luogo, "");
        partita->oggettiMiei[i].token = 0;
    }


    return partita;
}