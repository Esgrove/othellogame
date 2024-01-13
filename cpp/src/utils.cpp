#include "utils.hpp"

#include <openssl/evp.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace othello
{
std::string Move::to_log_entry()
{
    return fmt::format("{}:{},{}", board_char(this->disk, false), this->square, this->value);
}

fmt::terminal_color disk_color(const Disk& disk)
{
    using enum fmt::terminal_color;
    if (disk == Disk::empty) {
        return white;
    }
    return disk == Disk::white ? cyan : magenta;
}

std::string board_char(const Disk& disk, const bool color)
{
    if (disk == Disk::empty) {
        return "_";
    }
    std::string identifier = disk == Disk::white ? "W" : "B";
    return color ? get_color(identifier, disk_color(disk)) : identifier;
}

std::string disk_string(const Disk& disk)
{
    const auto color = disk_color(disk);
    switch (disk) {
        case Disk::empty:
            return get_color("EMPTY", color);
        case Disk::black:
            return get_color("BLACK", color);
        case Disk::white:
            return get_color("WHITE", color);
        default:
            return "UNKNOWN";
    }
}

Disk opponent(const Disk& disk)
{
    switch (disk) {
        case Disk::white:
            return Disk::black;
        case Disk::black:
            return Disk::white;
        case Disk::empty:
            [[fallthrough]];
        default:
            return Disk::empty;
    }
}

std::string calculate_sha256(const std::string& text)
{
    // Code adapted from:
    // https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
    std::stringstream stream;
    EVP_MD_CTX* context = EVP_MD_CTX_new();

    if (context != nullptr) {
        if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 0) {
            if (EVP_DigestUpdate(context, text.c_str(), text.length()) != 0) {
                unsigned char hash[EVP_MAX_MD_SIZE];
                unsigned int hash_length = 0;
                if (EVP_DigestFinal_ex(context, hash, &hash_length) != 0) {
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
