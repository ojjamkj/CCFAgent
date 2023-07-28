#ifndef   _INC_MRDEFINE
#define   _INC_MRDEFINE

#ifdef _WIN32
	#define   PATH_DELIM   '\\'
#else
	#define   PATH_DELIM   '/'
#endif


#if	defined(WIN32)
//  #define 	DEFAULT_INIFILE             "C:\\eclise_Engine\\W_BR\\BRMiner_engine4.5\\Bin\\WIN\\ORA11_64\\ABL60.INI"
    #define 	DEFAULT_INIFILE             ".\\agent.properties"

#elif defined(_OS390) || defined(_ZOS)
    #define DEFAULT_INIFILE             "DD:BRMCONF"
#else
    #define DEFAULT_INIFILE             "./agent.properties"
#endif


#define     LOCAL_DELIMITER_LEFT   '$'
#define     LOCAL_DELIMITER_RIGHT  '$'

#define     RULE_DELIMITER_LEFT   '{'
#define     RULE_DELIMITER_RIGHT  '}'

#ifdef _OS390
   #define     ITEM_DELIMITER_LEFT   0x70
   #define     ITEM_DELIMITER_RIGHT  0x80
#else
   #define     ITEM_DELIMITER_LEFT   '['
   #define     ITEM_DELIMITER_RIGHT  ']'
#endif

#endif    /*  _INC_MRDEFINE   */
