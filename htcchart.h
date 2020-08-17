#ifndef HTCCHART_H
#define HTCCHART_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QXYLegendMarker>
#include <QPen>
#include <QCategoryAxis>
#include <QChart>
#include <QLineSeries>
#include <QDebug>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>
#include <QFont>
#include <QBoxLayout>
#include <QPixmap>
#include <QFileDialog>
#include <QPen>
#include <QRegExp>
#include <QString>
#include <QSettings>
#include <QRect>
#include <QMessageBox>


#include <QFile>
#include <QStringList>
#include <QDesktopServices>
#include <QTextStream>
#include <QString>
#include "htcchartdataset.h"
#include "chartproperties.h"
#include "htcchartdatafile.h"


QT_CHARTS_USE_NAMESPACE

namespace Ui {
class HtcChart;
}

class HtcChart : public QMainWindow
{
    Q_OBJECT

public:
    //explicit HtcChart(QWidget *parent = 0);
    explicit HtcChart(QWidget *parent = 0);
    ~HtcChart();

    QColor ChartTitleTextColor();
    QFont ChartTitleTextFont();
    void setFileToOpen(QString fileName, bool RescaleFreq, QString baseFolder);
    void setChartByDataSet(HTCChartDataSet * ds, bool RescaleFreq);

    // new CI parts
    int getDataType();
    void setDataType(int dataType);

private slots:
    void on_actionProperties_triggered();

    void on_btnClose_clicked();

    void on_btnSaveImage_clicked();

    // Chart Title
    void ChartTitleText(QString title);
    void ChartTitleTextFont(QFont font);
    void ChartTitleTextColor(QColor color);
    // -------------------------------- //
    // Chart X Axis properties
    // -------------------------------- //
    void HTCChartXAxisUnitsText(QString text);
    void HTCChartXAxisUnitsTextFont(QFont font);
    void HTCChartXAxisUnitsTextColor(QColor color);

    // Chart X Axis Labels
    void HTCChartXAxisLabelsTextRotation(qint32 value);
    void HTCChartXAxisLabelsTextFont(QFont font);
    void HTCChartXAxisLabelsTextColor(QColor color);

    // Chart X Scaling
    void HTCChartXAxisLabelsTextScaleMin(double value);
    void HTCChartXAxisLabelsTextScaleMax(double value);

    void HTCChartXMajorThickness(int arg1);
    void HTCChartXMinorThickness(int arg1);

    void HTCChartXAxisMajorTicsColor(QColor color);
    void HTCChartXAxisMinorTicsColor(QColor color);

    void HTCChartXMajorTicsEnabled(bool checked);
    void HTCChartXMinorTicsEnabled(bool checked);

    void HTCChartXMinorTicsValue(double arg1);
    void HTCChartXMajorTicsValue(double arg1);

    void HTCChartXLogChart(bool checked);
    void HTCChartXLinChart(bool checked);

    void HTCChartXDecimalChart(bool checked);
    void HTCChartXScientificChart(bool checked);

    // -------------------------------- //
    // Chart Y Axis properties
    // -------------------------------- //
    void HTCChartYAxisUnitsText(QString text);
    void HTCChartYAxisUnitsTextFont(QFont font);
    void HTCChartYAxisUnitsTextColor(QColor color);

    // Chart Y Axis Labels
    void HTCChartYAxisLabelsTextRotation(qint32 value);
    void HTCChartYAxisLabelsTextFont(QFont font);
    void HTCChartYAxisLabelsTextColor(QColor color);

    // Chart Y Scaling
    void HTCChartYAxisLabelsTextScaleMin(double value);
    void HTCChartYAxisLabelsTextScaleMax(double value);

    void HTCChartYMajorThickness(int arg1);
    void HTCChartYMinorThickness(int arg1);

    void HTCChartYAxisMajorTicsColor(QColor color);
    void HTCChartYAxisMinorTicsColor(QColor color);

    void HTCChartYMajorTicsEnabled(bool checked);
    void HTCChartYMinorTicsEnabled(bool checked);

    void HTCChartYMinorTicsValue(double arg1);
    void HTCChartYMajorTicsValue(double arg1);

    void HTCChartYLogChart(bool checked);
    void HTCChartYLinChart(bool checked);

    void HTCChartYDecimalChart(bool checked);
    void HTCChartYScientificChart(bool checked);

    void HTCCHartXPrecisionChanged(int res);
    void HTCCHartYPrecisionChanged(int res);

    void HTCChartPenValues(int width, QColor color, int penStyle, QString penName, int penNumber);
    void HTCCHartNewPen(double baseValue, QString header);

    void HTCCHartRemovePenRequest(int penNumber);


    void on_btnSaveData_clicked();

private:
    Ui::HtcChart *ui;
    ChartProperties * cp = new ChartProperties;

    QChartView *chartView;

    HTCChartDataSet *_dataSet;

    void listThisList(QStringList list);

    void setChartTitle(QString title);
    QString stripTokensFromString(QString message, QString token);

    //Debug functions
    void listPenStyles(QString msg);
    void listHeaders();

    //end Debug finctions

    QString getCleanedYAxisUnits(QString target);

    int scanForChartType(QString filename);
    void writeTypetoFile(QString filename);

    void initProperties();
    void initConnects();
    void initChart();

    void saveChartData();
    void saveChartImage();

    QString _rawDataFileAndPath;

    void readfileIntoList(QString fileName);
    void listTheList(QStringList list);

    int _firstNumericRow;
    int _currentHeaderRow;
    QStringList _currentHeaderList;
    QChart *_chart;
    QLineSeries *_series;
    int _currentHeaderCount;
    QString _dataFileDelim;
    int _dataSetIndex;
    int _chartXOffset = 50;

    void setDataFileDelim(QString fileName);
    void setHeaderValues(QStringList list);
    int findFirstNumericRow(QStringList list, QString delimiter);
    void fillSeriesfromList(QLineSeries * series, int dataSet);


    // ------------------------------------------------- //
    //
    //  New code to accomodate RI & CI charts
    //
    bool _OverrideLegendValue;
    QString _dataTypes[2] = {"RI","CI"};

    // new cool stuff
    bool _EnableAnimations;

    // not yet implemented
    // (very difficult)
    bool _EnableHoverCallout;
    // ------------------------------------------------- //
    //
    //
    // ------------------------------------------------- //
    // Comm Check autodetect stuff
    //
    bool _dataIsCommCheck = false;
    // chart scaling when comm Check data detected
//    double _commCheckYMaxValue = 2.5;
//    double _commCheckYMinValue = -1;
    int _commCheckAutoDetect = 0;

    // ------------------------------------------------- //
    // analog autodetect stuff
    bool _dataIsAnalog = false;
    //
    //
    //
    //
    //
    // ------------------------------------------------- //

   // void setBaseFolder(QString file);
    QString getProperPath(QString target);
    QString _basePath;

    QStringList _masterlist;
    QStringList _reWriteList;

    // Chart Title items
    QColor _chartTitleTextColor;
    QFont _chartTitleTextFont;
    QString _chartTitleText;
    int _chartTitleFontSize = 14;

    // X Axis Label
    QFont _chartXAxisUnitsTextFont;
    QString _chartXAxisUnitsText;
    QBrush _chartXAxisUnitsBrush;
    int _chartXAxisUnitsFontSize = 12;
    QColor _chartXAxisLabelColor;
    QFont _chartXAxisLabelFont;
    int _chartXAxisLabelFontSize = 12;
    // rotation was double
    int _chartXAxisLabelRotation;
    QString _chartXAxisLinLogScale;
    QString _chartXAxisDecimalScientific;

    // X Axis grid Lines
    bool _chartXAxisMajorGridLinesVisible;
    int  _chartXAxisMajorGridLinesCount;
    bool _chartXAxisMinorGridLinesVisible;
    int  _chartXAxisMinorGridLinesCount;
    QColor _chartXAxisMajorGridLinesColor;
    QColor _chartXAxisMinorGridLinesColor;
    int _chartXAxisMajorGridLineSize = 2;
    int _chartXAxisMinorGridLineSize = 1;
    QPen _chartXAxisMajorGridLinesPen;
    QPen _chartXAxisMinorGridLinesPen;



    // Y Axis Label
    QFont _chartYAxisUnitsTextFont;
    QString _chartYAxisUnitsText;
    QBrush _chartYAxisUnitsBrush;
    int _chartYAxisUnitsFontSize = 12;
    QColor _chartYAxisLabelColor;
    QFont _chartYAxisLabelFont;
    int _chartYAxisLabelFontSize = 12;
    int _chartYAxisLabelRotation;
    QString _chartYAxisLinLogScale;
    QString _chartYAxisDecimalScientific;

    // Y Axis grid Lines
    bool _chartYAxisMajorGridLinesVisible;
    int  _chartYAxisMajorGridLinesCount;
    bool _chartYAxisMinorGridLinesVisible;
    int  _chartYAxisMinorGridLinesCount;
    QColor _chartYAxisMajorGridLinesColor;
    QColor _chartYAxisMinorGridLinesColor;
    int _chartYAxisMajorGridLineSize = 2;
    int _chartYAxisMinorGridLineSize = 1;
    QPen _chartYAxisMajorGridLinesPen;
    QPen _chartYAxisMinorGridLinesPen;



    // X Axis Scaling
    double _XAxisMinValue;
    double _XAxisMaxValue;
    double _XAxisRescaleValue;
    bool _reScaleFreqColumn = false;
    int _xPrecision = 0;

    double getFreqRescaleValue();

    // Y Axis Scaling
    double _YAxisMinValue;
    double _YAxisMaxValue;
    double _YAxisPaddingValue;

    int _yPrecision = 2;

    // pen color
    int defaultPenWidth;
    QString _defaultPenName = "Pen 01";
    QColor defaultPenColor;
    QString penStyles[7] = { "Qt::NoPen",
                             "Qt::SolidLine",
                             "Qt::DashLine",
                             "Qt::DotLine",
                             "Qt::DashDotLine",
                             "Qt::DashDotDotLine",
                             "Qt::CustomDashLine" };
    int defaultPenStyle;

    // Max 15 pens
    QColor _penColors[13] = {QColor("#FF0000"), QColor("#0000FF"), QColor("#2fc50a"), QColor("#f57900"),
                             QColor("#E61994"), QColor("#4e9a06"), QColor("#204a87"), QColor("#5c3566"),
                             QColor("#8f5902"), QColor("#993faf"), QColor("#830f0f"), QColor("#ab5a50"),
                            QColor("#4e9a06")};


//settings parts / vars

    QColor _penColorsReset[13] = {QColor("#FF0000"), QColor("#0000FF"), QColor("#2fc50a"), QColor("#f57900"),
                                  QColor("#E61994"), QColor("#4e9a06"), QColor("#204a87"), QColor("#5c3566"),
                                  QColor("#8f5902"), QColor("#993faf"), QColor("#830f0f"), QColor("#ab5a50"),
                                 QColor("#4e9a06")};

    void loadSettings();


    int _xGeoStart;
    int _yGeoStart;
    int _GeoWidth;
    int _GeoHeight;

    // ----------------------------------------------------- //
    // Chart X/Y scale padding vars
    // _ChartRIPaddingValueX/Y 1/0 On/Off
    // _ChartRIScalePaddingValueX/Y value in % for padding
    // ----------------------------------------------------- //
    double _ChartScalePaddingValueY;
    int _ChartPaddingValueY;
    double _defaultChartScalePaddingYValue = 0;

    // RI Padding values
    double _ChartRIScalePaddingValueX;
    int _ChartRIPaddingValueX;
    double _defaultChartRIScalePaddingXValue = 0;
    double _RIXaxisMaxMult = 3;

    // CI Padding values
    double _ChartCIScalePaddingValueX;
    int _ChartCIPaddingValueX;
    double _defaultChartCIScalePaddingXValue = 0;
    double _CIXaxisMaxMult = 400;


    // ----------------------------------------------------- //
    // ----------------------------------------------------- //

    int _ChartLegendFontSizeValue;
    int _ChartLegendFontFamilyValue;

    int _defaultXStart = 50;
    int _defaultYStart = 50;
    int _defaultWidth = 960;
    int _defaultHeight = 767;

    int _defaultChartLegendFontSizeValue = 9;
    int _defaultChartLegendFontFamilyValue = 1;

    QRect _layout;
    QRect _defaultLayout = QRect(_defaultXStart, _defaultYStart, _defaultWidth, _defaultHeight);

    int _defaultFontSize = 9;

    QString _ChartPaddingValues[2] = {"Off","On"};
    QString _ChartLegendFontValues[3] = {"Arial","Courier New", "Times New Roman"};

    QPalette palLabel;

    // ----------------------------------------------- //
    // CI/RI legend items
    // ----------------------------------------------- //
    QStringList _positions;
    QStringList _legendKeys;

    QStringList _defaultPositions;


    QStringList _positionsRI;
    QStringList _positionsCI;

    QStringList _defaultPositionsRI;
    QStringList _defaultPositionsCI;

    QStringList _legendKeysRI;
    QStringList _legendKeysCI;

    void setLegendText(int dType);
    QString stripMatch(QString target, QStringList strips);

    QStringList _ciLegendPrefixes, _ciUsedPrefixes;
    int _CIRangInService;

    // ----------------------------------------------- //
    // CI/RI legend items
    // ----------------------------------------------- //

//    QStringList _positions;
//    QStringList _defaultPositions;
//    QStringList _legendKeys;

    QString _ChartType = "";
    QString _ChartTestCode = "";
    QString _ChartModel = "";
    QString _ChartSerial = "";


// end settings vars


    int _penStates[13] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0 };
    int _penStyles[13] = { 1, 1, 1, 1, 1, 1, 1, 1,
                           1, 1, 1, 1, 1 };
    int _penWidths[13] = { 3, 1, 1, 1, 1, 1, 1, 1,
                           1, 1, 1, 1, 1 };



    QPen defaultChartPenStyle;
    QPen getPenStyle(int style);
    //int _legendFontPointSize = 10;

   void createPen(double baseValue, QString header);
   void updateHeaderCount();

   void adjustGeometry();
   int _addedPenSize = 3;

   double _minfreq;
   double _maxfreq;

   double _minlevel;
   double _maxlevel;

   void initChartScaleMemory();
   double setMinFreq(double freq);
   double setMaxFreq(double freq);
   double setMinLevel(double level);
   double setMaxLevel(double level);
   int getYAxisScalingResolution();
   int getXAxisScalingResolution();

   void discoverChartScaleValues();
   double getPaddingYValue();
   void setYaxisPaddingValue();
   double getPaddedYMaxValue();
   double getPaddedYMinValue();

   // new X Axis padding parts
   // -------------------------  //

   bool getXAxisPaddingEnabled();
   bool getYAxisPaddingEnabled();

   double getPaddedXMaxValue();
   double getPaddedXMinValue();
   // -------------------------  //




    // first run auto discovery
    bool _autoRangesDiscovered;
    bool _UpdatingFromProperties = false;
    bool _AddingNewPen = false;

    void initMinScaleValues(QStringList list);
    void initMaxScaleValues(QStringList list);


    void clearLayout(QLayout *layout);
    void assignChartToProperties();

    QColor GetNewColor(QColor currentColor);
    QFont GetNewFont(QFont currentFont);

    QPen GetNewGridLinesPen(QColor color, double size, Qt::PenStyle style);

    void setChartTitleTextColor(QColor color);

    QString StripQuotesFromString(QString wordToStrip);

    //Delete pen parts
    void stripLastHeaderItem(int column);
    void stripLastDataColumn(int column);

    // save data parts
    void setChartID();

    QString getSaveHeaderValues();

    QString getChartTypeSaveString();

    QString getChartKeySaveString();
    QStringList getSaveDataValues();

    QString getChartTitleSaveString();
    QString getChartTitleConfigSaveString();

    QString getChartXAxisUnitsSaveString();
    QString getChartXAxisUnitsConfigSaveString();

    QString getChartYAxisUnitsSaveString();
    QString getChartYAxisUnitsConfigSaveString();

    QString getChartXAxisParamsSaveString();
    QString getChartYAxisParamsSaveString();

    QString getChartXAxisMinSaveString();
    QString getChartXAxisMaxSaveString();

    QString getChartYAxisMinSaveString();
    QString getChartYAxisMaxSaveString();

    QString getChartXAxisGridLinesSaveString();
    QString getChartYAxisGridLinesSaveString();

    QString getPenStatesSaveString();
    QString getPenStylesSaveString();
    QString getPenWidthSaveString();
    QString getPenColorsSaveString();


    QString getParentDirForPath(QString path);
    void showNoAccessFileForSavingMessage(QString file);

    void setPropertiesFromOpenFile();
    int getfolderCountForPath(QString filename);

    QString getAnswerForBool(bool value);
    bool getBoolForAnswer(QString value);



    const QString _saveItemDelimiter = ":";
    const QString _saveChartTypeKey = "CHART_TYPE";
    const QString _saveChartKey = "CHART_KEY";
    const QString _saveItemChartTitleKey = "CHART_TITLE";
    const QString _saveItemChartTitleConfigKey = "CHART_TITLE_CONFIG";
    const QString _saveItemChartXAxisUnitsKey = "CHART_X_AXIS_UNITS";
    const QString _saveItemChartXAxisUnitsConfigKey = "CHART_X_AXIS_UNITS_CONFIG";
    const QString _saveItemChartYAxisUnitsKey = "CHART_Y_AXIS_UNITS";
    const QString _saveItemChartYAxisUnitsConfigKey = "CHART_Y_AXIS_UNITS_CONFIG";
    const QString _saveItemChartXAxisParamsKey = "CHART_X_AXIS_PARAMS";
    const QString _saveItemChartYAxisParamsKey = "CHART_Y_AXIS_PARAMS";

    const QString _saveItemChartXMinValueKey = "CHART_X_AXIS_MIN_VALUE";
    const QString _saveItemChartXMaxValueKey = "CHART_X_AXIS_MAX_VALUE";

    const QString _saveItemChartYMinValueKey = "CHART_Y_AXIS_MIN_VALUE";
    const QString _saveItemChartYMaxValueKey = "CHART_Y_AXIS_MAX_VALUE";

    const QString _saveItemChartXAxisGridLinesKey = "CHART_X_AXIS_GRID_LINES";
    const QString _saveItemChartYAxisGridLinesKey = "CHART_Y_AXIS_GRID_LINES";


    const QString _saveItemChartPenStatesKey = "CHART_PEN_STATES";
    const QString _saveItemChartPenStylesKey = "CHART_PEN_STYLES";
    const QString _saveItemChartPenWidthsKey = "CHART_PEN_WIDTHS";
    const QString _saveItemChartPenColorsKey = "CHART_PEN_COLORS";

    const QString _chartBasicTypes[2] = {"RI", "CI"};
    const int _chartTypeIDX = 0;
    const int _chartKeyIDX = 1;
    const int _chartTitleRowIDX = 2;
    const int _chartTitleConfigRowIDX = 3;
    const int _chartXaxisUnitsIDX = 4;
    const int _chartXaxisUnitsConfigIDX = 5;
    const int _chartYaxisUnitsIDX = 6;
    const int _chartYaxisUnitsConfigIDX = 7;
    const int _chartXAxisParamsIDX = 8;
    const int _chartYAxisParamsIDX = 9;

    const int _chartXaxisMinIDX = 10;
    const int _chartXaxisMaxIDX = 11;

    const int _chartYaxisMinIDX = 12;
    const int _chartYaxisMaxIDX = 13;

    const int _chartXAxisGridLines = 14;
    const int _chartYAxisGridLines = 15;

    const int _chartPenStatesIDX = 16;
    const int _chartPenStylesIDX = 17;
    const int _chartPenWidthsIDX = 18;
    const int _chartPenColorsIDX = 19;
    const int _lastSettingsIDX = _chartPenColorsIDX;


    bool _loadedChartFromFile;


    QString getLastChar(QString testValue);

    // new CI parts
    int RIdataType = 0;
    int CIdataType = 1;
    int _dataType = -1;
    QStringList dataTypes;

    //debug functions
    void listKeys();
    int listMasterList();
    int listHeaderList();



};

#endif // HTCCHART_H
