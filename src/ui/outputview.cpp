#include "outputview.h"
#include "ui_outputview.h"

#include <sstream>
#include <regex>
#include <iomanip>

#include <QMessageBox>

namespace Pattern{
std::regex setPattern = std::regex(R"/(\s*(\w+)\s*:\s*(\w+)\s*)/");
std::regex getPattern = std::regex(R"/(\s*get\s+(\w+)\s*:\s*(\w+))/");
}

OutputView::OutputView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OutputView){
    ui->setupUi(this);
}

OutputView::~OutputView(){
    delete ui;
}

void OutputView::setInterpreter(Interpreter* t){
    this->interpreter = t;
}

void OutputView::setValues(){
    std::stringstream& ss = this->interpreter->output;
    std::string s, text;
    while(ss >> s) text+=s+'\n';
    this->ui->outputLog->setText(QString::fromStdString(text));
    for(int i=0; i<6;i++){
        this->ui->registerView->topLevelItem(i)->setText(1,
            QString::fromStdString(std::to_string(this->interpreter->getRegister((i+1)*1000)))
        );
    }
    for(int i=6; i<14;i++){
        this->ui->registerView->topLevelItem(i)->setText(1,
            QString::fromStdString(std::to_string(this->interpreter->getRegister((i+4)*1000)))
        );
    }
    for(int i=0;i<8;i++){
        this->ui->flagView->topLevelItem(i)->setText(1,
            QString::fromStdString(this->interpreter->getFlag(i)?"True":"False")
        );
    }
}

/*
template< typename T >
std::string int_to_hex( T i )
{
  std::stringstream stream;
  stream << "0x"
         << std::setfill ('0') << std::setw(sizeof(T)*2)
         << std::hex << i;
  return stream.str();
}
*/

void OutputView::on_pushButton_clicked(){
    std::string s = this->ui->lineEdit->text().toStdString();
    std::smatch match;
    if(std::regex_search(s, match, Pattern::getPattern)){
        unsigned long long a = std::stoull(match[1], nullptr, 16);
        unsigned long long b = std::stoull(match[2], nullptr, 16);
        if(b<a){
            QMessageBox::critical(this, "Error", "Upperbound cannot be smaller than lower bound");
            return;
        }
        std::stringstream ss;
        for(;a<=b;a++){
            ss << std::hex << std::setfill('0') << std::setw(5)
               << a << " : " << this->interpreter->getMemory(a) << '\n';
        }
        this->ui->textBrowser->setText(QString::fromStdString(ss.str()));
    }
    else{
        std::sregex_iterator it(s.begin(), s.end(), Pattern::setPattern);
        std::sregex_iterator end;
        while(it!=end){
            match = *it;
            unsigned long long a = std::stoull(match[1], nullptr, 16);
            unsigned long long b = std::stoull(match[2], nullptr, 16);
            this->interpreter->setMemory(a, b);
            it++;
        }
        QMessageBox::information(this, "Success", "Values set");
    }
}

