#include <iostream>
#include <bitset>
using namespace std;

class WB
{
public:
    WB()
    {
        RegWrite = 0;
        MemtoReg = 0;
        implement = 0; // implement初始化為0
    }

    //* Instruction Field
    bitset<5> rs;
    bitset<5> rt;
    bitset<5> Write_reg; // R-type: rd，I-type: rt

    //* Register input
    bitset<32> Write_data;

    //* Control Sign
    bool RegWrite;
    bool MemtoReg;

    //* 在當前clock cycle，此stage是否在執行
    bool implement;
};