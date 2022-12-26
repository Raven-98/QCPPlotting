#ifndef FONTWIDGET_H
#define FONTWIDGET_H

#include <QWidget>
#include <QFontComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>

class FontWidget : public QWidget
{
  Q_OBJECT
public:
  explicit FontWidget(QFont font, QWidget *parent = nullptr);
  explicit FontWidget(QWidget *parent = nullptr);
  ~FontWidget();

  void set(QFont font);
  QFont get();

private:
  void init();

private:
  QFontComboBox *comboBox_FontName{nullptr};
  QSpinBox *spinBox_FontSize{nullptr};
  QPushButton *button_FontBold{nullptr};
  QPushButton *button_FontItalic{nullptr};
  QPushButton *button_FontUnderline{nullptr};
  QPushButton *button_FontStrikeOut{nullptr};
  QHBoxLayout *hLayout{nullptr};
};

#endif // FONTWIDGET_H
