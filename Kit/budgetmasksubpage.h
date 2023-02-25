#ifndef BUDGETMASKSUBPAGE_H
#define BUDGETMASKSUBPAGE_H

#include <QWidget>

namespace Ui {
class BudgetMaskSubpage;
}

class BudgetMaskSubpage : public QWidget
{
    Q_OBJECT

public:
    explicit BudgetMaskSubpage(QWidget *parent = nullptr);
    ~BudgetMaskSubpage();

private:
    Ui::BudgetMaskSubpage *ui;
};

#endif // BUDGETMASKSUBPAGE_H
