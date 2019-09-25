//
// Created by Vetle Wegner Ingeberg on 25/09/2019.
//

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <string>

#include <TGFrame.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TGButton.h>

#define MAX_NUM_MODULES 24

struct Settings_t {
    unsigned short num_modules;
    unsigned short plxMap[MAX_NUM_MODULES];
    std::string firmware_file;
    std::string settings_file;
};


class SettingsDialog : public TGTransientFrame
{

public:

    /*!
     * Ctor
     */
    SettingsDialog(const TGWindow *parent, Settings_t *settings);

    /*!
     * Dtor
     */
    virtual ~SettingsDialog();

    /*!
     * Handle a OK button press event.
     */
    void HandleOKButton();

    /*!
     * Handle a firmwareFileDialogButton press event.
     */
    void HandleFirmwareFileDialogButton();

    /*!
     * Handle a settingsFileDialogButton press event.
     */
    void HandleSettingsFileDialogButton();

private:

    enum k_Items {
        k_modEntry = 1,
        k_firmwareEntry = 2,
        k_firmwareButton = 3,
        k_settingsEntry = 4,
        k_settingsButton = 5,
        k_OKButton = 6,
        k_CancelButton = 7
    };

    //! Setup interface.
    void Setup();

    //! Pointer to the settings object.
    Settings_t *fSettings;

    //! Number input for the number of modules.
    TGNumberEntry *modEntry;

    //! Text input for the XIA firmware file.
    TGTextEntry *firmwareEntry;

    //! Button for firmware file dialog.
    TGTextButton *firmwareFileDialogButton;

    //! Text input for the XIA settings file.
    TGTextEntry *settingsEntry;

    //! Button for settings file dialog.
    TGTextButton *settingsFileDialogButton;

    //! OK button.
    TGTextButton *fOKbutton;

    ClassDef(SettingsDialog, 1);
};


#endif //SETTINGSDIALOG_H
