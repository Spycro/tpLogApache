#include <unistd.h>
#include <iostream>


using namespace std;

int main(int argc, char *argv[])
{
    //From man 3 getopt
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

    cout << optind << endl;

    if (optind >= argc) {
        cerr <<  "Expected argument after options" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "name argument = " <<  argv[optind] << endl;
    cout << "argument set : (g, t ,e) " <<graphMake << " " <<timeSort << " " << excludeFiles << endl;

    /* Other code omitted */

    exit(EXIT_SUCCESS);
}
