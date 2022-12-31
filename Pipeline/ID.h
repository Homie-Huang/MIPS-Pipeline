#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

//! Instruction decode & register read
class ID
{
public:
    ID(bitset<32> INS)
        : INS(INS)
    {
        implement = 0; // implement初始化為0
    }

protected:
    //* instruction
    bitset<32> INS;

    //* 在當前clock cycle，此stage是否在執行
    bool implement;
};