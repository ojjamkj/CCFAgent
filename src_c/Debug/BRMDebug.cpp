#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include    "BRMStart.h"

extern "C" __declspec(dllexport) int InitDebug( char *mode, int ruleId, char *appDate, char *msg  );
extern "C" __declspec(dllexport) int TermDebug();
extern "C" __declspec(dllexport) int NextStep( int *N1, int *N2, char *buf  );
extern "C" __declspec(dllexport) void InitPath(char *path );


BRDebugger	*m_pWorker;
int			g_dbg_step;
int			g_dbg_n1;
int			g_dbg_n2;
char		*g_dbg_buf;
char         filepath[1024];

void InitPath(char *path)
{
	memset(filepath,  0x00,sizeof(filepath));
	sprintf(filepath,"%s\\", path);
	printf("[SO] InitPath [%s]\n", InitPath);
}

int InitDebug( char *mode, int ruleId, char *appDate, char *msg )
{
	g_dbg_buf = (char*) malloc ( 1000 );
	g_dbg_step = 0;
	msg[0] = 0;

	m_pWorker = new BRDebugger( *mode, ruleId, appDate );

	if ( ! m_pWorker ) {
		strcpy( msg, "Debugger create error." );
		return _DBG_STEP_ERR;
	}

	if ( g_dbg_step != 0 ) {
		strcpy( msg, g_dbg_buf );
	}

	return g_dbg_step;
}


int TermDebug()
{
printf("[SO] TermDebug begin\n");

	if ( g_dbg_buf ) free(g_dbg_buf);
	if ( m_pWorker ) delete m_pWorker;

printf("[SO] TermDebug end\n");
	g_dbg_buf = 0;
	m_pWorker = 0;

	return 0;
}


int NextStep( int *N1, int *N2, char *buf )
{
printf("[SO] bef NextStep\n");

	m_pWorker->NextStep();
printf("[SO] STEP %d : %d %d [%s]\n", g_dbg_step, g_dbg_n1, g_dbg_n2, g_dbg_buf );

	*N1 = g_dbg_n1;
	*N2 = g_dbg_n2;
	strcpy( buf, g_dbg_buf );

	return g_dbg_step;
}

