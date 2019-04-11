#ifndef SSC_HPP
#define SSC_HPP

#include <iostream>

namespace SSC{  
    const uint16_t word_len {8};    // word contains 8 BITS

    class LFSR_Functor{
    public:
        LFSR_Functor(uint64_t seed, uint64_t mask);

        uint64_t operator()();
    private:
        uint64_t seed;
        uint64_t mask;
    };

    void run_stream_cipher(std::ifstream & ifs, std::ofstream & ofs, LFSR_Functor & lfsr_func);
}

#endif