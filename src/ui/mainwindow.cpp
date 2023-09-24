#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <algorithm>

#include <QDir>
#include <QFile>
#include <QFont>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QMainWindow>
#include <QInputDialog>
#include <QApplication>

const std::vector<std::string> _themes{":/themes/qss/MaterialDark.qss", ":/themes/qss/dark.qss",":/themes/qss/light.qss"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    saveOnClose.setWindowTitle("Close File");
    saveOnClose.setText("Are you sure you want to close the file?");
    saveOnClose.setStandardButtons(QMessageBox::Cancel | QMessageBox::Discard);
    saveOnClose.addButton("Save and close", QMessageBox::AcceptRole);
    saveOnClose.setDefaultButton(QMessageBox::Cancel);
    ui->textEdit->setFontPointSize(12);
    for(auto&c:_themes){
        QFile handle(c.c_str());
        handle.open(QFile::ReadOnly | QFile::Text);
        this->themes.push_back(handle.readAll());
        handle.close();
    }
    this->setTheme(1);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_actionUndo_triggered(){
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered(){
    ui->textEdit->redo();
}


void MainWindow::on_actionCut_triggered(){
    ui->textEdit->cut();
}

void MainWindow::on_actionCopy_triggered(){
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered(){
    ui->textEdit->paste();
}


void MainWindow::on_actionOpen_triggered(){
    this->on_actionNew_triggered();
    this->readFile();
}


void MainWindow::on_actionSave_triggered(){
    if(filePath.isEmpty()) this->on_actionSaveAs_triggered();
    else this->writeFile();
}

void MainWindow::on_actionSaveAs_triggered(){
    QString newFilePath = QFileDialog::getSaveFileName(
        this, "Save File As", QDir::currentPath(), "ASM Files (*.asm);;All Files (*)");

    if(!newFilePath.isEmpty()){
        filePath = newFilePath;
        this->writeFile();
    }
}

void MainWindow::on_actionNew_triggered(){
    if(ui->textEdit->document()->isModified())
    switch(saveOnClose.exec()){
    case QMessageBox::AcceptRole:
        this->on_actionSave_triggered();
    case QMessageBox::Discard:
        this->closeFile();
    case QMessageBox::Cancel:
    default: break;
    }
    ui->textEdit->clear();
}

void MainWindow::openFile(int mode){
    if(this->filePath.isEmpty())
        this->filePath = QFileDialog::getOpenFileName(this, "Open File",
            QDir::currentPath(), "ASM Files (*.asm);;All Files (*)");

    this->fileHandle = new QFile(filePath);
    if(mode) this->fileStatus = this->fileHandle->open(QIODevice::WriteOnly | QIODevice::Text);
    else this->fileStatus = this->fileHandle->open(QIODevice::ReadOnly|QIODevice::Text);
}

void MainWindow::readFile(){
    this->openFile();
    if(!this->fileStatus){
        QMessageBox::critical(this, "Error", "Failed to open/read the file.");
        return;
    }
    QTextStream in(fileHandle);
    QString fileContent = in.readAll();
    ui->textEdit->setPlainText(fileContent);
    this->closeFile();
}

void MainWindow::writeFile(){
    this->openFile(1);
    if(!this->fileStatus){
        QMessageBox::critical(this, "Error", "Failed to open/write to the file.");
        return;
    }
    QTextStream out(fileHandle);
    QString fileContent = ui->textEdit->toPlainText();
    out << fileContent << '\n';
    this->closeFile();
}

void MainWindow::closeFile(){
    if(this->fileStatus) this->fileHandle->close();
    delete this->fileHandle;
    this->fileHandle = nullptr;
    this->fileStatus=false;
}

void MainWindow::find(){
    if(this->searchTerm.isEmpty())
        this->searchTerm = QInputDialog::getText(this, "Find", "Enter text to find:");
    QTextCursor cursor(ui->textEdit->document());
    cursor = ui->textEdit->document()->find(searchTerm, cursor);
    if (!cursor.isNull()){
        cursor.movePosition(QTextCursor::WordRight, QTextCursor::KeepAnchor);
        QTextCharFormat format;
        format.setBackground(Qt::yellow);
        cursor.mergeCharFormat(format);
    }
    else{
        cursor = ui->textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        ui->textEdit->setTextCursor(cursor);
    }
}

void MainWindow::on_actionFindNext_triggered(){
    this->find();
}

void MainWindow::on_actionFind_triggered(){
    this->searchTerm = "";
    this->on_actionFindNext_triggered();
}

void MainWindow::on_actionZoom_In_triggered(){
    if(ui->textEdit->fontPointSize()<47){
        ui->textEdit->setFontPointSize(ui->textEdit->fontPointSize()+2);
        ui->textEdit->setText(ui->textEdit->document()->toPlainText());
    }
}

void MainWindow::on_actionZoom_Out_triggered(){
    if(ui->textEdit->fontPointSize()>13) ui->textEdit->setFontPointSize(ui->textEdit->fontPointSize()-2);
    ui->textEdit->setText(ui->textEdit->document()->toPlainText());
}

void MainWindow::setTheme(int k){
    switch(k){
    default:
    case 0: this->setStyleSheet(this->themes[0]); break;
    case 1: this->setStyleSheet(this->themes[1]); break;
    case 2: this->setStyleSheet(this->themes[2]); break;
    }
}

void MainWindow::on_actionMaterial_Dark_triggered(){
    this->setTheme(1);
}

void MainWindow::on_actionDark_triggered(){
    this->setTheme(0);
}

void MainWindow::on_actionLight_triggered(){
    this->setTheme(2);
}
