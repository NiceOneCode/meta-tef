/*
 *
 * Copyright (C) 2012 by Cambridge Silicon Radio Ltd.
 *
 * Refer to LICENSE.txt included with this source code for details on
 * the license terms.
 *
 */


#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_msgconv.h"
#include "csr_wifi_msgconv.h"
#include "csr_wifi_lib.h"

void CsrUint24Des(CsrUint32 *v, CsrUint8 *buffer, CsrSize *offset)
{
    CsrUint32 val;

    val = ((buffer[(*offset) + 2] << 16) |
           (buffer[(*offset) + 1] << 8) |
           (buffer[(*offset)]));

    *offset += 3;
    *v = val;
}


/* Big endian :e.g WSC, TCLAS */
void CsrUint16DesBigEndian(CsrUint16 *v, CsrUint8 *buffer, CsrSize *offset)
{
    CsrUint16 val;

    val = (buffer[(*offset)] << 8) | (buffer[(*offset) + 1]);
    *offset += 2;

    *v = val;
}


void CsrUint24DesBigEndian(CsrUint32 *v, CsrUint8 *buffer, CsrSize *offset)
{
    CsrUint32 val;

    val = ((buffer[(*offset)] << 16) |
           (buffer[(*offset) + 1] << 8) |
           (buffer[(*offset) + 2]));

    *offset += 3;
    *v = val;
}


void CsrUint32DesBigEndian(CsrUint32 *v, CsrUint8 *buffer, CsrSize *offset)
{
    CsrUint32 val;

    val = ((buffer[(*offset)] << 24) |
           (buffer[(*offset) + 1] << 16) |
           (buffer[(*offset) + 2] << 8) |
           (buffer[(*offset) + 3]));

    *offset += 4;
    *v = val;
}


void CsrUint24Ser(CsrUint8 *ptr, CsrSize *len, CsrUint32 v)
{
    ptr[(*len) + 2] = (CsrUint8)((v & 0x00ff0000) >> 16);
    ptr[(*len) + 1] = (CsrUint8)((v & 0x0000ff00) >> 8);
    ptr[(*len)]     = (CsrUint8)((v & 0x000000ff));

    *len += 3;
}


/* Big endian :e.g WSC, TCLAS */
void CsrUint16SerBigEndian(CsrUint8 *ptr, CsrSize *len, CsrUint16 v)
{
    ptr[(*len)] = (CsrUint8)((v & 0xff00) >> 8);
    ptr[(*len) + 1] = (CsrUint8)((v & 0x00ff));

    *len += 2;
}


void CsrUint32SerBigEndian(CsrUint8 *ptr, CsrSize *len, CsrUint32 v)
{
    ptr[(*len)] = (CsrUint8)((v & 0xff000000) >> 24);
    ptr[(*len) + 1] = (CsrUint8)((v & 0x00ff0000) >> 16);
    ptr[(*len) + 2] = (CsrUint8)((v & 0x0000ff00) >> 8);
    ptr[(*len) + 3] = (CsrUint8)((v & 0x000000ff));

    *len += 4;
}


void CsrUint24SerBigEndian(CsrUint8 *ptr, CsrSize *len, CsrUint32 v)
{
    ptr[(*len)] = (CsrUint8)((v & 0x00ff0000) >> 16);
    ptr[(*len) + 1] = (CsrUint8)((v & 0x0000ff00) >> 8);
    ptr[(*len) + 2] = (CsrUint8)((v & 0x000000ff));

    *len += 3;
}


CsrSize CsrWifiEventSizeof(void *msg)
{
    return 2;
}


CsrUint8* CsrWifiEventSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrWifiFsmEvent *primitive = (CsrWifiFsmEvent *)msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    return(ptr);
}


void* CsrWifiEventDes(CsrUint8 *buffer, CsrSize length)
{
    CsrWifiFsmEvent *primitive = (CsrWifiFsmEvent *) CsrPmemAlloc(sizeof(CsrWifiFsmEvent));
    CsrSize offset = 0;
    CsrUint16Des(&primitive->type, buffer, &offset);

    return primitive;
}


CsrSize CsrWifiEventCsrUint8Sizeof(void *msg)
{
    return 3;
}


CsrUint8* CsrWifiEventCsrUint8Ser(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrWifiEventCsrUint8 *primitive = (CsrWifiEventCsrUint8 *)msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->common.type);
    CsrUint8Ser(ptr, len, primitive->value);
    return(ptr);
}


void* CsrWifiEventCsrUint8Des(CsrUint8 *buffer, CsrSize length)
{
    CsrWifiEventCsrUint8 *primitive = (CsrWifiEventCsrUint8 *) CsrPmemAlloc(sizeof(CsrWifiEventCsrUint8));

    CsrSize offset = 0;
    CsrUint16Des(&primitive->common.type, buffer, &offset);
    CsrUint8Des(&primitive->value, buffer, &offset);

    return primitive;
}


CsrSize CsrWifiEventCsrUint16Sizeof(void *msg)
{
    return 4;
}


CsrUint8* CsrWifiEventCsrUint16Ser(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrWifiEventCsrUint16 *primitive = (CsrWifiEventCsrUint16 *)msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->common.type);
    CsrUint16Ser(ptr, len, primitive->value);
    return(ptr);
}


void* CsrWifiEventCsrUint16Des(CsrUint8 *buffer, CsrSize length)
{
    CsrWifiEventCsrUint16 *primitive = (CsrWifiEventCsrUint16 *) CsrPmemAlloc(sizeof(CsrWifiEventCsrUint16));

    CsrSize offset = 0;
    CsrUint16Des(&primitive->common.type, buffer, &offset);
    CsrUint16Des(&primitive->value, buffer, &offset);

    return primitive;
}


CsrSize CsrWifiEventCsrUint32Sizeof(void *msg)
{
    return 6;
}


CsrUint8* CsrWifiEventCsrUint32Ser(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrWifiEventCsrUint32 *primitive = (CsrWifiEventCsrUint32 *)msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->common.type);
    CsrUint32Ser(ptr, len, primitive->value);
    return(ptr);
}


void* CsrWifiEventCsrUint32Des(CsrUint8 *buffer, CsrSize length)
{
    CsrWifiEventCsrUint32 *primitive = (CsrWifiEventCsrUint32 *) CsrPmemAlloc(sizeof(CsrWifiEventCsrUint32));

    CsrSize offset = 0;
    CsrUint16Des(&primitive->common.type, buffer, &offset);
    CsrUint32Des(&primitive->value, buffer, &offset);

    return primitive;
}


CsrSize CsrWifiEventCsrUint16CsrUint8Sizeof(void *msg)
{
    return 5;
}


CsrUint8* CsrWifiEventCsrUint16CsrUint8Ser(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrWifiEventCsrUint16CsrUint8 *primitive = (CsrWifiEventCsrUint16CsrUint8 *)msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->common.type);
    CsrUint16Ser(ptr, len, primitive->value16);
    CsrUint8Ser(ptr, len, primitive->value8);
    return(ptr);
}


void* CsrWifiEventCsrUint16CsrUint8Des(CsrUint8 *buffer, CsrSize length)
{
    CsrWifiEventCsrUint16CsrUint8 *primitive = (CsrWifiEventCsrUint16CsrUint8 *) CsrPmemAlloc(sizeof(CsrWifiEventCsrUint16CsrUint8));

    CsrSize offset = 0;
    CsrUint16Des(&primitive->common.type, buffer, &offset);
    CsrUint16Des(&primitive->value16, buffer, &offset);
    CsrUint8Des(&primitive->value8, buffer, &offset);

    return primitive;
}


