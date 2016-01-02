#include <cryptlib.h>
#include <filters.h>
#include <hex.h>
#include <sha.h>

#include "ChecksumMgr.hpp"

using namespace std;

namespace app {
    string gen_string_checksum(const string& src)
    {
        CryptoPP::SHA1 sha1;
        string checksum = "";
        CryptoPP::StringSource(src, true,
            new CryptoPP::HashFilter(sha1, new CryptoPP::HexEncoder(new CryptoPP::StringSink(checksum))));

        return checksum;
    }
}
