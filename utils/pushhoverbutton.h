#ifndef QPUSHHOVERBUTTON_H
#define QPUSHHOVERBUTTON_H

#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include <QPainter>
#include <QColor>

#include "utils/util.h"

class QPushHoverButton : public QPushButton
{
    Q_OBJECT // 添加 Q_OBJECT 宏

private:
    Player player = PLAYER_NONE;
    bool isHovered;
    bool isActivate;
    QColor activateColor;
    int label;
    bool isShowLabel;

public:
    QPushHoverButton(QWidget* parent = nullptr)
        : QPushButton(parent),
        player(PLAYER_NONE),
        isHovered(false),
        isActivate(false),
        activateColor(Qt::red),
        label(-1),
        isShowLabel(true),
        cellSize(GameSetting::CellSize),
        isEnter(false),
        isLeave(true)
    {
        setChecked(false);
        setCheckable(true);
    }

    QPushHoverButton(const QString& text, QWidget* parent = nullptr)
        : QPushButton(text, parent),
        player(PLAYER_NONE),
        isHovered(false),
        isActivate(false),
        activateColor(Qt::red),
        label(-1),
        isShowLabel(true),
        cellSize(GameSetting::CellSize),
        isEnter(false),
        isLeave(true)
    {
        setChecked(false);
        setCheckable(true);
    }

    bool setPlayer(Player aplayer = PLAYER_NONE){
        isActivate = false;
        if(player == aplayer) return true;
        if(player == PLAYER_NONE){
            //设置为aplayer
            setChecked(true);
            setCheckable(false);

            player = aplayer;

            update();

            return true;
        }
        else if(aplayer == PLAYER_NONE){
            //清空当前棋子
            setChecked(false);
            setCheckable(true);

            player = PLAYER_NONE;

            update();

            return true;
        }

        //当前棋子被占用
        return false;

    }

    bool setActivate(QColor c){
        if(player == PLAYER_NONE){
            //设置为aplayer
            isActivate = true;
            activateColor = c;

            update();
            return true;
        }
        else
            return false;
    }

    bool setLabel(int alabel){
        if(player != PLAYER_NONE){
            //设置为aplayer
            label = alabel;
            update();
            return true;
        }
        else
            return false;
    }

    int cellSize = GameSetting::CellSize;

signals:
    void enter();
    void leave();
    void focusIn();

private:
    bool isEnter;
    bool isLeave;

protected:
//    bool setFixedSize() override{}

    void enterEvent(QEnterEvent* event)override
    {
        Q_UNUSED(event);

        // 在鼠标进入按钮时执行任何操作
        isEnter = true;
        isHovered = true;
        isLeave = false;
        update();
        emit enter();

    }

    void leaveEvent(QEvent* event) override
    {
        Q_UNUSED(event);

        // 在鼠标离开按钮时执行任何操作
        isLeave = true;
        isHovered = false;
        isEnter = false;
        update();
//        emit leave();
    }

    void focusInEvent(QFocusEvent *event) override
    {
        Q_UNUSED(event);

        // 当小部件获得焦点时，会执行此函数

//        emit focusIn();
    }


    void paintEvent(QPaintEvent* event) override
    {
        Q_UNUSED(event);

        // 先调用 QPushButton 的默认绘制
        QPushButton::paintEvent(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(Qt::black,2));
        //处理鼠标悬停
        if (isEnter && !isChecked())
        {
            painter.setBrush(Qt::red);

            int radius = cellSize * 1 / 4;
            painter.drawEllipse(rect().center(), radius, radius);

            isEnter = false;
        }
        // 绘制棋子
        if ((isChecked() && isCheckable()) || !isCheckable())
        {
            if (player == PLAYER_WHITE)
                painter.setBrush(Qt::white);
            else if (player == PLAYER_BLACK)
                painter.setBrush(Qt::black);
            else
                return;

            int radius = cellSize * 3 / 7;
            painter.drawEllipse(rect().center(), radius, radius);

            setCheckable(false);
        }
        // 高亮激活的棋子
        if (isActivate){
            painter.setBrush(activateColor);
            painter.setPen(Qt::NoPen);
            int radius = cellSize * 1 / 4;
            painter.drawEllipse(rect().center(), radius, radius);
        }
        // 显示标签
        if (isShowLabel && label != -1){
            painter.setPen(QPen(Qt::gray,2));
            painter.setFont(QFont("Arial", cellSize/2));
            painter.drawText(rect(), Qt::AlignCenter, QString::number(label));
        }
    }
};


#endif // QPUSHHOVERBUTTON_H
