# banC
Fonctionnement : exécuter le fichier "main.c" puis intéragir avec le menu suivant via la console.

printf("\n\nAjouter un nouveau client..............: A\n");
printf("Lister tous les comptes de clients.....: L\n");
printf("Releve d'un compte client..............: R\n");
printf("Virement depuis un compte client.......: V\n");
printf("Mise a jour du solde d'un client.......: M\n");
printf("Quitter................................: Q\n");


Les choix ci-dessus sont proposés à l'utilisateur après avoir exécuté le fichier "main.c".
Les lettres majuscules et minuscules sont acceptées.

- L'option "A" : Initialement la base de données des comptes est vide. Ajouter un premier compte client va créer le fichier "banque.dat" où seront stockés les prochains clients ajoutés. Ce fichier comporte le nom et le numéro de compte du client associé. A l'ajout d'un nouveau client un fichier personnel avec une en-tête comportant un solde et la date de la dernière mise à jour de celui-ci va être créé. Les transactions du client seront écrites dans ce même fichier ultérieurement.

- L'option "L" : Listage des comptes clients présents dans la base de données de la banque en affichant le nom du client et son numéro de compte correspondant. Les comptes sont lus dans le fichier binaire "banque.dat".

- L'option "R" : Affiche le relevé des transactions pour un mois défini et pour un client défini par l'utilisateur.

- L'option "V" : Effectue un virement à partir d'une personne emettrice vers une personne receptrice choisies par l'utilisateur. Un virement émetteur comporte une valeur négative et un récepteur positive. Le virement ne peut pas être effectué si le solde après virement de la personne émettrice est négatif. Le solde de la personne est comptabilisé comme étant le solde de l'entrée additionné aux transactions ayant été faites après la mise à jour de l'en-tête. Ce solde virtuel est ensuite comparé au montant souhaitant être transféré par l'utilisateur.

- L'option "M" : Mets à jour l'en-tête (i.e le solde et la date de la mise à jour) dans le fichier client choisi par l'utilisateur. Le solde est mis à jour grâce aux transactions faites après la date de la dernière mise à jour de l'en-tête. 
