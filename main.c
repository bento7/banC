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
struct Transaction
{
    struct Date date;
    float amount;
    char label[LENGTH_LABEL];
    char name[LENGTH_NAME]; // émetteur si transaction > 0 :réception d'€ | receveur si transaction < 0: envoi d'€
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
    printf("%i/%i/%i\n", d->day, d->month, d->year);
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

void fermer(FILE *f){
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
FILE* creation_fichier(struct Entete e, char* nom1){
    FILE *file;
    ouvrir(&file, &nom1);
    fwrite(&e, sizeof(e), 1, file);
    fermer(file);
}

int ajout_transaction(FILE* fp,struct Transaction* transaction){
    // note : use fwrite() fonction

    fp = fopen( "file.txt" , "w" );
    fwrite(transaction , sizeof(transaction) , 1 , fp);

//    return *transaction;
};





struct Transaction creation_transaction(struct Date d, float amt, char* label, char* name){
    struct Transaction t1;
    t1.date = d;
    t1.amount = amt;
    strcpy(t1.label, label);
    strcpy(t1.name, name);
    return t1;
};


int main() {
    struct Date d;
    date(&d);
    print_Date(&d);


    float montant = 100;
    char label[LENGTH_LABEL] = "Label";
    char nom[LENGTH_NAME] = "NomClt";
    struct Transaction trans1;
    trans1 = creation_transaction(d, montant, label, nom);
    printf("%f, %s, %s, ", trans1.amount, trans1.label, trans1.name);
    print_Date(&trans1.date);

    FILE *fich;
    char nom1[LENGTH_NAME] = "test1.txt";
    ouvrir(&fich, &nom1);
    fermer(fich);

    int resultat;
    FILE* fichier;
    resultat = ajout_transaction(&fichier, &trans1);
    printf("%i:", resultat);
    char nom1[LENGTH_NAME] = "compte.txt";
    struct Entete e;
    e = creation_entete(d, 54.0);
    printf("%f\n", e.solde);
    print_Date(&e.date);
    creation_fichier(e, &nom1);
    return 0;
}