#include "ssc.hpp"
#include <iostream>
#include <fstream>

namespace SSC{
    uint16_t get_highest_bit_index(uint64_t mask){      //counted from 1
        uint16_t i = 0;
        for(; mask; ++i) mask >>= 1;
        return i;
    }

    uint64_t lfsr_next(uint64_t seed, uint64_t mask){
        uint16_t high_bit_idx = get_highest_bit_index(mask); 
        uint64_t feedback_bit = 0;
        uint64_t shift;

        //  1           = 00000001b
        //  1 << 5      = 00100000b
        // (1 << 5) - 1 = 00011111b
        seed &= (1 << high_bit_idx/* + 1*/) - 1;                //make sure seed is the same bits-length as mask
        
        for(uint16_t i = 0; i </*<=*/ high_bit_idx; ++i){       //compute feedback_bit
            shift = (1 << i);

            if(shift & mask){
                feedback_bit ^= (seed & shift) >> i;
            }
        }

        return (feedback_bit << (high_bit_idx - 1)) | (seed >> 1);
    }

    LFSR_Functor::LFSR_Functor(uint64_t seed, uint64_t mask) : seed{seed}, mask{mask}
    {};

    uint64_t LFSR_Functor::operator()(){
        return seed = lfsr_next(seed, mask);
    }

    void run_stream_cipher(std::ifstream & ifs, std::ofstream & ofs, LFSR_Functor & lfsr_func){
        uint64_t    mask        {lfsr_func()}; //{0};   
        uint16_t    mask_len    {get_highest_bit_index(mask)};                // count from 1
        uint16_t    mask_cur    {mask_len};                                   // mask cursor indicating current bit
        uint64_t    temp_mask   {0};
        int16_t     diff        {0};

        for(char ch; ifs.read(&ch, 1); ){
            if(mask_cur == 0){
                mask = lfsr_func();
                mask_cur = mask_len;
            }

            diff = SSC::word_len - mask_cur;
            if(diff > 0){                                                       // current mask is too short we need to merge with new mask
                // get as much as possible of current mask
                temp_mask = (mask & ((1 << mask_cur) - 1)) << diff;
                // get remaining part from new mask and merge with current temp_mask
                mask = lfsr_func();
                mask_cur = mask_len - diff;
                temp_mask |= (mask >> mask_cur)/* & ((1 << diff) - 1)*/;        // & diff is redundant
            }else{                                                              // mask is long enough to XOR word_len
                temp_mask = (mask >> (mask_cur - SSC::word_len)) & ((1 << SSC::word_len) - 1);
            }

            ch ^= temp_mask;
            ofs.write(&ch, 1);
        }
    }
}