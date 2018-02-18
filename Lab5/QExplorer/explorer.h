#ifndef EXPLORER_H
#define EXPLORER_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class Explorer;
}

class Explorer : public QDialog
{
    Q_OBJECT

public:
    explicit Explorer(QWidget *parent = 0);
    ~Explorer();

private:
    Ui::Explorer *ui;
    QTimer timer;
    bool getSysMes;
private:
    void refreshData();
    void changeTab(int index);
    void updateBase();
    void updateProcess();
    void killProcess();
    void updateSys();
};

#endif // EXPLORER_H
