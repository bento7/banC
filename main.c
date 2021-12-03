#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define LENGTH_NAME 100
#define LENGTH_LABEL 50

// create struct Date
struct Date
{
    int day;
    int month;
    int year;
};

// create struct Transaction
typedef struct Transaction
{
    struct Date date;
    float amount;
    char label[LENGTH_LABEL];
    char name[LENGTH_NAME]; // émetteur si transaction > 0 :réception d'€ | receveur si transaction < 0: envoi d'€
}TRANSACTION;

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
    char name [LENGTH_NAME];
};

void date(struct Date *d) {
    // Utilisation du module <time.h>
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    // Récupération des informations utiles
    d->day = local->tm_mday;            // Jour (1 to 31)
    d->month = local->tm_mon + 1;      // Mois (0 to 11)
    d->year = local->tm_year + 1900;   // Année 1900
}

void print_Date(struct Date *d){
    printf("Date: %i/%i/%i\n", d->day, d->month, d->year);
}

void ouvrir(FILE **f, char nom[]) {
    //qui ouvre le fichier donné ou le crée sinon.
    *f = fopen(nom, "wb+");

    if (*f == NULL) {
        perror("Erreur à l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
}


void fermer(FILE*f){
    int res2 = 0;

    if (f != NULL) {
        fflush(f);
        res2 = (int) fclose(f);
        printf("Fermerture:%i\n", res2);
    }
}

struct Entete creation_entete(struct Date d, float solde){
    struct Entete e1;
    e1.date = d;
    e1.solde = solde;
    return e1;
};

FILE* creation_fichier(struct Entete, char*){
}

struct Transaction creation_transaction(struct Date d, float amt, char* label, char* name){
    struct Transaction t1;
    t1.date = d;
    t1.amount = amt;
    strcpy(t1.label, label);
    strcpy(t1.name, name);
    return t1;
};

int ajout_transaction(FILE* filepath, TRANSACTION* transaction){
    int res = 0;
    ouvrir(&filepath, "test.dat");        // Ouverture du fichier
    fseek(filepath, 0, SEEK_END);         // On se place à la fin du fichier
    res = (int) fwrite(transaction, sizeof(TRANSACTION), 1, (FILE *) filepath); // On écrit la dernière transaction et on récupère l'entier
    fermer(filepath);// On ferme le fichier
    printf("Res1:%i\n", res);

    return res;                                     // On retourne le résultat de fwrite
};


struct Transaction lire_transaction(FILE* fp){
    struct Transaction trans;
    FILE* ftoread = fopen("test.dat", "rb+");

    fread(&trans, sizeof(trans), 1, ftoread);
    // faire un close
    return trans;
}

void print_transaction(TRANSACTION trans){
    printf("\nAffichage de la transaction\nmontant: %f , label : %s, name : %s\n", trans.amount, trans.label, trans.name);
    print_Date(&trans.date);
}


int main() {
    struct Date d;
    date(&d);
    print_Date(&d);

    float montant = 100;
    const char label[LENGTH_LABEL] = "Label";
    const char nom[LENGTH_NAME] = "NomClt";
    TRANSACTION trans1;
    trans1 = creation_transaction(d, montant, &label, &nom);
    printf("%f, %s, %s, ", trans1.amount, trans1.label, trans1.name);
    print_Date(&trans1.date);

    int resultat;
    FILE* fichier;
    resultat = ajout_transaction((FILE *) &fichier, &trans1);
    printf("resultat: %i\n", resultat);

    TRANSACTION trans2;
    trans2 = lire_transaction((FILE *) &fichier);
    print_transaction(trans2);

    return 0;
}