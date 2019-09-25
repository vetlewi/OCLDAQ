//
// Created by Vetle Wegner Ingeberg on 24/09/2019.
//

#include "InterfaceDumper.h"

#include <TGMsgBox.h>

#include "pixie16app_export.h"

#include <fstream>
#include <iostream>
#include <string>
#include <utilities.h>
#include <ctime>
#include <thread>

ClassImp(InterfaceDumper)

bool next_line(std::istream &in, std::string &line)
{
    line = "";

    std::string tmp;
    while ( std::getline(in, tmp) ){
        size_t ls = tmp.size();
        if ( ls == 0 ){
            break;
        } else if ( tmp[ls-1] != '\\' ){
            line += tmp;
            break;
        } else {
            line += tmp.substr(0, ls-1);
        }
    }
    return in || !line.empty();
}

fwmap ReadConfigFile(const char *config)
{
    // We expect the file to have the following setup.
    /*
     * # - Indicates a comment
     * \\ - Indicates that the input continues on the next line
     * "comFPGAConfigFile_Rev<R>_<S>MHz_<B>Bit = /path/to/com/syspixie16_xx.bin" - R: Revision, S: ADC freqency and B: ADC bits
     * "SPFPGAConfigFile_Rev<R>_<S>MHz_<B>Bit = /path/to/SPFPGA/fippixie16_xx.bin" - R: Revision, S: ADC freqency and B: ADC bits
     * "DSPCodeFile_Rev<R>_<S>MHz_<B>Bit = /path/to/DSPCode/Pixie16DSP_xx.ldr" - R: Revision, S: ADC freqency and B: ADC bits
     * "DSPVarFile_Rev<R>_<S>MHz_<B>Bit = /path/to/DSPVar/Pixie16DSP_xx.var" - R: Revision, S: ADC freqency and B: ADC bits
     */

    std::ifstream input(config);
    std::string line;

    std::map<std::string, std::string> fw;

    if ( !input.is_open() ){
        std::cerr << "Unable to read firmware config file." << std::endl;
        return fwmap();
    }

    while ( next_line(input, line) ){
        if ( line.empty() || line[0] == '#' )
            continue; // Ignore empty lines or comments.

        // Search for "=" sign on the line.
        std::string::size_type pos_eq = line.find('=');

        // If not found, write a warning and continue to next line.
        if ( pos_eq == std::string::npos ){
            std::cerr << "Could not understand line '" << line << ", skipping..." << std::endl;
            continue;
        }

        std::string key = strip(line.substr(0, pos_eq));
        std::string val = strip(line.substr(pos_eq+1));

        // If the key have already been entered.
        if ( fw.find(key) != fw.end() ){
            std::cerr << "Found multiple definitions of '" << key << "', using latest..." << std::endl;
        }

        fw[key] = val;
    }
    return fw;
}



InterfaceDumper::InterfaceDumper(const TGWindow *parent)
    : TGMainFrame( parent )
    , current_status( no_XIA_connected )
{

    fSettings.num_modules = 7;
    for ( int i = 0 ; i < fSettings.num_modules ; ++i){
        fSettings.plxMap[i] = 2+i;
    }

    fSettings.firmware_file = "XIA_Firmware.txt";
    fSettings.settings_file = "settings.set";

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

            // Get interval. If less than 30 sec, do not start.
            int hour, min, sec;
            fInterval->GetTime(hour, min, sec);
            int time = 3600*hour + 60*min + sec;

            if ( time < 30 ){
                new TGMsgBox(fParent, this, "Error", "Minimum interval is 30 seconds. Please increase interval.");
                return;
            }
            current_interval = time;
            time *= 1000;
            if ( !StartXIA() ){
                std::cerr << "Unable to start run..." << std::endl;
                return;
            }
            current_status = running;
            UpdateGUI();
            fXIATimer.setInterval(std::bind(&InterfaceDumper::DumpMCA, this), time);
            fCountDownTimer.setInterval_CountDown(std::bind(&InterfaceDumper::UpdateCountdown, this, std::placeholders::_1), current_interval, 1000);
            break;
        }

        case running : {

            // First we will stop the XIA run.
            if ( !StopXIA() ){
                gApplication->Error("HandleStartStopButton", "Unable to stop XIA run.");
                return;
            }
            StopXIA();
            current_status = XIA_connected;
            fXIATimer.stop();
            fCountDownTimer.stop();
            UpdateGUI();
            break;
        }

        default : {
            break;
        }
    }
}

void InterfaceDumper::HandleExitButton()
{
    switch ( current_status ){

        case no_XIA_connected : {
            CloseWindow();
            break;
        }

        case XIA_connected : {
            CloseWindow();
            break;
        }

        case running : {
            gApplication->Error("HandleExitButton", "Cannot close while running.");
            UpdateGUI();
            break;
        }

    }
}


void InterfaceDumper::HandleSettingsButton()
{
    auto *dialog = new SettingsDialog(fClient->GetRoot(), &fSettings);
    dialog->Connect("OK_pushed()", "InterfaceDumper", this, "HandleOKClicked()");
    fSettingsButton->SetEnabled(false);
}

void InterfaceDumper::HandleOKClicked()
{
    firmwares = ReadConfigFile(fSettings.firmware_file.c_str());

    if ( !SetupXIA() ){
        gApplication->Error("HandleSettingsButton", "Unable to start XIA modules.");
        CloseWindow();
    }

    current_status = XIA_connected;
    UpdateGUI();
}

void InterfaceDumper::UpdateCountdown(int num)
{
    char text[1024];
    sprintf(text, "%d sec left", num);
    fStatus->SetText(text);
    DoRedraw();
}

void InterfaceDumper::UpdateGUI()
{
    switch ( current_status ){

        case no_XIA_connected : {
            fSettingsButton->SetEnabled(true);
            fStatus->SetText("XIA not ready");
            fStartButton->SetText("Start");
            fStartButton->SetEnabled(false);
            fExitButton->SetEnabled(true);
            fInterval->SetEditable(true);
            break;
        }

        case XIA_connected : {
            fSettingsButton->SetEnabled(false);
            fSettingsButton->SetText("XIA connected");
            fStatus->SetText("Ready...");
            fStartButton->SetText("Start");
            fStartButton->SetEnabled(true);
            fExitButton->SetEnabled(true);
            fInterval->SetEditable(true);
            break;
        }

        case running : {
            fSettingsButton->SetEnabled(false);
            fSettingsButton->SetText("XIA connected");
            fStatus->SetText("Running");
            fStartButton->SetText("Stop");
            fStartButton->SetEnabled(true);
            fExitButton->SetEnabled(false);
            fInterval->SetEditable(false);
            break;
        }

    }
}


void InterfaceDumper::CloseWindow()
{
    fXIATimer.stop();
    fCountDownTimer.stop();
    if ( CheckRunStatus() > 1 )
        StopXIA();

    int retval = Pixie16ExitSystem(fSettings.num_modules);
    gApplication->Terminate(0);
}


void InterfaceDumper::CreateInterface()
{
    auto *vFrame = new TGVerticalFrame(this);

    fSettingsButton = new TGTextButton(vFrame, "Start XIA", Settings_key);
    fSettingsButton->Connect("Clicked()", "InterfaceDumper", this, "HandleSettingsButton()");
    vFrame->AddFrame(fSettingsButton, new TGLayoutHints(kLHintsTop|kLHintsCenterX|kLHintsExpandX));

    fStatus = new TGLabel(vFrame, "XIA not ready");
    vFrame->AddFrame(fStatus, new TGLayoutHints(kLHintsTop|kLHintsCenterX|kLHintsExpandX));

    // Next we need to add time.
    auto *hFrame = new TGHorizontalFrame(vFrame);

    auto *dump_interval_label = new TGLabel(hFrame);
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


bool InterfaceDumper::SetupXIA()
{

    char ComFPGA[2048], SPFPGA[2048], DSPCode[2048], DSPVar[2048];
    char TrigFPGA[] = "trig";
    char DSPSet[2048];
    strcpy(DSPSet, fSettings.settings_file.c_str());

    unsigned short rev[PRESET_MAX_MODULES], bit[PRESET_MAX_MODULES], MHz[PRESET_MAX_MODULES];
    unsigned int sn[PRESET_MAX_MODULES];


    int retval = Pixie16InitSystem(fSettings.num_modules, fSettings.plxMap, 0);

    if ( retval < 0 ){
        std::cerr << "*ERROR* Pixie16InitSystem failed, retval = " << retval << std::endl;
        return false;
    }

    std::cout << "Reading XIA hardware information" << std::endl;
    for ( int i = 0 ; i < fSettings.num_modules ; ++i ){
        retval = Pixie16ReadModuleInfo(i, &rev[i], &sn[i], &bit[i], &MHz[i]);
        if ( retval < 0 ){
            std::cerr << "*ERROR* Pixie16ReadModuleInfo failed, retval = " << retval << std::endl;
            return false;
        }
    }

    for ( int i = 0 ; i < fSettings.num_modules ; ++i ){
        std::cout << "Booting Pixie-16 module #" << i << ", Rev=" << rev[i] << ", S/N=" << sn[i] << ", Bits=" << bit[i];
        std::cout << ", MSPS=" << MHz[i];
        if ( !GetFirmwareFile(rev[i], bit[i], MHz[i], ComFPGA, SPFPGA, DSPCode, DSPVar) ){
            std::cerr << ": Firmware not found!" << std::endl;
            return false;
        } else {
            std::cout << std::endl;
        }

        std::cout << "ComFPGAConfigFile: " << ComFPGA << std::endl;
        std::cout << "SPFPGAConfigFile: " << SPFPGA << std::endl;
        std::cout << "DSPCodeFile: " << DSPCode << std::endl;
        std::cout << "DSPVarFile: " << DSPVar << std::endl;

        retval = Pixie16BootModule(ComFPGA, SPFPGA, TrigFPGA, DSPCode, DSPSet, DSPVar, i, 0x7F);
        if ( retval < 0 ){
            std::cerr << "*ERROR* Pixie16BootModule failed, retval = " << retval << std::endl;
            return false;
        }

        std::cout << "Pixie-16 module #" << i << ": Booted\n\n" << std::endl;
    }

    std::cout << "All modules booted" << std::endl;
    std::cout << "DSPParFile: " << DSPSet << std::endl;
    return true;
}

int InterfaceDumper::CheckRunStatus()
{
    std::lock_guard<std::mutex> mutex_lock( fXIAComMutex );

    // Check that we are not currently running, if so. Return true? Pixie16StartHistogramRun
    for ( int i = 0 ; i < fSettings.num_modules ; ++i ){

        int retval = Pixie16CheckRunStatus(i);

        if ( retval == 0 ){
            continue;
        } else if ( retval == -1 ){
            return retval;
        } else if ( retval == 1 ){
            return retval;
        }
    }
    return 0;
}

bool InterfaceDumper::StartXIA()
{

    // Check that we are not currently running, if so. Return true? Pixie16StartHistogramRun
    int retval = CheckRunStatus();
    if ( retval == -1 ){
        std::cerr << "*ERROR* Unable to start run. Unable to communicate with modules" << std::endl;
        return false;
    } else if ( retval == 1 ){
        std::cerr << "*ERROR* Unable to start run. XIA is already running." << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> mutex_lock( fXIAComMutex );
        retval = Pixie16StartHistogramRun(fSettings.num_modules, NEW_RUN);
    }


    if ( retval < 0 ){
        std::cerr << "*ERROR* Pixie16StartHistogramRun failed, retval = " << retval << std::endl;
        return false;
    }
    return true;
}

bool InterfaceDumper::StopXIA()
{

    int retval;
    {
        std::lock_guard<std::mutex> mutex_lock( fXIAComMutex );
        retval = Pixie16EndRun(0); // We run all modules. Kill one and you'll kill all.
        if ( retval == 0 )
            return true;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Sleep for 0.5 seconds to make sure that we have killed everything.

    // Double check that we indeed have ended everything.
    retval = CheckRunStatus();
    return ( retval == 0 );
}

bool InterfaceDumper::DumpMCA()
{
    std::lock_guard<std::mutex> mutex_lock( fMCAMutex );
    fCountDownTimer.stop();
    fStatus->SetText("Dumping to file");

    // First check if there is a run, if not. return false.
    if ( CheckRunStatus() != 1 ){
        return false;
    }

    // End run.
    if ( !StopXIA() ){
        std::cerr << "*ERROR* Unable to stop XIA run." << std::endl;
        return false;
    }

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char fname[2048], timestring[1024];
    strftime(timestring, sizeof(timestring), "%d-%m-%Y %H-%M-%S", timeinfo);

    // Dump histograms to disk.
    for ( int i = 0 ; i < fSettings.num_modules ; ++i ){
        sprintf(fname, "dump/hist_%s_mod%d.bin", timestring, i);
        int retval = Pixie16SaveHistogramToFile(fname, i);
        if ( retval < 0 ){
            std::cerr << "*ERROR* Pixie16SaveHistogramToFile failed, retval = " << retval << std::endl;
            return false;
        }
        std::cout << "Dumped " << fname << " to disk." << std::endl;
    }

    // Restart the run.
    if ( !StartXIA() ){
        std::cerr << "Unable to start XIA." << std::endl;
        return false;
    }

    fCountDownTimer.setInterval_CountDown(std::bind(&InterfaceDumper::UpdateCountdown, this, std::placeholders::_1), current_interval, 1000);
    return true;
}


bool InterfaceDumper::GetFirmwareFile(const unsigned short &revision, const unsigned short &ADCbits, const unsigned short &ADCMSPS,
                                 char *ComFPGA, char *SPFPGA, char *DSPcode, char *DSPVar)
{
    std::string key_Com, key_SPFPGA, key_DSPcode, key_DSPVar;

    // First, if Rev 11, 12 or 13.
    if ( (revision == 11 || revision == 12 || revision == 13) ){

        // We set the keys.
        key_Com = "comFPGAConfigFile_RevBCD";
        key_SPFPGA = "SPFPGAConfigFile_RevBCD";
        key_DSPcode = "DSPCodeFile_RevBCD";
        key_DSPVar = "DSPVarFile_RevBCD";

    } else if ( revision == 15 ){

        key_Com = "comFPGAConfigFile_RevF_" + std::to_string(ADCMSPS) + "MHz_" + std::to_string(ADCbits) + "Bit";
        key_SPFPGA = "SPFPGAConfigFile_RevF_" + std::to_string(ADCMSPS) + "MHz_" + std::to_string(ADCbits) + "Bit";
        key_DSPcode = "DSPCodeFile_RevF_" + std::to_string(ADCMSPS) + "MHz_" + std::to_string(ADCbits) + "Bit";
        key_DSPVar = "DSPVarFile_RevF_" + std::to_string(ADCMSPS) + "MHz_" + std::to_string(ADCbits) + "Bit";

    } else {
        std::cerr << "Unknown Pixie-16 revision, rev=" << revision << std::endl;
        return false;
    }

    // Search our map for the firmware files.
    if ( firmwares.find(key_Com) == firmwares.end() ){
        std::cerr << "Missing firmware file '" << key_Com << "'" << std::endl;
        return false;
    }

    if ( firmwares.find(key_SPFPGA) == firmwares.end() ){
        std::cerr << "Missing firmware file '" << key_SPFPGA << "'" << std::endl;
        return false;
    }

    if ( firmwares.find(key_DSPcode) == firmwares.end() ){
        std::cerr << "Missing firmware file '" << key_DSPcode << "'" << std::endl;
        return false;
    }

    if ( firmwares.find(key_DSPVar) == firmwares.end() ){
        std::cerr << "Missing firmware file '" << key_DSPVar << "'" << std::endl;
        return false;
    }

    // If we reach this point, we know that we have all the firmwares!
    strcpy(ComFPGA, firmwares[key_Com].c_str());
    strcpy(SPFPGA, firmwares[key_SPFPGA].c_str());
    strcpy(DSPcode, firmwares[key_DSPcode].c_str());
    strcpy(DSPVar, firmwares[key_DSPVar].c_str());

    return true;
}