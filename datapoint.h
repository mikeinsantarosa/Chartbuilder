/* File     : acfdatapoint.h
 * Author   : Mike Purtell
 * Date     : Nov 18, 2017
 *
 * Purpose  :
 *
 * Header file for a
 * freq/level data point
 *
 *
 * */
#ifndef DATAPOINT_H
#define DATAPOINT_H
#include <QVector>



class DataPoint
{
public:
    DataPoint();
    DataPoint(double freq);
    DataPoint(double freq, QString firstValue);

    void setFreq(double value);
    void addValue(QString value);
    QString getValueByPosition(int Position);
    int getMemberCount();
    QVector <QString> getYValues();
    QVector <int> getDoublePositions();

    double getFrequency();


private:
    double _freq;
    QVector <QString> _yValues;
    QVector <int> _DoublePositions;


};

#endif // DATAPOINT_H
