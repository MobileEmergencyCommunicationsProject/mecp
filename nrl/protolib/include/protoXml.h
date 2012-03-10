#ifndef _PROTO_XML
#define _PROTO_XML

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlreader.h>
          
namespace ProtoXml
{
    // Here are some helper functions that can be applied to various libxml2 types
    
    bool GetPropAsDouble(xmlNodePtr nodePtr, const char* attrName, double& value);
    bool GetPropAsInt(xmlNodePtr nodePtr, const char* attrName, int& value);
    
    // This is a base class we use for common path filter stuff
    class IterFilterBase
    {
        public:
            enum {FILTER_PATH_MAX = 1023};
        
        protected:
            IterFilterBase(const char* filterPath);
            void SetFilter(const char* filterPath);
        
            bool UpdateCurrentPath(int depth, const char* nodeName);
            bool IsMatch();
            void Reset();
        
        private:
            char             path_filter[FILTER_PATH_MAX+1];     
            char             path_current[FILTER_PATH_MAX+1];    
            size_t           path_current_len;                   
            int              path_depth;
        
    };  // end class ProtoXml::IterFilterBase
    
    
    // The ProtoXML::IterParseFilter() use the libxml2 xmlTextReader API to
    // provide incremental reading of XML document, providing xmlNodePtr
    // values that match a hierarchical (slash-delimited) "filter" specification 
    // of element tags. The "GetNext()" method here gives xmlNodePtr that correspond to "end" 
    // event (i.e. the end of the element/node matching the filter spec has been reached.
    // In the future, we may expand this provide a "start" event (perhaps via a "PeekNext()"
    // method?) iteration option
    class IterParser : public IterFilterBase
    {
        public:
            IterParser(const char* filterPath = NULL);
            virtual ~IterParser();
            
            bool Open(const char* fileName, const char* filterPath = NULL);
            void Close();
            
            // note returned pointer is only valid until next "GetNext()" call
            xmlNodePtr GetNext();
            
        private:
            xmlTextReaderPtr reader_ptr;   
            xmlNodePtr       prev_node;      

    };  // end class ProtoXml::IterParser
    
    
    // This iterates over an xmlNodePtr sub-tree looking for matches against
    // the "filterPath" (only fully-qualifed paths are supported, the path
    // must begin with a '/' and name elements _below_ root node.  No
    // filter returns all direct children
    class IterFinder  : public IterFilterBase
    {
        public:
            IterFinder(xmlNodePtr rootElem, const char* filterPath = NULL);
            ~IterFinder();
            
            // reset and optionally change filterPath
            void Reset(const char* filterPath = NULL);
            
            xmlNodePtr GetNext();
                
        private:
            xmlNodePtr root_elem;
            xmlNodePtr prev_elem;
            int        iter_depth;
            
    };  // end class ProtoXml::IterFinder
    
};  // end class ProtoXml

#endif // _PROTO_XML
