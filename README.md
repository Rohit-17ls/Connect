# Connect
TCP connections within a machine

## Install locally by cloning the repository
Navigate to Connect\
`cd Connect/`

If it is your first time using this tool, run\
`sh init.sh`

Run the below command to get help\
`./connect -help`

Ensure there is exactly one server and that it is running\
To create a server : `./connect -server -8000 @serve`

All clients join to the port specified by the server\
To join as a client : `./connect -client -8000 @<username>`\

**NOTE** : All clients must quit by entering "exit" into the input stream, using ctrl+c destroys the environment for all connected clients
