#ifndef OUTPUTVIEW_H
#define OUTPUTVIEW_H

#include "interpreter.h"

#include <QMainWindow>

namespace Ui {
class OutputView;
}

class OutputView : public QMainWindow{
    Q_OBJECT
    Interpreter* interpreter;
public:
    explicit OutputView(QWidget *parent = nullptr);
    ~OutputView();

    void setInterpreter(Interpreter*);
    void setValues();

private slots:
    void on_pushButton_clicked();

private:
    Ui::OutputView *ui;
};

#endif // OUTPUTVIEW_H
