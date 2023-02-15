#ifndef BILLSUBAREA_H
#define BILLSUBAREA_H

#include <QWidget>

namespace Ui {
class BillSubArea;
}

class BillSubArea : public QWidget
{
    Q_OBJECT

public:
    explicit BillSubArea(QWidget *parent = nullptr);
    ~BillSubArea();

private:
    Ui::BillSubArea *ui;
};

#endif // BILLSUBAREA_H
