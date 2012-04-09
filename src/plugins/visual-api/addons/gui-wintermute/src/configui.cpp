/**
 * @file    ConfigurationDialog.cpp
 * @author  jacky
 * @created 9/24/2011
 *
 * Copyright (C) %YEAR by %USER%
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

// Qt includes
#include <QDBusMessage>
#include <QDBusConnection>
#include <QDir>
#include <QLCDNumber>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QCheckBox>
#include <QProgressDialog>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>

// Wintermute includes
#include <plugins.hpp>

// WntrData includes
#include <ontology/ontology.hpp>
#include <lexical/lexical.hpp>
#include <data/linguistics.hpp>

// WntrLing includes

// local includes
#include "configui.hpp"
#include "ui_configui.h"

using namespace Wintermute::Data::Linguistics;

namespace Wintermute
{
namespace Visual
{
namespace Graphics
{
ConfigurationDialog::ConfigurationDialog (QWidget* parent) : QDialog (parent), ui (new Ui::ConfigurationDialog)
{
    ui->setupUi (this);
    on_tabWidget_currentChanged (0);
    on_tabWidget_currentChanged (1);
    on_tabWidget_currentChanged (2);
    ui->tableWidgetLinks->setVerticalHeaderLabels (QString ("Semantic ID,Flag").split (","));

    QMenu* aMenu = new QMenu (ui->pushButtonSelect);
    aMenu->addAction (QIcon::fromTheme ("edit-select-all"), "&All", ui->treeWidgetDelete, SLOT (selectAll()));
    aMenu->addAction ("None", ui->treeWidgetDelete, SLOT (clearSelection()));
    ui->pushButtonSelect->setMenu (aMenu);

    QMenu* aMenu2 = new QMenu (ui->pushButtonDelete);
    aMenu2->addAction (QIcon::fromTheme ("edit-select-all"), "&All", this, SLOT (on_pushButtonDelete_all()));
    aMenu2->addAction (QIcon::fromTheme ("edit-select"), "&Selected", this, SLOT (on_pushButtonDelete_selected()));
    ui->pushButtonDelete->setMenu (aMenu2);
}

void ConfigurationDialog::on_pushButtonDelete_all()
{

}

void ConfigurationDialog::on_pushButtonDelete_selected()
{

}

void ConfigurationDialog::on_btnRefresh_clicked()
{
    on_tabWidget_currentChanged (0);
}

void ConfigurationDialog::on_pushButtonUpdate_clicked()
{
    if (QMessageBox::question (this, "Updating Lexical Information", "Are you sure you want to save these changes?<br>This can <b>not</b> be undone.", QMessageBox::Ok | QMessageBox::Close, QMessageBox::Close)) {
        const QString sym = ui->symbolLineEdit->text ().toLower ();
        const QString lcl = ui->localeComboBox->currentText ();
        Data::Linguistics::Lexical::Data dt (Data::Linguistics::Lexical::Data::idFromString (sym), lcl, sym);
        QVariantMap flgs;

        for (int i = 0; i < ui->tableWidgetLinks->rowCount(); i++) {
            const QString id = ui->tableWidgetLinks->item (i, 0)->text();
            const QString flg = ui->tableWidgetLinks->item (i, 1)->text();
            flgs.insert (id, flg);
        }

        dt.setFlags (flgs);
        Data::Linguistics::Lexical::Cache::write (dt);
    }
}

void ConfigurationDialog::on_pushButtonAdd_clicked()
{
    ui->tableWidgetLinks->setRowCount (ui->tableWidgetLinks->rowCount() + 1);
    ui->tableWidgetLinks->setItem (ui->tableWidgetLinks->rowCount(), 0, (new QTableWidgetItem ("-1")));
    ui->tableWidgetLinks->setItem (ui->tableWidgetLinks->rowCount(), 1, (new QTableWidgetItem ("---")));
}

void ConfigurationDialog::on_tabWidget_currentChanged (int index)
{
    QApplication::setOverrideCursor (Qt::WaitCursor);

    switch (index) {
    case 0: {
        ui->lcdWord->display (Data::Linguistics::Lexical::Cache::countSymbols());
        //ui->lcdOnto->display(Data::Ontology::Resource::countConcepts());
        ui->lcdConc->display (Data::Ontology::Repository::countOntologies());
        ui->lcdLink->display (Data::Linguistics::Lexical::Cache::countFlags());
    }
    break;

    case 1: {
        ui->tabWidget_2->setCurrentIndex (0);
    }
    break;

    case 2: {
    } break;

    case 3: {
        ui->listWidgetPlugins->clear();
        QDBusMessage callAll = QDBusMessage::createMethodCall ("org.thesii.Wintermute", "/Factory", "org.thesii.Wintermute.Factory", "allPlugins");
        QDBusMessage callLoaded = QDBusMessage::createMethodCall ("org.thesii.Wintermute", "/Factory", "org.thesii.Wintermute.Factory", "loadedPlugins");
        QDBusMessage replyAll = QDBusConnection::sessionBus ().call (callAll, QDBus::BlockWithGui);
        QDBusMessage replyLoaded = QDBusConnection::sessionBus ().call (callLoaded, QDBus::BlockWithGui);
        QStringList all, loaded;
        all = replyAll.arguments().at (0).toStringList();
        loaded = replyLoaded.arguments().at (0).toStringList();

        foreach (const QString & uuid, all) {
            QListWidgetItem* item = new QListWidgetItem();
            item->setText (Wintermute::Plugins::Factory::attribute (uuid, "Description/Name").toString());
            item->setData (0, uuid);
            ui->listWidgetPlugins->addItem (item);
        }

        ui->listWidgetPlugins->sortItems();
        ui->listWidgetPlugins->setCurrentRow (0);
    }
    }

    QApplication::setOverrideCursor (Qt::ArrowCursor);
}

void ConfigurationDialog::on_tabWidget_2_currentChanged (int index)
{
    QStringList comboBoxLingLocaleItems;
    const QStringList allLocales = Data::Linguistics::System::locales();
    comboBoxLingLocaleItems << "* all *" << allLocales;

    switch (index) {
    case 0: {
        on_symbolLineEdit_editingFinished ();
        ui->localeComboBox->clear ();
        ui->localeComboBox->addItems (allLocales);
    }
    break;

    case 1: {
        ui->comboBoxLingDeleteLocale->clear ();
        ui->comboBoxLingDeleteLocale->addItems (comboBoxLingLocaleItems);
    }
    break;

    case 2: {
        ui->cmbBxLingSaveLocales->clear ();
        ui->cmbBxLingSaveLocales->addItems (allLocales);
    }
    break;
    }

}

void ConfigurationDialog::loadLocale (const QString& p_lcl)
{
    const QStringList nodes = Lexical::Cache::allNodes (p_lcl);
    QProgressDialog prg ("Loading nodes...", "Cancel", 0, nodes.length (), this);
    prg.setWindowModality (Qt::WindowModal);
    prg.setValue (0);

    ui->treeWidgetDelete->setColumnCount (2);
    foreach (const QString ndID, nodes) {
        prg.setValue (prg.value () + 1);
        Lexical::Data dt (ndID, p_lcl);
        Lexical::Cache::read (dt);
        QTreeWidgetItem* item = new QTreeWidgetItem (QString (dt.symbol ()).split (","));

        for (QVariantMap::ConstIterator itr = dt.flags ().begin (); itr != dt.flags ().end (); itr++)
            item->addChild ( (new QTreeWidgetItem (QString (itr.key() + "," + itr.value().toString()).split (","))));

        ui->treeWidgetDelete->addTopLevelItem (item);
    }
}

void ConfigurationDialog::on_comboBoxLingDeleteLocale_activated (const QString& p_lcl)
{
    ui->treeWidgetDelete->clear ();

    if (p_lcl == "* all *") {
        foreach (const QString lcl, Data::Linguistics::System::locales())
        loadLocale (lcl);
    }
    else {
        loadLocale (p_lcl);
    }
}

void ConfigurationDialog::on_symbolLineEdit_editingFinished()
{
    const QString sym = ui->symbolLineEdit->text ().toLower ();
    const QString lcl = ui->localeComboBox->currentText ();
    Data::Linguistics::Lexical::Data dt (Data::Linguistics::Lexical::Data::idFromString (sym), lcl, sym);

    if (Data::Linguistics::Lexical::Cache::exists (dt)) {
        Data::Linguistics::Lexical::Cache::read (dt);
        qDebug() << dt;
        int i = 0;
        ui->tableWidgetLinks->setRowCount (dt.flags ().size ());
        ui->tableWidgetLinks->setColumnCount (2);

        for (QVariantMap::ConstIterator itr = dt.flags ().begin (); itr != dt.flags ().end (); itr ++, i++) {
            ui->tableWidgetLinks->setItem (i, 0, (new QTableWidgetItem (itr.key ())));
            ui->tableWidgetLinks->setItem (i, 1, (new QTableWidgetItem (itr.value ().toString())));
        }
    }
    else {
        ui->tableWidgetLinks->setRowCount (0);
        ui->tableWidgetLinks->setColumnCount (0);
    }
}

/// @todo Allow merging of lexical data.
/// @todo Make confirmation dialog more verbose.
void ConfigurationDialog::on_btnLingSerialDo_clicked()
{
    const QString fileStr = ui->uriLineEdit->text();
    const QString lcl = ui->cmbBxLingSaveLocales->currentText ();
    const QStringList ndLst = Lexical::Cache::allNodes (lcl);
    QFile file (fileStr);
    QDomDocument* dom = new QDomDocument ("Archive");

    if (ui->radioButtonSaving->isChecked ()) {
        if (!file.open (QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            QMessageBox::information (this, "Failed to Create Archive", "Failed to open" + file.fileName () + "for writing.", QMessageBox::Ok);
            return;
        }

        QTextStream outFile (&file);
        QDomElement rootElem = dom->createElement ("Archive");
        rootElem.setAttribute ("locale", lcl);
        dom->appendChild (rootElem);

        QProgressDialog prg ("Saving lexical information..", "Cancel", 0, ndLst.length (), this);
        prg.setValue (1);
        foreach (const QString nd, ndLst) {
            QDomElement newEle = dom->createElement ("Data");
            Lexical::Data dt (nd, lcl);
            Lexical::Cache::read (dt);
            prg.setLabelText ("Saving symbol '" + dt.symbol () + "'...");
            qDebug() << "(gui) [ConfigUi] Saving" << dt.symbol() << "...";

            Lexical::DomSaveModel svMdl (&newEle);
            svMdl.saveFrom (dt);
            rootElem.appendChild (newEle);
            prg.setValue (prg.value () + 1);
        }

        dom->save (outFile, 4);
    }
    else if (ui->radioButtonLoading->isChecked ()) {
        if (!file.exists ()) {
            QMessageBox::information (this, "Archive not Found", "Failed to find" + file.fileName () + "for reading.", QMessageBox::Ok);
            return;
        }

        if (!dom->setContent (&file)) {
            QMessageBox::information (this, "Unable to Save Archive", "Failed to parse" + file.fileName() + "for transversing.", QMessageBox::Ok);
        }

        QDomNodeList elemLst = dom->documentElement ().elementsByTagName ("Data");
        QProgressDialog prg ("Loading lexical information...", "Cancel", 0, elemLst.length (), this);

        for (int i = 0; i < elemLst.length (); i++) {
            QDomElement elem = elemLst.at (i).toElement ();
            Lexical::Data dt;
            Lexical::DomLoadModel ldMdl (&elem);
            ldMdl.loadTo (dt);
            qDebug() << "(gui) [ConfigUi] Loading" << dt.symbol() << "...";
            prg.setLabelText ("Loading symbol '" + dt.symbol () + "'...");

            if (Lexical::Cache::exists (dt)) {
                QMessageBox ques (QMessageBox::Question, "Node Exists", ("Do you want to overwrite, merge or skip the external data for locally found symbol '" + dt.symbol () + "'?"));
                ques.setInformativeText ("The symbol '" + dt.symbol () + "' already has information stored locally.");
                ques.addButton ("Overwrite", QMessageBox::DestructiveRole);
                ques.addButton ("Merge", QMessageBox::ActionRole);
                ques.addButton ("Skip", QMessageBox::RejectRole);

                if (ques.exec ())
                    Lexical::Cache::write (dt);
            }

            prg.setValue (i + 1);
        }
    }
}

void ConfigurationDialog::on_btnLingSaveURI_clicked()
{
    QFileDialog dlg (this, "Location of archive");
    dlg.setFileMode (QFileDialog::AnyFile);
    dlg.setDefaultSuffix (".xml");

    if (dlg.exec ())
        ui->uriLineEdit->setText (dlg.selectedFiles ().at (0));
    else
        ui->uriLineEdit->clear ();
}

void ConfigurationDialog::on_checkBoxEnabled_clicked()
{
    const QString& uuid = ui->listWidgetPlugins->item (ui->listWidgetPlugins->currentRow())->data (0).toString();
    const bool val = (ui->checkBoxEnabled->checkState() == Qt::Checked);
    Plugins::Factory::setAttribute (uuid, "Version/Enabled", val);
    qDebug() << Plugins::Factory::attribute (uuid, "Version/Enabled") << val;
}

void ConfigurationDialog::on_checkBoxAutoStart_clicked()
{
    const QString& uuid = ui->listWidgetPlugins->item (ui->listWidgetPlugins->currentRow())->data (0).toString();
    QSettings* settings = new QSettings ("Synthetic Intellect Institute", "Wintermute");
    QStringList plgnLst = settings->value ("Plugins/AutoStart").toStringList();

    if (ui->checkBoxAutoStart->checkState() == Qt::Checked)
        plgnLst << uuid;
    else
        plgnLst.removeAll (uuid);

    qDebug() << "(gui) [ConfigurationDialog] Removed" << plgnLst.removeDuplicates() << "duplicates.";
    settings->setValue ("Plugins/AutoStart", plgnLst);
    qDebug() << plgnLst;
}


void ConfigurationDialog::on_listWidgetPlugins_itemSelectionChanged()
{
    QSettings* settings = new QSettings ("Synthetic Intellect Institute", "Wintermute");
    const QStringList plgnLst = settings->value ("Plugins/AutoStart").toStringList();

    const QString& uuid = ui->listWidgetPlugins->item (ui->listWidgetPlugins->currentRow())->data (0).toString();
    ui->lblVersion->setText (Plugins::Factory::attribute (uuid, "Version/Plugin").toString());
    ui->lblName->setText (Plugins::Factory::attribute (uuid, "Description/Name").toString());
    ui->lblVendor->setText (Plugins::Factory::attribute (uuid, "Description/Vendor").toString());
    ui->lblAuthor->setText (Plugins::Factory::attribute (uuid, "Description/Author").toString());
    ui->lblWebPage->setText (Plugins::Factory::attribute (uuid, "Description/WebPage").toString());
    ui->txtDescription->setText (Plugins::Factory::attribute (uuid, "Description/Blurb").toString());

    if (Plugins::Factory::attribute (uuid, "Plugin/Type") == "Backend")
        ui->checkBoxAutoStart->setEnabled (false);
    else
        ui->checkBoxAutoStart->setEnabled (true);

    ui->checkBoxAutoStart->setChecked (plgnLst.contains (uuid));
    ui->checkBoxEnabled->setChecked (Plugins::Factory::attribute (uuid, "Plugin/Enabled").toBool());
}

void ConfigurationDialog::on_pushButtonPlgnActn_clicked()
{
    const QString& str = ui->comboBoxAction->currentText();
    const QString& uuid = ui->listWidgetPlugins->item (ui->listWidgetPlugins->currentRow())->data (0).toString();

    if (str == "Start") {
        QDBusMessage callStart = QDBusMessage::createMethodCall ("org.thesii.Wintermute", "/Factory", "org.thesii.Wintermute.Factory", "loadPlugin");
        callStart << uuid;
        QDBusConnection::sessionBus ().call (callStart, QDBus::BlockWithGui);
    }
    else if (str == "Stop") {
        QDBusMessage callStop = QDBusMessage::createMethodCall ("org.thesii.Wintermute", "/Factory", "org.thesii.Wintermute.Factory", "unloadPlugin");
        callStop << uuid;
        QDBusConnection::sessionBus ().call (callStop, QDBus::BlockWithGui);
    }
    else if (str == "Restart") {
        QDBusMessage callStart = QDBusMessage::createMethodCall ("org.thesii.Wintermute", "/Factory", "org.thesii.Wintermute.Factory", "loadPlugin");
        QDBusMessage callStop = QDBusMessage::createMethodCall ("org.thesii.Wintermute", "/Factory", "org.thesii.Wintermute.Factory", "unloadPlugin");
        callStart << uuid;
        callStop << uuid;
        QDBusConnection::sessionBus ().call (callStop, QDBus::BlockWithGui);
        QDBusConnection::sessionBus ().call (callStart, QDBus::BlockWithGui);
    }

    on_tabWidget_currentChanged (3);
}

ConfigurationDialog::~ConfigurationDialog()
{
    delete ui;
}

void ConfigurationDialog::changeEvent (QEvent* e)
{
    QDialog::changeEvent (e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi (this);
        break;
    default:
        break;
    }
}
}
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
