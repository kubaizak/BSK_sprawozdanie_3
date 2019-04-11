#include "ssc.hpp"
#include <iostream>
#include <fstream>
#include <bitset>
#include <cstddef>
#include <vector>
#include <deque>
#include <string>

static uint64_t seed;
static uint64_t mask;
static std::ifstream ifs;
static std::ofstream ofs;

void set_global_variables(int argc, char** argv) {
    if(argc == 1)   throw 0;
    if(argc < 5)    throw -1;

    std::deque<std::string> params_list;
    std::bitset<SSC::word_len * sizeof(uint64_t)> bits;

    for(int i {1}; i < argc; ++i){
        params_list.push_back(std::string(argv[i]));
    }
    
    //SEED
    // seed = std::stoi(params_list.front());                  
    bits = std::bitset<SSC::word_len * sizeof(uint64_t)>{params_list.front()};   //may throw invalid_argument or out_of_range
    ::seed = bits.to_ullong();
    params_list.pop_front();
    
    //MASK
    bits = std::bitset<SSC::word_len * sizeof(uint64_t)>{params_list.front()};   //may throw invalid_argument or out_of_range
    ::mask = bits.to_ullong();
    params_list.pop_front();
    
    //INPUT_FILE_STREAM
    std::string iname = params_list.front();
    ifs = std::ifstream{iname, std::ios_base::binary};
    if(!ifs) throw -2;
    params_list.pop_front();
    
    //OUTPUT_FILE_STREAM
    std::string oname = params_list.front();
    ofs = std::ofstream{oname, std::ios_base::binary};
    if(!ofs) throw -3;
}

int main(int argc, char* argv[]){
    std::string out;

    try{
        set_global_variables(argc, argv);
    }catch(std::invalid_argument e){
        std::cout << "Not a number argument!" << '\n';
        return -1;
    }catch(int e){
        std::string msg;

        switch(e){
            case  0: msg =  "Usage:\n"
                            "\tsscipher <seed> <mask> <input_file_path> <output_file_path>\n"
                            "\t\t- where seed is unsigned integer max 2^64\n"
                            "\t\t- where mask is unsigned integer max 2^64 which is\n"
                            "\t\t\tbitwise representation for polynomial\n";
                break;
            case -1: msg = "Invalid arguments!"; break;
            case -2: msg = "Input file not found!"; break;
            case -3: msg = "Output file not found!"; break;
            default: msg = "WTF?";
        }

        std::cout << msg << '\n';
        return -1;
    }

    SSC::LFSR_Functor lfsr_func {::seed, ::mask};
    SSC::run_stream_cipher(::ifs, ::ofs, lfsr_func);
    
    return 0;
}

/*
int main(void)
{ 
    // uint16_t start = 8ul;
    // uint16_t seed = start;
    // uint16_t mask = 9ull;
    
    // do{
    //     seed = lfsr_next(seed, mask);
    //     std::cout << seed  << std::endl;
    // }while(seed != start);

    return 0;
    // if(mask == 0) throw std::runtime_error("empty mask");
    
    // unsigned int start_state = 0x08u;
    // unsigned int lfsr = start_state;

    // int i = 16;
    // do
    // {
    //     unsigned int lsb = lfsr & 1u;

    //     lfsr >>= 1u;
    //     unsigned int mask = ((1U << 3) | (1U << 0));
    //     lfsr ^= (-lsb & mask);
    //     std::cout << lfsr << std::endl;
    // } while (--i > 0);
    // return (0);
}
*/

/*
#include <stdint.h>
unsigned lfsr1(void)
{
    uint16_t start_state = 0x08u;    
    uint16_t lfsr = start_state;
    uint16_t bit;                    
    unsigned period = 0;

    do
    {   
        bit = ((lfsr >> 0) ^ (lfsr >> 3));
        lfsr = (lfsr >> 1) | ((bit << 3) & 0x0F);
        ++period;
        std::cout << lfsr << std::endl;
    }
    while (lfsr != start_state);

    return period;
}

int main(){
    std::cout << lfsr1() << std::endl;
    return 0;
}
*/

/*#define POLY_MASK_32 0xB4BCD35C
#define POLY_MASK_31 0x7A5BC2E3

typedef unsigned int uint;««

uint lfsr32, lfsr31;

int shift_lfsr(uint *lfsr, uint polynomial_mask){
    int feedback;

    feedback = *lfsr & 1;
    *lfsr >>= 1;
    if (feedback == 1)
        *lfsr ^= polynomial_mask;
        return *lfsr;
}

void init_lfsrs(void){
    lfsr32 = 0xABCDE;
    lfsr31 = 0x23456789;
}

int get_random(void){
    shift_lfsr(&lfsr32, POLY_MASK_32);
    return (shift_lfsr(&lfsr32, POLY_MASK_32) ^ shift_lfsr(&lfsr31, POLY_MASK_31)) & 0xFFFF;
}

int main(void){
    uint num = 0x55;
    uint mask = 0x1A;
    int result = shift_lfsr(&num, mask);

    int random_value;
    
    init_lfsrs();
    random_value = get_random();

    return 0;
}
*/
/*
#define INT_SIZE 4 * sizeof(int)
#define CHAR_SIZE sizeof(char)

using namespace std;

//podaje seed
//podaje mask
//LFSR dla danych

*/

/*
//tekst, klucz
const std::string wiadomosc {"Jakis tam sobie tekst"};
const std::string key       {"password"};
static std::string szyfr    {};
static std::string szyfr1   {};
static std::string szyfr2   {};
std::string decrypted       {};

int main(){
    ifstream ifs{"dupa.txt", ios_base::binary};
    if(!ifs) throw "Cant open file";

    // cout << sizeof(int) << endl;

    // char b;
    // byte* x = static_cast<byte*>((void *)&b);
    const short ch_size = sizeof(char);
    const short ch_bit_size = ch_size * 8;
    
    for(char ch; ifs.read(&ch, ch_size); ){
        byte b = static_cast<byte>(ch);
        bitset<ch_bit_size > bits { static_cast<unsigned int>(ch) };
        // cout << b << endl;
        cout << bits << endl;
    }

    for(int i = 0; i < wiadomosc.size(); i++){
        bitset<8> m = wiadomosc[i];
        bitset<8> k = key[i % key.size()];
        szyfr1 += static_cast<unsigned char>((m^k).to_ulong());
    }

    cout << "MESSAGE: " << wiadomosc << endl;
    cout << "CIPHER1: " << szyfr1 << endl;
    cout << "CIPHER2: " << szyfr2 << endl;

    for(int i = 0; i < szyfr1.size(); ++i){
        // short int liczba {szyfr1[i]};
        // cout << liczba << endl;
        // bitset<8> b {liczba};
        cout << szyfr1[i] << "\t| " << (int)szyfr1[i] << "\t| " << bitset<8>(szyfr1[i]) << endl;
    }

    for(int i = 0; i < szyfr1.size(); ++i){
        unsigned char m = szyfr1[i];
        unsigned char k = key[i % key.size()];
        decrypted += (int)m ^ k;
    }

     cout << "DECRYPTED: " << decrypted << endl;

    // unsigned char t1 = '3';
    // unsigned char t2 = '7';

    // // cout << "PRZERWA" << endl;
    // // cout << (int)t1 << endl;
    // // cout << (int)t2 << endl;
    // // unsigned int result = (int)t1 ^ (int)t2;
    // unsigned int result = t1 ^ t2;
    // cout << "RESULT: " << result << endl;

    // cout << "PRZERWA" << endl;
    // cout << bitset<8>{t1} << endl;
    // cout << bitset<8>{t2} << endl;
    // cout << "RESULT: " << result << endl;

    // cout << "MESSAGE: " << wiadomosc << endl;
    // cout << "CIPHER: " << szyfr << endl;

    // bitset<16> b {string{"0000000110000000"}};
    // int x = b.to_ulong();

    // bitset<8> test = 'c';
    // cout << "CHAR: " << int('c') << endl;
    // cout << "CHAR: " << test << endl;

    // cout << x << endl;
    // unsigned char dupa  = x & 0xFF;
    // unsigned char low   = x;
    // unsigned char high  = x >> 8;

    // cout << "DUPA: " << (int)dupa << endl;
    // cout << "LOW: "  << (int)low  << endl;
    // cout << "HIGH: " << (int)high << endl;

    // unsigned long b1 = 7;
    // unsigned long b2 = 5;
    // unsigned long sum1 = b1^b2;
    // cout << "SUM: " << sum1 << endl;

    // bitset<8> a1{7};
    // bitset<8> a2{5};
    // bitset<8> sum2 = a1^a2;
    // cout << "DUPA: " << sum2 << '\n';

    // cout << endl;

    std::getchar();
    return 0;
}

*/