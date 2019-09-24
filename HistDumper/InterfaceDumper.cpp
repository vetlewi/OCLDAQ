//
// Created by Vetle Wegner Ingeberg on 24/09/2019.
//

#include "InterfaceDumper.h"

ClassImp(InterfaceDumper)

InterfaceDumper::InterfaceDumper(const TGWindow *parent)
    : TGMainFrame( parent )
    , current_status( no_XIA_connected )
{

    CreateInterface();
    UpdateGUI();

    SetWindowName("Histogram dumper");
    MapSubwindows();
    MapWindow();
    Resize(GetDefaultSize());
}

InterfaceDumper::~InterfaceDumper()
{
    CloseWindow();
}

void InterfaceDumper::HandleStartStopButton()
{
    switch ( current_status ){

        case no_XIA_connected : {
            gApplication->Error("HandleStartStopButton", "Start button clicked before XIA is initialized.");
            break;
        }

        case XIA_connected : {
            // First we start the XIA run.
            if ( !StartXIA() ){
                gApplication->Error("HandleStartStopButton", "Unable to start XIA run.");
                return;
            }

            // Update status
            current_status = running;
            int hour, min, sec;
            fInterval->GetTime(hour, min, sec);
            int time = 3600*hour + 60*min + sec;
            time *= 1000;
            fTimer.setInterval(std::bind(&InterfaceDumper::DumpMCA, this), time);
            UpdateGUI();
            break;
        }

        case running : {

            // First we will stop the XIA run.
            if ( !StopXIA() ){
                gApplication->Error("HandleStartStopButton", "Unable to stop XIA run.");
                return;
            }

            current_status = XIA_connected;
            fTimer.stop();
            UpdateGUI();
            break;
        }

        default : {
            break;
        }
    }
}

void InterfaceDumper::UpdateGUI()
{
    switch ( current_status ){

        case no_XIA_connected : {
            fStatus->SetText("XIA not ready");
            fStartButton->SetText("Start");
            fStartButton->SetEnabled(false);
            fExitButton->SetEnabled(true);
            break;
        }

        case XIA_connected : {
            fStatus->SetText("Ready...");
            fStartButton->SetText("Start");
            fStartButton->SetEnabled(true);
            fExitButton->SetEnabled(true);
        }

        case running : {
            fStatus->SetText("Running");
            fStartButton->SetText("Stop");
            fStartButton->SetEnabled(true);
            fExitButton->SetEnabled(false);
        }

    }
}

void InterfaceDumper::CreateInterface()
{
    TGVerticalFrame *vFrame = new TGVerticalFrame(this);

    fStatus = new TGLabel(vFrame);
    fStatus->SetText("XIA not ready");
    vFrame->AddFrame(fStatus, new TGLayoutHints(kLHintsTop|kLHintsCenterX|kLHintsExpandX));

    // Next we need to add time.
    TGHorizontalFrame *hFrame = new TGHorizontalFrame(vFrame);

    TGLabel *dump_interval_label = new TGLabel(hFrame);
    dump_interval_label->SetText("Dump interval (hr::min:sec):");
    hFrame->AddFrame(dump_interval_label, new TGLayoutHints(kLHintsNormal));

    fInterval = new TGNumberEntry(hFrame, 0, 8, Interval_key, TGNumberEntry::kNESHourMinSec, TGNumberEntry::kNEAPositive);
    fInterval->SetTime(0, 10, 0);
    hFrame->AddFrame(fInterval, new TGLayoutHints(kLHintsNormal|kLHintsExpandX));

    vFrame->AddFrame(hFrame, new TGLayoutHints(kLHintsTop|kLHintsExpandX));

    fStartButton = new TGTextButton(vFrame, "Start", StartStop_key);
    fStartButton->Connect("Clicked()", "InterfaceDumper", this, "HandleStartStopButton()");
    vFrame->AddFrame(fStartButton, new TGLayoutHints(kLHintsTop|kLHintsCenterX|kLHintsExpandX));

    fExitButton = new TGTextButton(vFrame, "Exit", Exit_key);
    fExitButton->Connect("Clicked()", "InterfaceDumper", this, "HandleExitButton()");
    vFrame->AddFrame(fExitButton, new TGLayoutHints(kLHintsTop|kLHintsCenterX|kLHintsExpandX));

    AddFrame(vFrame, new TGLayoutHints(kLHintsTop|kLHintsCenterX|kLHintsExpandX));
}