//==========================================================
// Utils source
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2026
//==========================================================

#include "utils.hpp"

#include <openssl/evp.h>

#include <array>
#include <iomanip>
#include <sstream>
#include <string>

namespace othello
{
/// Calculate SHA256 hash for the given string.
std::string calculate_sha256(const std::string& text)
{
    // Code adapted from:
    // https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
    std::stringstream stream;
    if (EVP_MD_CTX* context = EVP_MD_CTX_new(); context != nullptr) {
        if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 0) {
            if (EVP_DigestUpdate(context, text.c_str(), text.length()) != 0) {
                std::array<unsigned char, EVP_MAX_MD_SIZE> hash {};
                unsigned int hash_length = 0;
                if (EVP_DigestFinal_ex(context, hash.data(), &hash_length) != 0) {
                    for (unsigned int i = 0; i < hash_length; ++i) {
                        stream << std::hex << std::setw(2) << std::setfill('0')
                               << static_cast<int>(hash[i]);
                    }
                }
            }
        }
        EVP_MD_CTX_free(context);
    }

    return stream.str();
}
}  // namespace othello
