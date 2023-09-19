#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;
    QMessageBox saveOnClose;
    QString filePath="";
    QString searchTerm="";
    QFile* fileHandle=nullptr;
    bool fileStatus=false;

    void openFile(int=0);
    void closeFile();
    void writeFile();
    void readFile();
    void find();
};
#endif // MAINWINDOW_H
