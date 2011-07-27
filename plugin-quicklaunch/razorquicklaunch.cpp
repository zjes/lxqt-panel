/* BEGIN_COMMON_COPYRIGHT_HEADER
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * https://sourceforge.net/projects/razor-qt/
 *
 * Copyright: 2011 Razor team
 * Authors:
 *   Petr Vanek <petr@scribus.info>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;  only version 2 of
 * the License is valid for this program.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * END_COMMON_COPYRIGHT_HEADER */


#include "razorquicklaunch.h"
#include <qtxdg/xdgdesktopfile.h>
#include <qtxdg/xdgicon.h>
#include <qtxdg/xdgmime.h>

#include <QtDebug>
#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
#include <QtCore/QFileInfo>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QToolButton>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>
#include <QtGui/QFileIconProvider>


EXPORT_RAZOR_PANEL_PLUGIN_CPP(RazorQuickLaunch)


RazorQuickLaunch::RazorQuickLaunch(const RazorPanelPluginStartInfo* startInfo, QWidget* parent)
    : RazorPanelPlugin(startInfo, parent)
{
    setObjectName("QuickLaunch");
    setAcceptDrops(true);

    int count = settings().beginReadArray("apps");

    QString desktop;
    QString file;
    QString execname;
    QString exec;
    QIcon icon;
    for (int i = 0; i < count; ++i)
    {
        settings().setArrayIndex(i);
        desktop = settings().value("desktop", "").toString();
        file = settings().value("file", "").toString();
        if (! desktop.isEmpty())
        {
            XdgDesktopFile * xdg = XdgDesktopFileCache::getFile(desktop);
            if (!xdg->isValid())
            {
                qDebug() << "XdgDesktopFile" << desktop << "is not valid";
                continue;
            }
            addButton(new RazorQuickLaunchAction(xdg, this));
        }
        else if (! file.isEmpty())
        {
            addButton(new RazorQuickLaunchAction(file, this));
        }
        else
        {
            execname = settings().value("name", "").toString();
            exec = settings().value("exec", "").toString();
            icon = QIcon(settings().value("icon", "").toString());
            if (icon.isNull())
            {
                qDebug() << "Icon" << icon << "is not valid (isNull). Skipped.";
                continue;
            }
            addButton(new RazorQuickLaunchAction(execname, exec, icon, this));
        }
    } // for

    settings().endArray();
}

RazorQuickLaunch::~RazorQuickLaunch()
{
}

void RazorQuickLaunch::addButton(QAction* action)
{
    QToolButton* btn = new QToolButton(this);
    btn->setDefaultAction(action);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btn->setIconSize(QSize(22, 22));
    addWidget(btn);
}

void RazorQuickLaunch::dragEnterEvent(QDragEnterEvent *e)
{
    // Getting URL from mainmenu...
    if (e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void RazorQuickLaunch::dropEvent(QDropEvent *e)
{
    const QMimeData *mime = e->mimeData();
    // urls from mainmenu
    foreach (QUrl url, mime->urls())
    {
        QString fileName(url.toLocalFile());
        XdgDesktopFile * xdg = XdgDesktopFileCache::getFile(fileName);
        QFileInfo fi(fileName);
        qDebug() << fileName << fi.exists() << fi.isExecutable();
        if (xdg->isValid())
        {
            addButton(new RazorQuickLaunchAction(xdg, this));
            int count = settings().beginReadArray("apps");
            settings().endArray();
            settings().beginWriteArray("apps", count+1);
            settings().setArrayIndex(count);
            settings().setValue("desktop", fileName);
            settings().endArray();
        }
        else if (fi.exists() && fi.isExecutable() && !fi.isDir())
        {
            addButton(new RazorQuickLaunchAction(fileName, fileName, XdgIcon::defaultApplicationIcon(), this));
            int count = settings().beginReadArray("apps");
            settings().endArray();
            settings().beginWriteArray("apps", count+1);
            settings().setArrayIndex(count);
            settings().setValue("name", fileName);
            settings().setValue("exec", fileName);
            settings().setValue("icon", XdgIcon::defaultApplicationIcon().name());
            settings().endArray();
        }
        else if (fi.exists())
        {
            addButton(new RazorQuickLaunchAction(fileName, this));
            int count = settings().beginReadArray("apps");
            settings().endArray();
            settings().beginWriteArray("apps", count+1);
            settings().setArrayIndex(count);
            settings().setValue("file", fileName);
            settings().endArray();
        }
        else
        {
            qDebug() << "XdgDesktopFile" << fileName << "is not valid";
            QMessageBox::information(this, tr("Drop Error"),
                              tr("File/URL '%1' cannot be embedded into QuickLaunch for now").arg(fileName)
                            );
        }
    }
}

RazorQuickLaunchAction::RazorQuickLaunchAction(const QString & name,
                                               const QString & exec,
                                               const QIcon & icon,
                                               QWidget * parent)
    : QAction(icon, name, parent),
      m_valid(true)
{
    m_type = ActionLegacy;

    if (icon.isNull())
        setIcon(XdgIcon::defaultApplicationIcon());

    setData(exec);
    connect(this, SIGNAL(triggered()), this, SLOT(execAction()));
}

RazorQuickLaunchAction::RazorQuickLaunchAction(const XdgDesktopFile * xdg,
                                               QWidget * parent)
    : QAction(parent),
      m_valid(true)
{
    m_type = ActionXdg;

    QString title(xdg->localizedValue("Name").toString());
    QString gn(xdg->localizedValue("GenericName").toString());
    if (!gn.isEmpty())
        title += " (" + gn + ")";
    setText(title);

    setIcon(xdg->icon(XdgIcon::defaultApplicationIcon()));

    setData(xdg->fileName());
    connect(this, SIGNAL(triggered()), this, SLOT(execAction()));
}

RazorQuickLaunchAction::RazorQuickLaunchAction(const QString & fileName, QWidget * parent)
    : QAction(parent),
      m_valid(true)
{
    m_type = ActionFile;
    setText(fileName);
    setData(fileName);

    QFileInfo fi(fileName);
    if (fi.isDir())
    {
        QFileIconProvider ip;
        setIcon(ip.icon(fi));
    }
    else
    {
        XdgMimeInfo mi(fi);
        setIcon(mi.icon());
    }
    
    connect(this, SIGNAL(triggered()), this, SLOT(execAction()));
}

void RazorQuickLaunchAction::execAction()
{
    QString exec(data().toString());
    switch (m_type)
    {
        case ActionLegacy:
            QProcess::startDetached(exec);
            break;
        case ActionXdg:
        {
            XdgDesktopFile * xdg = XdgDesktopFileCache::getFile(exec);
            if (xdg->isValid())
                xdg->startDetached();
            break;
        }
        case ActionFile:
            QDesktopServices::openUrl(QUrl(exec));
            break;
    }
}
