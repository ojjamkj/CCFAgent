 

DIR_HOME	= /home/cf/Dev


DIR_DEV		= $(DIR_HOME)/
DIR_SRC		= $(DIR_DEV)/Src
DIR_ADM  	= $(DIR_SRC)/Admin
DIR_CF  	= $(DIR_SRC)/CFAPI
DIR_RLIB  = $(DIR_SRC)/Library
DIR_RUNN  = $(DIR_SRC)/Runner
DIR_INC		= $(DIR_SRC)/Include
DIR_TMP		= $(DIR_DEV)/Tmp
DIR_OUT		= $(DIR_DEV)/Bin/LINUX/$(ORAVER)
 
ORA_INC		=  -I"$(DIR_INC)/openssl/"

INC_PATH	=  -I"$(DIR_INC)"  $(ORA_INC) 

INC_PATH        = -I"$(DIR_INC)"  $(ORA_INC) -I"/usr/include/oracle/11.2/client64/"

ORA_PATH=$(ORACLE_HOME)/precomp
ORA_LIB=  -L $(DIR_HOME)/lib/LINUX/ -lcrypto
 
 
#HP 
COMP=aCC
LINK=aCC
Metis4RL_CF=+DD64 -g -c -w -D__hpux -D_RUNNER -D_TRACE -D_USE_CTX -D_ORACLE -D_REENTRANT $(INC_PATH)
LNK_CF=+DD64 -O -o $(DIR_OUT)/cfagent -lelf
LIBS=$(ORA_LIB) -lpthread

#g++ SUN
 
#SUN 
COMP=g++
LINK=g++  
Metis4RL_CF=-m64 -g  -c  -D_REDHAT52 -D_RUNNER -D_TRACE -D_USE_CTX -D_ORACLE -D_REENTRANT $(INC_PATH)
Metis4RL_CF=-m64  -g -c  -D__GNUC__ -D_RUNNER -D_TRACE -D_USE_CTX -D_ORACLE -D_REENTRANT $(INC_PATH)
Metis4RL_CF=-m64  -g -c  -D__GNUC__ -D_RUNNER -D_TRACE -D_USE_CTX -D_ORACLE -D_REENTRANT $(INC_PATH)  -Wno-write-strings -g -Wno-format-security -lpthread 

LNK_CF=-m64   -O -o $(DIR_OUT)/cfagent -lelf -lsocket -ldl
LNK_CF=-m64   -O -o $(DIR_OUT)/cfagent -lelf -ldl -lnsl  -lm 

LNK_CF=-m64   -O -o $(DIR_OUT)/cfagent  -lpthread -lssl -lcrypto -lnsl -lm  -L/usr/local/lib -L/usr/lib64

#AIX
#COMP	= xlC_r
#LINK	= xlC_r
#Metis4RL_CF=-q64 -w -c -D_AIX  -D_RUNNER -D_TRACE -D_USE_CTX -D_ORACLE -D_REENTRANT $(INC_PATH)
#LNK_CF=-brtl  -q64 -O -o $(DIR_OUT)/brminer 
LIBS=$(ORA_LIB)   

#linux 
LIBS=$(ORA_LIB) -ldl -lpthread 
COMP=g++
LINK=g++
#LNK_CF=-m64 -O -o $(DIR_OUT)/cfagent
Metis4RL_CF=-g -c  -m64 -D__GNUC__ -D_RUNNER -D_TRACE -D_USE_CTX -D_ORACLE -D_REENTRANT $(INC_PATH) -Wpointer-arith -Wno-write-strings -g -Wno-format-security -lpthread 

OBJS=BREXDiag.o \
	CFAPI.o \
	BREXListen.o \
	BREXLog.o \
	BRMiner.o \
	BRStart.o \
	MRADMIN.o \
	MRAgtMgr.o \
	MRConfig.o \
	MRCSMgr.o \
	MRCSQueu.o \
	MRCSThr.o \
	BREXThread.o \
	BREXThreadrun.o \
	BRMObj.o \
	MTConvt.o \
	MTCtSect.o \
	MTExcept.o \
	MTPrfile.o \
	MTUtil.o \
	BRTrace.o \
	MRARRFN.o \
	MRBRINTR.o \
	MRCALC.o \
	MRCRINTR.o \
	MRDTLIST.o \
	MREXPRES.o \
	MREXPSUB.o \
	MRFRINTR.o \
	MRFUNCTN.o \
	MRGBLVAR.o \
	MRLODDT.o \
	MRLODRUL.o \
	MRPARAM.o \
	MRPARSE.o \
	MRPGMEXP.o \
	MRQRINTR.o \
	MRQryRun.o \
	MRRUNNER.o \
	MRSEARCH.o \
	MRSFUNC.o \
	MRSTARES.o \
	MRToken.o \
	MRTRINTR.o

all : $(OBJS)
	$(LINK) $(LNK_CF) $(OBJS) $(LIBS)
	@echo "		====================================="
	@echo "		 Completed Execute File [cfagent]  "
	@echo "		====================================="

CFAPI.o : $(DIR_CF)/CFAPI.cpp  
	$(COMP) $(Metis4RL_CF) -o CFAPI.o $(DIR_CF)/CFAPI.cpp

BREXDiag.o : $(DIR_ADM)/BREXDiag.cpp  
	$(COMP) $(Metis4RL_CF) -o BREXDiag.o $(DIR_ADM)/BREXDiag.cpp

BREXListen.o : $(DIR_ADM)/BREXListen.cpp  
	$(COMP) $(Metis4RL_CF) -o BREXListen.o $(DIR_ADM)/BREXListen.cpp

BREXLog.o : $(DIR_ADM)/BREXLog.cpp  
	$(COMP) $(Metis4RL_CF) -o BREXLog.o $(DIR_ADM)/BREXLog.cpp

BRMiner.o : $(DIR_ADM)/BRMiner.cpp  
	$(COMP) $(Metis4RL_CF) -o BRMiner.o $(DIR_ADM)/BRMiner.cpp

BRStart.o : $(DIR_ADM)/BRStart.cpp  
	$(COMP) $(Metis4RL_CF) -o BRStart.o $(DIR_ADM)/BRStart.cpp

MRADMIN.o : $(DIR_ADM)/MRADMIN.cpp  
	$(COMP) $(Metis4RL_CF) -o MRADMIN.o $(DIR_ADM)/MRADMIN.cpp

MRAgtMgr.o : $(DIR_ADM)/MRAgtMgr.cpp  
	$(COMP) $(Metis4RL_CF) -o MRAgtMgr.o $(DIR_ADM)/MRAgtMgr.cpp

MRConfig.o : $(DIR_ADM)/MRConfig.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRConfig.o $(DIR_ADM)/MRConfig.cpp

MRCSMgr.o : $(DIR_ADM)/MRCSMgr.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRCSMgr.o $(DIR_ADM)/MRCSMgr.cpp

MRCSQueu.o : $(DIR_ADM)/MRCSQueu.cpp  
	$(COMP) $(Metis4RL_CF) -o MRCSQueu.o $(DIR_ADM)/MRCSQueu.cpp

MRCSThr.o : $(DIR_ADM)/MRCSThr.cpp  
	$(COMP) $(Metis4RL_CF) -o MRCSThr.o $(DIR_ADM)/MRCSThr.cpp

#LIB

BREXThread.o : $(DIR_RLIB)/BREXThread.cpp  
	$(COMP) $(Metis4RL_CF) -o BREXThread.o $(DIR_RLIB)/BREXThread.cpp

BREXThreadrun.o : $(DIR_RLIB)/BREXThreadrun.cpp  
	$(COMP) $(Metis4RL_CF) -o BREXThreadrun.o $(DIR_RLIB)/BREXThreadrun.cpp

BRMObj.o : $(DIR_RLIB)/BRMObj.cpp  
	$(COMP) $(Metis4RL_CF) -o BRMObj.o $(DIR_RLIB)/BRMObj.cpp


MTConvt.o :	$(DIR_RLIB)/MTConvt.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o  MTConvt.o $(DIR_RLIB)/MTConvt.cpp

MTCtSect.o : $(DIR_RLIB)/MTCtSect.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MTCtSect.o $(DIR_RLIB)/MTCtSect.cpp

MTExcept.o : $(DIR_RLIB)/MTExcept.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MTExcept.o $(DIR_RLIB)/MTExcept.cpp

MTPrfile.o :	$(DIR_RLIB)/MTPrfile.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MTPrfile.o $(DIR_RLIB)/MTPrfile.cpp

MTUtil.o :	$(DIR_RLIB)/MTUtil.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MTUtil.o $(DIR_RLIB)/MTUtil.cpp

#Runner
 
BRTrace.o :	$(DIR_RUNN)/BRTrace.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o BRTrace.o $(DIR_RUNN)/BRTrace.cpp
	
MRARRFN.o :	$(DIR_RUNN)/MRARRFN.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRARRFN.o $(DIR_RUNN)/MRARRFN.cpp
	
MRBRINTR.o :	$(DIR_RUNN)/MRBRINTR.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRBRINTR.o $(DIR_RUNN)/MRBRINTR.cpp
	
MRCALC.o :	$(DIR_RUNN)/MRCALC.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRCALC.o $(DIR_RUNN)/MRCALC.cpp
		
MRCRINTR.o :	$(DIR_RUNN)/MRCRINTR.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRCRINTR.o $(DIR_RUNN)/MRCRINTR.cpp
	
MRDTLIST.o :	$(DIR_RUNN)/MRDTLIST.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRDTLIST.o $(DIR_RUNN)/MRDTLIST.cpp
	
MREXPRES.o :	$(DIR_RUNN)/MREXPRES.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MREXPRES.o $(DIR_RUNN)/MREXPRES.cpp
	
MREXPSUB.o :	$(DIR_RUNN)/MREXPSUB.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MREXPSUB.o $(DIR_RUNN)/MREXPSUB.cpp
	
MRFRINTR.o :	$(DIR_RUNN)/MRFRINTR.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRFRINTR.o $(DIR_RUNN)/MRFRINTR.cpp
	
MRFUNCTN.o :	$(DIR_RUNN)/MRFUNCTN.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRFUNCTN.o $(DIR_RUNN)/MRFUNCTN.cpp
	
MRGBLVAR.o :	$(DIR_RUNN)/MRGBLVAR.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRGBLVAR.o $(DIR_RUNN)/MRGBLVAR.cpp
	
MRLODDT.o :	$(DIR_RUNN)/MRLODDT.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRLODDT.o $(DIR_RUNN)/MRLODDT.cpp

MRLODRUL.o : $(DIR_RUNN)/MRLODRUL.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRLODRUL.o MRLODRUL.cpp
		
MRPARAM.o :	$(DIR_RUNN)/MRPARAM.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRPARAM.o $(DIR_RUNN)/MRPARAM.cpp
	
MRPARSE.o :	$(DIR_RUNN)/MRPARSE.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRPARSE.o $(DIR_RUNN)/MRPARSE.cpp
	
MRPGMEXP.o :	$(DIR_RUNN)/MRPGMEXP.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRPGMEXP.o $(DIR_RUNN)/MRPGMEXP.cpp
	
MRQRINTR.o :	$(DIR_RUNN)/MRQRINTR.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRQRINTR.o $(DIR_RUNN)/MRQRINTR.cpp
	
MRQryRun.o :	$(DIR_RUNN)/MRQryRun.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRQryRun.o $(DIR_RUNN)/MRQryRun.cpp
	
MRRUNNER.o :	$(DIR_RUNN)/MRRUNNER.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRRUNNER.o $(DIR_RUNN)/MRRUNNER.cpp
	
MRSEARCH.o :	$(DIR_RUNN)/MRSEARCH.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRSEARCH.o $(DIR_RUNN)/MRSEARCH.cpp
	
MRSFUNC.o :	$(DIR_RUNN)/MRSFUNC.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRSFUNC.o $(DIR_RUNN)/MRSFUNC.cpp
	
MRSTARES.o :	$(DIR_RUNN)/MRSTARES.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRSTARES.o $(DIR_RUNN)/MRSTARES.cpp
	
MRToken.o :	$(DIR_RUNN)/MRToken.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRToken.o $(DIR_RUNN)/MRToken.cpp
	
MRTRINTR.o :	$(DIR_RUNN)/MRTRINTR.cpp $(DIR_INC)/*.h
	$(COMP) $(Metis4RL_CF) -o MRTRINTR.o $(DIR_RUNN)/MRTRINTR.cpp
	
