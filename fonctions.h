int vol_vide(char vol[]);
void afficherVol(VOL vol);
void handleSIGUSR1();
void handleSIGUSR2();
void ecrireNouveauVol(VOL vol_a_ecrire, VOL* vols, int mutex, int empty, int sem_affichage);
VOL genererVolRandom(char liste_dest[23][21]);
void lireDestinations(char liste_dest[23][21], char* nomFichier);
