#ifndef SUDOKUAPP_H
#define SUDOKUAPP_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

#include "SudokuWidget.h"


class SudokuApp : public QWidget {
    Q_OBJECT

public:
    SudokuApp(QWidget *parent = nullptr);

private:
    SudokuWidget *sudokuWidget;
    QPushButton *solveButton;
    QGridLayout *numberGrid;

private slots:
    void onNumberButtonClicked();
};

#endif // SUDOKUAPP_H