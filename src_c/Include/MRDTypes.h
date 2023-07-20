#ifndef   _INC_MRDTYPES
#define   _INC_MRDTYPES

#include "MItem.h"



#define  LSTR_VALUE_SIZE     300
#define  MRS_DEFAULT_DATE    "0001-01-01"

#define  DATA_TYPE_ERROR      0
#define  DATA_TYPE_DOUBLE     1
#define  DATA_TYPE_STRING     2
#define  DATA_TYPE_LONG       3
#define  DATA_TYPE_LSTRING    4
#define  DATA_TYPE_BOOL       5
#define  DATA_TYPE_VOID       6
//#define  DATA_TYPE_DATE       7

//  TableRule Input Type
#define  TR_IN_TP_CONST       0
#define  TR_IN_TP_RANGE       1
#define  TR_IN_TP_OPTR        2
#define  TR_IN_TP_COND        3
#define  TR_IN_TP_RONLY       4

//  TableRule Compare Type
#define  TR_COL_TP_RULE       1
#define  TR_COL_TP_EXTV       2
#define  TR_COL_TP_INTV       3
#define  TR_COL_TP_GBLV       4

//  FlowRule Node Type
#define  FR_NODE_TP_START     0
#define  FR_NODE_TP_END       1
#define  FR_NODE_TP_IF        2
#define  FR_NODE_TP_EXEC      3
#define  FR_NODE_TP_RET       4
#define  FR_NODE_TP_CASE      5
#define  FR_NODE_TP_LOOP      6

//  QueryRule Type
#define  QR_TP_SELECT         0
#define  QR_TP_CUD            1
#define  QR_TP_STMT           2

#endif    /*  _INC_MRDTYPES   */

