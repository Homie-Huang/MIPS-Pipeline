#include <iostream>
#include <bitset>
using namespace std;

class EX
{
public:
    EX()
    {
        ALUOp = 0;
        type = 0;
        RegDst = 0;
        ALUSrc = 0;
        Branch = 0;
        MemRead = 0;
        MemWrite = 0;
        RegWrite = 0;
        MemtoReg = 0;
        implement = 0; // implement初始化為0
    }

    //* Instruction Field
    bitset<5> rs;
    bitset<5> rt;
    bitset<5> Write_reg;  // R-type: rd，I-type: rt
    bitset<16> I_address; // I-type address

    //* ALU input
    bitset<32> Read_data1;
    bitset<32> Read_data2;

    //* OPcode
    bool ALUOp; // 0: sub，1: add、lw、sw
    bool type;  // 0: R-type，1: I-type

    //* Control Sign
    bool RegDst;
    bool ALUSrc;
    bool Branch;
    bool MemRead;
    bool MemWrite;
    bool RegWrite;
    bool MemtoReg;

    //* 在當前clock cycle，此stage是否在執行
    bool implement;
};