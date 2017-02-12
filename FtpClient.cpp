/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2004 Frank Fesevur                                       //
//                                                                         //
//  This program is free software; you can redistribute it and/or modify   //
//  it under the terms of the GNU General Public License as published by   //
//  the Free Software Foundation; either version 2 of the License, or      //
//  (at your option) any later version.                                    //
//                                                                         //
//  This program is distributed in the hope that it will be useful,        //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           //
//  GNU General Public License for more details.                           //
//                                                                         //
//  You should have received a copy of the GNU General Public License      //
//  along with this program; if not, write to the Free Software            //
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
// Heavy based upon class by Phil Anderson (philip@gd-ind.com) on 10-Apr-99
// at http://www.codeguru.com

#include "StdAfx.h"
#include "FtpClient.h"

// DUMMYs for testing
#define IDS_FTPMSG1 10
#define IDS_FTPMSG2 11
#define IDS_FTPMSG3 12
//#define IDS_FTPMSG4 13
#define IDS_FTPMSG5 14
#define IDS_FTPMSG6 15

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define LO        (-2)
#define ER        (-1)
#define NUMLOGIN    9 // Currently supports 9 different login sequences
#define BUFSIZE  4096

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction

CFtpClient::CFtpClient()
{
  m_pCtrlSockFile = NULL;
  m_pCtrlTxArch = NULL;
  m_pCtrlRxArch = NULL;
  m_pCtrlSock = NULL;
}

CFtpClient::~CFtpClient()
{
  CloseControlChannel();
}

/////////////////////////////////////////////////////////////////////////////
// Function to connect and log on to FTP server

BOOL CFtpClient::LogOnToServer(CString strHost, CString strUser,
                               CString strPasswd, int iPort,
                               CString strAccount, CString strFirewallHost,
                               CString strFirewallUser, CString strFirewallPasswd,
                               int iFirewallPort, int iLogonType)
{
  // This array stores all of the logon sequences for the various firewalls
  // in blocks of 3 numbers:
  //   - 1st num is command to send
  //   - 2nd num is next point in logon sequence array if 200 series response
  //     is received from server as the result of the command
  //   - 3rd num is next point in logon sequence if 300 series received
  int iLogonSeq[NUMLOGIN][25] =
  {
    {0,LO,3,  1,LO,6,  2,LO,ER},                            // no firewall
    {3,6,3,   4,6,ER,  5,ER,9, 0,LO,12, 1,LO,15, 2,LO,ER},  // SITE strHost
    {3,6,3,   4,6,ER,  6,LO,9, 1,LO,12, 2,LO,ER},           // USER after logon
    {7,3,3,   0,LO,6,  1,LO,9, 2,LO,ER},                    // proxy OPEN
    {3,6,3,   4,6,ER,  0,LO,9, 1,LO,12, 2,LO,ER},           // Transparent
    {6,LO,3,  1,LO,6,  2,LO,ER},                            // USER with no logon
    {8,6,3,   4,6,ER,  0,LO,9, 1,LO,12, 2,LO,ER},           // USER fireID@remotehost
    {9,ER,3,  1,LO,6,  2,LO,ER},                            // USER remoteID@remotehost fireID
    {10,LO,3, 11,LO,6, 2,LO,ER}                             // USER remoteID@fireID@remotehost
  };

  // Illegal connect code
  if (iLogonType < 0 || iLogonType >= NUMLOGIN)
    return(FALSE);

  // Are we connecting directly to the host (logon type 0) or via a firewall? (logon type>0)
  CString strTmp;
  int iRealPort;
  if (iLogonType == 0)
  {
    strTmp = strHost;
    iRealPort = iPort;
  }
  else
  {
    strTmp = strFirewallHost;
    iRealPort = iFirewallPort;
  }

  // Add port to strHost (only if port is not 21)
  if (iRealPort != 21)
    strHost.Format(strHost + ":%d", iRealPort);

  if (!OpenControlChannel(strTmp, iRealPort))
    return(FALSE);

  // Get initial connect msg off server
  if (!FtpCommand(""))
    return(FALSE);

  // Go through appropriate logon procedure
  int iLogonPoint = 0;
  while (1)
  {
    switch (iLogonSeq[iLogonType][iLogonPoint])
    {
      case  0: strTmp = "USER " + strUser;                                         break;
      case  1: strTmp = "PASS " + strPasswd;                                       break;
      case  2: strTmp = "ACCT " + strAccount;                                      break;
      case  3: strTmp = "USER " + strFirewallUser;                                 break;
      case  4: strTmp = "PASS " + strFirewallPasswd;                               break;
      case  5: strTmp = "SITE " + strHost;                                         break;
      case  6: strTmp = "USER " + strUser + "@" + strHost;                         break;
      case  7: strTmp = "OPEN " + strHost;                                         break;
      case  8: strTmp = "USER " + strFirewallUser + "@" + strHost;                 break;
      case  9: strTmp = "USER " + strUser + "@" + strHost + " " + strFirewallUser; break;
      case 10: strTmp = "USER " + strUser + "@" + strFirewallUser + "@" + strHost; break;
      case 11: strTmp = "PASS " + strPasswd + "@" + strFirewallPasswd;             break;
    }
    // Send command, get response
    if (!WriteStr(strTmp))
      return(FALSE);
    if (!ReadStr())
      return(FALSE);

    // Only these responses are valid
    if (m_iCommandResult != 2 && m_iCommandResult != 3)
      return(FALSE);

    // Get next command from array
    iLogonPoint = iLogonSeq[iLogonType][iLogonPoint + m_iCommandResult - 1];
    switch (iLogonPoint)
    {
      case ER: // ER means summat has gone wrong
        m_strRetMsg.LoadString(IDS_FTPMSG1);
        return(FALSE);
      case LO: // LO means we're fully logged on
        return(TRUE);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Function to log off and close connection to FTP server

void CFtpClient::LogOffServer()
{
  WriteStr("QUIT");
  CloseControlChannel();
}

/////////////////////////////////////////////////////////////////////////////
// Function to execute commands on the FTP server

BOOL CFtpClient::FtpCommand(CString strCmd, int iExpectedResult)
{
  if (!strCmd.IsEmpty())
    if (!WriteStr(strCmd))
      return(FALSE);

  if (!ReadStr())
    return(FALSE);

  return(m_iCommandResult == iExpectedResult);
}

/////////////////////////////////////////////////////////////////////////////
// Function to upload/download files

BOOL CFtpClient::TransferFile(CString strRemoteFile, CString strLocalFile, BOOL bGet, BOOL bPasv)
{
  // Open local file
  CFile fileLocal;
  if (!fileLocal.Open(strLocalFile, (bGet ? (CFile::modeWrite | CFile::modeCreate) : CFile::modeRead)))
  {
    m_strRetMsg.Format("Could not open local file: '%s'", strLocalFile);
    return(FALSE);
  }

  // Request BINARY mode
  if (!FtpCommand("TYPE I"))
    return(FALSE);

  // Declare the various variables used in next if() block
  CString strTmp, rhost;
  UINT i, j, localsock, serversock = 0;
  CSocket sockSrvr;
  CAsyncSocket datachannel;

  // Set up a PASSIVE type file transfer
  if (bPasv)
  {
    if (!FtpCommand("PASV"))
      return(FALSE);

    // Extract connect port number and IP from string returned by server
    i = m_strRetMsg.Find("(");
    j = m_strRetMsg.Find(")");
    if (i == -1 || j == -1)
      return(FALSE);

    strTmp = m_strRetMsg.Mid(i + 1, (j - i) - 1);
    i = strTmp.ReverseFind(',');

    // Get ls byte of server socket
    serversock = atol(strTmp.Right(strTmp.GetLength() - (i + 1)));
    strTmp = strTmp.Left(i);
    i = strTmp.ReverseFind(',');

    // Add ms byte to server socket
    serversock += 256 * atol(strTmp.Right(strTmp.GetLength() - (i + 1)));
    rhost = strTmp.Left(i);

    // Convert commas to dots in IP
    while (1)
    {
      i = rhost.Find(",");
      if (i == -1)
        break;
      rhost.SetAt(i, '.');
    }
  }
  else  // Set up a ACTIVE type file transfer
  {
    m_strRetMsg.LoadString(IDS_FTPMSG6);

    // Get the local IP address off the control channel socket
    CString lhost;
    if (!m_pCtrlSock->GetSockName(lhost, localsock))
      return(FALSE);

    // Convert returned dots in ip address to commas
    while (1)
    {
      i = lhost.Find(".");
      if (i == -1)
        break;
      lhost.SetAt(i, ',');
    }

    // Create listen socket (let MFC choose the port) and start the socket listening
    if (!sockSrvr.Create(0, SOCK_STREAM, NULL) || !sockSrvr.Listen())
      return(FALSE);

    // Get the port that MFC choose
    if (!sockSrvr.GetSockName(strTmp, localsock))
      return(FALSE);

    // Convert the port number to 2 bytes + add to the local IP
    lhost.Format(lhost + ",%d,%d", localsock / 256, localsock % 256);

    // Send PORT cmd to server
    if (!FtpCommand("PORT " + lhost))
      return(FALSE);
  }

  // Send RETR/STOR command to server
  if (!WriteStr((bGet ? "RETR " : "STOR ") + strRemoteFile))
    return(FALSE);

  // If PASV create the socket and initiate outbound data channel connection
  if (bPasv)
  {
    if (!datachannel.Create())
    {
      m_strRetMsg.LoadString(IDS_FTPMSG6);
      return(FALSE);
    }

    // Attempt to connect asynchronously (server will tell us if/when we're connected)
    datachannel.Connect(rhost, serversock);
  }

  // Get response to RETR/STOR command
  if (!ReadStr() || m_iCommandResult != 1)
    return(FALSE);

  // If not PASV accept inbound data connection from server
  if (!bPasv && !sockSrvr.Accept(datachannel))
    return(FALSE);

  // We're connected and ready to do the data transfer, so set blocking mode on data channel socket
  DWORD lpArgument = 0;
  if ((!datachannel.AsyncSelect(0)) || (!datachannel.IOCtl(FIONBIO, &lpArgument)))
  {
    m_strRetMsg.LoadString(IDS_FTPMSG6);
    return(FALSE);
  }

  // Move data from/to server and read/write local file
  char cBuf[BUFSIZE];
  int num, numread, numsent;
  while (1)
  {
    try
    {
      if (bGet)
      {
        // EOF or network error
        num = datachannel.Receive(cBuf, BUFSIZE, 0);
        if (num == 0 || num == SOCKET_ERROR)
          break;
        else
          fileLocal.Write(cBuf, num);
      }
      else
      {
        numread = fileLocal.Read(cBuf, BUFSIZE);
        if (numread == 0)   // EOF
          break;

        numsent = datachannel.Send(cBuf, numread, 0);
        if (numsent == SOCKET_ERROR)
          break;

        // If we sent fewer bytes than we read from file, rewind file pointer
        if (numread != numsent)
          fileLocal.Seek(numsent - numread, CFile::current);
      }
    }
    catch (...)
    {
      m_strRetMsg.LoadString(IDS_FTPMSG5);
      return(FALSE);
    }
  }

  datachannel.Close();
  fileLocal.Close();

  // Check transfer outcome msg from server
  if (!FtpCommand(""))
    return(FALSE);

  // Oh goody it worked.
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CFtpClient::DeleteRemoteFile(CString strFile)
{
  if (!FtpCommand("DELE " + strFile))
    return(FALSE);

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CFtpClient::RenameRemoteFile(CString strFrom, CString strTo)
{
  if (!FtpCommand("RNFR " + strFrom, 3))
    return(FALSE);

  if (!FtpCommand("RNTO " + strTo))
    return(FALSE);

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Function to send a command string on the server control channel

BOOL CFtpClient::WriteStr(CString str)
{
  // Pre-load "network error" msg (in case there is one) #-)
  m_strRetMsg.LoadString(IDS_FTPMSG6);

  try
  {
    m_pCtrlTxArch->WriteString(str + "\r\n");
    m_pCtrlTxArch->Flush();
  }
  catch (...)
  {
    return(FALSE);
  }

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// This function gets the server response line

BOOL CFtpClient::ReadStr()
{
  if (!ReadStrWorker())
    return(FALSE);

  if (m_strRetMsg.GetLength() < 4 || m_strRetMsg.GetAt(3) != '-')
    return(TRUE);

  long lRetCode = atol(m_strRetMsg);

  // Handle multi-line server responses
  while (1)
  {
    if (m_strRetMsg.GetLength() > 3 && (m_strRetMsg.GetAt(3) == ' ' && atol(m_strRetMsg) == lRetCode))
      return(TRUE);
    if (!ReadStrWorker())
      return(FALSE);
  }
}

/////////////////////////////////////////////////////////////////////////////
// Read a single response line from the server control channel

BOOL CFtpClient::ReadStrWorker()
{
  try
  {
    if (!m_pCtrlRxArch->ReadString(m_strRetMsg))
    {
      m_strRetMsg.LoadString(IDS_FTPMSG6);
      return(FALSE);
    }
  }
  catch (...)
  {
    m_strRetMsg.LoadString(IDS_FTPMSG6);
    return(FALSE);
  }

  // Get first digit of the return code (indicates primary result)
  if (m_strRetMsg.GetLength() > 0)
    m_iCommandResult = m_strRetMsg.GetAt(0) - 48;

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Open the control channel to the FTP server

BOOL CFtpClient::OpenControlChannel(CString strHost, int iPort)
{
  m_strRetMsg.LoadString(IDS_FTPMSG2);

  m_pCtrlSock = new CSocket;
  if (m_pCtrlSock == NULL)
    return(FALSE);

  if (!m_pCtrlSock->Create())
    return(FALSE);

  m_strRetMsg.LoadString(IDS_FTPMSG3);
  if (!(m_pCtrlSock->Connect(strHost, iPort)))
    return(FALSE);

  m_strRetMsg.LoadString(IDS_FTPMSG2);
  m_pCtrlSockFile = new CSocketFile(m_pCtrlSock);
  if (m_pCtrlSockFile == NULL)
    return(FALSE);

  m_pCtrlRxArch = new CArchive(m_pCtrlSockFile, CArchive::load);
  if (m_pCtrlRxArch == NULL)
    return(FALSE);

  m_pCtrlTxArch = new CArchive(m_pCtrlSockFile, CArchive::store);
  if (m_pCtrlTxArch == NULL)
    return(FALSE);

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Close the control channel to the FTP server

void CFtpClient::CloseControlChannel()
{
  if (m_pCtrlTxArch != NULL)
  {
    delete m_pCtrlTxArch;
    m_pCtrlTxArch = NULL;
  }

  if (m_pCtrlRxArch != NULL)
  {
    delete m_pCtrlRxArch;
    m_pCtrlRxArch = NULL;
  }

  if (m_pCtrlSockFile != NULL)
  {
    delete m_pCtrlSockFile;
    m_pCtrlSockFile = NULL;
  }

  if (m_pCtrlSock != NULL)
  {
    delete m_pCtrlSock;
    m_pCtrlSock = NULL;
  }
}
