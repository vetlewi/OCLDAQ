#ifndef XIACONTROL_H
#define XIACONTROL_H

// Header for containers
#include <queue>
#include <vector>
#include <string>
#include <functional>

// Headers for handling threads
#include <mutex>
#include <atomic>

class WriteTerminal;

#define XIA_MIN_READOUT 65536
#define XIA_FIFO_MIN_READOUT 16384
#define SIRIUS_BUFFER_SIZE 32768
#define SCALER_FILE_NAME_IN "scalers_in.dat"
#define SCALER_FILE_NAME_OUT "scalers_out.dat"
#define MAX_RAWDATA_LEN 16 // Maximum length of a raw event without any trace
#define PRESET_MAX_MODULES 24  // Maximum number of modules. As in the XIA API

typedef struct {
    int64_t timestamp;                  //! Timestamp of the event.
    uint32_t raw_data[MAX_RAWDATA_LEN]; //! Pointer to the raw data.
    int size_raw;                       //! Size of the raw data in number of 32 bit words.
} Event_t;
inline bool operator>(const Event_t &a, const Event_t &b) { return (a.timestamp>b.timestamp); }

class XIAControl
{
public:

    XIAControl(WriteTerminal *writeTerm,
               const unsigned short PXImap[PRESET_MAX_MODULES], /*!< PXI mapping                */
               const std::string &FWname="XIA_Firmware.txt",    /*!< Path to firmware settings  */
               const std::string &SETname="settings.set"        /*!< Path to settings file      */);


    // Destructor. Quite important for this
    // class, as it will release all resorces
    // related to the XIA modules and make sure
    // the thread is terminated.
    ~XIAControl(){ ExitXIA(); }

    // Function containing the thread loop.
    void XIAthread();

    // Set internal flag to indicate that the thread should exit.
    void Terminate(){ thread_is_running = false; }

    // Set the PXISlotMap.
    bool SetPXIMapping(const unsigned short PXImap[PRESET_MAX_MODULES]);

    // Set the firmware file. Will return false if run is active.
    bool SetFirmwareFile(const std::string &FWname);

    // Set the settings file. Will return false if run is active.
    bool SetSettingsFile(const std::string &SETname);

    // Poll to check if we have enough data to fill a buffer
    // (note, this will be false until we have 65536 32-bit words of data)
    bool XIA_check_buffer();

    // Ask for a buffer of data to be committed.
    bool XIA_fetch_buffer(uint32_t *buffer, int bufsize);

    // Ask the class to start the run in the XIA modules.
    bool XIA_start_run();

    // Check the run status
    bool XIA_check_status();

    // Ask the class to end the run in the XIA modules.
    // We will also ask for the file where data are stored as we
    // will need to flush all data to this file.
    bool XIA_end_run(FILE *output_file=NULL /* Ending the run will flush all remaning data to file */);

    // Ask for a reload.
    // Will return false if run is active.
    bool XIA_reload();

private:

    // Object responsible for I/O to the stdout & stderr
    WriteTerminal *termWrite;

    // Ordered queue to fill with data as it arrives
    std::priority_queue<Event_t, std::vector<Event_t>, std::greater<Event_t> > sorted_events;

    // Number of 32-bit words in the queue
    int data_avalible;

    // Mutex to make sure that only one of the threads will be able to write to the queue.
    std::mutex queue_mutex;

    // Only one thread can communicate with the XIA API at any given time. We will therefor
    // start all functions that do talk to XIA with a lock.
    std::mutex xia_mutex;

    // Temporary storage for data that are trapped between two buffers.
    std::vector<uint32_t> overflow_queue;

    // Temporary storage for data that are trapped between two buffers.
    std::vector<uint32_t> overflow_fifo[PRESET_MAX_MODULES];

    // Flag to indicate that modules are initialized.
    std::atomic_bool is_initialized;

    // Flag to indicate that modules are booted.
    std::atomic_bool is_booted;

    // Flag to indicate that a run is active
    std::atomic_bool is_running;

    // Flag to indicate that the thread is running.
    std::atomic_bool thread_is_running;

    // Flag to indicate that firmware file has been successfully read.
    //bool Have_firmwares; // I don't think we ever check this flag...

    // Filename of the current XIA DSP settings file.
    std::string settings_file;

    // Number of modules
    int num_modules;

    // PXI mapping of the modules.
    unsigned short PXISlotMap[PRESET_MAX_MODULES];

    // Com. firmware files of the various models.
    char comFPGAConfigFile_RevBCD[2048];
    char comFPGAConfigFile_RevF_100MHz_14Bit[2048];
    char comFPGAConfigFile_RevF_100MHz_16Bit[2048];
    char comFPGAConfigFile_RevF_250MHz_12Bit[2048];
    char comFPGAConfigFile_RevF_250MHz_14Bit[2048];
    char comFPGAConfigFile_RevF_250MHz_16Bit[2048];
    char comFPGAConfigFile_RevF_500MHz_12Bit[2048];
    char comFPGAConfigFile_RevF_500MHz_14Bit[2048];

    // SP FPGA firmware files.
    char SPFPGAConfigFile_RevBCD[2048];
    char SPFPGAConfigFile_RevF_100MHz_14Bit[2048];
    char SPFPGAConfigFile_RevF_100MHz_16Bit[2048];
    char SPFPGAConfigFile_RevF_250MHz_12Bit[2048];
    char SPFPGAConfigFile_RevF_250MHz_14Bit[2048];
    char SPFPGAConfigFile_RevF_250MHz_16Bit[2048];
    char SPFPGAConfigFile_RevF_500MHz_12Bit[2048];
    char SPFPGAConfigFile_RevF_500MHz_14Bit[2048];

    // DSP Code file
    char DSPCodeFile_RevBCD[2048];
    char DSPCodeFile_RevF_100MHz_14Bit[2048];
    char DSPCodeFile_RevF_100MHz_16Bit[2048];
    char DSPCodeFile_RevF_250MHz_12Bit[2048];
    char DSPCodeFile_RevF_250MHz_14Bit[2048];
    char DSPCodeFile_RevF_250MHz_16Bit[2048];
    char DSPCodeFile_RevF_500MHz_12Bit[2048];
    char DSPCodeFile_RevF_500MHz_14Bit[2048];

    // DSP Var file
    char DSPVarFile_RevBCD[2048];
    char DSPVarFile_RevF_100MHz_14Bit[2048];
    char DSPVarFile_RevF_100MHz_16Bit[2048];
    char DSPVarFile_RevF_250MHz_12Bit[2048];
    char DSPVarFile_RevF_250MHz_14Bit[2048];
    char DSPVarFile_RevF_250MHz_16Bit[2048];
    char DSPVarFile_RevF_500MHz_12Bit[2048];
    char DSPVarFile_RevF_500MHz_14Bit[2048];

    // Temporary buffer for strings
    char errmsg[1024];

    // Some private functions that are needed.

    // Delete the current queue of data.
    void flushQueue();

    // Function for reading and parsing Firmware file.
    bool ReadConfigFile(const char *config);

    // Function to initialize the XIA modules.
    bool InitializeXIA();

    // Function to boot the XIA modules.
    bool BootXIA();

    // Function to adjust the baseline.
    bool AdjustBaseline();

    // Function to adjust baseline cut.
    bool AdjustBlCut();

    // Function to start the actual list mode run.
    bool StartLMR();

    // Function where we do the actual stop of a run.
    bool StopRun();

    // Function to check if there is a run in progress.
    // Return false if no run is in progress, false otherwise.
    bool CheckIsRunning();

    // Function to synch all the modules.
    bool SynchModules();

    // Function to read and write scalers.
    bool WriteScalers();

    // Function checking if FIFO has enough data to force a readout.
    bool CheckFIFO(unsigned int minReadout=16384);

    // Read FIFO of all modules and commit data to the queue.
    bool ReadFIFO();

    // Release all XIA resorces.
    bool ExitXIA();

    // Emergency exit. To be called if CheckFIFO or ReadFIFO fails. Will kill any run and free all resorces.
    bool EmergencyExitXIA();

    // Parse data and commit to the queue.
    void ParseQueue(uint32_t *raw_data, int size, int module);

};

#endif // XIACONTROL_H