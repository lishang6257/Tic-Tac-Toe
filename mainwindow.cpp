// mainwindow.cpp
#include "mainwindow.h"
#include "utils/util.h"

#include <QGridLayout>
#include <QPainter>
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    activatePosition = QPoint(-1,-1);
    boardButtons.clear();
    createGameBoardUI();

    gameBoard.resetBoard();

    connect(&gameBoard, &GameBoard::boardChanged, this, &MainWindow::updateBoard);

    //         AI Caluate & Move
    Player currentPlayer = gameBoard.getCurrentPlayer();
    QPoint aip =gameBoard.calculateAIMove();
    if(gameBoard.makeMove(aip))
        boardButtons[aip]->setPlayer(currentPlayer);
    else
    {
        qDebug() << "AI cannot find aip";
    }


    update();

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // 获取棋盘格子大小和棋盘边界
    int cellSize = GameSetting::CellSize;
    int boardSize = GameSetting::BoardSize;
    int boardWidth = boardSize * cellSize;
    int boardHeight = boardSize * cellSize;

    // 创建绘图对象
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

    painter.setPen(Qt::black); // 设置画笔颜色为黑色

    // 绘制棋盘网格
    for (int row = 0; row < boardSize+1; ++row) {
        for (int col = 0; col < boardSize+1; ++col) {
            int x =  col * cellSize;
            int y =  row * cellSize;

            // 绘制棋盘的横线和竖线
            painter.drawLine(x, 0, x, boardHeight); // 绘制竖线
            painter.drawLine(0, y, boardWidth, y); // 绘制横线
        }
    }
    // 显示激活的棋子
    if(activatePosition != QPoint(-1,-1)){
        painter.setPen(Qt::red);
        painter.drawText(QPoint(0,cellSize*activatePosition.x()),QString::number(activatePosition.x()));
        painter.drawText(QPoint(cellSize*activatePosition.y() ,0),QString::number(activatePosition.y()));

    }


}

void MainWindow::createGameBoardUI(int cellSize,int boardSize)
{
    int boardWidth = boardSize * cellSize;
    int boardHeight = boardSize * cellSize ;

    for (int row = 0; row < GameSetting::BoardSize; ++row) {
        for (int col = 0; col < GameSetting::BoardSize; ++col) {
            QPushHoverButton* button = new QPushHoverButton(this);

            button->setGeometry(col * cellSize,row * cellSize, cellSize, cellSize);
            button->setFixedSize(cellSize, cellSize);
            button->setStyleSheet("border: none;opacity: 1;text-align: center; padding: 0px; margin: 0px;");

            connect(button, &QPushHoverButton::clicked,this, &MainWindow::handleCellClicked);
            connect(button, &QPushHoverButton::enter,this, &MainWindow::handleCellEnter);

            boardButtons[QPoint(row,col)] = button;

        }
    }

    setContentsMargins(0, 0, 0, 0);

    // 设置主窗口的固定大小为棋盘的大小
    setFixedSize(boardWidth, boardHeight);
}

void MainWindow::handleCellEnter()
{
    QPushHoverButton* clickedButton = qobject_cast<QPushHoverButton*>(sender());
    activatePosition = boardButtons.key(clickedButton, QPoint(-1, -1));
//    qDebug() << "enter :" + QString::number(activatePosition.x()) + "," + QString::number(activatePosition.y());

    update();
}

void MainWindow::handleCellClicked()
{
    QPushHoverButton* clickedButton = qobject_cast<QPushHoverButton*>(sender());
    if (clickedButton && clickedButton->isCheckable())
    {
        if(gameBoard.isGameOver()) return;
        QPoint position = boardButtons.key(clickedButton, QPoint(-1, -1));

        Player currentPlayer = gameBoard.getCurrentPlayer();
        if(gameBoard.makeMove(position))
            clickedButton->setPlayer(currentPlayer);


//        gameBoard.testEvaluateBoardCostTime(50000);

//         AI Caluate & Move
        currentPlayer = gameBoard.getCurrentPlayer();
        QPoint aip =gameBoard.calculateAIMove();
        if(gameBoard.makeMove(aip))
            boardButtons[aip]->setPlayer(currentPlayer);
        else
        {
            qDebug() << "AI cannot find aip";
        }

    }

    //显示 label
    for(int i = 0;i < gameBoard.historyPlayer.size();i ++){
        boardButtons[gameBoard.historyPlayer[i]]->setLabel(i);
    }

    update();
}

void MainWindow::updateBoard(const QPoint& position, Player player)
{

    Q_UNUSED(position);
    Q_UNUSED(player);

//    gameBoard.testEvaluateBoardCostTime(50000);
    //AI Caluate & Move
//    Player currentPlayer = gameBoard.getCurrentPlayer();
//    if(currentPlayer == Player::White)
//    {
//        QPoint aip =gameBoard.calculateAIMove();
//        if(gameBoard.makeMove(aip))
//            boardButtons[aip]->setPlayer(currentPlayer);
//        else{
//            qDebug() << "AI cannot find aip";

//            QVector<QPoint> sps = gameBoard.getSortedSearchSpaceAroundStones(2,true,Player::White);
//            for(const auto&p : sps){
//                boardButtons[p]->setActivate(Qt::darkYellow);
//            }
//        }

//    }
//    //显示 label
//    for(int i = 0;i < gameBoard.historyPlayer.size();i ++){
//        boardButtons[gameBoard.historyPlayer[i]]->setLabel(i);
//    }

    update();
}
