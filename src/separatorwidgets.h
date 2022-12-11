#ifndef SEPARATORWIDGETS_H
#define SEPARATORWIDGETS_H

#include <QSpacerItem>
#include <QFrame>

class Spacer : public QSpacerItem
{
public:
  Spacer(QSizePolicy::Policy hPolicy = QSizePolicy::Expanding, QSizePolicy::Policy vPolicy = QSizePolicy::Expanding);
};

class VSpacer : public Spacer
{
public:
  VSpacer();
};

class HSpacer : public Spacer
{
public:
  HSpacer();
};

class VLine : public QFrame
{
  Q_OBJECT
public:
  VLine(int margin = 10, QWidget* parent = nullptr);
};

class HLine : public QFrame
{
  Q_OBJECT
public:
  HLine(int margin = 10, QWidget* parent = nullptr);
};

#endif // SEPARATORWIDGETS_H
