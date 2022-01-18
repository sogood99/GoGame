#include "utils.h"

Pos::Pos(int x, int y)
{
    position[0] = x;
    position[1] = y;
}

void Pos::setPosition(int x, int y)
{
    position[0] = x;
    position[1] = y;
}

stone otherStone(stone currentStone)
{
    /*
        Get other stone, if black -> white, if white -> black
    */
    if (currentStone == black)
    {
        return white;
    }
    else if (currentStone == white)
    {
        return black;
    }
    return noStone;
}

GoMove::GoMove()
{
    /*
        Initiaize go move
    */
    isSet = false;
}

GoMove::GoMove(moveType mvtp, Pos pos)
{
    /*
        Initiaize go move
    */
    GoMove::isSet = true;
    GoMove::mt = mvtp;
    GoMove::pos = pos;
}
