#ifndef CHARTPROPERTIES_H
#define CHARTPROPERTIES_H

#include <QDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QDebug>
#include <QColor>
#include <QFont>
#include <QPalette>
#include <QDoubleValidator>
#include <QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QList>
#include <QMessageBox>
#include <QSettings>


namespace Ui {
class ChartProperties;
}

class ChartProperties : public QDialog
{
    Q_OBJECT

public:
    explicit ChartProperties(QWidget *parent = 0);
    ~ChartProperties();
    void setChartTitleItems(QString title, QFont font, QColor color);

    void setChartXAxisItems(QString UnitsText, QFont UnitsFont, QBrush UnitsBrush,
                            QFont LabelFont, QColor LabelColor, double LabelRotation,
                            double min, double max, QString linlog, QString DecScientific);

    void setChartXAxisGridLines(bool AxisMajorLinesAreVisible, int AxisMajorLinesTicCount,
                                QColor AxisMajorLinesColor, double AxisMajorGridLinesize,
                                bool AxisMinorLinesAreVisible, int AxisMinorLinesTicCount,
                                QColor AxisMinorLinesColor, double AxisMinorGridLinesize);

    void setChartYAxisItems(QString UnitsText, QFont UnitsFont, QBrush UnitsBrush,
                            QFont LabelFont, QColor LabelColor, double LabelRotation,
                            double min, double max, QString linlog, QString DecScientific);

    void setChartYAxisGridLines(bool AxisMajorLinesAreVisible, int AxisMajorLinesTicCount,
                                QColor AxisMajorLinesColor, double AxisMajorGridLinesize,
                                bool AxisMinorLinesAreVisible, int AxisMinorLinesTicCount,
                                QColor AxisMinorLinesColor, double AxisMinorGridLinesize);

    void setPenItems(int width, QColor color, int penStyle, QString penName, int penNumber);

    // new CI parts
    int getDataType();
    void setDataType(int dataType);


signals:
    // Chart Title
    void ChartTitleText(QString title);
    void ChartTitleTextFont(QFont font);
    void ChartTitleTextColor(QColor color);

    // Chart X Axis Units
    void HTCChartXAxisUnitsTextChanged(QString text);
    void HTCChartXAxisUnitsTextFontChanged(QFont font);
    void HTCChartXAxisUnitsTextColorChanged(QColor color);

   // Chart X Axis Labels
    void HTCChartXAxisLabelsTextRotationChanged(qint32 value);
    void HTCChartXAxisLabelsTextFontChanged(QFont font);
    void HTCChartXAxisLabelsTextColorChanged(QColor color);

    // Chart X Axis Scaling
    void HTCChartXAxisLabelsTextScaleMinChanged(double value);
    void HTCChartXAxisLabelsTextScaleMaxChanged(double value);

    void HTCChartXMajorThicknessValueChanged(int arg1);
    void HTCChartXMinorThicknessValueChanged(int arg1);

    void HTCChartXAxisMajorTicsColorChanged(QColor color);
    void HTCChartXAxisMinorTicsColorChanged(QColor color);

    void HTCChartXMajorTicsEnabledChanged(bool checked);
    void HTCChartXMinorTicsEnabledChanged(bool checked);

    void HTCChartXMinorTicsValueChanged(double arg1);
    void HTCChartXMajorTicsValueChanged(double arg1);

    void HTCChartXLogChartRequest(bool checked);
    void HTCChartXLinChartRequest(bool checked);

    void HTCChartXDecimalRequest(bool checked);
    void HTCChartXScientificRequest(bool checked);



    // Chart Y Axis Units
    void HTCChartYAxisUnitsTextChanged(QString text);
    void HTCChartYAxisUnitsTextFontChanged(QFont font);
    void HTCChartYAxisUnitsTextColorChanged(QColor color);

    // Chart Y Axis Labels
    void HTCChartYAxisLabelsTextRotationChanged(qint32 value);
    void HTCChartYAxisLabelsTextFontChanged(QFont font);
    void HTCChartYAxisLabelsTextColorChanged(QColor color);

    // Chart Y Axis Scaling
    void HTCChartYAxisLabelsTextScaleMinChanged(double value);
    void HTCChartYAxisLabelsTextScaleMaxChanged(double value);

    void HTCChartYMajorThicknessValueChanged(int arg1);
    void HTCChartYMinorThicknessValueChanged(int arg1);

    void HTCChartYAxisMajorTicsColorChanged(QColor color);
    void HTCChartYAxisMinorTicsColorChanged(QColor color);

    void HTCChartYMajorTicsEnabledChanged(bool checked);
    void HTCChartYMinorTicsEnabledChanged(bool checked);

    void HTCChartYMinorTicsValueChanged(double arg1);
    void HTCChartYMajorTicsValueChanged(double arg1);

    void HTCChartYLogChartRequest(bool checked);
    void HTCChartYLinChartRequest(bool checked);

    void HTCChartYDecimalRequest(bool checked);
    void HTCChartYScientificRequest(bool checked);

    void HTCCHartXPrecisionChangeRequest(int res);
    void HTCCHartYPrecisionChangeRequest(int res);


    void HTCChartPenValueChanged(int width, QColor color, int penStyle, QString penName, int penNumber);
    void HTCCHartAddPenRequest(double baseValue, QString header);

    void HTCChartRemovePen(int penNumber);


private slots:
    void on_btnClose_clicked();

    void on_btnChartTitleTextFont_clicked();

    void on_btnChartTitleTextColor_clicked();

    void on_textChartTitleText_textChanged();

    void on_btnXAxisLabelfont_clicked();

    void on_btnYAxisLabelfont_clicked();

    void on_btnXaxisLabelColor_clicked();

    void on_btnXAxisValuesColor_clicked();

    void on_btnYaxisLabelColor_clicked();

    void on_btnXAxisValuesFont_clicked();

    void on_btnYAxisValuesFont_clicked();

    void on_btnYAxisValuesColor_clicked();

    void on_spinXAxisRotation_valueChanged(int arg1);

    void on_spinYAxisRotation_valueChanged(int arg1);

    void on_txtXAxisLabelText_textChanged();

    void on_txtYAxisLabelText_textChanged();

    void on_radioYLog_clicked(bool checked);

    void on_btnXAxisMajorGridColor_clicked();

    void on_btnXAxisMinorGridColor_clicked();

    void on_btnYAxisMajorGridColor_clicked();

    void on_btnYAxisMinorGridColor_clicked();

    void on_checkXMinor_clicked(bool checked);

    void on_checkXMajor_clicked(bool checked);

    void on_checkYMajor_clicked(bool checked);

    void on_checkYMinor_clicked(bool checked);

    void on_radioXLin_clicked(bool checked);

    void on_radioXLog_clicked(bool checked);

    void on_radioYLin_clicked(bool checked);

    void on_lineXMax_editingFinished();

    void on_lineXMin_editingFinished();

    void on_lineYMin_editingFinished();

    void on_lineYMax_editingFinished();

    void on_pBttonPen01_clicked();

    void on_linePen01_textChanged(const QString &arg1);

    void on_comboPen01_currentIndexChanged(int index);

    void on_linePen01_editingFinished();

    void on_spinXMajorThickness_valueChanged(int arg1);

    void on_spinXMinorThickness_valueChanged(int arg1);

    void on_spinYMajorThickness_valueChanged(int arg1);

    void on_spinYMinorThickness_valueChanged(int arg1);

    void on_spinYMajorTics_valueChanged(int arg1);

    void on_spinYMinorTics_valueChanged(int arg1);

    void on_spinXMajorTics_valueChanged(int arg1);

    void on_spinXMinorTics_valueChanged(int arg1);

    void on_pBttonPen02_clicked();

    void on_pBttonPen03_clicked();

    void on_pBttonPen04_clicked();

    void on_pBttonPen05_clicked();

    void on_pBttonPen06_clicked();

    void on_pBttonPen07_clicked();

    void on_pBttonPen08_clicked();

    void on_pBttonPen09_clicked();

    void on_pBttonPen10_clicked();

    void on_pBttonPen11_clicked();

    void on_pBttonPen12_clicked();

    void on_pBttonPen13_clicked();

    void on_spinBox01_valueChanged(int arg1);

    void on_spinBox02_valueChanged(int arg1);

    void on_spinBox03_valueChanged(int arg1);

    void on_spinBox04_valueChanged(int arg1);

    void on_spinBox05_valueChanged(int arg1);

    void on_spinBox06_valueChanged(int arg1);

    void on_spinBox07_valueChanged(int arg1);

    void on_spinBox08_valueChanged(int arg1);

    void on_spinBox09_valueChanged(int arg1);

    void on_spinBox10_valueChanged(int arg1);

    void on_spinBox11_valueChanged(int arg1);

    void on_spinBox12_valueChanged(int arg1);

    void on_spinBox13_valueChanged(int arg1);

    void on_comboPen02_currentIndexChanged(int index);

    void on_comboPen03_currentIndexChanged(int index);

    void on_comboPen04_currentIndexChanged(int index);

    void on_comboPen05_currentIndexChanged(int index);

    void on_comboPen06_currentIndexChanged(int index);

    void on_comboPen07_currentIndexChanged(int index);

    void on_comboPen08_currentIndexChanged(int index);

    void on_comboPen09_currentIndexChanged(int index);

    void on_comboPen10_currentIndexChanged(int index);

    void on_comboPen11_currentIndexChanged(int index);

    void on_comboPen12_currentIndexChanged(int index);

    void on_comboPen13_currentIndexChanged(int index);

    void on_pButtonAddPen_clicked();

    void on_radioYDecimal_clicked(bool checked);

    void on_radioYScientific_clicked(bool checked);

    void on_radioXScientific_clicked(bool checked);

    void on_radioXDecimal_clicked(bool checked);

    void on_buttonRemovePen_clicked();



    void on_spinYPrecision_valueChanged(int arg1);

    void on_spinXPrecision_valueChanged(int arg1);

    void on_linePen02_editingFinished();

    void on_linePen03_editingFinished();

    void on_linePen04_editingFinished();

    void on_linePen05_editingFinished();

    void on_linePen06_editingFinished();

    void on_linePen07_editingFinished();

    void on_linePen08_editingFinished();

    void on_linePen09_editingFinished();

    void on_linePen10_editingFinished();

    void on_linePen11_editingFinished();

    void on_linePen12_editingFinished();

    void on_linePen13_editingFinished();

private:

    Ui::ChartProperties *ui;

    QPalette masterPal;
    QPalette subDialogPal;
    QColor _ChartDialogBaseColor;
    QColor _ChartSubPanelColor;

    QDoubleValidator *dblValidator;

    bool _busy = true;

    double _xAxisScaleMin = 0;
    double _xAxisScaleMax = 0;
    double _yAxisScaleMin = 0;
    double _yAxisScaleMax = 0;

    int _xPrecision = 0;
    int _yPrecision = 2;

    // ------------------------------------------------- //
    bool _OverrideLegendValue;
    // ------------------------------------------------- //

    void setControlStyles();
    void setIndicatorControlsFont();
    void setToolTips();

    QColor getColorFromPalette(QPalette pal);
    QString getColorStringFromPalette(QPalette pal);
    QString stripCharFromString(QString stripChar, QString Target);
    QString filterTitle(QString message);

    void setLinLogChecked(QString axis, QString mode);
    void setDecimalScientificChecked(QString axis, QString mode);

    // functions
    QFont getNewFont(QFont currentFont);
    QColor getNewColor(QColor currentColor);

    QFont _controlsDefaualtFont;
    int _controlsDefaualtPointSize;
    QString _controlsDefaualtFontName;
    QColor _controlsDefaualtFontColor;
    QPalette _controlsDefaualtFontColorPalette;

    QStringList getPenTypes();
    void initPenStyleCombos();

    int _penStates[13] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0 };

    int _penThicknesses[15] = { 1, 1, 1, 1, 1, 1, 1, 1,
                                1, 1, 1, 1, 1 };

    QStringList _nonDeletablePenNames;

    // ------------------------------- //
    // X/Y local tic color properties
    // ------------------------------- //
    QColor  _clrXMajorTicColor;
    QColor  _clrXMinorTicColor;
    QColor  _clrYMajorTicColor;
    QColor  _clrYMinorTicColor;




    //QFont _formLabelFont = QFont("Times New Roman",10, QFont::Normal );
    QFont _formLabelFont = QFont("Arial",9, QFont::Normal );


    void updatePenItemsAndNotify(int penNumber);
    int _maxNumberOfPens = 12;


    void initAllPenControls();
    void initPenFont(int pen);
    void disablePen(int pen);
    void enablePen(int pen);
    void setAllLabelsFont();
    void setAllCheckboxesFont();
    void setAllRadioButtonFonts();

    // remove pen functions
    int findLastEnabledPen();
    QString getPenTagByID(int pen);
    bool isPenDeletable(QString penName);
    void showNoDeleteForPenMessage(QString penName);
    void showNoMorePensAllowedMessage(int pen);
    void clearDeletedPen(int pen);

    //legend parts
    QStringList _positions;
    QStringList _defaultPositions;
    QStringList _legendKeys;
    void loadSettings();

    int RIdataType = 0;
    int CIdataType = 1;

    int _dataType = -1;
    QStringList dataTypes;



    // debug methods
    void listPenStates(QString msg);


};

#endif // CHARTPROPERTIES_H
