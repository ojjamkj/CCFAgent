COMP=cl
LINK=link

ROOT_DIR=C:\Users\syk\workspace\BRMiner_engine\src
##########################################################################################
ORA_PATH=C:\Users\syk\workspace\BRMiner_engine\lib\win_64\oracle_10\precomp
ORA_LIB=$(ORA_PATH)\lib\oraSQL10.lib
#ORA_LIB=$(ORA_PATH)\lib\orasql11.lib

##########################################################################################
# Compile and link options.
Metis4RL_CF=-Z7 -Od -c -W3 -EHsc -DWIN32 -D_ORACLE -D_USE_CTX -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -I"$(ORA_PATH)\public" -I"$(ROOT_DIR)\Include"
LINKFLAG=-DLL -NOLOGO /NODEFAULTLIB:LIBCD /NODEFAULTLIB:LIBC

all : MROraAgt
MROraAgt :
	proc CODE=CPP CPP_SUFFIX=cpp thread=yes sqlcheck=syntax define=TRACE_DISABLED INAME=MROraAgt.pc ONAME=MROraAgt.cpp	
	
	$(COMP) $(Metis4RL_CF) -FoMROraAgt.obj MROraAgt.cpp
	
	$(LINK) $(LINKFLAG) -out:C:\Users\syk\workspace\BRMiner_engine\Build\Win32\BRMiner\release\ORA10G\MROraAgt.dll MROraAgt.obj $(ORA_LIB)
    
	del MROraAgt.obj
	del MROraAgt.exp MROraAgt.lib

