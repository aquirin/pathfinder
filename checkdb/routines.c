
#ifndef __ROUTINES_C__
#define __ROUTINES_C__

#include "base.h"
#include "routines.h"


// return error("ERREUR");
int error(char *s)
{
	fprintf(stderr,s,NULL);
	return -1;
}
void die(char *s)
{
	fprintf(stderr,s,NULL);
	exit(-1);
}
void die2(int n)
{
	perror("exit");
	int ret = system("PAUSE");
	USE(ret);
	exit(n);
}
void die3(char *s, int n)
{
	perror(s);
	int ret = system("PAUSE");
	USE(ret);
	exit(n);
}

/*
#define DEBUG	vrai

#if DEBUG
#define debug(fmt,arg...) \
	fprintf(stderr,fmt,##arg)
#else
#define debug(fmt,arg...) \
	do { } while (0)
#endif
*/

/**************************************************\
Trie un tableau de notes a[N][2] par le Quick Sort
INPUT: a[][0] doit contenir les N notes (int)
       a[][1] doit contenir l'index des notes
       (lo0, hiO) : indices min et max
OUTPUT:
       a[][1] contientra les index des notes dans
l'ordre des notes croissantes
\**************************************************/
void qsorti(int **a, int lo0, int hi0)
{
	int lo = lo0; int hi = hi0; int mid;

	if ( hi0 > lo0)
	{
		/* Arbitrarily establishing partition element as the midpoint of
		 * the array.
		 */
		mid = a[ ( lo0 + hi0 ) / 2 ][0];

		// loop through the array until indices cross
		while( lo <= hi )
		{
			/* find the first element that is greater than or equal to
			 * the partition element starting from the left Index.
			 */
			while( ( lo < hi0 ) &&  ( a[lo][0] < mid ))
			++lo;

			/* find an element that is smaller than or equal to
			 * the partition element starting from the right Index.
			 */
			while( ( hi > lo0 ) &&  ( a[hi][0] > mid ))
			--hi;

			// if the indexes have not crossed, swap
			if( lo <= hi )
			{
				swapQSi(a, lo, hi);
				++lo;
				--hi;
			}
		}

		/* If the right index has not reached the left side of array
		 * must now sort the left partition.
		 */
		if( lo0 < hi )
			qsorti( a, lo0, hi );

		/* If the left index has not reached the right side of array
		 * must now sort the right partition.
		 */
		if( lo < hi0 )
			qsorti( a, lo, hi0 );

	}
}
// Utilisé par le Quick Sort
void swapQSi(int **a, int i, int j)
{
	int T0, T1;
	T0 = a[i][0];
	T1 = a[i][1];
	a[i][0] = a[j][0];
	a[i][1] = a[j][1];
	a[j][0] = T0;
	a[j][1] = T1;
}


/**************************************************\
Trie un tableau de notes a[N][2] par le Quick Sort
INPUT: a[][0] doit contenir les N notes (double)
       a[][1] doit contenir l'index des notes
       (lo0, hiO) : indices min et max
OUTPUT:
       a[][1] contientra les index des notes dans
l'ordre des notes croissantes
Utiliser qsortd2 pour utiliser 2 vecteurs au lieu du tableau.
\**************************************************/
void qsortd(double **a, int lo0, int hi0)
{
	int lo = lo0; int hi = hi0; double mid;
	if ( hi0 > lo0)
	{
		mid = a[ ( lo0 + hi0 ) / 2 ][0];
		while( lo <= hi )
		{
			while( ( lo < hi0 ) &&  ( a[lo][0] < mid )) ++lo;
			while( ( hi > lo0 ) &&  ( a[hi][0] > mid )) --hi;
			if( lo <= hi )
			{
				swapQSd(a, lo, hi);
				++lo;
				--hi;
			}
		}
		if( lo0 < hi ) qsortd( a, lo0, hi );
		if( lo < hi0 ) qsortd( a, lo, hi0 );
	}
}

void qsortd2(double *a, int *idx, int lo0, int hi0)
{
	int lo = lo0; int hi = hi0; int tmpi; double mid, tmpd;
	if ( hi0 > lo0)
	{
		mid = a[ ( lo0 + hi0 ) / 2 ];
		while( lo <= hi )
		{
			while( ( lo < hi0 ) &&  ( a[lo] < mid )) ++lo;
			while( ( hi > lo0 ) &&  ( a[hi] > mid )) --hi;
			if( lo <= hi )
			{
				tmpd=a[lo]; a[lo]=a[hi]; a[hi]=tmpd;
				tmpi=idx[lo]; idx[lo]=idx[hi]; idx[hi]=tmpi;
				++lo;
				--hi;
			}
		}
		if( lo0 < hi ) qsortd2( a, idx, lo0, hi );
		if( lo < hi0 ) qsortd2( a, idx, lo, hi0 );
	}
}

// Utilisé par le Quick Sort
void swapQSd(double **a, int i, int j)
{
	double T0, T1;
	T0 = a[i][0];
	T1 = a[i][1];
	a[i][0] = a[j][0];
	a[i][1] = a[j][1];
	a[j][0] = T0;
	a[j][1] = T1;
}

/**************************************************\
Gestion d'un chronomètre
\**************************************************/
#ifdef linux
	#ifdef cygwin
		struct timeb baseh_tim1;
		void baseh_chronostart(void)	// Démarre le chrono
		{
			ftime(&baseh_tim1);
		}
		double baseh_chronostop(char affiche)		// Stoppe le chrono et affiche (si 1) et retourne la différence de temps
		{
			struct timeb baseh_tim2; long diff;
			ftime(&baseh_tim2);
			diff = (baseh_tim2.time-baseh_tim1.time)*1000+baseh_tim2.millitm-baseh_tim1.millitm;
			if(affiche) printf("Temps: %ld ms\n",diff);
			return (double)diff;
		}
	#else
		#include <sys/times.h>
		struct tms baseh_tim1;
		void baseh_chronostart(void)	// Démarre le chrono
		{
			times(&baseh_tim1);
		}
		double baseh_chronostop(char affiche)		// Stoppe le chrono et affiche (si 1) et retourne la différence de temps
		{
			struct tms baseh_tim2; double diff;
			times(&baseh_tim2);
			diff = (baseh_tim2.tms_utime-baseh_tim1.tms_utime)/100.0;
			if(affiche) printf("Temps(s): %1.2f\n",diff);
			return diff;
		}
	#endif
#else
	struct timeb baseh_tim1;
	void baseh_chronostart(void)	// Démarre le chrono
	{
		ftime(&baseh_tim1);
	}
	double baseh_chronostop(char affiche)		// Stoppe le chrono et affiche (si 1) et retourne la différence de temps
	{
	    struct timeb baseh_tim2; long diff;
		ftime(&baseh_tim2);
		diff = (baseh_tim2.time-baseh_tim1.time)*1000+baseh_tim2.millitm-baseh_tim1.millitm;
		if(affiche) printf("Temps: %ld ms\n",diff);
		return (double)diff;
	}
#endif
void baseh_affdate(void)		// Affiche la date et l'heure
{
	time_t tim;
	tim=time(NULL);
    printf("Date: %s",asctime(localtime(&tim)));
}
void baseh_fct(void){}	// Fct qui ne fait rien

// Temps d'appel d'une fct en nano-secondes (10^-9 s).
// P2 200: 56.7735 ns
// P3 800: 7.590425 ns
// P4 2500: 3.207025 ns
double baseh_bench(void)
{
	unsigned int i, j, tps, nbt=2000000000;
	double ttps=0;
	printf("Démarrage dans 5s. Fermez les programmes inutiles.\n");
	baseh_chronostart();
	while(baseh_chronostop(0)<5000);
	printf("Démarré ! C'est parti pour 5 min.\n");
	for(j=0;j<10;j++)
	{
		baseh_chronostart();
		for(i=0;i<nbt;i++) baseh_fct();
		tps=baseh_chronostop(0);
		ttps+=(double)1000000*tps/nbt;
	}
	return ttps/10;
}

// Attendre un certain temps
#ifdef _WIN32
void mysleep (int milli)
{
	time_t tim1, tim2;
	time(&tim1);
	while(1)
	{
		time(&tim2);
		if(1000*difftime(tim2,tim1)>milli) break;
	}
}
#else
void mysleep (int milli)
{
	fd_set selectset;
	struct timeval tval;
	FD_ZERO (&selectset);
	tval.tv_sec = 0;
	tval.tv_usec = milli * 1000;
	select (1, &selectset, NULL, NULL, &tval); // WSOCK32.LIB sous WIN32
}
#endif

/**************************************************\
Gestion des nombres pseudo-aléatoires
\**************************************************/

int baseh_rcall;
int baseh_rseed;

int alea_getcall(void)		// Renvoit la position de la séquence [aleas() doit avoir été appelée]
{
	return baseh_rcall;
}

void alea_setcall(int n)		// Se place avant la position donnée [aleas() doit avoir été appelée]
{
	int i;
	aleas(baseh_rseed);
	for(i=0;i<n;i++) alead(0,1);
}

void aleas(unsigned int s)	// Initialise le générateur aléatoire
{
	baseh_rcall = 0;
	baseh_rseed = s;
	srand(baseh_rseed);
}

int aleai(int a, int b)		// Renvoit un nombre entier dans [a;b] (on peut avoir a>b)
{
	baseh_rcall++;
	return rand()%(abs(b-a)+1)+MIN(a,b);
}

double alead(double a, double b)		// Renvoit un nombre réel dans [a;b]
{
	baseh_rcall++;
	return fabs(b-a)*rand()/(RAND_MAX+1.0)+MIN(a,b);
}

double aleado(double a, double b)		// Renvoit un nombre réel dans [a;b[
{
	return fabs(b-a)*rand()/(RAND_MAX)+MIN(a,b);
}

void nomalea(char* nom, int lgr) // renvoit un nom aléatoire de taille lgr>0 du type "Golum"
{
	int i;
	char cons[]="bcdfghjklmnpqrstvwxz";
	char voy[]="aeiouy";
	nom[0]=cons[aleai(0,19)]-32;
	for(i=1;i<lgr;i++)
		if(i%2)
			nom[i]=voy[aleai(0,5)];
		else
			nom[i]=cons[aleai(0,19)];
	nom[i]=0;
}

// Renvoit un nombre entier dans [a;b], avec une préférence pour a (on peut avoir a>b)
// En moyenne, on obtiendra a+|a-b|/4 (si a<b) et b-|a-b|/4 (si a>b)
int aleap(int a, int b)
{
	return aleai(a,aleai(a,b));
}

// Renvoit l'une des valeur de t[n]
int aleati(int *t, int n)
{
	return t[aleai(0,n-1)];
}

double aleatd(double *t, int n)
{
	return t[aleai(0,n-1)];
}

/**************************************************\
Mathématiques
\**************************************************/

// Puissance entière
// ipow(f,n) = f^n
double ipow(double f, unsigned int n)
{
	unsigned int i;
	double g = 1;
	for(i=0;i<n;i++) g *= f;
	return g;
}

// Arrondi pour le nombre de chiffres indiqués
// Ex: rnd(343.3534,2) = 343.35
// n>0 : arrondi à droite de la virgule
// n<0 : arrondi à gauche de la virgule
double rnd(double f, int n)
{
	double g;
	if(n>=0) g = ipow(10,n);
	else g = 1/ipow(10,-n);
	//if(f>=0) return floor(f*g+0.5)/g;
	//else return ceil(f*g-0.5)/g;		// Comportement Excel : rnd(-2.5,0) = -3
	return floor(f*g+0.5)/g;			// Comportement OK : rnd(-2.5,0) = -2
}

// Arrondi supérieur
// Ex: rnd_sup(343.3534,1) = 343.4
double rnd_sup(double f, int n)
{
	double g;
	if(n>=0) g = ipow(10,n);
	else g = 1/ipow(10,-n);
	return ceil(f*g)/g;
}

// Arrondi inférieur
// Ex: rnd_inf(343.3534,1) = 343.3
double rnd_inf(double f, int n)
{
	double g;
	if(n>=0) g = ipow(10,n);
	else g = 1/ipow(10,-n);
	return floor(f*g)/g;
}

// Arrondi au multiple à la précision spécifiée
// Ex: rnd_mul(25.43,0,0.05) = 25.45
// Ex: rnd_mul(25.43,2,5) = 25.45
double rnd_mul(double f, int n, double m)
{
	return rnd(f/m,n)*m;
}


/**************************************************\
Gestions de son (que sous Windows)
\**************************************************/

//#ifndef _WIN32
void Beep(int f, int d)
{
	USE(f);
	USE(d);
}
//#endif

// Joue n notes. duree est en ms.
void joueson(int n, int *freq, int *duree)
{
	int i;
	for(i=0;i<n;i++) Beep(freq[i],duree[i]);
}

// Joue un petit jingle
void jouejingle(void)
{
	int freq[] = {390,465,390,390,515,390,350,390,565,390,390,590,565,465,390,565,690,390,350,350,275,440,390};
	int duree[] = {300,300,150,75,75,150,150,300,300,150,75,75,150,150,150,150,150,75,150,75,150,150,750};
	joueson(23,freq,duree);
}

/**************************************************\
Gestion de champs de bits. Un champs de bits (selon
cette bibliothèque et non le standard C) est une
chaîne pouvant contenir un nombre illimité de bit.
Le bit de poids faible (0) est toujours le premier
dans la chaîne (s[0]>>7).
Note: les define sont environ 20% plus rapides
\**************************************************/

// Fabrique un champ de bits de n bits
CBit cbitNouv(int n)
{
	return MALLOCN(1+(n-1)/8,char);
}

// Fabrique un champ de bits de n bits, initialisés à 0
CBit cbitNouv0(int n)
{
	return CALLOCN(1+(n-1)/8,char);
}

// Vide un champ de bits de n bits (met tous les bits à 0)
CBit cbitClear(CBit s, int n)
{
	bzero(s,(1+(n-1)/8));
	return s;
}

// Efface un champ de bits
void cbitDel(CBit s)
{
	FREE(s);
}

// Récupère le bit n (0=p.faible) de s
Bit cbitGet(CBit s, int n)
{
	return (s[n>>3]>>(n&0x7))&0x1;
}

// Met le bit n de s à v
void cbitSet(CBit s, int n, char v)
{
	if(v) s[n>>3]|=0x1<<(n&0x7);
	else s[n>>3]&=~(0x1<<(n&0x7));
}

// Renvoit 1 si le bit n de s est à v
Bit cbitTest(CBit s, int n, char v)
{
	return((s[n>>3]>>(n&0x7))&0x1)==v;
}

// Le bit n de s est inversé
void cbitInv(CBit s, int n)
{
	s[n>>3]^=0x1<<(n&0x7);
}

// Rempli de v le champ de bits de n bits
void cbitEff(CBit s, int n, char v)
{
	int i;
	for(i=0;i<n;i++) cbitSet(s,i,v);
}

// Convertit un champ de bits s de n bits dans sa représentation entière r
// Exemple : 0b011010 => "011010"
void cbitS2R(CBit s, char *r, int n)
{
	int i;
	for(i=0;i<n;i++) r[i]=cbitGet(s,i)+'0';
	r[i]=0;
}

// Convertit une représentation entière r de n bits en champ de bits s
// Exemple : "011010" => 0b011010
void cbitR2S(CBit s, char *r, int n)
{
	int i=-1;
	USE(n);
	while(r[++i]) cbitSet(s,i,(char)(r[i]-'0'));
}

// Ajoute à s1 (n1 bits) le champ de bits s2 de n2 bits
// Exemple: 0b001101010 + 0b000110101000 = 0b001101010000110101000
void cbitAjout(CBit s1, CBit s2, int n1, int n2)
{
	int i;
	for(i=0;i<n2;i++) cbitSet(s1,n1+i,cbitGet(s2,i));
}

// Ecrit un entier non signé x sur t bits
// après la position n du champ de bits
// L'entier est écrit à l'envers (ex: 8 => 00010 sur 5 bits)
void cbitWuint(CBit s, int n, unsigned int x, int t)
{
	int i, x2=x;
	for(i=n;i<n+t;i++)
	{
		cbitSet(s,i,(char)(x2&0x1));
		x2>>=1;
	}
}

// Lit l'entier non signé sur t bits
// à la position n du champ de bits
// L'entier est lu à l'envers
unsigned int cbitRuint(CBit s, int n, int t)
{
	int i, x2=0;
	for(i=n+t-1;i>=n;i--)
	{
		x2<<=1;
		x2|=cbitGet(s,i);
	}
	return x2;
}

/**************************************************\
Gestion de la mémoire.
Note: utiliser MALLOC1, MALLOC2, MALLOC3 et FREE1,
FREE2, FREE3 définies dans base.h à la place.
\**************************************************/

// Alloue un tableau de (i) objets de taille n.
// Le tableau est rempli d'objets NULL.
void* Malloc1(int i, int n)
{
	void *t = calloc(i*n,1);
	return t;
}
// Détruit ce tableau
void Free1(void* t, int i)
{
	USE(i);
	FREE(t);
}

// Alloue un tableau de (ixj) objets de taille n.
// Le tableau est rempli d'objets NULL.
void** Malloc2(int i, int j, int n)
{
	int ii; void **t = MALLOCN(i,void*);
	for(ii=0;ii<i;ii++)
	{
		t[ii] = malloc(j*n);
		bzero(t[ii],j*n);
	}
	return t;
}
// Détruit ce tableau
void Free2(void** t, int i, int j)
{
	int ii;
	USE(j);
	for(ii=0;ii<i;ii++) FREE(t[ii]);
	FREE(t);
}

// Alloue un tableau de (ixjxk) objets de taille n.
void*** Malloc3(int i, int j, int k, int n)
{
	int ii; void ***t = MALLOCN(i,void**);
	for(ii=0;ii<i;ii++) t[ii] = Malloc2(j,k,n);
	return t;
}
// Détruit ce tableau
void Free3(void*** t, int i, int j, int k)
{
	int ii;
	for(ii=0;ii<i;ii++) Free2(t[ii],j,k);
	FREE(t);
}


/**************************************************\
Traitement des chaînes.
\**************************************************/

// Remplace "12345678" par "12 345 678"
// s est modifiée et unit y est concaténé
char* formAlign3(char* s, char* unit)
{
	int i, j, l=strlen(s)-1, nl=l+l/3;
	s[nl+1]=0;
	for(i=l,j=nl;i>=0;i--,j--)
	{
		if(!((l-i)%3) && (l-i)) { s[j]=' '; j--; }
		s[j]=s[i];
	}
	return strcat(s,unit);
}

// Recherche tok dans str indépendamment de la casse.
// Renvoit la position correspondante si trouvé.
char* stristr(char* str, char* tok)
{
	char *sp=str; int tp=0;
	while(*str)
	{
		if(tolower(*str)==tolower(tok[tp])) { if(!tp++) sp=str; }
		else { if(tp) str=sp; tp=0; }
		if(!tok[tp]) return sp;
		str++;
	}
	return NULL;
}

// Recherche tok au début de str indépendemment de la
// casse. Renvoit 1 si trouvé.
int stribeg(char* str, char* tok)
{
	while(*tok)
	{
		if(tolower(*str)!=tolower(*tok)) return 0;
		str++; tok++;
	}
	return 1;
}

#ifdef _WIN32
// Transforme en majuscules
char* makecaps(char* str)
{
	unsigned int i;
	for(i=0; i<strlen(str); i++) if(__isascii(str[i])) if(islower(str[i])) str[i]=_toupper(str[i]);
	return str;
}

// Transforme en minuscules
char* makelower(char* str)
{
	unsigned int i;
	for(i=0; i<strlen(str); i++) if(__isascii(str[i])) if(isupper(str[i])) str[i]=_tolower(str[i]);
	return str;
}
#endif

// Ajoute (strcat) un char à la chaîne en argument et avance le pointeur
void cstrput(char** str, char c)
{
	sprintf(*str, "%c ", c);
	*str += 2;
}

// Ajoute (strcat) un entier à la chaîne en argument et avance le pointeur
void istrput(char** str, int i)
{
	sprintf(*str, "%d ", i);
	while(**str) (*str)++;
}

// Ajoute (strcat) un double à la chaîne en argument et avance le pointeur
void dstrput(char** str, double f)
{
	sprintf(*str, "%.17f ", f);
	while(**str) (*str)++;
}

// Ajoute (strcat) une chaîne à la chaîne en argument et avance le pointeur
void sstrput(char** str, char* s)
{
	if(!s) return;
	while(*s)
	{
		**str = *s;
		(*str)++;
		s++;
	}
	**str = ' ';
	(*str)++;
	**str = 0;
}

// Récupère le char en début de chaîne et avance le pointeur
void cstrget(char** str, char *c)
{
	while(**str==' ') (*str)++;
	sscanf(*str, "%c", c);
	while((**str!=' ') && (**str!=0)) (*str)++;
}

// Récupère le int en début de chaîne et avance le pointeur
void istrget(char** str, int *i)
{
	while(**str==' ') (*str)++;
	sscanf(*str, "%d", i);
	while((**str!=' ') && (**str!=0)) (*str)++;
}

// Récupère le double en début de chaîne et avance le pointeur
void dstrget(char** str, double *d)
{
	while(**str==' ') (*str)++;
	sscanf(*str, "%lf", d);
	while((**str!=' ') && (**str!=0)) (*str)++;
}

// Récupère la chaîne (ne contenant pas d'espaces) en début de chaîne et avance le pointeur
void sstrget(char** str, char *s)
{
	while(**str==' ') (*str)++;
	while((**str!=' ') && (**str!=0))
	{
		*s = **str;
		(*str)++;
		s++;
	}
	*s = 0;
}





/**************************************************\
Fonction qui permet de chercher des mots clés dans
le texte 'txt'. exp est une liste de token séparés par
& (tous les mots) et/ou | (l'un des mots).

Notes:
- Seule la casse d'un token entouré de "guillemets" est
  prise en compte.
- Mettre ~ devant une expression pour la négation.
- On a "A|B|C|D" => "A|(B|(C|D))" (pas de priorités)
  Ex: 0&1|1 = 0&1 = 0
- Utiliser des parenthèses pour gérer la priorité.
- Si A est faux dans A&(...), le reste n'est jamais
  exécuté.
- Utiliser \car pour avoir les symboles spéciaux dans
  un token.
  Ex: "\(". car = ()&|\"~
- Une expression mal formée "(A())", "((A(" ou "bon\" aura
  des résultats inattendus (erreurs, ...).

Exemples d'expressions:
- credit|(free&sex) => cherche "credit" ou ( "free" et "sex" ).
- "Jean"|michel     => cherche "Jean" avec une majuscule ou "michel" sans tenir compte de la casse.
- sex&~(pay|cash)   => cherche "sex" mais pas "pay" ou "cash".
\**************************************************/
ubool findexp(char *txt, char *exp)
{
	char c, *nnexp; ubool res;
	int p = 0;

	if((exp==NULL) || (exp[0]==0)) return true;
	nnexp = MALLOC1(strlen(exp)+1,char);

	// Remplacement des caractères spéciaux
	while((c=*exp))
	{
		switch(c)
		{
		case '\\' : nnexp[p++]=*(exp+1); exp++; break;
		case '(' :  nnexp[p++]=1; break;
		case ')' :  nnexp[p++]=2; break;
		case '&' :  nnexp[p++]=3; break;
		case '|' :  nnexp[p++]=4; break;
		case '\"' : nnexp[p++]=5; break;
		case '~' :  nnexp[p++]=6; break;
		default :   nnexp[p++]=c; break;
		}
		exp++;
	}
	nnexp[p]=0;
	res = findexp0(txt,nnexp);
	FREE(nnexp);
	return res;
}
ubool findexp0(char *txt, char *exp)
{
	ubool x; int pos; char c, d;
	if(exp==NULL) return true;
	if(exp[0]=='\x006') return !findexp0(txt,exp+1); // Négation ~
	if(exp[0]==0) return true;
	if(exp[0]=='\x001') // Expression ( )
	{
		int n=1, p=0;
		while(n)
		{
			p++;
			if(exp[p]=='\x001') n++;
			if(exp[p]=='\x002') n--;
		}
		c = exp[p];
		exp[p] = 0;
		x = findexp0(txt,exp+1);
		exp[p] = c;
		pos = p+1;
		c = exp[pos];
	}
	else
	{
		pos = strcspn(exp,"\x003\x004");
		c = exp[pos];
		exp[pos] = 0;
		if(exp[0]=='\x005') // Avec la casse "..."
		{
			d = exp[pos-1];
			exp[pos-1] = 0;
			x = (ubool) (strstr(txt,exp+1)==NULL?false:true);
			exp[pos-1] = d;
		}
		else x = (ubool) (stristr(txt,exp)==NULL?false:true);
		exp[pos] = c;
	}
	
	if(!c) return x;
	switch(c)
	{
		case '\x003': return x && findexp0(txt,exp+pos+1); // ET
		case '\x004': return x || findexp0(txt,exp+pos+1); // OU
		default: return false;
	}
}


/**************************************************\
Ecran, clavier.
\**************************************************/

// Efface l'écran texte et positionne le curseur en haut à gauche
void ClearScreen(void)
{
    #if defined(UTILISE_NCURSES)
	clear();
    move (0,0);

    #elif defined(_Windows)
	// Début : (1,1)
    CONSOLE_SCREEN_BUFFER_INFO Info;
    COORD Debut;
    DWORD NbrOctetEcrit;
    Debut.X = 0;
    Debut.Y = 0;
    
    // Lit les infos sur le buffer
    GetConsoleScreenBufferInfo (HdlConsole, &Info);
    // Remplit l'écran avec le caractère espace : ' '
    FillConsoleOutputCharacter (HdlConsole, ' ', Info.dwSize.X*Info.dwSize.Y, Debut, &NbrOctetEcrit);
    
    // Positionne le curseur
    SetConsoleCursorPosition (HdlConsole,Debut);

    #elif defined(__MSDOS__)
	union REGS inregs, outregs;
    struct SREGS segregs;
    
    inregs.h.ah = 0x00; // Fonction 00h : Change de mode video
    inregs.h.ah = 0x03; // Mode = 03h : 80x25 caracteres, 16 couleurs
    int86x(0x10, &inregs, &outregs, &segregs); // Interruption 10h : Video

	#elif defined(DOS)
	system("CLS");
    #endif
} 

// Gestion des touches étendues (F1...F12)
unsigned char GetExtendedKey(void)
{
	unsigned char c = 0;
#ifdef UTILISE_NCURSES
	c = getch();
	if (c == 0)
	{
	    // touche étendue
	    c = getch();
	    // c contient maintenant le code de la touche étendue
		return c;
	}
#endif
	return c;
} 

/**************************************************\
Affiche une barre de pourcentage (p entre 0 et 1)
sur une longueur de fenêtre l (en caractères).
Note: rien ne doit être affiché entre les différents appels de cette procédure.
A la fin, imprimer un retour à la ligne.
\**************************************************/
void printprc(double p, int l)
{
	int i;
	for(i=0;i<l+10;i++) printf("\b");
	printf("[");
	for(i=0;i<p*l;i++) printf("*");
	for(;i<l;i++) printf(" ");
	printf("] %.2f%%",p*100);
}

#endif

/**************************************************\
Traitement des fichiers.
\**************************************************/

// Récupère la prochaine ligne de FILE*, quelle que soit sa taille.
// Note: désallouer la chaîne à la fin
// 'size' renvoit la taille de la nouvelle ligne.
// Une ligne vide renvoit '\n' et la fin d'un fichier renvoit NULL.
#undef GETLINE_LINESIZE
#define GETLINE_LINESIZE 1000	// Peut être changé à souhait [3;+oo]
char* mygetline(FILE* in, int *size)
{
	char *buffer, line[GETLINE_LINESIZE], *ret;
	int lsize, pos;


	*size = 0;
	buffer = NULL;
	while(1)
	{
		ret = fgets(line, GETLINE_LINESIZE, in);
		if(ret == NULL) return NULL;
		lsize = strlen(line);
		if(lsize == 0) return NULL;
		pos = strcspn(line, "\r\n");
		line[pos] = 0;
		lsize = strlen(line);

		(*size) += lsize;
		buffer = (char*)realloc(buffer, (*size)+3);
		strncpy(buffer+(*size)-lsize, line, lsize+1);

		if(lsize < (GETLINE_LINESIZE-1)) break;
	}

	strcat(buffer, "\r\n");
	(*size)+=2;
	return buffer;
}

