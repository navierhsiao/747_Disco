#ifndef __TOUCH_H
#define __TOUCH_H

//Todo:1.思考架構(用中斷方式，需要什麼函式，設定哪些register)
//Todo:2.在中斷中，更新位移的量及手勢方向

#define MAX_TOUCH_NUMBER 2

enum touch_gesture
{
    TOUCH_LEFT_TO_RIGHT=1,
    TOUCH_RIGHT_TO_LEFT=2,
    TOUCH_UP_TO_DOWN=3,
    TOUCH_DOWN_TO_UP=4
};

typedef struct
{
    int Hoffset_min;
    int Hoffset_max;
}touch_gestureTypeDef;

struct touch_structDef
{
    uint8_t touch_state;
    int touch_horizontal_offset[MAX_TOUCH_NUMBER];
    int touch_vetical_offset[MAX_TOUCH_NUMBER];

    int (*get_touch_offset) (struct touch_structDef*);
};

typedef struct touch_structDef touch_objectTypeDef;



#endif