#include "positioneditor.h"
#include "ui_positioneditor.h"

PositionEditor::PositionEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PositionEditor)
{
    ui->setupUi(this);

    QString chartTitle = "";
    chartTitle.append("Legend Text Editor");
    this->setWindowTitle(chartTitle);

    QPoint startPos = QPoint(parentWidget()->window()->frameGeometry().topLeft() +
                           parentWidget()->window()->rect().center() -
                           rect().center());

    // We want the editor to be just right
    // and even vertically with the list so
    // the user is clear what they're working on.
    QPoint offset = QPoint(0,-100);

    QPoint finalPos = QPoint(startPos + offset);
    move(finalPos);


}

PositionEditor::~PositionEditor()
{
    delete ui;
}

void PositionEditor::SetPositionValue(QString value, int itemNumber)
{
    _busy = true;
    ui->btnSave->setEnabled(false);

    _item = itemNumber;
    _value = value;
    ui->linePosition->setText(_value);
    _busy = false;
}


void PositionEditor::on_btnCancel_clicked()
{
    close();
}



void PositionEditor::on_linePosition_textChanged(const QString &arg1)
{


    if (_busy == false)
    {
        ui->btnSave->setEnabled(true);
        _value = ui->linePosition->text();
    }

}

void PositionEditor::on_btnSave_clicked()
{
    emit EditedPositionValue(_value, _item);
    close();
}

void PositionEditor::moveRelativeToParent()
{
    int x, y, h, w;
    x = this->geometry().x();
    y = this->geometry().y();
    h =  this->geometry().height();
    w = this->geometry().width();

    qDebug() << "x was " << x;
    qDebug() << "y was " << y;

    x = x +  _xOffset;
    qDebug() << "x is " << x;
    y = y + _yOffset;
    qDebug() << "y is " << y;

    QRect newRect = QRect(x,y,h,w);
    this->setGeometry(newRect);




}
