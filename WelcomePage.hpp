#ifndef WELCOMEPAGE_HPP
#define WELCOMEPAGE_HPP

#include <QMainWindow>

namespace Ui {
class WelcomePage;
}

class WelcomePage : public QMainWindow
{
    Q_OBJECT

public:
    explicit WelcomePage(QWidget *parent = nullptr);
    ~WelcomePage();

private slots:
    void on_next_pressed();

private:
    Ui::WelcomePage *ui;
};

#endif // WELCOMEPAGE_HPP
