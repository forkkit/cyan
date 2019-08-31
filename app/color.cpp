/*
#
# Cyan <https://cyan.fxarena.net>
# Copyright Ole-André Rodlie, FxArena DA.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
*/

#include "editor.h"
#include <QSettings>
#include <QTimer>
#include <QMessageBox>
#include <QApplication>

#include "convertdialog.h"
#include "CyanColorConvert.h"

void Editor::populateColorProfileMenu(QMenu *menu,
                                      Magick::ColorspaceType colorspace)
{
    menu->clear();
    QMapIterator<QString, QString> i(CyanCommon::getColorProfiles(colorspace));
    while (i.hasNext()) {
        i.next();
        QAction *action = new QAction(menu);
        action->setIcon(QIcon::fromTheme("color-wheel"));
        action->setText(i.key());
        action->setData(i.value());
        action->setCheckable(true);
        menu->addAction(action);
        connect(action,
                SIGNAL(triggered()),
                this,
                SLOT(selectDefaultColorProfile()));
    }
}

void Editor::selectDefaultColorProfile()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action) { return; }

    QMenu *menu = qobject_cast<QMenu*>(action->parent());
    if (!menu) { return; }

    for (int i=0;i<menu->actions().size();++i) {
        QAction *otherAction = menu->actions().at(i);
        if (!otherAction) { continue; }
        otherAction->setChecked(false);
    }
    action->setChecked(true);

    QSettings settings;
    settings.beginGroup(QString("color"));
    if (menu->objectName() == QString("colorProfileRGBMenu")) {
        settings.setValue(QString("rgb_profile"),
                          action->data().toString());
    } else if (menu->objectName() == QString("colorProfileCMYKMenu")) {
        settings.setValue(QString("cmyk_profile"),
                          action->data().toString());
    } else if (menu->objectName() == QString("colorProfileGRAYMenu")) {
        settings.setValue(QString("gray_profile"),
                          action->data().toString());
    }
    settings.endGroup();
    settings.sync();
}

void Editor::setDefaultColorProfiles(QMenu *menu)
{
    QSettings settings;
    settings.beginGroup(QString("color"));

    if (menu->actions().size()==0) {
        if (menu->objectName() == QString("colorProfileRGBMenu")) {
            populateColorProfileMenu(menu,
                                     Magick::sRGBColorspace);
        } else if (menu->objectName() == QString("colorProfileCMYKMenu")) {
            populateColorProfileMenu(menu,
                                     Magick::CMYKColorspace);
        } else if (menu->objectName() == QString("colorProfileGRAYMenu")) {
            populateColorProfileMenu(menu,
                                     Magick::GRAYColorspace);
        }
    }

    if (menu->objectName() == QString("colorProfileRGBMenu")) {
        if (settings.value(QString("rgb_profile")).isValid()) {
            setDefaultColorProfileFromFilename(menu,
                                               settings.value(QString("rgb_profile"))
                                               .toString());
        } else {
            setDefaultColorProfileFromTitle(menu,
                                            QString("sRGB (built-in)"));
        }
    } else if (menu->objectName() == QString("colorProfileCMYKMenu")) {
        if (settings.value(QString("cmyk_profile")).isValid()) {
            setDefaultColorProfileFromFilename(menu,
                                               settings.value(QString("cmyk_profile"))
                                               .toString());
        } else {
            setDefaultColorProfileFromTitle(menu,
                                            QString("ISO Coated v2 (built-in)"));
        }
    } else if (menu->objectName() == QString("colorProfileGRAYMenu")) {
        if (settings.value(QString("gray_profile")).isValid()) {
            setDefaultColorProfileFromFilename(menu,
                                               settings.value(QString("gray_profile"))
                                               .toString());
        } else {
            setDefaultColorProfileFromTitle(menu,
                                            QString("ISO Coated v2 - GREY 1c - (built-in)"));
        }
    }
    settings.endGroup();
}

void Editor::setDefaultColorProfileFromFilename(QMenu *menu,
                                                const QString &filename)
{
    for (int i=0;i<menu->actions().size();++i) {
        QAction *action = menu->actions().at(i);
        if (!action) { continue; }
        if (action->data().toString() == filename) {
            action->setChecked(true);
        } else { action->setChecked(false); }
    }
}

void Editor::setDefaultColorProfileFromTitle(QMenu *menu,
                                             const QString &title)
{
    for (int i=0;i<menu->actions().size();++i) {
        QAction *action = menu->actions().at(i);
        if (!action) { continue; }
        if (action->text() == title) {
            action->setChecked(true);
        } else { action->setChecked(false); }
    }
}

void Editor::checkDefaultColorProfiles()
{
    bool hasRGBProfiles = colorProfileRGBMenu->actions().size()>0?true:false;
    bool hasCMYKProfiles = colorProfileCMYKMenu->actions().size()>0?true:false;
    bool hasGRAYProfiles = colorProfileGRAYMenu->actions().size()>0?true:false;
    bool hasRGBDefaultProfile = false;
    bool hasCMYKDefaultProfile = false;
    bool hasGRAYDefaultProfile = false;

    if (hasRGBProfiles) {
        for (int i=0;i<colorProfileRGBMenu->actions().size();++i) {
            QAction *action = colorProfileRGBMenu->actions().at(i);
            if (!action) { continue; }
            if (action->isChecked()) {
                hasRGBDefaultProfile = true;
                break;
            }
        }
        if (!hasRGBDefaultProfile) {
            qWarning() << "NO RGB PROFILE SELECTED! SETTING THE FIRST AVAILABLE";
            colorProfileRGBMenu->actions().at(0)->setChecked(true);
            QString defaultProfile = selectedDefaultColorProfile(colorProfileRGBMenu);
            if (!defaultProfile.isEmpty()) {
                setDefaultColorProfileFromFilename(colorProfileRGBMenu,
                                                   defaultProfile);
            }
        }
    }
    if (hasCMYKProfiles) {
        for (int i=0;i<colorProfileCMYKMenu->actions().size();++i) {
            QAction *action = colorProfileCMYKMenu->actions().at(i);
            if (!action) { continue; }
            if (action->isChecked()) {
                hasCMYKDefaultProfile = true;
                break;
            }
        }
        if (!hasCMYKDefaultProfile) {
            qWarning() << "NO CMYK PROFILE SELECTED! SETTING THE FIRST AVAILABLE";
            colorProfileCMYKMenu->actions().at(0)->setChecked(true);
            QString defaultProfile = selectedDefaultColorProfile(colorProfileCMYKMenu);
            if (!defaultProfile.isEmpty()) {
                setDefaultColorProfileFromFilename(colorProfileCMYKMenu,
                                                   defaultProfile);
            }
        }
    }
    if (hasGRAYProfiles) {
        for (int i=0;i<colorProfileGRAYMenu->actions().size();++i) {
            QAction *action = colorProfileGRAYMenu->actions().at(i);
            if (!action) { continue; }
            if (action->isChecked()) {
                hasGRAYDefaultProfile = true;
                break;
            }
        }
        if (!hasGRAYDefaultProfile) {
            qWarning() << "NO GRAY PROFILE SELECTED! SETTING THE FIRST AVAILABLE";
            colorProfileGRAYMenu->actions().at(0)->setChecked(true);
            QString defaultProfile = selectedDefaultColorProfile(colorProfileGRAYMenu);
            if (!defaultProfile.isEmpty()) {
                setDefaultColorProfileFromFilename(colorProfileCMYKMenu,
                                                   defaultProfile);
            }
        }
    }
}

const QString Editor::selectedDefaultColorProfile(QMenu *menu)
{
    for (int i=0;i<menu->actions().size();++i) {
        QAction *action = menu->actions().at(i);
        if (!action) { continue; }
        if (action->isChecked()) { return action->data().toString(); }
    }
    return QString();
}

Magick::Blob Editor::selectedDefaultColorProfileData(QMenu *menu)
{
    QString filename;
    for (int i=0;i<menu->actions().size();++i) {
        QAction *action = menu->actions().at(i);
        if (!action) { continue; }
        if (action->isChecked()) { filename =  action->data().toString(); }
    }
    if (!filename.isEmpty()) {
        try {
            Magick::Image image;
            image.read(filename.toStdString());
            Magick::Blob profile;
            image.write(&profile);
            return profile;
        }
        catch(Magick::Error &error_ ) { emit errorMessage(error_.what()); }
        catch(Magick::Warning &warn_ ) { emit warningMessage(warn_.what()); }
    }
    return Magick::Blob();
}

void Editor::populateColorIntentMenu()
{
    QIcon intentIcon = QIcon::fromTheme("video-display");
    QAction *action1 = new QAction(colorIntentMenu);
    action1->setText(tr("Undefined"));
    action1->setData(CyanCommon::UndefinedRenderingIntent);
    action1->setCheckable(true);
    action1->setIcon(intentIcon);
    connect(action1,
            SIGNAL(triggered()),
            this,
            SLOT(setDefaultColorIntent()));

    QAction *action2 = new QAction(colorIntentMenu);
    action2->setText(tr("Saturation"));
    action2->setData(CyanCommon::SaturationRenderingIntent);
    action2->setCheckable(true);
    action2->setIcon(intentIcon);
    connect(action2,
            SIGNAL(triggered()),
            this,
            SLOT(setDefaultColorIntent()));

    QAction *action3 = new QAction(colorIntentMenu);
    action3->setText(tr("Perceptual"));
    action3->setData(CyanCommon::PerceptualRenderingIntent);
    action3->setCheckable(true);
    action3->setIcon(intentIcon);
    connect(action3,
            SIGNAL(triggered()),
            this,
            SLOT(setDefaultColorIntent()));

    QAction *action4 = new QAction(colorIntentMenu);
    action4->setText(tr("Absolute"));
    action4->setData(CyanCommon::AbsoluteRenderingIntent);
    action4->setCheckable(true);
    action4->setIcon(intentIcon);
    connect(action4,
            SIGNAL(triggered()),
            this,
            SLOT(setDefaultColorIntent()));

    QAction *action5 = new QAction(colorIntentMenu);
    action5->setText(tr("Relative"));
    action5->setData(CyanCommon::RelativeRenderingIntent);
    action5->setCheckable(true);
    action5->setIcon(intentIcon);
    connect(action5,
            SIGNAL(triggered()),
            this,
            SLOT(setDefaultColorIntent()));

    colorIntentMenu->addAction(action1);
    colorIntentMenu->addAction(action2);
    colorIntentMenu->addAction(action3);
    colorIntentMenu->addAction(action4);
    colorIntentMenu->addAction(action5);
}

void Editor::setDefaultColorIntent()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action) { return; }

    for (int i=0;i<colorIntentMenu->actions().size();++i) {
        QAction *otherAction = colorIntentMenu->actions().at(i);
        if (!otherAction) { continue; }
        otherAction->setChecked(false);
    }
    action->setChecked(true);

    QSettings settings;
    settings.beginGroup(QString("color"));
    settings.setValue(QString("intent"),
                      action->data().toInt());
    settings.endGroup();
    settings.sync();
}

void Editor::loadDefaultColorIntent()
{
    QSettings settings;
    settings.beginGroup(QString("color"));
    for (int i=0;i<colorIntentMenu->actions().size();++i) {
        QAction *action = colorIntentMenu->actions().at(i);
        if (!action) { continue; }
        if (action->data().toInt() == settings.value(QString("intent"),
                                                     CyanCommon::PerceptualRenderingIntent).toInt())
        {
            action->setChecked(true);
        } else { action->setChecked(false); }
    }
    settings.endGroup();
}

void Editor::handleColorConvertRGB(bool ignoreColor, const QString &title)
{
    handleColorConvert(ignoreColor,
                       Magick::sRGBColorspace,
                       title);
}

void Editor::handleColorConvertCMYK(bool ignoreColor, const QString &title)
{
    handleColorConvert(ignoreColor,
                       Magick::CMYKColorspace,
                       title);
}

void Editor::handleColorConvertGRAY(bool ignoreColor, const QString &title)
{
    handleColorConvert(ignoreColor,
                       Magick::GRAYColorspace,
                       title);
}

void Editor::handleColorProfileAssign()
{
    if (!getCurrentCanvas()) { return; }
    switch(getCurrentCanvas()->getCanvas().colorSpace()) {
    case Magick::CMYKColorspace:
        handleColorConvertCMYK(true,
                               tr("Assign Color Profile"));
        break;
    case Magick::GRAYColorspace:
        handleColorConvertGRAY(true,
                               tr("Assign Color Profile"));
        break;
    default:
        handleColorConvertRGB(true,
                              tr("Assign Color Profile"));
    }
}

void Editor::handleColorConvert(bool ignoreColor,
                                Magick::ColorspaceType colorspace,
                                const QString &title)
{
    if (!getCurrentCanvas()) { return; }
    if (getCurrentCanvas()->getCanvas().colorSpace() == colorspace && !ignoreColor) {
        emit statusMessage(tr("Already the requested colorspace"));
        return;
    }
    QString profile;
    switch(colorspace) {
    case Magick::CMYKColorspace:
        profile = selectedDefaultColorProfile(colorProfileCMYKMenu);
        break;
    case Magick::GRAYColorspace:
        profile = selectedDefaultColorProfile(colorProfileGRAYMenu);
        break;
    default:
        profile = selectedDefaultColorProfile(colorProfileRGBMenu);
    }
    ConvertDialog *dialog = new ConvertDialog(this,
                                              title,
                                              profile,
                                              colorspace);
    int ret = dialog->exec();
    if (ret == QDialog::Accepted &&
        !dialog->getProfile().isEmpty())
    {
        qDebug() << "CONVERT USING" << dialog->getProfile();
        CyanImageFormat::CyanCanvas canvas = getCurrentCanvas()->getCanvasProject();
        canvas.image = ColorConvert::convertColorspace(canvas.image,
                                                 canvas.profile,
                                                 dialog->getProfile());
        for (int i=0;i<canvas.layers.size();++i) {
            canvas.layers[i].image= ColorConvert::convertColorspace(canvas.layers[i].image,
                                                              canvas.profile,
                                                              dialog->getProfile());
        }
        canvas.profile = canvas.image.iccColorProfile();
        getCurrentCanvas()->updateCanvas(canvas);
        updateTabTitle(getCurrentCanvas());
    }

    QTimer::singleShot(100,
                       dialog,
                       SLOT(deleteLater()));
}

void Editor::hasColorProfiles()
{
    int rgbs = CyanCommon::getColorProfiles(Magick::sRGBColorspace).size();
    int cmyks = CyanCommon::getColorProfiles(Magick::CMYKColorspace).size();
    int grays = CyanCommon::getColorProfiles(Magick::GRAYColorspace).size();
    if (rgbs<1 || cmyks<1 || grays<1) {
        QMessageBox::warning(this,
                             tr("Missing color profiles"),
                             tr("Missing color profiles."
                                " Cyan needs a minimum of one color profile"
                                " for each supported color space (RGB/CMYK/GRAY)."));
        QTimer::singleShot(100,
                           qApp,
                           SLOT(quit()));
    }
}

void Editor::handleColorChanged(const QColor &color)
{
    qDebug() << "brush color changed" << color;
    QList<QMdiSubWindow*> list = mdi->subWindowList();
    for (int i=0;i<list.size();++i) {
        QMdiSubWindow *window = qobject_cast<QMdiSubWindow*>(list.at(i));
        if (!window) { continue; }
        View *view = qobject_cast<View*>(window->widget());
        if (!view) { continue; }
        view->setBrushColor(color);
    }
    emit currentColorChanged(color);
}
