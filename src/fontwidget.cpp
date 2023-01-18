#include "fontwidget.h"

FontWidget::FontWidget(QFont font, QWidget *parent)
  : QWidget{parent}
{
  init();
  set(font);
}

FontWidget::FontWidget(QWidget *parent)
  : QWidget{parent}
{
  init();
}

FontWidget::~FontWidget()
{
  delete comboBox_FontName;
  delete spinBox_FontSize;
  delete button_FontBold;
  delete button_FontItalic;
  delete button_FontUnderline;
  delete button_FontStrikeOut;
  delete hLayout;
}

void FontWidget::set(QFont font)
{
  comboBox_FontName->setCurrentText(font.family());
  spinBox_FontSize->setValue(font.pointSize());
  button_FontBold->setChecked(font.bold());
  button_FontItalic->setChecked(font.italic());
  button_FontUnderline->setChecked(font.underline());
  button_FontStrikeOut->setChecked(font.strikeOut());
}

QFont FontWidget::get()
{
  QFont font;
  font.setFamily(comboBox_FontName->currentText());
  font.setPointSize(spinBox_FontSize->value());
  font.setBold(button_FontBold->isChecked());
  font.setItalic(button_FontItalic->isChecked());
  font.setUnderline(button_FontUnderline->isChecked());
  font.setStrikeOut(button_FontStrikeOut->isChecked());
  return font;
}

void FontWidget::init()
{
  comboBox_FontName = new QFontComboBox;
  comboBox_FontName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

  spinBox_FontSize = new QSpinBox;

//  button_FontBold = new QPushButton;
  button_FontBold = new QToolButton;
  button_FontBold->setCheckable(true);
  button_FontBold->setText("B");
  button_FontBold->setStyleSheet("font: bold");

//  button_FontItalic = new QPushButton;
  button_FontItalic = new QToolButton;
  button_FontItalic->setCheckable(true);
  button_FontItalic->setText("I");
  button_FontItalic->setStyleSheet("font: italic");

//  button_FontUnderline = new QPushButton;
  button_FontUnderline = new QToolButton;
  button_FontUnderline->setCheckable(true);
  button_FontUnderline->setText("U");
  button_FontUnderline->setStyleSheet("text-decoration: underline");

//  button_FontStrikeOut = new QPushButton;
  button_FontStrikeOut = new QToolButton;
  button_FontStrikeOut->setCheckable(true);
  button_FontStrikeOut->setText("S");
  button_FontStrikeOut->setStyleSheet("text-decoration: line-through");

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(0, 0, 0, 0);
  hLayout->addWidget(comboBox_FontName);
  hLayout->addWidget(spinBox_FontSize);
  hLayout->addWidget(button_FontBold);
  hLayout->addWidget(button_FontItalic);
  hLayout->addWidget(button_FontUnderline);
  hLayout->addWidget(button_FontStrikeOut);

  setLayout(hLayout);
}
