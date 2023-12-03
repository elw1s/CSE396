#ifndef XOXAPP_H
#define XOXAPP_H

#include "XOXWidget.h"

class XOXApp : public QWidget {
    Q_OBJECT

public:
    XOXApp(QWidget *parent = nullptr);

private slots:
    void gameFinished(int result);
    void resetTable();

private:
    XOXWidget *gameWidget;
    QPushButton* resetButton;
    QLabel *gameStatusLabel;
};

#endif // XOXAPP_H