/***************************************************************************
 *   Copyright (C) 2008 by Lorenzo Masini                                  *
 *   lorenxo86@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "preferencesdialog.h"

#include "preferencespage.h"

PreferencesDialog::PreferencesDialog( QWidget *parent ) 
    : QDialog(parent), Ui::PreferencesDialog()
{
    setupUi(this);
    PreferencesPage *page = NULL;
    for(int i = 0; i < tabWidget->count(); ++i){
        page = static_cast<PreferencesPage *>(tabWidget->widget(i));
        connect(page, SIGNAL(settingsChanged()), this, SLOT(enableApplyButton()));
    }
    buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);

    connect(buttonBox->button(QDialogButtonBox::RestoreDefaults), SIGNAL(clicked()), this, SLOT(restoreDefaults()));
    connect(buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(applyChanges()));
}

void PreferencesDialog::accept()
{
    PreferencesPage *page = NULL;
    for(int i = 0; i < tabWidget->count(); ++i){
        page = static_cast<PreferencesPage *>(tabWidget->widget(i));
        page->applyChanges();
    }
    QDialog::accept();
}

void PreferencesDialog::applyChanges()
{
    PreferencesPage *page;
    page = static_cast<PreferencesPage *>(tabWidget->widget(tabWidget->currentIndex()));
    page->applyChanges();
    buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}

void PreferencesDialog::restoreDefaults()
{
    PreferencesPage *page;
    page = static_cast<PreferencesPage *>(tabWidget->widget(tabWidget->currentIndex()));
    page->restoreDefaults();
    buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}

void PreferencesDialog::enableApplyButton()
{
    buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}
