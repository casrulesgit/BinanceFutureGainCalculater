#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>

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


void MainWindow::on_toolButton_triggered(QAction *arg1)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open CSV"), "", tr("csv Files (*.csv)"));
    ui->lineEdit->setText(fileName);
}

void MainWindow::on_pushButton_clicked()
{
    QString path = ui->lineEdit->text();
    if (path.isEmpty()) return;

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "error opening file: " << file.error();
    }

    QMap<QString, double> gainChange;

    int counter = 0;
    double totalGain = 0;
    QTextStream in(&file);
    while (!in.atEnd()) {
        counter++;
        QStringList list = in.readLine().split(";");
        if (counter == 1) continue;

        QString date = list.at(0);
        QString type = list.at(1);
        double amount = list.at(2).toDouble();
        QString asset = list.at(3);
        QString symbol = list.at(4);

        if (type == "REALIZED_PNL" || type == "INSURANCE_CLEAR" || type == "COMMISSION" || type == "FUNDING_FEE") totalGain += amount;

        gainChange.insert(date, totalGain);
    }

    QFileInfo info(path);
    QFile totalGainFile(info.absolutePath() + "/totalCapitalGainBinanceFuture-2020.txt");
    if (!totalGainFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Couldn't open file: " + totalGainFile.fileName();
    }

    QTextStream out(&totalGainFile);
    for (auto it = gainChange.begin(); it != gainChange.end(); ++it)
    {
        out << it.key() << ";" << QString::number(it.value()) << "\n";
    }

    out << "Total Gain: " << QString::number(totalGain);

    qDebug() << "Total Gain: " << QString::number(totalGain);

}

void MainWindow::on_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open CSV"), "", tr("csv Files (*.csv)"));
    ui->lineEdit->setText(fileName);
}
