#include <cassert>
//#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using VectorOfStrings_t = std::vector<std::string>;


bool split(const std::string &Str_in, char d, VectorOfStrings_t& VectStr_out)
{
//    assert(VectStr_out.empty());
    VectStr_out.clear();

    std::string::size_type start = 0;
    std::string::size_type stop = Str_in.find_first_of(d);
    while(stop != std::string::npos)
    {
        VectStr_out.push_back(Str_in.substr(start, stop - start));
        start = stop + 1;
        stop = Str_in.find_first_of(d, start);
    }

    VectStr_out.push_back(Str_in.substr(start));

    return true;
}

struct strIpAddress {
    struct strIpAddr4x8 {
        uint8_t fourth = 0;
        uint8_t third = 0;
        uint8_t second = 0;
        uint8_t first = 0;
    };

    union {
        uint32_t m_value32 = 0;
        strIpAddr4x8 m_value8x4;
//        unsigned char m_arrchars[4];
    };
    bool Test()
    {
        // ?? accept only little-endian
        static_assert(sizeof(*this) == 4, "fatal error");
        return m_value32 == ((m_value8x4.first * 256 + m_value8x4.second) * 256 + m_value8x4.third) * 256 + m_value8x4.fourth;
    };

    // constructor from std::string
    explicit strIpAddress(std::string& Str_in)
    {
        VectorOfStrings_t VectNumbers;
        split(Str_in, '.', VectNumbers);
        assert(VectNumbers.size() == 4);

        int tmp_int;

        // 4 copypaste - it might be better
        tmp_int = std::stoi(VectNumbers[0]);
        assert(tmp_int >=0 && tmp_int <=255);
        m_value8x4.first = tmp_int;

        tmp_int = std::stoi(VectNumbers[1]);
        assert(tmp_int >=0 && tmp_int <=255);
        m_value8x4.second = tmp_int;

        tmp_int = std::stoi(VectNumbers[2]);
        assert(tmp_int >=0 && tmp_int <=255);
        m_value8x4.third = tmp_int;

        tmp_int = std::stoi(VectNumbers[3]);
        assert(tmp_int >=0 && tmp_int <=255);
        m_value8x4.fourth = tmp_int;

        assert(this->Test());
    }

    friend std::ostream& operator<<(std::ostream& os, const strIpAddress& ip_in);

};

std::ostream &operator<<(std::ostream &os, const strIpAddress& ip_in) {
    os << unsigned(ip_in.m_value8x4.first) <<"." <<
        unsigned(ip_in.m_value8x4.second) << "." <<
        unsigned(ip_in.m_value8x4.third) << "." <<
        unsigned(ip_in.m_value8x4.fourth);
    return os;
}


//int main(int argc, char const *argv[]) {
int main()
{
//    try {
        std::vector<strIpAddress> ip_pool;

        for (std::string line; std::getline(std::cin, line);) {
            VectorOfStrings_t VectInputStrings;
            split(line, '\t', VectInputStrings);

            ip_pool.emplace_back(strIpAddress(VectInputStrings.at(0)));
        }

        // reverse lexicographically sort
        std::sort(ip_pool.begin(),
                  ip_pool.end(),
                  [](const strIpAddress& ip_addr1, const strIpAddress& ip_addr2)
                        {return ip_addr1.m_value32 > ip_addr2.m_value32;});

        for (auto ip : ip_pool)
            std::cout << ip << std::endl;

        // filter by first byte and output
        for (auto ip : ip_pool)
            if (ip.m_value8x4.first == 1)
                std::cout << ip << std::endl;

        // filter by first and second bytes and output
        for (auto ip : ip_pool)
            if (ip.m_value8x4.first == 46 && ip.m_value8x4.second == 70)
                std::cout << ip << std::endl;

        // filter by any byte
        for (auto ip : ip_pool)
            if (ip.m_value8x4.first == 46 ||
                    ip.m_value8x4.second == 46 ||
                    ip.m_value8x4.third == 46 ||
                    ip.m_value8x4.fourth == 46)
                std::cout << ip << std::endl;
//    }
}