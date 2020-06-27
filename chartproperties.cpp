// ++++++++++++++++++++++++++++++++++++++++++++ //
// File: chartproperties.cpp
// Description: Provides controls to
// modify the underlying chart.
//
// Date: 2019-03-07
//
//
// TODO:
//
//
//
// ++++++++++++++++++++++++++++++++++++++++++++ //

#include "chartproperties.h"
#include "ui_chartproperties.h"

ChartProperties::ChartProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartProperties)
{
    ui->setupUi(this);
    dataTypes.clear();
    dataTypes << "RI-Data_Type" << "CI-Data-Type";

    _positions.clear();
    _defaultPositions << "Padding Value"  << "Baseline" << "0° Horiz" << "0° Vert" << "90° Horiz"
                        << "90° Vert" << "180° Horiz" << "180° Vert" << "270° Horiz" << "270° Vert";

    _positions = _defaultPositions;


    loadSettings();


    initPenStyleCombos();
    setAllLabelsFont();
    setAllCheckboxesFont();
    setAllRadioButtonFonts();


    _busy = true;

    dblValidator = new QDoubleValidator();
    dblValidator->setNotation(QDoubleValidator::ScientificNotation);
    dblValidator->setRange(-9.9E19, 9.9E19,12);
    ui->lineXMin->setValidator(dblValidator);
    ui->lineXMax->setValidator(dblValidator);
    ui->lineYMin->setValidator(dblValidator);
    ui->lineYMin->setValidator(dblValidator);
    ui->linePenValue->setValidator(dblValidator);
    ui->spinXPrecision->setValue(_xPrecision);
    ui->spinYPrecision->setValue(_yPrecision);

    // set dialog colors
    // -----------------------------

    _ChartDialogBaseColor = QColor("#E6E3E3");  // E6E3E3 ; #CFCDCD - was BDBCBC
    _ChartSubPanelColor = QColor("#EFEFEF");    // EFEFEF ; #BDBCBC - was CFCDCD

    masterPal.setColor(QPalette::Background, _ChartDialogBaseColor);



    this->setAutoFillBackground(true);
    this->setPalette(masterPal);

    ui->tabWidgetChartProp->setCurrentIndex(0);
    subDialogPal.setColor(QPalette::Background, _ChartSubPanelColor);


    ui->groupPens->setAutoFillBackground(true);
    ui->groupPens->setPalette(subDialogPal);

    ui->groupChartTitle->setAutoFillBackground(true);
    ui->groupChartTitle->setPalette(subDialogPal);

    ui->groupXAxisItems->setAutoFillBackground(true);;
    ui->groupXAxisItems->setPalette(subDialogPal);

    ui->groupYAxisItems->setAutoFillBackground(true);
    ui->groupYAxisItems->setPalette(subDialogPal);



    // set non-sizable control fonts
    _controlsDefaualtFontColor = QColor("#000000");
    _controlsDefaualtFontColorPalette.setColor(QPalette::Text, _controlsDefaualtFontColor);
    _controlsDefaualtPointSize = 9;
    _controlsDefaualtFontName = "Times New Roman"; //Webdings Courier New
    _controlsDefaualtFont.setPointSize(_controlsDefaualtPointSize);
    _controlsDefaualtFont.setFamily(_controlsDefaualtFontName);

    ui->lineXMin->setFont(_controlsDefaualtFont);
    ui->lineXMin->setPalette(_controlsDefaualtFontColorPalette);
    ui->lineXMin->setAlignment(Qt::AlignRight);

    ui->lineXMax->setFont(_controlsDefaualtFont);
    ui->lineXMax->setPalette(_controlsDefaualtFontColorPalette);
    ui->lineXMax->setAlignment(Qt::AlignRight);

    ui->lineYMin->setFont(_controlsDefaualtFont);
    ui->lineYMin->setPalette(_controlsDefaualtFontColorPalette);
    ui->lineYMin->setAlignment(Qt::AlignRight);

    ui->lineYMax->setFont(_controlsDefaualtFont);
    ui->lineYMax->setPalette(_controlsDefaualtFontColorPalette);
    ui->lineYMax->setAlignment(Qt::AlignRight);

    ui->spinXAxisRotation->setFont(_controlsDefaualtFont);
    ui->spinXAxisRotation->setPalette(_controlsDefaualtFontColorPalette);

    ui->spinYAxisRotation->setFont(_controlsDefaualtFont);
    ui->spinYAxisRotation->setPalette(_controlsDefaualtFontColorPalette);

    ui->spinXMajorTics->setFont(_controlsDefaualtFont);
    ui->spinXMajorTics->setPalette(_controlsDefaualtFontColorPalette);

    ui->spinXMinorTics->setFont(_controlsDefaualtFont);
    ui->spinXMinorTics->setPalette(_controlsDefaualtFontColorPalette);

    ui->spinYMajorTics->setFont(_controlsDefaualtFont);
    ui->spinYMajorTics->setPalette(_controlsDefaualtFontColorPalette);

    ui->spinYMinorTics->setFont(_controlsDefaualtFont);
    ui->spinYMinorTics->setPalette(_controlsDefaualtFontColorPalette);

    ui->spinXMajorThickness->setFont(_controlsDefaualtFont);
    ui->spinXMajorThickness->setPalette(_controlsDefaualtFontColorPalette);

    ui->spinXMinorThickness->setFont(_controlsDefaualtFont);
    ui->spinXMinorThickness->setPalette(_controlsDefaualtFontColorPalette);

    ui->spinYMajorThickness->setFont(_controlsDefaualtFont);
    ui->spinYMajorThickness->setPalette(_controlsDefaualtFontColorPalette);

    ui->spinYMinorThickness->setFont(_controlsDefaualtFont);
    ui->spinYMinorThickness->setPalette(_controlsDefaualtFontColorPalette);

    initAllPenControls();

    _busy = false;

}

ChartProperties::~ChartProperties()
{
    delete ui;
}

void ChartProperties::setChartTitleItems(QString title, QFont font, QColor color)
{
    _busy = true;

    QPalette pal;
    pal.setColor(QPalette::Text, color);
    ui->textChartTitleText->setText(title);
    ui->textChartTitleText->setFont(font);
    ui->textChartTitleText->setPalette(pal);




    _busy = false;
}

void ChartProperties::setChartXAxisItems(QString UnitsText, QFont UnitsFont, QBrush UnitsBrush,
                                         QFont LabelFont, QColor LabelColor, double LabelRotation,
                                         double min, double max, QString linlog, QString DecScientific)
{
    _busy = true;

    ui->txtXAxisLabelText->setText(UnitsText);
    ui->txtXAxisLabelText->setFont(UnitsFont);
    QPalette pX;
    pX.setBrush(QPalette::Text, UnitsBrush);
    ui->txtXAxisLabelText->setPalette(pX);

    _xAxisScaleMin = min;
    _xAxisScaleMax = max;

    ui->txtXLabels->setText("Sample");
    ui->txtXLabels->setFont(LabelFont);
    QPalette pXLabel;
    pXLabel.setColor(QPalette::Text, LabelColor);
    ui->txtXLabels->setPalette(pXLabel);

    ui->lineXMin->setText(QString::number(_xAxisScaleMin));
    ui->lineXMax->setText(QString::number(_xAxisScaleMax));

    ui->spinXAxisRotation->setValue(QString::number(LabelRotation).toInt());
    setLinLogChecked("X", linlog);
    setDecimalScientificChecked("X", DecScientific);

    _busy = false;
}

void ChartProperties::setChartXAxisGridLines(bool AxisMajorLinesAreVisible, int AxisMajorLinesTicCount,
                                             QColor AxisMajorLinesColor, double AxisMajorGridLinesize,
                                             bool AxisMinorLinesAreVisible, int AxisMinorLinesTicCount,
                                             QColor AxisMinorLinesColor, double AxisMinorGridLinesize)
{

    ui->checkXMajor->setChecked(AxisMajorLinesAreVisible);
    ui->spinXMajorThickness->setValue(QString::number(AxisMajorGridLinesize).toInt());

    // Set the X Axis Major tic Label color
    // -----------------------------------------------------
    QPalette  palMajor;
    palMajor.setColor(QPalette::Base, Qt::white);
    palMajor.setColor(QPalette::WindowText, AxisMajorLinesColor);
    ui->labelXMajorTics->setPalette(palMajor);

    // Set the X Axis Major spinbox text/count value
    // -----------------------------------------------------
    ui->spinXMajorTics->setValue(AxisMajorLinesTicCount);

    // Set the X Axis Major spin box text color
    // -----------------------------------------------------
    palMajor.setColor(QPalette::Text, AxisMajorLinesColor);
    ui->spinXMajorTics->setPalette(palMajor);

    ui->checkXMinor->setChecked(AxisMinorLinesAreVisible);
    ui->spinXMinorThickness->setValue(QString::number(AxisMinorGridLinesize).toInt());

    // Set the X Axis Minor tic Label color
    // -----------------------------------------------------
    QPalette  palMinor;
    palMinor.setColor(QPalette::Base, Qt::white);
    palMinor.setColor(QPalette::WindowText, AxisMinorLinesColor);
    ui->labelXMinorTics->setPalette(palMinor);

    // Set the X Axis Minor spinbox text/count value
    // -----------------------------------------------------
    palMinor.setColor(QPalette::Text, AxisMajorLinesColor);
    ui->spinXMinorTics->setPalette(palMinor);
    ui->spinXMinorTics->setValue(AxisMinorLinesTicCount);

}

void ChartProperties::setChartYAxisItems(QString UnitsText, QFont UnitsFont, QBrush UnitsBrush,
                                         QFont LabelFont, QColor LabelColor, double LabelRotation,
                                         double min, double max, QString linlog, QString DecScientific)
{
    _busy = true;


    ui->txtYAxisLabelText->setText(UnitsText);
    ui->txtYAxisLabelText->setFont(UnitsFont);
    ui->txtYAxisLabelText->setTextColor(UnitsBrush.color());
    QPalette pY;
    pY.setBrush(QPalette::Text, UnitsBrush);
    ui->txtYAxisLabelText->setPalette(pY);

    ui->txtYLabels->setText("Sample");
    ui->txtYLabels->setFont(LabelFont);
    QPalette pYLabel;
    pYLabel.setColor(QPalette::Text, LabelColor);
    ui->txtYLabels->setPalette(pYLabel);


    _yAxisScaleMin = min;
    _yAxisScaleMax = max;

   // qDebug() << "received Y min/max from Chart ->" << _yAxisScaleMin << "/" << _yAxisScaleMax;

    ui->lineYMin->setText(QString::number(_yAxisScaleMin));
    ui->lineYMax->setText(QString::number(_yAxisScaleMax));

    ui->spinYAxisRotation->setValue(QString::number(LabelRotation).toInt());
    setLinLogChecked("Y", linlog);
    setDecimalScientificChecked("Y", DecScientific);

    // tooltips
    setToolTips();

    _busy = false;
}

void ChartProperties::setChartYAxisGridLines(bool AxisMajorLinesAreVisible, int AxisMajorLinesTicCount,
                                             QColor AxisMajorLinesColor, double AxisMajorGridLinesize,
                                             bool AxisMinorLinesAreVisible, int AxisMinorLinesTicCount,
                                             QColor AxisMinorLinesColor, double AxisMinorGridLinesize)
{
    ui->checkYMajor->setChecked(AxisMajorLinesAreVisible);
    ui->spinYMajorThickness->setValue(QString::number(AxisMajorGridLinesize).toInt());

    // Set the X Axis Major tic Label color
    // -----------------------------------------------------
    QPalette  palMajor;
    palMajor.setColor(QPalette::Base, Qt::white);
    palMajor.setColor(QPalette::WindowText, AxisMajorLinesColor);
    ui->labelYMajorTics->setPalette(palMajor);

    // Set the X Axis Major spinbox text/count value
    // -----------------------------------------------------
    ui->spinYMajorTics->setValue(AxisMajorLinesTicCount);

    // Set the X Axis Major spin box text color
    // -----------------------------------------------------
    palMajor.setColor(QPalette::Text, AxisMajorLinesColor);
    ui->spinYMajorTics->setPalette(palMajor);


    ui->checkYMinor->setChecked(AxisMinorLinesAreVisible);
    ui->spinYMinorThickness->setValue(QString::number(AxisMinorGridLinesize).toInt());

    // Set the X Axis Minor tic Label color
    // -----------------------------------------------------
    QPalette  palMinor = QPalette();
    palMinor.setColor(QPalette::Base, Qt::white);
    palMinor.setColor(QPalette::WindowText, AxisMinorLinesColor);
    ui->labelYMinorTics->setPalette(palMinor);

    // Set the X Axis Minor spinbox text/count value
    // -----------------------------------------------------
    palMinor.setColor(QPalette::Text, AxisMajorLinesColor);
    ui->spinYMinorTics->setPalette(palMinor);
    ui->spinYMinorTics->setValue(AxisMinorLinesTicCount);

}

void ChartProperties::setPenItems(int width, QColor color, int penStyle, QString penName, int penNumber)
{
    _busy = true;
    QPalette palPen;
    _penStates[penNumber - 1] = 1;
    palPen.setColor(QPalette::Text, color);
    palPen.setColor(QPalette::Base, Qt::white);

    switch(penNumber)
    {
        case 1:
            ui->linePen01->setPalette(palPen);
            ui->spinBox01->setPalette(palPen);
            ui->linePen01->setText(penName);
            ui->spinBox01->setValue(width);
            ui->comboPen01->setCurrentIndex(penStyle);
            enablePen(penNumber);
            break;

        case 2:
        ui->linePen02->setPalette(palPen);
        ui->spinBox02->setPalette(palPen);
        ui->linePen02->setText(penName);
        ui->spinBox02->setValue(width);
        ui->comboPen02->setCurrentIndex(penStyle);
        enablePen(penNumber);
            break;

        case 3:
        ui->linePen03->setPalette(palPen);
        ui->spinBox03->setPalette(palPen);
        ui->linePen03->setText(penName);
        ui->spinBox03->setValue(width);
        ui->comboPen03->setCurrentIndex(penStyle);
        enablePen(penNumber);
            break;

        case 4:
        ui->linePen04->setPalette(palPen);
        ui->spinBox04->setPalette(palPen);
        ui->linePen04->setText(penName);
        ui->spinBox04->setValue(width);
        ui->comboPen04->setCurrentIndex(penStyle);
        enablePen(penNumber);
             break;

        case 5:
        ui->linePen05->setPalette(palPen);
        ui->spinBox05->setPalette(palPen);
        ui->linePen05->setText(penName);
        ui->spinBox05->setValue(width);
        ui->comboPen05->setCurrentIndex(penStyle);
        enablePen(penNumber);
            break;

        case 6:
        ui->linePen06->setPalette(palPen);
        ui->spinBox06->setPalette(palPen);
        ui->linePen06->setText(penName);
        ui->spinBox06->setValue(width);
        ui->comboPen06->setCurrentIndex(penStyle);
        enablePen(penNumber);
             break;

        case 7:
        ui->linePen07->setPalette(palPen);
        ui->spinBox07->setPalette(palPen);
        ui->linePen07->setText(penName);
        ui->spinBox07->setValue(width);
        ui->comboPen07->setCurrentIndex(penStyle);
        enablePen(penNumber);
            break;

        case 8:
        ui->linePen08->setPalette(palPen);
        ui->spinBox08->setPalette(palPen);
        ui->linePen08->setText(penName);
        ui->spinBox08->setValue(width);
        ui->comboPen08->setCurrentIndex(penStyle);
        enablePen(penNumber);
            break;

        case 9:
        ui->linePen09->setPalette(palPen);
        ui->spinBox09->setPalette(palPen);
        ui->linePen09->setText(penName);
        ui->spinBox09->setValue(width);
        ui->comboPen09->setCurrentIndex(penStyle);
        enablePen(penNumber);
            break;

        case 10:
        ui->linePen10->setPalette(palPen);
        ui->spinBox10->setPalette(palPen);
        ui->linePen10->setText(penName);
        ui->spinBox10->setValue(width);
        ui->comboPen10->setCurrentIndex(penStyle);
        enablePen(penNumber);
            break;
        case 11:
        ui->linePen11->setPalette(palPen);
        ui->spinBox11->setPalette(palPen);
        ui->linePen11->setText(penName);
        ui->spinBox11->setValue(width);
        ui->comboPen11->setCurrentIndex(penStyle);
        enablePen(penNumber);
            break;

        case 12:
        ui->linePen12->setPalette(palPen);
        ui->spinBox12->setPalette(palPen);
        ui->linePen12->setText(penName);
        ui->spinBox12->setValue(width);
        ui->comboPen12->setCurrentIndex(penStyle);
        enablePen(penNumber);
            break;

        case 13:
            ui->linePen13->setPalette(palPen);
            ui->spinBox13->setPalette(palPen);
            ui->linePen13->setText(penName);
            ui->spinBox13->setValue(width);
            ui->comboPen13->setCurrentIndex(penStyle);
            enablePen(penNumber);
            break;



        default:
            // we should throw an error here.
            qDebug() << "ChartProperties::We don't do this number " << penNumber;
    }


    _busy = false;

}

int ChartProperties::getDataType()
{
    return _dataType;
}

void ChartProperties::setDataType(int dataType)
{
    _dataType = dataType;
}

void ChartProperties::on_btnClose_clicked()
{
    close();
}

void ChartProperties::on_btnChartTitleTextFont_clicked()
{
    QFont font = getNewFont(ui->textChartTitleText->font());
    ui->textChartTitleText->setFont(font);

    emit ChartTitleTextFont(font);
}

QFont ChartProperties::getNewFont(QFont currentFont)
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

QColor ChartProperties::getNewColor(QColor currentColor)
{
    QColor result = currentColor;

    QColor color = QColorDialog::getColor(currentColor, this);
        if( color.isValid() )
        {
          result = color;
        }
        else
        {
            result = currentColor;
        }

        return result;

}

QStringList ChartProperties::getPenTypes()
{
    QStringList result = QStringList() << "NoPen" << "Solid" << "Dash" << "Dot" << "DashDot" << "DashDotDot" << "CustomDash";

    return result;
}

void ChartProperties::initPenStyleCombos()
{
    ui->comboPen01->addItems(getPenTypes());
    ui->comboPen02->addItems(getPenTypes());
    ui->comboPen03->addItems(getPenTypes());
    ui->comboPen04->addItems(getPenTypes());
    ui->comboPen05->addItems(getPenTypes());
    ui->comboPen06->addItems(getPenTypes());
    ui->comboPen07->addItems(getPenTypes());
    ui->comboPen08->addItems(getPenTypes());
    ui->comboPen09->addItems(getPenTypes());
    ui->comboPen10->addItems(getPenTypes());
    ui->comboPen11->addItems(getPenTypes());
    ui->comboPen12->addItems(getPenTypes());
    ui->comboPen13->addItems(getPenTypes());

}

void ChartProperties::updatePenItemsAndNotify(int penNumber)
{
    int width = 0;
    QPalette palPen;
    QColor color;
    int penStyle = 0;
    QString penName;

    switch(penNumber)
    {
        case 1:
            width = ui->spinBox01->value();

            palPen = ui->linePen01->palette();
            color = palPen.color(QPalette::Text);
            penStyle = ui->comboPen01->currentIndex();
            penName = ui->linePen01->text();
            break;

        case 2:
            width = ui->spinBox02->value();
            palPen = ui->linePen02->palette();
            color = palPen.color(QPalette::Text);
            penStyle = ui->comboPen02->currentIndex();
            penName = ui->linePen02->text();
            break;

        case 3:
            width = ui->spinBox03->value();
            palPen = ui->linePen03->palette();
            color = palPen.color(QPalette::Text);
            penStyle = ui->comboPen03->currentIndex();
            penName = ui->linePen03->text();
            break;

        case 4:
            width = ui->spinBox04->value();
            palPen = ui->linePen04->palette();
            color = palPen.color(QPalette::Text);
            penStyle = ui->comboPen04->currentIndex();
            penName = ui->linePen04->text();
             break;

        case 5:
             width = ui->spinBox05->value();
             palPen = ui->linePen05->palette();
             color = palPen.color(QPalette::Text);
             penStyle = ui->comboPen05->currentIndex();
             penName = ui->linePen05->text();
            break;

        case 6:
             width = ui->spinBox06->value();
             palPen = ui->linePen06->palette();
             color = palPen.color(QPalette::Text);
             penStyle = ui->comboPen06->currentIndex();
             penName = ui->linePen06->text();
             break;

        case 7:
             width = ui->spinBox07->value();
             palPen = ui->linePen07->palette();
             color = palPen.color(QPalette::Text);
             penStyle = ui->comboPen07->currentIndex();
             penName = ui->linePen07->text();
            break;

        case 8:
            width = ui->spinBox08->value();
            palPen = ui->linePen08->palette();
            color = palPen.color(QPalette::Text);
            penStyle = ui->comboPen08->currentIndex();
            penName = ui->linePen08->text();
            break;

        case 9:
            width = ui->spinBox09->value();
            palPen = ui->linePen09->palette();
            color = palPen.color(QPalette::Text);
            penStyle = ui->comboPen09->currentIndex();
            penName = ui->linePen09->text();
            break;

        case 10:
            width = ui->spinBox10->value();
            palPen = ui->linePen10->palette();
            color = palPen.color(QPalette::Text);
            penStyle = ui->comboPen10->currentIndex();
            penName = ui->linePen10->text();
            break;

    case 11:
            width = ui->spinBox11->value();
            palPen = ui->linePen11->palette();
            color = palPen.color(QPalette::Text);
            penStyle = ui->comboPen11->currentIndex();
            penName = ui->linePen11->text();
            break;

        case 12:
            width = ui->spinBox12->value();
            palPen = ui->linePen12->palette();
            color = palPen.color(QPalette::Text);
            penStyle = ui->comboPen12->currentIndex();
            penName = ui->linePen12->text();
            break;

        case 13:
            width = ui->spinBox13->value();
            palPen = ui->linePen13->palette();
            color = palPen.color(QPalette::Text);
            penStyle = ui->comboPen13->currentIndex();
            penName = ui->linePen13->text();
            break;


            default:
            // we need to throw an error here
            qDebug() << "ChartProperties::We don't do this number!";
    }


    emit HTCChartPenValueChanged(width, color, penStyle, penName, penNumber);

}


void ChartProperties::initAllPenControls()
{
    // hide all controls
    int numberOfPenSets = sizeof(_penStates)/sizeof(_penStates[0]);

    for(int pen = 0; pen < numberOfPenSets; pen++)
    {
        initPenFont(pen + 1);
        disablePen(pen + 1);
    }

}

void ChartProperties::disablePen(int pen)
{
    switch(pen)
    {
        case 1:
            ui->linePen01->hide();
            ui->spinBox01->hide();
            ui->pBttonPen01->hide();
            ui->comboPen01->hide();
            break;

        case 2:
            ui->linePen02->hide();
            ui->spinBox02->hide();
            ui->pBttonPen02->hide();
            ui->comboPen02->hide();
            break;

        case 3:
            ui->linePen03->hide();
            ui->spinBox03->hide();
            ui->pBttonPen03->hide();
            ui->comboPen03->hide();
            break;

        case 4:
            ui->linePen04->hide();
            ui->spinBox04->hide();
            ui->pBttonPen04->hide();
            ui->comboPen04->hide();
             break;

        case 5:
            ui->linePen05->hide();
            ui->spinBox05->hide();
            ui->pBttonPen05->hide();
            ui->comboPen05->hide();
            break;

        case 6:
            ui->linePen06->hide();
            ui->spinBox06->hide();
            ui->pBttonPen06->hide();
            ui->comboPen06->hide();
             break;

        case 7:
            ui->linePen07->hide();
            ui->spinBox07->hide();
            ui->pBttonPen07->hide();
            ui->comboPen07->hide();
            break;

        case 8:
            ui->linePen08->hide();
            ui->spinBox08->hide();
            ui->pBttonPen08->hide();
            ui->comboPen08->hide();
            break;

        case 9:
            ui->linePen09->hide();
            ui->spinBox09->hide();
            ui->pBttonPen09->hide();
            ui->comboPen09->hide();
            break;

        case 10:
            ui->linePen10->hide();
            ui->spinBox10->hide();
            ui->pBttonPen10->hide();
            ui->comboPen10->hide();
            break;
        case 11:
            ui->linePen11->hide();
            ui->spinBox11->hide();
            ui->pBttonPen11->hide();
            ui->comboPen11->hide();
            break;

        case 12:
            ui->linePen12->hide();
            ui->spinBox12->hide();
            ui->pBttonPen12->hide();
            ui->comboPen12->hide();
            break;

        case 13:
            ui->linePen13->hide();
            ui->spinBox13->hide();
            ui->pBttonPen13->hide();
            ui->comboPen13->hide();
            break;




            default:
                // we need to throw an error here
                qDebug() << "ChartProperties::We don't do this number " << pen;
    }
}

void ChartProperties::enablePen(int pen)
{
    switch(pen)
    {
        case 1:
            ui->linePen01->setVisible(true);
            ui->spinBox01->setVisible(true);
            ui->pBttonPen01->setVisible(true);
            ui->comboPen01->setVisible(true);
            break;

        case 2:
            ui->linePen02->setVisible(true);
            ui->spinBox02->setVisible(true);
            ui->pBttonPen02->setVisible(true);
            ui->comboPen02->setVisible(true);
            break;

        case 3:
            ui->linePen03->setVisible(true);
            ui->spinBox03->setVisible(true);
            ui->pBttonPen03->setVisible(true);
            ui->comboPen03->setVisible(true);
            break;

        case 4:
            ui->linePen04->setVisible(true);
            ui->spinBox04->setVisible(true);
            ui->pBttonPen04->setVisible(true);
            ui->comboPen04->setVisible(true);
             break;

        case 5:
            ui->linePen05->setVisible(true);
            ui->spinBox05->setVisible(true);
            ui->pBttonPen05->setVisible(true);
            ui->comboPen05->setVisible(true);
            break;

        case 6:
            ui->linePen06->setVisible(true);
            ui->spinBox06->setVisible(true);
            ui->pBttonPen06->setVisible(true);
            ui->comboPen06->setVisible(true);
             break;

        case 7:
            ui->linePen07->setVisible(true);
            ui->spinBox07->setVisible(true);
            ui->pBttonPen07->setVisible(true);
            ui->comboPen07->setVisible(true);
            break;

        case 8:
            ui->linePen08->setVisible(true);
            ui->spinBox08->setVisible(true);
            ui->pBttonPen08->setVisible(true);
            ui->comboPen08->setVisible(true);
            break;

        case 9:
            ui->linePen09->setVisible(true);
            ui->spinBox09->setVisible(true);
            ui->pBttonPen09->setVisible(true);
            ui->comboPen09->setVisible(true);
            break;

        case 10:
            ui->linePen10->setVisible(true);
            ui->spinBox10->setVisible(true);
            ui->pBttonPen10->setVisible(true);
            ui->comboPen10->setVisible(true);
            break;
        case 11:
            ui->linePen11->setVisible(true);
            ui->spinBox11->setVisible(true);
            ui->pBttonPen11->setVisible(true);
            ui->comboPen11->setVisible(true);
            break;

        case 12:
            ui->linePen12->setVisible(true);
            ui->spinBox12->setVisible(true);
            ui->pBttonPen12->setVisible(true);
            ui->comboPen12->setVisible(true);
            break;

        case 13:
            ui->linePen13->setVisible(true);
            ui->spinBox13->setVisible(true);
            ui->pBttonPen13->setVisible(true);
            ui->comboPen13->setVisible(true);
            break;




        default:
            // we need to throw an error here
            qDebug() << "ChartProperties::We don't do this number " << pen;
    }
}

void ChartProperties::setAllLabelsFont()
{
    QList<QLabel *> butts = this->findChildren<QLabel *>();

    for (QLabel * label : qAsConst(butts))
    {
         label->setFont(_formLabelFont);
         QString setto = label->font().family();
         QString name = label->objectName();
    }


}

void ChartProperties::setAllCheckboxesFont()
{
    QList<QCheckBox *> butts = this->findChildren<QCheckBox *>();

    for (QCheckBox * box : qAsConst(butts))
    {
         box->setFont(_formLabelFont);
         QString setto = box->font().family();
         QString name = box->objectName();
    }

}

void ChartProperties::setAllRadioButtonFonts()
{
    ui->radioXLin->setFont(_formLabelFont);
    ui->radioXLog->setFont(_formLabelFont);
    ui->radioYLin->setFont(_formLabelFont);
    ui->radioYLog->setFont(_formLabelFont);
    ui->radioXDecimal->setFont(_formLabelFont);
    ui->radioYDecimal->setFont(_formLabelFont);
    ui->radioXScientific->setFont(_formLabelFont);
    ui->radioYScientific->setFont(_formLabelFont);


}

int ChartProperties::findLastEnabledPen()
{

    int result = -1;
    for (int i = 0; i < 13; i++)
    {
        if (_penStates[i] == 1)
        {
            result = i;
        }
    }

    return result;


}

QString ChartProperties::getPenTagByID(int pen)
{
    QString result = "";


    switch(pen)
    {
     case 1:
         result = ui->linePen01->text();
         break;

    case 2:
        result = ui->linePen02->text();
        break;

    case 3:
        result = ui->linePen03->text();
        break;

    case 4:
        result = ui->linePen04->text();
        break;

    case 5:
        result = ui->linePen05->text();
        break;

    case 6:
        result = ui->linePen06->text();
        break;

    case 7:
        result = ui->linePen07->text();
        break;

    case 8:
        result = ui->linePen08->text();
        break;

    case 9:
        result = ui->linePen09->text();
        break;

    case 10:
        result = ui->linePen10->text();
        break;
    case 11:
        result = ui->linePen11->text();
        break;

    case 12:
        result = ui->linePen12->text();
        break;

    case 13:
        result = ui->linePen13->text();
        break;

    default:
        // we need to throw an error here
        qDebug() << "we don't do that number -> " << pen;
        //result = ui->linePen13->text();

    }

    return result;


}

bool ChartProperties::isPenDeletable(QString penName)
{
    bool result = true;

    if (!penName.isEmpty())
    {
        if (_nonDeletablePenNames.contains(penName))
        {
            result = false;
        }
    }


    return result;

}

void ChartProperties::showNoDeleteForPenMessage(QString penName)
{
    QMessageBox msgBox;
    QString message = "";

    message.append("The last pen in inventory is : ");
    message.append(penName);
    message.append(" and it can not be deleted. ");
    message.append("Only Limit Line pens can be deleted.");


    msgBox.setText(message);
    msgBox.setInformativeText("There are no more pens that can be deleted.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();

}

void ChartProperties::showNoMorePensAllowedMessage(int pen)
{
    QMessageBox msgBox;
    QString message = "";

    message.append("You've requested to add a Pen#");
    message.append(QString::number(pen + 2) + ".");
    message.append(" I can only maintain a maximum of 13 pens.");


    msgBox.setText(message);
    msgBox.setInformativeText("There are no more pens that can be created.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();
}

void ChartProperties::clearDeletedPen(int pen)
{

    int penStateToDisable = pen - 1;

    switch(pen)
    {
    case 1:
        ui->linePen01->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 2:
        ui->linePen02->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 3:
        ui->linePen03->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 4:
        ui->linePen04->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 5:
        ui->linePen05->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 6:
        ui->linePen06->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 7:
        ui->linePen07->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 8:
        ui->linePen08->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 9:
        ui->linePen09->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 10:
        ui->linePen10->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 11:
        ui->linePen11->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 12:
        ui->linePen12->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    case 13:
        ui->linePen13->setText("");
        _penStates[penStateToDisable] = 0;
        break;

    default:
        // we need to throw an error here
        qDebug() << "ChartProperties::We don't do this number " << pen;

    }


}

void ChartProperties::loadSettings()
{
    QString positionValue = "";

    QSettings setting("Keysight","ChartBuilder");

    setting.beginGroup("ProgramFolders");

    positionValue = setting.value("LegendPosition1").toString();

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
    // testing
    // --------------------------------------------------------- //

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
    }
    else
    {
        _positions[1] = positionValue;
    }
    positionValue.clear();

    positionValue = setting.value("LegendPosition3").toString();
    if(positionValue.isEmpty())
    {
        _positions[2] = _defaultPositions[2];
    }
    else
    {
        _positions[2] = positionValue;
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

    setting.endGroup();

    // assign values to the non-deletable pen list
    if (!_nonDeletablePenNames.isEmpty())
    {
        _nonDeletablePenNames.clear();
    }

    _nonDeletablePenNames = _positions;


}


void ChartProperties::listPenStates(QString msg)
{
   qDebug() << "listing penStates: " << msg;
    for(int i = 0; i < 13; i++)
    {
        qDebug() << "pen state " << i << " = " << _penStates[i];
    }

}


void ChartProperties::initPenFont(int pen)
{
    switch(pen)
    {
        case 1:
            ui->linePen01->setFont(_controlsDefaualtFont);
            break;

        case 2:
            ui->linePen02->setFont(_controlsDefaualtFont);
            break;

        case 3:
            ui->linePen03->setFont(_controlsDefaualtFont);
            break;

        case 4:
            ui->linePen04->setFont(_controlsDefaualtFont);
             break;

        case 5:
             ui->linePen05->setFont(_controlsDefaualtFont);
            break;

        case 6:
             ui->linePen06->setFont(_controlsDefaualtFont);
             break;

        case 7:
             ui->linePen07->setFont(_controlsDefaualtFont);
            break;

        case 8:
            ui->linePen08->setFont(_controlsDefaualtFont);
            break;

        case 9:
            ui->linePen09->setFont(_controlsDefaualtFont);
            break;

        case 10:
            ui->linePen10->setFont(_controlsDefaualtFont);
            break;
        case 11:
            ui->linePen11->setFont(_controlsDefaualtFont);
            break;

        case 12:
            ui->linePen12->setFont(_controlsDefaualtFont);
            break;

        case 13:
            ui->linePen13->setFont(_controlsDefaualtFont);
            break;



        default:
            // we need to throw an error here
            qDebug() << "ChartProperties::We don't do this number " << pen;
    }

}



void ChartProperties::on_btnChartTitleTextColor_clicked()
{
    QPalette  pal = ui->textChartTitleText->palette();
    QColor currentColor = pal.color(QPalette::Text);

    QColor color = getNewColor(currentColor);

    if (color != currentColor)
    {
        QPalette newPalette;
        newPalette.setColor(QPalette::Text, color);

        ui->textChartTitleText->setPalette(newPalette);

        emit ChartTitleTextColor(getColorFromPalette(ui->textChartTitleText->palette()));

    }
}

QColor ChartProperties::getColorFromPalette(QPalette pal)
{
    QColor currentColor = pal.color(QPalette::Text);
    return currentColor;
}

QString ChartProperties::getColorStringFromPalette(QPalette pal)
{
    QString result = "";
    QColor currentColor = pal.color(QPalette::Text);
    int r;
    int g;
    int b;
    int a;
    currentColor.getRgb(&r,&g,&b,&a);

    result.append("RGB Color: " + QString::number(r));
    result.append("," + QString::number(g));
    result.append("," + QString::number(b));


    return result;
}

QString ChartProperties::stripCharFromString(QString stripChar, QString Target)
{
    QString result = "";
    result = Target.remove(stripChar);

    return result;
}

QString ChartProperties::filterTitle(QString message)
{
    QString result = "";
    result = message.replace("\n","<br>",Qt::CaseInsensitive);
    return result;

}

void ChartProperties::setLinLogChecked(QString axis, QString mode)
{

    if (axis == "X")
    {
        if (mode == "LIN")
        {
            // set X lin checked
            ui->radioXLin->setChecked(true);
        }
        else
        {
            // set X log checked
            ui->radioXLog->setChecked(true);
        }
    }
    else
        if (mode == "LIN")
        {
            // set Y lin checked
            ui->radioYLin->setChecked(true);
        }
        else
        {
            // set Y log checked
            ui->radioYLog->setChecked(true);
        }

}

void ChartProperties::setDecimalScientificChecked(QString axis, QString mode)
{
    if (axis=="X")
    {
        if (mode == "DEC")
        {
            // set X decimal checked
            ui->radioXDecimal->setChecked(true);
        }
        else
        {
            // set X Sci checked
            ui->radioXScientific->setChecked(true);
        }
    }
    else
        if (mode == "DEC")
        {
            // set Y Decimal checked
            ui->radioYDecimal->setChecked(true);
        }
        else
        {
            // set Y Scientific checked
            ui->radioYScientific->setChecked(true);
        }


}



void ChartProperties::on_textChartTitleText_textChanged()
{
        if (!_busy)
        {
            QString title = filterTitle(ui->textChartTitleText->toPlainText());

            emit ChartTitleText(title);
        }
}

void ChartProperties::on_btnXAxisLabelfont_clicked()
{
    QFont font = getNewFont(ui->txtXAxisLabelText->font());
    ui->txtXAxisLabelText->setFont(font);
    emit HTCChartXAxisUnitsTextFontChanged(font);

}

void ChartProperties::on_btnYAxisLabelfont_clicked()
{
    QFont font = getNewFont(ui->txtYAxisLabelText->font());
    ui->txtYAxisLabelText->setFont(font);
    emit HTCChartYAxisUnitsTextFontChanged(font);


}

void ChartProperties::on_btnXaxisLabelColor_clicked()
{
    QPalette  pal = ui->txtXAxisLabelText->palette();
    QColor currentColor = pal.color(QPalette::Text);

    QColor color = getNewColor(currentColor);

    if (color != currentColor)
    {
        QPalette newPalette;
        newPalette.setColor(QPalette::Text, color);

        ui->txtXAxisLabelText->setPalette(newPalette);
        emit HTCChartXAxisUnitsTextColorChanged(getColorFromPalette(ui->txtXAxisLabelText->palette()));

    }
}

void ChartProperties::on_btnXAxisValuesColor_clicked()
{
    QPalette  pal = ui->txtXLabels->palette();
    QColor currentColor = pal.color(QPalette::Text);

    QColor color = getNewColor(currentColor);

    if (color != currentColor)
    {
        QPalette newPalette;
        newPalette.setColor(QPalette::Text, color);

        ui->txtXLabels->setPalette(newPalette);
        emit HTCChartXAxisLabelsTextColorChanged(this->getColorFromPalette(ui->txtXLabels->palette()));


    }

}

void ChartProperties::on_btnYaxisLabelColor_clicked()
{
    QPalette  pal = ui->txtYAxisLabelText->palette();
    QColor currentColor = pal.color(QPalette::Text);

    QColor color = getNewColor(currentColor);

    if (color != currentColor)
    {
        QPalette newPalette;
        newPalette.setColor(QPalette::Text, color);

        ui->txtYAxisLabelText->setPalette(newPalette);
        emit HTCChartYAxisUnitsTextColorChanged(this->getColorFromPalette(ui->txtYAxisLabelText->palette()));


    }

}

void ChartProperties::on_btnXAxisValuesFont_clicked()
{
    QFont font = getNewFont(ui->txtXLabels->font());
    ui->txtXLabels->setFont(font);
    emit HTCChartXAxisLabelsTextFontChanged(font);

}

void ChartProperties::on_btnYAxisValuesFont_clicked()
{

    QFont font = getNewFont(ui->txtYLabels->font());
    ui->txtYLabels->setFont(font);
    emit HTCChartYAxisLabelsTextFontChanged(font);

}

void ChartProperties::on_btnYAxisValuesColor_clicked()
{
    QPalette  pal = ui->txtYLabels->palette();
    QColor currentColor = pal.color(QPalette::Text);

    QColor color = getNewColor(currentColor);

    if (color != currentColor)
    {
        QPalette newPalette;
        newPalette.setColor(QPalette::Text, color);

        ui->txtYLabels->setPalette(newPalette);
        emit HTCChartYAxisLabelsTextColorChanged(this->getColorFromPalette(ui->txtYLabels->palette()));

    }

}

void ChartProperties::on_spinXAxisRotation_valueChanged(int arg1)
{
   if (!_busy)
   {
        emit HTCChartXAxisLabelsTextRotationChanged(arg1);

   }

}

void ChartProperties::on_spinYAxisRotation_valueChanged(int arg1)
{
    if (!_busy)
    {
        emit HTCChartYAxisLabelsTextRotationChanged(arg1);
    }

}



void ChartProperties::on_txtXAxisLabelText_textChanged()
{
    if (!_busy)
    {
        emit HTCChartXAxisUnitsTextChanged(ui->txtXAxisLabelText->toPlainText());

    }

}

void ChartProperties::on_txtYAxisLabelText_textChanged()
{
    if (!_busy)
    {
        emit HTCChartYAxisUnitsTextChanged(ui->txtYAxisLabelText->toPlainText());

    }
}

// TODO:
// Add protection for negative min/max values
// +++++++++++++++++++++++++++++++++++++++++++
void ChartProperties::on_radioYLog_clicked(bool checked)
{
    if (!_busy)
    {

        if(_yAxisScaleMin < 0 || _yAxisScaleMax < 0)
        {
            //ignore click
            _busy = true;
            setLinLogChecked("Y", "LIN");
            _busy = false;
        }
        else
        {
            emit HTCChartYLogChartRequest(checked);
        }

    }

}


void ChartProperties::on_btnXAxisMajorGridColor_clicked()
{
   if (!_busy)
   {
       // Check the current label color against the new color
       // -------------------------------------------
       QPalette  pal = ui->labelXMajorTics->palette();
       QColor currentColor = pal.color(QPalette::WindowText);

       QColor color = getNewColor(currentColor);

       if (color != currentColor)
       {
           // change the label color
           pal.setColor(QPalette::WindowText, color);
           ui->labelXMajorTics->setPalette(pal);

           // change the spinbox color
           pal.setColor(QPalette::Text, color);
           ui->spinXMajorTics->setPalette(pal);

           // Fire the signal event
           // -------------------------------------------------------------------------------------------------
           emit HTCChartXAxisMajorTicsColorChanged(color);
       }
    }
}

void ChartProperties::on_btnXAxisMinorGridColor_clicked()
{
    if (!_busy)
    {
        // Check the current label color against the new color
        // -------------------------------------------
        QPalette  pal = ui->labelXMinorTics->palette();
        QColor currentColor = pal.color(QPalette::WindowText);

        QColor color = getNewColor(currentColor);

        if (color != currentColor)
        {
            // change the label color
            pal.setColor(QPalette::WindowText, color);
            ui->labelXMinorTics->setPalette(pal);

            // change the spinbox color
            pal.setColor(QPalette::Text, color);
            ui->spinXMinorTics->setPalette(pal);

            // Fire the signal event
            // -------------------------------------------------------------------------------------------------
            emit HTCChartXAxisMinorTicsColorChanged(color);

        }
    }
}

void ChartProperties::on_btnYAxisMajorGridColor_clicked()
{
    // Check the current label color against the new color
    // -------------------------------------------
    QPalette  pal = ui->labelYMajorTics->palette();
    QColor currentColor = pal.color(QPalette::WindowText);

    QColor color = getNewColor(currentColor);

    if (color != currentColor)
    {
        // change the label color
        pal.setColor(QPalette::WindowText, color);
        ui->labelYMajorTics->setPalette(pal);

        // change the spinbox color
        pal.setColor(QPalette::Text, color);
        ui->spinYMajorTics->setPalette(pal);

        // Fire the signal event
        // -------------------------------------------------------------------------------------------------
        emit HTCChartYAxisMajorTicsColorChanged(this->getColorFromPalette(ui->spinYMajorTics->palette()));
    }

}

void ChartProperties::on_btnYAxisMinorGridColor_clicked()
{
    // Check the current label color against the new color
    // -------------------------------------------
    QPalette  pal = ui->labelYMinorTics->palette();
    QColor currentColor = pal.color(QPalette::WindowText);

    QColor color = getNewColor(currentColor);

    if (color != currentColor)
    {
        // change the label color
        pal.setColor(QPalette::WindowText, color);
        ui->labelYMinorTics->setPalette(pal);

        // change the spinbox color
        pal.setColor(QPalette::Text, color);
        ui->spinYMinorTics->setPalette(pal);

        // Fire the signal event
        // -------------------------------------------------------------------------------------------------
        emit HTCChartYAxisMinorTicsColorChanged(this->getColorFromPalette(ui->spinYMinorTics->palette()));

    }
}


void ChartProperties::on_checkXMinor_clicked(bool checked)
{
    if (!_busy)
    {
        emit HTCChartXMinorTicsEnabledChanged(checked);
    }
}

void ChartProperties::on_checkXMajor_clicked(bool checked)
{
    if (!_busy)
    {
        emit HTCChartXMajorTicsEnabledChanged(checked);
    }
}

void ChartProperties::on_checkYMajor_clicked(bool checked)
{
    if (!_busy)
    {
        emit HTCChartYMajorTicsEnabledChanged(checked);
    }
}

void ChartProperties::on_checkYMinor_clicked(bool checked)
{
    if (!_busy)
    {
        emit HTCChartYMinorTicsEnabledChanged(checked);
    }
}


void ChartProperties::on_radioXLin_clicked(bool checked)
{
    if (!_busy)
    {
        emit HTCChartXLinChartRequest(checked);
    }
}

// TODO:
// Add protection for negative min/max values
// +++++++++++++++++++++++++++++++++++++++++++
void ChartProperties::on_radioXLog_clicked(bool checked)
{

    if(_xAxisScaleMin < 0 || _xAxisScaleMax < 0)
    {
        //ignore click
        _busy = true;
        setLinLogChecked("X", "LIN");
        _busy = false;
    }
    else
    {
        emit HTCChartXLogChartRequest(checked);
    }

}


void ChartProperties::on_radioYLin_clicked(bool checked)
{
    if (!_busy)
    {
        emit HTCChartYLinChartRequest(checked);

    }
}

void ChartProperties::on_lineXMax_editingFinished()
{
    _xAxisScaleMin = ui->lineXMax->text().toDouble();
    emit HTCChartXAxisLabelsTextScaleMaxChanged(_xAxisScaleMin);

}

void ChartProperties::on_lineXMin_editingFinished()
{
    _xAxisScaleMax = ui->lineXMin->text().toDouble();
    emit HTCChartXAxisLabelsTextScaleMinChanged(_xAxisScaleMax);
}

void ChartProperties::on_lineYMin_editingFinished()
{
    _yAxisScaleMin = ui->lineYMin->text().toDouble();
    qDebug() << "value from editing is " << _yAxisScaleMin;

    //qDebug() << "New Y-Min Value requested is " << _yAxisScaleMin;
    emit HTCChartYAxisLabelsTextScaleMinChanged(_yAxisScaleMin);
}

void ChartProperties::on_lineYMax_editingFinished()
{
    _yAxisScaleMax = ui->lineYMax->text().toDouble();

    qDebug() << "New Y-Max Value requested is " << _yAxisScaleMax;
    emit HTCChartYAxisLabelsTextScaleMaxChanged(_yAxisScaleMax);
}


// maybe pass a control and pen nmumber
// ------------------------------------
void ChartProperties::on_pBttonPen01_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen01->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen01->setPalette(palPen);
            ui->spinBox01->setPalette(palPen);

            updatePenItemsAndNotify(1);

        }
    }


}


void ChartProperties::on_linePen01_textChanged(const QString &arg1)
{
    //

}

void ChartProperties::on_comboPen01_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(1);
    }
}

void ChartProperties::on_linePen01_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(1);
    }
}

void ChartProperties::setToolTips()
{
    ui->textChartTitleText->setToolTip("Use &lt;br&gt; in the text to start a next line.");
    ui->pButtonAddPen->setToolTip("Clieck here to add a new Limit trace to the chart.");
    ui->buttonRemovePen->setToolTip("Click here to remove previously added limit lines, one at a time");
    ui->linePenName->setToolTip("Enter text here to give your limit line a meaningful identity.");
    ui->linePenValue->setToolTip("Whatever values you enter here will be used accross the full range of the chart.");

    ui->btnChartTitleTextFont->setToolTip(ui->textChartTitleText->font().toString());
    ui->btnXAxisLabelfont->setToolTip(ui->txtXAxisLabelText->font().toString());
    ui->btnYAxisLabelfont->setToolTip(ui->txtYAxisLabelText->font().toString());
    ui->btnXAxisValuesFont->setToolTip(ui->txtXLabels->font().toString());
    ui->btnYAxisValuesFont->setToolTip(ui->txtYLabels->font().toString());

    ui->btnChartTitleTextColor->setToolTip("Color: " + getColorStringFromPalette(ui->textChartTitleText->palette()));
    ui->btnXaxisLabelColor->setToolTip("Color: " + getColorStringFromPalette(ui->txtXAxisLabelText->palette()));
    ui->btnYaxisLabelColor->setToolTip("Color: " + getColorStringFromPalette(ui->txtYAxisLabelText->palette()));
    ui->btnXAxisValuesColor->setToolTip("Color: " + getColorStringFromPalette(ui->txtXLabels->palette()));
    ui->btnYAxisValuesColor->setToolTip("Color: " + getColorStringFromPalette(ui->txtYLabels->palette()));

    ui->btnXAxisMajorGridColor->setToolTip("Color: " + getColorStringFromPalette(ui->spinXMajorTics->palette()));
    ui->btnXAxisMinorGridColor->setToolTip("Color: " + getColorStringFromPalette(ui->spinXMinorTics->palette()));
    ui->btnYAxisMajorGridColor->setToolTip("Color: " + getColorStringFromPalette(ui->spinYMajorTics->palette()));
    ui->btnYAxisMinorGridColor->setToolTip("Color: " + getColorStringFromPalette(ui->spinYMinorTics->palette()));


}

void ChartProperties::on_spinXMajorThickness_valueChanged(int arg1)
{
    if (!_busy)
    {
        emit HTCChartXMajorThicknessValueChanged(arg1);
    }
}

void ChartProperties::on_spinXMinorThickness_valueChanged(int arg1)
{
    if (!_busy)
    {

        emit HTCChartXMinorThicknessValueChanged(arg1);
    }
}

void ChartProperties::on_spinYMajorThickness_valueChanged(int arg1)
{
    if (!_busy)
    {
        emit HTCChartYMajorThicknessValueChanged(arg1);
    }
}

void ChartProperties::on_spinYMinorThickness_valueChanged(int arg1)
{
    if (!_busy)
    {
        emit HTCChartYMinorThicknessValueChanged(arg1);
    }
}


void ChartProperties::on_spinYMajorTics_valueChanged(int arg1)
{
    if (!_busy)
    {
        if (ui->checkYMajor->isChecked() == true)
        {
            emit HTCChartYMajorTicsValueChanged(arg1);
        }
    }
}

void ChartProperties::on_spinYMinorTics_valueChanged(int arg1)
{
    if (!_busy)
    {
        if (ui->checkYMinor->isChecked() == true)
        {
            emit HTCChartYMinorTicsValueChanged(arg1);
        }
    }
}

void ChartProperties::on_spinXMajorTics_valueChanged(int arg1)
{

        if (!_busy)
        {
            if (ui->checkXMajor->isChecked() == true)
            {
                emit HTCChartXMajorTicsValueChanged(arg1);
            }

        }
}

void ChartProperties::on_spinXMinorTics_valueChanged(int arg1)
{

    if (!_busy)
    {
        if (ui->checkXMinor->isChecked() == true)
        {
            emit HTCChartXMinorTicsValueChanged(arg1);
        }
    }
}

void ChartProperties::on_pBttonPen02_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen02->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen02->setPalette(palPen);
            ui->spinBox02->setPalette(palPen);

            updatePenItemsAndNotify(2);

        }
    }

}

void ChartProperties::on_pBttonPen03_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen03->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen03->setPalette(palPen);
            ui->spinBox03->setPalette(palPen);

            updatePenItemsAndNotify(3);

        }
    }

}

void ChartProperties::on_pBttonPen04_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen04->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen04->setPalette(palPen);
            ui->spinBox04->setPalette(palPen);

            updatePenItemsAndNotify(4);

        }
    }

}

void ChartProperties::on_pBttonPen05_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen05->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen05->setPalette(palPen);
            ui->spinBox05->setPalette(palPen);

            updatePenItemsAndNotify(5);

        }
    }

}

void ChartProperties::on_pBttonPen06_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen06->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen06->setPalette(palPen);
            ui->spinBox06->setPalette(palPen);

            updatePenItemsAndNotify(6);

        }
    }

}

void ChartProperties::on_pBttonPen07_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen07->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen07->setPalette(palPen);
            ui->spinBox07->setPalette(palPen);

            updatePenItemsAndNotify(7);

        }
    }

}

void ChartProperties::on_pBttonPen08_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen08->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen08->setPalette(palPen);
            ui->spinBox08->setPalette(palPen);

            updatePenItemsAndNotify(8);

        }
    }

}

void ChartProperties::on_pBttonPen09_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen09->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen09->setPalette(palPen);
            ui->spinBox09->setPalette(palPen);

            updatePenItemsAndNotify(9);

        }
    }

}

void ChartProperties::on_pBttonPen10_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen10->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen10->setPalette(palPen);
            ui->spinBox10->setPalette(palPen);

            updatePenItemsAndNotify(10);

        }
    }

}

void ChartProperties::on_pBttonPen11_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen11->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen11->setPalette(palPen);
            ui->spinBox11->setPalette(palPen);

            updatePenItemsAndNotify(11);

        }
    }

}

void ChartProperties::on_pBttonPen12_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen12->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen12->setPalette(palPen);
            ui->spinBox12->setPalette(palPen);

            updatePenItemsAndNotify(12);

        }
    }

}

void ChartProperties::on_pBttonPen13_clicked()
{
    if (!_busy)
    {
        QPalette  pal = ui->linePen13->palette();
        QColor currentColor = pal.color(QPalette::Text);

        QColor Labelcolor = getNewColor(currentColor);
        QPalette palPen;

        palPen.setColor(QPalette::Text, Labelcolor);
        palPen.setColor(QPalette::Base, Qt::white);


        if (Labelcolor != currentColor)
        {
            ui->linePen13->setPalette(palPen);
            ui->spinBox13->setPalette(palPen);

            updatePenItemsAndNotify(13);

        }
    }

}



void ChartProperties::on_spinBox01_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(1);
    }
}

void ChartProperties::on_spinBox02_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(2);
    }
}

void ChartProperties::on_spinBox03_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(3);
    }
}

void ChartProperties::on_spinBox04_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(4);
    }
}

void ChartProperties::on_spinBox05_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(5);
    }
}

void ChartProperties::on_spinBox06_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(6);
    }
}

void ChartProperties::on_spinBox07_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(7);
    }
}

void ChartProperties::on_spinBox08_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(8);
    }
}

void ChartProperties::on_spinBox09_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(9);
    }
}

void ChartProperties::on_spinBox10_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(10);
    }
}

void ChartProperties::on_spinBox11_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(11);
    }
}

void ChartProperties::on_spinBox12_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(12);
    }
}

void ChartProperties::on_spinBox13_valueChanged(int arg1)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(13);
    }
}



void ChartProperties::on_comboPen02_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(2);
    }
}

void ChartProperties::on_comboPen03_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(3);
    }
}

void ChartProperties::on_comboPen04_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(4);
    }
}

void ChartProperties::on_comboPen05_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(5);
    }
}

void ChartProperties::on_comboPen06_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(6);
    }
}

void ChartProperties::on_comboPen07_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(7);
    }
}

void ChartProperties::on_comboPen08_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(8);
    }
}

void ChartProperties::on_comboPen09_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(9);
    }
}

void ChartProperties::on_comboPen10_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(10);
    }
}

void ChartProperties::on_comboPen11_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(11);
    }
}

void ChartProperties::on_comboPen12_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(12);
    }
}

void ChartProperties::on_comboPen13_currentIndexChanged(int index)
{
    if (!_busy)
    {
        updatePenItemsAndNotify(13);
    }
}



void ChartProperties::on_pButtonAddPen_clicked()
{
    double baseValue=0;
    QString header;
    bool valuesAreGood = true;


    //listPenStates("Before adding a pen");

    if(!ui->linePenValue->text().isEmpty())
    {
        baseValue = ui->linePenValue->text().toDouble();
    }
    else
    {
        valuesAreGood = false;
    }

    if(!ui->linePenName->text().isEmpty() && valuesAreGood == true)
    {
        header = ui->linePenName->text();
    }
    else
    {
        valuesAreGood = false;
    }

    // check to make sure we're not trying to create too many
    int nextPen = findLastEnabledPen();

    if(nextPen < _maxNumberOfPens)
    {
        if(valuesAreGood)
        {
            emit HTCCHartAddPenRequest(baseValue, header);
            ui->linePenName->setText("");
            ui->linePenValue->setText("");
        }
    }
    else
    {
        showNoMorePensAllowedMessage(nextPen);
        ui->linePenName->setText("");
        ui->linePenValue->setText("");
    }

    //listPenStates("After adding a pen");

}

void ChartProperties::setControlStyles()
{
    // Get all the labels into a collection
    // and set their stylesheet.


}



void ChartProperties::on_radioYDecimal_clicked(bool checked)
{
    if (!_busy)
    {

        emit HTCChartYDecimalRequest(checked);

    }

}

void ChartProperties::on_radioYScientific_clicked(bool checked)
{
    if (!_busy)
    {

        emit HTCChartYScientificRequest(checked);

    }
}

void ChartProperties::on_radioXScientific_clicked(bool checked)
{
    if (!_busy)
    {
        emit HTCChartXScientificRequest(checked);

    }
}

void ChartProperties::on_radioXDecimal_clicked(bool checked)
{
    if (!_busy)
    {
        emit HTCChartXDecimalRequest(checked);

    }
}

void ChartProperties::on_buttonRemovePen_clicked()
{

    int penToDelete = findLastEnabledPen();

    ui->linePenName->setText("");
    ui->linePenValue->setText("");

    QString penName = getPenTagByID(penToDelete + 1);

    if (isPenDeletable(penName) == true)
    {
        clearDeletedPen(penToDelete + 1);

        emit HTCChartRemovePen(penToDelete);

        disablePen(penToDelete + 1);

    }
    else
    {
        showNoDeleteForPenMessage(penName);

    }

}

void ChartProperties::on_spinYPrecision_valueChanged(int arg1)
{
    if (!_busy)
    {
        emit HTCCHartYPrecisionChangeRequest(arg1);
        //qDebug() << "Y request for " << arg1;

    }
}

void ChartProperties::on_spinXPrecision_valueChanged(int arg1)
{
    if (!_busy)
    {
        emit HTCCHartXPrecisionChangeRequest(arg1);
        //qDebug() << "X request for " << arg1;

    }
}

void ChartProperties::on_linePen02_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(2);
    }
}

void ChartProperties::on_linePen03_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(3);
    }
}

void ChartProperties::on_linePen04_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(4);
    }
}

void ChartProperties::on_linePen05_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(5);
    }
}

void ChartProperties::on_linePen06_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(6);
    }
}

void ChartProperties::on_linePen07_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(7);
    }
}

void ChartProperties::on_linePen08_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(8);
    }
}

void ChartProperties::on_linePen09_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(9);
    }
}

void ChartProperties::on_linePen10_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(10);
    }
}

void ChartProperties::on_linePen11_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(11);
    }
}

void ChartProperties::on_linePen12_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(12);
    }
}

void ChartProperties::on_linePen13_editingFinished()
{
    if (!_busy)
    {
        updatePenItemsAndNotify(13);
    }
}
