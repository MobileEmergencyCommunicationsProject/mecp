#ifndef _MANET_GRAPHML
#define _MANET_GRAPHML
#define MAXXMLIDLENGTH 255

#include <manetGraph.h>
#include <protoQueue.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>

#define MY_GRAPHML_ENCODING "UTF-8"
class ManetGraphMLParser
{
    public:
        virtual ~ManetGraphMLParser();
        const char* GetString(NetGraph::Interface& iface) const; 
        const char* GetString(NetGraph::Node& node) const;
        class AttributeKey : public ProtoQueue::Item 
        {
            public:
                struct Domains {enum Domain {INVALID, GRAPH, NODE, EDGE, ALL};};
                struct Types {enum Type { INVALID, BOOL, INT, LONG, FLOAT, DOUBLE, STRING };};
                //AttributeKey() : index(NULL), name(NULL), oldkey(NULL), domain(Domains::INVALID), type(Types::INVALID){}
                AttributeKey() : index(NULL), name(NULL), oldindex(NULL), defaultvalue(NULL), domain(Domains::INVALID),type(Types::INVALID){}
                bool Init(const char* theIndex,const char* theName,const char* theType, const char* theDomain = NULL,const char* theOldKey=NULL,const char* theDefault = NULL);
                ~AttributeKey();

                const char* GetIndex() const {return index;}
                const char* GetName() const {return name;}
                const char* GetOldIndex() const {return oldindex;}
                Types::Type GetType() {return type;}
                Domains::Domain GetDomain() {return domain;}
                const char* GetDefault() const {return defaultvalue;}
            private:
                char* index;
                char* name;
                char* oldindex;
                char* defaultvalue;
                Domains::Domain domain;
                Types::Type type;
                
                bool SetDomain(const char* theDomain);
                bool SetType(const char* theType);
                bool SetDefault(const char* theDefault);
        };
    protected:         
        ManetGraphMLParser();
        bool Read(const char* path, NetGraph& graph);   // load graph from GraphML file
        bool Write(NetGraph& graph, const char* path);  // make GraphML file from graph

        bool AddAttributeKey(const char* theName,const char* theType, const char* theDomain = NULL, const char* oldIndex = NULL,const char* theDefault = NULL);
        bool AddAttribute(NetGraph::Node& node, const char* theName, const char* theValue);
        bool AddAttribute(NetGraph::Link& link, const char* theName, const char* theValue);
        bool AddAttribute(NetGraph::Interface& interface, const char* theName, const char* theValue);
    private:
        const char* FindAttributeIndex(const char* theName);
        AttributeKey* FindAttributeKeyByOldIndex(const char* theOldIndex);

        //the next three classes are lists which provide index storage and look ups
        class IndexKeylist : public ProtoSortedQueueTemplate<AttributeKey>
        {
            const char* GetKey(const ProtoQueue::Item& item) const
                {return static_cast<const AttributeKey&>(item).GetIndex();}
            unsigned int GetKeysize(const ProtoQueue::Item& item) const
                {return 8*strlen(static_cast<const AttributeKey&>(item).GetIndex());}
        } indexkeylist;
        class NamedKeylist : public ProtoSortedQueueTemplate<AttributeKey>
        {
            const char* GetKey(const ProtoQueue::Item& item) const
                {return static_cast<const AttributeKey&>(item).GetName();}
            unsigned int GetKeysize(const ProtoQueue::Item& item) const
                {return 8*strlen(static_cast<const AttributeKey&>(item).GetName());}
        } namedkeylist;
        class OldIndexKeylist : public ProtoSortedQueueTemplate<AttributeKey>
        {
            const char* GetKey(const ProtoQueue::Item& item) const
                {return static_cast<const AttributeKey&>(item).GetOldIndex();}
            unsigned int GetKeysize(const ProtoQueue::Item& item) const
                {return 8*strlen(static_cast<const AttributeKey&>(item).GetOldIndex());}
        } oldindexkeylist;
        
        int indexes;
        class Attribute : public ProtoQueue::Item 
        {
            public:
                char* lookupvalue; //this string defines which node/port/edge the value belongs to.
                char* index;
                char* value;
                Attribute() : lookupvalue(NULL), index(NULL), value(NULL) {}
                bool Init(const char* theLookupvalue, const char* theIndex,const char* theValue);
                ~Attribute();
                const char* GetLookup() const {return lookupvalue;}
                const char* GetIndex() const {return index;}
                const char* GetValue() const {return value;}
        };
        class AttributeList : public ProtoSortedQueueTemplate<Attribute>
        {
            const char* GetKey(const ProtoQueue::Item& item) const
                {return static_cast<const Attribute&>(item).GetLookup();}
            unsigned int GetKeysize(const ProtoQueue::Item& item) const
                {return 8*strlen(static_cast<const Attribute&>(item).GetLookup());}
        } attributelist;
//        virtual bool WriteKeys(xmlTextWriter* writerPtr, NetGraph& graph) = 0;
        virtual bool UpdateKeys(NetGraph& graph) = 0; //this will replace the above as all keys will be stored locally
        bool WriteLocalKeys(xmlTextWriter* writerPtr);

        virtual NetGraph::Node* CreateNode() = 0;
//        virtual bool WriteNodeAttributes(xmlTextWriter* writerPtr,NetGraph::Node& theNode) = 0;
        virtual bool UpdateNodeAttributes(NetGraph::Node& theNode) = 0; //this will replace the above so keys will be stored locally
        bool WriteLocalNodeAttributes(xmlTextWriter* writerPtr,NetGraph::Node& theNode);

        virtual NetGraph::Interface* CreateInterface(NetGraph::Node& node) = 0;
        virtual NetGraph::Interface* CreateInterface(NetGraph::Node& node,ProtoAddress& addr) = 0;
  //      virtual bool WriteInterfaceAttributes(xmlTextWriter* writerPtr,NetGraph::Interface& theInterface) = 0;
        virtual bool UpdateInterfaceAttributes(NetGraph::Interface& theInterface) = 0; //this will replace the above so keys will be stored locally
        bool WriteLocalInterfaceAttributes(xmlTextWriter* writerPtr,NetGraph::Interface& theInterface);

        virtual NetGraph::Cost* CreateCost(double value) = 0;

        virtual bool Connect(NetGraph::Interface& iface1, NetGraph::Interface& iface2, NetGraph::Cost& cost, bool isDuplex) = 0;
//        virtual bool WriteLinkAttributes(xmlTextWriter* writerPtr,NetGraph::Link& theLink) = 0;
        virtual bool UpdateLinkAttributes(NetGraph::Link& theLink) = 0; //this will replace the above so keys will be stored locally
        bool WriteLocalLinkAttributes(xmlTextWriter* writerPtr,NetGraph::Link& theLInk); 
      
        bool ReadXMLNode(xmlTextReader*   readerPtr, 
                         NetGraph&        graph, 
                         char*            parentXMLGraphID, 
                         char*            parentXMLNodeID, 
                         bool&            isDuplex);
};  // end class ManetGraphMLParser

template <class COST_TYPE = ManetGraph::Cost, class IFACE_TYPE = ManetGraph::Interface, class LINK_TYPE = ManetGraph::Link,class NODE_TYPE = ManetGraph::Node>
class ManetGraphMLTemplate : public ManetGraphMLParser, public NetGraphTemplate<COST_TYPE, IFACE_TYPE, LINK_TYPE, NODE_TYPE>
{
    public:
        ManetGraphMLTemplate() {}
        virtual ~ManetGraphMLTemplate() {}
                
        bool Read(const char* path)   // load graph from GraphML file
            {return ManetGraphMLParser::Read(path, *this);}
            
        bool Write(const char* path)  // make GraphML file from graph
            {return ManetGraphMLParser::Write(*this, path);}
        bool Connect(NetGraph::Interface& iface1,NetGraph::Interface& iface2,NetGraph::Cost& theCost,bool isDuplex)
            {return NetGraph::Connect(iface1,iface2,theCost,isDuplex);}
    protected:
  //      virtual bool WriteKeys(xmlTextWriter* writerPtr, NetGraph& theGraph)
  //          {return true;}
        virtual bool UpdateKeys(NetGraph& theGraph)
            {return true;}
  /*      virtual bool WriteNodeAttributes(xmlTextWriter* writerPtr, NetGraph::Node& theNode)
            {return true;}
        virtual bool WriteInterfaceAttributes(xmlTextWriter* writerPtr, NetGraph::Interface& theInterface)
            {return true;}
        virtual bool WriteLinkAttributes(xmlTextWriter* writerPtr, NetGraph::Link& theLInk)
            {return true;}*/
        virtual bool UpdateNodeAttributes(NetGraph::Node& theNode)
            {return true;}
        virtual bool UpdateInterfaceAttributes(NetGraph::Interface& theInterface)
            {return true;}
        virtual bool UpdateLinkAttributes(NetGraph::Link& theLInk)
            {return true;}
    private:
        class NetGraph::Node* CreateNode()
            {return static_cast<NetGraph::Node*>(new NODE_TYPE());}
        class NetGraph::Interface* CreateInterface(NetGraph::Node& node)
            {return static_cast<NetGraph::Interface*>(new IFACE_TYPE(static_cast<NODE_TYPE&>(node)));}
        class NetGraph::Interface* CreateInterface(NetGraph::Node& node,ProtoAddress& addr)
            {return static_cast<NetGraph::Interface*>(new IFACE_TYPE(static_cast<NODE_TYPE&>(node),addr));}
        class NetGraph::Cost* CreateCost(double value)
            {return static_cast<NetGraph::Cost*>(new COST_TYPE(value));}
        class NetGraph::Cost* CreateCost()
            {return static_cast<NetGraph::Cost*>(new COST_TYPE());}
};  // end class ManetGraphMLParser    


//class ManetGraphML : public ManetGraphMLTemplate<> {};

#endif // _MANET_GRAPHML
