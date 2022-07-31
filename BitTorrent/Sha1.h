/*
    sha1.hpp - source code of
    ============
    SHA-1 in C++
    ============
    100% Public Domain.
    Original C Code
        -- Steve Reid <steve@edmweb.com>
    Small changes to fit into bglibs
        -- Bruce Guenter <bruce@untroubled.org>
    Translation to simpler C++ Code
        -- Volker Diels-Grabsch <v@njh.eu>
    Safety fixes
        -- Eugene Hopkinson <slowriot at voxelstorm dot com>
    Header-only library
        -- Zlatko Michailov <zlatko@michailov.org>
*/

#pragma once
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include "Defs.h"

static const size_t BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
static const size_t BLOCK_BYTES = BLOCK_INTS * 4;

class Sha1 {
public:
    Sha1();
    void update(const std::string& s);
    void update(std::istream& is);
    std::string final();
    static std::string from_file(const std::string& filename);

private:
    uint32_t digest[5];
    std::string buffer;
    uint64_t transforms;
};



