/**************************************************\
Routines les plus utilisées.
void qsorti(int **a, int lo0, int hi0)
-> Tri un tableau d'entiers en conservant la position

void qsortd(double **a, int lo0, int hi0)
-> Tri un tableau de doubles en conservant la position

void baseh_chronostart(void)
-> Démarre un chronomètre

long baseh_chronostop(char a)
-> Stoppe et affiche le temps écoulé (en ms) si a=1

void baseh_affdate(void)
-> Affiche la date courante

Constructeur (new ou malloc) : cbitNouv
Destructeur (delete ou free) : cbitDel
\**************************************************/

#ifndef __ROUTINES_H__
#define __ROUTINES_H__

#include "base.h"
#include <time.h>
#include "math.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _Windows
#include <windows.h>
#elif defined(linux) /* ou autres Unix qui possèdent NCurses */
#ifdef USE_NCURSES
#include <ncurses.h>
#define UTILISE_NCURSES
#endif
#elif defined(__MSDOS__)
#include <dos.h>
#endif

#define ubool unsigned char // Evite les conflits

// Tri
int error(char *s);
void die(char *s);
void die2(int n);
void die3(char *s, int n);
void qsorti(int **a, int lo0, int hi0);
void swapQSi(int **a, int i, int j);
void qsortd(double **a, int lo0, int hi0);
void qsortd2(double *a, int *idx, int lo0, int hi0);
void swapQSd(double **a, int i, int j);

// Chronomètre
void baseh_chronostart(void);
double baseh_chronostop(char affiche);
void baseh_affdate(void);
void baseh_fct(void);
double baseh_bench(void);
void mysleep(int milli);

// Nb pseudo-aléatoires
int alea_getcall(void);
void alea_setcall(int n);
void aleas(unsigned int s);
int aleai(int a, int b);
double alead(double a, double b);
double aleado(double a, double b);
void nomalea(char* nom, int lgr);
int aleap(int a, int b);
int aleati(int *t, int n);
double aleatd(double *t, int n);

// Mathématiques
double ipow(double f, unsigned int n);
double rnd(double f, int n);
double rnd_sup(double f, int n);
double rnd_inf(double f, int n);
double rnd_mul(double f, int n, double m);

// Sons
void joueson(int n, int *freq, int *duree);
void jouejingle(void);

// Champs de bits
// Note: les define sont environ 20% plus rapides
#define _cbitGet(s,n) ((s[(n)>>3]>>((n)&0x7))&0x1)
#define _cbitSet(s,n,v) do { if(v) s[(n)>>3]|=0x1<<((n)&0x7); else s[(n)>>3]&=~(0x1<<((n)&0x7)); } while (0)
#define _cbitTest(s,n,v) (((s[(n)>>3]>>((n)&0x7))&0x1)==(v))
#define _cbitInv(s,n) (s[(n)>>3]^=0x1<<((n)&0x7))
typedef char* CBit;	// [type] Champ de bit
typedef char Bit;	// [type] bit
CBit cbitNouv(int n);
CBit cbitNouv0(int n);

CBit cbitClear(CBit s, int n);
void cbitDel(CBit s);
Bit cbitGet(CBit s, int n);
void cbitSet(CBit s, int n, char v);
Bit cbitTest(CBit s, int n, char v);
void cbitInv(CBit s, int n);
void cbitEff(CBit s, int n, char v);
void cbitS2R(CBit s, char *r, int n);
void cbitR2S(CBit s, char *r, int n);
void cbitAjout(CBit s1, CBit s2, int n1, int n2);
void cbitWuint(CBit s, int n, unsigned int x, int t);
unsigned int cbitRuint(CBit s, int n, int t);

// Mémoire
void* Malloc1(int i, int n);
void** Malloc2(int i, int j, int n);
void*** Malloc3(int i, int j, int k, int n);
void Free1(void* t, int i);
void Free2(void** t, int i, int j);
void Free3(void*** t, int i, int j, int k);

// Traitement des chaînes
char* formAlign3(char* s, char* unit);
char* stristr(char* str, char* tok);
int stribeg(char* str, char* tok);
char* makecaps(char* str);
char* makelower(char* str);
ubool findexp(char *txt, char *exp);
ubool findexp0(char *txt, char *exp);
void cstrput(char** str, char c);
void istrput(char** str, int i);
void dstrput(char** str, double f);
void sstrput(char** str, char* s);
void cstrget(char** str, char *c);
void istrget(char** str, int *i);
void dstrget(char** str, double *d);
void sstrget(char** str, char *s);

// Ecran, clavier
void ClearScreen(void);
unsigned char GetExtendedKey(void);
void printprc(double p, int l);

#ifdef __cplusplus
}
#endif

#endif



