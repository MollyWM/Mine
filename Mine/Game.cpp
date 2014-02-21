#include "stdafx.h"

Game::Game()
{
}

void Game::SetEasyLevel()
{
    this->SetLevel(10, 9, 9);
}

void Game::SetMediumLevel()
{
    this->SetLevel(40, 16, 16);
}

void Game::SetHardLevel()
{
    this->SetLevel(99, 30, 16);
}

void Game::SetCustomLevel(UINT numberOfMines, UINT numberOfBlockInX, UINT numberOfBlockInY)
{
    this->SetLevel(numberOfMines, numberOfBlockInX, numberOfBlockInY);
}

void Game::SetLevel(UINT numberOfMines, UINT numberOfBlockInX, UINT numberOfBlockInY)
{
    this->_numberOfMines = numberOfMines;
    this->_numberOfBlockInX = numberOfBlockInX;
    this->_numberOfBlockInY = numberOfBlockInY;
}

UINT Game::Elapse()
{
    this->_elapsedTime++;
    return this->_elapsedTime > 999 ? 999 : _elapsedTime;
}

BOOL Game::Init()
{
    this->_elapsedTime = 0;
    this->_started = FALSE;
    this->_failed = FALSE;

    for (UINT y = 0; y < this->_numberOfBlockInY; y++)
    {
        for (UINT x = 0; x < this->_numberOfBlockInX; x++)
        {
            MyMine* mine = (&this->_mines[x][y]);

            mine->m_width = 16;
            mine->m_height = 16;
            mine->m_status = MineStatus::Normal;
            mine->m_isMine = FALSE;
            mine->m_aroundMines = 0;
            mine->m_x = x;
            mine->m_y = y;
        }
    }

    return FALSE;
}

BOOL Game::Start()
{
    this->LayMines(this->_numberOfBlockInX, this->_numberOfBlockInY);
    this->_started = TRUE;
    return TRUE;
}

BOOL Game::Fail(UINT x, UINT y)
{
    if (!this->LeftClickInMineArea(x, y))
    {
        return FALSE;
    }

    MyMine* mine = &(this->_mines[x][y]);

    if (mine->m_isMine)
    {
        mine->m_status = MineStatus::Error;
        this->_started = FALSE;
        this->_failed = TRUE;
        return TRUE;
    }

    return FALSE;
}

BOOL Game::Success()
{
    for (UINT i = 0; i < this->_numberOfBlockInY; i++)
    {
        for (UINT j = 0; j < this->_numberOfBlockInX; j++) 
        {
            MyMine* mine = &(this->_mines[i][j]);

            if (mine->m_status == MineStatus::Normal || mine->m_status == MineStatus::Unknown)
            {
                return FALSE;
            }
        }
    }
    this->_started = FALSE;
    return TRUE;
}

BOOL Game::IsInMineArea(UINT x, UINT y)
{
    return (x >= 0 && x < this->_numberOfBlockInX && y >= 0 && y < this->_numberOfBlockInY);
}

BOOL Game::LeftClickInMineArea(UINT x, UINT y)
{
    if (!this->_started)
    {
        return FALSE;
    }

    if (!this->IsInMineArea(x, y))
    {
        return FALSE;
    }

    MyMine* mine = &(this->_mines[x][y]);

    if (mine->m_status == MineStatus::Flag)
    {
        return FALSE;
    }

    if (mine->m_isMine)
    {
        mine->m_status = MineStatus::Bomb;
    }
    else
    {
        this->ExpandMines(x, y);
    }

    return TRUE;
}

BOOL Game::RightClickInMineArea(UINT x, UINT y)
{
    if (!this->_started)
    {
        return FALSE;
    }

    if (!this->IsInMineArea(x, y))
    {
        return FALSE;
    }

    MyMine* pMine = &(this->_mines[x][y]);

    this->_mines[x][y].ChangeStatus();

    if (pMine->m_status == MineStatus::Flag)
    {
        this->_numberOfMines--;
    }
    else if (pMine->m_status == MineStatus::Unknown)
    {
        this->_numberOfMines++;
    }

    return TRUE;
}

void Game::LayMines(UINT row, UINT col)
{
    //埋下随机种子
    srand( (unsigned)time( NULL ) );
    UINT i, j;

    for(UINT index = 0; index < this->_numberOfMines;) 
    {	
        //取随即数
        i = rand() % this->_numberOfBlockInX;
        j = rand() % this->_numberOfBlockInY;

        if (i == row && j == col) continue;

        MyMine* mine = &((this->_mines)[i][j]);

        if(!mine->m_isMine) 
        {
            mine->m_isMine = TRUE;//修改属性为雷
            index++;
        }
    }	
}

void Game::ExpandMines(UINT row, UINT col)
{
    UINT i, j;
    UINT minRow = (row == 0) ? 0 : row - 1;
    UINT maxRow = row + 2;
    UINT minCol = (col == 0) ? 0 : col - 1;
    UINT maxCol = col + 2;
    UINT around = GetAroundNum(row, col);


    this->_mines[row][col].m_status = (MineStatus)(15 - around);

    // “打开”该区域，重绘
    //DrawSpecialMine(row, col);

    //对周围一个雷都没有的空白区域
    if (around == 0)
    {
        for (i = minRow; i < maxRow; i++) 
        {
            for (j = minCol; j < maxCol; j++) 
            {
                //对于周围可以拓展的区域进行的规拓展		
                MyMine* mine = &(this->_mines[i][j]);
                if (!(i == row && j == col) && mine->m_status == MineStatus::Normal && !mine->m_isMine)
                {
                    if (!IsInMineArea(i, j)) continue;
                    ExpandMines(i, j);
                }
            }
        }
    }
}

UINT Game::GetAroundNum(UINT x, UINT y)
{
    UINT i, j;
    UINT around = 0;
    UINT minRow = (x == 0) ? 0 : x - 1;
    UINT maxRow = x + 2;
    UINT minCol = (y == 0) ? 0 : y - 1;
    UINT maxCol = y + 2;

    for (i = minRow; i < maxRow; i++) 
    {
        for (j = minCol; j < maxCol; j++)
        {
            if (!IsInMineArea(i, j)) continue;
            if (this->_mines[i][j].m_isMine) around++;
        }
    }
    return around;
}

UINT Game::GetAroundFlags(UINT x, UINT y)
{
    UINT i, j;
    UINT flags = 0;
    UINT minRow = (x == 0) ? 0 : x - 1;
    UINT maxRow = x + 2;
    UINT minCol = (y == 0) ? 0 : y - 1;
    UINT maxCol = y + 2;

    for (i = minRow; i < maxRow; i++) 
    {
        for (j = minCol; j < maxCol; j++)
        {
            if (!IsInMineArea(i, j)) continue;
            if (this->_mines[i][j].m_status == MineStatus::Flag) flags++;
        }
    }
    return flags;
}

UINT Game::GetElapsedTime()
{
    return this->_elapsedTime;
}

UINT Game::GetNumberOfMines()
{
    return this->_numberOfMines;
}

UINT Game::GetNumberOfBlockInX()
{
    return this->_numberOfBlockInX;
}

UINT Game::GetNumberOfBlockInY()
{
    return this->_numberOfBlockInY;
}

BOOL Game::GetStarted()
{
    return this->_started;
}

MyMine& Game::GetMine(UINT x, UINT y)
{
    return this->_mines[x][y];
}

BOOL Game::GetFailed()
{
    return this->_failed;
}