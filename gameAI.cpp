#include "gameboard.h"
#include <QDebug>
#include "utils/qlearning.h"

#include <QDateTime>

QPoint GameBoard::calculateAIMove()
{
    int maxScore = - INT_MAX; // 初始化最大分数为一个较小值
    QPoint bestMove(-1,-1);

    for(int row = 0;row < GameSetting::BoardSize; row++)
    {
        for(int col = 0;col < GameSetting::BoardSize; col++)
        {
            if (board[row][col] == PLAYER_NONE) // 检查是否是空白位置
            {
                //使用zobrist hash搜索
                setBoard(QPoint(row,col), currentPlayer);
                if (hashTable.contains(zobristHash.hash))
                {
                    int score = hashTable.value(zobristHash.hash).first;
                    if (score > maxScore)
                    {
                        maxScore = score;
                        bestMove = QPoint(row,col);
                    }
                }
                else
                {
                    // 哈希表中没有保存当前局面的哈希值，进行搜索
                    int score = minimax_pruning(minmaxSearchDepth - 1, maxScore, INT_MAX, false);
//                    int score = minimax(minmaxSearchDepth - 1, false);

                    // 将搜索结果保存到哈希表
                    hashTable[zobristHash.hash] = qMakePair(score, 1);

                    if (score > maxScore)
                    {
                        maxScore = score;
                        bestMove = QPoint(row,col);
                    }

                }
                qDebug() << "putting place:(" + QString::number(row) + "," + QString::number(col)
                                + ") : " + QString::number(maxScore);
                setBoard(QPoint(row,col), PLAYER_NONE);

            }
        }
    }

    qDebug() << "putting place:(" + QString::number(bestMove.x()) + "," + QString::number(bestMove.y()) + ") : "
                    + QString::number(maxScore);

    qDebug() << "-----------------putting place end-------------";

    return bestMove; // 返回最优的移动位置
}

int GameBoard::minimax(int depth, bool maximizingPlayer)
{
    if (depth == 0 || isGameOver()) // 达到搜索深度或游戏结束时，返回当前局面的评估分数
    {
        if (hashTable.contains(zobristHash.hash) && hashTable.value(zobristHash.hash).second >= depth)
        {
            return hashTable.value(zobristHash.hash).first;
        }
        hashTable[zobristHash.hash] = qMakePair(evaluateBoard(currentPlayer), depth);
        return hashTable[zobristHash.hash].first;
    }

    if (maximizingPlayer) // MAX节点
    {
        int maxScore = -INT_MAX; // 初始化最大分数为一个较小值

        // 遍历所有合法的空白位置，找到最大分数
        for(int row = 0;row < GameSetting::BoardSize; row++)
        {
            for(int col = 0;col < GameSetting::BoardSize; col++)
            {
                if (board[row][col] == PLAYER_NONE) // 检查是否是空白位置
                {
                    setBoard(QPoint(row,col), currentPlayer);// 暂时将空白位置设为当前玩家
                    int score = minimax(depth - 1, false); // 递归搜索下一层的MIN节点，注意此处传入false
                    setBoard(QPoint(row,col), PLAYER_NONE); // 恢复空白位置
                    maxScore = std::max(maxScore, score);
                }
            }
        }

        return maxScore;
    }
    else // MIN节点
    {
        int minScore = INT_MAX; // 初始化最小分数为一个较大值

        // 遍历所有合法的空白位置，找到最小分数
        for(int row = 0;row < GameSetting::BoardSize; row++)
        {
            for(int col = 0;col < GameSetting::BoardSize; col++)
            {

                if (board[row][col] == PLAYER_NONE) // 检查是否是空白位置
                {
                    setBoard(QPoint(row,col), (currentPlayer == PLAYER_BLACK) ? PLAYER_WHITE : PLAYER_BLACK);// 暂时将空白位置设为对手玩家
                    int score = minimax(depth - 1, true); // 递归搜索下一层的MAX节点，注意此处传入true
                    setBoard(QPoint(row,col), PLAYER_NONE);// 恢复空白位置

                    minScore = std::min(minScore, score);
                }
            }
        }
        return minScore;
    }
}

int GameBoard::minimax_pruning(int depth, int alpha, int beta, bool maximizingPlayer)
{
    if (depth == 0 || isGameOver()) // 达到搜索深度或游戏结束时，返回当前局面的评估分数
    {
        if (hashTable.contains(zobristHash.hash) && hashTable[zobristHash.hash].second >= depth)
        {
            return hashTable[zobristHash.hash].first;
        }
        int score = evaluateBoard(currentPlayer);
        hashTable[zobristHash.hash] = qMakePair(score, depth);
        return hashTable[zobristHash.hash].first;
    }

    if (maximizingPlayer) // MAX节点
    {
        int maxScore = -INT_MAX; // 初始化最大分数为一个较小值

        // 遍历所有合法的空白位置，找到最大分数


        for(int row = 0;row < GameSetting::BoardSize; row++)
        {
            for(int col = 0;col < GameSetting::BoardSize; col++)
            {
                if (board[row][col] == PLAYER_NONE) // 检查是否是空白位置
                {
                    setBoard(QPoint(row,col), currentPlayer);// 暂时将空白位置设为当前玩家
                    int score = minimax_pruning(depth - 1, alpha, beta, false); // 递归搜索下一层的MIN节点，注意此处传入false
                    setBoard(QPoint(row,col), PLAYER_NONE);// 恢复空白位置


                    maxScore = std::max(maxScore, score);
                    alpha = std::max(alpha, maxScore);

                    if (beta <= alpha){
                        break; // Beta剪枝
                    }
                }
            }
        }

        return maxScore;
    }
    else // MIN节点
    {
        int minScore = INT_MAX; // 初始化最小分数为一个较大值

        // 遍历所有合法的空白位置，找到最小分数

        for(int row = 0;row < GameSetting::BoardSize; row++)
        {
            for(int col = 0;col < GameSetting::BoardSize; col++)
            {
                if (board[row][col] == PLAYER_NONE) // 检查是否是空白位置
                {
                    setBoard(QPoint(row,col), (currentPlayer == PLAYER_BLACK) ? PLAYER_WHITE : PLAYER_BLACK); // 暂时将空白位置设为对手玩家
                    int score = minimax_pruning(depth - 1, alpha, beta, true); // 递归搜索下一层的MAX节点，注意此处传入true
                    setBoard(QPoint(row,col), PLAYER_NONE); // 恢复空白位置


                    minScore = std::min(minScore, score);
                    beta = std::min(beta, minScore);

                    if (beta <= alpha){
                        break; // Alpha剪枝
                    }
                }
            }
        }

        return minScore;
    }
}

int GameBoard::evaluateBoardLine(QPoint p1, QPoint p2,QPoint p3, Player evaluatePlayer)
{
    int countWhite = 0;
    int countBlack = 0;
    int score = 0;
    if(board[p1.x()][p1.y()] == PLAYER_WHITE) countWhite ++;
    if(board[p2.x()][p2.y()] == PLAYER_WHITE) countWhite ++;
    if(board[p3.x()][p3.y()] == PLAYER_WHITE) countWhite ++;
    if(board[p1.x()][p1.y()] == PLAYER_BLACK) countBlack ++;
    if(board[p2.x()][p2.y()] == PLAYER_BLACK) countBlack ++;
    if(board[p3.x()][p3.y()] == PLAYER_BLACK) countBlack ++;
    if(countWhite == 3 || countBlack == 3) score += (countBlack == 3)?100:-100;
    else if(countBlack + countWhite == 3) score += 0;
    else if(countBlack == 2 || countWhite == 2) score += (countBlack == 2)?30:-30;
    else if(countBlack + countWhite == 2) score += 5;
    else if(countBlack == 1) score += 10;
    else if(countWhite == 1) score += -10;
    else score += 1;

    if(evaluatePlayer == PLAYER_BLACK) return score;
    return -score;

}

int GameBoard::evaluateBoard(Player evaluatePlayer)
{
    int score = 0;

    if (currentPlayer == PLAYER_NONE) return score;

    evaluateBoardTimesCurrentTurn ++;

    //评估8条线
    for(int i = 0;i < GameSetting::BoardSize;i ++){
        score+=evaluateBoardLine(QPoint(0,i),QPoint(1,i),QPoint(2,i),evaluatePlayer);
        score+=evaluateBoardLine(QPoint(i,0),QPoint(i,1),QPoint(i,2),evaluatePlayer);
    }
    score+=evaluateBoardLine(QPoint(0,0),QPoint(1,1),QPoint(2,2),evaluatePlayer);
    score+=evaluateBoardLine(QPoint(2,0),QPoint(1,1),QPoint(0,2),evaluatePlayer);


    return score;
}

//QPoint GameBoard::calculateAIMove()
//{
//    double maxScore = - INT_MAX; // 初始化最大分数为一个较小值
//    QPoint bestMove(-1,-1);


//    if(!qlearning->qTable[zobristHash.hash].empty()){
//        qDebug() <<QString::number( qlearning->qTable[zobristHash.hash].size());
//        for (auto it = qlearning->qTable[zobristHash.hash].constBegin(); it != qlearning->qTable[zobristHash.hash].constEnd(); ++it) {
//            if(it.value() > maxScore) {
//                bestMove = it.key();
//                maxScore = it.value();// 下一个状态的最大Q值
//            }
//        }

//    }
//    else{
//        qDebug() << "not find state";
//    }


//    qDebug() << "putting place:(" + QString::number(bestMove.x()) + "," + QString::number(bestMove.y()) + ") : "
//                    + QString::number(maxScore);

//    qDebug() << "-----------------putting place end-------------";

//    return bestMove; // 返回最优的移动位置
//}
