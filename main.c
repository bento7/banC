#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

// create struct Date
struct Date
{
    int day;
    int month;
    int year;
};

// create struct Transaction
struct Transaction
{
    struct Date date;
    float amount;
    char label[30];
    char name[32]; // émetteur si transaction > 0 :réception d'€ | receveur si transaction < 0: envoi d'€
};

// create struct Entete
struct Entete
{
    struct Date date;
    float solde;
};

// create struct Account
struct Account
{
    int id;
    char name [32];
};

void date(struct Date *d) {
    // `time_t` is an arithmetic time type
    time_t now;

    // Obtain current time
    time(&now);

    // localtime converts a `time_t` value to calendar time and
    // returns a pointer to a `tm` structure with its members
    // filled with the corresponding values
    struct tm *local = localtime(&now);

    d->day = local->tm_mday;            // get day of month (1 to 31)
    d->month = local->tm_mon + 1;      // get month of year (0 to 11)
    d->year = local->tm_year + 1900;   // get year since 1900
}

void ouvrir(FILE **f, char nom[]) {
    //qui ouvre le fichier donné ou le crée sinon.
    *f = fopen(nom, "r+");
    if (*f == NULL) {
        *f = fopen(nom, "w");
        if (*f == NULL) {
            perror("Erreur à l'ouverture du fichier");
            exit(EXIT_FAILURE);
        }
    }
    // si File n'existe pas
    // Créé file
}

void fermer(FILE*f){
    if (f != NULL) {
        fclose(f);
    }
}

struct Entete creation_entete(struct Date d, float solde){
    struct Entete e1;
    e1.date = d;
    e1.solde = solde;
    return e1;
};

struct Transaction creation_transaction(struct Date d, float amt, char label[30], char name[32]){
    struct Transaction t1;
    t1.date = d;
    t1.amount = amt;
    strcpy(t1.label, label);
    strcpy(t1.name, name);
//   return t1;
};

int ajout_transaction(FILE* fp,struct Transaction *transaction){
    // note : use fwrite() fonction

    fp = fopen( "file.txt" , "w" );
    fwrite(transaction , 1 , sizeof(transaction) , fp);

//    return *transaction;
};

FILE* creation_fichier(struct Entete, char*){


}

int main() {
    struct Date d;
    struct Date * d1 = &d;
    date(d1);
//    printf("%i, %i, %i", d.year, d.day, d.month);
    struct Entete e;
    struct Entete *e1 = &e;
    creation_entete();
    printf("%i, %i, %i", d.year, d.day, d.month);

    return 0;
}