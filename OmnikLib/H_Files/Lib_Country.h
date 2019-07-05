/**********************************************************************
 * 版权:苏州欧姆尼克新能源科技有限公司软件组
 * 文件名称：Lib_Country.h
 * 修改人：沈仕强
 * 修改日期：2014.11.15
 * 描述：各个国家的并网法规
 * 修改内容：
 * 1.1版--添加国家代号
 * 1.2版--增加Ergon、Energex和SriLanka
 * 1.3版--增加波兰和智利法规
***********************************************************************/
#ifndef _LIB_COUNTRY_H_
#define _LIB_COUNTRY_H_

#define cUK          0
#define cGermany     1
#define cFrance      2
#define cItaly       3
#define cSpain       4
#define cAustria     5
#define cAustralia   6
#define cIndia       7
#define cBelgium     8
#define cDanish      9
#define cGreece_land 10
#define cGreece_Isl  11
#define cHolland     12
#define cPortugal    13
#define cChina       14
#define cSweden      15
#define cOther_50Hz  16
#define cRomania     17
#define cNew_Zealand 18
#define cBrazil      19
#define cWestAustril 20
#define cVrabelg     21
#define cCanada      22
#define cUSA_Mexico  23
#define cOther_60Hz  24
#define cSouth_Korea 25
#define cLuxembourg  26
#define cMaxSaftyCode	30


extern const INT16U mSafetyLawDefaultData[54][30];

#endif
