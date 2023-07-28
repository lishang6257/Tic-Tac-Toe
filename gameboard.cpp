// gameboard.cpp
#include "gameboard.h"
#include "utils/qlearning.h"
#include <QPair>


#include <QMessageBox>
#include <QRandomGenerator>


GameBoard::GameBoard(QObject *parent) : QObject(parent),
    currentPlayer(PLAYER_NONE),
    isgameOver(false),
    evaluateBoardTimesCurrentTurn(0)
{
    resetBoard();

    currentPlayer = PLAYER_BLACK;

    qlearning = new QLearning();
    QString qTableFilename = "./FinalQTable.dat";
    qlearning->loadQTableFromFile(qTableFilename);

}

// Define the explicit copy constructor
GameBoard::GameBoard(const GameBoard& other)
    : QObject(other.parent()),
//    board(other.board),
    zobristHash(other.zobristHash),
    currentPlayer(other.currentPlayer),
    minmaxSearchDepth(other.minmaxSearchDepth),
    isgameOver(other.isgameOver),
    evaluateBoardTimesCurrentTurn(other.evaluateBoardTimesCurrentTurn)
{

}

Player GameBoard::getCurrentPlayer() const
{
    return currentPlayer;
}

void GameBoard::resetBoard()
{
    for (int row = 0; row < GameSetting::BoardSize; ++row)
    {
        for (int col = 0; col < GameSetting::BoardSize; ++col)
        {
            board[row][col] = PLAYER_NONE;
        }
    }

}

void GameBoard::resetGame()
{
    resetBoard();
    isgameOver = false;
    currentPlayer = PLAYER_BLACK;
    zobristHash.reset();
    hashTable.clear();

    historyPlayer.clear();
    SearchHistoryPlayer.clear();

}

bool GameBoard::isGameOver()
{
    isgameOver = true;
    for(int i = 0;i < GameSetting::BoardSize; i++){
        int xWin = board[i][0] + board[i][1] + board[i][2];
        int yWin = board[0][i] + board[1][i] + board[2][i];
        if(xWin == 6 || yWin==6) return isgameOver;
        if((xWin == 3 && (board[i][0]!=2 && board[i][1]!=2 && board[i][2]!=2))
            || (yWin == 3 && (board[0][i]!=2 && board[1][i]!=2 && board[2][i]!=2))) return isgameOver;
    }
    int xWin = abs(board[0][0] + board[1][1] + board[2][2]);
    int yWin = abs(board[2][0] + board[1][1] + board[0][2]);
    if(xWin == 6 || yWin==6) return isgameOver;
    if((xWin == 3 && (board[0][0]!=2 && board[1][1]!=2 && board[2][2]!=2))
        || (yWin == 3 && (board[2][0]!=2 && board[1][1]!=2 && board[0][2]!=2))) return isgameOver;
    int count = 0;
    for(int i = 0;i < GameSetting::BoardSize;i ++){
        for(int j = 0;j < GameSetting::BoardSize;j ++){
            if(board[i][j] == PLAYER_NONE) count++;
        }
    }
    if(count == 0) return isgameOver;
    isgameOver = false;
    return isgameOver;
}

bool GameBoard::isValidPosition(const QPoint& position) const
{
    if(position.x() < 0 || position.x()>=GameSetting::BoardSize ||
        position.y() < 0 || position.y()>=GameSetting::BoardSize)
        return false;
    if(board[position.x()][position.y()] != PLAYER_NONE)
        return false;
    return true;
}

bool GameBoard::isValidPosition(const QPoint& position, bool isCheckPlayer) const
{
    if(position.x() < 0 || position.x()>=GameSetting::BoardSize ||
        position.y() < 0 || position.y()>=GameSetting::BoardSize)
        return false;
    if(isCheckPlayer){
        if(board[position.x()][position.y()] != PLAYER_NONE)
        return false;
    }
    return true;
}

bool GameBoard::setBoard(const QPoint& position, Player player)
{
    if(!isValidPosition(position, false)) return false;
    zobristHash.setBoardAndUpdateHash(board,position, player);
    //维护搜索历史
    if(player == PLAYER_NONE && !SearchHistoryPlayer.empty()) SearchHistoryPlayer.pop_back();
    if(player != PLAYER_NONE) SearchHistoryPlayer.push_back(position);
    return true;
}

bool GameBoard::makeMove(const QPoint& position)
{
    if(isGameOver()) return false;
    if(!isValidPosition(position)) return false;

    setBoard(position, currentPlayer);

    emit boardChanged(position, currentPlayer);

    if (checkWin())
    {
        qDebug() << "Game Over!";
        isgameOver = true;
    }
    currentPlayer = (currentPlayer == PLAYER_BLACK) ? PLAYER_WHITE : PLAYER_BLACK;

    historyPlayer.append(position);

    qDebug() << "本轮共计算叶子节点数目:"+ QString::number(evaluateBoardTimesCurrentTurn);

    evaluateBoardTimesCurrentTurn = 0;

    return true;
}

QVector<QPoint> GameBoard::getSearchStone() const
{
    QVector<QPoint> res;
    for (int i = 0; i < GameSetting::BoardSize; ++i)
    {
        for (int j = 0; j < GameSetting::BoardSize; ++j)
        {
            if(board[i][j]==PLAYER_NONE)res.append(QPoint(i,j));
        }
    }

    return res;
}

bool GameBoard::checkWin() const
{
    for(int i = 0;i < GameSetting::BoardSize; i++){
        int xWin = board[i][0] + board[i][1] + board[i][2];
        int yWin = board[0][i] + board[1][i] + board[2][i];
        if(xWin == 6 || yWin==6) return true;
        if((xWin == 3 && (board[i][0]!=2 && board[i][1]!=2 && board[i][2]!=2))
            || (yWin == 3 && (board[0][i]!=2 && board[1][i]!=2 && board[2][i]!=2))) return true;
    }
    int xWin = abs(board[0][0] + board[1][1] + board[2][2]);
    int yWin = abs(board[2][0] + board[1][1] + board[0][2]);
    if(xWin == 6 || yWin==6) return true;
    if((xWin == 3 && (board[0][0]!=2 && board[1][1]!=2 && board[2][2]!=2))
        || (yWin == 3 && (board[2][0]!=2 && board[1][1]!=2 && board[0][2]!=2))) return true;
    return false;
}

Player** GameBoard::getCurrentBoard() const
{
    Player** myArray = new Player*[GameSetting::BoardSize];
    for (int i = 0; i < GameSetting::BoardSize; ++i) {
        myArray[i] = new Player[GameSetting::BoardSize];
    }
    for (int i = 0; i < GameSetting::BoardSize; ++i) {
        for (int j = 0; i < GameSetting::BoardSize; ++i) {
            myArray[i][j] = board[i][j];
        }
    }
    return myArray;
}
