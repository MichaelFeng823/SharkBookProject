#ifndef ADDBUDGETPANE_H
#define ADDBUDGETPANE_H

#include <QWidget>

namespace Ui {
class AddBudgetPane;
}

class AddBudgetPane : public QWidget
{
    Q_OBJECT

public:
    explicit AddBudgetPane(QWidget *parent = nullptr);
    ~AddBudgetPane();

private:
    Ui::AddBudgetPane *ui;
};

#endif // ADDBUDGETPANE_H
