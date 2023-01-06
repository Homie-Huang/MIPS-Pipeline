#include <iostream>
#include <bitset>
using namespace std;

class ID
{
public:
    ID()
    {
        implement = 0; // implement初始化為0
        stall = false;
    }

    //* instruction
    bitset<32> ins;

    //* 在當前clock cycle，此stage是否在執行
    bool implement;

    //* 儲存當前狀態指令名稱
    string ins_name;

    // *儲存ID是否有stall 可提供給IF 使其一起stall
    bool stall;
};