//
// Created by Илья Евдокимов on 13.04.2020.
//

#ifndef LAB1_PNMEDITOR_H
#define LAB1_PNMEDITOR_H
#include <iostream>
#include <fstream>

class PNMEditor {
public:
    char* data;
    char* rdata;
    int width, height, depth, mult, offset, volume;

    PNMEditor(char* input, char* output, char* operation);

    void Export(char* output, int operation);

    void Invert();
    void MirrorX();
    void MirrorY();
    void RotateR();
    void RotateL();

};


#endif //LAB1_PNMEDITOR_H
