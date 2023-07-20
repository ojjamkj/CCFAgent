COMP=cl
LINK=link

ROOT_DIR=C:\Users\syk\workspace\BRMiner_engine\src
##########################################################################################
ORA_PATH=C:\app\syk\product\11.2.0\dbhome_2\precomp
#ORA_LIB=$(ORA_PATH)\lib\oraSQL10.lib
ORA_LIB=$(ORA_PATH)\lib\orasql11.lib

##########################################################################################
# Compile and link options.
Metis4RL_CF=-Z7 -Od -c -W3 -EHsc -DWIN32 -D_ORACLE -D_USE_CTX -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -I"$(ORA_PATH)\public" -I"$(ROOT_DIR)\Include"
LINKFLAG=-DLL -NOLOGO /NODEFAULTLIB:LIBCD /NODEFAULTLIB:LIBC

all : MROraAgt
MROraAgt :
	rem proc DBMS=V8 UNSAFE_NULL=YES CODE=CPP SQLCHECK=FULL RELEASE_CURSOR=YES USERID=BRM411/gtone@BRMINER INAME=MRLODRUL.pc ONAME=ORA\MRLODRUL.cpp
	proc CODE=CPP CPP_SUFFIX=cpp thread=yes sqlcheck=syntax define=TRACE_DISABLED 	INAME=MRLODRUL.pc ONAME=ORA\MRLODRUL.cpp