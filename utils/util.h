#ifndef UTIL_H
#define UTIL_H

#include <QPoint>
#include <QBitarray>

class GameSetting {
public:
    // 假设棋盘格子大小为30x30
    static const int CellSize = 100;

    static const int BoardSize = 3;

    //棋力
    static const int MinmaxSearchDepth = 9;

};


typedef quint8 Player; // Player 0:空 1：黑子 2：白子

#define PLAYER_NONE 0
#define PLAYER_BLACK 1
#define PLAYER_WHITE 2


//struct MyStruct {
//    unsigned int value : 2; // 定义一个占用 2 位的成员
//};

// 为QPoint定义一个自定义比较函数
struct QPointCompare {
    bool operator()(const QPoint& p1, const QPoint& p2) const {
        // 实现QPoint的比较逻辑
        // 例如，可以先根据x坐标进行比较，然后再比较y坐标。
        if (p1.x() < p2.x()) return true;
        if (p1.x() > p2.x()) return false;
        return p1.y() < p2.y();
    }
};

//class BitArray2Bit {
//public:
//    BitArray2Bit(int size) : size(size) {
//        // 计算位数组的大小（每两个元素占用一个位）
//        int bitArraySize = (size + 1) / 2;
//        // 创建位数组，并将所有位设置为 0
//        bitArray.resize(bitArraySize);
//        bitArray.fill(false, bitArraySize);
//    }

//    // 获取指定索引处的元素值
//    int get(int index) const {
//        if (index >= 0 && index < size) {
//            int bitArrayIndex = index / 2;
//            int shift = (index % 2) * 2;
//            return (bitArray[bitArrayIndex] >> shift) & 0b11;
//        }
//        return -1;
//    }

//    // 设置指定索引处的元素值
//    void set(int index, int value) {
//        if (index >= 0 && index < size && value >= 0 && value < 4) {
//            int bitArrayIndex = index / 2;
//            int shift = (index % 2) * 2;
//            bitArray[bitArrayIndex] &= ~(0b11 << shift);
//            bitArray[bitArrayIndex] |= (value << shift);
//        }
//    }

//private:
//    int size; // 元素个数
//    QBitArray bitArray; // 位数组，每个元素占用 1 bit
//};

#endif // UTIL_H
