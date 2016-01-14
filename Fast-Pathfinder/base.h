#ifndef __BASE_H__
#define __BASE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <math.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>

#ifdef __unix__
	#define linux 1
#endif

#ifdef linux
	// Dista ou Steed
	#define OS_RECONNU 1
	
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <ctype.h>
	#include <sys/types.h>
	#include <dirent.h>
	#include <unistd.h>
	#define stricmp strcasecmp
	#define strlwr makelower
#endif

#ifdef _WIN32
#ifndef WIN32
	#define WIN32
#endif
#endif

#ifdef WIN32
	#define DOS 1       // Pour les modes vidéos
	#define OS_RECONNU 1

	// DOS ou Windows
	#ifndef _MSC_VER
		// Tout sauf Microsoft Visual C++ 6.0
		#include <dirent.h>
		//#include <bios.h>
	#else
		#define snprintf _snprintf
	#endif
	#include <dos.h>
	#include <conio.h>
	#if _MSC_VER < 1400
		// Uniquement si VC++ < .NET 2005
		#include <windows.h>	// Beep()
	#endif
	#include <io.h> // _finddata_t
	#if _MSC_VER < 1400
		// Uniquement si VC++ < .NET 2005
		#include <winsock.h>
	#endif

	#define bcopy(from,to,len)      memmove(to,from,len)
	#define bzero(mem,len)          memset(mem,0,len)
	#define bcmp(a,b,len)           memcmp(a,b,len)
#endif

// __cplusplus WIN32

#define vrai (0==0)
#define faux (!vrai)
#define true (0==0)
#define false (!true)

#define Void	void
//#define bool unsigned char // Ne fait pas bon ménage avec la librairie std::
#define Bool unsigned char
#define Car    char
#define Nat    unsigned int
#define Ent    int
#define Reel   float
#define Rat    float
#define Chaine Car *

#define SIZEOF(x) ((Nat) (sizeof(x)))
#define MALLOC(t) ((t*) malloc(sizeof(t)))
#define MALLOCN(n, t) ((t*) malloc((n) * sizeof(t)))
#define CALLOCN(n, t) ((t*) calloc((n), sizeof(t)))
#define REALLOC(p,t,n)	((t*) realloc(p,(n)*sizeof(t)))
#define FREE(t) do { free(t); t = NULL; } while (0)
#define ZAP(t) do { delete(t); t = NULL; } while (0)
#define USE(t) t=t

// Création d'une matrice ixjxk du type t
#define MALLOC1(i, t) ((t*) Malloc1((i),sizeof(t)))
#define MALLOC2(i, j, t) ((t**) Malloc2((i),(j),sizeof(t)))
#define MALLOC3(i, j, k, t) ((t***) Malloc3((i),(j),(k),sizeof(t)))
// Destruction de la matrice t de dimension ixjxk
#define FREE1(t, i) Free1((void*)(t),(i))
#define FREE2(t, i, j) Free2((void**)(t),(i),(j))
#define FREE3(t, i, j, k) Free3((void***)(t),(i),(j),(k))

#define MIN(X,Y) (((X)<=(Y))?(X):(Y))
#define MAX(X,Y) (((X)>=(Y))?(X):(Y))
#define SQ(X) ((X)*(X))

#define RANDOMIZE srand((unsigned) time(NULL))
#define PI  3.1415926535897932385
#define EXP 2.7182818284590452354
#define CRLF "\x00D\x00A"

#endif // __BASE_H__


