/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://lxqt.org
 *
 * Copyright: 2015 LxQt team
 * Authors:
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


#ifndef SPACERCONFIGURATION_H
#define SPACERCONFIGURATION_H

#include <LXQt/Settings>

#include <QDialog>

class QSettings;
class QAbstractButton;

namespace Ui {
    class SpacerConfiguration;
}

class SpacerConfiguration : public QDialog
{
    Q_OBJECT

public:
    explicit SpacerConfiguration(QSettings *settings, QWidget *parent = 0);
    ~SpacerConfiguration();

public:
    static const QStringList msTypes;

private:
    Ui::SpacerConfiguration *ui;
    QSettings *mSettings;

private slots:
    /*
       Saves settings in conf file.
    */
    void loadSettings();
    void sizeChanged(int value);
    void typeChanged(int index);
};

#endif // SPACERCONFIGURATION_H
