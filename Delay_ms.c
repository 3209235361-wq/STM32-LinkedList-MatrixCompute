#include "Delay_ms.h"
void Delay_ms(uint16_t ms){
    uint32_t i;
    while(ms--){
        for(i=0;i<7200;i++);
    }
}