#ifndef _MANET_NHDP
#define _MANET_NHDP

#include "protoSocket.h"
#include "protoTimer.h"

class Nhdp
{
    public:
        Nhdp(ProtoSocket::Notifier& socketNotifier,
             ProtoTimerMgr&         timerMgr);
        ~Nhdp();
        
        bool IsRunning() const
            {return (nhdp_socket.IsOpen());}
        
        bool SetInterface(const char* ifaceName, ProtoAddress::Type addrType = ProtoAddress::IPv4);
        
        void SetAddress(const ProtoAddress& theAddr)
            {nhdp_addr = theAddr;}
        
        
        int GetPort() const
            {return (nhdp_addr.IsValid() ? nhdp_addr.GetPort() : -1);}
        
        bool Start();
        void Stop();
        
    private:
        bool OnHelloTimeout(ProtoTimer& theTimer);
        void OnSocketEvent(ProtoSocket&       theSocket, 
                           ProtoSocket::Event theEvent);
        
        ProtoTimerMgr&      timer_mgr;
        
        ProtoSocket         nhdp_socket;
        ProtoTimer          hello_timer; 
        
        char                nhdp_iface[64];
        ProtoAddress        nhdp_addr;
        ProtoAddressList    iface_addr_list;
    
}; // end class Nhdp

    
#endif // _MANET_NHDP
