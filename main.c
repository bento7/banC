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
typedef struct Entete
{
    struct Date date;
    float solde;
}ENTETE;

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
    *f = fopen(nom, "w");
    if (*f == NULL) {
        *f = fopen(nom, "w");
        if (*f == NULL) {
            perror("Erreur à l'ouverture du fichier");
            exit(EXIT_FAILURE);
        }
    }
}

void fermer(FILE *f){
    if (f != NULL) {
        fflush(f);
        fclose(f);
    }
}

struct Entete creation_entete(struct Date d, float solde){
    struct Entete e1;
    e1.date = d;
    e1.solde = solde;
    return e1;
};

FILE* creation_fichier(ENTETE entete, char* nom1){
    FILE* file;
    ouvrir(&file, nom1);
    ENTETE *e = &entete;
    fprintf(file, "Le solde du compte est %f € à la date suivante %i/%i/%i\n", entete.solde, e->date.day,e->date.month,e->date.year);
    fermer(file);
    return file;
}


int ajout_transaction(FILE* fp, TRANSACTION* transaction){
    int res;
    ouvrir(fp, "test.txt");        // Ouverture du fichier
    fseek(fp, 0, SEEK_END);         // On se place à la fin du fichier
    fwrite(transaction, sizeof(TRANSACTION), 1, fp); // On écrit la dernière transaction et on récupère l'entier
    fermer(fp);                                   // On ferme le fichier
    return res;                                     // On retourne le résultat de fwrite
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





    const char label[LENGTH_LABEL] = "Label";
    const char nom[LENGTH_NAME] = "NomClt";
    TRANSACTION trans1;
    trans1 = creation_transaction(d, montant, &label, &nom);
    printf("%f, %s, %s, ", trans1.amount, trans1.label, trans1.name);
    print_Date(&trans1.date);

//    int resultat;
//    FILE* fichier;
//    resultat = ajout_transaction(&fichier, &trans1);
//    printf("%i:", resultat);

    char nom1[LENGTH_NAME] = "compte_bento.txt";
    ENTETE e;
    float solde_bento = 54;
    e = creation_entete(d, solde_bento);
    printf("%f\n", e.solde);
    print_Date(&e.date);
    creation_fichier(e, &nom1);


    //struct Entete e;
    //struct Entete *e1 = &e;
    //creation_entete();
    return 0;
}