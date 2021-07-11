#include "../../system.h"

touch_objectTypeDef touch;

int touch_get_horizontal_offset(touch_objectTypeDef* obj);

void touch_init(void)
{
    I2C_Init();

    touch.get_touch_offset=touch_get_horizontal_offset;
    touch.get_touch_offset(&touch);
}

int touch_get_horizontal_offset(touch_objectTypeDef* obj)
{
    return 3;
}