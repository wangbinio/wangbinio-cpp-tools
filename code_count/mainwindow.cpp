#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QDebug>

bool kCount3rd = false;

bool kCountUi = false;

void code_count(const QString& path, int& lines) {
    QDir dir(path);
    auto subs = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    for (const auto& sub : subs) {
        if (sub.isFile()) {
            auto filename = sub.fileName();
            if (!kCountUi && filename.endsWith(".ui"))
                continue;

            if (filename.endsWith(".h") || filename.endsWith(".hpp") || filename.endsWith(".c")
                || filename.endsWith(".cc") || filename.endsWith(".cpp")
                || filename.endsWith(".inl") || filename.endsWith(".ui")
                || filename.endsWith(".qss")) {
                QFile file(sub.absoluteFilePath());
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    continue;
                while(!file.atEnd()) {
                    file.readLine();
                    ++lines;
                }
            }
        } else if(!sub.fileName().contains(".git") && (kCount3rd || !sub.fileName().contains("3rdParty"))){
            code_count(sub.absoluteFilePath(), lines);
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_cnt_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:/",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_path->setText(path);

    int lines = 0;
    kCount3rd = ui->checkBox->isChecked();
    kCountUi = ui->checkBox_2->isChecked();
    code_count(path, lines);

    ui->lineEdit_total->setText(QString::number(lines));

    ui->textEdit->append(path + " code lines = " + QString::number(lines) + "\n");
}

void MainWindow::on_pushButton_clicked()
{
    auto path = ui->lineEdit_path->text();

    int lines = 0;
    kCount3rd = ui->checkBox->isChecked();
    kCountUi = ui->checkBox_2->isChecked();
    code_count(path, lines);

    ui->lineEdit_total->setText(QString::number(lines));

    ui->textEdit->append(path + " code lines = " + QString::number(lines) + "\n");
}
