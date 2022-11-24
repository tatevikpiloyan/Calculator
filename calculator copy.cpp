#include "calculator.h"
#include "./ui_calculator.h"

Calculator::Calculator(QWidget *parent) : QMainWindow(parent), ui(new Ui::Calculator)
{
    ui->setupUi(this);
    ui->Display->setText(QString::number(finalSum));
    for (int i = 0; i < 10; ++i)
    {
        QString butName = "Button" + QString::number(i);
        digitButtons[i] = Calculator::findChild<QPushButton*>(butName);
        connect(digitButtons[i], SIGNAL(clicked(bool)), this, SLOT(digitClicked()));
    }
    connect(Calculator::findChild<QPushButton*>("AddButton"), SIGNAL(clicked(bool)), this, SLOT(additiveOperatorClicked()));
    connect(Calculator::findChild<QPushButton*>("SubButton"), SIGNAL(clicked(bool)), this, SLOT(additiveOperatorClicked()));
    connect(Calculator::findChild<QPushButton*>("MulButton"), SIGNAL(clicked(bool)), this, SLOT(multiplicativeOperatorClicked()));
    connect(Calculator::findChild<QPushButton*>("DivButton"), SIGNAL(clicked(bool)), this, SLOT(multiplicativeOperatorClicked()));
    connect(Calculator::findChild<QPushButton*>("ChangeSignButton"), SIGNAL(clicked(bool)), this, SLOT(changeSignClicked()));
    connect(Calculator::findChild<QPushButton*>("PercentButton"), SIGNAL(clicked(bool)), this, SLOT(percentageClicked()));
    connect(Calculator::findChild<QPushButton*>("EqualButton"), SIGNAL(clicked(bool)), this, SLOT(equalButton()));
    connect(Calculator::findChild<QPushButton*>("ClearButton"), SIGNAL(clicked(bool)), this, SLOT(clearAll()));

}

void Calculator::digitClicked()
{
    QPushButton* clickedButton = static_cast<QPushButton*>(sender());
    int digitValue = clickedButton->text().toInt();
    if (ui->Display->text() == "0" && digitValue == 0.0)
    {
        ui->Display->setText(QString::number(digitValue));
    }
    if (waitingForOperand)
    {
        ui->Display->clear();
        waitingForOperand = false;
    }
    ui->Display->setText(ui->Display->text() + QString::number(digitValue));
}

void Calculator::additiveOperatorClicked()
{
    QPushButton* clickedButton = static_cast<QPushButton*>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = ui->Display->text().toDouble();
    if (!pendingMultiplicativeOperator.isEmpty())
    {
        if (!calculate(operand, pendingMultiplicativeOperator))
        {
            abortOperation();
            return;
        }
        ui->Display->setText(QString::number(tempSum));
        operand = tempSum;
        tempSum = 0.0;
        pendingMultiplicativeOperator.clear();
    }
    if (!pendingAdditiveOperator.isEmpty())
    {
        if (!calculate(operand, pendingAdditiveOperator))
        {
            abortOperation();
            return;
        }
        ui->Display->setText(QString::number((finalSum)));
    }
    else
    {
        finalSum = operand;
    }
    pendingAdditiveOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::multiplicativeOperatorClicked()
{
    QPushButton* clickedButton = static_cast<QPushButton*>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = ui->Display->text().toDouble();
    if (!pendingMultiplicativeOperator.isEmpty())
    {
        if (!calculate(operand, pendingMultiplicativeOperator))
        {
            abortOperation();
            return;
        }
        ui->Display->setText(QString::number(tempSum));
        pendingMultiplicativeOperator.clear();

    }
    else
    {
        tempSum = operand;
    }
    pendingMultiplicativeOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::abortOperation()
{
    ui->Display->setText("ERROR");
}

void Calculator::equalButton()
{
    double operand = ui->Display->text().toDouble();
    if (!pendingMultiplicativeOperator.isEmpty())
    {
        if (!calculate(operand, pendingMultiplicativeOperator))
        {
            abortOperation();
            return;
        }
        operand = tempSum;
        tempSum = 0.0;
        pendingMultiplicativeOperator.clear();
    }
    if (!pendingAdditiveOperator.isEmpty())
    {
        if (!calculate(operand, pendingAdditiveOperator))
        {
            abortOperation();
            return;
        }
        pendingAdditiveOperator.clear();
    }
    else
    {
        finalSum = operand;
    }
    ui->Display->setText(QString::number(finalSum));
    finalSum = 0.0;
    waitingForOperand = true;
}

void Calculator::changeSignClicked()
{
    ui->Display->setText(QString::number(ui->Display->text().toDouble() * -1));
}

void Calculator::percentageClicked()
{
    ui->Display->setText(QString::number(ui->Display->text().toDouble() / 100));
}

bool Calculator::calculate(double rightOperand, const QString& pendingOperator)
{
    if (pendingOperator == tr("+"))
    {
        finalSum += rightOperand;
    }
    else if (pendingOperator == tr("-"))
    {
        finalSum -= rightOperand;
    }
    else if (pendingOperator == tr("x"))
    {
        tempSum *= rightOperand;
    }
    else if (pendingOperator == tr("/"))
    {
        if (rightOperand == 0.0)
        {
            return false;
        }
        tempSum /= rightOperand;
    }
    return true;
}

void Calculator::clearAll()
{
    tempSum = 0.0;
    finalSum = 0.0;
    pendingAdditiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    ui->Display->setText("0");
    waitingForOperand = true;
}

Calculator::~Calculator()
{
    delete ui;
}

