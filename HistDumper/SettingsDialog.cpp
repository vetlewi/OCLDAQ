//
// Created by Vetle Wegner Ingeberg on 25/09/2019.
//

#include "SettingsDialog.h"

#include <TGLabel.h>
#include <TGFileDialog.h>

ClassImp(SettingsDialog)

const char *OpenFileDialog()
{
    // Prompt for file to be opened. Depending on navigation in
    // dialog the current working directory can be changed.
    // The returned file name is always with respect to the
    // current directory.


    static TGFileInfo fi;
    new TGFileDialog(gClient->GetRoot(), gClient->GetRoot(), kFDOpen, &fi);
    return fi.fFilename;
}


SettingsDialog::SettingsDialog(const TGWindow *parent, Settings_t *settings)
    : TGTransientFrame(parent)
    , fSettings( settings )
{

    Setup();

    SetWindowName("Histogram dumper");
    MapSubwindows();
    MapWindow();
    Resize(GetDefaultSize());
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::Setup()
{

    auto *vFrame = new TGVerticalFrame(this);

    auto *modFrame = new TGHorizontalFrame(vFrame);

    auto *mLabel = new TGLabel(modFrame, "Number of modules:");
    modFrame->AddFrame(mLabel, new TGLayoutHints(kLHintsLeft));

    modEntry = new TGNumberEntry(modFrame, 7, 2, k_modEntry, TGNumberEntry::kNESInteger, TGNumberEntry::kNEAPositive, TGNumberEntry::kNELLimitMinMax, 1, 24);
    modFrame->AddFrame(modEntry, new TGLayoutHints(kLHintsLeft|kLHintsExpandX));

    vFrame->AddFrame(modFrame, new TGLayoutHints(kLHintsTop|kLHintsExpandX));

    auto *firmwareFrame = new TGHorizontalFrame(vFrame);

    auto *fLabel = new TGLabel(firmwareFrame, "Firmware file:");
    firmwareFrame->AddFrame(fLabel, new TGLayoutHints(kLHintsLeft));

    firmwareEntry = new TGTextEntry("XIA_Firmware.txt", firmwareFrame);
    firmwareFrame->AddFrame(firmwareEntry, new TGLayoutHints(kLHintsLeft|kLHintsExpandX));

    firmwareFileDialogButton = new TGTextButton(firmwareFrame, "Open", k_firmwareButton);
    firmwareFileDialogButton->Connect("Clicked()", "SettingsDialog", this, "HandleFirmwareFileDialogButton()");
    firmwareFrame->AddFrame(firmwareFileDialogButton, new TGLayoutHints(kLHintsLeft));

    vFrame->AddFrame(firmwareFrame, new TGLayoutHints(kLHintsTop|kLHintsExpandX));

    auto *settingsFrame = new TGHorizontalFrame(vFrame);

    auto *sLabel = new TGLabel(settingsFrame, "Settings file:");
    settingsFrame->AddFrame(sLabel, new TGLayoutHints(kLHintsLeft));

    settingsEntry = new TGTextEntry("settings.set", settingsFrame);
    settingsFrame->AddFrame(settingsEntry, new TGLayoutHints(kLHintsLeft|kLHintsExpandX));

    settingsFileDialogButton = new TGTextButton(settingsFrame, "Open", k_settingsButton);
    settingsFileDialogButton->Connect("Clicked()", "SettingsDialog", this, "HandleSettingsFileDialogButton()");
    settingsFrame->AddFrame(settingsFileDialogButton, new TGLayoutHints(kLHintsLeft));

    vFrame->AddFrame(settingsFrame, new TGLayoutHints(kLHintsTop|kLHintsExpandX));

    auto *ocFrame = new TGHorizontalFrame(vFrame);

    fOKbutton = new TGTextButton(ocFrame, "OK");
    fOKbutton->Connect("Clicked()", "SettingsDialog", this, "HandleOKButton()");
    ocFrame->AddFrame(fOKbutton, new TGLayoutHints(kLHintsCenterX));

    vFrame->AddFrame(ocFrame, new TGLayoutHints(kLHintsTop));

    AddFrame(vFrame, new TGLayoutHints(kLHintsNormal|kLHintsExpandX));

}

void SettingsDialog::HandleOKButton()
{

    fSettings->num_modules = modEntry->GetIntNumber();
    for (int i = 0 ; i < MAX_NUM_MODULES ; ++i){
        fSettings->plxMap[i] = i+2;
    }

    fSettings->firmware_file = std::string(firmwareEntry->GetText());
    fSettings->settings_file = std::string(settingsEntry->GetText());
    OK_pushed();
    CloseWindow();
}

void SettingsDialog::HandleFirmwareFileDialogButton()
{
    firmwareEntry->SetText(OpenFileDialog());
}

void SettingsDialog::HandleSettingsFileDialogButton()
{
    settingsEntry->SetText(OpenFileDialog());
}