/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Razor team
 *            2014 LXQt team
 * Authors:
 *   Kuzma Shapran <kuzma.shapran@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#ifndef LXQT_PANEL_WORLDCLOCK_CONFIGURATION_MANUAL_FORMAT_H
#define LXQT_PANEL_WORLDCLOCK_CONFIGURATION_MANUAL_FORMAT_H

#include <QDialog>
#include <QAbstractButton>


namespace Ui {
    class LxQtWorldClockConfigurationManualFormat;
}

class QTreeWidgetItem;

class LxQtWorldClockConfigurationManualFormat : public QDialog
{
    Q_OBJECT

public:
    explicit LxQtWorldClockConfigurationManualFormat(QWidget *parent = NULL);
    ~LxQtWorldClockConfigurationManualFormat();

    void setManualFormat(const QString&);

    QString manualFormat() const;

signals:
    void manualFormatChanged();

private:
    Ui::LxQtWorldClockConfigurationManualFormat *ui;
};

#endif // LXQT_PANEL_WORLDCLOCK_CONFIGURATION_MANUAL_FORMAT_H
