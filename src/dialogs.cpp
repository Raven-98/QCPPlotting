#include "dialogs.h"

#include <QDebug>

Dialogs::Dialogs(QWidget *parent)
  : QDialog{parent}
{
  dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  gridLayout = new QGridLayout;
  gridLayout->setMargin(0);

  vBoxLayout = new QVBoxLayout;
  vBoxLayout->addLayout(gridLayout);
  vBoxLayout->addWidget(dialogButtonBox);

  setLayout(vBoxLayout);
}

Dialogs::~Dialogs()
{
  if (gridLayout)
    delete gridLayout;
  if (dialogButtonBox)
    delete dialogButtonBox;
  if (vBoxLayout)
    delete vBoxLayout;
}

FileDialogSetDetails::FileDialogSetDetails(const QCPPlotting::FileDialogSetDetails::Type t, QWidget *parent)
  : Dialogs{parent},
    type(t)
{
  setWindowTitle(tr("Set details"));

  connect(this, &QDialog::accepted, this, &FileDialogSetDetails::slot_accepted);

  if (type == QCPPlotting::FileDialogSetDetails::Open) {
      checkBox_readZeroRow = new QCheckBox(tr("Read zero row"));
      gridLayout->addWidget(checkBox_readZeroRow, 0, 0);
      connect(checkBox_readZeroRow, &QCheckBox::stateChanged, this, &FileDialogSetDetails::slot_readZeroRow);

      checkBox_readZeroColumn = new QCheckBox(tr("Read zero column"));
      gridLayout->addWidget(checkBox_readZeroColumn, 0, 1);

      checkBox_useZeroRow = new QCheckBox(tr("Use a zero row as header"));
      gridLayout->addWidget(checkBox_useZeroRow, 1, 0, 1, 2);
      checkBox_useZeroRow->setEnabled(false);
    }
  else if (type == QCPPlotting::FileDialogSetDetails::Save) {
      checkBox_writeHHeader = new QCheckBox(tr("Write a horizontal header"));
      gridLayout->addWidget(checkBox_writeHHeader, 0, 0, 1, 2);
    }

  label_Delimiter = new QLabel(tr("Delimiter"));

  comboBox_Delimiter = new QComboBox;
  comboBox_Delimiter->addItems({tr("Comma"),
                                tr("Tab step"),
                                tr("Semicolon"),
                                tr("Space"),
                               });

  hBoxLayout_Delimiter = new QHBoxLayout;
  hBoxLayout_Delimiter->addWidget(label_Delimiter);
  hBoxLayout_Delimiter->addWidget(comboBox_Delimiter);
  if (type == QCPPlotting::FileDialogSetDetails::Open)
    gridLayout->addLayout(hBoxLayout_Delimiter, 2, 0, 1, 2);
  else if (type == QCPPlotting::FileDialogSetDetails::Save)
    gridLayout->addLayout(hBoxLayout_Delimiter, 1, 0, 1, 2);
}

FileDialogSetDetails::~FileDialogSetDetails()
{
  if (checkBox_readZeroRow)
    delete checkBox_readZeroRow;
  if (checkBox_readZeroColumn)
    delete checkBox_readZeroColumn;
  if (checkBox_useZeroRow)
    delete checkBox_useZeroRow;
  if (checkBox_writeHHeader)
    delete checkBox_writeHHeader;
  if (label_Delimiter)
    delete label_Delimiter;
  if (comboBox_Delimiter)
    delete comboBox_Delimiter;
  if (hBoxLayout_Delimiter)
    delete hBoxLayout_Delimiter;
}

void FileDialogSetDetails::setSettings(QCPPlotting::Settings &settings)
{
  if (type == QCPPlotting::FileDialogSetDetails::Open) {
      checkBox_readZeroRow->setChecked(settings.FileDialogSetDetails.readZeroRow);
      checkBox_readZeroColumn->setChecked(settings.FileDialogSetDetails.readZeroColumn);
      checkBox_useZeroRow->setChecked(settings.FileDialogSetDetails.useZeroRow);
    }
  else if (type == QCPPlotting::FileDialogSetDetails::Save) {
      checkBox_writeHHeader->setChecked(settings.FileDialogSetDetails.writeHHeader);
    }
  comboBox_Delimiter->setCurrentIndex(settings.FileDialogSetDetails.delimiter);
}

void FileDialogSetDetails::getSettings(QCPPlotting::Settings &settings)
{
  settings.FileDialogSetDetails << *data;
}

QCPPlotting::FileDialogSetDetails::Data *FileDialogSetDetails::getData()
{
  return data;
}

void FileDialogSetDetails::slot_readZeroRow(int state)
{
  switch (state) {
    case Qt::Checked:
      checkBox_useZeroRow->setEnabled(true);
      break;
    case Qt::Unchecked:
      checkBox_useZeroRow->setEnabled(false);
      break;
    }
}

void FileDialogSetDetails::slot_accepted()
{
  if (!data)
    data = new QCPPlotting::FileDialogSetDetails::Data;

  if (type == QCPPlotting::FileDialogSetDetails::Open) {
      data->readZeroRow = checkBox_readZeroRow->isChecked();
      data->readZeroColumn = checkBox_readZeroColumn->isChecked();
      data->useZeroRow = checkBox_useZeroRow->isChecked();
    }
  else if (type == QCPPlotting::FileDialogSetDetails::Save) {
      data->writeHHeader = checkBox_writeHHeader->isCheckable();
    }
  data->delimiter = comboBox_Delimiter->currentIndex();
}
