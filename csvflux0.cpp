#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <set>
#include <math.h>
#include <vector>
using namespace std;

#define kCategoryLine 5
#define kFirstLine 7
#define kMaxInput 32 
#define kMaxLine 512


class FileReader
{
public:
	 FileReader(char filename[]) {
	 	fp.open(filename, ios::in);
	 	string line;
	 	getOneDiff();
	 	for (int i = 0; i <= kCategoryLine; i++) {
	 		getline(fp, line);
	 	}
	 	begin = fp.tellg();
	 	createCategories(line);
	 	getline(fp, line);
	 	findDataCategories(line);
	 	getline(fp, line);
	 	createColumns(line);
	 }


	~ FileReader() { fp.close(); }

struct DataCategory
	{
		string name;
		string category;
		streampos startPos;
		int skipCommas;
		// streampos endPos;
		streampos columnStart;
		streampos columnEnd;
	};

	void getCategory(string category) {
		if (categoryNames.find(category) == categoryNames.end()) {
			cout << "Sorry, category not found." << endl;
			return;
		}
		for (int i = 0; i < subCategories.size(); i++) {
			if (subCategories[i].category == category) {
				getColumn(subCategories[i]);
			}
		}
	}

	void getName(string name) {
		for (int i = 0; i < subCategories.size(); i++) {
			if (subCategories[i].name == name) {
				getColumn(subCategories[i]);
				return;
			}
		}
		cout << "Sorry, data not found. Consider retyping column name" << endl;
	}

	void getAllData() {
		for (int i  = 0; i < subCategories.size(); i++) {
			getColumn(subCategories[i]);
		}
	}


	void getColumn(DataCategory dc) {
		string line;
		char cline[kMaxLine];
		fp.clear();
		fp.seekg(dc.startPos);
		while (getline(fp, line)) {

			fp.read(cline, dc.columnStart - oneDiff);
			fp.read(cline, dc.columnEnd - dc.columnStart);
			cout << cline << endl;
			// getline(fp, line);
		}
	}

	bool hasCat(string str) {
		return categoryNames.find(str) != categoryNames.end();
	}


private:

	
	
	fstream fp;
	vector<DataCategory> subCategories;
	char startDate;
	vector<string> catNames;
	set<string> categoryNames;
	streampos oneDiff;
	streampos begin;


	void getOneDiff() {
		char c;
	 	streampos a, b;
	 	a = fp.tellg();
	 	fp.get(c);
	 	b = fp.tellg();
	 	oneDiff = a - b;
	 	fp.clear();
	}

	void createCategories(string line) {
		size_t prev = 0, curr = 0; //change type to streampos for consistancy
		while (curr < line.length()) {
			curr = line.find(',');
			//set--> no duplicates UNLESS lower/upper mistakes or typos
			categoryNames.insert(line.substr(prev, curr - prev));
			prev = curr;
		}
	}

	void findDataCategories(string line) {
		streampos prev = 0, curr = 0;
		int i = 0;
		string currStr;
		while (curr < line.length()) {
			curr = line.find(',');
			currStr = line.substr(prev, curr - prev);
			subCategories[i].name = currStr;
			string findCat;
			for (int j = 0; j < currStr.length(); j++) {
				if (categoryNames.find(currStr.substr(0, j)) != categoryNames.end()) {
					subCategories[i].category = currStr.substr(0, j);
				}
			}
			i++;
		}
	}


	void createColumns(string line) {
		streampos prev = 0, curr = 0;
		vector<streampos> allPositions;
		int index = 0;
		while (curr < line.length()) {
			allPositions[index++] = fp.tellg();
			curr = line.find(',');
			curr = line.find(',');
			allPositions[index++] = prev;
			allPositions[index++] = curr - oneDiff;
			prev = curr + oneDiff;
		}
		for (int i = 0; i < index; i++) {
			int subIndex = floor(i/3);
			subCategories[subIndex].startPos = allPositions[i++];
			subCategories[subIndex].columnStart = allPositions[i++];
			subCategories[subIndex].columnEnd = allPositions[i];
		}
	}




};

string sToLower(string str) {
	for (int i = 0; i < str.length(); i++) {
		tolower(str[i]);
	}
	return str;
}


int main() {
	char filename[kMaxInput];
	cout << "Please enter the filename: ";
	cin.getline(filename, kMaxInput);
	FileReader reader(filename);
	char dataCategory[kMaxInput];
	cout << "Name a category or column: " << endl;
	cin.getline(dataCategory, kMaxInput);
	string dataCat2 = sToLower(dataCategory);
	if (dataCat2 == "all") reader.getAllData();
	else if (reader.hasCat(dataCat2)) reader.getCategory(dataCat2);
	else reader.getName(dataCat2);
}