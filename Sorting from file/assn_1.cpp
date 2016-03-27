#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <cstdlib>
using namespace std;

int *readBinaryToArray(string filePath) {
	// read integers from the file
	ifstream inFile;
	inFile.open(filePath.c_str(), ios::binary | ios::in);
	if (inFile.is_open()) {
		inFile.seekg(0, ios::end);
		int len = (int) (inFile.tellg());
		inFile.seekg(0, inFile.beg);
		int count = len / sizeof(int);
		int * arr = new int[count];
		for (int i = 0; i < count; i++) {
			inFile.read(reinterpret_cast<char*> (&arr[i]), sizeof(int));
		}
		inFile.close();
		return arr;
	}
	else if (inFile.fail()) {
		cerr << "unable to open the file: " << filePath << endl;
		exit(1);
	}
	return 0;
}

int getDataCount(string filePath) {
	ifstream inFile;
	int count = 0;
	inFile.open(filePath.c_str(), ios::binary | ios::in);
	if (inFile.is_open()) {
		inFile.seekg(0, ios::end);
		int len = (int) (inFile.tellg());
		inFile.seekg(0, inFile.beg);
		count = len / sizeof(int);
		inFile.close();
	}
	return count;
}

bool linearSearch(int s, int *K, int keyCnt) {
	for (int k = 0; k < keyCnt; k++) {
		if (K[k] == s) {
			return true;
		}
		else {continue;}
	}
	return false;
}

void inMemoryLinearSearch(string keyFilePath, string seekFilePath) {
	int *S = readBinaryToArray(seekFilePath);
	int seekListLength = getDataCount(seekFilePath);
	// start timer
	clock_t start, end;
	start = clock();
	int *K = readBinaryToArray(keyFilePath);
	int keyListLength = getDataCount(keyFilePath);
	int *hit = new int[seekListLength];
	ofstream out("mem-lin-output.txt");
	for (int i = 0; i < seekListLength; i++) {
		bool isFound = linearSearch(S[i], K, keyListLength);
		isFound ? hit[i] = 1 : hit[i] = 0;
	}
	//end timer
	end = clock();
	for (int j = 0; j < seekListLength; j++) {
		string isPresent = (hit[j]==1) ? "Yes" : "No";
		cout << setfill(' ') << setw(12) << S[j] << ": " << isPresent << endl;
		// writing output to a file
		out << setfill(' ') << setw(12) << S[j] << ": " << isPresent << endl;
	}
	cout << "Time: " << double(end - start)/CLOCKS_PER_SEC;
	// writing output to a file
	out << "Time: " << double(end - start) / CLOCKS_PER_SEC;
	delete[] S, K, hit;
	out.close();
}

int binarySearch(int s, int *K, int l, int r) {
	int n = r - l + 1;
	if (n <= 0) { return -1; }
	else {
		int c = (int) (l + floor(n / 2));
		if (s == K[c]) {
			return c;
		}
		else if (s < K[c]) {
			return binarySearch(s, K, l, c-1);
		}
		else {
			return binarySearch(s, K, c+1, r);
		}
	}
}

void inMemoryBinarySearch(string keyFilePath, string seekFilePath) {
	int *S = readBinaryToArray(seekFilePath);
	int seekListLength = getDataCount(seekFilePath);
	// start timer
	clock_t start, end;
	start = clock();
	int *K = readBinaryToArray(keyFilePath);
	int keyListLength = getDataCount(keyFilePath);
	int *hit = new int[seekListLength];
	ofstream out("mem-bin-output.txt");
	for (int i = 0; i < seekListLength; i++) {
		if (binarySearch(S[i], K, 0, keyListLength - 1) != -1) {
			hit[i] = 1;
		}
		else {
			hit[i] = 0;
		}
	}
	//end timer
	end = clock();
	for (int j = 0; j < seekListLength; j++) {
		string isPresent = "No";
		if (hit[j] == 1) {
			isPresent = "Yes";
		}
		else {
			isPresent = "No";
		}
		cout << setfill(' ') << setw(12) << S[j] << ": " << isPresent << endl;
		// writing output to a file
		out << setfill(' ') << setw(12) << S[j] << ": " << isPresent << endl;
	}
	cout << "Time: " << double(end - start) / CLOCKS_PER_SEC;
	// writing output to a file
	out << "Time: " << double(end - start) / CLOCKS_PER_SEC;
	delete[] S, K, hit;
	out.close();
}

void onDiskLinearSearch(string keyFilePath, string seekFilePath) {
	int *S = readBinaryToArray(seekFilePath);
	int seekListLength = getDataCount(seekFilePath);
	int keyListLength = getDataCount(keyFilePath);
	// read key values and compare
	ifstream inFile;
	ofstream out("disk-lin-output.txt");
	int *hit = new int[seekListLength];
	inFile.open(keyFilePath.c_str(), ios::binary | ios::in);
	clock_t start, end;
	if (inFile.is_open()) {
		// start timer
		start = clock();
		for (int i = 0; i < seekListLength; i++) {
			inFile.seekg(0, inFile.beg);
			hit[i] = 0;
			for (int j = 0; j < keyListLength; j++) {
				int k;
				inFile.read(reinterpret_cast<char*> (&k), sizeof(int));
				if (S[i] == k) {
					hit[i] = 1;
					break;
				}
				else { continue; }
			}
		}
		// end timer
		end = clock();
		inFile.close();
	}
	else if (inFile.fail()) {
		cerr << "unable to open the file: " << keyFilePath << endl;
		exit(1);
	}
	for (int k = 0; k < seekListLength; k++) {
		string isPresent = (hit[k]==1) ? "Yes" : "No";
		cout << setfill(' ') << setw(12) << S[k] << ": " << isPresent << endl;
		// writing output to a file
		out << setfill(' ') << setw(12) << S[k] << ": " << isPresent << endl;
	}
	cout << "Time: " << double(end - start) / CLOCKS_PER_SEC;
	// writing output to a file
	out << "Time: " << double(end - start) / CLOCKS_PER_SEC;
	out.close();
	delete[] S, hit;
}

int binarySearchOnDisk(int s, ifstream &keyFile, int l, int r) {
	int n = r - l + 1;
	if (n <= 0) { return -1; }
	else {
		int c = (int)(l + floor(n / 2));
		int k;
		keyFile.seekg((c-1)*sizeof(int));
		keyFile.read(reinterpret_cast<char*> (&k), sizeof(int));
		if (s == k) {
			return c;
		}
		else if (s < k) {
			return binarySearchOnDisk(s, keyFile, l, c - 1);
		}
		else {
			return binarySearchOnDisk(s, keyFile, c + 1, r);
		}
	}
}

void onDiskBinarySearch(string keyFilePath, string seekFilePath) {
	int *S = readBinaryToArray(seekFilePath);
	int seekListLength = getDataCount(seekFilePath);
	int keyListLength = getDataCount(keyFilePath);
	clock_t start, end;
	// read key values and compare
	ifstream inFile;
	ofstream out("disk-bin-output.txt");
	int *hit = new int[seekListLength];
	inFile.open(keyFilePath.c_str(), ios::binary | ios::in);
	if (inFile.is_open()) {
		// start timer
		start = clock();
		for (int i = 0; i < seekListLength; i++) {
			inFile.seekg(0, inFile.beg);
			hit[i] = 0;
			if (binarySearchOnDisk(S[i], inFile, 0, keyListLength) != -1) {
				hit[i] = 1;
			}
		}
		//end timer
		end = clock();
		inFile.close();
	}
	else if (inFile.fail()) {
		cerr << "unable to open the file: " << keyFilePath << endl;
		exit(1);
	}
	for (int k = 0; k < seekListLength; k++) {
		string isPresent = (hit[k]==1) ? "Yes" : "No";
		cout << setfill(' ') << setw(12) << S[k] << ": " << isPresent << endl;
		// writing output to a file
		out << setfill(' ') << setw(12) << S[k] << ": " << isPresent << endl;
	}
	cout << "Time: " << double(end - start) / CLOCKS_PER_SEC;
	// writing output to a file
	out << "Time: " << double(end - start) / CLOCKS_PER_SEC;
	out.close();
	delete[] S, hit;
}

int main(int argc, char *argv[]) {
	if(argc != 4){
		cout << "\n Enter the right number of parameters of the format: search-mode key-file seek-file" <<endl;
	} else{
		string searchMode = argv[1];
		string keyFilePath = argv[2];
		string seekFilePath = argv[3];

		if (searchMode.compare("--mem-lin") == 0) {
			inMemoryLinearSearch(keyFilePath, seekFilePath);

		} else if (searchMode.compare("--mem-bin") == 0) {
			inMemoryBinarySearch(keyFilePath, seekFilePath);

		}
		else if (searchMode.compare("--disk-lin") == 0) {
			onDiskLinearSearch(keyFilePath, seekFilePath);
		}
		else if (searchMode.compare("--disk-bin") == 0) {
			onDiskBinarySearch(keyFilePath, seekFilePath);
		} else{
			cout << "\n The search mode entered is invalid" << endl;
		}
	}
	return 0;
}
