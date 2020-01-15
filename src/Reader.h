/*************************************************************************
                           Reader  -  description
                             -------------------
    début                : $DATE$
    copyright            : (C) $YEAR$ par $AUTHOR$
    e-mail               : $EMAIL$
*************************************************************************/

//---------- Interface de la classe <Reader> (fichier Reader.h) ----------------
#if ! defined ( READER_H )
#define READER_H
#include <fstream>
//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types
typedef struct rawData
{
    std::string ip;
    std::string logName;
    std::string userName;
    std::string date;
    std::string method;
    std::string target;
    std::string httpVersion;
    std::string code;
    std::string dataQuantity;
    std::string referer;
    std::string userAgent;
} rawData;
//------------------------------------------------------------------------
// Rôle de la classe <Reader>
//
//
//------------------------------------------------------------------------

class Reader
{
//----------------------------------------------------------------- PUBLIC

public:
    //----------------------------------------------------- Méthodes publiques
    // type Méthode ( liste des paramètres );
    // Mode d'emploi :
    //
    // Contrat :
    //
    rawData GetNextLine();

    //------------------------------------------------- Surcharge d'opérateurs
    Reader & operator = ( const Reader & unReader );
    // Mode d'emploi :
    //
    // Contrat :
    //


    //-------------------------------------------- Constructeurs - destructeur
    Reader ( const Reader & unReader );
    // Mode d'emploi (constructeur de copie) :
    //
    // Contrat :
    //

    Reader (std::string logFile);
    // Mode d'emploi :
    //
    // Contrat :
    //

    virtual ~Reader ( );
    // Mode d'emploi :
    //
    // Contrat :
    //

//------------------------------------------------------------------ PRIVE

protected:
//----------------------------------------------------- Méthodes protégées

//----------------------------------------------------- Attributs protégés
    std::string fileName;
    std::ofstream fileStream;

};

//-------------------------------- Autres définitions dépendantes de <Reader>

#endif // Reader_H
