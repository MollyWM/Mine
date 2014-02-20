#include "stdafx.h"

MyMine::MyMine(){
}

void MyMine::ChangeStatus()
{
    switch (this->m_status)
    {
    case MineStatus::Normal:
        this->m_status = MineStatus::Flag;
        break;
    case MineStatus::Flag:
        this->m_status = MineStatus::Unknown;
        break;
    case MineStatus::Unknown:
        this->m_status = MineStatus::Normal;
        break;
    default:
        break;
    }
}