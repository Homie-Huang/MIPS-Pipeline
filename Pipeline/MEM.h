#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

//! Access memory operand
class MEM
{
public:
    MEM(bitset<5> rs, bitset<5> rt, bitset<5> Write_reg, bitset<32> ALU_result, bitset<32> Write_data)
        : rs(rs), rt(rt), Write_reg(Write_reg), ALU_result(ALU_result), Write_data(Write_data)
    {
        Branch = 0;
        MemRead = 0;
        MemWrite = 0;
        RegWrite = 0;
        MemtoReg = 0;
        implement = 0; // implement初始化為0
    }

protected:
    //* Instruction Field
    bitset<5> rs;
    bitset<5> rt;
    bitset<5> Write_reg;  // R-type: rd，I-type: rt

    //* Data Memory input
    bitset<32> ALU_result;
    bitset<32> Write_data; // sw

    //* Control Sign
    bool Branch;
    bool MemRead;
    bool MemWrite;
    bool RegWrite;
    bool MemtoReg;

    //* 在當前clock cycle，此stage是否在執行
    bool implement;
};