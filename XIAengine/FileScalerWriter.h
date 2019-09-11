//
// Created by Vetle Wegner Ingeberg on 10/09/2019.
//

#ifndef FILESCALERWRITER_H
#define FILESCALERWRITER_H

#include "ScalerWriter.h"

#include <iosfwd>

class FileScalerWriter : public ScalerWriter
{
private:

    //! Field to store the name of the file we will be writing to.
    char *fname;

protected:

    /*!
     * Transmit the scaler data.
     */
    void Transmit() override;

public:

    /*!
     * Constructor
     * @param fname - name of file to write to.
     * @param num_mod - number of modules.
     */
    FileScalerWriter(const char *name);

    /*!
     * Destructor
     */
    ~FileScalerWriter();



};


#endif //OCLDAQ_FILESCALERWRITER_H
