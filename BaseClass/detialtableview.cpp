#include "detialtableview.h"
#include <QScrollBar>
#include <QScroller>
#include <QHeaderView>
#include "Kit/LogInfo/clog.h"

DetialTableview::DetialTableview(QWidget *parent)
{
    //this->installEventFilter(this);
    QHeaderView * hor = this->horizontalHeader();
    hor->setVisible(false);
    QHeaderView * ver = this->verticalHeader();
    ver->setVisible(false);
    this->setShowGrid(false);
    this->setStyleSheet("QTableView, QHeaderView, QTableView::item {background: white;} QTableView::item:selected { /*被选中的index*/color: black;background: white;}");
    //this->setDisabled(true);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFocusPolicy(Qt::NoFocus); // 取消Item选中后的虚线边框
    this->setEditTriggers(QAbstractItemView::NoEditTriggers); // 设置不可编辑
    this->setMouseTracking(true);
    QScroller * vterbar = QScroller::scroller(this);
    vterbar->grabGesture(this,QScroller::TouchGesture);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    //this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

