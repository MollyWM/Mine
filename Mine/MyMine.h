#include "stdafx.h"

class MyMine
{
    
public:

    enum MineStatus
    {
        Normal,
        Flag,
        Unknown,
        Bomb,
        Error,
        Mine,
        Unknown2,
        Num8,
        Num7,
        Num6,
        Num5,
        Num4,
        Num3,
        Num2,
        Num1,
        Empty
    };

    MineStatus m_status;
    UINT m_width;
    UINT m_height;
    BOOL m_isMine;
    UINT m_aroundMines;
    UINT m_x;
    UINT m_y;

public:
    MyMine();
    void ChangeStatus();
};