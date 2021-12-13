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
    printf("Date: %i/%i/%i", d->day, d->month, d->year);
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
    fseek(file, 0, SEEK_SET);
    fwrite(&entete, sizeof(ENTETE), 1, (FILE *) file); // On écrit l'entete
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
    printf("TRANSACTION ");
    print_Date(&trans.date);
    printf("-- montant: %f , label : %s\n", trans.amount, trans.label);

}

void print_entete(ENTETE *e){
    printf("Le solde est de: %f\n",e->solde);
    print_Date(&e->date);

}

int compte_existant_num(FILE *file, int numcpt){
    ACCOUNT account;
    printf("Votre numero client est le: %i",numcpt);

    int exist = 0, end;
    while (!exist) {
        end = fread(&account, sizeof(ACCOUNT),1,file);
        if (end == 0) break;

        if (account.id== numcpt) {
            // on replace le curseur avant le compte qui existe pour le lire ensuite si besoin
            fseek(file -1l * sizeof(ACCOUNT), 1, SEEK_CUR);
            return 1;
        }
    }
    return 0;
}

int creer_utilisateur(char* nom){
    float montant;
    printf("Entrer le montant :");
    scanf("%f",&montant);

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

    entete = creation_entete(d, montant);//file en argv? car compte perso
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
    fseek(f, 0, SEEK_SET);
    fermer(f);
    return 0;
}

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

int compte_de(char *nomclt){
    /*
     * Cette fonction renvoie le numéro de compte
     * associé au nom donné en argument
    */
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

    // Récupération des numéros de compte des clients
    int nocpt_em, nocpt_rec;
    nocpt_em = compte_de(&nom_emetteur);
    nocpt_rec = compte_de(&nom_receveur);

    int res;
    do{
        printf("Entrer le montant :");
        rewind(stdin);
        scanf("%f",&montant);

        char charValue[3];
        sprintf(charValue, "%i", nocpt_em);
        char dest[7];
        char *pdest = &dest;
        strncat(dest,charValue, 3);
        strncat(dest,".dat", 7);

        // Récupération du solde en entête du client émetteur
        FILE *f;
        ouvrir(&f, dest);
        ENTETE e;
        fread(&e, sizeof(e),1,f);

        TRANSACTION t;
        //date(&e.date); // e.date = date aujourd'hui
        int res1;
        do{
            res1 = fread(&t, sizeof(t), 1, f); // Ecriture du compte dans account
            if(res1 > 0 && (comp_date(e.date,t.date) == 0)) {
                e.solde = e.solde + t.amount;
            }

        }while(res1 > 0);
        fermer(f);
        float solde = e.solde;
        // Vérification de la possibilité de virement
        res = solde - montant; // Calcul de la différence

        if(res <0) {
            printf("Virement impossible .Fonds indisponibles.\n");
        }
        // Condition d'arrêt : lorsque le virement est possible
    }while(res <0);

    // Initialisation des noms et des labels de transaction
    char label_em[LENGTH_LABEL], label_rec[LENGTH_LABEL];
    sprintf(label_em, "virement a %s - no %i", nom_receveur, nocpt_rec);
    sprintf(label_rec, "virement from %s - no%i", nom_emetteur, nocpt_em);

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

int mise_a_jour_solde(char *nom){
    int numclt = compte_de(nom);
    FILE *f;

    char charValue[3];
    sprintf(charValue, "%i", numclt);
    char dest[7];
    char *pdest = &dest;
    strncat(dest,charValue, 3);
    strncat(dest,".dat", 7);

    ouvrir(&f, dest);

    ENTETE e;
    TRANSACTION t;
    fread(&e, sizeof(e),1,f);
    //date(&e.date); // e.date = date aujourd'hui
    print_entete(&e);
    int res;
    do{
        res = fread(&t, sizeof(t), 1, f); // Ecriture du compte dans account
        if(res > 0 && (comp_date(e.date,t.date) == 0)) {
            printf("\nAffichage de la transaction\nmontant: %f , label : %s, name : %s\n", t.amount, t.label, t.name);
            e.solde = e.solde + t.amount;
            printf("Solde:%f", e.solde);
        }
    }while(res > 0);

    fseek(f, 0, SEEK_SET);
    fwrite(&e, sizeof (ENTETE), 1, f);
    fermer(f);
    return 0;
}

int imprimer_releve() {
    char nom[LENGTH_NAME];
    char* pfile_perso;
    char file_perso;
    pfile_perso = &file_perso;
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

    char filename [7];
    char str_numclt[3];
    sprintf(str_numclt, "%i", numclt); // convertir le num de compte en chaine de caractères
    strncat(filename, str_numclt,3);   // concaténer le path
    strncat(filename, ".dat",7);
    strcpy(pfile_perso, filename);

    int res;
    ouvrir(&file, pfile_perso);
    fseek(f, 0, SEEK_SET); // On se place au début du document
    fread(&e, sizeof(ENTETE),1,file);

    printf("En-tete : \n");
    print_entete(&e);

    printf("\nTransactions : \n");

    do{
        res = fread(&t, sizeof(t), 1, file); // Ecriture du compte dans account
        if(res > 0 && t.date.month == mois) print_transaction(t);
        // on évite d'imprimer en double la derniere ligne
    }while(res > 0);

    fermer(file);
    return 0;
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
                printf("Nom Client :");
                scanf("%s",&nom);
                mise_a_jour_solde(nom);
                break;
        }
    } while (choix != 'q' && choix != 'Q');
}

int main() {
    menu();
    return 0;
}