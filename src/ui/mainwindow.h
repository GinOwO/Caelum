#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "interpreter.h"

#include "outputview.h"

#include <vector>
#include <map>
#include <string>

#include <QFile>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionNew_triggered();

    void on_actionSaveAs_triggered();

    void on_actionFind_triggered();

    void on_actionFindNext_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionMaterial_Dark_triggered();

    void on_actionDark_triggered();

    void on_actionLight_triggered();

    void on_actionBuild_triggered();

    void on_actionRun_triggered();

private:
    Ui::MainWindow *ui;
    QMessageBox saveOnClose;
    QString filePath="";
    QString searchTerm="";
    QFile* fileHandle=nullptr;
    std::vector<QString> themes;
    bool fileStatus=false;
    Interpreter* interpreter;
    OutputView output;

    void openFile(int=0);
    void closeFile();
    void writeFile();
    void readFile();
    void find();
    void setTheme(int);

    void dispOutput();
};
#endif // MAINWINDOW_H
