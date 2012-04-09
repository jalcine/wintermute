/**
 * @file    configui.hpp
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

#ifndef CONFIGUI_HPP
#define CONFIGUI_HPP

#include <QtGui/QDialog>

namespace Ui
{
class ConfigurationDialog;
}

namespace Wintermute
{
namespace Visual
{
namespace Graphics
{
class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog (QWidget* parent = 0);
    ~ConfigurationDialog();

protected:
    void changeEvent (QEvent* e);

private slots:
    void on_tabWidget_currentChanged (int index);
    void on_btnRefresh_clicked();
    void on_symbolLineEdit_editingFinished();
    void on_pushButtonAdd_clicked();
    void on_pushButtonUpdate_clicked();
    void on_tabWidget_2_currentChanged (int index);
    void on_comboBoxLingDeleteLocale_activated (const QString&);
    void on_pushButtonDelete_all();
    void on_pushButtonDelete_selected();
    void on_btnLingSaveURI_clicked();
    void on_btnLingSerialDo_clicked();
    void on_listWidgetPlugins_itemSelectionChanged();
    void on_checkBoxEnabled_clicked();
    void on_checkBoxAutoStart_clicked();
    void on_pushButtonPlgnActn_clicked();

private:
    Ui::ConfigurationDialog* ui;
    void loadLocale (const QString&);
};
}
}
}

#endif // CONFIGUI_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
