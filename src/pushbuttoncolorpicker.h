#ifndef PUSHBUTTONCOLORPICKER_H
#define PUSHBUTTONCOLORPICKER_H

#include <QPushButton>

class PushButtonColorPicker : public QPushButton
{
  Q_OBJECT
public:
  PushButtonColorPicker(QColor color = QColor(0, 0, 0, 0), QWidget *parent = nullptr);
  ~PushButtonColorPicker();

  void setColor(QColor color);
  QColor getColor();

private slots:
  void buttonClicked();

private:
  void setButtonStyle(QColor color);
};

#endif // PUSHBUTTONCOLORPICKER_H
