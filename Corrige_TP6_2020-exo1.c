TP C N�6CorrectionsExercice N�1#include <stdio.h>#include <string.h>typedef struct{  char nom[10];  char prenom[10];  int tel;} personne;void ajout_personne (FILE *rep);void affiche_ensemble_personne(FILE *rep);void trouve_numero_personne(FILE *rep, char *nom);void menu(FILE *rep);void ajout_personne (FILE *rep) {  personne pers;  printf ("nom : ");  scanf ("%s", pers.nom);  printf ("prenom : ");  scanf ("%s", pers.prenom);  printf ("tel : ");  scanf ("%d", &pers.tel);  fwrite (&pers, sizeof(pers), 1, rep);}void affiche_ensemble_personne(FILE *rep) {   personne pers;   fseek(rep,0,SEEK_SET);   while (fread(&pers,sizeof(personne),1,rep) != 0) {     printf("nom .....: %s\n",pers.nom);     printf("prenom ..: %s\n",pers.prenom);     printf("tel .....: %d\n\n",pers.tel);   }}void trouve_numero_personne(FILE *rep, char *nom) {  personne pers;  int trouve = 1;    printf("Donnez le nom : ");  scanf("%s",nom);  fseek(rep, 0, SEEK_SET);  while (fread(&pers, sizeof(personne), 1, rep) != 0 && trouve != 0) {    if ((trouve = strcmp(pers.nom, nom)) == 0)      printf("Tel de %s %s : %d\n\n",pers.prenom, pers.nom, pers.tel);  }  if (trouve != 0) {    printf("Ce nom n'existe pas\n");  }}  void menu(FILE *rep) {  char reponse, nom[10];  do {     printf("AJOUTER.... : A\n");    printf("LISTER..... : L\n");    printf("CHERCHER....: T\n");    printf("QUITER......: Q\n");    printf("  votre choix: ");    scanf(" %c",&reponse);    switch(reponse) {    case 'a':case 'A': ajout_personne (rep);      break;    case 'l':case 'L': affiche_ensemble_personne(rep);      break;    case 't':case 'T': trouve_numero_personne(rep, nom);    }  }  while( reponse != 'q' && reponse != 'Q');}int main () {  FILE *repertoire;        repertoire = fopen ("reper.dat", "a+");     menu(repertoire);  fclose (repertoire);  return 0;}