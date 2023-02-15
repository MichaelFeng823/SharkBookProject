#ifndef CHANGEPASSWORDFORM_H
#define CHANGEPASSWORDFORM_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class ChangePassWordForm;
}

class ChangePassWordForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChangePassWordForm(QWidget *parent = nullptr);
    ~ChangePassWordForm();
public slots:
    void onButtonSureClicked();
    void onButtonCancelClicked();
private:
      bool eventFilter(QObject *obj, QEvent *event);
private:
    QSqlDatabase coon;
    bool hasConnected;
    Ui::ChangePassWordForm *ui;
};

#endif // CHANGEPASSWORDFORM_H
