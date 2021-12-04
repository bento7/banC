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
typedef struct Account
{
    int id;
    char name[LENGTH_NAME];
}ACCOUNT;

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
    *f = fopen(nom, "r+");
    if (*f == NULL) {
        *f = fopen(nom, "w+");
        if (*f == NULL) {
            perror("Erreur d'ouverture du fichier");
            exit(EXIT_FAILURE);
        }
    }
}


void fermer(FILE*f){
    int res2 = 0;

    if (f != NULL) {
        fflush(f);
        fclose(f);
//        printf("Fermerture:%i\n", res2);
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
//    fprintf(file, "Le solde du compte est %f € à la date suivante %i/%i/%i\n", entete.solde, e->date.day,e->date.month,e->date.year);
    fwrite(e, sizeof(TRANSACTION), 1, (FILE *) file); // On écrit l'entete
    fread(&entete, sizeof(entete), 1, file);
    printf("yo");
    fermer(file);
    return file;
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
    ouvrir(&filepath, "compte.dat");        // Ouverture du fichier
    fseek(filepath, 0, SEEK_END);         // On se place à la fin du fichier
    res = (int) fwrite(transaction, sizeof(TRANSACTION), 1, (FILE *) filepath); // On écrit la dernière transaction et on récupère l'entier
    fermer(filepath);// On ferme le fichier
    printf("Res1:%i\n", res);

    return res;                                     // On retourne le résultat de fwrite
};


struct Transaction lire_transaction(FILE* fp){
    struct Transaction trans;
    FILE* ftoread = fopen("compte.dat", "rb+");

    fread(&trans, sizeof(trans), 1, ftoread);
    fermer(ftoread);
    // faire un close
    return trans;
}
struct Entete lire_entete(FILE* fp){
    ENTETE entete;
    FILE* ftoread = fopen("compte.dat", "rb+");
    fseek(ftoread, 0, SEEK_SET);
    fread(&entete, sizeof(entete), 1, ftoread);
    // faire un close
    return entete;
}
void print_transaction(TRANSACTION trans){
    printf("\nAffichage de la transaction\nmontant: %f , label : %s, name : %s\n", trans.amount, trans.label, trans.name);
    print_Date(&trans.date);
}

void print_entete(ENTETE e){
    printf("\nSolde est %f\n",e.solde);
//    ENTETE *entete =&e;
//    print_Date(entete.date);

}



void mise_a_jour(FILE* f, struct Date date){
    ENTETE e_anc;

//    fread(&e_anc, sizeof(ENTETE), 1, f);
    e_anc = lire_entete(f);

    print_entete(e_anc);
    // placement pointeur pour lire la dernière transaction
    fseek(f,-1l *sizeof(TRANSACTION), SEEK_END);
    TRANSACTION trans;
    trans = lire_transaction(f);
    fread(&trans, sizeof(TRANSACTION), 1, f);
    ENTETE e_nouv;
//    print_entete(e_nouv);
    e_nouv.solde = e_anc.solde + trans.amount;

    print_transaction(trans);
    e_nouv.date = date;
    //réécriture de l'entete avec le nouveau solde et la date
    rewind(f);
    fwrite(&e_nouv, sizeof(ENTETE),1, f);
    fseek(f, 0, SEEK_SET);
    fermer(f);

}
int compte_existant(FILE *file, int numcpt){
    ACCOUNT account;
    int exist = 0, end;
    while (!exist) {
        end = fread(&account, sizeof(ACCOUNT),1,file);
        if (end == 0) break;
        if (strcmp(account.id, numcpt) == 0) {
            // on replace le curseur avant le compte qui existe pour le lire ensuite si besoin
            fseek(file -1l * sizeof(ACCOUNT), 1, SEEK_CUR);
            return 1;
        }
    }
    return 0;
}

int creer_utilisateur(char* nom){
    ACCOUNT account;
    strcpy(account.name, nom);
    //création du fihcier de compte personnel


    // mise à jour du répertoire de la banque
    FILE* rep;
    int num = rand(), inc = 0;
    ouvrir(&rep,"banque.dat");
    while (!inc){
        if (!compte_existant(rep, num)) break;
        num = rand();
    }
    account.id = num;
    fseek(rep, 0, SEEK_END);
    fwrite(&account, sizeof(ACCOUNT), 1, rep);
    fermer(rep);
}


int main() {
    struct Date d;
    date(&d);
    print_Date(&d);

    // On crée le fichier compte.dat, une entete avec un solde à 54
    // On ajoute une transactionde 100
    // On met à jour le fichier
    FILE* file;

    char nom1[LENGTH_NAME] = "compte.dat";
    ENTETE e;
    float solde_bento = 54;
    e = creation_entete(d, solde_bento);
//    printf("%f\n", e.solde);
//    print_Date(&e.date);
    creation_fichier(e, &nom1);
    ouvrir(&file, "compte.dat");

    float montant = 100;
    const char label[LENGTH_LABEL] = "Label";
    const char nom[LENGTH_NAME] = "NomClt";
    TRANSACTION trans1;
    trans1 = creation_transaction(d, montant, &label, &nom);
    printf("%f, %s, %s, ", trans1.amount, trans1.label, trans1.name);
    print_Date(&trans1.date);

    int resultat;

    resultat = ajout_transaction((FILE *) &file, &trans1);
    printf("resultat: %i\n", resultat);

//    TRANSACTION trans2;
//    trans2 = lire_transaction(file);
//    print_transaction(trans2);


    mise_a_jour(file, d);
    fermer(file);
    ENTETE be;
    be = lire_entete(file);
    print_entete(be);
    return 0;
}