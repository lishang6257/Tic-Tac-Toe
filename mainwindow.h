#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "utils/pushhoverbutton.h"
#include "gameboard.h"
#include "utils/util.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void updateBoard(const QPoint& position, Player player);
//    void handleCellClicked(int row, int col);
//    void handleGameFinished(GameBoard::Player winner);

protected:
    void paintEvent(QPaintEvent *event);

private:
    GameBoard gameBoard;
    QPoint activatePosition;

    QHash<QPoint, QPushHoverButton*> boardButtons;

    void createGameBoardUI(int cellSize = GameSetting::CellSize, int boardSize = GameSetting::BoardSize);

    void handleCellClicked();
    void handleCellEnter();
    void handleCellLeave();

    void resetGame();
};

#endif // MAINWINDOW_H
