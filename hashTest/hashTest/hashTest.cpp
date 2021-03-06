// TLS.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

//ASE
#include "include\cryp\modes.h"
#include "include\cryp\filters.h"
#include "include\cryp\aes.h"

//RSA
#include "include\cryp\cryptlib.h"
#include "include\cryp\integer.h"
#include "include\cryp\nbtheory.h"
#include "include\cryp\osrng.h"
#include "include\cryp\rsa.h"
#include "include\cryp\sha.h"

//SHA256
#include "include\cryp\sha.h"
#include "include\cryp\filters.h"
#include "include\cryp\base64.h"

//MD5
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "include\cryp\md5.h"
#include "include\cryp\hex.h"

#include <iostream>
#include <stdexcept>
using std::cout;
using std::endl;
using std::runtime_error;

#if CRYPTOPP_MSC_VERSION
# pragma warning(disable: 4505 4355)
#endif

#pragma comment(lib, "cryptlib.lib")

using namespace CryptoPP;
using namespace std;

void _AES() {

	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];
	memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

	string plaintext = "Now is the time for all good men to come to the aide...";
	string ciphertext;
	string decryptedtext;

	cout << "Plain Text(" << plaintext.size() << "bytes)" << endl;
	cout << plaintext;
	cout << endl << endl;

	//namespace CryptoPP 암호화 세팅 부분 
	AES::Encryption aesEncryption(key, AES::DEFAULT_KEYLENGTH);
	CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);   //암호와 알고리즘 선택 
	StreamTransformationFilter   stfEncryptor(cbcEncryption, new StringSink(ciphertext)); //풋할때마다 버퍼에 쓰기위해  
	
	//암호화 
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length() + 1);
	stfEncryptor.MessageEnd();

	//헤스코드 출럭
	cout << "Cipher Text(" << ciphertext.size() << " bytes)" << endl;
	for (int i = 0; i < ciphertext.size(); i++) {

		cout << "0x" << hex << (0xFF & static_cast<byte>(ciphertext[i])) << " ";
	}
	cout << endl << endl;

	//복호화세팅
	AES::Decryption aesDecryption(key, AES::DEFAULT_KEYLENGTH);
	CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);   //암호와 알고리즘 선택 
	StreamTransformationFilter stfDecryptor(cbcDecryption, new StringSink(decryptedtext));//풋할때마다 버퍼에 쓰기위해  

	//복호화 
	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
	stfDecryptor.MessageEnd();

	cout << "Decrypted Text: " << endl;
	cout << decryptedtext;
	cout << endl << endl;

}
void _RSA()
{
	AutoSeededRandomPool prng;
	RSA::PrivateKey privKey;

	privKey.GenerateRandomWithKeySize(prng, 64);
	RSA::PublicKey pubKey(privKey);

	const Integer& n = pubKey.GetModulus();
	const Integer& e = pubKey.GetPublicExponent();
	const Integer& d = privKey.GetPrivateExponent();

	cout << "Pub mod:" << hex << pubKey.GetModulus() << endl;
	cout << "Pub exp: " << hex << e << endl;
	cout << "Priv mod: " << hex << privKey.GetModulus() << endl;
	cout << "Priv exp: " << hex << d << endl;

	const size_t SIG_SIZE = UnsignedMin(SHA256::BLOCKSIZE, n.ByteCount());

	SecByteBlock buff1, buff2, buff3;

	SecByteBlock orig((const byte*)"secret", 6);
	Integer m(orig.data(), orig.size());
	cout << "Message: " << hex << m << endl;

	buff1.resize(SIG_SIZE);
	SHA256 hash1;
	hash1.CalculateTruncatedDigest(buff1, buff1.size(), orig, orig.size());

	Integer hm(buff1.data(), buff1.size());
	cout << "H(m): " << hex << hm << endl;

	Integer r;
	do {
		r.Randomize(prng, Integer::One(), n - Integer::One());
	} while (!RelativelyPrime(r, n));

	Integer b = a_exp_b_mod_c(r, e, n);
	cout << "Random: " << hex << b << endl;


	Integer mm = a_times_b_mod_c(hm, b, n);
	cout << "Blind msg: " << hex << mm << endl;

	Integer ss = privKey.CalculateInverse(prng, mm);
	cout << "Blind sign: " <<  ss << endl;

	Integer c = pubKey.ApplyFunction(ss);
	cout << "Check sign: " << c << endl;
	if (c != mm)
		throw runtime_error("Alice cross-check failed");

	Integer s = a_times_b_mod_c(ss, r.InverseMod(n), n);
	cout << "UnBlind sign: " << s << endl;

	Integer v = pubKey.ApplyFunction(s);
	cout << "Verify: " <<hex << v << endl;

	size_t req = v.MinEncodedSize();
	buff2.resize(req);
	v.Encode(&buff2[0], buff2.size());

	buff3.resize(SIG_SIZE);
	SHA256 hash2;
	hash2.CalculateTruncatedDigest(buff3, buff3.size(), orig, orig.size());

	bool equal = buff2.size() == buff3.size() && VerifyBufsEqual(
		buff2.data(), buff3.data(), buff3.size());

	if (!equal)
		throw runtime_error("Eve verified failed");

	cout << "Verified signature" << endl;

}
void _MD5()
{
	byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];
	std::string message = "foobarbat";

	CryptoPP::Weak::MD5 hash;
	hash.CalculateDigest(digest, (byte*)message.c_str(), message.length());

	CryptoPP::HexEncoder encoder;
	std::string output;

	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();

	cout << output;
}
std::string SHA256HashString(std::string aString) {
	std::string digest;
	CryptoPP::SHA256 hash;

	CryptoPP::StringSource foo(aString, true
		, new CryptoPP::HashFilter(hash
			, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(digest))));

	return digest;
}

int main() {
	cout << SHA256HashString("aa");
	return 0;
}