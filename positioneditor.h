#ifndef POSITIONEDITOR_H
#define POSITIONEDITOR_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class PositionEditor;
}

class PositionEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PositionEditor(QWidget *parent = nullptr);
    ~PositionEditor();

    void SetPositionValue(QString value, int itemNumber);


signals:

    void EditedPositionValue(QString value, int itemNumber);

private slots:
    void on_btnCancel_clicked();

    void on_linePosition_textChanged(const QString &arg1);

    void on_btnSave_clicked();

private:
    Ui::PositionEditor *ui;

    int _item = -1;
    QString _value = "";
    bool _busy = false;
    QRect _parentGeometry;
    int _xOffset = 50;
    int _yOffset = 0;

    void moveRelativeToParent();


};

#endif // POSITIONEDITOR_H
