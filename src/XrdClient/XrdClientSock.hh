//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientSock                                                        //
//                                                                      //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
// Adapted from TXNetFile (root.cern.ch) originally done by             //
//  Alvise Dorigo, Fabrizio Furano                                      //
//          INFN Padova, 2003                                           //
//                                                                      //
// Client Socket with timeout features                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef XRC_DNS_H
#define XRC_DNS_H

#include <XrdClient/XrdClientSockImp.hh>

class XrdClientUrlInfo;

class XrdClientSock {

private:

   XrdClientSockImp  *fSockImp;

public:
   XrdClientSock(XrdClientUrlInfo host, int windowsize = 0);
   virtual ~XrdClientSock() { if (fSockImp) delete fSockImp; }

   int    RecvRaw(void* buffer, int length);
   int    SendRaw(const void* buffer, int length);

   void   TryConnect();

   void   Disconnect();

   bool   IsConnected();
};

#endif
