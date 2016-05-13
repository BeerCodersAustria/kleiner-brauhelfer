#include <QMessageBox>
#include "erweitertezutatimpl.h"
#include "definitionen.h"
#include "rohstoffaustauschen.h"

//
ErweiterteZutatImpl::ErweiterteZutatImpl( QWidget * parent, Qt::WindowFlags f)
  : QWidget(parent, f)
{
  setupUi(this);
  erg_Menge = 0;
  ausbeute = 0;
  farbe = 0;
  typ = -1;
  animationPos = new QPropertyAnimation(this, "pos");
  dateEdit_zugabezeitpunkt_von->setDate(QDate::currentDate());
}

void ErweiterteZutatImpl::WerteNeuAusRohstoffeHolen()
{
  on_comboBox_Zutat_currentIndexChanged(comboBox_Zutat -> currentText());
}

void ErweiterteZutatImpl::setUIStatus()
{
  QAbstractSpinBox::ButtonSymbols bs;

  if (!statusDisabled) {
    bs = QAbstractSpinBox::UpDownArrows;
  }
  else {
    bs = QAbstractSpinBox::NoButtons;
  }

  //Editfelder Disablen
  comboBox_Zutat -> setDisabled(statusDisabled);
  comboBox_Zutat -> setEditable(statusDisabled);
  comboBox_Zugabezeitpunkt -> setDisabled(statusDisabled);
  comboBox_Zugabezeitpunkt -> setEditable(statusDisabled);
  dsb_Menge -> setReadOnly(statusDisabled);
  dsb_Menge -> setButtonSymbols(bs);
  textEdit_Komentar -> setReadOnly(statusDisabled);
  pushButton_del -> setDisabled(statusDisabled);

  if (zugabestatus == 0)
    dateEdit_zugabezeitpunkt_von->setDisabled(statusDisabledZeitraum);
  else
    dateEdit_zugabezeitpunkt_von->setDisabled(true);

  if (zugabestatus < 2)
    dateEdit_zugabezeitpunkt_bis->setDisabled(statusDisabledZeitraum);
  else
    dateEdit_zugabezeitpunkt_bis->setDisabled(true);

  comboBox_entnahme->setDisabled(statusDisabledZeitraum);
  //je nach zugabestatus Buttons ein oder ausblenden
  if (comboBox_Zugabezeitpunkt->currentIndex() == 0) {
    label_von->setVisible(true);
    comboBox_entnahme->setVisible(true);
    dateEdit_zugabezeitpunkt_von->setVisible(true);
    if (comboBox_entnahme->currentIndex() == 0) {
      dateEdit_zugabezeitpunkt_bis->setVisible(true);
    }
    else {
      dateEdit_zugabezeitpunkt_bis->setVisible(false);
    }

    if (zugabestatus == 0) {
      if (!statusDisabledZeitraum && BierWurdeGebraut)
        buttonZugeben->setVisible(true);
      else
        buttonZugeben->setVisible(false);
      buttonEntnehmen->setVisible(false);
    }
    if (zugabestatus == 1) {
      buttonZugeben->setVisible(false);
      if (comboBox_entnahme->currentIndex() == 0) {
        if (!statusDisabledZeitraum && BierWurdeGebraut)
          buttonEntnehmen->setVisible(true);
        else
          buttonEntnehmen->setVisible(false);
      }
      else {
        buttonEntnehmen->setVisible(false);
      }
    }
    if (zugabestatus == 2) {
      buttonZugeben->setVisible(false);
      buttonEntnehmen->setVisible(false);
      comboBox_entnahme->setDisabled(true);
    }
  }
  else {
    label_von->setVisible(false);
    comboBox_entnahme->setVisible(false);
    dateEdit_zugabezeitpunkt_von->setVisible(false);
    dateEdit_zugabezeitpunkt_bis->setVisible(false);
    buttonZugeben->setVisible(false);
    buttonEntnehmen->setVisible(false);
  }

}

void ErweiterteZutatImpl::setDisabled(bool status, bool statusZeitraum)
{
  statusDisabled = status;
  statusDisabledZeitraum = statusZeitraum;
  setUIStatus();
}

void ErweiterteZutatImpl::setBierWurdeGebraut(bool value)
{
  BierWurdeGebraut = value;
}

void ErweiterteZutatImpl::setZugabezeitpunkt(QDate datum_von, QDate datum_bis)
{
  if (datum_von < QDate::currentDate() && !BierWurdeAbgefuellt && zugabestatus == 0) {
    datum_von = QDate::currentDate();
  }
  if (datum_bis < QDate::currentDate() && !BierWurdeAbgefuellt && zugabestatus < 2) {
    datum_bis = QDate::currentDate();
  }
  dateEdit_zugabezeitpunkt_von->setDate(datum_von);
  dateEdit_zugabezeitpunkt_bis->setDate(datum_bis);
}

QDate ErweiterteZutatImpl::getZugabezeitpunkt_von()
{
  return dateEdit_zugabezeitpunkt_von->date();
}

QDate ErweiterteZutatImpl::getZugabezeitpunkt_bis()
{
  return dateEdit_zugabezeitpunkt_bis->date();
}

void ErweiterteZutatImpl::setEntnahmeindex(int index)
{
  comboBox_entnahme->setCurrentIndex(index);
}

int ErweiterteZutatImpl::getEntnahmeindex()
{
  return comboBox_entnahme->currentIndex();
}

int ErweiterteZutatImpl::getID()
{
  return ID;
}


void ErweiterteZutatImpl::setID(int value)
{
  ID = value;
}



void ErweiterteZutatImpl::setEwListe(QStringList value)
{
  ewListe = value;
}


void ErweiterteZutatImpl::setHopfenListe(QStringList value)
{
  hopfenListe = value;
  ErstelleAuswahlliste();
}


void ErweiterteZutatImpl::ErstelleAuswahlliste()
{
  if (!BierWurdeGebraut){
    //Aktuelle Auswahl speichern
    QString s = comboBox_Zutat -> currentText();
    comboBox_Zutat -> clear();
    comboBox_Zutat -> addItems(ewListe);
    comboBox_Zutat -> addItems(hopfenListe);

    //zwischengespeicherte auswahl wieder anwählen
    for (int i=0; i < comboBox_Zutat -> count(); i++){
      if (comboBox_Zutat -> itemText(i) == s){
        comboBox_Zutat -> setCurrentIndex(i);
      }
    }
  }

}

bool ErweiterteZutatImpl::getBierWurdeAbgefuellt() const
{
  return BierWurdeAbgefuellt;
}

void ErweiterteZutatImpl::setBierWurdeAbgefuellt(bool value)
{
  BierWurdeAbgefuellt = value;
}

int ErweiterteZutatImpl::getZugabestatus() const
{
  return zugabestatus;
}

void ErweiterteZutatImpl::setZugabestatus(int value)
{
  if (zugabestatus != value)
    emit sig_Aenderung();
  zugabestatus = value;
  setUIStatus();
}


void ErweiterteZutatImpl::on_comboBox_Zutat_currentIndexChanged(QString string)
{
  if (string != ""){
    // Überprüfen ob Eintrag in der Hopfenliste ist
    if ((hopfenListe.indexOf(string) < 0)  && !((typ == 100) && (BierWurdeGebraut))){
      if (!BierWurdeGebraut){
        ausbeute = sig_getEwzAusbeute(string);
        farbe = sig_getEwzFarbe(string);
        typ = sig_getEwzTyp(string);
        preis = sig_getEwzPreis(string);
      }
      //Icon setzten
      //Honig
      if (typ == 0){
        pixmapTyp.load(":/ewz/ewz_typ_0.svg");
        label_Icon -> setPixmap(pixmapTyp);
      }
      //Zucker
      else if (typ == 1){
        pixmapTyp.load(":/ewz/ewz_typ_1.svg");
        label_Icon -> setPixmap(pixmapTyp);
      }
      //Gewürze
      else if (typ == 2){
        pixmapTyp.load(":/ewz/ewz_typ_2.svg");
        label_Icon -> setPixmap(pixmapTyp);
      }
      //Früchte
      else if (typ == 3){
        pixmapTyp.load(":/ewz/ewz_typ_3.svg");
        label_Icon -> setPixmap(pixmapTyp);
      }
      //Sonstiges
      else if (typ == 4){
        pixmapTyp.load(":/ewz/ewz_typ_4.svg");
        label_Icon -> setPixmap(pixmapTyp);
      }

      //Einheit festlegen
      if (!BierWurdeGebraut){
        einheit = sig_getEwzEinheit(string);
      }
      //Kilogramm
      if (einheit == 0){
        label_Mengeneinheit -> setText("g/L");
        ergWidget -> label_Einheit -> setText("Kg");
        ergWidget -> spinBox_Wert -> setDecimals(3);
      }
      //Gramm
      else if (einheit == 1){
        label_Mengeneinheit -> setText("g/L");
        ergWidget -> label_Einheit -> setText("g");
        ergWidget -> spinBox_Wert -> setDecimals(0);
      }
    }
    //Typ ist Hopfen
    else {
      typ = 100;
      pixmapTyp.load(":/ewz/ewz_typ_100.svg");
      label_Icon -> setPixmap(pixmapTyp);
      label_Mengeneinheit -> setText("g/L");
      ergWidget -> label_Einheit -> setText("g");
      ergWidget -> spinBox_Wert -> setDecimals(1);
      //Einheit auf gramm festlegen
      einheit = 1;
      if (!BierWurdeGebraut){
        preis = sig_getEwzPreisHopfen(string);
      }
    }

    //Auswahlliste für Zugabezeitpunkt füllen
    ZugabezeitpunktListe.clear();
    ZugabezeitpunktListe.append(trUtf8("bei der Gärung"));
    // Alles ausser Hopfen
    if (typ == 0 || typ == 1){
      ZugabezeitpunktListe.append(trUtf8("bei Kochbegin"));
      ZugabezeitpunktListe.append(trUtf8("beim Maischen"));
    }
    else if (typ < 100){
      ZugabezeitpunktListe.append(trUtf8("beim Kochen"));
      ZugabezeitpunktListe.append(trUtf8("beim Maischen"));
    }
    //aktuellen zugabezeitpunt merken
    QString s = comboBox_Zugabezeitpunkt -> currentText();
    comboBox_Zugabezeitpunkt -> clear();
    comboBox_Zugabezeitpunkt -> addItems(ZugabezeitpunktListe);
    //zwischengespeicherte auswahl wieder anwählen
    for (int i=0; i < comboBox_Zugabezeitpunkt -> count(); i++){
      if (comboBox_Zugabezeitpunkt -> itemText(i) == s){
        comboBox_Zugabezeitpunkt -> setCurrentIndex(i);
      }
    }

    ergWidget -> label_Beschreibung -> setText(string);
    emit sig_Aenderung();
  }
}

QString ErweiterteZutatImpl::getName()
{
  return comboBox_Zutat -> currentText();
}


double ErweiterteZutatImpl::getMenge()
{
  return dsb_Menge -> value();
}


int ErweiterteZutatImpl::getEinheit()
{
  return einheit;
}

void ErweiterteZutatImpl::setEinheit(int value)
{
  einheit = value;
}


int ErweiterteZutatImpl::getTyp()
{
  return typ;
}

void ErweiterteZutatImpl::setTyp(int value)
{
  typ = value;
}


int ErweiterteZutatImpl::getZeitpunkt()
{
  return comboBox_Zugabezeitpunkt -> currentIndex();
}


QString ErweiterteZutatImpl::getBemerkung()
{
  return textEdit_Komentar -> toPlainText();
}


double ErweiterteZutatImpl::getErg_Menge()
{
  return erg_Menge;
}


void ErweiterteZutatImpl::setErg_Menge(double value)
{
  //Menge in gramm
  erg_Menge = value;
  //Kilogramm
  if (einheit == 0){
    value = value / 1000;
  }
  //Gramm
  else if (einheit == 1){
  }
  ergWidget -> spinBox_Wert -> setValue(value);
}


void ErweiterteZutatImpl::setName(QString Name)
{
  if (!BierWurdeGebraut){
    bool gefunden = false;
    for (int i=0; i < comboBox_Zutat -> count(); i++){
      if (comboBox_Zutat -> itemText(i) == Name){
        comboBox_Zutat -> setCurrentIndex(i);
        ergWidget -> label_Beschreibung -> setText(Name);
        gefunden = true;
      }
    }
    if (!gefunden){
      //Dialog mit Rohstoffauswahl zum Austauschen des zu löschenden Rohstoffes anzeigen
      RohstoffAustauschen raDia;
      raDia.setButtonCancelVisible(false);

      //Text für Dialog erstellen
      QString text = trUtf8("Rohstoffeintrag in den Weiteren Zutaten ist nicht mehr vorhanden") + " <br><b>" + Name + "</b> ";
      text += "<br>Bitte einen Ersatz auswählen?";
      raDia.SetText(text);
      //Auswahl für Ersetzung füllen
      for (int i=0; i < comboBox_Zutat->count(); i++){
        if (Name != comboBox_Zutat -> itemText(i)){
          raDia.addAuswahlEintrag(comboBox_Zutat -> itemText(i) );
        }
      }
      //raDia.setAktAuswahl(letzeAuswahl);
      raDia.exec();
      //Austauschen
      QString auswahl = raDia.GetAktAuswahl();
      for (int i=0; i < comboBox_Zutat -> count(); i++){
        if (comboBox_Zutat -> itemText(i) == auswahl){
          comboBox_Zutat -> setCurrentIndex(i);
          ergWidget -> label_Beschreibung -> setText(auswahl);
        }
      }
    }
  }
  else {
    comboBox_Zutat -> addItem(Name);
    comboBox_Zutat -> setCurrentIndex(0);
    ergWidget -> label_Beschreibung -> setText(Name);
  }
}


void ErweiterteZutatImpl::setMenge(double Menge)
{
  dsb_Menge -> setValue(Menge);
}


void ErweiterteZutatImpl::setZeitpunkt(int Zeitpunkt)
{
  comboBox_Zugabezeitpunkt -> setCurrentIndex(Zeitpunkt);
}


void ErweiterteZutatImpl::setBemerkung(QString Bemerkung)
{
  textEdit_Komentar -> setPlainText(Bemerkung);
}


void ErweiterteZutatImpl::on_comboBox_Zugabezeitpunkt_currentIndexChanged(int )
{
  setUIStatus();
  emit sig_Aenderung();
}

void ErweiterteZutatImpl::on_dsb_Menge_valueChanged(double )
{
  emit sig_Aenderung();
}

void ErweiterteZutatImpl::on_textEdit_Komentar_textChanged()
{
  emit sig_Aenderung();
}

void ErweiterteZutatImpl::closeEvent(QCloseEvent *)
{
  emit sig_Aenderung();
  ergWidget -> close();
}

void ErweiterteZutatImpl::on_pushButton_del_clicked()
{
  if (faderWidget)
    faderWidget->close();

  faderWidget = new FaderWidget(this);
  connect(faderWidget, SIGNAL(sig_fertig()), this, SLOT(on_fadeout_fertig()));
  animationAktiv = true;
  faderWidget->start();
}

void ErweiterteZutatImpl::on_fadeout_fertig()
{
  emit sig_vorClose(ID);
  close();
}

int ErweiterteZutatImpl::getAusbeute()
{
  return ausbeute;
}

void ErweiterteZutatImpl::setAusbeute(int value)
{
  ausbeute = value;
}

double ErweiterteZutatImpl::getFarbe()
{
  return farbe;
}

void ErweiterteZutatImpl::setFarbe(double value)
{
  farbe = value;
}


double ErweiterteZutatImpl::getErg_Kosten()
{
  //Menge in Gramm Preis ist in Kg
  return erg_Menge * preis/1000;
}


void ErweiterteZutatImpl::on_dateEdit_zugabezeitpunkt_von_dateChanged(const QDate &date)
{
  if (dateEdit_zugabezeitpunkt_bis->date() < date) {
    dateEdit_zugabezeitpunkt_bis->setDate(date);
  }
  emit sig_Aenderung();
}

void ErweiterteZutatImpl::on_dateEdit_zugabezeitpunkt_bis_dateChanged(const QDate &date)
{
  if (date < dateEdit_zugabezeitpunkt_von->date()) {
    dateEdit_zugabezeitpunkt_bis->setDate(dateEdit_zugabezeitpunkt_von->date());
  }
  emit sig_Aenderung();
}

void ErweiterteZutatImpl::on_comboBox_entnahme_currentIndexChanged(int index)
{
  setUIStatus();
  emit sig_Aenderung();
}

void ErweiterteZutatImpl::on_buttonZugeben_clicked()
{
  zugabestatus = 1;
  setUIStatus();
  emit sig_Aenderung();
}

void ErweiterteZutatImpl::on_buttonEntnehmen_clicked()
{
  zugabestatus = 2;
  setUIStatus();
  emit sig_Aenderung();
}
