//
// Created by Vetle Wegner Ingeberg on 04/05/2020.
//

#ifndef SCALERTRANSMITTER_H
#define SCALERTRANSMITTER_H

#include <array>
#include <memory>

#include <InfluxDB.h>

#define PRESET_MAX_MODULES 24
#define SCALER_LENGTH 448

class ScalerTransmitter
{
public:
    typedef std::array<unsigned int, SCALER_LENGTH> scaler_array_t;
    typedef std::vector<scaler_array_t> scaler_t;

    /*!
     * Setup of the transmitter class
     * \param url of the TS database
     * \param ts_factor time scale factor
     */
    explicit ScalerTransmitter(const char *url, const int *ts_factor = nullptr);

private:

    //! Object with InfluxDB interface
    std::unique_ptr<influxdb::InfluxDB> db;

    //! A list of factors needed to calculate the scalers correctly
    std::array<int, PRESET_MAX_MODULES> timestamp_factor{};

    //! Array containing the previously processed scalers
    scaler_t pre_scalers{};

    //! Time point where readout started
    std::chrono::time_point<std::chrono::system_clock> start_time;

    //! Make class own its own instance
    //static ScalerTransmitter *instance;


public:



    /*!
     * Destructor. Only responsible for resetting global pointer to this object.
     */
    ~ScalerTransmitter();

    //! Set timestamp factors
    void SetTS_Factor(const int *ts_factor);

    //! Set start time
    void Start();

    //! Process the scalers and fill buffer
    void ProcessScalers(const scaler_t &scalers);

    //! Static method to get the ONE (and only one) instance of this class
    //static ScalerTransmitter *Get();

    //! Get (and construct)
    //static ScalerTransmitter *Get(const char *url, const int *ts_factor = nullptr);

};


#endif // SCALERTRANSMITTER_H
