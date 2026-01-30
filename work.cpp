#include <iostream>
using namespace std;

int main(){
    
    for(int i = 0; i < 8191;++i){
        cout << 'f' << i << ',';
        if(i % 15 == 0) cout <<'\n';
    }
    
    /*for(int i = 0; i < 8191;++i){
        cout <<"void f" << i << "(){ int i = 32767; int j = 32767; int k = 32767;};\n";
    }
    */
}