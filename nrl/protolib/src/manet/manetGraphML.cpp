#include <manetGraphML.h>
#include <protoDebug.h>

ManetGraphMLParser::ManetGraphMLParser() : indexes(0) 
{
}

ManetGraphMLParser::~ManetGraphMLParser() 
{
}
const char*
ManetGraphMLParser::GetString(NetGraph::Interface& iface) const
{
    if(iface.GetDefaultAddress().IsValid())
    {
        return iface.GetDefaultAddress().GetHostString();
    }
    else
    {
        return iface.GetName();
    }
}
const char*
ManetGraphMLParser::GetString(NetGraph::Node& node) const
{
    return GetString(*node.GetDefaultInterface());
}
bool
ManetGraphMLParser::AttributeKey::Init(const char* theIndex,const char* theName,const char* theType,const char* theDomain, const char* theOldIndex,const char*theDefault)
{
    if((NULL != index) || (NULL != name) || (NULL != oldindex) || (NULL !=type) || (NULL != domain) )
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AttributeKey::Init: Error attempting to Init AttributeKey which has already called Init!\n");
        return false;
    }
    index = new char[strlen(theIndex)];
    name = new char[strlen(theName)];
    
    if((NULL == index) || (NULL == name))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AttributeKey::Init: Error allocating space for index or name strings\n");
        return false;
    }
    if(NULL != theOldIndex)
    {
        oldindex = new char[strlen(theOldIndex)];
        if(NULL == oldindex)
        {
            PLOG(PL_ERROR,"ManetGraphMLParser::AttributeKey::Init: Error allocating space for oldindex string\n");
            return false;
        }
        strcpy(oldindex,theOldIndex);
    }
    strcpy(name,theName);
    strcpy(index,theIndex);
    if(!SetType(theType))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AttributeKey::Init: Error setting the type to %s\n",theType);
        return false;
    }
    if(NULL != theDomain)
    {
        if(!SetDomain(theDomain))
        {
            PLOG(PL_ERROR,"ManetGraphMLParser::AttributeKey::Init: Error setting the domain to %s\n",theDomain);
            return false;
        }
    }
    if(NULL != theDefault)
    {
        if(!SetDefault(theDefault))
        {
            PLOG(PL_ERROR,"ManetGraphMLParser::AttributeKey::Init: Error setting the default to %s\n",theDefault);
            return false;
        }
    }
    return true;
}
bool
ManetGraphMLParser::AttributeKey::SetType(const char* theType)
{
    if((!strcmp(theType,"bool")) || 
       (!strcmp(theType,"Bool")) ||
       (!strcmp(theType,"BOOL")) ||
       (!strcmp(theType,"boolean")) ||
       (!strcmp(theType,"Boolean")) ||
       (!strcmp(theType,"BOOLEAN"))) {
        type = Types::BOOL;
    } else if 
       ((!strcmp(theType,"int")) ||
        (!strcmp(theType,"Int")) ||
        (!strcmp(theType,"INT")) ||
        (!strcmp(theType,"integer")) ||
        (!strcmp(theType,"Integer")) ||
        (!strcmp(theType,"INTEGER"))) {
         type = Types::INT;
    } else if 
       ((!strcmp(theType,"long")) ||
        (!strcmp(theType,"Long")) ||
        (!strcmp(theType,"LONG"))) {
         type = Types::LONG;
    } else if 
       ((!strcmp(theType,"float")) ||
        (!strcmp(theType,"Float")) ||
        (!strcmp(theType,"FLOAT"))) {
         type = Types::FLOAT;
    } else if 
       ((!strcmp(theType,"double")) ||
        (!strcmp(theType,"Double")) ||
        (!strcmp(theType,"DOUBLE"))) {
         type = Types::DOUBLE;
    } else if 
       ((!strcmp(theType,"string")) ||
        (!strcmp(theType,"String")) ||
        (!strcmp(theType,"STRING"))) {
         type = Types::STRING;
    } else {
        type = Types::INVALID;
        return false;
    }
    return true;
}
bool
ManetGraphMLParser::AttributeKey::SetDomain(const char* theDomain)
{
    if((!strcmp(theDomain,"graph")) || 
       (!strcmp(theDomain,"Graph")) ||
       (!strcmp(theDomain,"GRAPH"))) {
        domain = Domains::GRAPH;
    } else if 
       ((!strcmp(theDomain,"node")) ||
        (!strcmp(theDomain,"Node")) ||
        (!strcmp(theDomain,"NODE"))) {
         domain = Domains::NODE;
    } else if 
       ((!strcmp(theDomain,"edge")) ||
        (!strcmp(theDomain,"Edge")) ||
        (!strcmp(theDomain,"EDGE"))) {
         domain = Domains::EDGE;
    } else if 
       ((!strcmp(theDomain,"all")) ||
        (!strcmp(theDomain,"All")) ||
        (!strcmp(theDomain,"ALL"))) {
         domain = Domains::ALL;
    } else {
        domain = Domains::INVALID;
        return false;
    }
    return true;
}
bool
ManetGraphMLParser::AttributeKey::SetDefault(const char* theDefault)
{
    if(NULL == theDefault)
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AttributeKey::SetDefault: default value pointer is NULL!\n");
        return false;
    }
    if(NULL != defaultvalue)
    {
        PLOG(PL_WARN,"ManetGraphMLParser::AttributeKey::SetDefault: default has already been set to \"%s\" and is being switched to \"%s\"\n",defaultvalue,theDefault);
        delete[] defaultvalue;
    }
    defaultvalue = new char[strlen(theDefault)];
    if(NULL == defaultvalue)
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AttributeKey::SetDefault: error allocing space for the defaultvalue pointer!\n");
        return false;
    }
    if(!strcpy(defaultvalue,theDefault))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AttributeKey::SetDefault: Error copying the string!\n");
        delete[] defaultvalue;
        return false;
    }
    return true;
}
ManetGraphMLParser::AttributeKey::~AttributeKey()
{
    delete[] index;
    delete[] name;
    if(NULL != oldindex)
        delete[] oldindex;
    if(NULL != defaultvalue)
        delete[] defaultvalue;
}
bool
ManetGraphMLParser::AddAttributeKey(const char* theName,const char* theType, const char* theDomain, const char* theOldKey,const char* theDefault)
{
    //lets look to see if the name exists already
    if(NULL != FindAttributeIndex(theName))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttributeKey has been called with attribute name \"%s\" which already exists!\n",theName);
        return false;
    }
    AttributeKey* newKey = new AttributeKey;
    if(NULL == newKey)
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttributeKey Error allocating a new AttributeKey\n");
        return false;
    }
    char theIndex[20];//this will work as we are only using d%d as our indexes.
    sprintf(theIndex,"d%d",indexes++);
    if(!newKey->Init(theIndex,theName,theType,theDomain,theOldKey,theDefault))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttributeKey Error Init'ing the newKey with index \"d%d\" and name \"%s\"\n");
        delete newKey;
        indexes--;
        return false;
    }
    indexkeylist.Insert(*newKey);
    namedkeylist.Insert(*newKey);
    if(NULL != theOldKey)
        oldindexkeylist.Insert(*newKey);
    return true;
}
bool ManetGraphMLParser::AddAttribute(NetGraph::Node& node, const char* theName, const char* theValue)
{
    char theLookup[250];    //TBD
    const char* theIndex = FindAttributeIndex(theName);
    if(NULL == theIndex)
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(Node): Error finding index for attribute %s\n",theName);
        return false;
    }
    Attribute* newAttribute = new Attribute();
    if(NULL == newAttribute)
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(Node): Error allocating attribute\n");
        return false;
    }
     
    sprintf(theLookup,"node:%s",GetString(node));
    if(!newAttribute->Init(theLookup,theIndex,theValue))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(Node): Error init the attribute\n");
        return false;
    }
    if(!attributelist.Insert(*newAttribute))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(Node): Error inserting the attribute\n");
        return false;
    }
    
    return true;
}
bool ManetGraphMLParser::AddAttribute(NetGraph::Link& link, const char* theName, const char* theValue)
{
    char theLookup[250];
    const char* theIndex = FindAttributeIndex(theName);
    if(NULL == theIndex)
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(Link): Error finding index for attribute %s\n",theName);
        return false;
    }
    Attribute* newAttribute = new Attribute();
    if(NULL == newAttribute)
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(Link): Error allocating attribute\n");
        return false;
    }
    char* sourceName;
    char* sourcePortName;
    char* targetName;
    char* targetPortName;
    sourceName = new char[strlen(GetString(link.GetSrc()->GetNode()))+1];
    strcpy(sourceName,GetString(link.GetSrc()->GetNode()));
    sourcePortName = new char[strlen(GetString(*link.GetSrc()))+1];
    strcpy(sourcePortName,GetString(*link.GetSrc()));
    if(!strcmp(sourcePortName,sourceName))
    {
        delete[] sourcePortName;
        sourcePortName = NULL;
    }
    targetName = new char[strlen(GetString(link.GetDst()->GetNode()))+1];
    strcpy(targetName,GetString(link.GetDst()->GetNode()));
    targetPortName = new char[strlen(GetString(*link.GetDst()))+1];
    strcpy(targetPortName,GetString(*link.GetDst()));
    if(!strcmp(targetPortName,targetName))
    {

        delete[] targetPortName;
        targetPortName = NULL;
    }
    sprintf(theLookup,"edge:source:%s:%s:dest:%s:%s",sourceName,sourcePortName,targetName,targetPortName);

    delete[] sourceName;
    if(NULL != sourcePortName) delete[] sourcePortName;
    delete[] targetName;
    if(NULL != targetPortName) delete[] targetPortName;
    
    if(!newAttribute->Init(theLookup,theIndex,theValue))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(Link): Error initing the attribute\n");
        return false;
    }
    if(!attributelist.Insert(*newAttribute))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(Link): Error inserting the attribute\n");
        return false;
    }
    return true; 
}
bool ManetGraphMLParser::AddAttribute(NetGraph::Interface& interface, const char* theName, const char* theValue)
{
    char theLookup[250];
    const char* theIndex = FindAttributeIndex(theName);
    if(NULL == theIndex)
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(interface): Error finding index for attribute %s\n",theName);
        return false;
    }
    Attribute* newAttribute = new Attribute();
    if(NULL == newAttribute)
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(Link): Error allocating attribute\n");
        return false;
    }
    char *portName;
    portName = new char[strlen(GetString(interface))+1];
    strcpy(portName,GetString(interface));
    if(!strcmp(portName,GetString(interface.GetNode())))
    {
        delete[] portName;
        portName = NULL;
    }
    sprintf(theLookup,"node:%s:port:%s",GetString(interface.GetNode()),portName);
    if(NULL != portName) delete[] portName;
    
    if(!newAttribute->Init(theLookup,theIndex,theValue))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(Interface): Error init the attribute\n");
        return false;
    }
    if(!attributelist.Insert(*newAttribute))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::AddAttribute(Interface): Error inserting the attribute\n");
        return false;
    }
    return true;
}
const char* ManetGraphMLParser::FindAttributeIndex(const char* theName)
{
    AttributeKey* theKey = namedkeylist.Find(theName,strlen(theName)*8);
    if(NULL == theKey)
    {
        return NULL;
    }
    return theKey->GetIndex();
}     
ManetGraphMLParser::AttributeKey* ManetGraphMLParser::FindAttributeKeyByOldIndex(const char* theOldIndex)
{
    return oldindexkeylist.Find(theOldIndex,strlen(theOldIndex)*8);
}
bool
ManetGraphMLParser::Attribute::Init(const char* theLookupvalue, const char* theIndex,const char* theValue)
{
    if((NULL!=lookupvalue) || (NULL != index) || (NULL != value))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::Attribute::Init: Error attempting to Init Attribute which has already called Init!\n");
        return false;
    }
    lookupvalue = new char[strlen(theLookupvalue)];
    index = new char[strlen(theIndex)];
    value = new char[strlen(theValue)];
    if((NULL == lookupvalue) || (NULL == index) || (NULL == value))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::Attribute::Init: Error allocating space for lookup/index/value strings\n");
        return false;
    }
    strcpy(lookupvalue,theLookupvalue);
    strcpy(value,theValue);
    strcpy(index,theIndex);
    return true;
}
ManetGraphMLParser::Attribute::~Attribute()
{
    delete[] lookupvalue;
    delete[] index;
    delete[] value;
}
bool ManetGraphMLParser::ReadXMLNode(xmlTextReader*   readerPtr, 
                                       NetGraph&        graph, 
                                       char*            parentXMLGraphID, 
                                       char*            parentXMLNodeID, 
                                       bool&            isDuplex)
{
    const xmlChar *name, *value;
    int count, depth, type, isempty;

    type = xmlTextReaderNodeType(readerPtr);
    if(XML_READER_TYPE_END_ELEMENT==type)
        return true;

    name = xmlTextReaderConstName(readerPtr);
    if (name == NULL)
      name = BAD_CAST "--";
    value = xmlTextReaderConstValue(readerPtr);
    depth = xmlTextReaderDepth(readerPtr);
    isempty = xmlTextReaderIsEmptyElement(readerPtr); 
    count = xmlTextReaderAttributeCount(readerPtr);
    //printf("Bunny processsing depth=%d, type=%d, name=%s, isempty=%d, value=%s, attributes=%d\n",depth,type,name,isempty,value,count);
    if(!strcmp("graph",(const char*)name))
    {
        //printf("bunny processing graph node\n");
        xmlChar* graphId;
        graphId = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"id");
        if(NULL == graphId)
        {
            PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode(): Error missing graph id attribute!\n");
            return false;
        }
        if(strlen((const char*)graphId) > MAXXMLIDLENGTH)
        {
            PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode: Error the graph id value of \"%s\" is too large\n",graphId);
            return false;
        }
        strcpy(parentXMLGraphID,(const char*)graphId);
        xmlChar* graphEdgeType;
        graphEdgeType = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"edgedefault");
        if(!strcmp("directed",(const char*)graphEdgeType))
        {
            isDuplex = false;
        }
        else
        {
            isDuplex = true;
        }
        //printf("bunny exiting graph node id = \"%s\" is duplex is %s\n",graphId,isDuplex ? "true" : "false");
    }
    else if(!strcmp("node",(const char*)name))
    {
        xmlChar* nodeId = NULL;
        while (xmlTextReaderMoveToNextAttribute(readerPtr)>0)
        {

            //printf("bunny found attribute %s with name %s\n",xmlTextReaderName(readerPtr),xmlTextReaderConstValue(readerPtr));
            if(!strcmp((const char*)xmlTextReaderConstName(readerPtr),"id"))
            {
                nodeId = xmlTextReaderValue(readerPtr);
            }
        }
        if(NULL == nodeId)
        {
            PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode(): Error missing node id attribute!\n");
            return false;
        }
        if(strlen((const char*)nodeId) > MAXXMLIDLENGTH)
        {
            PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode: Error the node id value of \"%s\" is too large\n",nodeId);
            return false;
        }
        strcpy(parentXMLNodeID,(const char*)nodeId);
        
        NetGraph::Interface* interface;
        ProtoAddress addr;
        addr.ResolveFromString((const char*)nodeId);
        if(addr.IsValid())
        {
            interface = graph.FindInterface(addr);
        } 
        else 
        {
            interface = graph.FindInterfaceByName((const char*)nodeId);
        }
        if (NULL == interface)
        {
            //Create new node and associated interface
            NetGraph::Node* node = CreateNode();
            
            if(NULL == node)
            {
                PLOG(PL_ERROR, "NetGraph::ProcessXMLNode: error Creating new node!\n");
                return -1;
            }    
            if(addr.IsValid())
            {
                interface = CreateInterface(*node,addr);
            } 
            else 
            {
                interface = CreateInterface(*node);
                interface->SetName((const char*)nodeId);
            }
            node->AddInterface(*interface,true);
            graph.InsertInterface(*interface);

        }
        else
        {
            //existing node so update anything we might have on it
        }
        //printf("bunny exiting node node\n");
    }
    else if(!strcmp("port",(const char*)name))
    {
        //printf("bunny processing port node\n");
        xmlChar* portname;
        portname = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"name");
        NetGraph::Interface* portinterface;
        portinterface = graph.FindInterfaceByString((const char*)portname);

        ProtoAddress portAddr;
        portAddr.ResolveFromString((const char*)portname);
        if (NULL == portinterface)
        {
            //Find the parent node and associated with this interface
            NetGraph::Interface* interface;
            interface = graph.FindInterfaceByString(parentXMLNodeID);
            ProtoAddress addr;
            addr.ResolveFromString(parentXMLNodeID);
            if(NULL == interface)
            {
                PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode: Error finding the parent \"%s\" interface in the graph for port \"%s\"!\n",parentXMLNodeID,portname);
                return false;
            }
            
            NetGraph::Node& node = interface->GetNode();
            //NetGraph::Node *node = &interface->GetNode();
            if(portAddr.IsValid())
            {
                portinterface = new NetGraph::Interface(node,portAddr);
            } 
            else 
            {
                portinterface = CreateInterface(node);
                portinterface->SetName((const char*)portname);
            }
            //node->SetPosition(x, y);
            node.AddInterface(*portinterface,false);
            graph.InsertInterface(*portinterface);
            //printf(" bunny just added port \"%s\" to node \"%s\"\n",portname,parentXMLNodeID);
        }
        else
        {
            //portname exists already so we just need to update any info associated with it
        }
        //printf("bunny exiting port node\n");
/*    PLOG(PL_ERROR,"nodeid=%s, depth=%d, type=%d, name=%s, isEmpty=%d, hasValue=%d",
        parentXMLNodeID,
        xmlTextReaderDepth(readerPtr),
        xmlTextReaderNodeType(readerPtr),
        name,
        xmlTextReaderIsEmptyElement(readerPtr),
        xmlTextReaderHasValue(readerPtr));
        //printf("is a port\n");*/
    }
    else if(!strcmp("edge",(const char*)name))
    {
        //printf("bunny processing edge node\n");
        //check to see if this edge is a port type first
        xmlChar *targetPortName, *sourcePortName;
        targetPortName = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"targetport");
        sourcePortName = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"sourceport");
        if((NULL != targetPortName) && (NULL != sourcePortName))
        {
            //find the target port interface
            NetGraph::Interface* targetPortInterface;
            NetGraph::Interface* sourcePortInterface;
            targetPortInterface = graph.FindInterfaceByString((const char*)targetPortName);
            sourcePortInterface = graph.FindInterfaceByString((const char*)sourcePortName);
            if((NULL == sourcePortInterface) || (NULL == targetPortInterface))
            {
                PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode: Error finding the source interface \"%s\" or target interface \"%s\" to create an edge!\n",sourcePortName,targetPortName);
                return false;
            }
            NetGraph::Cost* mycost = CreateCost(1.0);
            if(!Connect(*sourcePortInterface,*targetPortInterface,*mycost,isDuplex))
            {
                PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode: Error connecting the source interface \"%s\" or target interface \"%s\" to create an edge!\n",sourcePortName,targetPortName);
                return false;
            }
            //printf(" connected interface ports %s to %s\n",sourcePortName,targetPortName);
        }
        else //isn't a port type just a normal source target type
        {
            xmlChar *targetName, *sourceName;
            targetName = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"target");
            sourceName = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"source");
            //find the target interface
            NetGraph::Interface* targetInterface;
            NetGraph::Interface* sourceInterface;
            sourceInterface = graph.FindInterfaceByString((const char*)sourceName);
            targetInterface = graph.FindInterfaceByString((const char*)targetName);
            if((NULL == sourceInterface) || (NULL == targetInterface))
            {
                PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode: Error finding the source interface \"%s\" or target interface \"%s\" to create an edge!\n",sourceName,targetName);
                return false;
            }
            NetGraph::Cost* mycost = CreateCost(1.0);
            if(!Connect(*sourceInterface,*targetInterface,*mycost,isDuplex))
            {
                PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode: Error connecting the source interface \"%s\" or target interface \"%s\" to create an edge!\n",sourceName,targetName);
                return false;
            }
            //printf(" connected interface %s to %s\n",sourceName, targetName);
        }
        //printf("bunny exiting edge node\n");
    }
    else if(!strcmp("key",(const char*)name))
    {
        xmlChar* oldkey = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"id");
        xmlChar* type = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"attr.type");
        xmlChar* name = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"attr.name");
        xmlChar* domain = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"for");

        xmlChar* defaultvalue = NULL;
        xmlNode* myXMLNode = xmlTextReaderExpand(readerPtr);
        xmlNode* childXMLNode = myXMLNode->children;
        while(NULL != childXMLNode)
        {
            if(!strcmp((char*)childXMLNode->name,"default"))
            {
                if(NULL == childXMLNode->children)
                {
                    PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode(): %s has an empty default value!\n",name);
                    return false;
                }
                defaultvalue = childXMLNode->children->content;
            }
            childXMLNode = childXMLNode->next;
        }
        //printf("reading in attribute name=%s key=%s defaultvalue=%s\n",name,oldkey,defaultvalue);
        if(!AddAttributeKey((const char*)name,(const char*)type,(const char*)domain,(const char*)oldkey,(const char*)defaultvalue))
        {
            PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode(): Error adding attribute (name=%s,oldkey=%s)\n",name,oldkey);
            return false;
        }
    }
    else if(!strcmp("data",(const char*)name))
    {
        char newlookup[250];//this really should be dynamic and checks on size should take place TBD
        xmlChar* oldIndex;
        const char* newIndex;
        xmlChar* newValue;
        xmlNode* myxmlnode = xmlTextReaderExpand(readerPtr);

        if(!strcmp((const char*)myxmlnode->parent->name,"node"))
        {
            sprintf(newlookup,"node:%s",parentXMLNodeID);
        } else if(!strcmp((const char*)myxmlnode->parent->name,"port")) {
            xmlAttr* tempattribute = myxmlnode->parent->properties;
            const xmlChar *portName = NULL;
            while(NULL != tempattribute) 
            {
                if(!strcmp((const char*)tempattribute->name,"name")) 
                    portName = tempattribute->children->content;
                tempattribute= tempattribute->next;
            }
            sprintf(newlookup,"node:%s:port:%s",parentXMLNodeID,portName);
            
        } else if(!strcmp((const char*)myxmlnode->parent->name,"edge")) {
            xmlAttr* tempattribute = myxmlnode->parent->properties;
            const xmlChar *targetPortName(NULL), *sourcePortName(NULL), *targetName(NULL), *sourceName(NULL);
            while(NULL != tempattribute)
            {
                if(!strcmp((const char*)tempattribute->name,"sourceport")) 
                    sourcePortName = tempattribute->children->content;
                if(!strcmp((const char*)tempattribute->name,"targetport")) 
                    targetPortName = tempattribute->children->content;
                if(!strcmp((const char*)tempattribute->name,"source")) 
                    sourceName = tempattribute->children->content;
                if(!strcmp((const char*)tempattribute->name,"target")) 
                    targetName = tempattribute->children->content;
                tempattribute= tempattribute->next;
            }
            if((NULL == targetName) || (NULL == sourceName))
            {
                PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode(): Error finding target/source for link data\n");
                return false;
            }
            sprintf(newlookup,"edge:source:%s:%s:dest:%s:%s",sourceName,sourcePortName,targetName,targetPortName);
        } else {
            PLOG(PL_WARN,"ManetGraphMLParser::ReadXMLNode(): Ignoring data for unknown node type\n");
        }
        oldIndex = xmlTextReaderGetAttribute(readerPtr,(xmlChar*)"key");
        AttributeKey* tempakey = FindAttributeKeyByOldIndex((const char*)oldIndex);
        if(NULL == tempakey)
        {
            PLOG(PL_ERROR,"ManetGraphMLParser::ReadXMLNode(): found data with key type %s but it wasn't listed at one of the keys\n",oldIndex);
            return false;
        } else {
            newIndex = tempakey->GetIndex();
            //printf("%s\n",newIndex);
        }
        if(!xmlTextReaderRead(readerPtr))
            return false;
        newValue = xmlTextReaderValue(readerPtr);
        //printf("bunny found data for \"%s\" with key %s with value %s\n",newlookup,oldIndex,newValue);
        
        Attribute* newAttribute = new Attribute();
        newAttribute->Init(newlookup,(const char*)newIndex,(const char*)newValue);
        //newAttribute->Init(newlookup,newIndex,(const char*)newValue);
        attributelist.Insert(*newAttribute);
    }
    else
    {
        //ignorning xml node
        //printf("bunny ignoring xml node %s\n",name);
    }
    return true;

}  // end ManetGraphMLParser::ReadXMLNode()


bool ManetGraphMLParser::Read(const char* path, NetGraph& graph)
{
    // Iteratively read the file's XML tree and build up "graph"
    xmlTextReader* readerPtr = xmlReaderForFile(path, NULL, 0);
    if (NULL == readerPtr)
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::Read() xmlReaderForFile(%s) error: %s\n",path, GetErrorString());
        return false;
    }
    
    bool isDuplex = true;
    char parentXMLGraphID[MAXXMLIDLENGTH+1], parentXMLNodeID[MAXXMLIDLENGTH+1];
    memset(parentXMLGraphID, 0, MAXXMLIDLENGTH+1);
    memset(parentXMLNodeID, 0, MAXXMLIDLENGTH+1);       
    int result = xmlTextReaderRead(readerPtr);
    while (1 == result)
    {
        if (!ReadXMLNode(readerPtr, graph, parentXMLGraphID, parentXMLNodeID, isDuplex))
        {
            PLOG(PL_ERROR,"ManetGraphMLParser::Read() error: invalid XML node!\n");
            break;
        }
        result = xmlTextReaderRead(readerPtr);
    } 
    xmlFreeTextReader(readerPtr);
    if (0 != result)
        PLOG(PL_ERROR,"ManetGraphMLParser::Read() error: invalid XML file %s\n", path);
    xmlCleanupParser();
    return (0 == result);
}  // end ManetGraphMLParser::Read()


bool ManetGraphMLParser::Write(NetGraph& graph, const char* path)
{
    PLOG(PL_INFO,"gettting into the function!\n");
/* Create a new XmlWriter for DOM, with no compression. */
    int returnvalue;
    xmlTextWriter* writerPtr;
    xmlDoc* docPtr;

    writerPtr = xmlNewTextWriterDoc(&docPtr, 0);
    if (writerPtr == NULL) {
        PLOG(PL_ERROR,"ManetGraphMLParser::Write::testXmlwriterPtrDoc: Error creating the xml writer\n");
        return false;
    }

    /* Start the docPtrument with the xml default for the version,
     * encoding UTF-8 and the default for the standalone
     * declarao*/
    returnvalue = xmlTextWriterStartDocument(writerPtr, NULL, MY_GRAPHML_ENCODING, NULL);
    if (returnvalue < 0) {
        PLOG(PL_ERROR,"ManetGraphMLParser::Write::testXmlwriterPtrDoc: Error at xmlTextWriterStartDocument\n");
        return false;;
    }

    /* Start an element named "graphml". Since thist is the first
     * element, this will be the root element of the docPtrument. */
    returnvalue = xmlTextWriterStartElement(writerPtr, BAD_CAST "graphml");
    if (returnvalue < 0) {
        PLOG(PL_ERROR,"ManetGraphMLParser::Write::testXmlwriterPtrDoc: Error at xmlTextWriterStartElement\n");
        return false;
    }
    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "xmlns", BAD_CAST "http://graphml.graphdrawing.org/xmlns");
    if (returnvalue < 0) {
        PLOG(PL_ERROR,"ManetGraphMLParser::Write::testXmlWriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return false;
    } 
    if(!UpdateKeys(graph))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::Write::testXmlWriterDoc: Error updating key elements in the header\n");
        return false;
    }
    if(!WriteLocalKeys(writerPtr))
    {
        PLOG(PL_ERROR,"ManetGraphMLParser::Write::testXmlWriterDoc: Error at writing key elements in header\n");
        return false;
    }
    
    /* We are done with the header so now we go through the actual graph and add each node and edge */
    /* We are adding each node */
    returnvalue = xmlTextWriterStartElement(writerPtr, BAD_CAST "graph");
    if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error starting XML graph element\n"); return false;}
    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "id",BAD_CAST "GraphIDNumber");
    if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error setting XML graph attribute id\n"); return false;}
    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "edgedefault",BAD_CAST "directed");
    if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error setting graph attribute directed\n"); return false;}

    NetGraph::InterfaceIterator it(graph);
    NetGraph::Interface* iface;
    //adding the nodes to the xml
    while (NULL != (iface = it.GetNextInterface()))
    {
        //check to see if this is a default "node" interface
        //if(!iface->IsPort())
        if(iface == iface->GetNode().GetDefaultInterface())
        {
            //Node& node = static_cast<Node&>(iface->GetNode());
            returnvalue = xmlTextWriterStartElement(writerPtr, BAD_CAST "node");
            if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error adding XML node\n"); return false;}
            if(iface->GetAddress().IsValid())
            {
                returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "id", BAD_CAST iface->GetAddress().GetHostString());
                //printf("bunny writing node %s\n",iface->GetAddress().GetHostString());
            } else {
                returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "id", BAD_CAST iface->GetName());
                //printf("bunny writing node %s\n",iface->GetName());
            }
            if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error adding setting node id\n"); return false;}
            
            //update the node attributes using the virtual function
            if(!UpdateNodeAttributes(iface->GetNode()))
            {
                PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error updating the node attributes\n");
                return false;
            } 
            //call the local function write the attributes out
            if(!WriteLocalNodeAttributes(writerPtr,iface->GetNode()))
            {
                PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error writing local node attributes\n");
                return false;
            }
            //call the virtual function to allow the node to fill in any attributes if there
/*            if(!WriteNodeAttributes(writerPtr,iface->GetNode())) //this shouldn't really have to be called if we add/update the attributes correctly TBD
            {
                PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error writing node attributes\n");
                return false;
            }*/

            //we need to iterate through the any "port" interfaces which we may have
            NetGraph::Node *theNode = &iface->GetNode();
            NetGraph::Node::InterfaceIterator nodeIt(*theNode);
            NetGraph::Interface* portIface;
            while (NULL != (portIface = static_cast<NetGraph::Interface*>(nodeIt.GetNextInterface())))
            {
                //we only want ports
                if(!(portIface == portIface->GetNode().GetDefaultInterface()))
//                if(portIface->IsPort())
                {
                    returnvalue = xmlTextWriterStartElement(writerPtr, BAD_CAST "port");
                    if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error adding XML node\n"); return false;}
                    if(portIface->GetAddress().IsValid())
                    {
                        returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "name", BAD_CAST portIface->GetAddress().GetHostString());
                        //printf("bunny writing interface %s\n",iface->GetAddress().GetHostString());
                    } else {
                        returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "name", BAD_CAST portIface->GetName());
                        //printf("bunny writing node %s\n",iface->GetName());
                    } 
                    if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error adding setting node id\n"); return false;}
                    //update attributes to the port/interface using the virutal function
                    if(!UpdateInterfaceAttributes(*portIface))
                    {
                        PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error updating the interface attributes\n");
                        return false;
                    } 
                    //write the attributes to the port/interface
                    if(!WriteLocalInterfaceAttributes(writerPtr,*portIface))
                    {
                        PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error writing local interface attributes\n");
                        return false;
                    }
                    //write any attributes to the port/interface using the virutal function
/*                    if(!WriteInterfaceAttributes(writerPtr,*portIface))//this needn't be called if we add/update the attributes in the local list TBD
                    {
                        PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error writing interface attributes\n");
                        return false;
                    }*/
                    returnvalue = xmlTextWriterEndElement(writerPtr);
                    if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error ending node element\n"); return false;}
                }
            }
            //close up the node node element
            returnvalue = xmlTextWriterEndElement(writerPtr);
            if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error ending node element\n"); return false;}
        }
    }
    //adding the directional links to the xml
    it.Reset();
    while (NULL != (iface = it.GetNextInterface()))
    {
        //if(!iface->IsPort())//not a port node so we can just print it normally
        if(iface == iface->GetNode().GetDefaultInterface())
        {
            NetGraph::AdjacencyIterator iteratorN1(*iface);
            NetGraph::Interface* nbrIface;
            while (NULL != (nbrIface = iteratorN1.GetNextAdjacency()))
            {
                returnvalue = xmlTextWriterStartElement(writerPtr, BAD_CAST "edge");
                if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write Error adding edge\n"); return false;}
                if(iface->GetAddress().IsValid()) {
                    //printf("bunny writing connection %s ->",iface->GetAddress().GetHostString());
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "source", BAD_CAST iface->GetAddress().GetHostString());
                } else { 
                    //printf("bunny writing connection %s ->",iface->GetName());
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "source", BAD_CAST iface->GetName());
                }
                if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write Error adding setting source attribute\n"); return false;}
                if(nbrIface->GetAddress().IsValid()) {
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "target", BAD_CAST nbrIface->GetAddress().GetHostString());
                    ////printf("%s\n",nbrIface->GetAddress().GetHostString());
                } else {
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "target", BAD_CAST nbrIface->GetName());
                    ////printf("%s\n",nbrIface->GetName());
                }
                if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write Error adding setting source attribute\n"); return false;}
                NetGraph::Link* link = iface->GetLinkTo(*nbrIface);
                if(NULL == link)
                {
                    PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error getting link which should always exist\n");
                    return false;
                }
                //update the link attributes using the virtual function
                if(!UpdateLinkAttributes(*link))
                {
                    PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error updating the link attributes\n");
                    return false;
                } 
                //actually write the attributes out
                if(!WriteLocalLinkAttributes(writerPtr,*link))
                {
                    PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error writing link attributes\n");
                    return false;
                }
                returnvalue = xmlTextWriterEndElement(writerPtr);
                if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write Error ending node element\n"); return false;}
            } 
        }
        else //it is a port interface so we need to find the "node" interface
        {
            //lets first find the "src" node interface
            NetGraph::Interface* nodeIface = iface->GetNode().GetDefaultInterface();
            if(!(nodeIface == nodeIface->GetNode().GetDefaultInterface()))
//            if(nodeIface->IsPort())
            {
                PLOG(PL_ERROR,"ManetGraphMLParser::Write Error the default interface was not the \"node\" interface for the src.  You should iterate over them all and find the right one\n");
                return false;
            }
            
            NetGraph::AdjacencyIterator iteratorN1(*iface);
            NetGraph::Interface* nbrIface, *nbrNodeIface;
            while (NULL != (nbrIface = iteratorN1.GetNextAdjacency()))
            {
                //lets find the "nbr" node interface
                nbrNodeIface = static_cast<NetGraph::Interface*>(nbrIface->GetNode().GetDefaultInterface());
                //if(nbrNodeIface->IsPort())
                if(!(nbrNodeIface == nbrNodeIface->GetNode().GetDefaultInterface()))
                {
                    PLOG(PL_ERROR,"ManetGraphMLParser::Write Error the default interface was not the \"node\" interface for the nbr.  You should iterate over them all and find the right one\n");
                    return false;
                }
                returnvalue = xmlTextWriterStartElement(writerPtr, BAD_CAST "edge");
                if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write Error adding edge\n"); return false;}

                if(nodeIface->GetAddress().IsValid()) {
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "source", BAD_CAST nodeIface->GetAddress().GetHostString());
                } else { 
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "source", BAD_CAST nodeIface->GetName());
                }
                if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write Error adding setting source attribute\n"); return false;}
                if(nbrNodeIface->GetAddress().IsValid()) {
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "target", BAD_CAST nbrNodeIface->GetAddress().GetHostString());
                } else {
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "target", BAD_CAST nbrNodeIface->GetName());
                }
                if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write Error adding setting source attribute\n"); return false;}
                if(iface->GetAddress().IsValid()) {
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "sourceport", BAD_CAST iface->GetAddress().GetHostString());
                } else { 
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "sourceport", BAD_CAST iface->GetName());
                }
                if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write Error adding setting source attribute\n"); return false;}
                if(nbrIface->GetAddress().IsValid()) {
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "targetport", BAD_CAST nbrIface->GetAddress().GetHostString());
                } else {
                    returnvalue = xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "targetport", BAD_CAST nbrIface->GetName());
                }
                if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write Error adding setting source attribute\n"); return false;}

                NetGraph::Link* link = iface->GetLinkTo(*nbrIface);
                if(NULL == link)
                {
                    PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error getting link which should always exist\n");
                    return false;
                }
                //update the link attributes using the virtual function
                if(!UpdateLinkAttributes(*link))
                {
                    PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error updating the link attributes\n");
                    return false;
                }
                //actually write the attributes out 
                if(!WriteLocalLinkAttributes(writerPtr,*link))
                {
                    PLOG(PL_ERROR,"ManetGraphMLParser::Write: Error writing link attributes\n");
                    return false;
                }
                returnvalue = xmlTextWriterEndElement(writerPtr);
                if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write Error ending node element\n"); return false;}
            } 
        }
    }
 
    returnvalue = xmlTextWriterEndDocument(writerPtr);
    if (returnvalue < 0) { PLOG(PL_ERROR,"ManetGraphMLParser::Write:testXmlwriterPtrDoc: Error at xmlTextWriterEndDocument\n"); return false;}

    xmlFreeTextWriter(writerPtr);

    xmlSaveFormatFileEnc(path, docPtr, MY_GRAPHML_ENCODING,1);

    xmlFreeDoc(docPtr);
 
    return true;
}  // end ManetGraphMLParser::Write()
bool
ManetGraphMLParser::WriteLocalKeys(xmlTextWriter* writerPtr)
{
    int rv = 0;
    IndexKeylist::Iterator it(indexkeylist);
    AttributeKey* key(NULL);
    while(NULL != (key = it.GetNextItem()))
    {
        rv += xmlTextWriterStartElement(writerPtr, BAD_CAST "key");
        rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "id",BAD_CAST key->GetIndex());
        rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "attr.name",BAD_CAST key->GetName());
        switch (key->GetType())
        {
          case AttributeKey::Types::BOOL:
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "attr.type",BAD_CAST "boolean");
            break;
          case AttributeKey::Types::INT:
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "attr.type",BAD_CAST "int");
            break;
          case AttributeKey::Types::LONG:
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "attr.type",BAD_CAST "long");
            break;
          case AttributeKey::Types::FLOAT:
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "attr.type",BAD_CAST "float");
            break;
          case AttributeKey::Types::DOUBLE:
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "attr.type",BAD_CAST "double");
            break;
          case AttributeKey::Types::STRING:
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "attr.type",BAD_CAST "string");
            break;
          default:
            PLOG(PL_ERROR,"ManetGraphMLParser::WriteLocalKeys: type is invalid!\n");
            return false;
        }
        switch (key->GetDomain())
        {
          case AttributeKey::Domains::GRAPH:
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "for",BAD_CAST "graph");
            break;
          case AttributeKey::Domains::NODE:
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "for",BAD_CAST "node");
            break;
          case AttributeKey::Domains::EDGE:
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "for",BAD_CAST "edge");
            break;
          case AttributeKey::Domains::ALL:
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "for",BAD_CAST "all");
            break;
          default:
            break;
        }
        if (NULL != key->GetDefault())
        {
            rv += xmlTextWriterWriteElement(writerPtr,BAD_CAST "default", BAD_CAST key->GetDefault());
        }
        rv += xmlTextWriterEndElement(writerPtr);
    }
    if(rv < 0)
        return false;
    return true;
}
bool 
ManetGraphMLParser::WriteLocalNodeAttributes(xmlTextWriter* writerPtr,NetGraph::Node& theNode)
{
    bool rv = true;
    char key[255];//this should be dynamic or checks added TBD
    sprintf(key,"node:%s",GetString(theNode));
    AttributeList::Iterator it(attributelist,false,key,strlen(key)*8);
    Attribute* attr(NULL);
    //iterate over items which have the matching keys
    attr = it.GetNextItem();
    while(NULL != attr)
    {
        if(strcmp(attr->GetLookup(),key))
        {
            attr = NULL;
        } else {
            rv += xmlTextWriterStartElement(writerPtr, BAD_CAST "data");
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "key",BAD_CAST attr->GetIndex());
            rv += xmlTextWriterWriteString(writerPtr, BAD_CAST attr->GetValue());
            rv += xmlTextWriterEndElement(writerPtr);
            attr = it.GetNextItem();
        }
    }
    return rv;
}
bool 
ManetGraphMLParser::WriteLocalInterfaceAttributes(xmlTextWriter* writerPtr,NetGraph::Interface& theInterface)
{
    bool rv = true;
    char key[255];//this should be dynamic or checks added TBD
    sprintf(key,"node:%s",GetString(theInterface.GetNode()));
    sprintf(key,"%s:port:%s",key,GetString(theInterface));
    AttributeList::Iterator it(attributelist,false,key,strlen(key)*8);
    Attribute* attr(NULL);
    attr = it.GetNextItem();
    while(NULL != attr)
    {
        if(strcmp(attr->GetLookup(),key))
        {
            attr = NULL;
        } else {
            rv += xmlTextWriterStartElement(writerPtr, BAD_CAST "data");
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "key",BAD_CAST attr->GetIndex());
            rv += xmlTextWriterWriteString(writerPtr, BAD_CAST attr->GetValue());
            rv += xmlTextWriterEndElement(writerPtr);
            attr = it.GetNextItem();
        }
    }
    return rv;
}
bool 
ManetGraphMLParser::WriteLocalLinkAttributes(xmlTextWriter* writerPtr,NetGraph::Link& theLink)
{
    bool rv = true;
    char key[255];//this should be dynamic or checkes added TBD
    sprintf(key,"edge:source:%s",GetString(theLink.GetSrc()->GetNode()));
    if(!strcmp(GetString(*theLink.GetSrc()),GetString(theLink.GetSrc()->GetNode())))
    {
        char* temp = NULL;
        sprintf(key,"%s:%s",key,temp);
    } else {
        sprintf(key,"%s:%s",key,GetString(*theLink.GetSrc()));
    }
    sprintf(key,"%s:dest:%s",key,GetString(theLink.GetDst()->GetNode()));
    if(!strcmp(GetString(*theLink.GetDst()),GetString(theLink.GetDst()->GetNode())))
    {
        char* temp = NULL;
        sprintf(key,"%s:%s",key,temp);
    } else {
        sprintf(key,"%s:%s",key,GetString(*theLink.GetDst()));
    }
    //printf("%s\n",key);
    
    AttributeList::Iterator it(attributelist,false,key,strlen(key)*8);
    Attribute* attr(NULL);
    attr = it.GetNextItem();
    while(NULL != attr)
    {
        if(strcmp(attr->GetLookup(),key))
        {
            attr = NULL;
        } else {
            rv += xmlTextWriterStartElement(writerPtr, BAD_CAST "data");
            rv += xmlTextWriterWriteAttribute(writerPtr, BAD_CAST "key",BAD_CAST attr->GetIndex());
            rv += xmlTextWriterWriteString(writerPtr, BAD_CAST attr->GetValue());
            rv += xmlTextWriterEndElement(writerPtr);
            attr = it.GetNextItem();
        }
    }
    return rv;
}
