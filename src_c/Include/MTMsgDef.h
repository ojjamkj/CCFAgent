#ifndef     _INC_MTMSGDEF
#define     _INC_MTMSGDEF

/*  RC  */
#define MRC_OK                      0  /* 정상                     */
#define MRC_SYS_ERR                 1  /* 시스템 오류				*/
#define MRC_DB_ERR                  2  /* DB 오류( Rule에서 비사용 )*/
#define MRC_INPUT_ERR               3  /* 사용자 입력 오류          */
#define MRC_RULE_ERR                4  /* Rule InValid             */

/* LOG LEVEL   */
#define     LOGLEVEL_ERROR               3
#define     LOGLEVEL_SYSINFO             2
#define     LOGLEVEL_USERINFO            1
#define     LOGLEVEL_ALL                 LOGLEVEL_USERINFO

/* MSG LEVEL   */
#define     SYSERR_                      4
#define     USERERR_                     3
#define     SYSINFO_                     2
#define     USERINFO_                    1

/* SYSERR_ MESSAGE   */
#define     SYSERR_BASE                                  4000000
/*
            THREAD ERROR                                 4010000
            SOCKET ERROR                                 4020000
            FILE   ERROR                                 4030000
*/

#ifdef _MSG_KO
	#define     USERERR_BASE                                 3000000
	#define     USERERR_PARSE               ( USERERR_BASE +   10000 )
	#define     EMSG_PARSE                  "구문 오류!"
	#define     USERERR_GRAMMAR             ( USERERR_BASE +   20000 )
	#define     EMSG_GRAMMAR                "문법적 오류!"
	#define     USERERR_RUN                 ( USERERR_BASE +   30000 )
	#define     EMSG_RUN                    "실행 오류!"
#else
#define     USERERR_BASE                                 3000000
#define     USERERR_PARSE               ( USERERR_BASE +   10000 )
#define     EMSG_PARSE_01                  "Syntax error!(01)"
#define     EMSG_PARSE_02                  "Syntax error!(02)"
#define     EMSG_PARSE_03                  "Syntax error!(03)"
#define     EMSG_PARSE_04                  "Syntax error!(04)"
#define     EMSG_PARSE_05                  "Syntax error!(05)"
#define     EMSG_PARSE_06                  "Syntax error!(06)"
#define     EMSG_PARSE_07                  "Syntax error!(07)"
#define     EMSG_PARSE_08                  "Syntax error!(08)"
#define     EMSG_PARSE_09                  "Syntax error!(09)"
#define     EMSG_PARSE_10                  "Syntax error!(10)"
#define     EMSG_PARSE                  "Syntax error!"
#define     USERERR_GRAMMAR             		( USERERR_BASE +   20000 )
#define     USERERR_GRAMMAR_01             	( USERERR_GRAMMAR +   001 )
#define     USERERR_GRAMMAR_02	( USERERR_GRAMMAR +   002 )
#define     USERERR_GRAMMAR_03	( USERERR_GRAMMAR +   003 )
#define     USERERR_GRAMMAR_04	( USERERR_GRAMMAR +   004 )
#define     USERERR_GRAMMAR_05	( USERERR_GRAMMAR +   005 )
#define     USERERR_GRAMMAR_06	( USERERR_GRAMMAR +   006 )
#define     USERERR_GRAMMAR_07	( USERERR_GRAMMAR +   007 )
#define     USERERR_GRAMMAR_08	( USERERR_GRAMMAR +   008 )
#define     USERERR_GRAMMAR_09	( USERERR_GRAMMAR +   009 )
#define     USERERR_GRAMMAR_10	( USERERR_GRAMMAR +   010 )
#define     USERERR_GRAMMAR_11	( USERERR_GRAMMAR +   011 )
#define     USERERR_GRAMMAR_12	( USERERR_GRAMMAR +   012 )
#define     USERERR_GRAMMAR_13	( USERERR_GRAMMAR +   013 )
#define     USERERR_GRAMMAR_14	( USERERR_GRAMMAR +   014 )
#define     USERERR_GRAMMAR_15	( USERERR_GRAMMAR +   015 )
#define     USERERR_GRAMMAR_16	( USERERR_GRAMMAR +   016 )
#define     USERERR_GRAMMAR_17	( USERERR_GRAMMAR +   017 )
#define     USERERR_GRAMMAR_18	( USERERR_GRAMMAR +   018 )
#define     USERERR_GRAMMAR_19	( USERERR_GRAMMAR +   019 )
#define     USERERR_GRAMMAR_20	( USERERR_GRAMMAR +   020 )
#define     USERERR_GRAMMAR_21	( USERERR_GRAMMAR +   021 )
#define     USERERR_GRAMMAR_22	( USERERR_GRAMMAR +   022 )
#define     USERERR_GRAMMAR_23	( USERERR_GRAMMAR +   023 )
#define     USERERR_GRAMMAR_24	( USERERR_GRAMMAR +   024 )
#define     USERERR_GRAMMAR_25	( USERERR_GRAMMAR +   025 )
#define     USERERR_GRAMMAR_26	( USERERR_GRAMMAR +   026 )
#define     USERERR_GRAMMAR_27	( USERERR_GRAMMAR +   027 )
#define     USERERR_GRAMMAR_28	( USERERR_GRAMMAR +   028 )
#define     USERERR_GRAMMAR_29	( USERERR_GRAMMAR +   029 )
#define     USERERR_GRAMMAR_30	( USERERR_GRAMMAR +   030 )
#define     USERERR_GRAMMAR_31	( USERERR_GRAMMAR +   031 )
#define     USERERR_GRAMMAR_32	( USERERR_GRAMMAR +   032 )
#define     USERERR_GRAMMAR_33	( USERERR_GRAMMAR +   033 )
#define     USERERR_GRAMMAR_34	( USERERR_GRAMMAR +   034 )
#define     USERERR_GRAMMAR_35	( USERERR_GRAMMAR +   035 )
#define     USERERR_GRAMMAR_36	( USERERR_GRAMMAR +   036 )
#define     USERERR_GRAMMAR_37	( USERERR_GRAMMAR +   037 )
#define     USERERR_GRAMMAR_38	( USERERR_GRAMMAR +   038 )
#define     USERERR_GRAMMAR_39	( USERERR_GRAMMAR +   039 )
#define     USERERR_GRAMMAR_40	( USERERR_GRAMMAR +   040 )
#define     USERERR_GRAMMAR_41	( USERERR_GRAMMAR +   041 )
#define     USERERR_GRAMMAR_42	( USERERR_GRAMMAR +   042 )
#define     USERERR_GRAMMAR_43	( USERERR_GRAMMAR +   043 )
#define     USERERR_GRAMMAR_44	( USERERR_GRAMMAR +   044 )
#define     USERERR_GRAMMAR_45	( USERERR_GRAMMAR +   045 )
#define     USERERR_GRAMMAR_46	( USERERR_GRAMMAR +   046 )
#define     USERERR_GRAMMAR_47	( USERERR_GRAMMAR +   047 )
#define     USERERR_GRAMMAR_48	( USERERR_GRAMMAR +   048 )
#define     USERERR_GRAMMAR_49	( USERERR_GRAMMAR +   049 )
#define     USERERR_GRAMMAR_50	( USERERR_GRAMMAR +   050 )
#define     USERERR_GRAMMAR_51	( USERERR_GRAMMAR +   051 )
#define     USERERR_GRAMMAR_52	( USERERR_GRAMMAR +   052 )
#define     USERERR_GRAMMAR_53	( USERERR_GRAMMAR +   053 )
#define     USERERR_GRAMMAR_54	( USERERR_GRAMMAR +   054 )
#define     USERERR_GRAMMAR_55	( USERERR_GRAMMAR +   055 )
#define     USERERR_GRAMMAR_56	( USERERR_GRAMMAR +   056 )
#define     USERERR_GRAMMAR_57	( USERERR_GRAMMAR +   057 )
#define     USERERR_GRAMMAR_58	( USERERR_GRAMMAR +   058 )
#define     USERERR_GRAMMAR_59	( USERERR_GRAMMAR +   059 )
#define     USERERR_GRAMMAR_60	( USERERR_GRAMMAR +   060 )
#define     USERERR_GRAMMAR_61	( USERERR_GRAMMAR +   061 )
#define     USERERR_GRAMMAR_62	( USERERR_GRAMMAR +   062 )
#define     USERERR_GRAMMAR_63	( USERERR_GRAMMAR +   063 )
#define     USERERR_GRAMMAR_64	( USERERR_GRAMMAR +   064 )
#define     USERERR_GRAMMAR_65	( USERERR_GRAMMAR +   065 )
#define     USERERR_GRAMMAR_66	( USERERR_GRAMMAR +   066 )
#define     USERERR_GRAMMAR_67	( USERERR_GRAMMAR +   067 )
#define     USERERR_GRAMMAR_68	( USERERR_GRAMMAR +   068 )
#define     USERERR_GRAMMAR_69	( USERERR_GRAMMAR +   069 )
#define     USERERR_GRAMMAR_70	( USERERR_GRAMMAR +   070 )
#define     USERERR_GRAMMAR_71	( USERERR_GRAMMAR +   071 )
#define     USERERR_GRAMMAR_72	( USERERR_GRAMMAR +   072 )
#define     USERERR_GRAMMAR_73	( USERERR_GRAMMAR +   073 )
#define     USERERR_GRAMMAR_74	( USERERR_GRAMMAR +   074 )
#define     USERERR_GRAMMAR_75	( USERERR_GRAMMAR +   075 )
#define     USERERR_GRAMMAR_76	( USERERR_GRAMMAR +   076 )
#define     USERERR_GRAMMAR_77	( USERERR_GRAMMAR +   077 )
#define     USERERR_GRAMMAR_78	( USERERR_GRAMMAR +   078 )
#define     USERERR_GRAMMAR_79	( USERERR_GRAMMAR +   079 )
#define     USERERR_GRAMMAR_80	( USERERR_GRAMMAR +   080 )
#define     USERERR_GRAMMAR_81	( USERERR_GRAMMAR +   081 )
#define     USERERR_GRAMMAR_82	( USERERR_GRAMMAR +   082 )
#define     USERERR_GRAMMAR_83	( USERERR_GRAMMAR +   083 )
#define     USERERR_GRAMMAR_84	( USERERR_GRAMMAR +   084 )
#define     USERERR_GRAMMAR_85	( USERERR_GRAMMAR +   085 )
#define     USERERR_GRAMMAR_86	( USERERR_GRAMMAR +   086 )
#define     USERERR_GRAMMAR_87	( USERERR_GRAMMAR +   087 )
#define     USERERR_GRAMMAR_88	( USERERR_GRAMMAR +   088 )
#define     USERERR_GRAMMAR_89	( USERERR_GRAMMAR +   089 )
#define     USERERR_GRAMMAR_90	( USERERR_GRAMMAR +   090 )
#define     USERERR_GRAMMAR_91	( USERERR_GRAMMAR +   091 )
#define     USERERR_GRAMMAR_92	( USERERR_GRAMMAR +   092 )
#define     USERERR_GRAMMAR_93	( USERERR_GRAMMAR +   093 )
#define     USERERR_GRAMMAR_94	( USERERR_GRAMMAR +   094 )
#define     USERERR_GRAMMAR_95	( USERERR_GRAMMAR +   095 )
#define     USERERR_GRAMMAR_96	( USERERR_GRAMMAR +   096 )
#define     USERERR_GRAMMAR_97	( USERERR_GRAMMAR +   097 )
#define     USERERR_GRAMMAR_98	( USERERR_GRAMMAR +   098 )
#define     USERERR_GRAMMAR_99	( USERERR_GRAMMAR +   099 )
#define     USERERR_GRAMMAR_100	( USERERR_GRAMMAR +   100 )
#define     USERERR_GRAMMAR_101	( USERERR_GRAMMAR +   101 )
#define     USERERR_GRAMMAR_102	( USERERR_GRAMMAR +   102 )
#define     USERERR_GRAMMAR_103	( USERERR_GRAMMAR +   103 )
#define     USERERR_GRAMMAR_104	( USERERR_GRAMMAR +   104 )
#define     USERERR_GRAMMAR_105	( USERERR_GRAMMAR +   105 )
#define     USERERR_GRAMMAR_106	( USERERR_GRAMMAR +   106 )
#define     USERERR_GRAMMAR_107	( USERERR_GRAMMAR +   107 )
#define     USERERR_GRAMMAR_108	( USERERR_GRAMMAR +   108 )
#define     USERERR_GRAMMAR_109	( USERERR_GRAMMAR +   109 )
#define     USERERR_GRAMMAR_110	( USERERR_GRAMMAR +   110 )
#define     USERERR_GRAMMAR_111	( USERERR_GRAMMAR +   111 )
#define     USERERR_GRAMMAR_112	( USERERR_GRAMMAR +   112 )
#define     USERERR_GRAMMAR_113	( USERERR_GRAMMAR +   113 )
#define     USERERR_GRAMMAR_114	( USERERR_GRAMMAR +   114 )
#define     USERERR_GRAMMAR_115	( USERERR_GRAMMAR +   115 )
#define     USERERR_GRAMMAR_116	( USERERR_GRAMMAR +   116 )
#define     USERERR_GRAMMAR_117	( USERERR_GRAMMAR +   117 )
#define     USERERR_GRAMMAR_118	( USERERR_GRAMMAR +   118 )
#define     USERERR_GRAMMAR_119	( USERERR_GRAMMAR +   119 )
#define     USERERR_GRAMMAR_120	( USERERR_GRAMMAR +   120 )
#define     USERERR_GRAMMAR_121	( USERERR_GRAMMAR +   121 )
#define     USERERR_GRAMMAR_122	( USERERR_GRAMMAR +   122 )
#define     USERERR_GRAMMAR_123	( USERERR_GRAMMAR +   123 )
#define     USERERR_GRAMMAR_124	( USERERR_GRAMMAR +   124 )
#define     USERERR_GRAMMAR_125	( USERERR_GRAMMAR +   125 )
#define     USERERR_GRAMMAR_126	( USERERR_GRAMMAR +   126 )
#define     USERERR_GRAMMAR_127	( USERERR_GRAMMAR +   127 )
#define     USERERR_GRAMMAR_128	( USERERR_GRAMMAR +   128 )
#define     USERERR_GRAMMAR_129	( USERERR_GRAMMAR +   129 )
#define     USERERR_GRAMMAR_130	( USERERR_GRAMMAR +   130 )
#define     USERERR_GRAMMAR_131	( USERERR_GRAMMAR +   131 )
#define     USERERR_GRAMMAR_132	( USERERR_GRAMMAR +   132 )
#define     USERERR_GRAMMAR_133	( USERERR_GRAMMAR +   133 )
#define     USERERR_GRAMMAR_134	( USERERR_GRAMMAR +   134 )
#define     USERERR_GRAMMAR_135	( USERERR_GRAMMAR +   135 )
#define     USERERR_GRAMMAR_136	( USERERR_GRAMMAR +   136 )
#define     USERERR_GRAMMAR_137	( USERERR_GRAMMAR +   137 )
#define     USERERR_GRAMMAR_138	( USERERR_GRAMMAR +   138 )
#define     USERERR_GRAMMAR_139	( USERERR_GRAMMAR +   139 )
#define     USERERR_GRAMMAR_140	( USERERR_GRAMMAR +   140 )

#define     EMSG_GRAMMAR_01                "Grammatical error!_01"
#define     EMSG_GRAMMAR_02                "Grammatical error!_02"
#define     EMSG_GRAMMAR_03                "Grammatical error!_03"
#define     EMSG_GRAMMAR_04                "Grammatical error!_04"
#define     EMSG_GRAMMAR_05                "Grammatical error!_05"
#define     EMSG_GRAMMAR_06                "Grammatical error!_06"
#define     EMSG_GRAMMAR_07                "Grammatical error!_07"
#define     EMSG_GRAMMAR_08                "Grammatical error!_08"
#define     EMSG_GRAMMAR_09                "Grammatical error!_09"
#define     EMSG_GRAMMAR_10                "Grammatical error!_10"
#define     EMSG_GRAMMAR_11                "Grammatical error!_11"
#define     EMSG_GRAMMAR_12                "Grammatical error!_12"
#define     EMSG_GRAMMAR_13                "Grammatical error!_13"
#define     EMSG_GRAMMAR_14                "Grammatical error!_14"
#define     EMSG_GRAMMAR_15                "Grammatical error!_15"
#define     EMSG_GRAMMAR_16                "Grammatical error!_16"
#define     EMSG_GRAMMAR_17                "Grammatical error!_17"
#define     EMSG_GRAMMAR_18                "Grammatical error!_18"
#define     EMSG_GRAMMAR_19                "Grammatical error!_19"
#define     EMSG_GRAMMAR_20                "Grammatical error!_20"
#define     EMSG_GRAMMAR_21                "Grammatical error!_21"
#define     EMSG_GRAMMAR_22                "Grammatical error!_22"
#define     EMSG_GRAMMAR_23                "Grammatical error!_23"
#define     EMSG_GRAMMAR_24                "Grammatical error!_24"
#define     EMSG_GRAMMAR_25                "Grammatical error!_25"
#define     EMSG_GRAMMAR_26                "Grammatical error!_26"
#define     EMSG_GRAMMAR_27                "Grammatical error!_27"
#define     EMSG_GRAMMAR_28                "Grammatical error!_28"
#define     EMSG_GRAMMAR_29                "Grammatical error!_29"
#define     EMSG_GRAMMAR_30                "Grammatical error!_30"
#define     EMSG_GRAMMAR_31                "Grammatical error!_31"
#define     EMSG_GRAMMAR_32                "Grammatical error!_32"
#define     EMSG_GRAMMAR_33                "Grammatical error!_33"
#define     EMSG_GRAMMAR_34                "Grammatical error!_34"
#define     EMSG_GRAMMAR_35                "Grammatical error!_35"
#define     EMSG_GRAMMAR_36                "Grammatical error!_36"
#define     EMSG_GRAMMAR_37                "Grammatical error!_37"
#define     EMSG_GRAMMAR_38                "Grammatical error!_38"
#define     EMSG_GRAMMAR_39                "Grammatical error!_39"
#define     EMSG_GRAMMAR_40                "Grammatical error!_40"
#define     EMSG_GRAMMAR_41                "Grammatical error!_41"
#define     EMSG_GRAMMAR_42                "Grammatical error!_42"
#define     EMSG_GRAMMAR_43                "Grammatical error!_43"
#define     EMSG_GRAMMAR_44                "Grammatical error!_44"
#define     EMSG_GRAMMAR_45                "Grammatical error!_45"
#define     EMSG_GRAMMAR_46                "Grammatical error!_46"
#define     EMSG_GRAMMAR_47                "Grammatical error!_47"
#define     EMSG_GRAMMAR_48                "Grammatical error!_48"
#define     EMSG_GRAMMAR_49                "Grammatical error!_49"
#define     EMSG_GRAMMAR_50                "Grammatical error!_50"
#define     EMSG_GRAMMAR_51                "Grammatical error!_51"
#define     EMSG_GRAMMAR_52                "Grammatical error!_52"
#define     EMSG_GRAMMAR_53                "Grammatical error!_53"
#define     EMSG_GRAMMAR_54                 "Grammatical error!_54 "
#define     EMSG_GRAMMAR_55                 "Grammatical error!_55 "
#define     EMSG_GRAMMAR_56                 "Grammatical error!_56 "
#define     EMSG_GRAMMAR_57                 "Grammatical error!_57 "
#define     EMSG_GRAMMAR_58                 "Grammatical error!_58 "
#define     EMSG_GRAMMAR_59                 "Grammatical error!_59 "
#define     EMSG_GRAMMAR_60                 "Grammatical error!_60 "
#define     EMSG_GRAMMAR_61                 "Grammatical error!_61 "
#define     EMSG_GRAMMAR_62                 "Grammatical error!_62 "
#define     EMSG_GRAMMAR_63                 "Grammatical error!_63 "
#define     EMSG_GRAMMAR_64                 "Grammatical error!_64 "
#define     EMSG_GRAMMAR_65                 "Grammatical error!_65 "
#define     EMSG_GRAMMAR_66                 "Grammatical error!_66 "
#define     EMSG_GRAMMAR_67                 "Grammatical error!_67 "
#define     EMSG_GRAMMAR_68                 "Grammatical error!_68 "
#define     EMSG_GRAMMAR_69                 "Grammatical error!_69 "
#define     EMSG_GRAMMAR_70                 "Grammatical error!_70 "
#define     EMSG_GRAMMAR_71                 "Grammatical error!_71 "
#define     EMSG_GRAMMAR_72                 "Grammatical error!_72 "
#define     EMSG_GRAMMAR_73                 "Grammatical error!_73 "
#define     EMSG_GRAMMAR_74                 "Grammatical error!_74 "
#define     EMSG_GRAMMAR_75                 "Grammatical error!_75 "
#define     EMSG_GRAMMAR_76                 "Grammatical error!_76 "
#define     EMSG_GRAMMAR_77                 "Grammatical error!_77 "
#define     EMSG_GRAMMAR_78                 "Grammatical error!_78 "
#define     EMSG_GRAMMAR_79                 "Grammatical error!_79 "
#define     EMSG_GRAMMAR_80                 "Grammatical error!_80 "
#define     EMSG_GRAMMAR_81                 "Grammatical error!_81 "
#define     EMSG_GRAMMAR_82                 "Grammatical error!_82 "
#define     EMSG_GRAMMAR_83                 "Grammatical error!_83 "
#define     EMSG_GRAMMAR_84                 "Grammatical error!_84 "
#define     EMSG_GRAMMAR_85                 "Grammatical error!_85 "
#define     EMSG_GRAMMAR_86                 "Grammatical error!_86 "
#define     EMSG_GRAMMAR_87                 "Grammatical error!_87 "
#define     EMSG_GRAMMAR_88                 "Grammatical error!_88 "
#define     EMSG_GRAMMAR_89                 "Grammatical error!_89 "
#define     EMSG_GRAMMAR_90                 "Grammatical error!_90 "
#define     EMSG_GRAMMAR_91                 "Grammatical error!_91 "
#define     EMSG_GRAMMAR_92                 "Grammatical error!_92 "
#define     EMSG_GRAMMAR_93                 "Grammatical error!_93 "
#define     EMSG_GRAMMAR_94                 "Grammatical error!_94 "
#define     EMSG_GRAMMAR_95                 "Grammatical error!_95 "
#define     EMSG_GRAMMAR_96                 "Grammatical error!_96 "
#define     EMSG_GRAMMAR_97                 "Grammatical error!_97 "
#define     EMSG_GRAMMAR_98                 "Grammatical error!_98 "
#define     EMSG_GRAMMAR_99                 "Grammatical error!_99 "
#define     EMSG_GRAMMAR_100                "Grammatical error!_100"
#define     EMSG_GRAMMAR_101                "Grammatical error!_101"
#define     EMSG_GRAMMAR_102                "Grammatical error!_102"
#define     EMSG_GRAMMAR_103                "Grammatical error!_103"
#define     EMSG_GRAMMAR_104                "Grammatical error!_104"
#define     EMSG_GRAMMAR_105                "Grammatical error!_105"
#define     EMSG_GRAMMAR_106                "Grammatical error!_106"
#define     EMSG_GRAMMAR_107                "Grammatical error!_107"
#define     EMSG_GRAMMAR_108                "Grammatical error!_108"
#define     EMSG_GRAMMAR_109                "Grammatical error!_109"
#define     EMSG_GRAMMAR_110                "Grammatical error!_110"
#define     EMSG_GRAMMAR_111                "Grammatical error!_111"
#define     EMSG_GRAMMAR_112                "Grammatical error!_112"
#define     EMSG_GRAMMAR_113                "Grammatical error!_113"
#define     EMSG_GRAMMAR_114                "Grammatical error!_114"
#define     EMSG_GRAMMAR_115                "Grammatical error!_115"
#define     EMSG_GRAMMAR_116                "Grammatical error!_116"
#define     EMSG_GRAMMAR_117                "Grammatical error!_117"
#define     EMSG_GRAMMAR_118                "Grammatical error!_118"
#define     EMSG_GRAMMAR_119                "Grammatical error!_119"
#define     EMSG_GRAMMAR_120                "Grammatical error!_120"
#define     EMSG_GRAMMAR_121                "Grammatical error!_121"
#define     EMSG_GRAMMAR_122                "Grammatical error!_122"
#define     EMSG_GRAMMAR_123                "Grammatical error!_123"
#define     EMSG_GRAMMAR_124                "Grammatical error!_124"
#define     EMSG_GRAMMAR_125                "Grammatical error!_125"
#define     EMSG_GRAMMAR_126                "Grammatical error!_126"
#define     EMSG_GRAMMAR_127                "Grammatical error!_127"
#define     EMSG_GRAMMAR_128                "Grammatical error!_128"
#define     EMSG_GRAMMAR_129                "Grammatical error!_129"
#define     EMSG_GRAMMAR_130                "Grammatical error!_130"
#define            	 EMSG_GRAMMAR_0131	  "Grammatical error!(0131)"
#define            	 EMSG_GRAMMAR_0132	  "Grammatical error!(0132)"
#define            	 EMSG_GRAMMAR_0133	  "Grammatical error!(0133)"
#define            	 EMSG_GRAMMAR_0134	  "Grammatical error!(0134)"
#define            	 EMSG_GRAMMAR_0135	  "Grammatical error!(0135)"
#define            	 EMSG_GRAMMAR_0136	  "Grammatical error!(0136)"
#define            	 EMSG_GRAMMAR_0137	  "Grammatical error!(0137)"
#define            	 EMSG_GRAMMAR_0138	  "Grammatical error!(0138)"
#define            	 EMSG_GRAMMAR_0139	  "Grammatical error!(0139)"
#define            	 EMSG_GRAMMAR_0140	  "Grammatical error!(0140)"
#define            	 EMSG_GRAMMAR_0141	  "Grammatical error!(0141)"
#define            	 EMSG_GRAMMAR_0142	  "Grammatical error!(0142)"
#define            	 EMSG_GRAMMAR_0143	  "Grammatical error!(0143)"
#define            	 EMSG_GRAMMAR_0144	  "Grammatical error!(0144)"
#define            	 EMSG_GRAMMAR_0145	  "Grammatical error!(0145)"
#define            	 EMSG_GRAMMAR_0146	  "Grammatical error!(0146)"
#define            	 EMSG_GRAMMAR_0147	  "Grammatical error!(0147)"
#define            	 EMSG_GRAMMAR_0148	  "Grammatical error!(0148)"
#define            	 EMSG_GRAMMAR_0149	  "Grammatical error!(0149)"
#define            	 EMSG_GRAMMAR_0150	  "Grammatical error!(0150)"

#define     EMSG_GRAMMAR                		"Grammatical error!"

#define     USERERR_RUN                 ( USERERR_BASE +   30000 )
#define     EMSG_RUN                    "Running error!"
#endif
#define     SYSINFO_BASE                                 2000000
#define     USERINFO_BASE                                1000000

#endif

