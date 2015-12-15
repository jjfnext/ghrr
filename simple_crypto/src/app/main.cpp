// Crypto++
#include <cryptlib.h>
#include <files.h>

#include <sha.h>
#include <filters.h>
#include <hex.h>

 // std
 #include <iostream>
 #include <string>
 
 using namespace std;

 string gen_string_checksum(string& src)
 {
     CryptoPP::SHA1 sha1;
     std::string checksum = "";
     CryptoPP::StringSource(src, true,
         new CryptoPP::HashFilter(sha1, new CryptoPP::HexEncoder(new CryptoPP::StringSink(checksum))));

     return checksum;
 }

 void test_simple()
 {
  CryptoPP::SHA1 sha1;
  std::string source = "Hello";  //This will be randomly generated somehow
  std::string hash = "";
  CryptoPP::StringSource(source, true, 
    new CryptoPP::HashFilter(sha1, 
        new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));
  cout << "hello: " << hash << endl;
 }

 void test_cksum()
 {
     string txt = "Hello";
     cout << txt << ": " << gen_string_checksum(txt) << endl;

     txt = "hello";
     cout << txt << ": " << gen_string_checksum(txt) << endl;
 }

 void DumpHash_SingleStep(
     CryptoPP::HashTransformation& hash,
     char const* szModuleName,
     std::string const& strData)
 {
     using namespace std;
     using namespace CryptoPP;
 
     // Cannot use std::string for buffer;
     // its internal storage might not be contiguous
     SecByteBlock sbbDigest(hash.DigestSize());
 
     hash.CalculateDigest(
         sbbDigest.begin(),
         (byte const*) strData.data(),
         strData.size());
 
     cout << szModuleName << " SS: ";
     HexEncoder(new FileSink(cout)).Put(sbbDigest.begin(), sbbDigest.size());
     cout << endl;
 }
 
 void DumpHash_MultiStep(
     CryptoPP::HashTransformation& hash,
     char const* szModuleName,
     std::string const& strDataPart1,
     std::string const& strDataPart2,
     std::string const& strDataPart3)
 {
     using namespace std;
     using namespace CryptoPP;
 
     hash.Update((byte const*) strDataPart1.data(), strDataPart1.size());
     hash.Update((byte const*) strDataPart2.data(), strDataPart2.size());
     hash.Update((byte const*) strDataPart3.data(), strDataPart3.size());
 
     // Cannot use std::string for buffer;
     // its internal storage might not be contiguous
     SecByteBlock sbbDigest(hash.DigestSize());
 
     hash.Final(sbbDigest.begin());
 
     cout << szModuleName << " MS: ";
     HexEncoder(new FileSink(cout)).Put(sbbDigest.begin(), sbbDigest.size());
     cout << endl;
 }
 
 
 
 void DumpHash_HashFilter(
     CryptoPP::HashTransformation& hash,
     char const* szModuleName,
     std::string const& strDataPart1,
     std::string const& strDataPart2,
     std::string const& strDataPart3)
 {
     using namespace std;
     using namespace CryptoPP;
 
     // Here, we are free to use std::string as the destination,
     // because StringSink uses the correct std::string interface to append data
     string strDigest;
     HashFilter hashFilter(hash, new StringSink(strDigest));
     hashFilter.Put((byte const*) strDataPart1.data(), strDataPart1.size());
     hashFilter.Put((byte const*) strDataPart2.data(), strDataPart2.size());
     hashFilter.Put((byte const*) strDataPart3.data(), strDataPart3.size());
     hashFilter.MessageEnd();
 
     cout << szModuleName << " HF: ";
     StringSource(strDigest, true,
         new HexEncoder(
             new FileSink(cout)));
     cout << endl;
 }
 
 
 
 void DumpHash(
     CryptoPP::HashTransformation& hash,
     char const* szModuleName,
     std::string const& strDataPart1,
     std::string const& strDataPart2,
     std::string const& strDataPart3)
 {
     DumpHash_SingleStep(hash, szModuleName, strDataPart1 + strDataPart2 + strDataPart3);
     DumpHash_MultiStep(hash, szModuleName, strDataPart1, strDataPart2, strDataPart3);
     DumpHash_HashFilter(hash, szModuleName, strDataPart1, strDataPart2, strDataPart3);
 }
 
 
 
 
 // Crypto++
 #include "sha.h"        // SHA-1, SHA-256, SHA-384, SHA-512
 #include "ripemd.h"     // RIPEMD160
 #include "md5.h"        // MD5
 #include "crc.h"        // CRC-32
 
 
 int main()
 {
     using namespace std;
     using namespace CryptoPP;
 
     std::string strDataPart1 = "part 1; ";
     std::string strDataPart2 = "part two; ";
     std::string strDataPart3 = "PART THREE.";
 
     try
     {
         DumpHash(SHA(),       "SHA      ", strDataPart1, strDataPart2, strDataPart3);
//         DumpHash(SHA256(),    "SHA256   ", strDataPart1, strDataPart2, strDataPart3);
         DumpHash(RIPEMD160(), "RIPEMD160", strDataPart1, strDataPart2, strDataPart3);
         DumpHash(MD5(),       "MD5      ", strDataPart1, strDataPart2, strDataPart3);
         DumpHash(CRC32(),     "CRC32    ", strDataPart1, strDataPart2, strDataPart3);
     }
     catch (CryptoPP::Exception const& e)
     {
         cout << "CryptoPP::Exception caught: " << endl
              << e.what() << endl;
         return 1;
     }
 
    test_simple();
    test_cksum();
    
     return 0;
 }
