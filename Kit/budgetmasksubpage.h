#ifndef BUDGETMASKSUBPAGE_H
#define BUDGETMASKSUBPAGE_H

#include <QWidget>
#include "Controler/PublicDataStruct.h"

namespace Ui {
class BudgetMaskSubpage;
}

class BudgetMaskSubpage : public QWidget
{
    Q_OBJECT

public:
    explicit BudgetMaskSubpage(TypeBudget typebudget,QWidget *parent = nullptr);
    ~BudgetMaskSubpage();

signals:
    void sendTypeBudget(TypeBudget type);
protected:
    void setTypeBudget(TypeBudget type){m_TypeBudget = type;};

private:
    bool eventFilter(QObject *obj, QEvent *event); //事件过滤器
    void ckeckBudgetType();                        //检查预算类型

private:
    bool m_IsClicked = false;                      //是否被按下
    TypeBudget m_TypeBudget;                       //类型
    Ui::BudgetMaskSubpage *ui;
};

#endif // BUDGETMASKSUBPAGE_H
