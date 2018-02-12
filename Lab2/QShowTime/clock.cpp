#include "clock.h"

Clock::Clock(QWidget *parent)
{
    setSegmentStyle(Filled);
    setDigitCount(8);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    showTime();

    setWindowTitle(tr("Clock"));
    resize(150, 60);
}

void Clock::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    if ((time.second() % 2) == 0) {
        text[2] = ' ';
        text[5] = ' ';
    }
    display(text);
}
