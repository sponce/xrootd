//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientConnMgr                                                     //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
// Adapted from TXNetFile (root.cern.ch) originally done by             //
//  Alvise Dorigo, Fabrizio Furano                                      //
//          INFN Padova, 2003                                           //
//                                                                      //
// The connection manager maps multiple logical connections on a single //
// physical connection.                                                 //
// There is one and only one logical connection per client              //
// and one and only one physical connection per server:port.            //
// Thus multiple objects withing a given application share              //
// the same physical TCP channel to communicate with a server.          //
// This reduces the time overhead for socket creation and reduces also  //
// the server load due to handling many sockets.                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//       $Id$

#ifndef XRC_CONNMGR_H
#define XRC_CONNMGR_H



#include <pthread.h>
#include "XrdClient/XrdClientMutex.hh"
#include "XrdClient/XrdClientUnsolMsg.hh"
#include "XrdClient/XrdClientLogConnection.hh"
#include "XrdClient/XrdClientMessage.hh"
#include "XrdClient/XrdClientVector.hh"
#include "XrdClient/XrdClientThread.hh"

#define ConnectionManager XrdClientConnectionMgr::Instance()



// Ugly prototype to avoid warnings under solaris
extern "C" void * GarbageCollectorThread(void * arg, XrdClientThread *thr);

class XrdClientConnectionMgr: public XrdClientAbsUnsolMsgHandler, 
                       XrdClientUnsolMsgSender {
private:
   XrdClientVector<XrdClientLogConnection*> fLogVec;
   XrdClientVector<XrdClientPhyConnection*> fPhyVec;
   XrdClientMutex             fMutex; // mutex used to protect local variables
                                      // of this and TXLogConnection, TXPhyConnection
                                      // classes; not used to protect i/o streams

   XrdClientThread            *fGarbageColl;


   static XrdClientConnectionMgr*   fgInstance;

   void          GarbageCollect();
   friend void * GarbageCollectorThread(void *, XrdClientThread *thr);
   bool          ProcessUnsolicitedMsg(XrdClientUnsolMsgSender *sender,
                                       XrdClientMessage *unsolmsg);
protected:
   XrdClientConnectionMgr();

public:
   virtual ~XrdClientConnectionMgr();

   short         Connect(XrdClientUrlInfo RemoteAddress);
   void          Disconnect(short LogConnectionID, bool ForcePhysicalDisc);
   XrdClientLogConnection *GetConnection(short LogConnectionID);
   short         GetPhyConnectionRefCount(XrdClientPhyConnection *PhyConn);

   XrdClientMessage*   ReadMsg(short LogConnectionID);

   int           ReadRaw(short LogConnectionID, void *buffer, int BufferLength);
   int           WriteRaw(short LogConnectionID, const void *buffer, 
                          int BufferLength);

   static XrdClientConnectionMgr* Instance();
   static bool IsAlive() {
     return (fgInstance);
   }

   static void              Reset();


};


#endif
