// -*- coding: utf-8 -*-

#ifndef QLEARNING_H
#define QLEARNING_H

#include <QObject>
#include <QHash>
#include <QVector>
#include <QTime>
#include <QRandomGenerator>
#include "gameboard.h"

typedef QHash<quint64,QHash<QPoint,double>>QLearningTable;

class QLearning : public QObject
{
    Q_OBJECT

private:



public:
    // 初始化 Q 表
    QLearningTable qTable;

    // 自定义quint64类型的哈希函数
    quint64 qHash(const quint64 key) {
        return key;
    }

    explicit QLearning(QObject *parent = nullptr);

    // 将 Q 表保存到文件中
    void saveQTableToFile(const QString& filename) const;

    // 从文件中加载 Q 表
    void loadQTableFromFile(const QString& filename);

    // Q 学习函数
    void train(GameBoard& game, int numEpisodes, double learningRate, double discountFactor, double explorationProb);

};

#endif // QLEARNING_H
