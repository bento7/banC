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

int comp_date(struct Date d1, struct Date d2){

    //Convertion en nombre de jours pour faciliter la comparaison en comparaison de deux entiers
    int nb1 ,nb2;
    nb1 = 365 * d1.year + 30 * d1.month + d1.day;
    nb2 = 365 * d2.year + 30 * d2.month + d2.day;

    if (nb1 > nb2) return 1;
    if (nb1 <= nb2) return 0;

}

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
//    printf( "Le solde du compte est %f a la date suivante %i/%i/%i\n", entete.solde, entete.date.day,entete.date.month,entete.date.year);
    fprintf(file, "%i %i %i %f\n", entete.date.day,entete.date.month,entete.date.year,entete.solde);
//    fwrite(e, sizeof(TRANSACTION), 1, (FILE *) file); // On écrit l'entete
//    fread(&entete, sizeof(entete), 1, file);
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
//    ouvrir(&filepath, "compte.dat");        // Ouverture du fichier
    fseek(filepath, 0, SEEK_END);         // On se place à la fin du fichier
    res = (int) fwrite(transaction, sizeof(TRANSACTION), 1, (FILE *) filepath); // On écrit la dernière transaction et on récupère l'entier
//    fermer(filepath);// On ferme le fichier

    return res;                                     // On retourne le résultat de fwrite
};


struct Transaction lire_transaction(FILE* fp){
    struct Transaction trans;
    FILE* ftoread = fopen(fp, "rb+");

    fread(&trans, sizeof(trans), 1, ftoread);
    fermer(ftoread);
    // faire un close
    return trans;
}
int lire_entete(FILE* fp, ENTETE *e){

    int jour, mois, annee;
    float s;
    if(fscanf(fp, "%i %i %i %f\n", &jour, &mois, &annee, &s) != 4) {
        printf("yoo");
        return 1;
    }
    e->date.day = jour;
    e->date.month = mois;
    e->date.year = annee;
    e->solde = s;

    printf("le jour %i, mois %i, annee %i sont ",jour,mois,annee);
    return 0;
}
void print_transaction(TRANSACTION trans){
    printf("\nAffichage de la transaction\nmontant: %f , label : %s, name : %s\n", trans.amount, trans.label, trans.name);
    print_Date(&trans.date);
}

void print_entete(ENTETE e){
    printf("\nSolde est %f\n",e.solde);
    print_Date(&e.date);

}


void mise_a_jour(FILE* f, struct Date date){
    ENTETE e_anc;
    ouvrir(&f, "compte.dat");

//    fread(&e_anc, sizeof(ENTETE), 1, f);
//    e_anc = lire_entete(f);

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
int compte_existant_num(FILE *file, int numcpt){
    ACCOUNT account;
    printf("%i",numcpt);

    int exist = 0, end;
    while (!exist) {
        end = fread(&account, sizeof(ACCOUNT),1,file);
        if (end == 0) break;
        printf("%i",&account.id);

        if (account.id== numcpt) {
            // on replace le curseur avant le compte qui existe pour le lire ensuite si besoin
            fseek(file -1l * sizeof(ACCOUNT), 1, SEEK_CUR);
            return 1;
        }
    }
    return 0;
}

int compte_existant_char(FILE *file, int numcpt){
    ACCOUNT account;
    int exist = 0, end;
    while (!exist) {
        end = fread(&account, sizeof(ACCOUNT),1,file);
        if (end == 1) break;
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
    FILE* f;
    ouvrir(&f,"banque.dat");

    srand(time(NULL));
    int num = rand(), inc = 0;
    num = num % 1000;

    while (!inc){
        if (!compte_existant_num(f, num)) break;
        num = rand();
    }
    account.id = num;

    strcpy(account.name, nom);
    //création du fichier de compte personnel
    struct Date d;
    date(&d);
    ENTETE entete;
    entete = creation_entete(d, 54);//file en argv? car compte perso

    char charValue[3];
    sprintf(charValue, "%i", account.id);
    char dest[7];
    char *pdest = &dest;
    strncat(dest,charValue, 3);
    strncat(dest,".dat", 7);

    creation_fichier(entete,pdest);

    // Mise à jour du fichier banque
    fseek(f, 0, SEEK_END);
    fwrite(&account, sizeof(ACCOUNT), 1, f);
    fermer(f);
    return 0;
}

int test() {
    FILE *file;
    ouvrir(&file, "banque.dat");
    struct Date d;
    date(&d);
    print_Date(&d);

    // On crée le fichier compte.dat, une entete avec un solde à 54
    // On ajoute une transactionde 100
    // On met à jour le fichier
//    FILE* file;
//    ouvrir(&file, "compte.dat");

    char nom1[LENGTH_NAME] = "compte.dat";
    ENTETE e;
    float solde_bento = 54;
    e = creation_entete(d, solde_bento);
//    printf("%f\n", e.solde);
//    print_Date(&e.date);
    creation_fichier(e, &nom1);
//    ouvrir(&file, "compte.dat");

    float montant = 100;
    const char label[LENGTH_LABEL] = "Label";
    const char nom[LENGTH_NAME] = "NomClt";
    TRANSACTION trans1;
    trans1 = creation_transaction(d, montant, &label, &nom);
    printf("%f, %s, %s, ", trans1.amount, trans1.label, trans1.name);
    print_Date(&trans1.date);

    int resultat;

    resultat = ajout_transaction((FILE *) file, &trans1);
    printf("resultat: %i\n", resultat);

//    TRANSACTION trans2;
//    trans2 = lire_transaction(file);
//    print_transaction(trans2);


    mise_a_jour(file, d);
    fermer(file);
    ENTETE be;
    FILE* f;
    ouvrir(&f, "compte.dat");
    lire_entete(file, &e);
    print_entete(be);
    return 0;
}
// //////////////////////////////// //
//                  TOM             //
// //////////////////////////////// //

void read_Banque(){
    /*
     * Cette Fonction a pour but de lister les comptes clients
     * */
    int res;
    FILE *f;
    ACCOUNT account;

    printf("Liste des comptes clients :\n");

    ouvrir(&f, "banque.dat");
    fseek(f, 0, SEEK_SET); // On se place au début du document
    do{
        res = fread(&account, sizeof(ACCOUNT), 1, f); // Ecriture du compte dans account
        if(res > 0) printf("N0 COMPTE : %i || NOM CLIENT : %s\n", account.id, account.name); // on évite d'imprimer en double la derniere ligne
    }while(res > 0);
    fermer(f);
}

// Renvoie le numéro de compte associé à ce nom.
int compte_de(char *nomclt){
    ACCOUNT account;

    FILE *f;
    ouvrir(&f, "banque.dat");

    int res;
    do{
        res = fread(&account, sizeof(ACCOUNT), 1, f); // Ecriture du compte dans account
        if (strcmp(nomclt, account.name) == 0){
            return account.id;
        }
    }while(res > 0);
    return -1; // pas de compte
}

virement_de_a(){
    // Récupération des informations nécessaires
    char nom_emetteur[LENGTH_NAME];
    char nom_receveur[LENGTH_NAME];
    float montant;

    printf("Quel le nom du client emetteur? :");
    scanf("%s",&nom_emetteur);
    printf("Quel est le nom du client receveur? :");
    scanf("%s",&nom_receveur);
    printf("Entrer le montant :");
    scanf("%f",&montant);

    // Récupération des numéros de compte des clients
    int nocpt_em, nocpt_rec;
    nocpt_em = compte_de(&nom_emetteur);
    nocpt_rec = compte_de(&nom_receveur);

    // Initialisation des noms et des labels
    char label_em[LENGTH_LABEL], label_rec[LENGTH_LABEL];

    sprintf(label_em, "virement to %i", nocpt_rec);
    sprintf(label_rec, "virement from %i", nocpt_em);

    // création des transactions
    struct Transaction trans_em, trans_rec;
    struct Date d;
    date(&d);

    trans_em = creation_transaction(d, - 1 * montant, label_em, nom_emetteur);
    trans_rec = creation_transaction(d, montant, label_rec, nom_receveur);

    print_transaction(trans_em);
    print_transaction(trans_rec);
    // Ecriture dans le compte de l'emetteur
    FILE *f;

    char charValue[3];
    sprintf(charValue, "%i", nocpt_em);
    char dest[7];
    char *pdest = &dest;
    strncat(dest,charValue, 3);
    strncat(dest,".dat", 7);

    ouvrir(&f, dest);
    ajout_transaction(f, &trans_em);
    fermer(f);

    // Ecriture sur le compte du receveur
    FILE *frec;

    char charValuer[3];
    sprintf(charValuer, "%i", nocpt_rec);
    char destr[7];
    char *pdestr = &destr;
    strncat(destr,charValuer, 3);
    strncat(destr,".dat", 7);

    ouvrir(&frec, destr);
    ajout_transaction(frec, &trans_rec);
    fermer(frec);

    return 0;
}

int nom_compte(int num_compte, char* nom){
    char filename [7];
    char str_num_compte[3];
    sprintf(str_num_compte, "%i", num_compte); // convertir le num de compte en chaine de caractères
    strncat(filename, str_num_compte,3);   // concaténer le path
    strncat(filename, ".dat",7);
    strcpy(nom, filename);

    //    char charValue[3];
//    sprintf(charValue, "%i", numclt);
//    char dest[7];
//    char *pdest = &dest;
//    strncat(dest, charValue, 3);
//    strncat(dest, ".dat", 7);
    return 0;
}

int imprimer_releve() {
    char nom[LENGTH_NAME];
    char file_perso[LENGTH_NAME];
    int exist = 0, mois = 0, numclt;

    ACCOUNT account;
    FILE *f;
    ouvrir(&f, "banque.dat");

    printf("Vous souhaitez acceder au releve de compte.\n");

    while (exist == 0) {
        printf("Entrez le nom du client : \n");
        scanf("%s", &nom);
        numclt = compte_de(&nom);
        if ((compte_de(&nom) < 0 )) {
            printf("Aucun compte existant a ce nom. Veuillez rentrer un nom valide. \n");
        } else {
            exist = 1;
            printf("Compte existant. \n");
        }
    }
    fermer(f);

    while (mois < 1 || mois > 12) {
        printf("Veuillez entrer un mois par son numero (1 pour janvier et 12 pour decembre)\n");
        scanf("%i", &mois);
    }

    printf("Voici votre releve de compte : \n");
    printf("Nom : %s ; Numero de compte : %i \n", &nom, numclt);

    ENTETE e;
    TRANSACTION t;
    FILE *file;


    nom_compte(numclt, &file_perso);
    char charValue[3];
    sprintf(charValue, "%i", numclt);
    printf("charvalue %s\n",charValue);
    char dest[7];
    char *pdest = &dest;
    strncat(dest, charValue, 3);
    strncat(dest, ".dat", 7);
    printf("le nom du fichier est %s\n",dest);
    ouvrir(&file, dest);
    int res;
    fseek(f, 0, SEEK_SET); // On se place au début du document

//    lire_entete(file, &e);

    printf("En-tete : \n");
    fread(&e, sizeof(e),1,file);
    printf("Date: %i / %i / %i Solde : %f \n", e.date.day, e.date.month, e.date.year, e.solde);

    printf(("Transactions : \n"));


    do{
        res = fread(&t, sizeof(t), 1, file); // Ecriture du compte dans account
        if(res > 0) printf("\nAffichage de la transaction\nmontant: %f , label : %s, name : %s\n", t.amount, t.label, t.name);
        // on évite d'imprimer en double la derniere ligne
    }while(res > 0);
    fermer(file);
    return 1;
}
void test_test(){
    FILE * f;
    ENTETE e;
    ouvrir(&f, "681.dat");
    lire_entete(f, &e);
    fermer(f);
    printf("En-tete : \n");
    printf("Date: %i / %i / %i Solde : %f \n", e.date.day, e.date.month, e.date.year, e.solde);

}
void menu()
{
    char choix;
    char nom[LENGTH_NAME];

    do {
        printf("\n\nAjouter un nouveau client..............: A\n");
        printf("Lister tous les comptes de clients.....: L\n");
        printf("Releve d'un compte client..............: R\n");
        printf("Virement depuis un compte client.......: V\n");
        printf("Mise a jour du solde d'un client.......: M\n");
        printf("Quitter................................: Q\n");
        printf(" votre choix: ");
        rewind(stdin);
        scanf("%c",&choix);
        switch(choix)
        {
            case 'a':
            case 'A':
                printf("Nom Client :");
                scanf("%s",&nom);
                creer_utilisateur(&nom);
                break;
            case 'l':
            case 'L':
                read_Banque();
                break;
            case 'r':
            case 'R' :
                imprimer_releve();
                break;
            case 'v':
            case 'V':
                virement_de_a();
                break;
            case 'm':
            case 'M':

                test();
                break;
        }
    } while (choix != 'q' && choix != 'Q');
}

int main() {
    menu();
    return 0;
}