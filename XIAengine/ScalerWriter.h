//
// Created by Vetle Wegner Ingeberg on 10/09/2019.
//

#ifndef SCALERWRITER_H
#define SCALERWRITER_H

#include <cstdint>

struct Module_Stats;

/*!
 * The class responsible for writing scaler statistics.
 */
class ScalerWriter {

protected:

    //! Field storing previous scaler data.
    Module_Stats *pre_scaler;

    //! Field storing current scaler data.
    Module_Stats *scaler;

    //! Save/transmit result.
    virtual void Transmit() = 0;

public:

    /*!
     * Constructor
     */
    ScalerWriter();

    /*!
     * Destructor
     */
    virtual ~ScalerWriter();

    /*!
     * Instruct the class to acquire a new scaler
     */
    int AcquireScalers();

    /*!
     * Call whenever a run is ended.
     */
    void Clear();

};


#endif // SCALERWRITER_H
