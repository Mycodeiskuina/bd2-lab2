//
// Created by VIRGINIA on 13/04/2023.
//

#include "randomfile.h"

void writeFile(string filename){
	RandomFile file(filename);
	Record record;
	for(int i=0;i<3;i++){
		record.setData();
		file.write(record);
	}
}

void readFile(string filename)
{
	RandomFile file(filename);
	cout<<"-------- show data --- \n";
	file.scanAll();
	cout<<"-------- show sorted data --- \n";
	file.scanAllByIndex();
}

int main(){
	writeFile("../data.dat");
	readFile("../data.dat");
}