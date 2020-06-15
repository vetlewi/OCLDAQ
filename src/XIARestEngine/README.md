# XIARestEngine
This is the XIA frontend deamon.
Once finished, this application will run on the frontend computer and
provide a RESTful API for starting/stopping runs, reading and updating
parameters and much more.

## Endpoints
The API will provide a number of endpoints

1) GET: /run/start/listmode - Start the DAQ frontend in list mode
2) GET: /run/start/histmode - Start the DAQ frontend in histogram mode
3) GET: /run/stop - Stop any running acquistion
4) GET: /module/parameters - Get a list of all parameters of all modules
5) POST: /module/parameters - Set module parameters
6) GET: /channel/parameters - Get a list of all channel parameters
7) POST: /channel/parameters - Set channel parameters
8) POST: 