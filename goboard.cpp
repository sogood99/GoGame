#include "goboard.h"

// public _______

GoBoard::GoBoard(size boardSize){
    /*
        Initialize GoBoard Class
    */
    GoBoard::boardSize = boardSize;
    currentBoard = QVector<QVector<stone> >(boardSize, QVector<stone>(boardSize, noStone));
    lastBoard = currentBoard;
}

void GoBoard::putStone(Pos p, stone stn){
    /*
        Put stone on board and dont care if it's valid
    */
    int x = p.position[0];
    int y = p.position[1];
    currentBoard[x][y] = stn;
}

boolInt GoBoard::placeStone(Pos p, stone stn){
    /*
        Put stone on board and check if it's valid
        Returns bool for if valid and placed
                int for capture count
    */
    if (!positionInBoard(p)){
        return boolInt(false);
    }
    if (!positionIsEmpty(p)){
        return boolInt(false);
    }

    if (isKo(p, stn)){
        return boolInt(false);
    }

    CapturedTuple captup = captureOther(p, stn);

    if (captup.isCaptured){
        setLastBoard();
        removeCapturedStones(captup);
        int x = p.position[0];
        int y = p.position[1];
        currentBoard[x][y] = stn;
        captup.countCaptured();
        return boolInt(true, captup.captureNum);
    }
    if (isSuicide(p, stn)){
        return boolInt(false);
    }
    setLastBoard();
    putStone(p, stn);
    return boolInt(true);
}

bool GoBoard::canPlaceStone(Pos p, stone stn){
    /*
        Only check if it's valid to place stone
    */
    if (!positionInBoard(p)){
        return false;
    }
    if (!positionIsEmpty(p)){
        return false;
    }
    if (isKo(p, stn)){
        return false;
    }
    CapturedTuple captup = captureOther(p, stn);
    if (captup.isCaptured){
        return true;
    }
    if (isSuicide(p, stn)){
        return false;
    }
    return true;
}

QVector<QVector<stone>> GoBoard::floodFill(QVector<QVector<DeadStone>> deadStones){
    /*
        Flood fill based on DFS used in area scoring
    */
    QVector<QVector<stone>> filled(boardSize, QVector<stone>(boardSize,noStone));

    QVector<QVector<bool>> accessed(boardSize, QVector<bool>(boardSize,false));
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            DFSFloodFill(i,j,accessed,filled,deadStones);
        }
    }
    return filled;
}

void GoBoard::displayBoardConsole(){
    /*
        Display board used in early versions
    */
    std::cout<< std::setw(2)<<"";
    for (int i = 0; i < boardSize; i++){
        std::cout<< std::setw(2)<<std::right<< i;
    }
    std::cout<<std::endl;
    for (int i = 0; i < boardSize; i++){
        std::cout<< std::setw(2)<<std::left<< i;
        for (int j = 0; j < boardSize; j++){
            if (currentBoard[i][j] == noStone){
                std::cout<< std::setw(2)<<std::right<< ".";
            }else if (currentBoard[i][j] == black){
                std::cout<< std::setw(2)<<std::right<< "X";
            }else{
                std::cout<< std::setw(2)<<std::right<< "O";
            }
        }
        std::cout<<std::endl;
    }
}

void GoBoard::displayLastBoardConsole(){
    /*
        Display last board used in early versions
    */
    std::cout<< std::setw(2)<<"";
    for (int i = 0; i < boardSize; i++){
        std::cout<< std::setw(2)<<std::right<< i;
    }
    std::cout<<std::endl;
    for (int i = 0; i < boardSize; i++){
        std::cout<< std::setw(2)<<std::left<< i;
        for (int j = 0; j < boardSize; j++){
            if (lastBoard[i][j] == noStone){
                std::cout<< std::setw(2)<<std::right<< ".";
            }else if (lastBoard[i][j] == black){
                std::cout<< std::setw(2)<<std::right<< "X";
            }else{
                std::cout<< std::setw(2)<<std::right<< "O";
            }
        }
        std::cout<<std::endl;
    }
}

// private ________

QVector<Pos> GoBoard::neighbor(Pos p){
    /*
        Find neighbors of a point, accounting for edges
    */
    QVector<Pos> N;
    int x = p.position[0];
    int y = p.position[1];
    if (x > 0){
        N.push_back(Pos(x - 1, y));
    }
    if (x < boardSize - 1){
        N.push_back(Pos(x+1, y));
    }
    if (y > 0){
        N.push_back(Pos(x, y-1));
    }
    if (y < boardSize - 1){
        N.push_back(Pos(x, y+1));
    }
    return N;
}

bool GoBoard::positionInBoard(Pos p){
    /*
        Check if position is in board
    */
    int x = p.position[0];
    int y = p.position[1];
    if (x<boardSize && x >= 0){
        if (y < boardSize && y >= 0){
            return true;
        }
    }
    return false;
}

bool GoBoard::positionIsEmpty(Pos p){
    /*
        Check if position is empty, assumes position is in board
    */
    int x = p.position[0];
    int y = p.position[1];
    return (currentBoard[x][y] == noStone);
}

bool GoBoard::isKo(Pos p, stone stn){
    /*
        Checks if position is ko, assumes position is in board
    */
    int x = p.position[0];
    int y = p.position[1];
    if (lastBoard[x][y] != stn){
        return false;
    }
    QVector<Pos> neigh = neighbor(p);
    bool onlyOne = true;
    for (int i = 0; i < neigh.size(); i++){
        int a = neigh[i].position[0];
        int b = neigh[i].position[1];

        if (lastBoard[a][b] == stn){
            onlyOne = false;
            break;
        }
    }
    return (onlyOne);
}

bool GoBoard::isSuicide(Pos p, stone stn){
    /*
        Checks if position is suicide, assumes position is in board
    */
    int x = p.position[0];
    int y = p.position[1];
    currentBoard[x][y] = stn;
    CapturedTuple captup = capture(p, stn);
    currentBoard[x][y] = noStone;
    if (captup.isCaptured){
        return true;
    }
    return false;
}

bool GoBoard::isEye(Pos p, stone stn){
    /*
        Check is position is eye, assumes position is in board
    */
    QVector<Pos> N = neighbor(p);
    for (int i = 0; i < N.size(); i++){
        int x = N[i].position[0];
        int y = N[i].position[1];
        if (currentBoard[x][y] != stn){
            return false;
        }
    }
    return true;
}

void GoBoard::setLastBoard(){
    /*
        Set last board to current board, used in setting next move
    */
    lastBoard = currentBoard;
}

CapturedTuple GoBoard::captureOther(Pos p, stone stn){
    /*
        Capture other by using capture function and place stone
    */
    CapturedTuple captureTup(boardSize);
    QVector<Pos> neigh = neighbor(p);

    int x = p.position[0];
    int y = p.position[1];
    currentBoard[x][y] = stn;
    for (int i = 0; i < neigh.size(); i++){
        CapturedTuple capTemp = capture(neigh[i], otherStone(stn));
        if (capTemp.isCaptured){
            captureTup.append(capTemp);
        }
    }
    currentBoard[x][y] = noStone;
    return captureTup;
}

CapturedTuple GoBoard::capture(Pos p, stone stn){
    /*
        Returns capturedTuple (bool int vector) for detecting captured stones
        Combined with place stone function to detect if capture other (used in capture other)
    */
    int x = p.position[0];
    int y = p.position[1];

    if (currentBoard[x][y] != stn){
        return CapturedTuple(boardSize);
    }

    std::deque<Pos> Front = {p};
    QVector<QVector<bool>> accessed(boardSize, QVector<bool>(boardSize, false));
    accessed[x][y] = true;

    capturedState capturedStateofGroup = notAccessed;
    CapturedTuple capturedTup(boardSize);

    while (!Front.empty()){
        long int sz = Front.size();
        for (long int i = 0; i < sz; i++){
            Pos front = Front.front();
            Front.pop_front();
            QVector<Pos> N = neighbor(front);
            for (int j = 0; j < N.size(); j++){
                int x = N[j].position[0];
                int y = N[j].position[1];
                if (!accessed[x][y] && currentBoard[x][y] != otherStone(stn)){
                    // make sure in group and not accessed
                    Front.push_back(N[j]);
                    accessed[x][y] = true;
                    if (capturedTup.capturedVector[x][y] != notAccessed){
                        capturedStateofGroup = capturedTup.getState(Pos(x,y));
                    }else if (currentBoard[x][y] == noStone){
                        capturedStateofGroup = notCaptured;
                        break;
                    }
                }
            }
        }

        if (capturedStateofGroup == notCaptured){
            break;
        }
    }

    if (capturedStateofGroup == notAccessed || capturedStateofGroup == Captured){
        for (int i = 0; i < boardSize; i++){
            for (int j = 0; j < boardSize; j++){
                if (accessed[i][j]){
                    capturedTup.setCaptured(Pos(i, j));
                }
            }
        }
        capturedTup.setBool(true);
        capturedTup.countCaptured();
    }
    return capturedTup;
}

void GoBoard::removeCapturedStones(CapturedTuple capturedTup){
    /*
        Removes captured stones
    */
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            if (capturedTup.getState(Pos(i, j)) == Captured){
                currentBoard[i][j] = noStone;
            }
        }
    }
}

//private

void GoBoard::DFSFloodFill(int i, int j, QVector<QVector<bool>> &accessed,QVector<QVector<stone> > &filled,
                            QVector<QVector<DeadStone> > deadStones){
    /*
        Uses DPS for flood fill, used in flood fill
    */
    if (accessed[i][j]){
        return;
    }
    if (currentBoard[i][j] != noStone && deadStones[i][j] == notDead){
        accessed[i][j] = true;
        filled[i][j] = currentBoard[i][j];
        return;
    }
    QVector<Pos> front = {Pos(i,j)};
    QVector<Pos> nextFront;
    QVector<QVector<bool>> currentAccessed(boardSize, QVector<bool>(boardSize, false));
    currentAccessed[i][j] = true;
    stone groupStone = noStone;
    int n = front.size();
    bool bothStones = false;
    while (n){
        for (int i = 0; i < n; i++){
            int x = front[i].position[0];
            int y = front[i].position[1];
            if (accessed[x][y] && filled[x][y]){
                groupStone = filled[x][y];
            }else if (currentBoard[x][y] != noStone && deadStones[x][y] == notDead){
                if (groupStone != noStone && currentBoard[x][y] != groupStone){
                    groupStone = noStone;
                    bothStones = true;
                }else if (!bothStones){
                    groupStone = currentBoard[x][y];
                }
            }
            if (currentBoard[x][y] == noStone || deadStones[x][y] == isDead){
                if (!accessed[x][y]){
                QVector<Pos> neigh = neighbor(front[i]);
                int m = neigh.size();
                for (int j = 0; j < m; j++){
                    int a = neigh[j].position[0];
                    int b = neigh[j].position[1];
                    if (!currentAccessed[a][b]){
                        nextFront.append(Pos(a,b));
                        currentAccessed[a][b] = true;
                    }
                }
                }
            }
        }
        front = nextFront;
        nextFront = QVector<Pos>();
        n = front.size();
    }
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            if (currentAccessed[i][j]){
                if (deadStones[i][j] == isDead || currentBoard[i][j] == noStone){
                    filled[i][j] = groupStone;
                    accessed[i][j] = true;
                }
            }
        }
    }

}

// caputure class

CapturedTuple::CapturedTuple(size boardSize){
    /*
        Initialize captured tuple
    */
    capturedVector = QVector<QVector<capturedState>>(boardSize, QVector<capturedState>(boardSize, notAccessed));
}

CapturedTuple::CapturedTuple(size boardSize, bool tfCaptured){
    /*
        Initialize captured tuple
    */
    capturedVector = QVector<QVector<capturedState>>(boardSize, QVector<capturedState>(boardSize, notAccessed));
    setBool(tfCaptured);
}

void CapturedTuple::setBool(bool tf){
    /*
        Set bool
    */
    isCaptured = tf;
}

void CapturedTuple::setCaptured(Pos p){
    /*
        Set position captured
    */
    int x = p.position[0];
    int y = p.position[1];
    capturedVector[x][y] = Captured;
}

void CapturedTuple::setNotCaptured(Pos p){
    /*
        Set position not captured
    */
    int x = p.position[0];
    int y = p.position[1];
    capturedVector[x][y] = notCaptured;
}

capturedState CapturedTuple::getState(Pos p){
    /*
        Get position of position
    */
    int x = p.position[0];
    int y = p.position[1];
    return capturedVector[x][y];
}

void CapturedTuple::append(CapturedTuple other){
    /*
        Merge two captured tuples
    */
    int n = capturedVector[0].size();
    if (!other.isCaptured){
        return;
    }
    isCaptured = true;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (other.capturedVector[i][j] == Captured){
                capturedVector[i][j] = Captured;
            }
        }
    }
}

int CapturedTuple::countCaptured(){
    /*
        Count number of captured stones
    */
    captureNum = 0;
    long int n = capturedVector[0].size();
    for (long int i = 0; i < n; i++){
        for (long int j = 0; j < n; j++){
            if (capturedVector[i][j] == Captured){
                captureNum ++;
            }
        }
    }
    return captureNum;
}
