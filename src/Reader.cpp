/*************************************************************************
                           Reader  -  description
                             -------------------
    début                : $DATE$
    copyright            : (C) $YEAR$ par $AUTHOR$
    e-mail               : $EMAIL$
*************************************************************************/

//---------- Réalisation de la classe <Reader> (fichier Reader.cpp) ------------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>
#include <fstream>

//------------------------------------------------------ Include personnel
#include "Reader.h"

//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
// type Reader::Méthode ( liste des paramètres )
// Algorithme :
//
//{
//} //----- Fin de Méthode


rawData Reader::GetNextLine()
{
    rawData data;
    string placeHolder;
    getline(fileStream, data.ip, ' ');
    getline(fileStream, data.logName, ' ');
    getline(fileStream, data.userName, ' ');
    getline(fileStream, data.date, ' ');
    getline(fileStream, placeHolder, '"')
    getline(fileStream, data.method, ' ');
    getline(fileStream, data.target, ' ');
    getline(fileStream, data.httpVersion, '"');
    getline(fileStream, placeHolder, ' ');
    getline(fileStream, data.code, ' ');
    getline(fileStream, data.dataQuantity, ' ');
    getline(fileStream, placeHolder, '"');
    getline(fileStream, data.referer, '"');
    getline(fileStream, placeHolder, '"');
    getline(fileStream, data.userAgent, '"');

    //permet de placer le fichier en bonne position pour recuperer la lligne suivante
    getline(fileStream, placeHolder);

    return data;

}


//------------------------------------------------- Surcharge d'opérateurs
Reader & Reader::operator = ( const Reader & unReader )
// Algorithme :
//
{
} //----- Fin de operator =


//-------------------------------------------- Constructeurs - destructeur
Reader::Reader ( const Reader & unReader )
// Algorithme :
//
{
#ifdef MAP
    cout << "Appel au constructeur de copie de <Reader>" << endl;
#endif
} //----- Fin de Reader (constructeur de copie)


Reader::Reader ( string logFile )
// Algorithme :
//
{
#ifdef MAP
    cout << "Appel au constructeur de <Reader>" << endl;
#endif
    fileName = logFile;
    fileStream.open(fileName);

} //----- Fin de Reader


Reader::~Reader ( )
// Algorithme :
//
{
#ifdef MAP
    cout << "Appel au destructeur de <Reader>" << endl;
#endif
} //----- Fin de ~Reader


//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées
