#ifndef ASSETMANAGERPAGE_H
#define ASSETMANAGERPAGE_H

#include <QWidget>

namespace Ui {
class AssetManagerPage;
}

class AssetManagerPage : public QWidget
{
    Q_OBJECT

public:
    explicit AssetManagerPage(QWidget *parent = nullptr);
    ~AssetManagerPage();

private:
    Ui::AssetManagerPage *ui;
};

#endif // ASSETMANAGERPAGE_H
