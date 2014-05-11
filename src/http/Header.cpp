#include "Header.h"

#include "../ParseUtil.h"

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
            int offset = 0;
            for(int i=0;i<header.size()-1;i++){
                
                /* check CrLf */
                if( EP_STRCMP2( &header[i], '\r','\n' ) ){
                    
                    /* parse field */
                    if( i != offset ){ /* for ignore empty field */
                        int ws1 = 0, ws2 = 0;
                        int delim = -1;
                        
                        for(int j=offset;j<i;j++){
                            if( header[j] == ' ' ){
                                if( delim == -1 ) ws1 ++;
                                else              ws2 ++;
                            }
                            else if( delim > 0 ){
                                setField(
                                    header.substr( offset, delim-offset-ws1 ),
                                    header.substr( offset+ws2+1, i-(delim+ws2+1) ) );
                                break;
                            }
                            else if( header[j] == ':' )
                                delim = j;
                        }
                    }
                    offset = i+2;
                }
                /* check double-CrLf */
                if( i+3 < header.size() &&
                    EP_STRCMP4( &header[i], '\r','\n','\r','\n' ) ){
                    break;
                }
            }
        }
        string Header::dump() const{
            string header;
            
            for( auto &pair : fields ){
                header += pair.first + ":" + pair.second;
                header += "\r\n";
            }
            header += "\r\n";
            
            return header;
        }
    };
};
