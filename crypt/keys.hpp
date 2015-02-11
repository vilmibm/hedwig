#include <openssl/rsa.h>
#include <string>
#include <memory>

auto rsa_del = [](RSA* ptr) { RSA_free(ptr); };
typedef std::unique_ptr<RSA, decltype(rsa_del)> RSAp;

RSAp read_key(std::string armored);
