#ifndef _HTTP_HEADER_H
#define _HTTP_HEADER_H

#include <map>
#include <string>

namespace EventPie{
    namespace Http{
    
        class Header{
        public:
            Header();
            virtual ~Header();
            
            void setField(const std::string &name, const std::string &value);
            const std::string &getField(const std::string &name) const;
            
            void load(const std::string &header);
            std::string dump() const;
        
        protected:
            std::map<std::string,std::string> fields;
        };
    };
};

#endif //_HTTP_HEADER_H
