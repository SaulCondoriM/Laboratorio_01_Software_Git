#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <direct.h>
#include <sys/stat.h>
#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include <deque>
#include <bitset>

using namespace std;

class Platter {
public:
    Platter(int platterNumber)
        : platterNumber(platterNumber) {}

    int getPlatterNumber() const {
        return platterNumber;
    }

    void setPlatterNumber(int platterNumber) {
        this->platterNumber = platterNumber;
    }

private:
    int platterNumber;
};

class Surface {
public:
    Surface(int surfaceNumber, Platter* platter)
        : surfaceNumber(surfaceNumber),
          platter(platter) {}

    int getSurfaceNumber() const {
        return surfaceNumber;
    }
    void setSurfaceNumber(int surfaceNumber) {
        this->surfaceNumber = surfaceNumber;
    }

    Platter* getPlatter() const {
        return platter;
    }

private:
    int surfaceNumber;
    Platter* platter;
};

class Track {
public:
    Track(int trackNumber, Surface* surface)
        : trackNumber(trackNumber),
          surface(surface) {}

    int getTrackNumber() const {
        return trackNumber;
    }

    void setTrackNumber(int trackNumber) {
        this->trackNumber = trackNumber;
    }

    Surface* getSurface() const {
        return surface;
    }

    

private:
    int trackNumber;
    Surface* surface;
};

class Sector {
public:
    Sector(int sectorNumber, Track* track)
        : sectorNumber(sectorNumber),
          track(track) {}

    int getSectorNumber() const {
        return sectorNumber;
    }
    void setSectorNumber(int sectorNumber) {
        this->sectorNumber = sectorNumber;
    }

    Track* getTrack() const {
        return track;
    }

private:
    int sectorNumber;
    Track* track;
};

class Block {
public:
    Block(int blockNumber, Sector* sector)
        : blockNumber(blockNumber),
          sector(sector) {}

    int getBlockNumber() const {
        return blockNumber;
    }

    Sector* getSector() const {
        return sector;
    }

private:
    int blockNumber;
    Sector* sector;
};



class File {
public:
    File(const char* inputCsv, const char* outputFilename)
        : outputFilename(outputFilename) {
        generateFormattedFile(inputCsv, outputFilename);
    }

    void printAll() {
        printRecord(-1);
    }

    void printRecord(int recordNumber) {
        ifstream file(outputFilename);

        if (!file.is_open()) {
            cerr << "Error al abrir el archivo." << endl;
            return;
        }

        char line[256];
        int lineNumber = 1;

        while (file.getline(line, sizeof(line))) {
            if (recordNumber == -1 || lineNumber == recordNumber) {
                cout << line << endl;
            }

            lineNumber++;
        }

        file.close();
    }

private:
    const char* outputFilename;

    void generateFormattedFile(const char* inputCsv, const char* outputFilename) {
        ifstream inputFile(inputCsv);
        ofstream outputFile(outputFilename);

        if (!inputFile.is_open() || !outputFile.is_open()) {
            cerr << "Error al abrir los archivos." << endl;
            return;
        }

        char line[256];
        inputFile.getline(line, sizeof(line)); // Ignorar la primera línea (encabezados)

        while (inputFile.getline(line, sizeof(line))) {
            istringstream lineStream(line);
            char field[256];

            lineStream.getline(field, sizeof(field), ',');
            outputFile << field << setw(8) << ""; // Index

            lineStream.getline(field, sizeof(field), ',');
            outputFile << setw(12) << field; // Customer Id

            lineStream.getline(field, sizeof(field), ',');
            outputFile << setw(20) << field; // First Name

            lineStream.getline(field, sizeof(field), ',');
            outputFile << setw(20) << field; // Last Name

            lineStream.getline(field, sizeof(field), ',');
            outputFile << setw(30) << field; // Company

            lineStream.getline(field, sizeof(field), ',');
            outputFile << setw(20) << field; // City

            lineStream.getline(field, sizeof(field), ',');
            outputFile << setw(20) << field; // Country

            lineStream.getline(field, sizeof(field), ',');
            outputFile << setw(15) << field; // Phone 1

            lineStream.getline(field, sizeof(field), ',');
            outputFile << setw(15) << field; // Phone 2

            lineStream.getline(field, sizeof(field), ',');
            outputFile << setw(30) << field; // Email

            lineStream.getline(field, sizeof(field), ',');
            outputFile << setw(25) << field; // Subscription Date

            lineStream.getline(field, sizeof(field), ',');
            outputFile << setw(20) << field; // Website

            outputFile << endl;
        }

        inputFile.close();
        outputFile.close();
    }




};

class Disco {
public:
    Disco(int numeroDePlatos, int numeroDePistasPorPlato, int numeroDeSectoresPorPista, int numeroDeSectoresPorBloque, int numeroDeRegistrosPorSector, int tamanioDelSector)
        : numeroDePlatos(numeroDePlatos),
          numeroDePistasPorPlato(numeroDePistasPorPlato),
          numeroDeSectoresPorPista(numeroDeSectoresPorPista),
          numeroDeSectoresPorBloque(numeroDeSectoresPorBloque),
          numeroDeRegistrosPorSector(numeroDeRegistrosPorSector),
          tamanioDelSector(tamanioDelSector) {
    }

    long tamanoDelArchivo(const char* filename) {
        ifstream file(filename, ifstream::ate | ifstream::binary);
        return file.tellg();
    }

    long capacidadTotalDelDisco() {
        int tamanoDelRegistro = 4 + 1 + 4 + 100 + 10 + 5 + 4 + 4 + 25 + 15 + 20 + 1; // Suma del tamaño de cada campo
        int tamanoDelBloque = tamanoDelRegistro * numeroDeRegistrosPorSector * numeroDeSectoresPorBloque;
        int numeroTotalDeBloques = numeroDePlatos * 2 * numeroDePistasPorPlato * numeroDeSectoresPorPista / numeroDeSectoresPorBloque;
        return tamanoDelBloque * numeroTotalDeBloques;
    }
    

    void dividirSectores(const char* inputFilename) {
        ifstream inputFile(inputFilename);
        if (!inputFile.is_open()) {
            cerr << "Error al abrir el archivo." << endl;
            return;
        }

        mkdir("sectores"); // Crear directorio si no existe

        int sectorNumber = 1;
        string line;

        while (inputFile.good()) {
            ofstream outputFile("sectores/" + to_string(sectorNumber) + ".txt");

            if (!outputFile.is_open()) {
                cerr << "Error al abrir el archivo de sector." << endl;
                return;
            }

            for (int i = 0; i < numeroDeRegistrosPorSector && getline(inputFile, line); ++i) {
                outputFile << line << endl;
            }

            outputFile.close();
            sectorNumber++;
        }

        inputFile.close();
    }
    void imprimirSector(int sectorNumber) {
        ifstream inputFile("sectores/" + to_string(sectorNumber) + ".txt");

        if (!inputFile.is_open()) {
            cerr << "Error al abrir el archivo de sector." << endl;
            return;
        }

        string line;
        while (getline(inputFile, line)) {
            stringstream lineStream(line);

            while (lineStream.good()) {
                string field;
                getline(lineStream, field, ' ');

                if (!field.empty()) {
                    cout << field << ' ';
                }
            }

            cout << endl;
        }

        inputFile.close();
    }


    void imprimirBloque(int bloqueNumber) {
        for (int i = 0; i < numeroDeSectoresPorBloque; i++) {
            int sectorNumber = (bloqueNumber - 1) * numeroDeSectoresPorBloque + i + 1;
            imprimirSector(sectorNumber);
        }
    }


    void imprimirMetadataDelRegistro(const char* outputFilename, int recordNumber) {
    ifstream file(outputFilename);

    if (!file.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return;
    }

    char line[512];
    int lineNumber = 1;

    Platter* platter = new Platter(1);
    Surface* surface = new Surface(1, platter);
    Track* track = new Track(1, surface);
    Sector* sector = new Sector(1, track);
    Block* block = new Block(1, sector);

    int registrosLeidosEnSectorActual = 0;

    while (file.getline(line, sizeof(line))) {
        if (lineNumber == recordNumber) {
            cout << "Registro encontrado en:" << endl;
            cout << "Pista: " << block->getSector()->getTrack()->getTrackNumber() << endl;
            cout << "Plato: " << block->getSector()->getTrack()->getSurface()->getPlatter()->getPlatterNumber() << endl;
            cout << "Superficie: " << block->getSector()->getTrack()->getSurface()->getSurfaceNumber() << endl;
            cout << "Sector: " << block->getSector()->getSectorNumber() << endl;
            break;
        }

        lineNumber++;
        registrosLeidosEnSectorActual++;

        if (registrosLeidosEnSectorActual >= numeroDeRegistrosPorSector) {
            registrosLeidosEnSectorActual = 0;
            block->getSector()->setSectorNumber(block->getSector()->getSectorNumber() + 1);
            if (block->getSector()->getSectorNumber() > numeroDeSectoresPorPista) {
                block->getSector()->setSectorNumber(1);
                block->getSector()->getTrack()->getSurface()->setSurfaceNumber(block->getSector()->getTrack()->getSurface()->getSurfaceNumber() == 1 ? 2 : 1);
                if (block->getSector()->getTrack()->getSurface()->getSurfaceNumber() == 1) {
                    block->getSector()->getTrack()->setTrackNumber(block->getSector()->getTrack()->getTrackNumber() + 1);
                    if (block->getSector()->getTrack()->getTrackNumber() > numeroDePistasPorPlato) {
                        block->getSector()->getTrack()->setTrackNumber(1);
                        block->getSector()->getTrack()->getSurface()->getPlatter()->setPlatterNumber(block->getSector()->getTrack()->getSurface()->getPlatter()->getPlatterNumber() + 1);
                        if (block->getSector()->getTrack()->getSurface()->getPlatter()->getPlatterNumber() > numeroDePlatos) {
                            block->getSector()->getTrack()->getSurface()->getPlatter()->setPlatterNumber(1);
                        }
                    }
                }
            }
        }

        
    }

    if (lineNumber < recordNumber) {
        cerr << "Registro no encontrado." << endl;
    }

    file.close();

    
    delete block;
    delete sector;
    delete track;
    delete surface;
    delete platter;
}

private:
    int numeroDePlatos;
    int numeroDePistasPorPlato;
    int numeroDeSectoresPorPista;
    int numeroDeSectoresPorBloque;
    int numeroDeRegistrosPorSector;
    int tamanioDelSector;

    const char* outputFilename;
};

struct PageTableEntry {
    int frameId;
    int pageId;
    bool dirtyBit;
    int pinCount;
};


class BufferManager {
public:
    BufferManager(Disco* disco, int poolSize)
        : disco(disco), poolSize(poolSize), sectoresPorBloque(4), registrosPorSector(10) {
            clockPointer = bufferQueue.begin();
    }

    void moverRegistro(int sourcePage, int sourceIndex, int destPage, int destIndex) {
        string registro = leerRegistro(sourcePage, sourceIndex);
        if (!registro.empty()) {
            escribirRegistro(destPage, destIndex, registro);
            eliminarRegistro(sourcePage, sourceIndex);
        }
    }

    void eliminarRegistro(int page, int index) {
        string registroVacio = "";
        escribirRegistro(page, index, registroVacio);
    }

    void insertarRegistro(int page, int index, const string& registro) {
        escribirRegistro(page, index, registro);

    }

    void printPage(int page) {
    cargarPaginaEnBuffer(page);
    auto& registros = bufferPool[page];
    for (const auto& registro : registros) {
        cout << registro << endl;
    }
    
}


private:
    Disco* disco;
    int poolSize;
    int sectoresPorBloque;
    int registrosPorSector;

    int frameIdCounter;

    unordered_map<int, vector<string>> bufferPool;

    unordered_map<int, PageTableEntry> pageTable;


    deque<int> bufferQueue;
    unordered_map<int, bool> referenceBits;
    deque<int>::iterator clockPointer;

    string leerRegistro(int page, int index) {
        cargarPaginaEnBuffer(page);
        auto& registros = bufferPool[page];
        if (index >= 0 && index < registros.size()) {
            return registros[index];
        }
        return "";
    }

    void escribirRegistro(int page, int index, const string& registro) {
        cargarPaginaEnBuffer(page);
        
        auto& registros = bufferPool[page];
        if (index >= 0 && index < registros.size()) {
            registros[index] = registro;
            guardarPaginaEnDisco(page);
        }

        for (auto& pair : pageTable) {
            if (pair.second.pageId == page) {
                pair.second.dirtyBit = true; // marca la página como dirty
            break;
        }
    }

    }

    void cargarPaginaEnBuffer(int page) {
    if (bufferPool.find(page) == bufferPool.end()) {
        // El buffer está lleno, necesitamos reemplazar una página.
        if (bufferPool.size() >= poolSize) {
           // Bucle para encontrar una página con bit de referencia 0.
            while (true) {
                // Si el bit de referencia es 0, reemplazamos la página.
                if (!referenceBits[*clockPointer]) {
                    bufferPool.erase(*clockPointer);
                    bufferQueue.erase(clockPointer);
                    break;
                }
                // Si el bit de referencia es 1, lo ponemos en 0 y avanzamos el puntero.
                referenceBits[*clockPointer] = 0;
                ++clockPointer;
                // Si hemos alcanzado el final del bufferQueue, volvemos al comienzo.
                if (clockPointer == bufferQueue.end()) {
                    clockPointer = bufferQueue.begin();
                }
            }
        }

        // Cargamos la nueva página en el bufferPool.
        vector<string> registros;
        for (int i = 0; i < sectoresPorBloque; i++) {
            int sectorNumber = (page - 1) * sectoresPorBloque + i + 1;
            ifstream inputFile("sectores/" + to_string(sectorNumber) + ".txt");

            if (!inputFile.is_open()) {
                cerr << "Error al abrir el archivo de sector." << endl;
                return;
            }

            string line;
            while (getline(inputFile, line)) {
                registros.push_back(line);
            }

            inputFile.close();
        }

        PageTableEntry entry {page, false, 0}; // crea una nueva entrada con dirtyBit = false y pinCount = 0
        pageTable[frameIdCounter++] = entry;  // inserta la entrada en la tabla de páginas

        for (auto& pair : pageTable) {
            if (pair.second.pageId == page) {
                pair.second.pinCount++; // incrementa el pinCount
                break;
            }
        }

        // Añadimos la página al bufferPool y ponemos su bit de referencia en 1.
        bufferPool[page] = registros;
        bufferQueue.push_back(page);
        referenceBits[page] = 1;
        // Si este es el primer elemento, inicializamos el puntero del reloj.
        if (bufferQueue.size() == 1) {
            clockPointer = bufferQueue.begin();
        }
    } else {
        // Si la página ya está en el buffer, ponemos su bit de referencia en 1.
        referenceBits[page] = 1;
    }
}


    void guardarPaginaEnDisco(int page) {
        auto& registros = bufferPool[page];
        auto it = registros.begin();
        for (int i = 0; i < sectoresPorBloque; i++) {
            int sectorNumber = (page - 1) * sectoresPorBloque + i + 1;
            ofstream outputFile("sectores/" + to_string(sectorNumber) + ".txt");

            if (!outputFile.is_open()) {
                cerr << "Error al abrir el archivo de sector." << endl;
                return;
            }

            for (int j = 0; j < registrosPorSector && it != registros.end(); ++j, ++it) {
                outputFile << *it << endl;
            }

            outputFile.close();
        }
    }
};




int main() {
    
    File file("customers-100000.csv", "file");


    Disco disco(5, 10000, 100, 4, 5000, 512);
    disco.dividirSectores("file");


    BufferManager bufferManager(&disco, 5);

    
    
    int opcion, num, page1, page2, index1, index2;
    string registro;

    do
    {
        cout<<"1. Imprimir file"<<endl;
        cout<<"2. Imprimir un registro"<<endl;
        cout<<"3. Imprimir metadata del registro"<<endl;
        cout<<"4. Tamanio del file"<<endl;
        cout<<"5. Tamanio del disco"<<endl;
        cout<<"6. Imprimir un sector"<<endl;
        cout<<"7. Imprimir un bloque"<<endl;

        cout<<"8. Agregar registro"<<endl;
        cout<<"9. Mover registros"<<endl;
        cout<<"10. Eliminar registro"<<endl;
        cout<<"11. Imprimir pagina"<<endl;

        cout<<"12. Salir"<<endl;       
        cout<<"Coloque la opcion que desee: ";
        cin>>opcion;


        switch (opcion)
        {
        case 1: file.printAll(); break;

        case 2: {
            cout << "Numero de registro: "; 
            cin >> num;
            file.printRecord(num);
        } break;

        case 3: {
            cout<<"Informacion del registro: "; cin>>num;
            disco.imprimirMetadataDelRegistro("file", num);
        } break;

        case 4: cout << "Tamanio del archivo 'file': " << disco.tamanoDelArchivo("file") << " bytes" << endl; break;

        case 5:  cout << "Capacidad total del disco: " << disco.capacidadTotalDelDisco() << " bytes" << endl; break;

        case 6: {
            cout<<"Nombre de sector: "; cin>>num;
            cout<<"Imprimiendo sector: "<<endl;
            disco.imprimirSector(num);
        } break;

        case 7: {
            cout<<"Nombre de bloque: "; cin>>num;
            cout<<"Imprimiendo bloque: "<<endl;
            disco.imprimirBloque(num);
        } break;

        case 8: {
            cout<<"Pagina: "; cin>>page1;
            cout<<"Indice: "; cin>>index1;
            cout<<"Registro Entero:" ; cin>>registro;
            bufferManager.insertarRegistro(page1, index1, registro);
        } break;
        case 9: {
            cout<<"Pagina desde: "; cin>>page1;
            cout<<"Indice desde: "; cin>>index1;
            cout<<"Pagina hasta: "; cin>>page2;
            cout<<"Indice hasta: "; cin>>index2;
            bufferManager.moverRegistro(page1, index1, page2, index2);
        } break;
        case 10:{
            cout<<"Pagina: "; cin>>page1;
            cout<<"Indice: "; cin>>index1;
            bufferManager.eliminarRegistro(page1, index1);
        } break;
        case 11:{
            cout<<"Pagina: "; cin>>page1;
            bufferManager.printPage(page1);
        } break;

        case 12: break;
        default:
            cout<<"No es una opcion valida"<<endl;
            break;
        }

    } while (opcion != 12);
    
    

    

   

    return 0;
}
