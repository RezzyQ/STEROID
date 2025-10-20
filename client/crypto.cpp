#include "crypto.h"

string auth(string salt, string pass){

CPP::Weak1::MD5 md5;
string hash;
CPP::StringSource(
    string(salt).append(pass), 
    true,
        new CPP::HashFilter(
            md5,
            new CPP::HexEncoder(
                new CPP::StringSink(hash)))
);

return hash;
}
