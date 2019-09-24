//
// Created by Vetle Wegner Ingeberg on 24/09/2019.
//

#include "InterfaceDumper.h"

#include "timercpp.hpp"

int main(int argc, char **argv)
{
    TApplication app("HistDumper", &argc, argv);

    InterfaceDumper mainWindow(gClient->GetRoot());
    app.Run();


    return 0;
}
