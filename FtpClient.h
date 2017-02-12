/////////////////////////////////////////////////////////////////////////////
// FtpClient.h: interface for the CFtpClient class.

#ifndef __FTPCLIENT_H__
#define __FTPCLIENT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CFtpClient
{
public:
  CFtpClient();
  ~CFtpClient();

  BOOL LogOnToServer(CString strHost, CString strUser, CString strPasswd, int iPort = 21, CString strAcct = "", CString strFirewallHost = "", CString strFirewallUser = "", CString strFirewallPasswd = "", int iFirewallPort = 0, int iLogonType = 0);
  void LogOffServer();

  BOOL TransferFile(CString strRemoteFile, CString strLocalFile, BOOL bGet, BOOL bPasv = FALSE);
  BOOL RenameRemoteFile(CString strFrom, CString strTo);
  BOOL DeleteRemoteFile(CString strFile);

  BOOL FtpCommand(CString strCmd, int iExpectedResult = 2);

  CString m_strRetMsg;

private:
  BOOL WriteStr(CString str);
  BOOL ReadStr();
  BOOL ReadStrWorker();
  BOOL OpenControlChannel(CString strHost, int iPort);
  void CloseControlChannel();

  CArchive*     m_pCtrlRxArch;
  CArchive*     m_pCtrlTxArch;
  CSocketFile*  m_pCtrlSockFile;
  CSocket*      m_pCtrlSock;
  int           m_iCommandResult;

};

#endif // __FTPCLIENT_H__
