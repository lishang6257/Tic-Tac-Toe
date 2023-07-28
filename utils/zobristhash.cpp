#include "zobristhash.h"

#include <QVector>
#include <QDateTime>

ZobristHash::ZobristHash():hash(0)
{
    currentSeed = QDateTime::currentMSecsSinceEpoch();
    randomGenerator64.seed(0);

    initializeZobristHash();
}

ZobristHash::ZobristHash(const ZobristHash&other):hash(other.hash),
    randomGenerator64(other.randomGenerator64),
    currentSeed(other.currentSeed),
    playerSize(other.playerSize)
{
    zobristHash.append(QVector<QVector<quint64>>());
    for (int row = 0; row < GameSetting::BoardSize; ++row) {
        zobristHash[row].append(QVector<quint64>());
        for (int col = 0; col < GameSetting::BoardSize; ++col) {
            for (int piece = 0; piece < playerSize; ++piece) {
                zobristHash[row][col].append(other.zobristHash[row][col][piece]);
            }
        }
    }
}

void ZobristHash::reset()
{
    hash = 0;
    currentSeed = 0;

//    currentSeed = QDateTime::currentMSecsSinceEpoch();

    randomGenerator64.seed(currentSeed);

    initializeZobristHash();
}

// 生成一个随机的64位哈希值
quint64 ZobristHash::generateRandomHash() {
    return randomGenerator64.generate64();
}

// 初始化Zobrist哈希数组，为每个元素生成随机哈希值
void ZobristHash::initializeZobristHash() {
    zobristHash.resize(GameSetting::BoardSize);
    for (int row = 0; row < GameSetting::BoardSize; ++row) {
        zobristHash[row].resize(GameSetting::BoardSize);
        for (int col = 0; col < GameSetting::BoardSize; ++col) {
            zobristHash[row][col].resize(playerSize);
            for (int piece = 0; piece < playerSize; ++piece) {
                zobristHash[row][col][piece] = randomGenerator64.generate();
            }
        }
    }
}

// 计算当前棋盘状态的Zobrist哈希值
quint64 ZobristHash::calculateBoardHash(const Player board[GameSetting::BoardSize][GameSetting::BoardSize]) {
    quint64 ahash = 0;
    for (int row = 0; row < GameSetting::BoardSize; ++row) {
        for (int col = 0; col < GameSetting::BoardSize; ++col) {
            Player piece = board[row][col];
            if (piece != PLAYER_NONE) {
                ahash ^= zobristHash[row][col][(int)piece];
            }
        }
    }
    return ahash;
}

// 在棋盘上进行一步移动并更新Zobrist哈希值
void ZobristHash::setBoardAndUpdateHash(Player board[GameSetting::BoardSize][GameSetting::BoardSize], QPoint point, Player piece) {
    int row = point.x();
    int col = point.y();
    Player oldPiece = board[row][col];
    board[row][col] = piece;
    hash ^= zobristHash[row][col][(int)oldPiece];
    hash ^= zobristHash[row][col][(int)piece];
}
