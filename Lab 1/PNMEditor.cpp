//
// Created by Илья Евдокимов on 13.04.2020.
//

#include "PNMEditor.h"
using namespace std;

PNMEditor::PNMEditor(char* input, char* output, char* operation) {
    ifstream fin;
    fin.open(input, ios::binary);
    fin.exceptions(!istream::goodbit);
    if(!fin.is_open()) {
        cerr << "Error while opening file for reading!" << endl;
        exit(1);
    }
    string temp, format;
    int finpos;
    try {
        fin >> format;
        while (true) {
            finpos = fin.tellg();
            getline(fin, temp);
            if (temp[0] != '#') {
                fin.seekg(finpos);
                break;
            }
        fin >> this->width >> this->height >> this->depth;
        }
    }
    catch(exception& e) {
        cerr << "Error while reading the file!" << endl;
        cerr << "Error: " << e.what() << endl;
        exit(1);
    }
    if(format == "P5") this->mult = 1;
    else if(format == "P6") this->mult = 3;
    else {
        cerr << "File format not supported" << endl;
        exit(1);
    }
    this->volume = this->width * this->height * this->mult;
    this->offset = (int)fin.tellg() + 1;
    fin.seekg(this->offset);
    try {
        this->data = new char[this->volume];
    }
    catch(exception& e) {
        cerr << "Failed to allocate memory!" << endl;
        cerr << "Error: " << e.what() << endl;
        exit(1);
    }
    try {
        fin.read(this->data, this->volume);
    }
    catch(exception& e) {
        cerr << "Error while reading the file!" << endl;
        cerr << "Error: " << e.what() << endl;
        exit(1);
    }
    fin.close();
    int oper = atoi(operation);
    switch(oper) {
        case 0:
            Invert();
            break;
        case 1:
            MirrorX();
            break;
        case 2:
            MirrorY();
            break;
        case 3:
            RotateR();
            break;
        case 4:
            RotateL();
            break;
    }
    Export(output, oper);
}

void PNMEditor::Export(char* output, int operation) {
    ofstream fout;
    fout.open(output, ios::binary);
    fout.exceptions(!ostream::goodbit);
    if(!fout.is_open()) {
        cerr << "Error while opening file for writing!" << endl;
        exit(1);
    }
    try {
        fout << (this->mult == 1 ? "P5" : "P6") << endl;
        fout << this->width << ' ' << this->height << endl;
        fout << this->depth << endl;
        if (operation == 3 || operation == 4)
            fout.write(this->rdata, this->volume);
        else
            fout.write(this->data, this->volume);
    }
    catch(exception& e) {
        cerr << "Error while exporting the image!" << endl;
        cerr << "Error: " << e.what() << endl;
    }
    fout.close();
}

void PNMEditor::Invert() {
    for(int i = 0; i < volume; i++) {
        unsigned char temp;
        temp = (unsigned char)data[i];
        data[i] = (char)~temp;
    }
}

void PNMEditor::MirrorX() {
    for(int y = 0; y < this->height; y++)
        for(int x = 0; x < this->width * this->mult / 2; x += this->mult)
            for(int i = 0; i < this->mult; i++)
                swap(this->data[y * this->width * this->mult + x + i], this->data[this->mult * (y * this->width + this->width) - x - this->mult + i]);
}

void PNMEditor::MirrorY() {
    for (int x = 0; x < this->width * this->mult; x++)
        for (int y = 0; y < this->height / 2; y++)
            std::swap(this->data[y * this->width * this->mult + x], this->data[(this->height - 1 - y) * this->width * this->mult + x]);
}

void PNMEditor::RotateR() {
    this->rdata = new char[this->volume];
    int newWidth = this->height;
    int newHeight = this->width;
    for(int y = 0; y < this->height; y++)
        for(int x = 0; x < this->width; x++)
            for(int i = 0; i < this->mult; i++)
                this->rdata[this->mult * (newWidth * x + (newWidth - y - 1)) + i] = this->data[this->mult * (y * this->width + x) + i];
    this->width = newWidth;
    this->height = newHeight;
}

void PNMEditor::RotateL() {
    this->rdata = new char[this->volume];
    int newWidth = this->height;
    int newHeight = this->width;
    for(int y = 0; y < this->height; y++)
        for(int x = 0; x < this->width; x++)
            for(int i = 0; i < this->mult; i++)
                this->rdata[this->mult * (newWidth * (newHeight - 1 - x) + y) + i] = this->data[this->mult * (y * this->width + x) + i];
    this->width = newWidth;
    this->height = newHeight;
}