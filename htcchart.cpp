// ++++++++++++++++++++++++++++++++++++++++++++ //
// File: htcchart.cpp
// Description: Draws an analog chart
//
// Rev Date: 2020-01-27
//
// Working on: Changed file save location
// to be the same location that was pointed to
// when the user selects data.
//
// TODO: This thing should not process any
// raw data. It should receive an object
// that has a data, headers description, etc
//
// ++++++++++++++++++++++++++++++++++++++++++++ //
#include "htcchart.h"
#include "ui_htcchart.h"

HtcChart::HtcChart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HtcChart)
{
    ui->setupUi(this);

    dataTypes.clear();
    dataTypes << "RI-Data_Type" << "CI-Data-Type";

    _firstNumericRow = -1;
    _autoRangesDiscovered = false;

    _legendKeys.clear();
    _legendKeys << "PV" << "0_BL" << "0_H" << "0_V" << "90_H" << "90_V" << "180_H" << "180_V" << "270_H" << "270_V";


    _positions.clear();
    _defaultPositions << "Padding Value"  << "Baseline" << "0° Horiz" << "0° Vert" << "90° Horiz"
                      << "90° Vert" << "180° Horiz" << "180° Vert" << "270° Horiz" << "270° Vert";

    _positions = _defaultPositions;

    loadSettings();

    initConnects();
    _loadedChartFromFile = false;

    //initProperties();

}

HtcChart::~HtcChart()
{
    delete cp;
    delete ui;
}

void HtcChart::setFileToOpen(QString fileName, bool RescaleFreq, QString baseFolder)
{

    if(!fileName.isEmpty())
    {
        _basePath = baseFolder;
        _UpdatingFromProperties = true;
        _loadedChartFromFile = true;
        _rawDataFileAndPath = fileName;
        _autoRangesDiscovered = true;
        reScaleFreqColumn = RescaleFreq;
        _dataSetIndex = 0;



        //setDataType by filename using test code

        adjustGeometry();
        int isMissingChartType = scanForChartType(fileName);
        if (isMissingChartType == 0)
        {
            writeTypetoFile(fileName);
        }
        readfileIntoList(_rawDataFileAndPath);

        setPropertiesFromOpenFile();

        initChart();

    }


}

void HtcChart::setChartByDataSet(HTCChartDataSet *ds, bool RescaleFreq)
{

   _autoRangesDiscovered = false;
    _dataSet = ds;

    // set data type
    _dataType = ds->getDataType();
    initProperties();

    _basePath = ds->GetBaseFolder();
    _dataIsCommCheck = ds->GetIsCommCheckData();

    setLegendText(_dataType);

    if(getDataType() == CIdataType)
    {
        _chartXAxisMinorGridLinesVisible = true;
    }
    else
    {
        _chartXAxisMinorGridLinesVisible = false;
    }

    _masterlist = ds->GetData();

    _dataSetIndex = ds->GetDataSetIndex();

    _chartTitleText = ds->GetChartTitle();

    _chartXAxisUnitsText = ds->GetXAxisTitle();
    _chartYAxisUnitsText = ds->GetYAxisTitle();

    _chartXAxisLinLogScale = ds->GetXAxisScale();
    _chartYAxisLinLogScale = ds->GetYAxisScale();

    reScaleFreqColumn = RescaleFreq;
    _rawDataFileAndPath = ds->GetSampleFileName();

    setDataFileDelim(_rawDataFileAndPath);
    setChartID();
    setHeaderValues(_masterlist);
    adjustGeometry();
    setChartTitle(_chartTitleText);
    initChart();


}

int HtcChart::getDataType()
{
    return _dataType;
}

void HtcChart::setDataType(int dataType)
{
    _dataType = dataType;
}




void HtcChart::on_actionProperties_triggered()
{

    cp->setDataType(getDataType());
    cp->setChartTitleItems(_chartTitleText, _chartTitleTextFont, _chartTitleTextColor);

    cp->setChartXAxisItems(_chartXAxisUnitsText, _chartXAxisUnitsTextFont, _chartXAxisUnitsBrush,
                           _chartXAxisLabelFont, _chartXAxisLabelColor, _chartXAxisLabelRotation,
                           _XAxisMinValue, _XAxisMaxValue, _chartXAxisLinLogScale, _chartXAxisDecimalScientific);

    cp->setChartXAxisGridLines(_chartXAxisMajorGridLinesVisible,_chartXAxisMajorGridLinesCount,
                               _chartXAxisMajorGridLinesColor, _chartXAxisMajorGridLineSize,
                               _chartXAxisMinorGridLinesVisible,_chartXAxisMinorGridLinesCount,
                               _chartXAxisMinorGridLinesColor, _chartXAxisMinorGridLineSize);

    cp->setChartYAxisItems(_chartYAxisUnitsText, _chartYAxisUnitsTextFont, _chartYAxisUnitsBrush,
                           _chartYAxisLabelFont, _chartYAxisLabelColor, _chartYAxisLabelRotation,
                           _YAxisMinValue, _YAxisMaxValue, _chartYAxisLinLogScale, _chartYAxisDecimalScientific);

    cp->setChartYAxisGridLines(_chartYAxisMajorGridLinesVisible,_chartYAxisMajorGridLinesCount,
                               _chartYAxisMajorGridLinesColor, _chartYAxisMajorGridLineSize,
                               _chartYAxisMinorGridLinesVisible,_chartYAxisMinorGridLinesCount,
                               _chartYAxisMinorGridLinesColor, _chartYAxisMinorGridLineSize);

    cp->setModal(true);
    cp->show();

}

void HtcChart::on_btnClose_clicked()
{
    if (cp != 0)
    {
        cp->close();
    }

    close();


}


// ----------------------------------------------
// The default stuff should maybe be loaded
// from a settings file instead of hard coded
// here.
// ----------------------------------------------
void HtcChart::initProperties()
{

        // Chart Title font
        _chartTitleText = "";
        _chartTitleTextFont = QFont("Arial",_chartTitleFontSize, QFont::Normal );
        _chartTitleTextColor = QColor("#000000");

        // Chart Title is now set in the dataMangler
        _chartXAxisUnitsBrush.setColor(QColor("#000000"));
        _chartXAxisUnitsTextFont = QFont("Arial",_chartXAxisUnitsFontSize, QFont::Normal);

        _chartXAxisLabelColor = QColor("#000000");
        _chartXAxisLabelFont = QFont("Arial",_chartXAxisLabelFontSize, QFont::Normal );
        _chartXAxisLabelRotation = -45;
        _chartXAxisLinLogScale = "LIN";
        _chartXAxisDecimalScientific = "DEC";
        _chartXAxisMajorGridLinesVisible = true;
        _chartXAxisMajorGridLinesCount = 11;

        if (getDataType() == RIdataType)
        {
            _chartXAxisMinorGridLinesCount = 4;
        }
        else
        {
            _chartXAxisMinorGridLinesCount = 9;
        }

        _chartXAxisMajorGridLinesColor = QColor("#E0E3DD"); // original color "#000000"
        _chartXAxisMinorGridLinesColor = QColor("#E0E3DD"); // original color "#000000"
        //_chartXAxisMajorGridLineSize = 2;
        //_chartXAxisMinorGridLineSize = 1;

        _chartXAxisMajorGridLinesPen = GetNewGridLinesPen(_chartXAxisMajorGridLinesColor, _chartXAxisMajorGridLineSize, Qt::SolidLine);
        _chartXAxisMinorGridLinesPen = GetNewGridLinesPen(_chartXAxisMinorGridLinesColor, _chartXAxisMinorGridLineSize, Qt::SolidLine);


        _chartYAxisUnitsText = "Correction Factor";
        _chartYAxisUnitsBrush.setColor(QColor("#000000"));
        _chartYAxisUnitsTextFont = QFont("Arial",_chartXAxisUnitsFontSize, QFont::Normal);

        _chartYAxisLabelFont = QFont("Arial",_chartYAxisLabelFontSize, QFont::Normal );
        _chartYAxisLabelColor = QColor("#000000");
        _chartYAxisLabelRotation = 0;

        _chartYAxisLinLogScale = "LIN";
        _chartYAxisDecimalScientific = "DEC";

        _chartYAxisMajorGridLinesVisible = true;

        if (getDataType() == RIdataType)
        {
            _chartYAxisMajorGridLinesCount = 11;
        }
        else
        {
            _chartYAxisMajorGridLinesCount = 11;
        }

        _chartYAxisMinorGridLinesVisible = false;
        _chartYAxisMinorGridLinesCount = 4;
        _chartYAxisMajorGridLinesColor = QColor("#E0E3DD"); // original color "#000000"
        _chartYAxisMinorGridLinesColor = QColor("#E0E3DD"); // original color "#000000"
        //_chartYAxisMajorGridLineSize = 2;
        //_chartYAxisMinorGridLineSize = 4; //DEFAULT CHANGE


        _chartYAxisMajorGridLinesPen = GetNewGridLinesPen(_chartYAxisMajorGridLinesColor, _chartYAxisMajorGridLineSize, Qt::SolidLine);
        _chartYAxisMinorGridLinesPen = GetNewGridLinesPen(_chartYAxisMinorGridLinesColor, _chartYAxisMinorGridLineSize, Qt::SolidLine);



        defaultPenWidth = 2;
        defaultPenColor = QColor(Qt::blue);
        defaultPenStyle = 1;


}

void HtcChart::initConnects()
{
    // Chart Title
    connect(cp,SIGNAL(ChartTitleText(QString)),this,SLOT(ChartTitleText(QString)));
    connect(cp,SIGNAL(ChartTitleTextFont(QFont)),this,SLOT(ChartTitleTextFont(QFont)));
    connect(cp,SIGNAL(ChartTitleTextColor(QColor)),this,SLOT(ChartTitleTextColor(QColor)));

    // Chart X Axis Items
    connect(cp,SIGNAL(HTCChartXAxisUnitsTextChanged(QString)),this,SLOT(HTCChartXAxisUnitsText(QString)));
    connect(cp,SIGNAL(HTCChartXAxisUnitsTextFontChanged(QFont)),this,SLOT(HTCChartXAxisUnitsTextFont(QFont)));
    connect(cp,SIGNAL(HTCChartXAxisUnitsTextColorChanged(QColor)),this,SLOT(HTCChartXAxisUnitsTextColor(QColor)));


    connect(cp,SIGNAL(HTCChartXAxisLabelsTextRotationChanged(qint32)),this,SLOT(HTCChartXAxisLabelsTextRotation(qint32)));
    connect(cp,SIGNAL(HTCChartXAxisLabelsTextFontChanged(QFont)),this,SLOT(HTCChartXAxisLabelsTextFont(QFont)));
    connect(cp,SIGNAL(HTCChartXAxisLabelsTextColorChanged(QColor)),this,SLOT(HTCChartXAxisLabelsTextColor(QColor)));

    connect(cp,SIGNAL(HTCChartXAxisLabelsTextScaleMinChanged(double)),this,SLOT(HTCChartXAxisLabelsTextScaleMin(double)));
    connect(cp,SIGNAL(HTCChartXAxisLabelsTextScaleMaxChanged(double)),this,SLOT(HTCChartXAxisLabelsTextScaleMax(double)));

    connect(cp,SIGNAL(HTCChartXMajorThicknessValueChanged(int)),this,SLOT(HTCChartXMajorThickness(int)));
    connect(cp,SIGNAL(HTCChartXMinorThicknessValueChanged(int)),this,SLOT(HTCChartXMinorThickness(int)));

    connect(cp,SIGNAL(HTCChartXAxisMajorTicsColorChanged(QColor)),this,SLOT(HTCChartXAxisMajorTicsColor(QColor)));
    connect(cp,SIGNAL(HTCChartXAxisMinorTicsColorChanged(QColor)),this,SLOT(HTCChartXAxisMinorTicsColor(QColor)));

    connect(cp,SIGNAL(HTCChartXMinorTicsEnabledChanged(bool)),this,SLOT(HTCChartXMinorTicsEnabled(bool)));
    connect(cp,SIGNAL(HTCChartXMajorTicsEnabledChanged(bool)),this,SLOT(HTCChartXMajorTicsEnabled(bool)));

    connect(cp,SIGNAL(HTCChartXMinorTicsValueChanged(double)),this,SLOT(HTCChartXMinorTicsValue(double)));
    connect(cp,SIGNAL(HTCChartXMajorTicsValueChanged(double)),this,SLOT(HTCChartXMajorTicsValue(double)));

    connect(cp,SIGNAL(HTCCHartXPrecisionChangeRequest(int)),this,SLOT(HTCCHartXPrecisionChanged(int)));

    connect(cp,SIGNAL(HTCChartXLogChartRequest(bool)),this,SLOT(HTCChartXLogChart(bool)));
    connect(cp,SIGNAL(HTCChartXLinChartRequest(bool)),this,SLOT(HTCChartXLinChart(bool)));

    connect(cp,SIGNAL(HTCChartXDecimalRequest(bool)),this,SLOT(HTCChartXDecimalChart(bool)));
    connect(cp,SIGNAL(HTCChartXScientificRequest(bool)),this,SLOT(HTCChartXScientificChart(bool)));


    // Chart Y Axis Items
    connect(cp,SIGNAL(HTCChartYAxisUnitsTextChanged(QString)),this,SLOT(HTCChartYAxisUnitsText(QString)));
    connect(cp,SIGNAL(HTCChartYAxisUnitsTextFontChanged(QFont)),this,SLOT(HTCChartYAxisUnitsTextFont(QFont)));
    connect(cp,SIGNAL(HTCChartYAxisUnitsTextColorChanged(QColor)),this,SLOT(HTCChartYAxisUnitsTextColor(QColor)));

    connect(cp,SIGNAL(HTCChartYAxisLabelsTextRotationChanged(qint32)),this,SLOT(HTCChartYAxisLabelsTextRotation(qint32)));
    connect(cp,SIGNAL(HTCChartYAxisLabelsTextFontChanged(QFont)),this,SLOT(HTCChartYAxisLabelsTextFont(QFont)));
    connect(cp,SIGNAL(HTCChartYAxisLabelsTextColorChanged(QColor)),this,SLOT(HTCChartYAxisLabelsTextColor(QColor)));

    connect(cp,SIGNAL(HTCChartYAxisLabelsTextScaleMinChanged(double)),this,SLOT(HTCChartYAxisLabelsTextScaleMin(double)));
    connect(cp,SIGNAL(HTCChartYAxisLabelsTextScaleMaxChanged(double)),this,SLOT(HTCChartYAxisLabelsTextScaleMax(double)));

    connect(cp,SIGNAL(HTCChartYMajorThicknessValueChanged(int)),this,SLOT(HTCChartYMajorThickness(int)));
    connect(cp,SIGNAL(HTCChartYMinorThicknessValueChanged(int)),this,SLOT(HTCChartYMinorThickness(int)));

    connect(cp,SIGNAL(HTCChartYAxisMajorTicsColorChanged(QColor)),this,SLOT(HTCChartYAxisMajorTicsColor(QColor)));
    connect(cp,SIGNAL(HTCChartYAxisMinorTicsColorChanged(QColor)),this,SLOT(HTCChartYAxisMinorTicsColor(QColor)));

    connect(cp,SIGNAL(HTCChartYMinorTicsEnabledChanged(bool)),this,SLOT(HTCChartYMinorTicsEnabled(bool)));
    connect(cp,SIGNAL(HTCChartYMajorTicsEnabledChanged(bool)),this,SLOT(HTCChartYMajorTicsEnabled(bool)));

    connect(cp,SIGNAL(HTCChartYMinorTicsValueChanged(double)),this,SLOT(HTCChartYMinorTicsValue(double)));
    connect(cp,SIGNAL(HTCChartYMajorTicsValueChanged(double)),this,SLOT(HTCChartYMajorTicsValue(double)));

    connect(cp,SIGNAL(HTCChartYLogChartRequest(bool)),this,SLOT(HTCChartYLogChart(bool)));
    connect(cp,SIGNAL(HTCChartYLinChartRequest(bool)),this,SLOT(HTCChartYLinChart(bool)));

    connect(cp,SIGNAL(HTCChartYDecimalRequest(bool)),this,SLOT(HTCChartYDecimalChart(bool)));
    connect(cp,SIGNAL(HTCChartYScientificRequest(bool)),this,SLOT(HTCChartYScientificChart(bool)));

    connect(cp,SIGNAL(HTCCHartYPrecisionChangeRequest(int)),this,SLOT(HTCCHartYPrecisionChanged(int)));

    connect(cp,SIGNAL(HTCChartPenValueChanged(int, QColor, int, QString, int)),this,SLOT(HTCChartPenValues(int, QColor, int, QString, int)));
    connect(cp,SIGNAL(HTCCHartAddPenRequest(double, QString)),this,SLOT(HTCCHartNewPen(double, QString)));

    connect(cp,SIGNAL(HTCChartRemovePen(int)),this,SLOT(HTCCHartRemovePenRequest(int)));
}

void HtcChart::initChart()
{
    bool axisXHasBeenAdded = false;
    bool axisYHasBeenAdded = false;
    QString cName = "";
    QString s1, match;

    int YaxisLabelResolution;
    //int XaxisLabelResolution = 0;
    QString newLegendText;
    int posIDX = -1;

    if(!_UpdatingFromProperties)
    {
       initChartScaleMemory();
       discoverChartScaleValues();

      if (getXAxisPaddingEnabled() == 1 && _loadedChartFromFile == false)
      {
          _XAxisMinValue = getPaddedXMinValue();
          _XAxisMaxValue = getPaddedXMaxValue();
      }

      if (_commCheckAutoDetect == 1 && _dataIsCommCheck == true && _loadedChartFromFile == false)
      {
          _YAxisMinValue = _commCheckYMinValue;
          _YAxisMaxValue = _commCheckYMaxValue;
          // qDebug() << "used comm Check auto scaling";

      }
      else if (_ChartPaddingValueY == 1 && _loadedChartFromFile == false)
       {
           setYaxisPaddingValue();
           _YAxisMinValue = getPaddedYMinValue();
           _YAxisMaxValue = getPaddedYMaxValue();
           //qDebug() << "used Y Padding values";
       }

    }

    QString YAxisFormat = "";
    YaxisLabelResolution = getYAxisScalingResolution();
    YAxisFormat.append("%.");
    YAxisFormat.append(QString::number(YaxisLabelResolution));
    YAxisFormat.append("f");


    QString XAxisFormat = "";
    XAxisFormat.append("%.");
    XAxisFormat.append(QString("%1").arg(getXAxisScalingResolution()));
    XAxisFormat.append("f");



    _chart = new QChart();

    // Chart Title
    // ----------------------------------
    _chart->setTitle(_chartTitleText);
    _chart->setTitleFont(_chartTitleTextFont);
    _chart->setTitleBrush(QBrush(_chartTitleTextColor));

    // ----------------------------------------------------------+
    // How do I set the title to be center aligned?
    //_chart->setLayoutDirection(Qt::LayoutDirection center());
    // ----------------------------------------------------------+
    _chart->legend()->hide();

    if(_EnableAnimations == true)
    {
        _chart->setAnimationOptions(QChart::AllAnimations);
    }
    else
    {
        _chart->setAnimationOptions(QChart::NoAnimation);
    }


    // -------------------------------------------------- //
    // We need to iterate over all of the data first
    // to figure out wat the min/max scaling needs to be
    //
    // * Whatever the first series uses as min/max is what
    // all the additional scales will use regardless if
    // subsequent sets are different.
    //
    // -------------------------------------------------- //
    for(int dataSet = 1; dataSet < _currentHeaderCount ; dataSet++)
    {
        QLineSeries *_series = new QLineSeries();
        fillSeriesfromList(_series, dataSet);

        //qDebug() << "loading with override legend == " << _OverrideLegendValue;

        if (_OverrideLegendValue == false)
        {
            // set pen name normally
            s1.clear();
            match.clear();
            newLegendText.clear();
            s1 = _currentHeaderList[dataSet];

            // the CI legends include voltage of test
            // we need to strip that info to do indexing
            // then we need to add it back in to the original.
            match = stripMatch(s1,_ciLegendPrefixes);

            posIDX = _legendKeys.indexOf(match);

            if(posIDX != -1)
            {

                //strips
                if (_dataType == CIdataType)
                {
                    newLegendText.append(_ciUsedPrefixes.at(_CIRangInService));
                    newLegendText.append(" ");
                    newLegendText.append(_positions.at(posIDX));
                    // qDebug() << "Loaded legend from #1";
                }
                else
                {
                    newLegendText =  _positions.at(posIDX);
                    //qDebug() << "Loaded legend from #2";
                }

            }
            else
            {
                // this legend gets called when loading a chart
                // from a stored .chart file

                newLegendText = _currentHeaderList[dataSet];
                //qDebug() << "Loaded legend from #3";
            }
        }
        else
        {
            // set it by header values explicitly
            newLegendText = _currentHeaderList[dataSet];
        }

        cName = StripQuotesFromString(newLegendText);

        _series->setName(StripQuotesFromString(cName));

        //Series pen color & thickness
        QPen SeriesPen(_penColors[dataSet - 1]);
        SeriesPen.setWidth(_penWidths[dataSet - 1]);
        SeriesPen.setStyle(Qt::PenStyle(_penStyles[dataSet - 1]));

        _series->setPen(SeriesPen);
        _chart->addSeries(_series);

        //this only gets called on initial chart construction
        if(_UpdatingFromProperties == false )
        {
            _penStates[dataSet - 1] = 1;
            cp->setPenItems(_penWidths[dataSet - 1], _penColors[dataSet - 1], defaultPenStyle, cName, dataSet);
        }

        //this gets called when tweaking the chart or
        // when adding a new pen
        if (_UpdatingFromProperties == true ||_AddingNewPen == true)
        {
            _penStates[dataSet - 1] = 1;
            cp->setPenItems(_penWidths[dataSet - 1], _penColors[dataSet - 1], _penStyles[dataSet - 1], cName, dataSet);
        }


        if (_chartXAxisLinLogScale == "LIN" )
        {
            QValueAxis *axisX = new QValueAxis();

            // qDebug() << "Dropped into XAxis scaling as LIN";

            axisX->setMin(_XAxisMinValue);
            axisX->setMax(_XAxisMaxValue);
            axisX->setTitleText(_chartXAxisUnitsText);
            axisX->setTitleBrush(_chartXAxisUnitsBrush);
            axisX->setTitleFont(_chartXAxisUnitsTextFont);

            if(_chartXAxisDecimalScientific == "DEC")
            {

                if (_xPrecision < 1)
                {
                    axisX->setLabelFormat("%i");  //
                }
                else
                {
                    axisX->setLabelFormat(XAxisFormat);
                }

            }
            else
            {
                axisX->setLabelFormat("%.2E");
            }

            axisX->setGridLineVisible(_chartXAxisMajorGridLinesVisible);
            axisX->setTickCount(_chartXAxisMajorGridLinesCount);
            axisX->setGridLinePen(_chartXAxisMajorGridLinesPen);
            axisX->setMinorGridLineVisible(_chartXAxisMinorGridLinesVisible);
            axisX->setMinorTickCount(_chartXAxisMinorGridLinesCount);
            axisX->setMinorGridLinePen(_chartXAxisMinorGridLinesPen);
            axisX->setLabelsColor(_chartXAxisLabelColor);
            axisX->setLabelsFont(_chartXAxisLabelFont);
            axisX->setLabelsAngle(_chartXAxisLabelRotation);

            _chart->addAxis(axisX, Qt::AlignBottom);

            if (axisXHasBeenAdded == false)
            {
                axisX->setVisible(true);
                axisXHasBeenAdded = true;
            }
            else
            {
                axisX->setVisible(false);
            }

            _series->attachAxis(axisX);

        }
        else
        {
            QLogValueAxis *axisX = new QLogValueAxis();

            axisX->setTitleText(_chartXAxisUnitsText);
            axisX->setTitleBrush(_chartXAxisUnitsBrush);
            axisX->setTitleFont(_chartXAxisUnitsTextFont);

            if(_chartXAxisDecimalScientific == "DEC")
            {
                if (_xPrecision < 1)
                {
                    axisX->setLabelFormat("%i");
                }
                else
                {
                    axisX->setLabelFormat(XAxisFormat);
                }
            }
            else
            {
                axisX->setLabelFormat("%.2E");
            }

            axisX->setMin(_XAxisMinValue);
            axisX->setMax(_XAxisMaxValue);
            axisX->setGridLineVisible(_chartXAxisMajorGridLinesVisible);
            axisX->setGridLinePen(_chartXAxisMajorGridLinesPen);
            axisX->setMinorGridLineVisible(_chartXAxisMinorGridLinesVisible);
            axisX->setMinorTickCount(_chartXAxisMinorGridLinesCount);
            axisX->setMinorGridLinePen(_chartXAxisMinorGridLinesPen);
            axisX->setLabelsColor(_chartXAxisLabelColor);
            axisX->setLabelsFont(_chartXAxisLabelFont);
            axisX->setLabelsAngle(_chartXAxisLabelRotation);

            _chart->addAxis(axisX, Qt::AlignBottom);

            if (axisXHasBeenAdded == false)
            {
                axisX->setVisible(true);
                axisXHasBeenAdded = true;
            }
            else
            {
                axisX->setVisible(false);
            }

            _series->attachAxis(axisX);


        }

        // Y Axis values
        // ------------------------------------
        if (_chartYAxisLinLogScale == "LIN")
        {
             QValueAxis *axisY = new QValueAxis();

             axisY->setMin(_YAxisMinValue);
             axisY->setMax(_YAxisMaxValue);

             axisY->setTitleText(_chartYAxisUnitsText);


             if(_chartYAxisDecimalScientific == "DEC")
             {
                 axisY->setLabelFormat(YAxisFormat);
             }
             else
             {
                 axisY->setLabelFormat("%.2E");
             }

             axisY->setTitleBrush(_chartYAxisUnitsBrush);
             axisY->setTitleFont(_chartYAxisUnitsTextFont);
             axisY->setLabelsColor(_chartYAxisLabelColor);
             axisY->setLabelsFont(_chartYAxisLabelFont);
             axisY->setLabelsAngle(_chartYAxisLabelRotation);
             axisY->setGridLineVisible(_chartYAxisMajorGridLinesVisible);
             axisY->setTickCount(_chartYAxisMajorGridLinesCount);
             axisY->setGridLinePen(_chartYAxisMajorGridLinesPen);
             axisY->setMinorGridLineVisible(_chartYAxisMinorGridLinesVisible);
             axisY->setMinorTickCount(_chartYAxisMinorGridLinesCount);
             axisY->setMinorGridLinePen(_chartYAxisMinorGridLinesPen);


             _chart->addAxis(axisY, Qt::AlignLeft);

            if (axisYHasBeenAdded == false)
            {
                axisY->setVisible(true);
                axisYHasBeenAdded = true;
            }
            else
            {
                axisY->setVisible(false);
            }

            _series->attachAxis(axisY);

        }
        else
        {
            QLogValueAxis *axisY = new QLogValueAxis();

            axisY->setTitleText(_chartYAxisUnitsText);
            if(_chartYAxisDecimalScientific == "DEC")
            {
                axisY->setLabelFormat(YAxisFormat);
            }
            else
            {
                axisY->setLabelFormat("%.2E");
            }

            axisY->setTitleBrush(_chartYAxisUnitsBrush);
            axisY->setTitleFont(_chartYAxisUnitsTextFont);
            axisY->setLabelsColor(_chartYAxisLabelColor);
            axisY->setLabelsFont(_chartYAxisLabelFont);
            axisY->setLabelsAngle(_chartYAxisLabelRotation);

            axisY->setMin(_YAxisMinValue);
            axisY->setMax(_YAxisMaxValue);

            axisY->setGridLineVisible(_chartYAxisMajorGridLinesVisible);
            axisY->setGridLinePen(_chartYAxisMajorGridLinesPen);
            axisY->setMinorGridLineVisible(_chartYAxisMinorGridLinesVisible);
            axisY->setMinorTickCount(_chartXAxisMinorGridLinesCount);
            axisY->setMinorGridLinePen(_chartYAxisMinorGridLinesPen);

            _chart->addAxis(axisY, Qt::AlignLeft);

            if (axisYHasBeenAdded == false)
            {
                axisY->setVisible(true);
                axisYHasBeenAdded = true;
            }
            else
            {
                axisY->setVisible(false);
            }

            _series->attachAxis(axisY);

        }

    }

    // added for legend test
    // We need to find a way to get
    // the chart to do multiple rows
    // -----------------------------------
    _chart->legend()->setVisible(true);
    _chart->legend()->setAlignment(Qt::AlignBottom); // Qt::AlignBottom
    _chart->legend()->setBackgroundVisible(true);

    QFont font = _chart->legend()->font();
    font.setPointSize(_ChartLegendFontSizeValue);

    font.setFamily(_ChartLegendFontValues[_ChartLegendFontFamilyValue]);
   _chart->legend()->setFont(font);

        //  Create the cart
    chartView = new QChartView(_chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartLayout->addWidget(chartView,0,0);
    ui->chartLayout->update();

}

void HtcChart::readfileIntoList(QString fileName)
{

    QFile file(fileName);
    setDataFileDelim(fileName);

    _masterlist.clear();


    QString msg;
    msg.append("Chart built from - ");
    msg.append(fileName);
    ui->statusbar->showMessage(msg);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            while (!in.atEnd())
            {
                _masterlist += in.readLine();

            }
                file.close();
        }

    // set headers

    setHeaderValues(_masterlist);


}

void HtcChart::listTheList(QStringList list)
{
    int count = list.count();
    for(int i = 0; i < count;i++)
    {
        qDebug() << " row " << i  << " is " << list.at(i);
    }

}

void HtcChart::setDataFileDelim(QString fileName)
{
    QFileInfo info = QFileInfo(fileName);
    QString extension = info.suffix();

    if (extension == "txt")
    {
        _dataFileDelim = "\t";
    }
    else
    {
        _dataFileDelim = ",";
    }

}

void HtcChart::setHeaderValues(QStringList list)
{

    if (!_masterlist.isEmpty())
    {
        if(_loadedChartFromFile == false)
        {
            _firstNumericRow = findFirstNumericRow(_masterlist, _dataFileDelim);
        }
        else
        {
            _firstNumericRow = _lastSettingsIDX + 2;

        }

        QString current;

        current = list[_firstNumericRow - 1];

       // qDebug() << "listing the header from the file";

        // listTheList(list);

        if (!_currentHeaderList.isEmpty())
        {
            _currentHeaderList.clear();
        }

        _currentHeaderList = current.split(_dataFileDelim);
        _currentHeaderCount = _currentHeaderList.count();

    }

}

int HtcChart::findFirstNumericRow(QStringList list, QString delimiter)
{
    QStringList group;
    QString current;
    QString dataItem;
    int numFinds;
    bool isNumber;
    int result = -1;
    bool found = false;
    QRegExp re("^-?\\d*\\.?\\d+");
    QRegExp re2("(([-+]?[0-9]+)?\\.)?\\b[0-9]+([eE][-+]?[0-9]+)?");


    for (int listRow = 0; listRow < list.count(); listRow++)
    {

        if (!found)
        {
            numFinds = 0;
            current = list[listRow];
            group = current.split(delimiter);

            for (int i = 0; i < group.count(); i++)
            {

                dataItem = group[i];

                dataItem = dataItem.trimmed();
                isNumber = false;

                if(re.exactMatch(dataItem) || re2.exactMatch(dataItem))
                {

                    numFinds = numFinds + 1;

                    if (numFinds > 1)
                    {
                        result = listRow;
                        found = true;
                        break;

                    }
                }
                else {
                    break;
                }

            }

        }
        else
        {
            break;
        }


    }
    return result;

}



    void HtcChart::fillSeriesfromList(QLineSeries *series, int dataSet)
{


    QStringList group;
    int start;
    double freq;
    double level;
    QString l1;

    if (!_masterlist.isEmpty())
    {


        _currentHeaderRow = _firstNumericRow -1;

        if (_firstNumericRow != -1)
        {

            start = _firstNumericRow;
        }
        else
        {
            start = 0;
        }

        for (int i = start; i < _masterlist.count(); i++)
        {

            group = _masterlist[i].split(_dataFileDelim);

            freq = QString(group.at(0)).toDouble()/getFreqRescaleValue();
            level = group.at(dataSet).toDouble();


            series->append(freq, level);
        }

    }


    }

//    void HtcChart::setBaseFolder(QString file)
//    {

//        qDebug() << "base path is " << _basePath;
//    }

    QString HtcChart::getProperPath(QString target)
    {
        QString result = target;
        int len = result.length();
        QString bs = "/";

        QString lastChar = result.right(1);

        if(lastChar != bs);
        {
            result.append(bs);
        }

        return result;


    }

    double HtcChart::getFreqRescaleValue()
    {
        if(reScaleFreqColumn == true)
        {
            _XAxisRescaleValue = 1.0e06;
        }
        else
        {
            _XAxisRescaleValue = 1;
        }
        return _XAxisRescaleValue;
    }

    void HtcChart::loadSettings()
    {
        QString positionValue = "";
        QString penValue = "";

        QSettings setting("Keysight","ChartBuilder");

        setting.beginGroup("ProgramFolders");

        _ChartLegendFontSizeValue = setting.value("ChartLegendFontSize").toInt();
        _ChartLegendFontFamilyValue = setting.value("ChartLegendFontFamily").toInt();

        // --------------------------------------------------------- //
        //
        // new legend override
        //

        if (setting.value("ChartLegendOverride").toInt() == 1)
        {
            _OverrideLegendValue = true;
        }
        else
        {
            _OverrideLegendValue = false;
        }

        // --------------------------------------------------------- //

        // the cool stuff
        if (setting.value("EnableChartAnimations").toInt() == 1)
        {
            _EnableAnimations = true;
        }
        else
        {
            _EnableAnimations = false;
        }

        //
        // new hover callout
        //
        // Not in service yet
        //
        if (setting.value("EnableHoverCallout").toInt() == 1)
        {
            _EnableHoverCallout =  true;
        }
        else
        {
            _EnableHoverCallout = false;
        }


        _ChartPaddingValueY = setting.value("ChartScalePaddingYOn").toInt();

        // new comm check autodetect
        _commCheckAutoDetect = setting.value("CommCheckAutoDetectEnabled").toInt();


        if(_ChartPaddingValueY == 1)
        {
            _ChartScalePaddingValueY = setting.value("ChartScalePaddingYValue").toDouble();
        }
        else
        {
            _ChartScalePaddingValueY = _defaultChartScalePaddingYValue;
        }

        // RI X Axis padding values

        _ChartRIPaddingValueX = setting.value("ChartRIScalePaddingXOn").toInt();


        if(_ChartRIPaddingValueX == 1)
        {
            _ChartRIScalePaddingValueX = setting.value("ChartRIScalePaddingXValue").toDouble();
        }
        else
        {
            _ChartRIScalePaddingValueX = _defaultChartRIScalePaddingXValue;
        }

        // CI X Axis padding values
        _ChartCIPaddingValueX = setting.value("ChartCIScalePaddingXOn").toInt();


        if(_ChartCIPaddingValueX == 1)
        {
            _ChartCIScalePaddingValueX = setting.value("ChartCIScalePaddingXValue").toDouble();
        }
        else
        {
            _ChartCIScalePaddingValueX = _defaultChartCIScalePaddingXValue;
        }


        // math to calculate X Scale padding
        // new min = detected X-Min + (X Padding value / 100) * Detected X-Min)


        if(_ChartLegendFontSizeValue == 0)
        {
            _ChartLegendFontSizeValue = _defaultChartLegendFontSizeValue;
        }

        if(_ChartLegendFontFamilyValue > 2 || _ChartLegendFontFamilyValue < 0)
        {
            _ChartLegendFontFamilyValue = _defaultChartLegendFontFamilyValue;
        }


         penValue = setting.value("ChartPen01").toString();
        if(penValue.isEmpty())
        {
            _penColors[0] = QColor(_penColorsReset[0]);
        }
        else
        {
            _penColors[0] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen02").toString();
        if(penValue.isEmpty())
        {
            _penColors[1] = QColor(_penColorsReset[1]);
        }
        else
        {
            _penColors[1] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen03").toString();
        if(penValue.isEmpty())
        {
            _penColors[2] = QColor(_penColorsReset[2]);
        }
        else
        {
            _penColors[2] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen04").toString();
        if(penValue.isEmpty())
        {
            _penColors[3] = QColor(_penColorsReset[3]);
        }
        else
        {
            _penColors[3] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen05").toString();
        if(penValue.isEmpty())
        {
            _penColors[4] = QColor(_penColorsReset[4]);
        }
        else
        {
            _penColors[4] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen06").toString();
        if(penValue.isEmpty())
        {
            _penColors[5] = QColor(_penColorsReset[5]);
        }
        else
        {
            _penColors[5] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen07").toString();
        if(penValue.isEmpty())
        {
            _penColors[6] = QColor(_penColorsReset[6]);
        }
        else
        {
            _penColors[6] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen08").toString();
        if(penValue.isEmpty())
        {
            _penColors[7] = QColor(_penColorsReset[7]);
        }
        else
        {
            _penColors[7] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen09").toString();
        if(penValue.isEmpty())
        {
            _penColors[8] = QColor(_penColorsReset[8]);
        }
        else
        {
            _penColors[8] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen10").toString();
        if(penValue.isEmpty())
        {
            _penColors[9] = QColor(_penColorsReset[9]);
        }
        else
        {
            _penColors[9] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen11").toString();
        if(penValue.isEmpty())
        {
            _penColors[10] = QColor(_penColorsReset[10]);
        }
        else
        {
            _penColors[10] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen12").toString();
        if(penValue.isEmpty())
        {
            _penColors[11] = QColor(_penColorsReset[11]);
        }
        else
        {
            _penColors[11] = QColor(penValue);
        }
        penValue.clear();

        penValue = setting.value("ChartPen13").toString();
        if(penValue.isEmpty())
        {
            _penColors[12] = QColor(_penColorsReset[12]);
        }
        else
        {
            _penColors[12] = QColor(penValue);
        }
        penValue.clear();

        // Save Legend Position Strings

        positionValue = setting.value("LegendPosition1").toString();
        if(positionValue.isEmpty())
        {
            _positions[0] = _defaultPositions[0];
        }
        else
        {
            _positions[0] = positionValue;
        }
        positionValue.clear();

        positionValue = setting.value("LegendPosition2").toString();
        if(positionValue.isEmpty())
        {
            _positions[1] = _defaultPositions[1];
            //
        }
        else
        {
            _positions[1] = positionValue;
            //
        }
        positionValue.clear();

        positionValue = setting.value("LegendPosition3").toString();

        if(positionValue.isEmpty())
        {
            _positions[2] = _defaultPositions[2];
            //
        }
        else
        {
            _positions[2] = positionValue;
            //
        }
        positionValue.clear();

        positionValue = setting.value("LegendPosition4").toString();
        if(positionValue.isEmpty())
        {
            _positions[3] = _defaultPositions[3];
        }
        else
        {
            _positions[3] = positionValue;
        }
        positionValue.clear();

        positionValue = setting.value("LegendPosition5").toString();
        if(positionValue.isEmpty())
        {
            _positions[4] = _defaultPositions[4];
        }
        else
        {
            _positions[4] = positionValue;
        }
        positionValue.clear();

        positionValue = setting.value("LegendPosition6").toString();
        if(positionValue.isEmpty())
        {
            _positions[5] = _defaultPositions[5];
        }
        else
        {
            _positions[5] = positionValue;
        }
        positionValue.clear();

        positionValue = setting.value("LegendPosition7").toString();
        if(positionValue.isEmpty())
        {
            _positions[6] = _defaultPositions[6];
        }
        else
        {
            _positions[6] = positionValue;
        }
        positionValue.clear();

        positionValue = setting.value("LegendPosition8").toString();
        if(positionValue.isEmpty())
        {
            _positions[7] = _defaultPositions[7];
        }
        else
        {
            _positions[7] = positionValue;
        }
        positionValue.clear();

        positionValue = setting.value("LegendPosition9").toString();
        if(positionValue.isEmpty())
        {
            _positions[8] = _defaultPositions[8];
        }
        else
        {
            _positions[8] = positionValue;
        }
        positionValue.clear();

        positionValue = setting.value("LegendPosition10").toString();
        if(positionValue.isEmpty())
        {
            _positions[9] = _defaultPositions[9];
        }
        else
        {
            _positions[9] = positionValue;
        }


        // Do the Geometry and check the layout for bad data
        _layout = setting.value("ChartStartupPosition").toRect();

        int w, h;
        w =_layout.width();
        h = _layout.height();

        if(h == 0 || w == 0 )
        {
           _layout = _defaultLayout;
        }
        _xGeoStart = _layout.x();


        _yGeoStart = _layout.y();


        _GeoWidth = _layout.width();


        _GeoHeight = _layout.height();



        setGeometry(_layout);

        setting.endGroup();

    }

    void HtcChart::setLegendText(int dType)
    {

       //qDebug() << "setting legend for datatype " << dType;

        // in service vars
        // -------------------------- //
        _positions.clear();
        _legendKeys.clear();

        _ciLegendPrefixes << "3vm_" << "10vm_";
        _ciUsedPrefixes << "3vm" << "10vm";

        // RI Legend parts
        _legendKeysRI.clear();
        _legendKeysRI << "PV" << "0_BL" << "0_H" << "0_V" << "90_H" << "90_V" << "180_H" << "180_V" << "270_H" << "270_V";

        _defaultPositionsRI.clear();
        _defaultPositionsRI << "Padding Value"  << "Baseline" << "0° Horiz" << "0° Vert" << "90° Horiz"
                              << "90° Vert" << "180° Horiz" << "180° Vert" << "270° Horiz" << "270° Vert";

        // CI Legend parts
        _legendKeysCI.clear();
        _legendKeysCI << "Padder" << "Baseline" << "CDN-LAN1" << "CDN-LAN2" << "CDN-USB1" << "CDN-USB2" << "CLAMP-GPIB" << "CLAMP-THUNDERBOLT" << "CDN-POWER" << "CDN-COAX" << "CDN-DC";

        _defaultPositionsCI.clear();
        _defaultPositionsCI << "Padding Value"  << "Baseline" << "CDN-S8 (LAN1)" << "CDN-S8 (LAN2)" << "CDN-S4 (USB1)" << "CDN-S4 (USB2)" << "Current Clamp (GPIB)"
                                << "Current Clamp (Thunderbolt)" << "CDN-M3 (AC mains)" << "CDN-S1 (Coaxial cable)" << "CDN-M2 (d.c. power)";


        if(dType == RIdataType)
        {
            _positions = _defaultPositionsRI;
            _legendKeys = _legendKeysRI;
        }
        else
        {
            _positions = _defaultPositionsCI;
            _legendKeys = _legendKeysCI;
        }

    }

    QString HtcChart::stripMatch(QString target, QStringList strips)
    {
        QString answer;
        answer.clear();
        int len, stripLen;

        for (int i = 0; i< strips.count(); i++)

        if (target.contains(strips.at(i)))
        {
            len = target.length();
            stripLen = strips.at(i).length();
            _CIRangInService = i;

            answer = target.right(len - stripLen);
            break;

        }
        else
        {
            answer = target;
        }

        //qDebug() << "stripped is " << answer;

        return answer;
    }

    QPen HtcChart::getPenStyle(int style)
    {
        return QPen(penStyles[style]);
    }

    void HtcChart::createPen(double baseValue, QString header)
    {

        QString currentRow;
        QString test;
        int start = _currentHeaderRow +1;

        _currentHeaderList.append(header);

        _penWidths[_currentHeaderList.count() - 2] = _addedPenSize;

        //qDebug() << "master count before " << listMasterList();

        test = getLastChar(_masterlist.at(1));


        if(test == ",")
        {
            for (int i = start; i < _masterlist.count(); i++)
            {
               currentRow = _masterlist.at(i);
               currentRow.append(QString::number(baseValue));
               currentRow.append(_dataFileDelim);
               _masterlist[i] = currentRow;

            }
        }
        else
        {
            for (int i = start; i < _masterlist.count(); i++)
            {
               currentRow = _masterlist.at(i);
               currentRow.append(_dataFileDelim);
               currentRow.append(QString::number(baseValue));
               _masterlist[i] = currentRow;

            }

        }

        updateHeaderCount();

        // reload the chart
        clearLayout(ui->chartLayout);
        initChart();

    }

    void HtcChart::updateHeaderCount()
    {
        _currentHeaderCount = _currentHeaderList.count();
    }

    void HtcChart::adjustGeometry()
    {
        int specialX = _chartXOffset * _dataSetIndex + _layout.x();

        _layout.setX(specialX);
        setGeometry(_layout);


    }

    void HtcChart::initChartScaleMemory()
    {

        _XAxisMinValue  = 9999999999;
        _XAxisMaxValue = -9999999999;
        _YAxisMinValue = 9999999999;
        _YAxisMaxValue = -9999999999;

    }

    double HtcChart::setMinFreq(double freq)
    {
        if (freq < _XAxisMinValue)
        {
            _XAxisMinValue = freq;
        }

        return _XAxisMinValue;

    }

    double HtcChart::setMaxFreq(double freq)
    {
        if (freq > _XAxisMaxValue)
        {
            _XAxisMaxValue = freq;
        }

        return _XAxisMaxValue;

    }

    double HtcChart::setMinLevel(double level)
    {

        if (level < _YAxisMinValue)
        {
            _YAxisMinValue = level;
        }

        return _YAxisMinValue;


    }

    double HtcChart::setMaxLevel(double level)
    {
        if (level > _YAxisMaxValue)
        {
            _YAxisMaxValue = level;
        }

        return _YAxisMaxValue;
        ;
    }

    int HtcChart::getYAxisScalingResolution()
    {
        //default resolution
        int temp = 2;
        int result = 2;

        int len;
        int pos;
        int delta;

        QString match = "e-";
        int offset = match.length();
        QString check = QString::number(_YAxisMinValue);

        if (check.contains(match))
        {
            len = check.length();
            pos = check.indexOf(match);
            delta = len - pos;

            QString whatsLeft = check.mid(pos + offset,delta);
            if(whatsLeft.toInt() > 0)
            {
                temp = whatsLeft.toInt() + 1;
            }

        }
        else
        {
            temp =  _yPrecision;
        }


        if(temp < _yPrecision)
        {
            temp = _yPrecision;
        }

        result = temp;


        return result;

    }

    int HtcChart::getXAxisScalingResolution()
    {
        return _xPrecision;
    }

void HtcChart::discoverChartScaleValues()
{
    // -------------------------------------- //
    // -------------------------------------- //
    // -------------------------------------- //
    // Need to add rescaler method to
    // fix Y Axis ranges that are < e-12
    // So they will render correctly
    // in the chart.
    //
    // -------------------------------------- //
    // -------------------------------------- //

    QStringList group;
    int start;
    double freq;
    double level;
    QString l1;

    double minFreq;
    double maxFreq;
    double minLevel;
    double maxLevel;

    if (!_masterlist.isEmpty())
    {
        start = _firstNumericRow;

        for (int i = start; i < _masterlist.count(); i++)
        {

            group = _masterlist[i].split(_dataFileDelim);

            for(int col = 0; col < group.count(); col++)
            {
                if(col==0)
                {
                    freq = QString(group.at(col)).toDouble()/getFreqRescaleValue();

                    if(!_UpdatingFromProperties)
                    {
                        minFreq = setMinFreq(freq);
                        maxFreq = setMaxFreq(freq);
                    }

                }
                else
                {
                    level = group.at(col).toDouble();
                    if(!_UpdatingFromProperties)
                    {
                        minLevel = setMinLevel(level);
                        maxLevel = setMaxLevel(level);
                    }

                }
            }


       }

    }

}

double HtcChart::getPaddingYValue()
{
    double result;
    double mult = _ChartScalePaddingValueY / 100;
    double delta = _YAxisMaxValue -_YAxisMinValue;

    result = delta * mult;

    return result;
}

void HtcChart::setYaxisPaddingValue()
{
    double result;
    double mult = _ChartScalePaddingValueY / 100;
    double delta = _YAxisMaxValue -_YAxisMinValue;

    result = delta * mult;

    _YAxisPaddingValue = result;
}

double HtcChart::getPaddedYMaxValue()
{

    double result = _YAxisMaxValue + _YAxisPaddingValue;
    return result;
}

double HtcChart::getPaddedYMinValue()
{
    double result = _YAxisMinValue - _YAxisPaddingValue;
    return result;
}

bool HtcChart::getXAxisPaddingEnabled()
{
    // _ChartRIScalePaddingValueX : the value
    // _ChartRIPaddingValueX : On/OFf

    bool result = false;

    if (_dataType == RIdataType)
    {
        if (_ChartRIPaddingValueX == 1 && _ChartRIScalePaddingValueX > 0)
        {
            result = true;
        }
    }
    else
    {
        if (_ChartCIPaddingValueX == 1 && _ChartCIScalePaddingValueX > 0)
        {
            result = true;
        }

    }

    return result;
}

double HtcChart::getPaddedXMaxValue()
{
    double result = _XAxisMaxValue;
    double factor, mult;
    QString chartType;





    if (_dataType == RIdataType)
    {
        factor = _ChartRIScalePaddingValueX / 100;
        mult = _RIXaxisMaxMult;
        chartType = "RI";

    }
    else
    {
        factor = _ChartCIScalePaddingValueX / 100;
        mult = _CIXaxisMaxMult;
        chartType = "CI";
    }

    if (getXAxisPaddingEnabled() == 1)
    {
        result = _XAxisMaxValue + (_XAxisMinValue * factor * mult);
    }


    return result;

}

double HtcChart::getPaddedXMinValue()
{
    double result = _XAxisMinValue;
    double factor;
    QString chartType;

    if (_dataType == RIdataType)
    {
        factor = _ChartRIScalePaddingValueX / 100;
        chartType = "RI";
    }
    else
    {
        factor = _ChartCIScalePaddingValueX / 100;
        chartType = "CI";
    }

    if (getXAxisPaddingEnabled() == 1)
    {
        result = _XAxisMinValue - (_XAxisMinValue * factor);
    }


    return result;

}



void HtcChart::clearLayout(QLayout *layout)
{
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0) {
    if(child->layout() != 0)
        clearLayout( child->layout() );
    else if(child->widget() != 0)
     delete child->widget();
        delete child;
    }

}

void HtcChart::assignChartToProperties()
{

}

QColor HtcChart::GetNewColor(QColor currentColor)
{
    QColor result = currentColor;

    QColor color = QColorDialog::getColor(currentColor, this);
       if( color.isValid() )
       {
         result = color;

       }

       return result;
}

QFont HtcChart::GetNewFont(QFont currentFont)
{
    QFont result = currentFont;
    bool ok;

        QFont font = QFontDialog::getFont(
                        &ok,
                        currentFont,
                        this,
                        tr("Pick a font") );
        if(font != currentFont)
        {
            result = font;
        }

        return result;
}

QPen HtcChart::GetNewGridLinesPen(QColor color, double size, Qt::PenStyle style)
{
    QPen result = QPen(QBrush(color), size,style, Qt::SquareCap, Qt::BevelJoin);
    return result;
}

QString HtcChart::StripQuotesFromString(QString wordToStrip)
{
    QString target = "";
    QString result = "";

    result = wordToStrip;

    target.append('"');

    if(wordToStrip.startsWith(target))
     {
            wordToStrip.remove(0,1);
            //qDebug() << "stripped begin";
     }
    if(wordToStrip.endsWith(target))
    {
        wordToStrip.remove(wordToStrip.size()-1,1);
        //qDebug() << "stripped end";
    }
    result = wordToStrip;

    return result;
}

void HtcChart::stripLastHeaderItem(int column)
{
    QStringList newList;

    if(!newList.isEmpty())
    {
        newList.clear();
    }

    for(int i = 0; i < column +1; i++)
    {
        newList.append(_currentHeaderList.at(i));
    }


    _currentHeaderList.clear();
    _currentHeaderList = newList;


}

void HtcChart::stripLastDataColumn(int column)
{
    QStringList newMasterList;
    QStringList rowGroup;
    QString line = "";

    int numberOfrows = _masterlist.count();

    if(!newMasterList.isEmpty())
    {
        newMasterList.clear();
    }

    for(int row = 0; row < numberOfrows; row++)
    {
        line.clear();

        if(row != 0)
        {
            rowGroup = _masterlist.at(row).split(_dataFileDelim);

            for(int col = 0; col < column; col++)
            {

                line.append(rowGroup.at(col));
                line.append(_dataFileDelim);
            }

            // do the last column
            line.append(rowGroup.at(column));

        }
        else
        {
            line.append(_masterlist.at(0));
        }

        newMasterList.append(line);
    }

    _masterlist.clear();
    _masterlist = newMasterList;

}

void HtcChart::setChartID()
{
    HTCChartDataFile df = HTCChartDataFile(_rawDataFileAndPath, _dataType);
    _ChartTestCode = df.getOrientationTestCode();
    _ChartModel = df.getOrientationEUTModel();
    _ChartSerial = df.getOrientationEUTSerial();

}

QString HtcChart::getSaveHeaderValues()
{
    //listHeaders();
    QString result = "";

    for (int i = 0; i < _currentHeaderList.count() - 1; i++)
    {
        result.append(_currentHeaderList.at(i));
        result.append(_dataFileDelim);
    }
    result.append(_currentHeaderList.at(_currentHeaderList.count() - 1));



    return result;
}

QString HtcChart::getChartTypeSaveString()
{
    QString result = "";
    //_saveChartTypeKey _saveItemDelimiter
    result.append(_saveChartTypeKey);
    result.append(_saveItemDelimiter);
    result.append(_chartBasicTypes[getDataType()]);

    return result;
}

QString HtcChart::getChartKeySaveString()
{
    QString result = "";
    result.append(_saveChartKey);
    result.append(_saveItemDelimiter);
    result.append(_ChartTestCode);
    result.append(_dataFileDelim);
    result.append(_ChartModel);
    result.append(_dataFileDelim);
    result.append(_ChartSerial);

    return result;

}

QStringList HtcChart::getSaveDataValues()
{
    QStringList data;
    int start = 0;

        // strip the header then
        // return all the rest of the rows
        if(!data.isEmpty())
        {
           data.clear();
        }

        if (_loadedChartFromFile == true)
        {
            start = _lastSettingsIDX + 1 ;
        }
        for (int i = start; i < _masterlist.count(); i++)
        {
            //qDebug() << "line of data ->" << _masterlist.at(i);
            data.append(_masterlist.at(i));
        }


        return data;
}

QString HtcChart::getChartTitleSaveString()
{
    QString result = "";
    result.append(_saveItemChartTitleKey);
    result.append(_saveItemDelimiter);
    result.append(_chartTitleText);

    return result;
}

QString HtcChart::getChartTitleConfigSaveString()
{
    QString result = "";
    QString fontName = _chartTitleTextFont.family();
    QString fontSize = QString::number(_chartTitleTextFont.pointSize());
    QString fontColor = _chartTitleTextColor.name();

    result.append(_saveItemChartTitleConfigKey);
    result.append(_saveItemDelimiter);
    result.append(fontName);
    result.append(_dataFileDelim);
    result.append(fontSize);
    result.append(_dataFileDelim);
    result.append(fontColor);

    return result;
}

QString HtcChart::getChartXAxisUnitsSaveString()
{
    QString result = "";
    result.append(_saveItemChartXAxisUnitsKey);
    result.append(_saveItemDelimiter);
    result.append(_chartXAxisUnitsText);

    return result;

}

QString HtcChart::getChartXAxisUnitsConfigSaveString()
{
    QString result = "";
    QString fontName = _chartXAxisUnitsTextFont.family();
    QString fontSize = QString::number(_chartXAxisUnitsTextFont.pointSize());
    QString fontColor = _chartXAxisUnitsBrush.color().name();


    result.append(_saveItemChartXAxisUnitsConfigKey);
    result.append(_saveItemDelimiter);
    result.append(fontName);
    result.append(_dataFileDelim);
    result.append(fontSize);
    result.append(_dataFileDelim);
    result.append(fontColor);

    return result;

}

QString HtcChart::getChartYAxisUnitsSaveString()
{
    QString result = "";
    result.append(_saveItemChartYAxisUnitsKey);
    result.append(_saveItemDelimiter);
    result.append(_chartYAxisUnitsText);

    return result;
}

QString HtcChart::getChartYAxisUnitsConfigSaveString()
{
    QString result = "";
    QString fontName = _chartYAxisUnitsTextFont.family();
    QString fontSize = QString::number(_chartYAxisUnitsTextFont.pointSize());
    QString fontColor = _chartYAxisUnitsBrush.color().name();


    result.append(_saveItemChartYAxisUnitsConfigKey);
    result.append(_saveItemDelimiter);
    result.append(fontName);
    result.append(_dataFileDelim);
    result.append(fontSize);
    result.append(_dataFileDelim);
    result.append(fontColor);

    return result;
}

QString HtcChart::getChartXAxisParamsSaveString()
{
    QString result = "";
    result.append(_saveItemChartXAxisParamsKey);
    result.append(_saveItemDelimiter);
    result.append(_chartXAxisLinLogScale);
    result.append(_dataFileDelim);
    result.append(_chartXAxisDecimalScientific);
    result.append(_dataFileDelim);
    result.append(_chartXAxisLabelColor.name());
    result.append(_dataFileDelim);
    result.append(_chartXAxisLabelFont.family());
    result.append(_dataFileDelim);
    result.append(QString::number(_chartXAxisLabelFont.pointSize()));
    result.append(_dataFileDelim);
    result.append(QString::number(_chartXAxisLabelRotation));

    return result;
}

QString HtcChart::getChartYAxisParamsSaveString()
{

    QString result = "";
    result.append(_saveItemChartYAxisParamsKey);
    result.append(_saveItemDelimiter);
    result.append(_chartYAxisLinLogScale);
    result.append(_dataFileDelim);
    result.append(_chartYAxisDecimalScientific);
    result.append(_dataFileDelim);
    result.append(_chartYAxisLabelColor.name());
    result.append(_dataFileDelim);
    result.append(_chartYAxisLabelFont.family());
    result.append(_dataFileDelim);
    result.append(QString::number(_chartYAxisLabelFont.pointSize()));
    result.append(_dataFileDelim);
    result.append(QString::number(_chartYAxisLabelRotation));



    return result;
}


QString HtcChart::getChartXAxisMinSaveString()
{
    QString result = "";

    result.append(_saveItemChartXMinValueKey);
    result.append(_saveItemDelimiter);
    result.append(QString::number(_XAxisMinValue));

    return result;

}

QString HtcChart::getChartXAxisMaxSaveString()
{

    QString result = "";

    result.append(_saveItemChartXMaxValueKey);
    result.append(_saveItemDelimiter);
    result.append(QString::number(_XAxisMaxValue));

    return result;
}

QString HtcChart::getChartYAxisMinSaveString()
{
    QString result = "";

    result.append(_saveItemChartYMinValueKey);
    result.append(_saveItemDelimiter);
    result.append(QString::number(_YAxisMinValue));

    return result;

}

QString HtcChart::getChartYAxisMaxSaveString()
{
    QString result = "";

    result.append(_saveItemChartYMaxValueKey);
    result.append(_saveItemDelimiter);
    result.append(QString::number(_YAxisMaxValue));

    return result;

}

QString HtcChart::getChartXAxisGridLinesSaveString()
{
    QString result = "";

    result.append(_saveItemChartXAxisGridLinesKey);
    result.append(_saveItemDelimiter);
    result.append(getAnswerForBool(_chartXAxisMajorGridLinesVisible));
    result.append(_dataFileDelim);
    result.append(QString::number(_chartXAxisMajorGridLinesCount));
    result.append(_dataFileDelim);
    result.append(getAnswerForBool(_chartXAxisMinorGridLinesVisible));
    result.append(_dataFileDelim);
    result.append(QString::number(_chartXAxisMinorGridLinesCount));
    result.append(_dataFileDelim);
    result.append(_chartXAxisMajorGridLinesColor.name());
    result.append(_dataFileDelim);
    result.append(_chartXAxisMinorGridLinesColor.name());
    result.append(_dataFileDelim);
    result.append(QString::number(_chartXAxisMinorGridLineSize));
    result.append(_dataFileDelim);
    result.append(QString::number(_chartXAxisMajorGridLineSize));


    return result;

}

QString HtcChart::getChartYAxisGridLinesSaveString()
{
    QString result = "";

    result.append(_saveItemChartYAxisGridLinesKey);
    result.append(_saveItemDelimiter);

    result.append(getAnswerForBool(_chartYAxisMajorGridLinesVisible));

    result.append(_dataFileDelim);
    result.append(QString::number(_chartYAxisMajorGridLinesCount));
    result.append(_dataFileDelim);
    result.append(getAnswerForBool(_chartYAxisMinorGridLinesVisible));
    result.append(_dataFileDelim);
    result.append(QString::number(_chartYAxisMinorGridLinesCount));
    result.append(_dataFileDelim);
    result.append(_chartYAxisMajorGridLinesColor.name());
    result.append(_dataFileDelim);
    result.append(_chartYAxisMinorGridLinesColor.name());
    result.append(_dataFileDelim);
    result.append(QString::number(_chartYAxisMinorGridLineSize));
    result.append(_dataFileDelim);
    result.append(QString::number(_chartYAxisMajorGridLineSize));
    return result;
}

QString HtcChart::getPenStatesSaveString()
{
    QString result = "";
    result.append(_saveItemChartPenStatesKey);
    result.append(_saveItemDelimiter);

    int numElements =  sizeof(_penStates)/sizeof(_penStates[0]);
    for(int i = 0; i < numElements - 1; i++)
    {
        result.append(QString::number(_penStates[i]));
        result.append(_dataFileDelim);
    }

    // do the last one
    result.append(QString::number(_penStates[numElements - 1]));

    return result;

}

QString HtcChart::getPenStylesSaveString()
{

    QString result = "";
    result.append(_saveItemChartPenStylesKey);
    result.append(_saveItemDelimiter);

    int numElements =  sizeof(_penStyles)/sizeof(_penStyles[0]);
    for(int i = 0; i < numElements - 1; i++)
    {
        result.append(QString::number(_penStyles[i]));
        result.append(_dataFileDelim);
    }

    // do the last one
    result.append(QString::number(_penStyles[numElements - 1]));

    return result;
}

QString HtcChart::getPenWidthSaveString()
{

    QString result = "";
    result.append(_saveItemChartPenWidthsKey);
    result.append(_saveItemDelimiter);

    int numElements =  sizeof(_penWidths)/sizeof(_penWidths[0]);
    for(int i = 0; i < numElements - 1; i++)
    {
        result.append(QString::number(_penWidths[i]));
        result.append(_dataFileDelim);
    }

    // do the last one
    result.append(QString::number(_penWidths[numElements - 1]));

    return result;
}

QString HtcChart::getPenColorsSaveString()
{
    QString result = "";
    QColor color;

    result.append(_saveItemChartPenColorsKey);
    result.append(_saveItemDelimiter);

    int numElements =  sizeof(_penColors)/sizeof(_penColors[0]);
    for(int i = 0; i < numElements - 1; i++)
    {
        color = _penColors[i];
        result.append(color.name());
        result.append(_dataFileDelim);
    }

    // do the last one
    color = _penColors[numElements - 1];
    result.append(color.name());

    return result;

}

QString HtcChart::getParentDirForPath(QString path)
{
    QString delim = "/";
    QStringList temp = path.split(delim);
    QString result = "";

    for (int i = 0; i < temp.count()-1; i++)
    {
        result.append(temp.at(i));
        result.append(delim);
    }

    if(_loadedChartFromFile)
    {
        result.append(temp.at(temp.count() - 1));
        result.append(delim);
    }

    return result;

}

void HtcChart::showNoAccessFileForSavingMessage(QString file)
{
    QMessageBox msgBox;
    QString message = "";

    message.append("I can't open the file you selected [");
    message.append(file);
    message.append("] Maybe someone else already has it open ");
    message.append(" or you've lost access to the path.");


    msgBox.setText(message);
    msgBox.setInformativeText("Can't access the selected file & path.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();
    qDebug() << "messagebox return - " << ret;
}

void HtcChart::setPropertiesFromOpenFile()
{

    QStringList searchlist;
    QStringList answer;
    QStringList penParts;
    QColor color;

    answer.clear();

    qDebug() << "in setPropertiesFromOpenFile";

    qDebug() << "loading a chart from a stored file";

    answer = _masterlist.at(_chartTypeIDX).split(_saveItemDelimiter); // 1

    if (answer.contains(_saveChartTypeKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);

        _ChartType = penParts.at(0);

        if(_ChartType == "RI")
        {
            setDataType(RIdataType);
        }
        else
        {
            setDataType(CIdataType);
        }

        setLegendText(getDataType());

    }
    else
    {
        // We should probably ahve some error handling here
        // Cancel loading if the Chart type is unknown.
        _ChartType = "UNKNOWN_TEST_CODE";
    }



    penParts.clear();
    answer.clear();

    answer = _masterlist.at(_chartKeyIDX).split(_saveItemDelimiter); // 2

    if (answer.contains(_saveChartKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);

        _ChartTestCode = penParts.at(0);
        _ChartModel = penParts.at(1);
        _ChartSerial = penParts.at(2);
    }
    else
    {
        //qDebug() << "Chart key NOT found";
        _ChartTestCode = "UNKNOWN_TEST_CODE";
        _ChartModel = "UNKNOWN_MODEL-NUMBER";
        _ChartSerial = "UNKNOWN_SERIAL-NUMBER";
    }


    answer.clear();

    answer = _masterlist.at(_chartTitleRowIDX).split(_saveItemDelimiter); // 3


    if(answer.contains(_saveItemChartTitleKey))
        {

            if(answer.count() > 2)
            {
                for (int i = 1; i < answer.count(); i++)
                {
                    _chartTitleText.append(answer.at(i));
                }
            }
            else
            {
                _chartTitleText = answer.at(1);
            }
        }
        else
        {

            _chartTitleText = "UnKnown";
        }


    answer.clear();
    penParts.clear();

    answer = _masterlist.at(_chartTitleConfigRowIDX).split(_saveItemDelimiter); // 4

    if (answer.contains(_saveItemChartTitleConfigKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);
        _chartTitleTextFont.setFamily(penParts.at(0));
        _chartTitleTextFont.setPointSize(penParts.at(1).toInt());
        _chartTitleTextColor = QColor(penParts.at(2));
    }
    else
    {
        _chartTitleTextFont.setFamily("Arial");

        _chartTitleTextFont.setPointSize(12);
        _chartTitleTextColor = QColor("#000000");

    }

    answer.clear();
    penParts.clear();


    answer = _masterlist.at(_chartXaxisUnitsIDX).split(_saveItemDelimiter); // 5

    if (answer.contains(_saveItemChartXAxisUnitsKey))
    {
        if(answer.count() > 2)
        {
            for (int i = 1; i < answer.count(); i++)
            {
                _chartXAxisUnitsText.append(answer.at(i));
            }
        }
        else
        {
            _chartXAxisUnitsText = answer.at(1);

        }
    }
    else
    {
        _chartXAxisUnitsText = "Test Frequency";
    }

    answer.clear();
    penParts.clear();

    answer = _masterlist.at(_chartXaxisUnitsConfigIDX).split(_saveItemDelimiter); // 6

    if (answer.contains(_saveItemChartXAxisUnitsConfigKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);
        _chartXAxisUnitsTextFont.setFamily(penParts.at(0));
        _chartXAxisUnitsTextFont.setPointSize(penParts.at(1).toInt());
        _chartXAxisUnitsBrush.setColor(penParts.at(2));
    }
    else
    {
        _chartXAxisUnitsTextFont.setFamily("Arial");

        _chartXAxisUnitsTextFont.setPointSize(12);
        _chartXAxisUnitsBrush.setColor("#000000");

    }

    answer.clear();
    penParts.clear();

    answer = _masterlist.at(_chartYaxisUnitsIDX).split(_saveItemDelimiter); // 7

    if(answer.contains(_saveItemChartYAxisUnitsKey))
    {
        if(answer.count() > 2)
        {
            for (int i = 1; i < answer.count(); i++)
            {
                _chartYAxisUnitsText.append(answer.at(i));
            }
        }
        else
        {
            _chartYAxisUnitsText = answer.at(1);

        }
    }
    else
    {
         _chartYAxisUnitsText = "Unknown Level?";
    }

    answer = _masterlist.at(_chartYaxisUnitsConfigIDX).split(_saveItemDelimiter); // 8

    if (answer.contains(_saveItemChartYAxisUnitsConfigKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);
        _chartYAxisUnitsTextFont.setFamily(penParts.at(0));
        _chartYAxisUnitsTextFont.setPointSize(penParts.at(1).toInt());
        _chartYAxisUnitsBrush.setColor(penParts.at(2));
    }
    else
    {
        _chartYAxisUnitsTextFont.setFamily("Arial");

        _chartYAxisUnitsTextFont.setPointSize(12);
        _chartYAxisUnitsBrush.setColor("#000000");

    }


    answer.clear();
    penParts.clear();

    answer = _masterlist.at(_chartXAxisParamsIDX).split(_saveItemDelimiter); // 9

    if (answer.contains(_saveItemChartXAxisParamsKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);
        _chartXAxisLinLogScale = penParts.at(0);
        _chartXAxisDecimalScientific = penParts.at(1);
        color = QColor(penParts.at(2));
        _chartXAxisLabelColor = color;
        _chartXAxisLabelFont.setFamily(penParts.at(3));
        _chartXAxisLabelFont.setPointSize(penParts.at(4).toInt());
        _chartXAxisLabelRotation = penParts.at(5).toInt();

    }
    else
    {
        _chartXAxisLinLogScale = "LIN";
        _chartXAxisDecimalScientific = "DEC";
        color = QColor("#000000");
        _chartXAxisLabelColor = color;
        _chartXAxisLabelFont.setFamily("Arial");
        _chartXAxisLabelFont.setPointSize(12);
        _chartXAxisLabelRotation = 45;
    }

    answer.clear();
    penParts.clear();

    answer = _masterlist.at(_chartYAxisParamsIDX).split(_saveItemDelimiter); // 10

    if (answer.contains(_saveItemChartYAxisParamsKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);
        _chartYAxisLinLogScale = penParts.at(0);
        _chartYAxisDecimalScientific = penParts.at(1);
        color = QColor(penParts.at(2));
        _chartYAxisLabelColor = color;
        _chartYAxisLabelFont.setFamily(penParts.at(3));
        _chartYAxisLabelFont.setPointSize(penParts.at(4).toInt());
        _chartYAxisLabelRotation = penParts.at(5).toInt();

    }
    else
    {
        _chartYAxisLinLogScale = "LIN";
        _chartYAxisDecimalScientific = "DEC";
        color = QColor("#000000");
        _chartYAxisLabelColor = color;
        _chartYAxisLabelFont.setFamily("Arial");
        _chartYAxisLabelFont.setPointSize(12);
        _chartYAxisLabelRotation = 0;
    }

    answer.clear();
    penParts.clear();

    answer = _masterlist.at(_chartXaxisMinIDX).split(_saveItemDelimiter); // 11

    if (answer.contains(_saveItemChartXMinValueKey))
    {
       _XAxisMinValue = answer.at(1).toDouble();
    }
    else
    {
        _XAxisMinValue = 80;
    }


    answer.clear();

    answer = _masterlist.at(_chartXaxisMaxIDX).split(_saveItemDelimiter); // 12

    if(answer.contains(_saveItemChartXMaxValueKey))
    {
        _XAxisMaxValue = answer.at(1).toDouble();
    }
    else
    {
        _XAxisMaxValue = 6000;
    }

    answer.clear();

    answer = _masterlist.at(_chartYaxisMinIDX).split(_saveItemDelimiter); // 13

    if(answer.contains(_saveItemChartYMinValueKey))
    {
        _YAxisMinValue = answer.at(1).toDouble();
    }
    else
    {
        _YAxisMinValue = 0;
    }


    answer.clear();

    answer = _masterlist.at(_chartYaxisMaxIDX).split(_saveItemDelimiter); // 14

    if(answer.contains(_saveItemChartYMaxValueKey))
    {
        _YAxisMaxValue = answer.at(1).toDouble();
    }
    else
    {
        _YAxisMaxValue = 100;
    }

    answer.clear();
    penParts.clear();

    answer = _masterlist.at(_chartXAxisGridLines).split(_saveItemDelimiter);  // 15

    if (answer.contains(_saveItemChartXAxisGridLinesKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);
        _chartXAxisMajorGridLinesVisible = getBoolForAnswer(penParts.at(0));
        _chartXAxisMajorGridLinesCount = penParts.at(1).toInt();
        _chartXAxisMinorGridLinesVisible = getBoolForAnswer(penParts.at(2));
        _chartXAxisMinorGridLinesCount = penParts.at(3).toInt();

        color = QColor(penParts.at(4));
        _chartXAxisMajorGridLinesColor = color;

        color = QColor(penParts.at(5));
        _chartXAxisMinorGridLinesColor = color;

        _chartXAxisMinorGridLineSize = penParts.at(6).toInt();
        _chartXAxisMinorGridLinesPen = GetNewGridLinesPen(_chartXAxisMinorGridLinesColor, _chartXAxisMinorGridLineSize, Qt::SolidLine);
        _chartXAxisMajorGridLineSize = penParts.at(7).toInt();
        _chartXAxisMajorGridLinesPen = GetNewGridLinesPen(_chartXAxisMajorGridLinesColor, _chartXAxisMajorGridLineSize, Qt::SolidLine);

    }
    else
    {
        _chartXAxisMajorGridLinesVisible = true;
        _chartXAxisMajorGridLinesCount = 11;
        _chartXAxisMinorGridLinesVisible = false;
        _chartXAxisMinorGridLinesCount = 5;

        color = QColor("#000000");
        _chartXAxisMajorGridLinesColor = color;

        color = QColor("#000000");
        _chartXAxisMinorGridLinesColor = color;

        _chartXAxisMinorGridLineSize = 2;
        _chartXAxisMinorGridLinesPen = GetNewGridLinesPen(_chartXAxisMinorGridLinesColor, _chartXAxisMinorGridLineSize, Qt::SolidLine);
        _chartXAxisMajorGridLineSize = 1;
        _chartXAxisMajorGridLinesPen = GetNewGridLinesPen(_chartXAxisMajorGridLinesColor, _chartXAxisMajorGridLineSize, Qt::SolidLine);

    }

    answer.clear();
    penParts.clear();

    answer = _masterlist.at(_chartYAxisGridLines).split(_saveItemDelimiter);  // 16

    if (answer.contains(_saveItemChartYAxisGridLinesKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);
        _chartYAxisMajorGridLinesVisible = getBoolForAnswer(penParts.at(0));
        _chartYAxisMajorGridLinesCount = penParts.at(1).toInt();
        _chartYAxisMinorGridLinesVisible = getBoolForAnswer(penParts.at(2));
        _chartYAxisMinorGridLinesCount = penParts.at(3).toInt();

        color = QColor(penParts.at(4));
        _chartYAxisMajorGridLinesColor = color;

        color = QColor(penParts.at(5));
        _chartYAxisMinorGridLinesColor = color;

        _chartYAxisMinorGridLineSize = penParts.at(6).toInt();
        _chartYAxisMinorGridLinesPen = GetNewGridLinesPen(_chartYAxisMinorGridLinesColor, _chartYAxisMinorGridLineSize, Qt::SolidLine);
        _chartYAxisMajorGridLineSize = penParts.at(7).toInt();
        _chartYAxisMajorGridLinesPen = GetNewGridLinesPen(_chartYAxisMajorGridLinesColor, _chartYAxisMajorGridLineSize, Qt::SolidLine);

    }
    else
    {
        qDebug() << "didn't find Y Axis match string";
        _chartYAxisMajorGridLinesVisible = true;
        _chartYAxisMajorGridLinesCount = 11;
        _chartYAxisMinorGridLinesVisible = false;
        _chartYAxisMinorGridLinesCount = 5;
        color = QColor("#000000");
        _chartYAxisMajorGridLinesColor = color;
        color = QColor("#000000");
        _chartYAxisMinorGridLinesColor = color;
        _chartYAxisMajorGridLineSize = 2;
        _chartYAxisMinorGridLinesPen = GetNewGridLinesPen(_chartYAxisMinorGridLinesColor, _chartYAxisMinorGridLineSize, Qt::SolidLine);
        _chartYAxisMinorGridLineSize = 1;
        _chartYAxisMajorGridLinesPen = GetNewGridLinesPen(_chartYAxisMajorGridLinesColor, _chartYAxisMajorGridLineSize, Qt::SolidLine);

    }


    answer.clear();
    penParts.clear();

    answer = _masterlist.at(_chartPenStatesIDX).split(_saveItemDelimiter); // 17

    if(answer.contains(_saveItemChartPenStatesKey))
    {

        penParts = answer.at(1).split(_dataFileDelim);
        int numElements =  sizeof(_penStates)/sizeof(_penStates[0]);

        for(int i = 0; i < numElements - 1; i++)
        {
            _penStates[i] = penParts.at(i).toInt();
        }
        // do the last
        _penStates[numElements - 1] = penParts.at(numElements - 1).toInt();
    }
    else
    {
        for (int i = 0; i < 9; i++)
        {
           _penStates[i] = 1;
        }
    }

    answer.clear();

    answer = _masterlist.at(_chartPenStylesIDX).split(_saveItemDelimiter); // 18

    if(answer.contains(_saveItemChartPenStylesKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);

        int numElements =  sizeof(_penStyles)/sizeof(_penStyles[0]);
        for(int i = 0; i < numElements - 1; i++)
        {
            _penStyles[i] = penParts.at(i).toInt();
        }
        // do the last
        _penStyles[numElements - 1] = penParts.at(numElements - 1).toInt();
    }
    else
    {
        for (int i = 0; i < 9; i++)
        {
           _penStyles[i] = 1;
        }
    }

    answer.clear();

    answer = _masterlist.at(_chartPenWidthsIDX).split(_saveItemDelimiter); // 19

    if(answer.contains(_saveItemChartPenWidthsKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);

        int numElements =  sizeof(_penWidths)/sizeof(_penWidths[0]);
        for(int i = 0; i < numElements - 1; i++)
        {
            _penWidths[i] = penParts.at(i).toInt();
        }
        // do the last
        _penWidths[numElements - 1] = penParts.at(numElements - 1).toInt();
    }
    else
    {
        for (int i = 0; i < 9; i++)
        {
           _penWidths[i] = 1;
        }
    }

    answer.clear();

    answer = _masterlist.at(_chartPenColorsIDX).split(_saveItemDelimiter); // 20

    if(answer.contains(_saveItemChartPenColorsKey))
    {
        penParts = answer.at(1).split(_dataFileDelim);
        // color
        int numElements =  sizeof(_penColors)/sizeof(_penColors[0]);
        for(int i = 0; i < numElements - 1; i++)
        {
            color = QColor(penParts.at(i));
            _penColors[i] = color;
        }
        // do the last
        color = QColor(penParts.at(numElements - 1));
        _penColors[numElements - 1] = color;
    }
    else
    {
        // do nothing
        // use the default list
    }


}

int HtcChart::getfolderCountForPath(QString filename)
{
    QStringList list = filename.split("/");
    int result = list.count();


    return result;

}

QString HtcChart::getAnswerForBool(bool value)
{
    QString result = "false";

    if(value == true)
    {
        result = "true";
    }

    return result;

}

bool HtcChart::getBoolForAnswer(QString value)
{
    bool result = false;

    if(value == "true" )
    {
        result = true;
    }

    return result;
}

QString HtcChart::getLastChar(QString testValue)
{
    int len = testValue.length();
    QString lastChar = testValue.at(len-1);
    qDebug() << "test String == " << testValue;

    return lastChar;

}



void HtcChart::listKeys()
{

    qDebug() << "last header IDX" << _currentHeaderList.count()-1
             << " is " << _currentHeaderList.at(_currentHeaderList.count()-1);
    qDebug() << "last position IDX" << _positions.count()-1
             << " is " << _positions.at(_positions.count()-1);
    qDebug() << "last Key IDX" << _legendKeys.count()-1
             << " is " << _legendKeys.at(_legendKeys.count()-1);


}

int HtcChart::listMasterList()
{
    qDebug() << "<------------------ Listing Master List ------------------>";

    for(int i = 0; i < _masterlist.count(); i++)
    {
        qDebug() << "master-" << i << _masterlist.at(i);
    }

    return _masterlist.count();
}

int HtcChart::listHeaderList()
{
    qDebug() << "<------------- Listing header ----------------->";

    for(int i = 0; i < _currentHeaderList.count(); i++)
    {
        qDebug() << "header-" << i << ": " << _currentHeaderList.at(i);
    }

    return _currentHeaderList.count();
}

//
// Latest work
//
void HtcChart::on_btnSaveImage_clicked()
{
    QPixmap p = chartView->grab();

    QString path = getProperPath(_basePath);
    QString parentFolder = path;
//    QFileInfo info = QFileInfo(_rawDataFileAndPath);
//    QString path = info.path();
    QString outputFileName;

    // TODO Fix this like we did for
//    QString parentFolder = getParentDirForPath(path);
    QString imagefileExtension = ".png";
    QString delim = "_";

    outputFileName.append(parentFolder);
    outputFileName.append(_ChartTestCode);
    outputFileName.append(delim);
    outputFileName.append(_ChartModel);
    outputFileName.append(delim);
    outputFileName.append(_ChartSerial);
    outputFileName.append(delim);
    outputFileName.append(_chartYAxisUnitsText);
    outputFileName.append(imagefileExtension);

    qDebug() << "saving image using " << outputFileName;

    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), outputFileName, tr("Images (*.png)"));
    p.save(filename, "PNG");


}


void HtcChart::ChartTitleText(QString title)
{

    if (_chartTitleText != title)
    {
        _UpdatingFromProperties = true;
        _chartTitleText = title;
        clearLayout(ui->chartLayout);
        initChart();
    }


}

void HtcChart::ChartTitleTextFont(QFont font)
{

    if (_chartTitleTextFont != font)
    {
        _UpdatingFromProperties = true;
        _chartTitleTextFont = font;

        clearLayout(ui->chartLayout);
        initChart();
    }

}

void HtcChart::ChartTitleTextColor(QColor color)
{

    if (_chartTitleTextColor != color)
    {
        _UpdatingFromProperties = true;
        _chartTitleTextColor = color;

        clearLayout(ui->chartLayout);
        initChart();
    }

}

void HtcChart::HTCChartXAxisUnitsText(QString text)
{

    if (_chartXAxisUnitsText != text)
    {
        _UpdatingFromProperties = true;
        _chartXAxisUnitsText =  text;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartXAxisUnitsTextFont(QFont font)
{

    if (_chartXAxisUnitsTextFont != font)
    {
        _UpdatingFromProperties = true;
        _chartXAxisUnitsTextFont = font;
        clearLayout(ui->chartLayout);
        initChart();
    }

}

void HtcChart::HTCChartXAxisUnitsTextColor(QColor color)
{

    if (_chartXAxisUnitsBrush.color() != color)
    {
        _UpdatingFromProperties = true;
        _chartXAxisUnitsBrush.setColor(color);
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartXAxisLabelsTextRotation(qint32 value)
{
    if (_chartXAxisLabelRotation != value)
    {
        _UpdatingFromProperties = true;
        _chartXAxisLabelRotation = value;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartXAxisLabelsTextFont(QFont font)
{
    if (_chartXAxisLabelFont != font)
    {
        _UpdatingFromProperties = true;
        _chartXAxisLabelFont = font;

        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartXAxisLabelsTextColor(QColor color)
{
    if (_chartXAxisLabelColor != color)
    {
        _UpdatingFromProperties = true;
        _chartXAxisLabelColor = color;
        clearLayout(ui->chartLayout);
        initChart();
    }
}


void HtcChart::HTCChartXAxisLabelsTextScaleMin(double value)
{
//    QString v1 = QString::number(value);
//    QString v2 = QString::number(_XAxisMinValue);
    if (value < _XAxisMaxValue)
    {

        _UpdatingFromProperties = true;
        _ChartPaddingValueY = 0;
        _XAxisMinValue = value;

        clearLayout(ui->chartLayout);
        initChart();
    }

}

void HtcChart::HTCChartXAxisLabelsTextScaleMax(double value)
{
//    QString v1 = QString::number(value);
//    QString v2 = QString::number(_XAxisMaxValue);
    if (value > _XAxisMinValue)
    {


        _UpdatingFromProperties = true;
        _ChartPaddingValueY = 0;
        _XAxisMaxValue = value;

        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartXMajorThickness(int arg1)
{
    if (_chartXAxisMajorGridLineSize != arg1)
    {
        _UpdatingFromProperties = true;
        _chartXAxisMajorGridLineSize = arg1;
        _chartXAxisMajorGridLinesPen = GetNewGridLinesPen(_chartXAxisMajorGridLinesColor,
                                                          _chartXAxisMajorGridLineSize, Qt::SolidLine);

        clearLayout(ui->chartLayout);
        initChart();

    }
}

void HtcChart::HTCChartXMinorThickness(int arg1)
{
    if (_chartXAxisMinorGridLineSize != arg1)
    {
        _UpdatingFromProperties = true;
        _chartXAxisMinorGridLineSize = arg1;
        _chartXAxisMinorGridLinesPen = GetNewGridLinesPen(_chartXAxisMinorGridLinesColor,
                                                          _chartXAxisMinorGridLineSize, Qt::SolidLine);

        clearLayout(ui->chartLayout);
        initChart();

    }
}

void HtcChart::HTCChartXAxisMajorTicsColor(QColor color)
{
    if(_chartXAxisMajorGridLinesColor != color)
    {
        _UpdatingFromProperties = true;
        _chartXAxisMajorGridLinesColor = color;
        _chartXAxisMajorGridLinesPen = GetNewGridLinesPen(_chartXAxisMajorGridLinesColor,
                                                          _chartXAxisMajorGridLineSize, Qt::SolidLine);

        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartXAxisMinorTicsColor(QColor color)
{
    if(_chartXAxisMinorGridLinesColor != color)
    {
        _UpdatingFromProperties = true;
        _chartXAxisMinorGridLinesColor = color;
        _chartXAxisMinorGridLinesPen = GetNewGridLinesPen(_chartXAxisMinorGridLinesColor,
                                                          _chartXAxisMinorGridLineSize, Qt::SolidLine);
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartXMajorTicsEnabled(bool checked)
{
    if(_chartXAxisMajorGridLinesVisible != checked)
    {
        _UpdatingFromProperties = true;
        _chartXAxisMajorGridLinesVisible = checked;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartXMinorTicsEnabled(bool checked)
{
    if (_chartXAxisMinorGridLinesVisible != checked)
    {
        _UpdatingFromProperties = true;
        _chartXAxisMinorGridLinesVisible = checked;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartXMinorTicsValue(double arg1)
{
    int value = static_cast<int>(arg1);
    if(_chartXAxisMinorGridLinesCount != value)
    {
        _UpdatingFromProperties = true;
        _chartXAxisMinorGridLinesCount = value;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartXMajorTicsValue(double arg1)
{
    int value = static_cast<int>(arg1);
    if (_chartXAxisMajorGridLinesCount != value)
    {
        _UpdatingFromProperties = true;
        _chartXAxisMajorGridLinesCount = value;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartXLogChart(bool checked)
{

    _UpdatingFromProperties = true;
    _chartXAxisLinLogScale = "LOG";
    clearLayout(ui->chartLayout);
    initChart();
}

void HtcChart::HTCChartXLinChart(bool checked)
{

    _UpdatingFromProperties = true;
    _chartXAxisLinLogScale = "LIN";
    clearLayout(ui->chartLayout);
    initChart();
}

void HtcChart::HTCChartXDecimalChart(bool checked)
{
    //_UpdatingFromProperties = true;
    _chartXAxisDecimalScientific = "DEC";
    clearLayout(ui->chartLayout);
    initChart();
}

void HtcChart::HTCChartXScientificChart(bool checked)
{
    //_UpdatingFromProperties = true;
    _chartXAxisDecimalScientific = "SCI";
    clearLayout(ui->chartLayout);
    initChart();

}

void HtcChart::HTCChartYAxisUnitsText(QString text)
{
    if (_chartYAxisUnitsText != text)
    {
        _UpdatingFromProperties = true;
        _chartYAxisUnitsText =  text;
        clearLayout(ui->chartLayout);
        initChart();
    }
}


void HtcChart::HTCChartYAxisUnitsTextFont(QFont font)
{
    if (_chartYAxisUnitsTextFont != font)
    {
        _UpdatingFromProperties = true;
        _chartYAxisUnitsTextFont = font;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYAxisUnitsTextColor(QColor color)
{
    if (_chartYAxisUnitsBrush.color() != color)
    {
        _UpdatingFromProperties = true;
        _chartYAxisUnitsBrush.setColor(color);
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYAxisLabelsTextRotation(qint32 value)
{
    if (_chartYAxisLabelRotation != value)
    {
        _UpdatingFromProperties = true;
        _chartYAxisLabelRotation = value;
        clearLayout(ui->chartLayout);
        initChart();
    }

}

void HtcChart::HTCChartYAxisLabelsTextFont(QFont font)
{
    if (_chartYAxisLabelFont != font)
    {
        _UpdatingFromProperties = true;
        _chartYAxisLabelFont = font;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYAxisLabelsTextColor(QColor color)
{
    if (_chartYAxisLabelColor != color)
    {
        _UpdatingFromProperties = true;
        _chartYAxisLabelColor = color;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYAxisLabelsTextScaleMin(double value)
{
//    QString v1 = QString::number(value);
//    QString v2 = QString::number(_YAxisMinValue);
    //if (v1 != v2)

    if(value < _YAxisMaxValue)
    {

        _UpdatingFromProperties = true;
        _ChartPaddingValueY = 0;
        _YAxisMinValue = value;

        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYAxisLabelsTextScaleMax(double value)
{
//    QString v1 = QString::number(value);
//    QString v2 = QString::number(_YAxisMaxValue);
//    if (v1 != v2)

    if(value >_YAxisMinValue)
    {
        _UpdatingFromProperties = true;
        _ChartPaddingValueY = 0;
        _YAxisMaxValue = value;

        //qDebug() << "Received Y Max request and am updating with " << _YAxisMaxValue;
        clearLayout(ui->chartLayout);
        initChart();
    }

}


void HtcChart::HTCChartYMajorThickness(int arg1)
{

    if (_chartYAxisMajorGridLineSize != arg1)
    {
        _UpdatingFromProperties = true;
        _chartYAxisMajorGridLineSize = arg1;
        _chartYAxisMajorGridLinesPen = GetNewGridLinesPen(_chartYAxisMajorGridLinesColor,
                                                          _chartYAxisMajorGridLineSize, Qt::SolidLine);

        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYMinorThickness(int arg1)
{

   if (_chartYAxisMinorGridLineSize != arg1)
   {
       _UpdatingFromProperties = true;
       _chartYAxisMinorGridLineSize = arg1;
       _chartYAxisMinorGridLinesPen = GetNewGridLinesPen(_chartYAxisMinorGridLinesColor,
                                                         _chartYAxisMinorGridLineSize, Qt::SolidLine);

       clearLayout(ui->chartLayout);
       initChart();
   }
}

void HtcChart::HTCChartYAxisMajorTicsColor(QColor color)
{
    if(_chartYAxisMajorGridLinesColor != color)
    {
        _UpdatingFromProperties = true;
        _chartYAxisMajorGridLinesColor = color;
        _chartYAxisMajorGridLinesPen = GetNewGridLinesPen(_chartYAxisMajorGridLinesColor,
                                                          _chartYAxisMajorGridLineSize, Qt::SolidLine);

        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYAxisMinorTicsColor(QColor color)
{
    if(_chartYAxisMinorGridLinesColor != color)
    {
        _UpdatingFromProperties = true;
        _chartYAxisMinorGridLinesColor = color;
        _chartYAxisMinorGridLinesPen = GetNewGridLinesPen(_chartYAxisMinorGridLinesColor,
                                                          _chartYAxisMinorGridLineSize, Qt::SolidLine);

        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYMajorTicsEnabled(bool checked)
{
    if(_chartYAxisMajorGridLinesVisible != checked)
    {
        _UpdatingFromProperties = true;
        _chartYAxisMajorGridLinesVisible = checked;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYMinorTicsEnabled(bool checked)
{
    if(_chartYAxisMinorGridLinesVisible != checked)
    {
        _UpdatingFromProperties = true;
        _chartYAxisMinorGridLinesVisible = checked;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYMinorTicsValue(double arg1)
{
    int value = static_cast<int>(arg1);
    if(_chartYAxisMinorGridLinesCount != value)
    {
        _UpdatingFromProperties = true;
        _chartYAxisMinorGridLinesCount = value;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYMajorTicsValue(double arg1)
{
    int value = static_cast<int>(arg1);
    if(_chartYAxisMajorGridLinesCount != value)
    {
        _UpdatingFromProperties = true;
        _chartYAxisMajorGridLinesCount = value;
        clearLayout(ui->chartLayout);
        initChart();
    }
}

void HtcChart::HTCChartYLogChart(bool checked)
{


    _UpdatingFromProperties = true;
    _chartYAxisLinLogScale = "LOG";
    clearLayout(ui->chartLayout);
    initChart();
}

void HtcChart::HTCChartYLinChart(bool checked)
{

    _UpdatingFromProperties = true;
    _chartYAxisLinLogScale = "LIN";
    clearLayout(ui->chartLayout);


    initChart();
}

void HtcChart::HTCChartYDecimalChart(bool checked)
{
    //_UpdatingFromProperties = true;
    _chartYAxisDecimalScientific = "DEC";
    clearLayout(ui->chartLayout);
    initChart();
}

void HtcChart::HTCChartYScientificChart(bool checked)
{

    //_UpdatingFromProperties = true;
    _chartYAxisDecimalScientific = "SCI";
    clearLayout(ui->chartLayout);
    initChart();
}

void HtcChart::HTCCHartXPrecisionChanged(int res)
{
    _UpdatingFromProperties = true;
    _xPrecision = res;

    clearLayout(ui->chartLayout);
    initChart();
}

void HtcChart::HTCCHartYPrecisionChanged(int res)
{
    _UpdatingFromProperties = true;
    _yPrecision = res;


    clearLayout(ui->chartLayout);
    initChart();
}

void HtcChart::HTCChartPenValues(int width, QColor color, int penStyle, QString penName, int penNumber)
{
    _UpdatingFromProperties = true;

    _penStates[penNumber - 1] = 1;
    _penWidths[penNumber - 1] = width;
    _penColors[penNumber - 1] = color;
    _penStyles[penNumber - 1] = penStyle;

    _currentHeaderList[penNumber] = penName;

    clearLayout(ui->chartLayout);
    initChart();

    _UpdatingFromProperties = false;

}

void HtcChart::HTCCHartNewPen(double baseValue, QString header)
{
    _UpdatingFromProperties = true;
    _AddingNewPen = true;

    createPen(baseValue, header);

}

void HtcChart::HTCCHartRemovePenRequest(int penNumber)
{
   _UpdatingFromProperties = true;
    stripLastHeaderItem(penNumber);
    stripLastDataColumn(penNumber);

    updateHeaderCount();

    // reload the chart
    clearLayout(ui->chartLayout);
    initChart();

}

void HtcChart::setChartTitle(QString title)
{
    QString chartTitle = "";

    QString newTitle = stripTokensFromString(title,"<br>");
    chartTitle.append("Chart: " + newTitle);
    this->setWindowTitle(chartTitle);

}

QString HtcChart::stripTokensFromString(QString message, QString token)
{
    QString result = message;
    result.replace(token, "");

    return result;

}

void HtcChart::listPenStyles(QString msg)
{

    for(int i = 0; i < 15; i++)
    {
        qDebug() << "pen#" << i << " is style " << _penStyles[i];
    }

}

void HtcChart::listHeaders()
{
    for (int i = 0; i < _currentHeaderList.count(); i++)
    {
        qDebug() << "header #" << i << " is " << _currentHeaderList.at(i);
    }

}

int HtcChart::scanForChartType(QString filename)
{
    int result = 0;

    QStringList list;
    if(!_reWriteList.isEmpty())
    {
        _reWriteList.clear();
    }

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            _reWriteList += in.readLine();

        }

        file.close();

        QString sample = _reWriteList.at(0);
        QStringList parts = sample.split(_saveItemDelimiter);

        if(parts.at(0).contains(_saveChartTypeKey))
        {
           result = 1;
        }

    }
    return result;


}

void HtcChart::writeTypetoFile(QString filename)
{

    qDebug() << "number of rows before insert " << _reWriteList.count();
    //insert the new line and resave
    QString insertedTypeLine = "";
    insertedTypeLine.append(_saveChartTypeKey);
    insertedTypeLine.append(_saveItemDelimiter);
    insertedTypeLine.append(_chartBasicTypes[0]);
    _reWriteList.insert(0,insertedTypeLine);

    qDebug() << "number of rows after insert " << _reWriteList.count();

    //reWrite the file with the new line

    QFile outputFile(filename);
    QTextStream outStream(&outputFile);
    outputFile.open(QIODevice::ReadWrite | QIODevice::Truncate);

    for(int i = 0; i < _reWriteList.count(); i++)
    {
        outStream << _reWriteList.at(i) << endl;
    }


    outputFile.close();


}

void HtcChart::on_btnSaveData_clicked()
{

    QString msg;
    QString hdr = getSaveHeaderValues();
    QStringList data = getSaveDataValues();
    QString outputFileName;

     if (_loadedChartFromFile == false)
    {
        QString path = getProperPath(_basePath);
        QString parentFolder = path;
        QString fileExtension = ".chart";
        QString delim = "_";

        outputFileName.append(parentFolder);
        outputFileName.append(_ChartTestCode);
        outputFileName.append(delim);
        outputFileName.append(_ChartModel);
        outputFileName.append(delim);
        outputFileName.append(_ChartSerial);
        outputFileName.append(delim);
        outputFileName.append(_chartYAxisUnitsText);
        outputFileName.append(fileExtension);

//        qDebug() << "output file being saved after loading file";
//        qDebug() << "filename: " << outputFileName;
//        qDebug() << " <end of saveFileName construction>";
    }
    else
    {

         qDebug() << "loading chart from data ";
         outputFileName = _rawDataFileAndPath;

        qDebug() << "output file being saved after being construted from file";
        qDebug() << "filename used from file: " << outputFileName;
    }


    qDebug() << "Launching Chart Save dialog";

     QString fileName = QFileDialog::getSaveFileName(this, tr("Save Chart File"),
                               outputFileName,
                               "Charts (*.chart)");



    if(!fileName.isEmpty())
    {
        QFile outputFile(fileName);
        outputFile.open(QIODevice::ReadWrite | QIODevice::Truncate);

        if(!outputFile.isOpen())
        {
            showNoAccessFileForSavingMessage(fileName);
            msg.append("Couldn't access the file you selected.");

        }
        else
        {
            QTextStream outStream(&outputFile);

            // new
            outStream << getChartTypeSaveString() << endl;

            outStream << getChartKeySaveString() << endl;

            outStream << getChartTitleSaveString() << endl;
            outStream << getChartTitleConfigSaveString() << endl;
            outStream << getChartXAxisUnitsSaveString() << endl;
            outStream << getChartXAxisUnitsConfigSaveString() << endl;
            outStream << getChartYAxisUnitsSaveString() << endl;
            outStream << getChartYAxisUnitsConfigSaveString() << endl;

            outStream << getChartXAxisParamsSaveString() << endl;
            outStream << getChartYAxisParamsSaveString() << endl;
            outStream << getChartXAxisMinSaveString() << endl;
            outStream << getChartXAxisMaxSaveString() << endl;
            outStream << getChartYAxisMinSaveString() << endl;
            outStream << getChartYAxisMaxSaveString() << endl;

            outStream << getChartXAxisGridLinesSaveString() << endl;
            outStream << getChartYAxisGridLinesSaveString() << endl;

            outStream << getPenStatesSaveString() << endl;
            outStream << getPenStylesSaveString() << endl;
            outStream << getPenWidthSaveString() << endl;
            outStream << getPenColorsSaveString() << endl;


            outStream << hdr << endl;

            for (int i = 1; i < data.count(); i++)
            {
                outStream << data[i] << endl;
            }

            outputFile.close();

            msg.append("Chart data has been saved...");

        }


        ui->statusbar->showMessage(msg);
    }


}
