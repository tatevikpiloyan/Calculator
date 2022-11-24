#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "QtWidgets/qpushbutton.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void digitClicked();
    void additiveOperatorClicked();
    void multiplicativeOperatorClicked();
    void equalButton();
    void changeSignClicked();
    void percentageClicked();
    void clearAll();

private:
    double finalSum = 0.0;
    double tempSum = 0.0;
    QString pendingAdditiveOperator = "";
    QString pendingMultiplicativeOperator = "";
    bool waitingForOperand = true;
    QPushButton* digitButtons[10];

private:
    void abortOperation();
    bool calculate(double rightOperand, const QString& pendingOperator);

private:
    Ui::Calculator *ui;
};
#endif // CALCULATOR_H
