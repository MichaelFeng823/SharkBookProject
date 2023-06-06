#ifndef CHARTMASKSUBPAGE_H
#define CHARTMASKSUBPAGE_H

#include <QWidget>
#include "Controler/DataStruct/PublicDataStruct.h"

namespace Ui {
class ChartMaskSubPage;
}

class ChartMaskSubPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChartMaskSubPage(InOrOut type,QWidget *parent = nullptr);
    ~ChartMaskSubPage();
protected:
    void setType(InOrOut type){m_Type = type;};
signals:
    void sendButtonType(InOrOut type);               //设置当前按钮类型-支出/收入
private:
    void paintEvent(QPaintEvent *event) override;  //重写绘画事件
    bool eventFilter(QObject *obj, QEvent *event); //事件过滤器
    void ckeckBudgetType();                        //检查预算类型
    bool m_IsClicked = false;                      //是否按下
private:
    Ui::ChartMaskSubPage *ui;
    InOrOut m_Type;                                 //当前类型
};

#endif // CHARTMASKSUBPAGE_H
