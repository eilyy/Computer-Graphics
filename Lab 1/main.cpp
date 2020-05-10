#include "PNMEditor.h"
using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 4) {
        cerr << "Exactly 4 arguments were expected!";
        exit(1);
    }
    auto* pnm = new PNMEditor(argv[1], argv[2], argv[3]);
    return 0;
}