#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>

#include "structs.h"

class Dialogs;
class FileDialogSetDetails;

class Dialogs : public QDialog
{
  Q_OBJECT
public:
  Dialogs(QWidget *parent = nullptr);
  ~Dialogs();

public:
  QGridLayout *gridLayout = nullptr;

private:
  QDialogButtonBox *dialogButtonBox = nullptr;
  QVBoxLayout *vBoxLayout = nullptr;
};


class FileDialogSetDetails : public Dialogs
{
  Q_OBJECT
public:
  FileDialogSetDetails(const QCPPlotting::FileDialogSetDetails::Type t, QWidget *parent = nullptr);
  ~FileDialogSetDetails();

  void setSettings(QCPPlotting::Settings &settings);
  void getSettings(QCPPlotting::Settings &settings);
  QCPPlotting::FileDialogSetDetails::Data *getData();

private slots:
  void slot_readZeroRow(int state);
  void slot_accepted();

private:
  const QCPPlotting::FileDialogSetDetails::Type type;
  QCPPlotting::FileDialogSetDetails::Data *data = nullptr;
  QCheckBox *checkBox_readZeroRow  = nullptr;
  QCheckBox *checkBox_readZeroColumn  = nullptr;
  QCheckBox *checkBox_useZeroRow  = nullptr;
  QCheckBox *checkBox_writeHHeader  = nullptr;
  QLabel *label_Delimiter = nullptr;
  QComboBox *comboBox_Delimiter = nullptr;
  QHBoxLayout *hBoxLayout_Delimiter = nullptr;
};

#endif // DIALOGS_H
