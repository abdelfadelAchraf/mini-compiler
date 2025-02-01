#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Définir la structure
typedef struct liste {
    char mot[100];
    int taille;
    int ligne;
    struct liste *next;
} liste;

// Fonction pour créer une nouvelle liste
liste *creerliste(char mot[], int taille , int ligne) {
    liste* nouveau = (liste*) malloc(sizeof(liste));
    strcpy(nouveau->mot, mot);
    nouveau->taille = taille;
    nouveau->ligne = ligne;
    nouveau->next = NULL;
    return nouveau;
}

// Ajouter un mot à la fin de la liste
liste* ajouterAuFin(liste* tete, char mot[], int taille, int ligne) {
    liste *nouveau, *courant;
//    nouveau = (liste*) malloc(sizeof(liste));
//    strcpy(nouveau->mot, mot);
//    nouveau->taille = taille;
//    nouveau->next = NULL;
    nouveau = creerliste(mot,taille, ligne);
    if (tete == NULL)
        return nouveau;
    courant = tete;
    while (courant->next != NULL)
        courant = courant->next;
    courant->next = nouveau;
    return tete;
}

// Afficher la liste
void afficherListe(liste *tete) {
    liste *courant = tete;
    if (courant == NULL) {
        printf("La liste est vide.\n");
        return;
    }
    while (courant != NULL) {
        printf("Mot : %s, Taille : %d, la ligne: %d\n", courant->mot, courant->taille, courant->ligne);
        courant = courant->next;
    }
}

// Supprimer le premier élément de la liste
liste* supprimerPremierElement(liste *tete) {
    if (tete == NULL) {
        printf("La liste est déjà vide.\n");
        return NULL;
    }
    liste *temp = tete;
    tete = tete->next;
    free(temp);
    return tete;
}
// Construire une liste depuis un fichier sans analyser chaque mot immédiatement
liste* construction(liste *tete, FILE *p) {
    char mot[100] = "";
    int ligne = 1;
    char c;

    // Lire caractère par caractère
    while ((c = fgetc(p)) != EOF) {
        // Si on trouve un caractère spécial, on traite le mot actuel
        if (c == ' ' || c == '\n' || c == '(' || c == ')' || c == ';' || c == '>' || c =='"') {
            if (strlen(mot) > 0) {
                tete = ajouterAuFin(tete, mot, strlen(mot),ligne);
                memset(mot, 0, sizeof(mot));
            }
            // Traite les caractères spéciaux comme mots séparés
            if (c == '(' || c == ')' || c == ';' || c == '>' || c == ':' ) {
                char special[2] = {c, '\0'};
                tete = ajouterAuFin(tete, special, 1,ligne);
            }
            if (c == '\n') ligne++;
        } else {
            // Ajoute le caractère au mot actuel
            int len = strlen(mot);
            mot[len] = c;
            mot[len + 1] = '\0';
        }
    }

    // Analyse du dernier mot si nécessaire
    if (strlen(mot) > 0) {
        tete = ajouterAuFin(tete, mot, strlen(mot),ligne);
    }

    return tete;
}
//Fonction pour effectuer l'analyse lexical.
void Analyse_lexique(char *mot, char *prog, int ligne) {

     //Ouvrir le fichier programme.
    FILE *programme = fopen(prog, "r");

    //Vérifier si le fichier est vide.
    if (!programme) {
        printf("Erreur : Impossible d'ouvrir le fichier du programme.\n");
        exit(1);
    }

    char lexique[100];
    int trouve = 0;

     //Lire les donnéés à partir du fichier pointer par programme.
    while (fscanf(programme, "%s", lexique) != EOF) {
        //Si la 1ere string == la 2eme string
        if (strcmp(mot, lexique) == 0) {
            trouve = 1;
            break;
        }
    }

    fclose(programme);
    //
    if (!trouve) {
        FILE *errors = fopen("errors.txt", "a+");
        if (errors) {
            fprintf(errors, "Erreur Lexicale en ligne %d : '%s' n'existe pas dans le dictionnaire de la langue.\n", ligne, mot);
            fclose(errors);
        }
        printf("Erreur Lexicale en ligne %d : '%s' n'existe pas dans le dictionnaire de la langue.\n", ligne, mot);
    }
}
// Analyse lexicale après la construction de la liste
void analyserListeLexicale(liste *tete, char *dict) {
    liste *courant = tete;
    int ligne = 1;

    while (courant != NULL) {
        Analyse_lexique(courant->mot, dict,courant->ligne);
        courant = courant->next;

    }
}

void Analyse_syntaxique_programme(liste *tete) {
    FILE *errors = fopen("errors.txt", "a+");
    if (!errors) {
        printf("Erreur : Impossible de créer le fichier errors.txt.\n");
        exit(1);
    }

    liste* tmp = tete;
    int erreurSyntaxique = 0;

    // Vérification du début du programme
    if (strcmp(tmp->mot, "debut") != 0) {
     
        printf("Erreur Syntaxique (ligne %d) : Le programme doit commencer par 'debut'.\n", tmp->ligne);
        fprintf(errors, "Erreur Syntaxique (ligne %d) : Le programme doit commencer par 'debut'.\n", tmp->ligne);
        erreurSyntaxique = 1;
    } else {
        tmp = tmp->next;
        if (tmp == NULL || strcmp(tmp->mot, ":") != 0) {
            printf("Erreur Syntaxique (ligne %d) : ':' manquant après 'debut'.\n", tmp->ligne);
            fprintf(errors, "Erreur Syntaxique (ligne %d) : ':' manquant après 'debut'.\n", tmp->ligne);
            erreurSyntaxique = 1;
        }
    }
    if (erreurSyntaxique) {
        fclose(errors);
        return;
    }
    tmp = tmp->next;

    while (tmp != NULL && !erreurSyntaxique) {
        if (strcmp(tmp->mot, "var") == 0) {
            // Vérification de la déclaration de variable
            tmp = tmp->next;
            if (tmp == NULL || tmp->next == NULL || strcmp(tmp->next->mot, ":") != 0) {
                printf("Erreur Syntaxique (ligne %d) : La déclaration de variable doit inclure ':'.\n", tmp->ligne);
                fprintf(errors, "Erreur Syntaxique (ligne %d) : La déclaration de variable doit inclure ':'.\n", tmp->ligne);
                erreurSyntaxique = 1;
                break;
            }
            tmp = tmp->next;
            if (tmp == NULL || strcmp(tmp->next->mot, "Entier") != 0) {
             
                printf("Erreur Syntaxique (ligne %d) : Le type de variable doit être 'Entier'.\n", tmp->ligne);
                fprintf(errors, "Erreur Syntaxique (ligne %d) : Le type de variable doit être 'Entier'.\n", tmp->ligne);
               

                erreurSyntaxique = 1;
                break;
            }
            tmp = tmp->next;

            if (tmp == NULL || strcmp(tmp->next->mot, ";") != 0) {
                printf("Erreur Syntaxique (ligne %d) : La déclaration de variable doit se terminer par ';'.\n", tmp->ligne);
                fprintf(errors, "Erreur Syntaxique (ligne %d) : La déclaration de variable doit se terminer par ';'.\n", tmp->ligne);
                erreurSyntaxique = 1;
                break;
            }
            tmp = tmp->next->next;

        } else if (strcmp(tmp->mot, "ecrire") == 0) {
            // Vérification de l'instruction ecrire
            tmp = tmp->next;

            if (tmp == NULL || strcmp(tmp->mot, "(") != 0) {
                printf("Erreur Syntaxique (ligne %d) : 'ecrire' doit être suivi de '('.\n", tmp->ligne);
                fprintf(errors, "Erreur Syntaxique (ligne %d) : 'ecrire' doit être suivi de '('.\n", tmp->ligne);
                erreurSyntaxique = 1;
                break;
            }
            tmp = tmp->next;
            while (strcmp(tmp->mot,")") != 0)
            {
                tmp = tmp->next ;
            }
            

            if (tmp == NULL || strcmp(tmp->mot, ")") != 0) {
                printf("Erreur Syntaxique (ligne %d) : Parenthèse fermante ')' manquante dans 'ecrire'.\n", tmp->ligne);
                fprintf(errors, "Erreur Syntaxique (ligne %d) : Parenthese fermante ')' manquante dans 'ecrire'.\n", tmp->ligne);
                erreurSyntaxique = 1;
                break;
            }
            tmp = tmp->next;
               

            if (tmp == NULL || strcmp(tmp->mot, ";") != 0) {
                printf("Erreur Syntaxique (ligne %d) : L'instruction 'ecrire' doit se terminer par ';'.\n", tmp->ligne);
                fprintf(errors, "Erreur Syntaxique (ligne %d) : L'instruction 'ecrire' doit se terminer par ';'.\n", tmp->ligne);
                erreurSyntaxique = 1;
                break;
            }
            tmp = tmp->next;
           
        } else if (strcmp(tmp->mot, "lire") == 0) {
            // Vérification de l'instruction lire
            tmp = tmp->next;
            if (tmp == NULL || strcmp(tmp->mot, "(") != 0) {
                printf("Erreur Syntaxique (ligne %d) : 'lire' doit être suivi de '('.\n", tmp->ligne);
                fprintf(errors, "Erreur Syntaxique (ligne %d) : 'lire' doit être suivi de '('.\n", tmp->ligne);
                erreurSyntaxique = 1;
                break;
            }
            tmp = tmp->next;
            if (tmp == NULL || strcmp(tmp->next->mot, ")") != 0) {
                printf("Erreur Syntaxique (ligne %d) : Parenthèse fermante ')' manquante dans 'lire'.\n", tmp->ligne);
                fprintf(errors, "Erreur Syntaxique (ligne %d) : Parenthèse fermante ')' manquante dans 'lire'.\n", tmp->ligne);
                erreurSyntaxique = 1;
                break;
            }
            tmp = tmp->next;

            if (tmp == NULL || strcmp(tmp->next->mot, ";") != 0) {
                printf("Erreur Syntaxique (ligne %d) : L'instruction 'lire' doit se terminer par ';'.\n", tmp->ligne);
                fprintf(errors, "Erreur Syntaxique (ligne %d) : L'instruction 'lire' doit se terminer par ';'.\n", tmp->ligne);
                erreurSyntaxique = 1;
                break;
            }
            tmp = tmp->next;
            
        } else if (strcmp(tmp->next->mot, "Si") == 0) {
            // Vérification de l'instruction Si
            tmp = tmp->next;
            
             while (strcmp(tmp->mot, "alors") != 0)
            {
                tmp = tmp->next ;
            }
            
            if (tmp == NULL || strcmp(tmp->mot, "alors") != 0) {
                printf("Erreur Syntaxique (ligne %d) : 'Si' doit être suivi par une condition et 'alors'.\n", tmp->ligne);
                fprintf(errors, "Erreur Syntaxique (ligne %d) : 'Si' doit être suivi par une condition et 'alors'.\n", tmp->ligne);
                erreurSyntaxique = 1;
                break;
            }
            tmp = tmp->next; // Passer au contenu après 'alors'

        } else if (strcmp(tmp->mot, "sinon") == 0 || strcmp(tmp->mot, "FinSi") == 0 || strcmp(tmp->mot, "Fin") == 0) {
            if (strcmp(tmp->mot, "Fin") == 0) {
                tmp = tmp->next;
                
                if (tmp == NULL || strcmp(tmp->mot, ".") != 0) {
                    printf("Erreur Syntaxique (ligne %d) : 'Fin' doit être suivi d'un point final '.'\n", tmp->ligne);
                    fprintf(errors, "Erreur Syntaxique (ligne %d) : 'Fin' doit être suivi d'un point final '.'\n", tmp->ligne);
                    erreurSyntaxique = 1;
                    break;
                }
            }
            tmp = tmp->next;
        } else {
            printf("Erreur Syntaxique (ligne %d) : Instruction inconnue ou incorrecte.\n", tmp->ligne);
            fprintf(errors, "Erreur Syntaxique (ligne %d) : Instruction inconnue ou incorrecte.\n", tmp->ligne);
            erreurSyntaxique = 1;
            break;
        }
    }

    if (!erreurSyntaxique) {
        printf("Analyse syntaxique reussie.\n");
    } else {
        printf("Analyse syntaxique echoue. Verifiez les erreurs dans errors.txt.\n");
    }

    fclose(errors);
}

//le programme principal
int main() {
   char *cheminProgramme = "programme.txt";
    char *dictionnaire = "dictionnaire.txt";

    FILE *p = fopen(cheminProgramme, "r");
    if (!p) {
        printf("Erreur : Impossible d'ouvrir le fichier du programme.\n");
        exit(1);
    }

    liste *tete = NULL;
    tete = construction(tete, p);
    afficherListe(tete);
    fclose(p);
    analyserListeLexicale(tete , dictionnaire);
    Analyse_syntaxique_programme(tete);
    return 0;
}
