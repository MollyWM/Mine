#include "stdafx.h"


class Game
{
public:
    BOOL Init();
    BOOL Start();
    BOOL ReStart();
    BOOL Fail(UINT x, UINT y);
    BOOL Success();
    UINT Elapse();
    BOOL RightClickInMineArea(UINT x, UINT y);
    BOOL LeftClickInMineArea(UINT x, UINT y);

    void SetEasyLevel();
    void SetMediumLevel();
    void SetHardLevel();
    void SetCustomLevel(UINT numberOfMines, UINT numberOfBlockInX, UINT numberOfBlockInY);


	MyMine _mines[100][100];
    UINT _elapsedTime;
    UINT _numberOfMines;
    UINT _numberOfBlockInX;
    UINT _numberOfBlockInY;
    BOOL _started;

protected: 
    void SetLevel(UINT numberOfMines, UINT numberOfBlockInX, UINT numberOfBlockInY);
    BOOL IsInMineArea(UINT x, UINT y);
    void LayMines(UINT row, UINT col);
    void ExpandMines(UINT row, UINT col);
    UINT GetAroundNum(UINT row, UINT col);
    UINT GetAroundFlags(UINT row, UINT col);

private:
    
};