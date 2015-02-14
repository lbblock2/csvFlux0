#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <string.h>
#include <set>
#include <math.h>
#include <vector>
using namespace std;

#define kMaxInput 32
#define kMaxCols 16
#define kCatLine 3
#define kDataLine 6
#define kColWidth 2


class FileReader
{
public:
	 FileReader(char filename[]) : numcols(1), numLines(0) {
	 	fp.open(filename, ios::in);
	 	if(!fp) {
	 		cout << "sorry, file not found" << endl;
			return;
	 	}
	 	//add to filename four outfile
	 	strcat(filename, "out");
	 	outfile.open(filename, ios::out);
	 	getNumCols();
	 	// printFile();
	 	skipLine();
	 	createCategories();
	 	DataColumn dc;
	 	dc.nColumnsSkipped = 2;
	 	createColumns();
	 	getColumn(columns[3]);
	 }

	~ FileReader() {
		outfile.seekp(ios::beg);
		outfile << numLines << endl;
		outfile.close();
		fp.close();
	}

	struct DataColumn
	{
		int nColumnsSkipped;
		string name;
		string category;
	};

	void getColumn(DataColumn dc) {
		string line;
		for(int i = 0; i < kDataLine; i++) skipLine();
		while(!fp.eof()) {
			for (int i = 0; i < dc.nColumnsSkipped; i++) getline(fp, line, ',');
			for(int i = 0; i < kColWidth; i++) {
				getline(fp, line, ',');
				outfile << line << "    ";
				cout << line << "     ";
			}
			outfile << endl;
			numLines++;
			cout << endl;
			for (int i = 0; i < (numcols - kColWidth - dc.nColumnsSkipped); i++) getline(fp, line, ',');
		}

	}


	void printFile() {
		string line;
		int counter = 0;
	 	while(!fp.eof()){
	 		getline(fp, line, ',');
	 		cout << line;
	 		if (counter % numcols == 0) cout << endl;
	 		if(counter++ > 488 * numcols) break;
	 	}
	}


private:
	vector<int> columnCommas;
	int numLines;
	vector<DataColumn> columns;
	set<string> categoryNames;
	int numcols;

	//map<string, DataColumn> columnMap;
	fstream fp;
	fstream outfile;

	struct Category
	{
		string name;
		vector<DataColumn> catColumns;
		
	};

	set<Category> allCats;

	void skipLine() {
		string tempLine;
		for (int i = 0; i < numcols; i++) {
			getline(fp, tempLine, ',');
		}
	}

	void getNumCols() {
		numcols = 0;
	 	string line;
	 	getline(fp, line, ',');
	 	while (line.find("Fin") == string::npos) {
	 		getline(fp, line, ',');
	 		if (numcols++ > kMaxCols) break;
	 	}
	}

	void createCategories() {
		string line;
		fp.clear();
		fp.seekg(ios::beg);
		for (int i = 0; i < kCatLine; i++) skipLine();
		for(int i = 0; i < numcols; i++) {
			getline(fp, line, ',');
			// cout << line << endl;
			if (line.length() > 0) {
				if(categoryNames.find(line) == categoryNames.end()) categoryNames.insert(line);
				else continue;
				cout << line << endl;
			}
		}
	}

	void createColumns() {
		string line;
		fp.clear();
		fp.seekg(ios::beg);
		for (int i = 0; i < kDataLine - 2; i++) skipLine();
		for (int i = 0; i < numcols + 1; i++) {
			DataColumn datac;
			getline(fp, line, ',');
			datac.name = line;
			datac.nColumnsSkipped = i-1;
			i++;
			columns.push_back(datac);
			getline(fp, line, ',');
		}
	}


};


int main() {
	char filename[kMaxInput];
	cout << "Filename: ";
	cin.getline(filename, kMaxInput);
	FileReader reader(filename);
}