#include "IF.h"
#include "ID.h"
#include "EX.h"
#include "MEM.h"
#include "WB.h"

class State
{
public:
    State(){};
    
    IF IF_stage;
    ID ID_stage;
    EX EX_stage;
    MEM MEM_stage;
    WB WB_stage;
};