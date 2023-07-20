#ifndef	__INC_BRAGTDEF
#define	__INC_BRAGTDEF

	#define	MAX_BIND_VARS		200 /*2014.10.09 100->200(2020.06.10)*/    
	#define	MAX_SEL_VARS		200 /*200(2020.06.10)*/
	#define	MAX_VNAME_LEN		200
	#define	MAX_INAME_LEN		100
	//확인사항 보상금..지급심사.. 
	#define	MAX_FETCH_ROWS		100


typedef struct {
	void	*ctx;
	void    *stmt; // added for MYSQL 2019.06.13
	int		free;
	time_t	stTime;
	int		ruleId;

	int		m_bConnected;
	int		m_fetch_rows;
	int		m_fetch_total;
	int		m_idx;
	int		conIdx;
//	void		*sqlca;
	void		*connection;
	void		*bind_dp;
	void		*select_dp;
	
} DB_AGENT;
 
#endif
