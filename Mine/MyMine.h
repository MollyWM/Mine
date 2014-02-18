#include "stdafx.h"

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
	Num5,
	Num4,
	Num3,
	Num2,
	Num1,
	Empty
};

class MyMine
{
public:
	MineStatus m_status;
	UINT m_width;
	UINT m_height;

public:
	MyMine();
	void ChangeStatus();
};