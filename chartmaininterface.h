#ifndef CHARTMAININTERFACE_H
#define CHARTMAININTERFACE_H

#include <QWidget>

namespace Ui {
class ChartMainInterface;
}

class ChartMainInterface : public QWidget
{
    Q_OBJECT

public:
    explicit ChartMainInterface(QWidget *parent = nullptr);
    ~ChartMainInterface();

private:
    Ui::ChartMainInterface *ui;
};

#endif // CHARTMAININTERFACE_H
