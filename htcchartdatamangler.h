#ifndef HTCCHARTDATAMANGLER_H
#define HTCCHARTDATAMANGLER_H

#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>

#include "htcchartdatafile.h"
#include "htcchartdataset.h"

#include "htcchart.h"

#include <algorithm>

// ------------------------------------------------- //
//
// June 24 2020
// Changed _maxNumberOfFilesPerRIRange from 9 to 20
// to accomodate larger number of files per range.
//
// -------------------------------------------------- //


class HTCChartDataMangler : public QObject
{
    Q_OBJECT
public:
    explicit HTCChartDataMangler(QObject *parent = nullptr);

    void Init(QStringList fileList, QVector <int> columns, int dataType, QString baseFolder);
    bool GetItitializedDataOK();

    // new CI parts
    int getDataType();


signals:

    void ChartDataSetsReady(QVector <HTCChartDataSet> DataSets);

    void sendMsg(QString msg);

    //emit ChartDataSetsReady(_dataSets);

public slots:


private:

    // debug functions
    void listFiles();
    void listDataFiles();
    void listDataSets();
    void listThisList(QStringList list);

    void listTheFileStates();
    void listTheRangeFileStates();



    int findFirstDataRow(QStringList list, QString delimiter);
    void setDataFileDelim(QString fileName);
    void loadFilesIntoData();
    bool sortFileset();

    void discoverRanges();
    void discoverStartStopRangeIDs();
    int discoverStartRangeID(QString range);
    int discoverStopRangeID(QString range);
    void setNumberOfChartsTobuild();
    void setColumnLists();
    void setFileDelim();

    void getMetrics();
    int getFileSetStatus();

    void showBadFileDataMessage(QString key);
    void showBadFilesperRnageMessage(HTCChartDataSet * dset);


    void setFilesPerRange();
    int getNumberOfFilesPerRange(QString range);

    void setRowCountsStatus();
    int setRowCountStatusByRange(QString range, int rangeID);

    void BuildAllChartDataSets();
    QStringList BuildDataSet(QString columns);

    QStringList getDataSetHeader(QString columns, int startFileNum, int stopFileNum, bool LoadFromFilePositions);
    QStringList getDataChunkByRange(QStringList list, QString columns, int range);

    void setDataSetProperties(HTCChartDataSet * ChartDataSet, QString columns, int chartNumber);

    bool getFilesPerRangeIsValid(HTCChartDataSet * dset);

    bool _initialized = false;
    QStringList _columnSets;
    QStringList _baseFileList;
    QStringList _headerList;
    QStringList _rangeList;
    int _numberOfRanges;
    int _numberOfChartsToBuild;
    QString _dataFileDelim;
    bool _foundBadRange;

    bool _loadHeaderFromFilePositions = true;
    int _maxNumberOfFilesPerRIRange = 20; // normally set to 9
    double _lastFreqInList;

    int FileStartIDX[3] = {-1,-1,-1};
    int FileStopIDX[3] = {-1,-1,-1};
    int NumFilesPerRange[3] = {-1,-1,-1};
    int RowCountsGoodPerRange[3] = {-1,-1,-1};

    QMap <int,QStringList> RowCountsPerFile;

    QVector <int> _selectedColumns;
    QVector <int> _rangeIDList;
    QVector <HTCChartDataFile> _dataFiles;


    //QVector <QStringList> _dataSets;

    QVector  <HTCChartDataSet>  _dataSets;
    HtcChart * testchart;

    int _dataSetIndex = 0;

    // new CI parts
    int RIdataType = 0;
    int CIdataType = 1;

    int _dataType = -1;

    void setDataType(int dataType);
    QString _baseFolder;


};



#endif // HTCCHARTDATAMANGLER_H
