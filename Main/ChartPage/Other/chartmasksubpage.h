#ifndef CHARTMASKSUBPAGE_H
#define CHARTMASKSUBPAGE_H

#include <QWidget>

namespace Ui {
class ChartMaskSubPage;
}

class ChartMaskSubPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChartMaskSubPage(QWidget *parent = nullptr);
    ~ChartMaskSubPage();

private:
    Ui::ChartMaskSubPage *ui;
};

#endif // CHARTMASKSUBPAGE_H
