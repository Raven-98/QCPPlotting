#include "pushbuttoncolorpicker.h"
#include "qpainter.h"

#include <QColorDialog>

PushButtonColorPicker::PushButtonColorPicker(QColor color, QWidget *parent)
  : QPushButton(parent)
{
  setButtonStyle(color);
  connect(this, &QPushButton::clicked, this, &PushButtonColorPicker::buttonClicked);
}

PushButtonColorPicker::~PushButtonColorPicker()
{}

void PushButtonColorPicker::setColor(QColor color)
{
  setButtonStyle(color);
}

QColor PushButtonColorPicker::getColor()
{
  return QColor(text());
}

void PushButtonColorPicker::buttonClicked()
{
  QColorDialog colorDialog(QColor(text()), this);
  colorDialog.setOption(QColorDialog::ShowAlphaChannel);
  switch (colorDialog.exec()) {
    case QDialog::Accepted:
      setButtonStyle(colorDialog.currentColor());
    case QDialog::Rejected:
      break;
    default:
      break;
    }
}

void PushButtonColorPicker::setButtonStyle(QColor color)
{
  setText(color.name(QColor::HexArgb));

  QPixmap image(32, 32);
  image.fill(color);
  QPainter p(&image);
  QPen pen(Qt::black, 2);
  p.drawRect(0, 0, 30, 30);
  setIcon(QIcon(image));
//  setStyleSheet(QString("QPushButton:enabled {border-color: %1;}").arg(color.name(QColor::HexRgb)));
}
