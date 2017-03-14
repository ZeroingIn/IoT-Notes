/**************************************************************************************************
  Filename:       
  Revised:        $Date: 2008-03-14 11:20:21 -0700 (Fri, 14 Mar 2008) $
  Revision:       $Revision: 16590 $

  Description:    Z-Stack Simple Application Interface.

*/
#ifndef FLASHAPI_H
#define FLASHAPI_H

/******************************************************************************
 * INCLUDES
 */
#include "ZComDef.h"
#include "af.h"

/******************************************************************************/
 // CONSTANTS
extern  void test_read_flash (void);
extern  void    read_appconfig(void * buf);
extern  void    write_appconfig(void * buf);
extern  void    configset(uint8 * pBuf,uint8 len,uint8 logtype);
extern  void    configread(uint8 * pBuf);
extern uint8 calcXOR(uint8 *pBuf, uint8 len);
extern void zb_Readpandid(void * buf);
extern void zb_Writepandid(void * buf);
extern uint8 zb_Readchannel(void);
extern void zb_Writechannel(uint8 ch);
#endif  
