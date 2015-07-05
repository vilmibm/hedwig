#include "keys.hpp"
#include "base64.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <openssl/err.h>

#define CRC24_INIT 0x0B704CE
#define CRC24_POLY 0x1864CFB

static uint32_t calc_crc(std::vector<BYTE> data) {
  uint32_t crc = CRC24_INIT;
  for(BYTE b : data) {
    crc ^= b << 16;
    for (int i = 0; i < 8; i++) {
      crc <<= 1;
      if (crc & 0x1000000)
	crc ^= CRC24_POLY;
    }
  }
  return crc & 0xFFFFFF;
}

std::vector<BYTE> read_armored(std::string armored) {
  auto splitter = armored.find_last_of('=');
  auto data_str = armored.substr(0, splitter);
  auto crc_str = armored.substr(splitter+1);
  auto data_bytes = base64_decode(data_str);
  auto crc_bytes = base64_decode(crc_str);
  if(3 != crc_bytes.size())
    throw std::runtime_error("CRC must be 24 bits");
  uint32_t crc = (crc_bytes[0] << 16) | (crc_bytes[1] << 8) | (crc_bytes[2]);
  if(calc_crc(data_bytes) != crc)
    throw std::runtime_error("CRC does not match!");
  return data_bytes;
}

RSAp read_pubkey(std::string armored) {
  RSAp result = RSAp(RSA_new(), rsa_del);
  auto data = read_armored(armored);
  auto tag = (data[0] >> 2) & 0x0f;
  auto len_type = data[0] & 0x03;
  auto style = (data[0] >> 6) & 1;
  auto check = data[0] >> 7;
  size_t next_byte = 1;

  if (!check) {
    throw std::runtime_error("Invalid PGP packet");
  }
  if (style) {
    throw std::runtime_error("New-style PGP packet lengths not supported");
  }
  if (tag != 6) {
    std::stringstream ss;
    ss << "Got unexpected tag: " << tag;
    throw std::runtime_error(ss.str());
  }
  if (len_type == 3) {
    throw std::runtime_error("Indeterminate packets are not supported");
  }
  auto len_bytes = 1 << len_type;
  size_t length = 0;
  for(size_t i = 0; i < len_bytes; i++) {
    length <<= 8;
    length |= data[next_byte++];
  }

  auto version = data[next_byte++];
  if (version != 4) {
    throw std::runtime_error("Only V4 PGP key material packets are supported");
  }

  next_byte += 4; // skip timestamp; we don't care about it.

  auto algorithm = data[next_byte++];
  if (algorithm != 1) {
    throw std::runtime_error("Only RSA keys are supported");
  }
  uint16_t mpi_len = 0;
  const BYTE* mpi_data = 0;
  void* bn_result;

  for(size_t i = 0; i < 2; i++) {
    mpi_len <<=8;
    mpi_len |= data[next_byte++];
  }
  mpi_len /= 8; // THAT'S RIGHT FOLKS, LENGTH IS IN BITS
  mpi_data = &(data[next_byte]);
  next_byte += mpi_len;
  result->n = BN_new();
  bn_result = BN_bin2bn(mpi_data, mpi_len, result->n);
  if (!bn_result) {
    ERR_print_errors_fp(stdout);
  }

  for(size_t i = 0; i < 2; i++) {
    mpi_len <<=8;
    mpi_len |= data[next_byte++];
  }
  mpi_len /= 8; // THAT'S RIGHT FOLKS, LENGTH IS IN BITS
  mpi_data = &(data[next_byte]);
  next_byte += mpi_len;
  result->e = BN_new();
  bn_result = BN_bin2bn(mpi_data, mpi_len, result->e);
  if (!bn_result) {
    ERR_print_errors_fp(stdout);
  }

  BN_print_fp(stdout, result->e);
  std::cout << std::endl;
  BN_print_fp(stdout, result->n);
  std::cout << std::endl;
  std::fill(data.begin(), data.end(), 0);
  return result;
}

RSAp read_privkey(std::string armored) {
  RSAp result = RSAp(RSA_new(), rsa_del);
  auto data = read_armored(armored);
  return result;
}

int main(int, char**) {
  ERR_load_crypto_strings();
  read_pubkey("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=o05J");
  return 0;
}
