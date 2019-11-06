#include <fstream>
#include <iostream>

using namespace std;

int main() {
    ifstream fin("coco_labels_list.txt");
    ofstream fout("label_output.txt");
    int n = 0;
    while( !fin.eof() ) {
        string str; fin >> str;
        fout << "item {" << endl;
        fout << "  id: " << n++ << endl;
        fout << "  name: '" << str << "'" << endl;
        fout << "}" << endl;
        //cout << str << endl;
    }
}
