#ifndef HTCCHARTDATASET_H
#define HTCCHARTDATASET_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <algorithm>
#include <QFileInfo>

class HTCChartDataSet
{

public:
    explicit HTCChartDataSet(QObject *parent = nullptr);

    void SetData(QStringList data);
    void SetChartTitle(QString title);
    void SetModel(QString model);
    void SetSerial(QString serial);
    void SetXAxisTitle(QString title);
    void SetYAxisTitle(QString title);
    void SetXAxisScale(QString scale);
    void SetYAxisScale(QString scale);
    void SetSampleFileName(QString file);
    void SetDataSetIndex(int value);

    // new CI parts
    int getDataType();
    void setDataType(int dataType);
    void setBaseFolder(QString baseFolder);


    QStringList GetData();
    QString GetChartTitle();
    QString GetModel();
    QString GetSerial();
    QString GetXAxisTitle();
    QString GetYAxisTitle();
    QString GetXAxisScale();
    QString GetYAxisScale();
    QString GetSampleFileName();
    int GetDataSetIndex();
    bool GetInitializedOK();
    QStringList GetFilesperRangeAreGood();
    int GetFilesPerRangeIsGood(int rangeID);
    void SetFilesPerRangeIsGood(int RangeID, int value);
    QString GetBaseFolder();

    // -------------------------------- //
    // comm check data
    // -------------------------------- //
    bool GetIsCommCheckData();
    // -------------------------------- //
    // comm check data
    // -------------------------------- //


signals:

public slots:


private:
    QStringList _data;
    QString _chartTitle;
    QString _model;
    QString _serial;
    QString _xAxisTitle;
    QString _yAxisTitle;
    QString _xAxisScaling = "LIN";
    QString _yAxisScaling = "LIN";
    bool _initializedOK;
    QString _sampleFileName;
    int _datasetIDX;
    QStringList _numFilesperRangeIsGood;


    void setInitializedOKState();

    // new CI parts
    int RIdataType = 0;
    int CIdataType = 1;

    int _dataType = -1;
    QString _baseFolder;

    // debug parts
    void listThisList(QStringList list);

    // -------------------------------- //
    // comm check data
    // -------------------------------- //
    QList<double> _testValues;
    bool _isCommCheckData;

    void SetCommCheckAutoDetect();


    bool IsCommCheckData(QStringList commCkData); //main call
    QString getFileDelim();

    double getMin(QList<double> values);
    double getMax(QList<double> values);
    double getMean(QList<double> values);
    QStringList getMasterList();
    QString getShortenedParts(QString target, QString delim);

    bool ThisLineIsCommCk(QString target, QString del);
    QList<double> ConvertToDoubleList(QString target, QString delim);
    // -------------------------------- //
    // comm check data
    // -------------------------------- //

};

#endif // HTCCHARTDATASET_H
