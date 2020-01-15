#include <unistd.h>
#include <iostream>
#include "Reader.h"

using namespace std;

const string localPath = "http://intranet-if.insa-lyon.fr";

void parseData(rawData data, bool exclude, bool date, string heure);



int main(int argc, char *argv[])
{
    //----------From man 3 getopt---------------
    //regarding time options
    string time;
    bool timeSort = false;
    //ragarding graph otions
    string graphFile;
    bool graphMake = false;
    //regarding file exclusion
    bool excludeFiles = false;
    string fileName;



    char opt;
    while ((opt = getopt(argc, argv, "g:et:")) != -1) {
        switch (opt) {
        case 'g':
            graphFile = optarg; //optarg peut etre un fichier commencant par un tiret (du style "-e")
            graphMake = true;
            break;
        case 't':
            time = optarg;
            timeSort = true;
            break;
        case 'e':
            excludeFiles = true;
            break;
        default:
            cerr << "Usage: " << argv[0] << "[-g filename] [-e] [-t time] logfile" << endl;
            exit(EXIT_FAILURE);
        }
    }

    if (argv[optind] == nullptr) {
        cerr << "Mandatory argument(s) missing" << endl;
        cerr << "Usage: " << argv[0] << "[-g filename] [-e] [-t time] logfile" << endl;
        exit(EXIT_FAILURE);
    }

    if (argv[optind + 1] != nullptr){
        cerr << "Too many argument" << endl;
        cerr << "Usage: " << argv[0] << "[-g filename] [-e] [-t time] logfile" << endl;
        exit(EXIT_FAILURE);
    }

    if (optind >= argc) {
        cerr <<  "Expected argument after options" << endl;
        exit(EXIT_FAILURE);
    }



    cout << "name argument = " <<  argv[optind] << endl;
    cout << "argument set : (g, t ,e) " <<graphMake << " " <<timeSort << " " << excludeFiles << endl;
    fileName = argv[optind];
    Reader logReader(fileName);
    rawData donnee = logReader.GetNextLine();
    cout << donnee.ip << " " << donnee.userAgent << " " << donnee.target << " " << donnee.referer << " " << endl;

    for (size_t i = 0; i < 50; i++) {
        parseData(donnee, excludeFiles, timeSort, time);
        donnee = logReader.GetNextLine();
    }
    /* Other code omitted */

    exit(EXIT_SUCCESS);
}


void parseData(rawData data, bool exclude, bool date, string heure){
    size_t index;
    if((index = data.referer.find(localPath)) != std::string::npos){
        cout << "need to trim it" << endl;

        data.referer.erase(index, localPath.length());
        cout << "trimmed : " << data.referer << endl;
    }


    if(exclude && (data.referer.find(".jpg") == std::string::npos
                    || data.target.find(".jpg") == std::string::npos )){
        cout << "found an image" << endl;
    }
}
