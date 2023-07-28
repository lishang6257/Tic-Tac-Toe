// gameboard.h
#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QObject>
#include <QPoint>
#include <Qhash>

#include "utils/util.h"
#include "utils/zobristhash.h"

class QLearning;

class GameBoard : public QObject
{
    Q_OBJECT

public:
    explicit GameBoard(QObject *parent = nullptr);
    explicit GameBoard(const GameBoard& other);
//    GameBoard& operator=(const GameBoard& other);


    Player getCurrentPlayer() const;
    void resetBoard();
    void resetGame();
    bool makeMove(const QPoint& position);
    bool isGameOver();
    bool isValidPosition(const QPoint& position) const;
    bool isValidPosition(const QPoint& position, bool isCheckPlayer) const;

    QVector<QPoint> getSearchStone() const;
    ZobristHash zobristHash;


    bool setBoard(const QPoint& position, Player player);

    Player** getCurrentBoard() const;

    QVector<QPoint> historyPlayer;
    QVector<QPoint> SearchHistoryPlayer;

    int minimax(int depth, bool maximizingPlayer);
    int minimax_pruning(int depth, int alpha, int beta, bool maximizingPlayer);

    QPoint calculateAIMove();
    int evaluateBoard(Player evaluatePlayer);



signals:
    void boardChanged(const QPoint& position, Player player);

private:
    QLearning *qlearning;
    Player board[GameSetting::BoardSize][GameSetting::BoardSize];

    Player currentPlayer;

    int minmaxSearchDepth = GameSetting::MinmaxSearchDepth;

    bool checkWin() const;
    bool isgameOver;

    int evaluateBoardTimesCurrentTurn;

    // 自定义quint64类型的哈希函数
    quint64 qHash(const quint64 key) {
        return key;
    }

    // 添加哈希表，用于保存局面的哈希值和对应的分数
    QHash<quint64, QPair<int,int>> hashTable;

    int evaluateBoardLine(QPoint p1, QPoint p2,QPoint p3, Player evaluatePlayer);

};

#endif // GAMEBOARD_H
