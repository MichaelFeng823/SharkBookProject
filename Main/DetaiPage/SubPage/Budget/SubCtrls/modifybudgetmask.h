#ifndef MODIFYBUDGETMASK_H
#define MODIFYBUDGETMASK_H

#include <QWidget>
#include "Controler/DataStruct/PublicDataStruct.h"

namespace Ui {
class ModifyBudgetMask;
}

class ModifyBudgetMask : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyBudgetMask(TypeBudget Type,QWidget *parent = nullptr);
    ~ModifyBudgetMask();
    void setEditButtonText();
    void setClearButtonText();
    void initSlots();

public slots:
    void onButtonEditClicked();
    void onButtonClearClicked();
    void onButtonCancelClicked();
private:
    void paintEvent(QPaintEvent *event) override;           //重写绘画事件

signals:
    void editBudget(TypeBudget);                                      //编辑预算
    void clearBudget(TypeBudget);                                     //清楚预算
private:
    Ui::ModifyBudgetMask *ui;
    TypeBudget m_BudgetType;
};

#endif // MODIFYBUDGETMASK_H
