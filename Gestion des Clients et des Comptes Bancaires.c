#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// ANSI Color Codes
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"

// Constants
#define MAXCLIENTS 100
#define MAXCOMPTES 200
#define SOLDEMINIMUM 1000.0
#define RETRAITMAX 700.0

// Data Structures
typedef struct {
    int Idclient;
    char nom[50];
    char prenom[50];
    char profession[50];
    char Numtel[15];
} Client;

typedef struct {
    int Idcompte;
    int Idclient;
    float Soldebase;
    char Dateouverture[11]; // "JJ/MM/AAAA"
} Compte;

// Global Data
Client clients[MAXCLIENTS];
int nbclients = 0;

Compte comptes[MAXCOMPTES];
int nbcomptes = 0;

// Utility & UI Function Prototypes
void viderBuffer();
void afficherEntete(const char* titre);
void afficherLigne(char c, int longueur);
void afficherMessageSucces(const char* message);
void afficherMessageErreur(const char* message);
void afficherMessageInfo(const char* message);
void pause();
void clearScreen();
void obtenirDateActuelle(char date[]);
int lireIdValide(const char* message);

// Menu Function Prototypes
void afficherMenuPrincipal();
void menuGestionClients();
void menuGestionComptes();
void menuGestionOperations();

// Client Management Function Prototypes
void ajouterClient();
void modifierClient();
void supprimerClient();
void rechercherClient();
void afficherTousLesClients();
int trouverClientParId(int id);
int trouverClientParNom(char nom[]);

// Account Management Function Prototypes
void nouveauCompte();
void consulterComptes();
void fermerCompte();
int trouverCompteParId(int id);

// Operations Function Prototypes
void effectuerRetrait();
void effectuerVirement();


int main() {
    char choix;
    do {
        afficherMenuPrincipal();
        scanf(" %c", &choix);
        viderBuffer();
        choix = toupper(choix);

        switch (choix) {
            case 'A':
                menuGestionClients();
                break;
            case 'B':
                menuGestionComptes();
                break;
            case 'C':
                menuGestionOperations();
                break;
            case 'D':
                afficherMessageInfo("Merci d'avoir utilise notre application. A bientot !");
                break;
            default:
                afficherMessageErreur("Choix invalide. Veuillez reessayer.");
                pause();
                break;
        }
    } while (choix != 'D');

    return 0;
}

// --- Utility & UI Functions ---

void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    printf("\nAppuyez sur " COLOR_YELLOW "Entree" COLOR_RESET " pour continuer...");
    getchar();
}

void afficherEntete(const char* titre) {
    clearScreen();
    afficherLigne('=', 60);
    printf(COLOR_CYAN "         %s\n" COLOR_RESET, titre);
    afficherLigne('=', 60);
    printf("\n");
}

void afficherLigne(char c, int longueur) {
    for (int i = 0; i < longueur; i++) {
        putchar(c);
    }
    printf("\n");
}

void afficherMessageSucces(const char* message) {
    printf(COLOR_GREEN "[SUCCES] %s\n" COLOR_RESET, message);
}

void afficherMessageErreur(const char* message) {
    printf(COLOR_RED "[ERREUR] %s\n" COLOR_RESET, message);
}

void afficherMessageInfo(const char* message) {
    printf(COLOR_YELLOW "[INFO] %s\n" COLOR_RESET, message);
}

void obtenirDateActuelle(char date[]) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(date, 11, "%d/%m/%Y", t);
}

int lireIdValide(const char* message) {
    char buffer[50];
    int id;
    int valide;
    
    do {
        valide = 1;
        printf("%s", message);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        
        // Vérifier que la chaîne n'est pas vide
        if (strlen(buffer) == 0) {
            printf(COLOR_RED "Erreur : l'ID ne peut pas etre vide.\n" COLOR_RESET);
            valide = 0;
            continue;
        }
        
        // Vérifier que tous les caractères sont des chiffres
        for (int i = 0; buffer[i]; i++) {
            if (!isdigit(buffer[i])) {
                printf(COLOR_RED "Erreur : l'ID doit contenir uniquement des chiffres.\n" COLOR_RESET);
                valide = 0;
                break;
            }
        }
        
        if (valide) {
            id = atoi(buffer);
            if (id <= 0) {
                printf(COLOR_RED "Erreur : l'ID doit etre un nombre positif.\n" COLOR_RESET);
                valide = 0;
            }
        }
    } while (!valide);
    
    return id;
}

// --- Menu Functions ---

void afficherMenuPrincipal() {
    afficherEntete("GESTION DES CLIENTS ET DES COMPTES BANCAIRES");
    printf(COLOR_MAGENTA "  MENU PRINCIPAL\n\n" COLOR_RESET);
    printf("  " COLOR_CYAN "A)" COLOR_RESET " Gestion des clients\n");
    printf("  " COLOR_CYAN "B)" COLOR_RESET " Gestion des comptes\n");
    printf("  " COLOR_CYAN "C)" COLOR_RESET " Gestion des operations\n");
    printf("  " COLOR_CYAN "D)" COLOR_RESET " Quitter\n\n");
    afficherLigne('-', 60);
    printf("Votre choix : ");
}

void menuGestionClients() {
    char choix;
    do {
        afficherEntete("GESTION DES CLIENTS");
        printf("  1. Ajouter un client\n");
        printf("  2. Modifier un client\n");
        printf("  3. Supprimer un client\n");
        printf("  4. Rechercher un client\n");
        printf("  5. Afficher tous les clients\n");
        printf("  6. Retour au menu principal\n\n");
        afficherLigne('-', 60);
        printf("Votre choix : ");
        scanf(" %c", &choix);
        viderBuffer();

        switch (choix) {
            case '1': ajouterClient(); break;
            case '2': modifierClient(); break;
            case '3': supprimerClient(); break;
            case '4': rechercherClient(); break;
            case '5': afficherTousLesClients(); break;
            case '6': break;
            default:
                afficherMessageErreur("Choix invalide.");
                break;
        }
        if (choix != '6') pause();
    } while (choix != '6');
}

void menuGestionComptes() {
    char choix;
    do {
        afficherEntete("GESTION DES COMPTES");
        printf("  1. Creer un nouveau compte\n");
        printf("  2. Consulter les comptes\n");
        printf("  3. Fermer un compte\n");
        printf("  4. Retour au menu principal\n\n");
        afficherLigne('-', 60);
        printf("Votre choix : ");
        scanf(" %c", &choix);
        viderBuffer();

        switch (choix) {
            case '1': nouveauCompte(); break;
            case '2': consulterComptes(); break;
            case '3': fermerCompte(); break;
            case '4': break;
            default:
                afficherMessageErreur("Choix invalide.");
                break;
        }
        if (choix != '4') pause();
    } while (choix != '4');
}

void menuGestionOperations() {
    char choix;
    do {
        afficherEntete("GESTION DES OPERATIONS");
        printf("  1. Effectuer un retrait\n");
        printf("  2. Effectuer un virement\n");
        printf("  3. Retour au menu principal\n\n");
        afficherLigne('-', 60);
        printf("Votre choix : ");
        scanf(" %c", &choix);
        viderBuffer();

        switch (choix) {
            case '1': effectuerRetrait(); break;
            case '2': effectuerVirement(); break;
            case '3': break;
            default:
                afficherMessageErreur("Choix invalide.");
                break;
        }
        if (choix != '3') pause();
    } while (choix != '3');
}

// --- Client Management Functions ---

int trouverClientParId(int id) {
    for (int i = 0; i < nbclients; i++) {
        if (clients[i].Idclient == id) {
            return i; // Found
        }
    }
    return -1; // Not found
}

int trouverClientParNom(char nom[]) {
    for (int i = 0; i < nbclients; i++) {
        if (strcmp(clients[i].nom, nom) == 0) {
            return i; // Found
        }
    }
    return -1; // Not found
}

void ajouterClient() {
    afficherEntete("AJOUTER UN NOUVEAU CLIENT");
    if (nbclients >= MAXCLIENTS) {
        afficherMessageErreur("Nombre maximum de clients atteint.");
        return;
    }

    Client c;
    int valide;
    
    c.Idclient = lireIdValide("ID du client : ");

    if (trouverClientParId(c.Idclient) != -1) {
        afficherMessageErreur("Un client avec cet ID existe deja.");
        return;
    }

    /* NOM */
    do {
        valide = 1;
        printf("Nom : ");
        fgets(c.nom, sizeof(c.nom), stdin);
        c.nom[strcspn(c.nom, "\n")] = '\0';

        if (strlen(c.nom) == 0) valide = 0;

        for (int i = 0; c.nom[i]; i++) {
            if (!isalpha(c.nom[i]) && c.nom[i] != ' ') {
                valide = 0;
                break;
            }
        }

        if (!valide)
            printf(COLOR_RED "Erreur : nom invalide (lettres uniquement).\n" COLOR_RESET);
    } while (!valide);

    /* PRENOM */
    do {
        valide = 1;
        printf("Prenom : ");
        fgets(c.prenom, sizeof(c.prenom), stdin);
        c.prenom[strcspn(c.prenom, "\n")] = '\0';

        if (strlen(c.prenom) == 0) valide = 0;

        for (int i = 0; c.prenom[i]; i++) {
            if (!isalpha(c.prenom[i]) && c.prenom[i] != ' ') {
                valide = 0;
                break;
            }
        }

        if (!valide)
            printf(COLOR_RED "Erreur : prenom invalide (lettres uniquement).\n" COLOR_RESET);
    } while (!valide);

    /* PROFESSION */
    do {
        valide = 1;
        printf("Profession : ");
        fgets(c.profession, sizeof(c.profession), stdin);
        c.profession[strcspn(c.profession, "\n")] = '\0';

        if (strlen(c.profession) == 0) valide = 0;

        for (int i = 0; c.profession[i]; i++) {
            if (!isalpha(c.profession[i]) && c.profession[i] != ' ') {
                valide = 0;
                break;
            }
        }

        if (!valide)
            printf(COLOR_RED "Erreur : profession invalide (lettres uniquement).\n" COLOR_RESET);
    } while (!valide);

    // Validation du numéro de téléphone
    do {
        printf("Numero de telephone : ");
        fgets(c.Numtel, sizeof(c.Numtel), stdin);
        c.Numtel[strcspn(c.Numtel, "\n")] = '\0';

        if (strlen(c.Numtel) != 10 ||
            c.Numtel[0] != '0' ||
            (c.Numtel[1] != '6' && c.Numtel[1] != '7')) {
            printf(COLOR_RED " Numero invalide ! (ex: 06XXXXXXXX ou 07XXXXXXXX)\n" COLOR_RESET);
        }

    } while (strlen(c.Numtel) != 10 ||
             c.Numtel[0] != '0' ||
             (c.Numtel[1] != '6' && c.Numtel[1] != '7'));

    clients[nbclients++] = c;
    afficherMessageSucces("Client ajoute avec succes !");
}

void modifierClient() {
    afficherEntete("MODIFIER UN CLIENT");
    int id, index;
    int valide;  // CORRECTION: Déclaration de la variable valide
    
    id = lireIdValide("Entrez l'ID du client a modifier : ");

    index = trouverClientParId(id);
    if (index == -1) {
        afficherMessageErreur("Aucun client trouve avec cet ID.");
        return;
    }

    printf("\nInformations actuelles du client :\n");
    printf("  Nom: %s, Prenom: %s, Profession: %s, Tel: %s\n\n",
           clients[index].nom, clients[index].prenom, clients[index].profession, clients[index].Numtel);

    char choix;
    printf("Que voulez-vous modifier ?\n");
    printf("  1. Nom\n  2. Prenom\n  3. Profession\n  4. Telephone\n  5. Tout modifier\n  Autre. Annuler\n");
    printf("Votre choix : ");
    scanf(" %c", &choix);
    viderBuffer();

    int modif = 0;
    
    if (choix == '1' || choix == '5') {
        do {
            valide = 1;
            printf("Nouveau nom : ");
            fgets(clients[index].nom, sizeof(clients[index].nom), stdin);
            clients[index].nom[strcspn(clients[index].nom, "\n")] = '\0';

            if (strlen(clients[index].nom) == 0) valide = 0;

            for (int i = 0; clients[index].nom[i]; i++) {
                if (!isalpha(clients[index].nom[i]) && clients[index].nom[i] != ' ') {
                    valide = 0;
                    break;
                }
            }

            if (!valide)
                printf(COLOR_RED "Nom invalide (lettres uniquement).\n" COLOR_RESET);
        } while (!valide);
        modif = 1;  // CORRECTION: Ajout de modif = 1
    }
    
    if (choix == '2' || choix == '5') {
        do {
            valide = 1;
            printf("Nouveau prenom : ");
            fgets(clients[index].prenom, sizeof(clients[index].prenom), stdin);
            clients[index].prenom[strcspn(clients[index].prenom, "\n")] = '\0';

            if (strlen(clients[index].prenom) == 0) valide = 0;

            for (int i = 0; clients[index].prenom[i]; i++) {
                if (!isalpha(clients[index].prenom[i]) && clients[index].prenom[i] != ' ') {
                    valide = 0;
                    break;
                }
            }

            if (!valide)
                printf(COLOR_RED "Prenom invalide (lettres uniquement).\n" COLOR_RESET);
        } while (!valide);
        modif = 1;
    }
    
    if (choix == '3' || choix == '5') {
        do {
            valide = 1;
            printf("Nouvelle profession : ");
            fgets(clients[index].profession, sizeof(clients[index].profession), stdin);
            clients[index].profession[strcspn(clients[index].profession, "\n")] = '\0';

            if (strlen(clients[index].profession) == 0) valide = 0;

            for (int i = 0; clients[index].profession[i]; i++) {
                if (!isalpha(clients[index].profession[i]) && clients[index].profession[i] != ' ') {
                    valide = 0;
                    break;
                }
            }

            if (!valide)
                printf(COLOR_RED "Profession invalide (lettres uniquement).\n" COLOR_RESET);
        } while (!valide);
        modif = 1;
    }
    
    if (choix == '4' || choix == '5') {
        // CORRECTION: Ajout de la validation du téléphone
        do {
            printf("Nouveau telephone : ");
            fgets(clients[index].Numtel, sizeof(clients[index].Numtel), stdin);
            clients[index].Numtel[strcspn(clients[index].Numtel, "\n")] = '\0';

            if (strlen(clients[index].Numtel) != 10 ||
                clients[index].Numtel[0] != '0' ||
                (clients[index].Numtel[1] != '6' && clients[index].Numtel[1] != '7')) {
                printf(COLOR_RED " Numero invalide ! (ex: 06XXXXXXXX ou 07XXXXXXXX)\n" COLOR_RESET);
            }
        } while (strlen(clients[index].Numtel) != 10 ||
                 clients[index].Numtel[0] != '0' ||
                 (clients[index].Numtel[1] != '6' && clients[index].Numtel[1] != '7'));
        modif = 1;
    }

    if (modif) {
        afficherMessageSucces("Client modifie avec succes !");
    } else {
        afficherMessageInfo("Modification annulee.");
    }
}

void supprimerClient() {
    afficherEntete("SUPPRIMER UN CLIENT");
    int id, index;
    id = lireIdValide("Entrez l'ID du client a supprimer : ");

    index = trouverClientParId(id);
    if (index == -1) {
        afficherMessageErreur("Aucun client trouve avec cet ID.");
        return;
    }

    printf("Client a supprimer : %s %s (ID: %d)\n", clients[index].prenom, clients[index].nom, clients[index].Idclient);
    printf(COLOR_YELLOW "Etes-vous sur de vouloir supprimer ce client ? (O/N) : " COLOR_RESET);
    char conf;
    scanf(" %c", &conf);
    viderBuffer();

    if (toupper(conf) == 'O') {
        // Shift elements
        for (int i = index; i < nbclients - 1; i++) {
            clients[i] = clients[i + 1];
        }
        nbclients--;
        afficherMessageSucces("Client supprime avec succes.");
    } else {
        afficherMessageInfo("Suppression annulee.");
    }
}

void rechercherClient() {
    afficherEntete("RECHERCHER UN CLIENT");
    printf("Rechercher par :\n  1. ID\n  2. Nom\n");
    printf("Votre choix : ");
    char choix;
    scanf(" %c", &choix);
    viderBuffer();

    int index = -1;
    if (choix == '1') {
        int id;
        id = lireIdValide("Entrez l'ID du client : ");
        index = trouverClientParId(id);
    } else if (choix == '2') {
        char nom[50];
        printf("Entrez le nom du client : ");
        fgets(nom, sizeof(nom), stdin);
        nom[strcspn(nom, "\n")] = 0;
        index = trouverClientParNom(nom);
    } else {
        afficherMessageErreur("Choix invalide.");
        return;
    }

    if (index != -1) {
        afficherMessageSucces("Client trouve !");
        afficherLigne('-', 60);
        printf("ID: %d | Nom: %s | Prenom: %s | Profession: %s | Tel: %s\n",
               clients[index].Idclient, clients[index].nom, clients[index].prenom,
               clients[index].profession, clients[index].Numtel);
        afficherLigne('-', 60);
    } else {
        afficherMessageErreur("Aucun client trouve avec ce critere.");
    }
}

void afficherTousLesClients() {
    afficherEntete("LISTE DE TOUS LES CLIENTS");
    if (nbclients == 0) {
        afficherMessageInfo("Aucun client a afficher.");
        return;
    }
    printf("%-5s | %-15s | %-15s | %-15s | %-15s\n", "ID", "Nom", "Prenom", "Profession", "Telephone");
    afficherLigne('-', 80);
    for (int i = 0; i < nbclients; i++) {
        printf("%-5d | %-15s | %-15s | %-15s | %-15s\n",
               clients[i].Idclient, clients[i].nom, clients[i].prenom,
               clients[i].profession, clients[i].Numtel);
    }
    afficherLigne('-', 80);
}

// --- Account Management Functions ---

int trouverCompteParId(int id) {
    for (int i = 0; i < nbcomptes; i++) {
        if (comptes[i].Idcompte == id) {
            return i; // Found
        }
    }
    return -1; // Not found
}

void nouveauCompte() {
    afficherEntete("CREER UN NOUVEAU COMPTE");
    if (nbcomptes >= MAXCOMPTES) {
        afficherMessageErreur("Nombre maximum de comptes atteint.");
        return;
    }

    Compte c;
    c.Idcompte = lireIdValide("ID du compte : ");

    if (trouverCompteParId(c.Idcompte) != -1) {
        afficherMessageErreur("Un compte avec cet ID existe deja.");
        return;
    }

    c.Idclient = lireIdValide("ID du client proprietaire : ");

    if (trouverClientParId(c.Idclient) == -1) {
        afficherMessageErreur("Aucun client trouve avec cet ID. Creation de compte annulee.");
        return;
    }

    printf("Solde de base (>= %.2f DH) : ", SOLDEMINIMUM);
    scanf("%f", &c.Soldebase);
    viderBuffer();

    if (c.Soldebase < SOLDEMINIMUM) {
        afficherMessageErreur("Le solde de base doit etre superieur ou egal a 1000.00 DH.");
        return;
    }

    obtenirDateActuelle(c.Dateouverture);
    comptes[nbcomptes++] = c;

    afficherMessageSucces("Compte cree avec succes !");
    printf("Date d'ouverture : %s\n", c.Dateouverture);
}

void consulterComptes() {
    afficherEntete("CONSULTER LES COMPTES");
    printf("  1. Lister tous les comptes\n");
    printf("  2. Afficher un compte par ID\n");
    printf("  3. Afficher les comptes d'un client\n");
    printf("Votre choix : ");
    char choix;
    scanf(" %c", &choix);
    viderBuffer();

    if (choix == '1') {
        if (nbcomptes == 0) {
            afficherMessageInfo("Aucun compte a afficher.");
            return;
        }
        printf("\n%-10s | %-10s | %-15s | %-12s\n", "ID Compte", "ID Client", "Solde (DH)", "Date Ouv.");
        afficherLigne('-', 60);
        for (int i = 0; i < nbcomptes; i++) {
            printf("%-10d | %-10d | %-15.2f | %-12s\n",
                   comptes[i].Idcompte, comptes[i].Idclient,
                   comptes[i].Soldebase, comptes[i].Dateouverture);
        }
        afficherLigne('-', 60);
    } else if (choix == '2') {
        int id;
        id = lireIdValide("Entrez l'ID du compte : ");
        int index = trouverCompteParId(id);
        if (index != -1) {
            printf("\n%-10s | %-10s | %-15s | %-12s\n", "ID Compte", "ID Client", "Solde (DH)", "Date Ouv.");
            afficherLigne('-', 60);
            printf("%-10d | %-10d | %-15.2f | %-12s\n",
                   comptes[index].Idcompte, comptes[index].Idclient,
                   comptes[index].Soldebase, comptes[index].Dateouverture);
            afficherLigne('-', 60);
        } else {
            afficherMessageErreur("Aucun compte trouve avec cet ID.");
        }
    } else if (choix == '3') {
        int idClient;
        idClient = lireIdValide("Entrez l'ID du client : ");
        if (trouverClientParId(idClient) == -1) {
            afficherMessageErreur("Ce client n'existe pas.");
            return;
        }

        int found = 0;
        printf("\nComptes du client ID %d:\n", idClient);
        printf("%-10s | %-15s | %-12s\n", "ID Compte", "Solde (DH)", "Date Ouv.");
        afficherLigne('-', 50);
        for (int i = 0; i < nbcomptes; i++) {
            if (comptes[i].Idclient == idClient) {
                printf("%-10d | %-15.2f | %-12s\n",
                       comptes[i].Idcompte, comptes[i].Soldebase, comptes[i].Dateouverture);
                found = 1;
            }
        }
        if (!found) {
            afficherMessageInfo("Aucun compte trouve pour ce client.");
        } else {
            afficherLigne('-', 50);
        }
    } else {
        afficherMessageErreur("Choix invalide.");
    }
}

void fermerCompte() {
    afficherEntete("FERMER UN COMPTE");
    int id, index;
    id = lireIdValide("Entrez l'ID du compte a fermer : ");

    index = trouverCompteParId(id);
    if (index == -1) {
        afficherMessageErreur("Aucun compte trouve avec cet ID.");
        return;
    }

    printf("Compte a fermer : ID %d, Solde %.2f DH\n", comptes[index].Idcompte, comptes[index].Soldebase);
    printf(COLOR_YELLOW "Etes-vous sur de vouloir fermer ce compte ? (O/N) : " COLOR_RESET);
    char conf;
    scanf(" %c", &conf);
    viderBuffer();

    if (toupper(conf) == 'O') {
        for (int i = index; i < nbcomptes - 1; i++) {
            comptes[i] = comptes[i + 1];
        }
        nbcomptes--;
        afficherMessageSucces("Compte ferme avec succes.");
    } else {
        afficherMessageInfo("Fermeture annulee.");
    }
}

// --- Operations Functions ---

void effectuerRetrait() {
    afficherEntete("EFFECTUER UN RETRAIT");
    int id, index;
    id = lireIdValide("Entrez l'ID du compte pour le retrait : ");

    index = trouverCompteParId(id);
    if (index == -1) {
        afficherMessageErreur("Aucun compte trouve avec cet ID.");
        return;
    }

    float montant;
    printf("Solde actuel : " COLOR_CYAN "%.2f DH\n" COLOR_RESET, comptes[index].Soldebase);
    printf("Entrez le montant a retirer (max %.2f DH) : ", RETRAITMAX);
    scanf("%f", &montant);
    viderBuffer();

    if (montant <= 0) {
        afficherMessageErreur("Le montant doit etre positif.");
        return;
    }
    if (montant > RETRAITMAX) {
        afficherMessageErreur("Le montant de retrait depasse le maximum autorise.");
        return;
    }
    if (comptes[index].Soldebase - montant < SOLDEMINIMUM) {
        afficherMessageErreur("Operation impossible. Le solde minimum ne serait pas respecte.");
        return;
    }

    comptes[index].Soldebase -= montant;
    afficherMessageSucces("Retrait effectue avec succes !");
    printf("Nouveau solde : " COLOR_CYAN "%.2f DH\n" COLOR_RESET, comptes[index].Soldebase);
}

void effectuerVirement() {
    afficherEntete("EFFECTUER UN VIREMENT");
    int idSource, idDest, indexSource, indexDest;

    idSource = lireIdValide("Entrez l'ID du compte source : ");
    indexSource = trouverCompteParId(idSource);

    idDest = lireIdValide("Entrez l'ID du compte de destination : ");
    indexDest = trouverCompteParId(idDest);

    if (idSource == idDest) {
        afficherMessageErreur("Les comptes source et destination doivent etre differents.");
        return;
    }
    if (indexSource == -1 || indexDest == -1) {
        afficherMessageErreur("Un ou plusieurs comptes sont invalides.");
        return;
    }

    float montant;
    printf("Solde du compte source (ID %d) : " COLOR_CYAN "%.2f DH\n" COLOR_RESET, idSource, comptes[indexSource].Soldebase);
    printf("Entrez le montant a virer : ");
    scanf("%f", &montant);
    viderBuffer();

    if (montant <= 0) {
        afficherMessageErreur("Le montant doit etre positif.");
        return;
    }
    if (comptes[indexSource].Soldebase - montant < SOLDEMINIMUM) {
        afficherMessageErreur("Virement impossible. Le solde minimum du compte source ne serait pas respecte.");
        return;
    }

    // Perform transfer
    comptes[indexSource].Soldebase -= montant;
    comptes[indexDest].Soldebase += montant;

    afficherMessageSucces("Virement effectue avec succes !");
    printf("Nouveau solde du compte source (ID %d) : " COLOR_CYAN "%.2f DH\n" COLOR_RESET, idSource, comptes[indexSource].Soldebase);
    printf("Nouveau solde du compte destination (ID %d) : " COLOR_CYAN "%.2f DH\n" COLOR_RESET, idDest, comptes[indexDest].Soldebase);
}
