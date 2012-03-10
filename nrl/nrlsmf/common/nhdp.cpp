#include "nhdp.h"

Nhdp::Nhdp(ProtoSocket::Notifier& socketNotifier,
           ProtoTimerMgr&         timerMgr)
  : nhdp_socket(ProtoSocket::UDP), timer_mgr(timerMgr)
{
    nhdp_iface[0] = '\0';
    
    nhdp_socket.SetNotifier(&socketNotifier);
    
    nhdp_socket.SetListener(this, &Nhdp::OnSocketEvent);
    
    hello_timer.SetListener(this, &Nhdp::OnHelloTimeout);
    hello_timer.SetRepeat(-1);
}


Nhdp::~Nhdp()
{
    Stop();
}


bool Nhdp::SetInterface(const char* ifaceName, ProtoAddress::Type addrType)
{
    unsigned int ifaceIndex = ProtoSocket::GetInterfaceIndex(ifaceName);
    if (0 == ifaceIndex)
    {
        DMSG(0, "Nhdp::SetInterface() error: invalid interface name\n");
        return false;
    } 
    nhdp_iface[63] = '\0';
    if (!ProtoSocket::GetInterfaceName(ifaceIndex, nhdp_iface, 63))
    {
        // this shouldn't happen since GetInterfaceIndex() succeeded?
        DMSG(0, "Nhdp::SetInterface() error: couldn't resolve interface name\n");
        return false;
    }
    iface_addr_list.Destroy();
    if (!ProtoSocket::GetInterfaceAddressList(nhdp_iface, addrType, iface_addr_list))
    {
        DMSG(0, "Nhdp::SetInterface() error: couldn't get address list\n");
        return false;
    }
    
    // Display our "iface_addr_list"
    ProtoAddressList::Iterator it(iface_addr_list);
    ProtoAddress addr;
    while (it.GetNextAddress(addr))
        TRACE("Nhdp::SetInterface(%s) has local address: %s\n", ifaceName, addr.GetHostString());
    return true;
}  // end Nhdp::SetInterface()

bool Nhdp::Start()
{
    if ('\0' == nhdp_iface[0])
    {
        // (TBD) should we allow for operation with a "default" iface?
        DMSG(0, "Nhdp::Start() error: no interface specified\n");
        return false;
    }
    
    if (nhdp_addr.IsValid())
    {
        
        if (!nhdp_socket.Open(nhdp_addr.GetPort()))
        {
            DMSG(0, "Nhdp::Start() error: couldn't open socket on desired port number\n");
            return false;
        }
        if (nhdp_addr.IsMulticast())
        {
            TRACE("joining group %s\n", nhdp_addr.GetHostString());
            if (!nhdp_socket.JoinGroup(nhdp_addr, nhdp_iface))
            {
                DMSG(0, "Nhdp::Start() error: couldn't join NHDP group on specified interface\n");
                nhdp_socket.Close();
                return false;
            }
            TRACE("setting mcast iface ...\n");
            nhdp_socket.SetMulticastInterface(nhdp_iface);
        }
        
    }
    else
    {
        DMSG(0, "Nhdp::Start() error: invalid address\n");
        return false;
    }
    
   timer_mgr.ActivateTimer(hello_timer);
   
   
   
   return true;
}  // end Nhdp::Start()

void Nhdp::Stop()
{
    if (nhdp_socket.IsOpen())
        nhdp_socket.Close();
    if (hello_timer.IsActive())
        hello_timer.Deactivate();   
}  // end Nhdp::Stop()


bool Nhdp::OnHelloTimeout(ProtoTimer& /*theTimer*/)
{
    TRACE("Nhdp::OnHelloTimeout() ...\n");
    
    // 1) Build an NHDP_HELLO message
    
    return true;
}  // end Nhdp::OnHelloTimeout()


void Nhdp::OnSocketEvent(ProtoSocket& /*theSocket*/, 
                         ProtoSocket::Event theEvent)
{
    TRACE("Nhdp::OnSocketEvent() ...\n");
}  // end Nhdp::OnSocketEvent()
