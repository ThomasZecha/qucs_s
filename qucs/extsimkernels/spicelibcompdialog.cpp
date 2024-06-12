#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "misc.h"
#include "main.h"
#include "component.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>

#include "symbolwidget.h"
#include "spicelibcompdialog.h"


SpiceLibCompDialog::SpiceLibCompDialog(Component *pc, QWidget* parent) : QDialog{parent}
{
  comp = pc;
  symbolPinsCount = 0;

  QString file = comp->Props.at(0)->Value;
  if (!file.isEmpty()) {
    file = misc::properAbsFileName(file);
  }
  QString device = comp->Props.at(1)->Value;
  QString sym = comp->Props.at(2)->Value;

  QLabel *lblLibfile = new QLabel("SPICE library:");
  edtLibPath = new QLineEdit;
  edtLibPath->setText(file);
  connect(edtLibPath,SIGNAL(textChanged(QString)),this,SLOT(slotFillSubcirComboBox()));
  btnOpenLib = new QPushButton(tr("Open"));
  connect(btnOpenLib,SIGNAL(clicked(bool)),this,SLOT(slotBtnOpenLib()));

  QLabel *lblDevice = new QLabel("Subcircuit:");
  cbxSelectSubcir = new QComboBox;
  connect(cbxSelectSubcir,SIGNAL(currentIndexChanged(int)),this,SLOT(slotFillPinsTable()));

  QLabel *lblPattern = new QLabel("Symbol pattern");
  cbxSymPattern = new QComboBox;
  QStringList lst_patterns;
  lst_patterns.append("auto");
  misc::getSymbolPatternsList(lst_patterns);
  cbxSymPattern->addItems(lst_patterns);
  connect(cbxSymPattern,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetSymbol()));

  symbol = new SymbolWidget;
  symbol->disableDragNDrop();
  symbol->enableShowPinNumbers();

  tbwPinsTable = new QTableWidget;
  tbwPinsTable->setColumnCount(2);
  tbwPinsTable->setRowCount(100);
  QStringList lbl_cols;
  lbl_cols<<"Subcircuit pin"<<"Symbol pin";
  tbwPinsTable->setHorizontalHeaderLabels(lbl_cols);
  connect(tbwPinsTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slotTableCellDoubleClick()));

  btnOK = new QPushButton(tr("OK"));
  connect(btnOK,SIGNAL(clicked(bool)),this,SLOT(slotBtnOK()));
  btnApply = new QPushButton(tr("Apply"));
  connect(btnApply,SIGNAL(clicked(bool)),this,SLOT(slotBtnApply()));
  btnCancel = new QPushButton(tr("Cancel"));
  connect(btnCancel,SIGNAL(clicked(bool)),this,SLOT(slotBtnCancel()));

  QVBoxLayout *top = new QVBoxLayout;
  QHBoxLayout *l2 = new QHBoxLayout;
  l2->addWidget(lblLibfile);
  l2->addWidget(edtLibPath,4);
  l2->addWidget(btnOpenLib,1);
  top->addLayout(l2);
  QHBoxLayout *l5 = new QHBoxLayout;
  l5->addWidget(lblDevice);
  l5->addWidget(cbxSelectSubcir);
  top->addLayout(l5);
  QHBoxLayout *l6 = new QHBoxLayout;
  l6->addWidget(lblPattern);
  l6->addWidget(cbxSymPattern);
  top->addLayout(l6);
  QHBoxLayout *l3 = new QHBoxLayout;
  l3->addWidget(tbwPinsTable);
  l3->addWidget(symbol);
  top->addLayout(l3,3);
  QHBoxLayout *l4 = new QHBoxLayout;
  l4->addWidget(btnOK);
  l4->addWidget(btnApply);
  l4->addWidget(btnCancel);
  l4->addStretch();
  top->addLayout(l4);

  this->slotSetSymbol();
  this->setLayout(top);

}

void SpiceLibCompDialog::slotFillSubcirComboBox()
{
  if (!parseLibFile(edtLibPath->text())) {
    QMessageBox::critical(this,tr("Error"),tr("SPICE library parse error"));
    return;
  }

  cbxSelectSubcir->blockSignals(true);
  cbxSelectSubcir->clear();
  for(const auto &key: subcirPins.keys()) {
    cbxSelectSubcir->addItem(key);
  }
  cbxSelectSubcir->blockSignals(false);
  slotFillPinsTable();
}

void SpiceLibCompDialog::slotFillPinsTable()
{
  QString subcir_name = cbxSelectSubcir->currentText();
  if (subcirPins.find(subcir_name) == subcirPins.end()) return;
  QStringList pins = subcirPins[subcir_name];
  tbwPinsTable->clearContents();
  tbwPinsTable->setRowCount(pins.count());
  for (int i = 0; i < pins.count(); i++) {
    QTableWidgetItem *itm1 = new QTableWidgetItem(pins.at(i));
    tbwPinsTable->setItem(i,0,itm1);
    QTableWidgetItem *itm2 = new QTableWidgetItem("NC");
    tbwPinsTable->setItem(i,1,itm2);
  }
}

bool SpiceLibCompDialog::parseLibFile(const QString &filename)
{
  if (!QFileInfo::exists(filename)) return false;
  QFile f(filename);
  if (!f.open(QIODevice::ReadOnly)) {
    return false;
  }

  subcirPins.clear();
  QTextStream ts(&f);

  while (!ts.atEnd()) {
    QString line = ts.readLine();
    line = line.trimmed();
    line = line.toUpper();
    if (line.startsWith(".SUBCKT")) {
      QStringList pin_names;
      QString subname;
      QStringList tokens = line.split(QRegularExpression("[ \\t]"),Qt::SkipEmptyParts);
      if (tokens.count() > 3) {
        subname = tokens.at(1);
      } else continue;
      tokens.removeFirst();
      tokens.removeFirst();
      for (const auto &s1: tokens) {
        if (!s1.contains('=') && (s1 != "PARAMS:")) {
          pin_names.append(s1);
        }
      }
      subcirPins[subname] = pin_names;
    }
  }

  f.close();
  if (subcirPins.isEmpty()) {
    return false;
  }
  return true;

}

void SpiceLibCompDialog::slotSetSymbol()
{
  if (cbxSymPattern->currentText() == "auto") {
    tbwPinsTable->setEnabled(false);
    QString s1 = "";
    QString s2 = "SpLib";
    symbol->setSymbol(s1, s1, s2);
    symbolPinsCount = 0;
  } else {
    tbwPinsTable->setEnabled(true);
    QString dir_name = QucsSettings.BinDir + "/../share/" QUCS_NAME "/symbols/";
    QString file = dir_name + cbxSymPattern->currentText() + ".sym";
    symbol->loadSymFile(file);
    symbolPinsCount = symbol->getPortsNumber();
  }
  for (int i = 0; i < tbwPinsTable->rowCount(); i++) {
    QTableWidgetItem *itm = new QTableWidgetItem("NC");
    tbwPinsTable->setItem(i,1,itm);
  }
}

void SpiceLibCompDialog::slotTableCellDoubleClick()
{
  int r = tbwPinsTable->currentRow();
  int c = tbwPinsTable->currentColumn();
  if (c == 0) return; // do not edit the forst column
  QComboBox *cbxSelectPin = new QComboBox;
  cbxSelectPin->addItem("NC");
  for (int i = 1; i <= symbolPinsCount; i++) {
    bool pinAssigned = false;
    for(int j = 0; j < tbwPinsTable->rowCount(); j++) {
      if (j == r) continue;
      auto itm = tbwPinsTable->item(j,1);
      if (itm == nullptr) continue;
      QString s = itm->text();
      if (s.isEmpty()) continue;
      int pin_num = s.toInt();
      if (pin_num == i) {
        pinAssigned = true;
        break;
      }
    }
    if (!pinAssigned) {
      cbxSelectPin->addItem(QString::number(i));
    }
  }
  tbwPinsTable->setCellWidget(r,c,cbxSelectPin);
  connect(cbxSelectPin,SIGNAL(activated(int)),this,SLOT(slotSelectPin()));
}

void SpiceLibCompDialog::slotSelectPin()
{
  QComboBox *cbxSelectPin = qobject_cast<QComboBox*>(sender());
  QString pin = cbxSelectPin->currentText();
  int r = tbwPinsTable->currentRow();
  int c = tbwPinsTable->currentColumn();
  QTableWidgetItem *itm = new QTableWidgetItem(pin);
  tbwPinsTable->removeCellWidget(r,c);
  tbwPinsTable->setItem(r,c,itm);
}

void SpiceLibCompDialog::slotBtnOpenLib()
{
  QString s = QFileDialog::getOpenFileName(this, tr("Open SPICE library"),
                                           QDir::homePath(),
                                           tr("SPICE files (*.cir +.ckt *.sp *.lib)"));
  edtLibPath->setText(s);
}

void SpiceLibCompDialog::slotBtnApply()
{

}

void SpiceLibCompDialog::slotBtnOK()
{
  slotBtnApply();
  accept();
}

void SpiceLibCompDialog::slotBtnCancel()
{
  reject();
}
