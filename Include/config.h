#ifndef CONFIG_H
#define CONFIG_H
#include "TypeDefine.h"
#ifdef __cplusplus
#if __cplusplus
	extern "C"{
#endif
#endif

#define STORE_TYPE_U32      0x0000
#define STORE_TYPE_I32      0x0010
#define SEND_TYPE_16BIT     0x0000
#define SEND_TYPE_32BIT     0x0100
#define CFG_TYPE_U16        0x0000
#define CFG_TYPE_S16        0x0001
#define CFG_TYPE_FLO        0x0002
#define CFG_TYPE_U32        0x0003
#define CFG_TYPE_S32        0x0004
#define CFG_TYPE_FUN        0x0005

#define RESET_TYPE          0x0000
#define NO_RESET_TYPE       0x1000

#define I32_STORE_OFFSET    0x00010000

#define CFG_RESET_MASK      0xF000
#define CFG_SEND_MASK       0x0F00
#define STORE_TYPE_MASK     0x00F0
#define CFG_TYPE_MASK       0x000F

#define	PARAM_NUM            320
#define INV_TYPE_PARAM_CFG_NUM 64

#define PARAM_SET_SUCCESS    0
#define PARAM_SET_FAILED     1

#define CFG_SUCCESS	0
#define CFG_FAILED  1

typedef void (* CONFIG_CALL_FUNC)(INT16U);

typedef union
{
	void              *pvData;
	INT16U            *pusData;
	INT16S            *psData;
	FP32              *pfData;
	INT32U 			  *pulData;
	INT32S 			  *plData;
	CONFIG_CALL_FUNC  pFunCall;
}CONFIG_DEST_UN;

typedef struct
{
	INT16U    usRdWrtAddr;
	INT16U    usRdAddr;
	INT32S    lMin;
	INT32S    lMax;
	INT16U    usGain;
	INT16U    usType;
	CONFIG_DEST_UN    unCfgDest;
}PARAM_CFG_TBL_STR;

extern INT16U CFG_ParamCfg(const PARAM_CFG_TBL_STR *pusDataTarget, INT16U *pusDataSource, INT16U usStartAddr, INT16U usLen);
extern INT16U CFG_LoadParam(INT16U *pusDataTarget, const PARAM_CFG_TBL_STR *pusDataSource, INT16U usStartAddr, INT16U usLen, INT16U usCmdType);

#ifdef __cplusplus
#if __cplusplus
	}
#endif
#endif

#endif
