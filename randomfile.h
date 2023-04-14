//
// Created by VIRGINIA on 13/04/2023.
//

#ifndef BD2_LAB2_RANDOMFILE_H
#define BD2_LAB2_RANDOMFILE_H

#include <iostream>
#include <fstream>
#include <map>
using namespace std;

void readFromConsole(char buffer[], int size) {
    string temp;
    cin >> temp;
    for (int i = 0; i < size; i++)
        buffer[i] = (i < temp.size()) ? temp[i] : ' ';
    buffer[size - 1] = '\0';
    cin.clear();
}

struct Record
{
    char nombre[30];
    char carrera[20];
    int ciclo;
    void setData(){
        cout<<"Nombre: ";
        readFromConsole(nombre, 30);
        cout<<"Carrera: ";
        readFromConsole(carrera, 20);
        cout<<"Ciclo: ";
        cin>>ciclo;
        cin.ignore();
    }

    string getKey(){
        return nombre;
    }
};

ostream &operator<<(ostream &stream, Record &p) {
    stream.write((char *) &p, sizeof(p));
    stream << flush;
    return stream;
}

istream &operator>>(istream &stream, Record &p) {
    stream.read((char *) &p, sizeof(p));
    return stream;
}

int size_file(string filename){
    ifstream file(filename, ios::binary);
    file.seekg(0, ios::end);
    int size = file.tellg();
    file.close();
    return size;
}

class RandomFile{
private:
    string filename;
    string indexname;
    map<string, long> index;
public:
    RandomFile(string filename){
        this->filename = filename;
        this->indexname = "../dataind.dat";
        // read index
        ifstream file(filename, ios::binary);
        int pos = 1;
        if (file.is_open()) {
            Record record;
            while (file >> record) {
                index[record.getKey()] = pos;
                //cout<<record.getKey()<<endl;
                pos++;
            }
            file.close();
        } else {
            cerr << "No se pudo abrir el archivo\n";
        }
    }

    void write(Record data){
        ofstream file;
        file.open(filename, ios::binary | ios::app );
        if (file.is_open()){
            file << data;
            file.close();
        } else {
            cerr << "No se pudo abrir el archivo\n";
        }
        index[data.getKey()] = size_file(filename)/ sizeof(Record);
    }

    void scanAll(){
        ifstream file;
        file.open(filename, ios::binary);
        if (file.is_open()) {
            Record record;
            while (file >> record) {
                cout << record.nombre << record.carrera << record.ciclo << endl;
            }
            file.close();
        } else {
            cerr << "No se pudo abrir el archivo\n";
        }
    }

    //recorrido en O(n) usando el index
    void scanAllByIndex(){
        ifstream file(filename, ios::binary );
        Record record;
        for(auto it = index.begin(); it != index.end(); it++){
            //cout<< it -> first << " " << it -> second << endl;
            long pos = it->second - 1;
            file.seekg(pos * sizeof(Record));
            file >> record;
            cout << record.nombre << record.carrera << record.ciclo << endl;
        }
        file.close();
    }

    ~RandomFile() {
        // write index
        if (size_file(indexname) == 0) {
            ofstream file(indexname, ios::binary | ios::app);
            for (auto it = index.begin(); it != index.end(); it++) {
                file << it->first << it->second;
            }
            file.close();
        }
        index.clear();
    }
};
#endif //BD2_LAB2_RANDOMFILE_H
