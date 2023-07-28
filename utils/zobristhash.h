#ifndef ZOBRISTHASH_H
#define ZOBRISTHASH_H

#include<QObject>
#include<QRandomGenerator64>

#include"utils/util.h"

class ZobristHash : public QObject
{
    Q_OBJECT

public:
    ZobristHash();
    ZobristHash(const ZobristHash&other);

    void reset();

    quint64 hash;
    void setBoardAndUpdateHash(Player board[GameSetting::BoardSize][GameSetting::BoardSize], QPoint point, Player piece);

private:
    // 定义Zobrist哈希数组
    QVector<QVector<QVector<quint64>>> zobristHash;

    QRandomGenerator64 randomGenerator64;
    qint64 currentSeed;
    const int playerSize = 3;

    void initializeZobristHash();
    quint64 generateRandomHash();
    quint64 calculateBoardHash(const Player board[GameSetting::BoardSize][GameSetting::BoardSize]);


};

#endif // ZOBRISTHASH_H
