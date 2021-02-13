#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>

//class QList;
class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = nullptr);
    Q_INVOKABLE QList<double> calc(double, double, double, double);

signals:

};

#endif // CALCULATOR_H
