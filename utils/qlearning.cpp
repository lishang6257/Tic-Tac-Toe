
#include "utils/qlearning.h"
#include <QFile>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

QLearning::QLearning(QObject *parent)
{
    qTable = QLearningTable(); // 初始化qTable

}

// QLearning算法的训练函数，用于训练智能体学习最佳策略
void QLearning::train(GameBoard& game, int numEpisodes, double learningRate, double discountFactor, double explorationProb)
{
    QRandomGenerator randomGenerator(QRandomGenerator::global()->generate());
    randomGenerator.seed(QTime::currentTime().msec());
    //    randomGenerator.seed(0);

    for (int episode = 0; episode < numEpisodes; ++episode)
    {
        game.resetGame(); // 重置当前游戏模式，开始新的一局游戏


        while (!game.isGameOver()) // 在游戏没有结束的情况下进行训练
        {
            Player currentPlayer = game.getCurrentPlayer();
            quint64 currentState = game.zobristHash.hash;
            QVector<QPoint> actions = game.getSearchStone();
            QPoint action;
            if (qTable.contains(game.zobristHash.hash))
            {
                // 如果当前状态已经在Q表中存在，则根据贪心策略或者随机策略选择动作
                if (randomGenerator.generateDouble() < explorationProb)
                {
                    // 随机选择动作
                    action = actions[randomGenerator.bounded(actions.size())];
                }
                else
                {
                    // 根据贪心策略选择最优动作
                    int bestValue = -INT_MAX;
                    for (auto it = qTable[currentState].constBegin(); it != qTable[currentState].constEnd(); ++it) {
                        if(it.value() > bestValue) {
                            action = it.key();
                            bestValue = it.value();
                        }
                    }
                }
            }
            else
            {
                // 如果当前状态在Q表中不存在，则随机选择动作
                action = actions[randomGenerator.bounded(actions.size())];
            }

            game.makeMove(action);


            quint64 nextState = game.zobristHash.hash;
            double reward = game.evaluateBoard(currentPlayer); // 执行选择的动作并获取奖励

            if (!qTable.contains(currentState))
            {
                // 如果当前状态不在Q表中，则在Q表中新建该状态的记录
                qTable[currentState] = QHash<QPoint, double>();
            }

            if (!qTable[nextState].empty())
            {
                // 如果下一个状态不在Q表中，则在Q表中新建该状态的记录
                qTable[nextState] = QHash<QPoint, double>();
            }

            double maxNextQValue = 0;
            if(!qTable[nextState].empty()){
                double maxScore = -INT_MAX;
                for (auto it = qTable[nextState].constBegin(); it != qTable[nextState].constEnd(); ++it) {
                    if(it.value() > maxScore) {
                        action = it.key();
                        maxScore = it.value();
                    }
                }
                maxNextQValue = maxScore;
            }
            double currentQValue = qTable[currentState][action]; // 当前状态执行当前动作的Q值
            double updatedQValue = currentQValue + learningRate * (reward + discountFactor * maxNextQValue - currentQValue); // 更新Q值
            qTable[currentState][action] = updatedQValue; // 更新Q表中的Q值
        }

        QString autoSaveFilename = "./qTable/SnakeGameQTable_E" + QString::number(episode) + ".dat";
//        qDebug() << "epoch " + QString::number(episode);
        if((episode%10000) == 0){
            saveQTableToFile(autoSaveFilename); // 每500个epoch自动保存Q表到文件
            qDebug()<< QString::number(qTable.size());
        }
    }
}

// 从文件中加载Q表
void QLearning::loadQTableFromFile(const QString &filename)
{
    QLearningTable table{};
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        stream >> table;
        file.close();
    }

     qTable = table;
}

// 将Q表保存到文件
void QLearning::saveQTableToFile(const QString& filename) const
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);
        stream << qTable;
        file.close();
    }
}

