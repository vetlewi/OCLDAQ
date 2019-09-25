//
// Created by Vetle Wegner Ingeberg on 24/09/2019.
//

#ifndef INTERFACEDUMPER_H
#define INTERFACEDUMPER_H

#include <TGApplication.h>
#include <TGWindow.h>

#include <TGLabel.h>
#include <TGButton.h>
#include <TGNumberEntry.h>

#include "timercpp.hpp"
#include "SettingsDialog.h"

#include <mutex>

typedef std::map<std::string, std::string> fwmap;

class InterfaceDumper : public TGMainFrame
{

public:

    /*!
     * Constructor.
     * @param parent - parent window.
     */
    InterfaceDumper(const TGWindow *parent);

    /*!
     * Destructor
     */
    virtual ~InterfaceDumper();

    /*!
     * Method for updating all GUI elements.
     */
    void UpdateGUI();

    /*!
     * Method for handling start/stop button click.
     */
    void HandleStartStopButton();

    /*!
     * Method for handling exit button click.
     */
    void HandleExitButton();

    /*!
     * Method for handling settings button click.
     */
    void HandleSettingsButton();

    /*!
     * Method for dumping MCA to file and restarting the XIA run.
     */
    bool DumpMCA();

private:

    /*!
     * Current status of the setup.
     */
    enum Run_status_t {
        no_XIA_connected,   //!< Flag to indicate that XIA has not yet been initialized.
        XIA_connected,      //!< Flag to indicate that XIA is connected, but no run has been started yet.
        running             //!< Flag to indicate that XIA is currently running.
    };

    //! Current status.
    Run_status_t current_status;

    /*!
     * Widget ID's
     */
    enum WidgetID {
        Interval_key = 1,
        StartStop_key = 2,
        Exit_key = 3,
        Settings_key = 4
    };

    /*!
     * Method responsible for creating and setting up the window.
     */
    void CreateInterface();

    /*!
     * Method for connecting and setting up the XIA modules.
     */
    bool SetupXIA();

    /*!
     * Method for starting the XIA run.
     */
    bool StartXIA();

    /*!
     * Method for stopping the XIA run.
     */
    bool StopXIA();

    /*!
     * Check run status.
     */
    int CheckRunStatus();

    /*!
     * Method for closing the application.
     */
    void CloseWindow();

    //! Label for current status.
    TGLabel *fStatus;

    //! XIA settings button.
    TGTextButton *fSettingsButton;

    //! Entry for interval for dumping data to file.
    TGNumberEntry *fInterval;

    //! Start/stop button.
    TGTextButton *fStartButton;

    //! Exit button.
    TGTextButton *fExitButton;

    //! Timer object that will perform the callback.
    Timer fTimer;

    //! Mutex to make calls to XIA interface thread safe.
    std::mutex fMCAMutex, fXIAComMutex;

    //! Settings object to read XIA settings from.
    Settings_t fSettings;

    //! Mapping of XIA firmware data.
    fwmap firmwares;

    //! Function to get the firmware file paths.
    bool GetFirmwareFile(const unsigned short &revision, const unsigned short &ADCbits, const unsigned short &ADCMSPS,
                         char *ComFPGA, char *SPFPGA, char *DSPcode, char *DSPVar);

    ClassDef(InterfaceDumper, 1)
};


#endif // INTERFACEDUMPER_H
