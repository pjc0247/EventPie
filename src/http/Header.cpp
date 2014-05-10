#include "Header.h"

using namespace std;

namespace EventPie{
    namespace Http{
      
        Header::Header(){
            
        }
        Header::~Header(){
            
        }
        
        void Header::setField(const string &name, const string &value){
            fields[name] = value;
        }
        const string &Header::getField(const string &name) const{
            static string noValue = "";
            auto it = fields.find( name );
            
            if( it == fields.end() )
                return noValue;
            else
                return it->second;
        }
        
        void Header::load(const string &header){
        
        }
        string Header::dump() const{
            string header;
            
            return header;
        }
    };
};
