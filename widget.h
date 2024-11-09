#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>



QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public:
    void setLabelText(const QString &text);


private slots:
    void on_lineEdit_5_textChanged(const QString &arg1);

    void on_lineEdit_6_textChanged(const QString &arg1);

    void on_lineEdit_7_textChanged(const QString &arg1);

    void on_lineEdit_8_textChanged(const QString &arg1);

    void on_lineEdit_9_textChanged(const QString &arg1);

    void on_codeEnter_clicked();

    void on_pushButton_2_clicked();

    void on_changename_clicked();

    void on_getfile_clicked();

    void on_newpage_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
