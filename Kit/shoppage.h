#ifndef SHOPPAGE_H
#define SHOPPAGE_H

#include <QWidget>

namespace Ui {
class ShopPage;
}

class ShopPage : public QWidget
{
    Q_OBJECT

public:
    explicit ShopPage(QWidget *parent = nullptr);
    ~ShopPage();

protected:
    bool requestPermission();                   //请求网络权限
    QString parsingData(QByteArray * data);     //解析数据
    QString mergeData(QJsonObject object_data); //合并数据

public slots:
    void onQueryButtonClicked();                //当查询按钮被点击是的槽函数
    void onRecvMsg(QByteArray data, QObject * obj); //当收到消息时的槽函数

private:
    Ui::ShopPage *ui;
};

#endif // SHOPPAGE_H
