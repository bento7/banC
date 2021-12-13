# banC
Fonctionnement : ex�cuter le fichier "main.c" puis int�ragir avec le menu suivant via la console.

printf("\n\nAjouter un nouveau client..............: A\n");
printf("Lister tous les comptes de clients.....: L\n");
printf("Releve d'un compte client..............: R\n");
printf("Virement depuis un compte client.......: V\n");
printf("Mise a jour du solde d'un client.......: M\n");
printf("Quitter................................: Q\n");


Les choix ci-dessus sont propos�s � l'utilisateur apr�s avoir ex�cut� le fichier "main.c".
Les lettres majuscules et minuscules sont accept�es.

- L'option "A" : Initialement la base de donn�es des comptes est vide. Ajouter un premier compte client va cr�er le fichier "banque.dat" o� seront stock�s les prochains clients ajout�s. Ce fichier comporte le nom et le num�ro de compte du client associ�. A l'ajout d'un nouveau client un fichier personnel avec une en-t�te comportant un solde et la date de la derni�re mise � jour de celui-ci va �tre cr��. Les transactions du client seront �crites dans ce m�me fichier ult�rieurement.

- L'option "L" : Listage des comptes clients pr�sents dans la base de donn�es de la banque en affichant le nom du client et son num�ro de compte correspondant. Les comptes sont lus dans le fichier binaire "banque.dat".

- L'option "R" : Affiche le relev� des transactions pour un mois d�fini et pour un client d�fini par l'utilisateur.

- L'option "V" : Effectue un virement � partir d'une personne emettrice vers une personne receptrice choisies par l'utilisateur. Un virement �metteur comporte une valeur n�gative et un r�cepteur positive. Le virement ne peut pas �tre effectu� si le solde apr�s virement de la personne �mettrice est n�gatif. Le solde de la personne est comptabilis� comme �tant le solde de l'entr�e additionn� aux transactions ayant �t� faites apr�s la mise � jour de l'en-t�te. Ce solde virtuel est ensuite compar� au montant souhaitant �tre transf�r� par l'utilisateur.

- L'option "M" : Mets � jour l'en-t�te (i.e le solde et la date de la mise � jour) dans le fichier client choisi par l'utilisateur. Le solde est mis � jour gr�ce aux transactions faites apr�s la date de la derni�re mise � jour de l'en-t�te. 
