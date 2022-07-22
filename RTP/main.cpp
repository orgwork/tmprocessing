#include "../includes/SystemFiles.h"
#include "ThreadSpawner.h"

int main(int argc, char *argv[])
{
    //    Check and Exit if SCID & Message Queue Identifier are not provided
    if (argc < 3)
    {
        cout << "Please Provide Spacecraft-ID & Message Queue Identifier as Arguments" << endl;
        return EXIT_FAILURE;
    }

    string scid = argv[1];
    string msgQIdentifier = argv[2];

    // Spawn Threads & Start Receiving Data
    ThreadSpawner ts(scid, msgQIdentifier);

    string errMsg;

    //    Initiation
    if (!ts.Init(errMsg))
    {
        cout << errMsg << endl;
        return EXIT_FAILURE;
    }

    //    Attach to all SHM Segments for the group
    if (!ts.AttachToShmSegments(errMsg))
    {
        cout << errMsg << endl;
        return EXIT_FAILURE;
    }

    //    Spawn Threads to process all streams of the group
    if (!ts.SpawnThreads(errMsg))
    {
        cout << errMsg << endl;
        return EXIT_FAILURE;
    }

    //    Start Receiving data and process
    if (!ts.StartProcessing(errMsg))
    {
        cout << errMsg << endl;
        return EXIT_FAILURE;
    }

    cout << "Gracefully Exited" << endl;


    return EXIT_SUCCESS;
}
