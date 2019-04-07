/*
  Quick hack to wrap FTD2xx lib for linux
  such that it may be called from the Wine Windows Emulator
  
  revised:  brent@mbari.org  -- 9/29/14
  tackleSIGSEGV11 steini 6.4.19
*/

#include <stdlib.h>
#include <unistd.h>

typedef const char * LPCTSTR; //quick fix for 1.4.8

#include "xftd2xx.h"
#include "wine/debug.h"
WINE_DEFAULT_DEBUG_CHANNEL(ftd2xx);

//#define Trace(...) //TRACE(__VA_ARGS__)

//a list of FTD product ids to access (in hexidecimal)
#define FTDprodVar "FTDID"


BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{
TRACE("reason=%d\n", fdwReason);
  switch(fdwReason) {
    case DLL_PROCESS_ATTACH: {
      char *cursor = getenv(FTDprodVar);
      if (cursor) {
        char *end;
        unsigned vendor = 0x403;
        unsigned long product = strtoul(cursor, &end, 16);
        if (product && end && *end) {
          unsigned long prod2 = strtoul(end+1, &end, 16);
          if (prod2) {
            vendor = product;
            product = prod2;
          }
        }
        if (FT_SetVIDPID(vendor, product))
          return FALSE;
        TRACE("Seeking vendor:product 0x%04x:0x%04x\n", (uint)vendor, (uint)product);
      }
    }
  }
  return TRUE;
}



FT_STATUS WINAPI FT_CreateDeviceInfoList(LPDWORD lpdwNumDevs)
{
TRACE("U\n");
	return xFT_CreateDeviceInfoList(lpdwNumDevs);
}


FT_STATUS WINAPI FT_GetDeviceInfoList(FT_DEVICE_LIST_INFO_NODE *pDest,
                                   LPDWORD lpdwNumDevs)
{
TRACE("U\n");
	return xFT_GetDeviceInfoList(pDest, lpdwNumDevs);
}


FT_STATUS WINAPI FT_GetDeviceInfoDetail (DWORD dwIndex, LPDWORD lpdwFlags,
                                    LPDWORD lpdwType,
                                    LPDWORD lpdwID, LPDWORD lpdwLocId,
                                    PCHAR pcSerialNumber, PCHAR pcDescription,
                                    FT_HANDLE *ftHandle)
{
TRACE("\n");
	return xFT_GetDeviceInfoDetail(dwIndex, lpdwFlags,
                                       lpdwType, lpdwID, lpdwLocId,
                                       pcSerialNumber, pcDescription,
                                       ftHandle);
}


FT_STATUS WINAPI FT_ListDevices(PVOID pArg1, PVOID pArg2, DWORD flags)
{
TRACE("pArg1=%p, pArg2=%p, flags=0x%x\n", pArg1, pArg2, flags);
  FT_STATUS result = xFT_ListDevices( pArg1,  pArg2,  flags);
TRACE("returning %d, nDevs=%d\n", result, *(DWORD *)pArg1);
  return result;
}


FT_STATUS WINAPI FT_Open(int deviceNumber, FT_HANDLE* pHandle)
{
TRACE("\n");
  return xFT_Open(deviceNumber, pHandle);
}


FT_STATUS WINAPI FT_OpenEx(PVOID pvArg1, DWORD dwFlags, FT_HANDLE* pHandle)
{
TRACE("\n");
  return xFT_OpenEx(pvArg1, dwFlags, pHandle);
}


FT_STATUS WINAPI FT_SetBaudRate(FT_HANDLE ftHandle, DWORD dwBaudRate)
{
TRACE("\n");
  return xFT_SetBaudRate(ftHandle, dwBaudRate);
}	


FT_STATUS WINAPI FT_SetDivisor(FT_HANDLE ftHandle, USHORT usDivisor)
{
TRACE("\n");
  return xFT_SetDivisor(ftHandle, usDivisor);
}	


FT_STATUS WINAPI FT_SetDataCharacteristics(FT_HANDLE ftHandle,
                          UCHAR uWordLength, UCHAR uStopBits, UCHAR uParity)
{
TRACE("\n");
  return xFT_SetDataCharacteristics(ftHandle, uWordLength, uStopBits, uParity);
}	


FT_STATUS WINAPI FT_SetFlowControl(FT_HANDLE ftHandle, USHORT usFlowControl,
                               UCHAR uXon, UCHAR uXoff)
{
TRACE("\n");
  return xFT_SetFlowControl(ftHandle, usFlowControl, uXon, uXoff);
}	


FT_STATUS WINAPI FT_SetDtr(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_SetDtr(ftHandle);
}


FT_STATUS WINAPI FT_ClrDtr(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_ClrDtr(ftHandle);
}


FT_STATUS WINAPI FT_SetRts(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_SetRts(ftHandle);
}


FT_STATUS WINAPI FT_ClrRts(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_ClrRts(ftHandle);
}


FT_STATUS WINAPI FT_SetBreakOn(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_SetBreakOn(ftHandle);
}


FT_STATUS WINAPI FT_SetBreakOff(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_SetBreakOff(ftHandle);
}


FT_STATUS WINAPI FT_GetModemStatus(FT_HANDLE ftHandle, LPDWORD lpdwModemStatus)
{
//TRACE("\n");
 FT_STATUS tmp = xFT_GetModemStatus(ftHandle, lpdwModemStatus);
 TRACE("ftHandle=%p, lpdwModemStatus=%x, FT_STATUS=0x%x\n", ftHandle, *lpdwModemStatus, tmp);
 return tmp;
}


BOOL WINAPI FT_W32_GetCommModemStatus(FT_HANDLE ftHandle, LPDWORD lpdwStat)
{
TRACE("\n");
  return xFT_W32_GetCommModemStatus(ftHandle, lpdwStat);
}


FT_STATUS WINAPI FT_Close(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_Close(ftHandle);
}


FT_STATUS WINAPI FT_StopInTask(FT_HANDLE ftHandle)
{
TRACE("FT_StopInTask\n");
  return xFT_StopInTask(ftHandle);
}


FT_STATUS WINAPI FT_RestartInTask(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_RestartInTask(ftHandle);
}


DWORD WINAPI FT_W32_GetLastError(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_W32_GetLastError(ftHandle);
}


BOOL WINAPI FT_W32_ClearCommError(FT_HANDLE ftHandle,
                            LPDWORD lpdwErrors, LPFTCOMSTAT lpftComstat)
{
TRACE("\n");
  return xFT_W32_ClearCommError(ftHandle, lpdwErrors, lpftComstat);
}


BOOL WINAPI FT_W32_PurgeComm(FT_HANDLE ftHandle, DWORD dwFlags)
{
TRACE("\n");
  return xFT_W32_PurgeComm(ftHandle, dwFlags);
}


BOOL WINAPI FT_W32_EscapeCommFunction(FT_HANDLE ftHandle, DWORD dwFunc)
{
TRACE("\n");
  return xFT_W32_EscapeCommFunction(ftHandle, dwFunc);
}


BOOL WINAPI FT_W32_SetupComm(FT_HANDLE ftHandle,
                              DWORD dwReadBufferSize, DWORD dwWriteBufferSize)
{
TRACE("\n");
  return xFT_W32_SetupComm(ftHandle, dwReadBufferSize, dwWriteBufferSize);
}

 
BOOL WINAPI FT_W32_WaitCommEvent(FT_HANDLE ftHandle, LPDWORD lpdwEvent,
                                  LPOVERLAPPED lpOverlapped)
{
TRACE("\n");
  return xFT_W32_WaitCommEvent(ftHandle, lpdwEvent, lpOverlapped);
}

 
BOOL WINAPI FT_W32_SetCommMask(FT_HANDLE ftHandle, DWORD dwMask)
{
TRACE("\n");
  return xFT_W32_SetCommMask(ftHandle, dwMask);
}

 
BOOL WINAPI FT_W32_SetCommBreak(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_W32_SetCommBreak(ftHandle);
}


BOOL WINAPI FT_W32_ClearCommBreak(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_W32_ClearCommBreak(ftHandle);
}


FT_HANDLE FT_W32_CreateFile(PVOID pvArg1, DWORD dwAccess, DWORD dwShareMode,
                                LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                DWORD dwCreate, DWORD dwAttrsAndFlags,
                                HANDLE hTemplate)
{
TRACE("\n");
  return xFT_W32_CreateFile(pvArg1, dwAccess, dwShareMode,
                                lpSecurityAttributes,
                                dwCreate, dwAttrsAndFlags,
                                hTemplate);
}


BOOL WINAPI FT_W32_CloseHandle(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_W32_CloseHandle(ftHandle);
}


BOOL WINAPI FT_W32_SetCommState(FT_HANDLE ftHandle, LPFTDCB lpftDcb)
{
TRACE("\n");
  return xFT_W32_SetCommState(ftHandle, lpftDcb);
}


BOOL WINAPI FT_W32_GetCommState(FT_HANDLE ftHandle, LPFTDCB lpftDcb)
{
TRACE("\n");
  return xFT_W32_GetCommState(ftHandle, lpftDcb);
}

 
BOOL WINAPI FT_W32_SetCommTimeouts(FT_HANDLE ftHandle, LPFTTIMEOUTS lpftTimeouts)
{
TRACE("\n");
  return xFT_W32_SetCommTimeouts(ftHandle, lpftTimeouts);
}

 
BOOL WINAPI FT_W32_GetCommTimeouts(FT_HANDLE ftHandle, LPFTTIMEOUTS lpftTimeouts)
{
TRACE("\n");
  return xFT_W32_GetCommTimeouts(ftHandle, lpftTimeouts);
}

 
BOOL WINAPI FT_W32_GetOverlappedResult(FT_HANDLE ftHandle,
  LPOVERLAPPED lpOverlapped, LPDWORD lpdwBytesTransferred, BOOL bWait)
{
TRACE("\n");
  return xFT_W32_GetOverlappedResult(ftHandle, 
              lpOverlapped, lpdwBytesTransferred, bWait);
}

 
BOOL WINAPI FT_W32_WriteFile(FT_HANDLE ftHandle, LPVOID lpBuffer,
   DWORD dwBytesToWrite, LPDWORD lpdwBytesWritten, LPOVERLAPPED lpOverlapped)
{
TRACE("\n");
  return xFT_W32_WriteFile(ftHandle, 
                  lpBuffer, dwBytesToWrite, lpdwBytesWritten, lpOverlapped);
}


BOOL WINAPI FT_W32_ReadFile(FT_HANDLE ftHandle, LPVOID lpBuffer,
    DWORD dwBytesToRead, LPDWORD lpdwBytesReturned, LPOVERLAPPED lpOverlapped)
{
TRACE("\n");
  return xFT_W32_ReadFile(ftHandle, 
                  lpBuffer, dwBytesToRead, lpdwBytesReturned, lpOverlapped);
}


FT_STATUS WINAPI FT_EE_UASize(FT_HANDLE ftHandle, DWORD *lpdwSize)
{
TRACE("\n");
  return xFT_EE_UASize(ftHandle, lpdwSize);
}


FT_STATUS WINAPI FT_EE_UAWrite(FT_HANDLE ftHandle,
                                PUCHAR pucData, DWORD dwDataLen)
{
TRACE("\n");
  return xFT_EE_UAWrite(ftHandle, pucData, dwDataLen);
}


FT_STATUS WINAPI FT_EE_UARead(FT_HANDLE ftHandle, unsigned char *pucData,
                              DWORD dwDataLen, DWORD *lpdwBytesRead)
{
TRACE("\n");
  return xFT_EE_UARead(ftHandle, pucData, dwDataLen, lpdwBytesRead);
}	


FT_STATUS WINAPI FT_EE_Program(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData)
{
TRACE("\n");
  return xFT_EE_Program(ftHandle, pData);
}	


FT_STATUS WINAPI FT_EE_Read(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData)
{
TRACE("\n");
  return xFT_EE_Read(ftHandle, pData);
}	


FT_STATUS WINAPI FT_EraseEE(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_EraseEE(ftHandle);
}	


FT_STATUS WINAPI FT_WriteEE(FT_HANDLE ftHandle, DWORD dwWordOffset, WORD wValue)
{
TRACE("\n");
  return xFT_WriteEE(ftHandle, dwWordOffset, wValue);
}	


FT_STATUS WINAPI FT_ReadEE(FT_HANDLE ftHandle, DWORD dwWordOffset, LPWORD lpwValue)
{
TRACE("\n");
  return xFT_ReadEE(ftHandle, dwWordOffset, lpwValue);
}	


FT_STATUS WINAPI FT_EE_ProgramEx(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData,
                    char *Manufacturer, char *ManufacturerId,
                    char *Description, char *SerialNumber)
{
TRACE("\n");
  return xFT_EE_ProgramEx(ftHandle, pData,
                    Manufacturer, ManufacturerId,
                    Description, SerialNumber);
}	


FT_STATUS WINAPI FT_EE_ReadEx(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData,
                    char *Manufacturer, char *ManufacturerId,
                    char *Description, char *SerialNumber)
{
TRACE("\n");
  return xFT_EE_ReadEx(ftHandle, pData,
                    Manufacturer, ManufacturerId,
                    Description, SerialNumber);
}	


FT_STATUS WINAPI FT_GetDeviceInfo(FT_HANDLE ftHandle, FT_DEVICE *lpftDevice, 
      LPDWORD lpdwID, PCHAR SerialNumber, PCHAR Description,	LPVOID Dummy)
{
TRACE("\n");
  return xFT_GetDeviceInfo(ftHandle, lpftDevice, 
                            lpdwID, SerialNumber, Description, Dummy);
}

FT_STATUS WINAPI FT_Read(FT_HANDLE ftHandle, LPVOID lpBuffer,
                          DWORD nBufferSize, LPDWORD lpBytesReturned)
{
//TRACE("\n");
  FT_STATUS tmp = xFT_Read(ftHandle, lpBuffer, nBufferSize, lpBytesReturned);
  TRACE("ftHandle=%p, lpBuffer=%s, nBufferSize=0x%x, lpBytesReturned=0x%x, FT_STATUS=0x%x\n", ftHandle, debugstr_a(lpBuffer), nBufferSize, *lpBytesReturned, tmp);
  return tmp;
}


FT_STATUS WINAPI FT_Write(FT_HANDLE ftHandle, LPVOID lpBuffer,
                          DWORD nBufferSize, LPDWORD lpBytesWritten)
{
//TRACE("\n");
  FT_STATUS tmp = xFT_Write(ftHandle, lpBuffer, nBufferSize, lpBytesWritten);
  TRACE("ftHandle=%p, lpBuffer=%s, nBufferSize=0x%x, lpBytesWritten=0x%x, FT_STATUS=0x%x\n", ftHandle, debugstr_a(lpBuffer), nBufferSize, *lpBytesWritten, tmp);
  return tmp;
}


FT_STATUS WINAPI FT_ResetDevice(FT_HANDLE ftHandle)
{
TRACE("\n");
  return xFT_ResetDevice(ftHandle);
}


FT_STATUS WINAPI FT_SetTimeouts(FT_HANDLE ftHandle,
                                DWORD dwReadTimeout, DWORD dwWriteTimeout)
{
TRACE("\n");
  return xFT_SetTimeouts(ftHandle, dwReadTimeout, dwWriteTimeout);
}


BOOL WINAPI FT_Purge(FT_HANDLE ftHandle, ULONG Mask)
{
TRACE("\n");
  return xFT_Purge(ftHandle, Mask);
}


FT_STATUS WINAPI FT_GetQueueStatus(FT_HANDLE ftHandle, DWORD *dwRxBytes)
{
//TRACE("\n"); to noisi
  FT_STATUS tmp = xFT_GetQueueStatus(ftHandle, dwRxBytes);
///  TRACE("dwRxBytes=0x%x\n",*dwRxBytes);
  return tmp;
}


FT_STATUS WINAPI FT_GetStatus(FT_HANDLE ftHandle,
                      DWORD *dwRxBytes, DWORD *dwTxBytes, DWORD *dwEventDWord)
{
TRACE("\n");
  return xFT_GetStatus(ftHandle, dwRxBytes, dwTxBytes, dwEventDWord);
}


FT_STATUS WINAPI FT_SetLatencyTimer(FT_HANDLE ftHandle, UCHAR ucLatency)
{
TRACE("\n");
  return xFT_SetLatencyTimer(ftHandle, ucLatency);
}


FT_STATUS WINAPI FT_GetLatencyTimer(FT_HANDLE ftHandle, PUCHAR pucLatency)
{
TRACE("\n");
  return xFT_GetLatencyTimer(ftHandle, pucLatency);
}


FT_STATUS WINAPI FT_SetBitMode(FT_HANDLE ftHandle, UCHAR ucMask, UCHAR ucEnable)
{
TRACE("\n");
  return xFT_SetBitMode(ftHandle, ucMask, ucEnable);
}


FT_STATUS WINAPI FT_GetBitMode(FT_HANDLE ftHandle, PUCHAR pucMode)
{
TRACE("\n");
  return xFT_GetBitMode(ftHandle, pucMode);
}


FT_STATUS WINAPI FT_SetUSBParameters(FT_HANDLE ftHandle,
                          ULONG ulInTransferSize, ULONG ulOutTransferSize)
{
TRACE("\n");
  return xFT_SetUSBParameters(ftHandle, ulInTransferSize, ulOutTransferSize);
}


FT_STATUS WINAPI FT_SetEventNotification(FT_HANDLE ftHandle,
                                         DWORD dwEventMask, PVOID pvArg)
{
TRACE("BUG2SIGSEGV11\n");
  TRACE("ftHandle=%p, dwEventMask=0x%x, pvArg=%p \n", ftHandle, dwEventMask, &pvArg);
  return 0x0;//QuickFIX but i have not a single clue why this helps
  //dwEventMask = FT_EVENT_MODEM_STATUS; //test only this 
  //FT_EVENT_RXCHAR 1 FT_EVENT_MODEM_STATUS 2 FT_EVENT_LINE_STATUS 4
  FT_STATUS tmp = xFT_SetEventNotification(ftHandle, dwEventMask, pvArg);
  TRACE("ftHandle=%p, dwEventMask=0x%x, pvArg=%p FT_STATUS=0x%x\n", ftHandle, dwEventMask, &pvArg, tmp);
  return tmp;
}


FT_STATUS WINAPI FT_SetChars (FT_HANDLE ftHandle,
   UCHAR uEventCh, UCHAR uEventChEn, UCHAR uErrorCh, UCHAR uErrorChEn)
{
TRACE("\n");
  return xFT_SetChars(ftHandle, uEventCh, uEventChEn, uErrorCh, uErrorChEn);
}

