#ifndef	__INC_MRDBAGT
#define	__INC_MRDBAGT

extern "C"
#if defined(WIN32)
   __declspec(dllexport)
#endif
	int INITAGENT( void *, void*, char *msg );

		
extern "C"
#if defined(WIN32)
   __declspec(dllexport)
#endif
//   int EXECSQLCUROPEN( void *, void *, char *stmt, char *msg );
   int EXECSQLCUROPEN( void *, void *, char *stmt, int nVal, char *Val[], char *msg );
   
extern "C"
#if defined(WIN32)
   __declspec(dllexport)
#endif
   int EXECSQLCURCLOSE( void *, char *dummy1, char *dummy2 );

extern "C"
#if defined(WIN32)
   __declspec(dllexport)
#endif
   int EXECSQLFETCH( void *, char *res, char *msg );
   
   		
extern "C"
#if defined(WIN32)
   __declspec(dllexport)
#endif
//	int EXECSQLCUD( void *, void *, char *stmt, char *res );
	int EXECSQLCUD( void *, void *, char *stmt, int nVal, char *Val[], char *res );

extern "C"
#if defined(WIN32)
   __declspec(dllexport)
#endif
	int TERMAGENT( void *, char *msg, char *dummy );

#endif
