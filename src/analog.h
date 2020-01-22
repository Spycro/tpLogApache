#if ! defined ( ANALOG_H )
#define ANALOG_H


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

typedef std::pair<std::string, std::string> Key; // pair de (target, ref)


typedef struct mapStruct{
	std::unordered_map<Key, int, pair_hash> graphMap;
	std::unordered_map<std::string, int> unorderedHitMap;
	std::multimap<int, std::string> orderedHitMap;
} mapStruct;

void parseData(rawData & data, bool exclude, bool date, std::string & heure, bool graph, mapStruct & mesMaps);
bool isExludedFileType(std::string url);
void makeGraphFile(mapStruct &mesMaps, std::string nameFile);
void reverse(mapStruct & mesMaps);


#endif
