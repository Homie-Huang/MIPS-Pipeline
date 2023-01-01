#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
using namespace std;

//* 32words = 128bytes
//* 將每個instruction(32bits=4bytes)的每個byte存成一行，因此mem_size設成128
//* 記憶體是以byte為基本單位，這樣也能符合 PC = PC + 4
#define mem_size 128

class Instruction_Memory
{
public:
    Instruction_Memory()
    {
        ins_mem.resize(mem_size); // 將容器(ins_mem)大小，使其大小調整為mem_size

        //* 讀取input.txt
        ifstream file("input.txt");
        string line;
        int i = 0;
        while (getline(file, line))
        {
            ins_mem[i] = bitset<8>(line);
            i++;
        }
        file.close();
    }

    //* 根據PC抓取[PC ~ PC+3]的ins_mem中的bits，組合成一個完整32bits的instruction
    bitset<32> read(bitset<32> PC)
    {
        string ins;
        for (int k = 0; k < 4; k++)
        {
            ins.append(ins_mem[PC.to_ulong() + k].to_string());
        }
        instruction = bitset<32>(ins);

        return instruction;
    }

private:
    vector<bitset<8>> ins_mem; // 以byte(8bits)為單位
    bitset<32> instruction;
};