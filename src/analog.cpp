#include <unistd.h>
#include <iostream>
#include <map>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <string>
#include "Reader.h"
#include "analog.h"
//#include <boost/functional/hash.hpp>

using namespace std;

//Nom du domaine local
const string localPath = "http://intranet-if.insa-lyon.fr";

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




    //cout << "flag set (g, t, e) : " <<graphMake << " " <<timeSort << " " << excludeFiles << endl;
    fileName = argv[optind];
    Reader logReader(fileName);
    rawData donnee;
    //cout << donnee.ip << " " << donnee.userAgent << " " << donnee.target << " " << donnee.referer << " " << endl;

    int numberOfLineParsed = 0;
    while(!logReader.EndOfFile()){
        donnee = logReader.GetNextLine();
        if (donnee.target != "")
        {
            parseData(donnee, excludeFiles, timeSort, time, graphMake, mesMaps);
            numberOfLineParsed++;
        }
    }

    //cout << "Nombre de ligne lues : " << numberOfLineParsed << endl;
    // Maintenant il faut inverser la map non ordonnee
    reverse(mesMaps);

    //Sortie du top 10
    cout << "TOP 10 : " << endl;
    int i =0;
    for(auto it=mesMaps.orderedHitMap.end(); it!=mesMaps.orderedHitMap.begin() && i<10;  i++)
    {
        it--;
        cout << it->second << " | hit : " << it->first << endl;
    }

    // si -g specifie
    if (graphMake)
    {
        makeGraphFile(mesMaps, graphFile);
    }

    exit(EXIT_SUCCESS);
}


void parseData(rawData & data, bool exclude, bool date, string & heure, bool graph,  mapStruct & mesMaps){
    size_t index;

    //On passe tout en petite lettre
    transform(data.referer.begin(), data.referer.end(), data.referer.begin(),::tolower);
    transform(data.target.begin(), data.target.end(), data.target.begin(),::tolower);

    //On supprime le localPath
    if((index = data.referer.find(localPath)) != std::string::npos){
        data.referer.erase(index, localPath.length());
    }
    //On supprime les PostParameter
    if((index = data.referer.find("?")) != std::string::npos){
        data.referer.erase(index, data.referer.length()-index);
    }
    if((index = data.target.find("?")) != std::string::npos){
        data.target.erase(index, data.target.length()-index);
    }
    //On supprime les "/" de fin d'URL
    if(data.referer.length() > 1 && data.referer.back() == '/'){
        data.referer.erase(data.referer.length() - 1, 1);
    }

    if(data.target.length() > 1 && data.target.back() == '/'){
        data.target.erase(data.target.length() - 1, 1);
    }




    if(exclude && (isExludedFileType(data.referer) ||isExludedFileType(data.target)) ){
        //cout << "found an image" << endl;
        return;
    }

    if(date){
        if (data.date.length() == 26)
        {
            //cout << data.date << " " << data.date.length() << endl;
            data.date.erase(0, 12);
            //cout << data.date << " " << data.date.length() << endl;
            data.date.erase(2, 14);
            //cout << data.date <<endl;
        }
        else return;

        if (data.date != heure)
        {
            return;
        }
    }

    Key cle (data.target, data.referer);
    //On rempli la graphMap seulement si -g est speciefie
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
    // On rempli la map intermediaire au top 10
    if((at = mesMaps.unorderedHitMap.find(data.target)) != mesMaps.unorderedHitMap.end())
    {
        at->second++;
    }
    else
    {
        mesMaps.unorderedHitMap.insert(make_pair(data.target, 1));
    }



}

//Function permettant de passer de la map non ordonnee a la map ordonnee
void reverse(mapStruct & mesMaps)
{
    for(auto it = mesMaps.unorderedHitMap.begin(); it!= mesMaps.unorderedHitMap.end(); it++)
    {
        mesMaps.orderedHitMap.insert(make_pair(it->second, it->first));
    }
}



//Permet de savoir si une extension interdite est dans l'URL
bool isExludedFileType(string url)
{
    int numberOfFormat = 7;
    static const string imageType[] = {".jpg", ".png", ".gif", ".bmp", ".ico", ".js", ".css"};
    for(int i = 0; i< numberOfFormat; ++i)
    {
        unsigned int pos;
        if((pos = url.rfind(imageType[i])) != std::string::npos && pos == (url.length()-imageType[i].length()))
        {
            return true;
        }
    }
    return false;
}

//Permet de construire le graphe
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
