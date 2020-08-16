#ifndef HTCCHARTDATASET_H
#define HTCCHARTDATASET_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <algorithm>
#include <QFileInfo>
#include <QSettings>
#include "math.h"

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
    QStringList GetHeaderList();

    // -------------------------------- //
    // comm check data
    // -------------------------------- //
    bool GetIsCommCheckData();
    // -------------------------------- //
    // comm check data
    // -------------------------------- //
    // Analog min/max values
    //
    bool GetAnalogMaxMinValuesSet();
    double GetAnalogYMaxValue();
    double GetAnalogYMinValue();
    double GetAnalogXMinValue();
    double GetAnalogXMaxValue();

    // --------------------------------- //
    //
    //
    // --------------------------------- //
    bool GetDataIsUnderRange();
    double GetRangeMultiplier();
    // --------------------------------- //
    //
    //
    // --------------------------------- //




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
    QString _header = "";
    QStringList _headerList;


    void setInitializedOKState();

    // new CI parts
    int RIdataType = 0;
    int CIdataType = 1;

    int _dataType = -1;
    QString _baseFolder;

    // debug parts
    void listThisStringList(QStringList list);
    void listThisList(QList<double> list);

    void listMasterList();

    // -------------------------------- //
    // comm check data
    // -------------------------------- //
    QList<double> _testValues;
    bool _isCommCheckData;
    int _commCheckAutoDetect;

    void SetCommCheckAutoDetect();
    void loadSettings();

    //
    // -------------------------------- //



    bool IsCommCheckData(QStringList commCkData); //main call
    QString getFileDelim();

    double getMin(QList<double> values);
    double getMax(QList<double> values);
    double getMean(QList<double> values);


    QStringList _ProcessedDataList;

    QStringList getMasterList();
    QString getShortenedParts(QString target, QString delim);

    bool ThisLineIsCommCk(QString target, QString del);
    QList<double> ConvertToDoubleList(QString target, QString delim);
    // -------------------------------- //
    // comm check data
    // -------------------------------- //

    // -------------------------------- //
    // Analog min max data
    // -------------------------------- //
    void SetAnalogMinMax();
    // -------------------------------- //
    // aug-14-2020
    void SetBinaryMinMax();
    // -------------------------------- //
    void FillMinMaxLists();
    void FlushMinMaxLists();
    void SetFreqValues();
    double getSingleValueDelimString(QString target, QString delim, int position);

    // -------------------------------- //
    // aug-14-2020
    double _defaultBinaryYMin = -0.5;
    double _defaultBinaryYMax = 2.5;

    double _binaryYMax;
    double _binaryYMin;


    // -------------------------------- //

    // -------------------------------- //
    //
    // New underRange check stuff
    //
    // -------------------------------- //
    bool _dataSetIsUnderRange;
    double _lowestAmplitudePlotable = 1.0E-12;
    double _lowestAmplitudeMultiplier = 1;

    bool setDataIsUnderRange(double limit, double testValue);
    //bool SetNeedsFactoring(double limit, double testValue);;
    // -------------------------------- //


    double _dataSetYMinValue;
    double _dataSetYMaxValue;
    double _dataSetXMinValue;
    double _dataSetXMaxValue;
    bool _analogMinMaxValuesSet = false;

    QList<double> _minValuesList;
    QList<double> _maxValuesList;
    QList<double> _freqList;
    // -------------------------------- //
    // Analog min max data
    // -------------------------------- //

    void setHeader();
    void setHeaderList(QString target, QString delim);

};

#endif // HTCCHARTDATASET_H
