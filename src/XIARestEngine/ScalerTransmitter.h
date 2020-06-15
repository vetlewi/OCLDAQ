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

private:

    //! Object with InfluxDB interface
    std::unique_ptr<influxdb::InfluxDB> db;

    //! A list of factors needed to calculate the scalers correctly
    std::array<int, PRESET_MAX_MODULES> timestamp_factor{};

    //! Array containing the previously processed scalers
    scaler_t pre_scalers{};

public:

    //! Constructor
    explicit ScalerTransmitter(const char *url);

    //! Process the scalers and fill buffer
    void ProcessScalers(const scaler_t &scalers);

};


#endif // SCALERTRANSMITTER_H
