ifndef ADDSCORE_H
#define ADDSCORE_H

#include <QMainWindow>

namespace Ui {
class addScore;
}

class addScore : public QMainWindow
{
    Q_OBJECT

public:
    explicit addScore(QWidget *parent = nullptr);
    ~addScore();

signals:
        void giveBoolScore(bool changeScore);

private slots:

    void on_yesButton_clicked();

    void on_noButton_clicked();

private:
    Ui::addScore *ui;
};

#endif // ADDSCORE_H
