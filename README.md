
uint8_t beepOnTickTable[10]  = {0,1,1,2,2,3,3,4,4,5};
uint8_t beepOffTickTable[10] = {1,1,1,2,2,3,3,4,4,5};

uint8_t ddd(uint8_t warninglevel)
{
    static uint8_t step = 1;
    static uint8_t preLevel = 0;
    uint8_t beepState = 0;

    static uint8_t onTicks = 0;
    static uint8_t offTicks = 1;

    if(preLevel != warninglevel)
    {
        preLevel = warninglevel;
        step = 0;
        
    }
    switch(step)
    {
        case 0:
            onTicks = beepOnTickTable[warninglevel];
            offTicks = beepOffTickTable[warninglevel];
            step = 1;
            break;
        case 1://ready
            if(onTicks)
            {
                beepState = 1;
                onTicks--;
            }
            else
            {
                step = 2;
            }
            beepState = 0;
            step++;
        case 2:
            if(offTicks)
            {
                beepState = 0;
                offTicks--;
            }
            else
            {
                step = 0;
            }
            beepState = 0;
            step++;
            break;
        default:
            beepState = 0;
        break;
    }

    return beepState;
}# Portable-Radiation-Meter
