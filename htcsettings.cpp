// ++++++++++++++++++++++++++++++++++++++++++++ //
// File: htcsettings.cpp
// Description: Provides a method for users
// to change program settings
//
//
// Date: 2019-03-07
//
//
// TODO:
//
//
//
// ++++++++++++++++++++++++++++++++++++++++++++

#include "htcsettings.h"
#include "ui_htcsettings.h"

HTCSettings::HTCSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HTCSettings)
{
    ui->setupUi(this);

    _busy = true;

    QString chartTitle = "";
    chartTitle.append("HTC ChartBuilder Settings");
    this->setWindowTitle(chartTitle);

    _positions.clear();
    _defaultPositions << "Padding Value" << "Baseline" << "0° Horiz" << "0° Vert" << "90° Horiz"
                      << "90° Vert" << "180° Horiz" << "180° Vert" << "270° Horiz" << "270° Vert";

    _positions = _defaultPositions;

    loadSettings();
    setupControls();
    loadPositionsList();
    connect(ui->listPositions, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(positionItemSelected(QListWidgetItem*)));



    _busy = false;

}

HTCSettings::~HTCSettings()
{
    delete ui;
}

void HTCSettings::on_btnClose_clicked()
{
    close();
}

void HTCSettings::setupControls()
{
    connect(ui->listChartColors, SIGNAL(itemSelectionChanged()), this, SLOT(ListViewItemSelected()));

    ui->btnApply->setEnabled(false);

    _intValidator = new QIntValidator();
    _intValidator->setRange(0,2000);

    _dblValidator = new QDoubleValidator();
    _dblValidator->setBottom(0.00);
    _dblValidator->setTop(100.00);
    _dblValidator->setDecimals(2);

    _formFont = QFont("Arial",9, QFont::Normal );
    ui->comboChartPaddingY->addItem(_ChartPaddingValuesY[0]);
    ui->comboChartPaddingY->addItem(_ChartPaddingValuesY[1]);
    ui->comboChartPaddingX->addItem(_ChartPaddingValuesX[0]);
    ui->comboChartPaddingX->addItem(_ChartPaddingValuesX[1]);

    setValidators();
    loadColorList();
    loadFontList();
    setControlsFont();
    setCombos();
    getformValues();
    setFormValues();



}

QColor HTCSettings::getColorFromPalette(QPalette pal)
{
    QColor currentColor = pal.color(QPalette::Text);
    return currentColor;
}

QColor HTCSettings::getNewColor(QColor currentColor)
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

void HTCSettings::loadColorList()
{



    for (int i = 0; i < 13; i++)
    {
        //QListWidget t;
        QColor c = QColor(_penColors[i]);
        ui->listChartColors->addItem(_penColors[i].name());
        ui->listChartColors->item(i)->setForeground(c);
    }
    palLabel.setColor(QPalette::Base, Qt::white);
    palLabel.setColor(QPalette::Background, Qt::white);
    ui->lineColorTell->setPalette(palLabel);
    //qDebug() << "set the label base color";
}

void HTCSettings::loadFontList()
{
    ui->comboFontNames->addItem(_ChartLegendFontValues[0]);
    ui->comboFontNames->addItem(_ChartLegendFontValues[1]);
    ui->comboFontNames->addItem(_ChartLegendFontValues[2]);

}

void HTCSettings::setControlsFont()
{
    // set 2 combo boxes
    ui->comboFontNames->setFont(_formFont);
    ui->comboFontNames->setEditable(true);
    ui->comboFontNames->lineEdit()->setReadOnly(true);
    ui->comboFontNames->lineEdit()->setAlignment(Qt::AlignCenter);
    ui->comboChartPaddingY->setFont(_formFont);
    ui->comboChartPaddingY->setEditable(true);
    ui->comboChartPaddingY->lineEdit()->setReadOnly(true);
    ui->comboChartPaddingY->lineEdit()->setAlignment(Qt::AlignCenter);

    ui->comboChartPaddingX->setFont(_formFont);
    ui->comboChartPaddingX->setEditable(true);
    ui->comboChartPaddingX->lineEdit()->setReadOnly(true);
    ui->comboChartPaddingX->lineEdit()->setAlignment(Qt::AlignCenter);

    ui->listChartColors->setFont(_formFont);

    // set 6 labels
    ui->lblXPos->setFont(_formFont);


    ui->lblYPos->setFont(_formFont);


    ui->lblWidth->setFont(_formFont);

    ui->lblHeight->setFont(_formFont);
    ui->lblFontSize->setFont(_formFont);
    ui->lblFontFamily->setFont(_formFont);

    // set 5 line edits
    ui->lineSize->setFont(_formFont);
    ui->lineSize->setAlignment(Qt::AlignRight);
    ui->lineXpos->setFont(_formFont);
    ui->lineXpos->setAlignment(Qt::AlignRight);
    ui->lineYpos->setFont(_formFont);
    ui->lineYpos->setAlignment(Qt::AlignRight);
    ui->lineWidth->setFont(_formFont);
    ui->lineWidth->setAlignment(Qt::AlignRight);
    ui->lineHeight->setFont(_formFont);
    ui->lineHeight->setAlignment(Qt::AlignRight);
    ui->linePaddingValueY->setFont(_formFont);
    ui->linePaddingValueY->setAlignment(Qt::AlignRight);
    ui->linePaddingValueX->setFont(_formFont);
    ui->linePaddingValueX->setAlignment(Qt::AlignRight);


}

void HTCSettings::setValidators()
{
    ui->lineSize->setValidator(_intValidator);
    ui->lineXpos->setValidator(_intValidator);
    ui->lineYpos->setValidator(_intValidator);
    ui->lineWidth->setValidator(_intValidator);
    ui->lineHeight->setValidator(_intValidator);
    ui->linePaddingValueY->setValidator(_dblValidator);
    ui->linePaddingValueX->setValidator(_dblValidator);

}

void HTCSettings::getformValues()
{
//    _xStart = 50;
//    _yStart = 50;
//    _width = 960;
//    _height = 767;
//    _fontSize = 9;



}

void HTCSettings::setFormValues()
{

    ui->lineSize->setText(QString::number(_ChartLegendFontSizeValue));
    ui->lineXpos->setText(QString::number(_xGeoStart));
    ui->lineYpos->setText(QString::number(_yGeoStart));
    ui->lineWidth->setText(QString::number(_GeoWidth));
    ui->lineHeight->setText(QString::number(_GeoHeight));
    ui->linePaddingValueY->setText(QString::number(_ChartScalePaddingValueY));
    ui->linePaddingValueX->setText(QString::number(_ChartScalePaddingValueX));

}

void HTCSettings::setCombos()
{
    ui->comboFontNames->setCurrentIndex(_ChartLegendFontFamilyValue);
    ui->comboChartPaddingY->setCurrentIndex(_ChartPaddingValueY);
    ui->comboChartPaddingX->setCurrentIndex(_ChartPaddingValueX);


}

void HTCSettings::saveSettings()
{


    // store settings
    _layout = QRect(_xGeoStart, _yGeoStart, _GeoWidth, _GeoHeight);

    QSettings setting("Keysight","ChartBuilder");

    setting.beginGroup("ProgramFolders");

    setting.setValue("ChartLegendFontSize",QString::number(_ChartLegendFontSizeValue));
    setting.setValue("ChartLegendFontFamily",_ChartLegendFontFamilyValue);

    // since I couldn't figure out how to store an array
    // we'll just save them one at a time.
    setting.setValue("ChartPen01", _penColors[0].name());
    setting.setValue("ChartPen02", _penColors[1].name());
    setting.setValue("ChartPen03", _penColors[2].name());
    setting.setValue("ChartPen04", _penColors[3].name());
    setting.setValue("ChartPen05", _penColors[4].name());
    setting.setValue("ChartPen06", _penColors[5].name());
    setting.setValue("ChartPen07", _penColors[6].name());
    setting.setValue("ChartPen08", _penColors[7].name());
    setting.setValue("ChartPen09", _penColors[8].name());
    setting.setValue("ChartPen10", _penColors[9].name());
    setting.setValue("ChartPen11", _penColors[10].name());
    setting.setValue("ChartPen12", _penColors[11].name());
    setting.setValue("ChartPen13", _penColors[12].name());


    setting.setValue("ChartScalePaddingYOn",QString::number(_ChartPaddingValueY));
    setting.setValue("ChartScalePaddingYValue",QString::number(_ChartScalePaddingValueY));

    setting.setValue("ChartScalePaddingXOn",QString::number(_ChartPaddingValueX));
    setting.setValue("ChartScalePaddingXValue",QString::number(_ChartScalePaddingValueX));


    setting.setValue("CommCheckAutoDetectEnabled",QString::number(_commCheckAutoDetect));

    setting.setValue("ChartStartupPosition",_layout);

    setting.setValue("LegendPosition1", _positions[0]);
    setting.setValue("LegendPosition2", _positions[1]);
    setting.setValue("LegendPosition3", _positions[2]);
    setting.setValue("LegendPosition4", _positions[3]);
    setting.setValue("LegendPosition5", _positions[4]);
    setting.setValue("LegendPosition6", _positions[5]);
    setting.setValue("LegendPosition7", _positions[6]);
    setting.setValue("LegendPosition8", _positions[7]);
    setting.setValue("LegendPosition9", _positions[8]);
    setting.setValue("LegendPosition10", _positions[9]);

    setting.setValue("ChartLegendOverride",QString::number(_OverrideLegendValue)); //
    setting.setValue("EnableChartAnimations",QString::number(_EnableAnimations));
    setting.setValue("EnableHoverCallout",QString::number(_EnableHoverCallout));

   // qDebug() << "saved Enable animations/EnableHoverCallout " << _EnableAnimations << "/" << _EnableHoverCallout;


    setting.endGroup();
}

void HTCSettings::loadSettings()
{
    QString penValue = "";
    QString positionValue = "";

    QSettings setting("Keysight","ChartBuilder");

    setting.beginGroup("ProgramFolders");

    _ChartLegendFontSizeValue = setting.value("ChartLegendFontSize").toInt();
    _ChartLegendFontFamilyValue = setting.value("ChartLegendFontFamily").toInt();

    _ChartPaddingValueY = setting.value("ChartScalePaddingYOn").toInt();
    ui->comboChartPaddingY->setCurrentIndex(_ChartPaddingValueY);

    _ChartScalePaddingValueY = setting.value("ChartScalePaddingYValue").toDouble();
    ui->linePaddingValueY->setText(QString::number(_ChartScalePaddingValueY));


    _ChartPaddingValueX = setting.value("ChartScalePaddingXOn").toInt();
    ui->comboChartPaddingX->setCurrentIndex(_ChartPaddingValueX);

    _ChartScalePaddingValueX = setting.value("ChartScalePaddingXValue").toDouble();
    ui->linePaddingValueX->setText(QString::number(_ChartScalePaddingValueX));


    _commCheckAutoDetect = setting.value("CommCheckAutoDetectEnabled").toInt();

    if(_commCheckAutoDetect == 1)
    {
        ui->radioCommAutoOn->toggle();
    }
    else
    {
        ui->radioCommAutoOff->toggle();
    }

    // ---------------------------------------------------------//
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

    // New Override for legend IDs
    // ----------------------------------------------- //
    setLegendOverrideSelector(_OverrideLegendValue);




    //
    //  new chart animations
    //
    // this is in service and works
    //
    if (setting.value("EnableChartAnimations").toInt() == 1)
    {
        _EnableAnimations = true;
    }
    else
    {
        _EnableAnimations = false;


    }

    setEnableChartAnimationsSelector(_EnableAnimations);


    //
    // new hover callout
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


    setEnableHoverCalloutSelector(_EnableHoverCallout);

   // end of new cool stuff

    // chart fonts
    if(_ChartLegendFontSizeValue == 0)
    {
        _ChartLegendFontSizeValue = _defaultChartLegendFontSizeValue;
    }
    ui->lineSize->setText(QString::number(_ChartLegendFontSizeValue));

    if(_ChartLegendFontFamilyValue > 2 || _ChartLegendFontFamilyValue < 0)
    {
        _ChartLegendFontFamilyValue = _defaultChartLegendFontFamilyValue;
    }

    ui->comboFontNames->setCurrentIndex(_ChartLegendFontFamilyValue);





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
    // the first value is read only

//    positionValue = setting.value("LegendPosition1").toString();
//    if(positionValue.isEmpty())
//    {
//        _positions[0] = _defaultPositions[0];
//    }
//    else
//    {
//        _positions[0] = positionValue;
//    }
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
    ui->lineXpos->setText(QString::number(_xGeoStart));

    _yGeoStart = _layout.y();
    ui->lineYpos->setText(QString::number(_yGeoStart));

    _GeoWidth = _layout.width();
    ui->lineWidth->setText(QString::number(_GeoWidth));

    _GeoHeight = _layout.height();
    ui->lineHeight->setText(QString::number(_GeoHeight));


    setting.endGroup();
}

void HTCSettings::setLegendOverrideSelector(bool SetOVerrideOn)
{
    if (SetOVerrideOn ==  true)
    {
        ui->radioRILegendFile->toggle();
    }
    else
    {
        ui->radioRILegendAuto->toggle();
    }



}

void HTCSettings::setEnableChartAnimationsSelector(bool SetAnimationsOn)
{
    if (SetAnimationsOn == true)
    {
        ui->radioAnimationsOn->toggle();
    }
    else
    {
        ui->radioAnimationsOff->toggle();
    }

}

void HTCSettings::setEnableHoverCalloutSelector(bool SetCalloutHoverOn)
{
    if (SetCalloutHoverOn)
    {
        ui->radioHoverOn->toggle();
    }
    else
    {
        ui->radioHoverOff->toggle();
    }

}





void HTCSettings::initPositions()
{


}

void HTCSettings::loadPositionsList()
{
    ui->listPositions->setFont(_formFont);
    for (int i = 1; i < _positions.count(); i++)
    {
        ui->listPositions->addItem(_positions.at(i));

    }

}

void HTCSettings::on_btnPenColor_clicked()
{

    QString s1;
    QString s2;
    QString listValue;

    int selectedIDX = ui->listChartColors->currentRow();

    if(selectedIDX > -1)
    {
        listValue = ui->listChartColors->currentItem()->text();

        QColor color = QColor(listValue);
        QColor newColor = getNewColor(color);

        s1 = color.name();
        s2 = newColor.name();

        if(s1 != s2)
        {
            ui->listChartColors->currentItem()->setText(newColor.name());

            ui->listChartColors->currentItem()->setForeground(newColor);

            palLabel.setColor(QPalette::Base, newColor);
            palLabel.setColor(QPalette::Background, newColor);
            ui->lineColorTell->setPalette(palLabel);

            ui->btnApply->setEnabled(true);
            _penColors[selectedIDX] = newColor;

        }


    }


}

void HTCSettings::ListViewItemSelected()
{
    QString msg = "";
    int selectedIDX = ui->listChartColors->currentRow();
    selectedIDX++;

    QString name = ui->listChartColors->currentItem()->text();


    QColor color = QColor(name);
    palLabel.setColor(QPalette::Base, color);
    palLabel.setColor(QPalette::Background, color);

    ui->lineColorTell->setPalette(palLabel);


    msg.append("Pen# ");
    msg.append(QString::number(selectedIDX));
    msg.append(" - current hex color value is ");
    msg.append(name);
    this->statusBar()->showMessage(msg);
}

void HTCSettings::on_btnResetColors_clicked()
{

    _busy = true;
    ui->listChartColors->clear();
    for (int i = 0; i < 15; i++)
    {
        QColor c = QColor(_penColorsReset[i]);
        ui->listChartColors->addItem(_penColorsReset[i].name());
        ui->listChartColors->item(i)->setForeground(c);
    }

    _busy = false;

}

//void HTCSettings::on_comboChartPadding_currentIndexChanged(int index)
//{

//    if(!_busy)
//    {
//        ui->btnApply->setEnabled(true);
//        _ChartPaddingValue = index;

//    }

//}

void HTCSettings::on_lineXpos_editingFinished()
{
    if(!_busy)
    {
        QString v1 = ui->lineXpos->text();
        QString v2 = QString::number(_xGeoStart);
        if(v1 != v2)
        {
            ui->btnApply->setEnabled(true);
            _xGeoStart = v1.toInt();
        }

    }

}

void HTCSettings::on_lineYpos_editingFinished()
{
    if(!_busy)
    {
        QString v1 = ui->lineYpos->text();
        QString v2 = QString::number(_yGeoStart);
        if(v1 != v2)
        {
            ui->btnApply->setEnabled(true);
            _yGeoStart = v1.toInt();
        }
    }
}

void HTCSettings::on_lineWidth_editingFinished()
{
    if(!_busy)
    {
        QString v1 = ui->lineWidth->text();
        QString v2 = QString::number(_GeoWidth);
        if(v1 != v2)
        {
            ui->btnApply->setEnabled(true);
            _GeoWidth = v1.toInt();
        }
    }
}

void HTCSettings::on_lineHeight_editingFinished()
{
    if(!_busy)
    {
        QString v1 = ui->lineHeight->text();
        QString v2 = QString::number(_GeoHeight);
        if(v1 != v2)
        {
            ui->btnApply->setEnabled(true);
            _GeoHeight = v1.toInt();
        }
    }
}

void HTCSettings::on_lineSize_editingFinished()
{
    if(!_busy)
    {
        QString v1 = ui->lineSize->text();
        QString v2 = QString::number(_ChartLegendFontSizeValue);
        if(v1 != v2)
        {
            ui->btnApply->setEnabled(true);
            _ChartLegendFontSizeValue = v1.toInt();
        }
    }
}


void HTCSettings::on_listChartColors_currentTextChanged(const QString &currentText)
{
    if(!_busy)
    {
        int selectedIDX = ui->listChartColors->currentRow();
        QString SelectedName = ui->listChartColors->currentItem()->text();
        QString currentName = QColor(_penColors[selectedIDX]).name();



        if (SelectedName != currentName)
        {
            ui->btnApply->setEnabled(true);
            QColor newColor = QColor(SelectedName);
            _penColors[selectedIDX] = newColor;
        }



    }
}

void HTCSettings::on_btnApply_clicked()
{
    saveSettings();
    ui->btnApply->setEnabled(false);
}

void HTCSettings::on_comboFontNames_currentIndexChanged(int index)
{
    if(!_busy)
    {
        ui->btnApply->setEnabled(true);
        qDebug() << "font Names index changed to " << index;
        _ChartLegendFontFamilyValue = index;
    }
}



void HTCSettings::positionItemSelected(QListWidgetItem *item)
{
    int selectedIDX = ui->listPositions->currentRow();
    pos = new PositionEditor(this);

    connect(pos, SIGNAL(EditedPositionValue(QString, int)), this, SLOT(EditedPositionValueChanged(QString, int)));
    pos->SetPositionValue(item->text(), selectedIDX);
    pos->show();


}

void HTCSettings::EditedPositionValueChanged(QString value, int itemNumber)
{
    ui->listPositions->setCurrentRow(itemNumber);
    ui->listPositions->currentItem()->setText(value);
    //_positions[itemNumber] = value;
    _positions[itemNumber + 1] = value;
    ui->btnApply->setEnabled(true);
}

void HTCSettings::on_radioRILegendAuto_clicked()
{
    // Default - use fixed legend strings
    _OverrideLegendValue = false;
    ui->btnApply->setEnabled(true);
}

void HTCSettings::on_radioRILegendFile_clicked()
{
    // new - use file name parts
    _OverrideLegendValue = true;
    ui->btnApply->setEnabled(true);

}

void HTCSettings::on_radioAnimationsOn_clicked()
{
    _EnableAnimations = true;
    ui->btnApply->setEnabled(true);

    qDebug() << "set animations " << _EnableAnimations;

}

void HTCSettings::on_radioAnimationsOff_clicked()
{
    _EnableAnimations = false;
    ui->btnApply->setEnabled(true);

    qDebug() << "set animations " << _EnableAnimations;
}

void HTCSettings::on_radioHoverOn_clicked()
{
    _EnableHoverCallout = true;
    ui->btnApply->setEnabled(true);
    qDebug() << "Set hover " << _EnableHoverCallout;
}

void HTCSettings::on_radioHoverOff_clicked()
{
    _EnableHoverCallout = false;
    ui->btnApply->setEnabled(true);
    qDebug() << "Set hover " << _EnableHoverCallout;
}

void HTCSettings::on_comboChartPaddingY_currentIndexChanged(int index)
{
    if(!_busy)
    {
        ui->btnApply->setEnabled(true);
        _ChartPaddingValueY = index;

    }
}

void HTCSettings::on_comboChartPaddingX_currentIndexChanged(int index)
{
    if(!_busy)
    {
        ui->btnApply->setEnabled(true);
        _ChartPaddingValueX = index;

    }
}

void HTCSettings::on_linePaddingValueY_editingFinished()
{
    if(!_busy)
    {
        if(_ChartPaddingValueY==1)
        {
            _ChartScalePaddingValueY = ui->linePaddingValueY->text().toDouble();
            ui->btnApply->setEnabled(true);
        }
        else
        {
            ui->linePaddingValueY->setText(QString::number(_ChartScalePaddingValueY));
        }
    }
}



void HTCSettings::on_linePaddingValueX_editingFinished()
{
    if(!_busy)
    {
        if(_ChartPaddingValueX==1)
        {
            _ChartScalePaddingValueX = ui->linePaddingValueX->text().toDouble();

            ui->btnApply->setEnabled(true);
        }
        else
        {
            ui->linePaddingValueX->setText(QString::number(_ChartScalePaddingValueX));
        }
    }
}

void HTCSettings::on_radioCommAutoOn_clicked()
{
    if(!_busy)
    {
        _commCheckAutoDetect = 1;
        ui->btnApply->setEnabled(true);
    }
}

void HTCSettings::on_radioCommAutoOff_clicked()
{
    if(!_busy)
    {
        _commCheckAutoDetect = 0;
        ui->btnApply->setEnabled(true);
    }
}
