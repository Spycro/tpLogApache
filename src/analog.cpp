#include <unistd.h>
#include <iostream>
#include <map>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <string>
#include "Reader.h"
//#include <boost/functional/hash.hpp>

using namespace std;

const string localPath = "http://intranet-if.insa-lyon.fr";


//Fonction de hashage simple prise de stack overflow
//Necessaire pour l'utilisation d'un pair<string, string> en tant que cle
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

typedef pair<string, string> Key; // pair de (target, ref)


typedef struct mapStruct{
    unordered_map<Key, int, pair_hash> graphMap;
    unordered_map<string, int> unorderedHitMap;
    multimap<int, string> orderedHitMap;
} mapStruct;



void parseData(rawData & data, bool exclude, bool date, string & heure, bool graph, mapStruct & mesMaps);
bool isImage(string url);
void makeGraphFile(mapStruct &mesMaps, string nameFile);
void reverse(mapStruct & mesMaps);

int main(int argc, char *argv[])
{
    //initialisation de la structures

    mapStruct mesMaps;



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
            cerr << "Usage: " << argv[0] << " [-g filename] [-e] [-t time] logfile" << endl;
            exit(EXIT_FAILURE);
        }
    }

    if (argv[optind] == nullptr) {
        cerr << "Mandatory argument(s) missing" << endl;
        cerr << "Usage: " << argv[0] << " [-g filename] [-e] [-t time] logfile" << endl;
        exit(EXIT_FAILURE);
    }

    if (argv[optind + 1] != nullptr){
        cerr << "Too many argument" << endl;
        cerr << "Usage: " << argv[0] << " [-g filename] [-e] [-t time] logfile" << endl;
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
    rawData donnee;
    //cout << donnee.ip << " " << donnee.userAgent << " " << donnee.target << " " << donnee.referer << " " << endl;


    while(!logReader.EndOfFile()){
        donnee = logReader.GetNextLine();
        if (donnee.target != "")
        {
            parseData(donnee, excludeFiles, timeSort, time, graphMake, mesMaps);
        }
    }
    /* Other code omitted */
    // Maintenant il faut inverser la map non ordonnee

    reverse(mesMaps);
    int i =0;
    for(auto it=mesMaps.orderedHitMap.end(); it!=mesMaps.orderedHitMap.begin() && i<10;  i++)
    {
        it--;
        cout << it->second << " | hit : " << it->first << endl;
    }

    if (graphMake)
    {
        makeGraphFile(mesMaps, graphFile);
    }

    exit(EXIT_SUCCESS);
}


void parseData(rawData & data, bool exclude, bool date, string & heure, bool graph,  mapStruct & mesMaps){
    size_t index;

    //first we need to convert to lowercase everything :
    transform(data.referer.begin(), data.referer.end(), data.referer.begin(),::tolower);
    transform(data.target.begin(), data.target.end(), data.target.begin(),::tolower);


    if((index = data.referer.find(localPath)) != std::string::npos){
        data.referer.erase(index, localPath.length());
    }
    if((index = data.referer.find("?")) != std::string::npos){
        data.referer.erase(index, data.referer.length()-index);
    }
    if((index = data.target.find("?")) != std::string::npos){
        data.target.erase(index, data.target.length()-index);
    }

    if(exclude && (isImage(data.referer) ||isImage(data.target)) ){
        //cout << "found an image" << endl;
        return;
    }

    if(date){
        //Here things to do with date
        //TODO parse date and time
        if (data.date.length() == 26)
        {
            //cout << data.date << " " << data.date.length() << endl;
            data.date.erase(0, 15);
            //cout << data.date << " " << data.date.length() << endl;
            data.date.erase(2, 11);
            //cout << data.date <<endl;
        }
        else return;

        if (data.date != heure)
        {
            return;
        }
    }

    Key cle (data.target, data.referer);

    std::unordered_map<Key, int, pair_hash>::iterator it;
    if(graph)
    {
        if((it = mesMaps.graphMap.find(cle)) != mesMaps.graphMap.end())
        {
            it->second++;
        }
        else
        {
            mesMaps.graphMap.insert(make_pair(cle, 1));
        }
    }

    std::unordered_map<string, int>::iterator at;

    if((at = mesMaps.unorderedHitMap.find(data.target)) != mesMaps.unorderedHitMap.end())
    {
        at->second++;
    }
    else
    {
        mesMaps.unorderedHitMap.insert(make_pair(data.target, 1));
    }



}


void reverse(mapStruct & mesMaps)
{
    for(auto it = mesMaps.unorderedHitMap.begin(); it!= mesMaps.unorderedHitMap.end(); it++)
    {
        mesMaps.orderedHitMap.insert(make_pair(it->second, it->first));
    }
}



//Grep pulls up 24926 results witt these criteria
//Ours pulls up the same number : everything ok
bool isImage(string url)
{
    int numberOfFormat = 7;
    static const string imageType[] = {".jpg", ".png", ".gif", ".bmp", ".ico", ".js", ".css"};
    for(int i = 0; i< numberOfFormat; ++i)
    {
        if(url.find(imageType[i]) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

void makeGraphFile(mapStruct &mesMaps, string nameFile)
{
    ofstream file(nameFile);
    if(!file || file.fail()){
        cerr << "Could not open file : + " << nameFile << "for writing" << endl;
        exit(EXIT_FAILURE);
    }

    file << "digraph {" << endl;
    auto itEnd = mesMaps.graphMap.end();
    for (auto it = mesMaps.graphMap.begin(); it != itEnd; it++)
    {
        file << "    \"" << it->first.second << "\" -> \"" << it->first.first << "\" [label=\"" << to_string(it->second) << "\"];" << endl;
    }
    file << "}";

    file.close();
}
