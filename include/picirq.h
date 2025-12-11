#pragma once

void init_pic(void);
void pic_eoi(int irq);
void pic_enable_irq(int irq);
void pic_disable_irq(int irq);
int pic_spurious_irq(int irq);
