#ifndef BUDGETSUBPAGE_H
#define BUDGETSUBPAGE_H

#include <QWidget>

namespace Ui {
class BudGetSubPage;
}

class BudGetSubPage : public QWidget
{
    Q_OBJECT

public:
    explicit BudGetSubPage(QWidget *parent = nullptr);
    ~BudGetSubPage();

private:
    QWidget * m_MonthlBudgetWidget;    //月预算界面widget
    QWidget * m_AnnualBudgetWidget;    //年预算界面widget

    Ui::BudGetSubPage *ui;
};

#endif // BUDGETSUBPAGE_H
