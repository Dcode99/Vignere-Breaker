// Dillon Tate
// C++ Vignere Cipher Breaking
// CS 378

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

const int alphabet = 26;
//frequencies are given in homework pdf
const double frequencies[26] = { 0.082,0.015,0.028,0.043,0.127,0.022,0.020,0.061,0.070,0.002,0.008,0.040,0.024,0.067,0.075,0.019,0.001,0.060,0.063,0.091,0.028,0.010,0.023,0.001,0.020,0.001 };

string read() {
	string s = " ";
	ifstream ctext;
	ctext.open("v.txt");
	if (!ctext.fail()) {
		ctext >> s;
		ctext.close();
		cout << "Ciphertext length is " << s.length() << endl;
	}
	return s;
}

void pairs(string in, int pcount[]) {
	int length = 0;
	for (int j = 1; j < in.length()-6; j++) {
		for (int i = 0; i < j; i++) {	
			if (in[i] == in[j]) {
				length = 1;
				if (in[i + 1] == in[j + 1]) {
					length = 2;
					if (in[i + 2] == in[j + 2]) {
						length = 3;
						if (in[i + 3] == in[j + 3]) {
							length = 4;
							if (in[i + 4] == in[j + 4]) {
								length = 5;
								if (in[i + 5] == in[j + 5]) {
									length = 6;
			}}}}}}
			else { length = 0; }
			// if there are at least two matching characters increment the index of the gcd of the difference
			if (length >= 2) {
				if ((j - i) % 6 == 0) {
					pcount[4]++;
				}
				else if ((j - i) % 5 == 0)
					pcount[3]++;
				else if ((j - i) % 4 == 0) {
					pcount[2]++;
				}
				else if ((j - i) % 3 == 0) {
					pcount[1]++;
				}
				else if ((j - i) % 2 == 0) {
					pcount[0]++;
				}
			}

		}
	}

}

//find index of max element in an array of length 6
int array_max(int pcount[], int size) {
	int max = -999;
	int index = -999;
	for (int x = 0; x < size; x++) {
		if (pcount[x] > max) {
			max = pcount[x];
			index = x;
		}
	}
	return index;
}

int array_max(double pcount[], int size) {
	int max = -999;
	int index = -999;
	for (int x = 0; x < size; x++) {
		if (pcount[x] > max) {
			max = pcount[x];
			index = x;
		}
	}
	return index;
}

// print out array
void print_array(int pcount[]) {
	for (int i = 0; i < 5; i++) {
		cout << "Key length " << i+2 << ": " << pcount[i] << endl;
	}
}

//split input into substrings based on key length guess
void split(string input, string strings[], int guess) {
	for (int i = 0; i < input.length(); i++) {
		strings[(i + guess) % guess] += input[i];
	}
}

void freq_analysis(string s[], int size, int shifts[]) {
	int index = 0;
	double occurances[alphabet];
	double sum = 0, maxsum = 0;
	// for each substring
	for (int i = 0; i < size; i++) {
		for (int x = 0; x < alphabet; x++) {
			occurances[x] = 0;
		}
		// loop through substring and find occurances of each letter
		for (int j = 0; j < s[i].length(); j++) {
			occurances[s[i][j] - 'a'] = occurances[s[i][j] - 'a'] + 1;
		}
		// find the frequencies in decimal form for that substring
		for (int y = 0; y < alphabet; y++) {
			occurances[y] = occurances[y] / s[i].length();
		}
		// print out all frequencies
		//index = array_max(occurances, size);
		cout << "Character " << i << " frequencies:" << endl;
		for (int z = 0; z < alphabet; z++) {
			cout << occurances[z] << " ";
		}
		cout << endl;

		//finding likely shift based on frequencies
		maxsum = 0;
		for (int s = 0; s < alphabet; s++) {
			sum = 0;
			for (int e = 0; e < alphabet; e++) {
				sum += frequencies[e] * occurances[(e + s) % 26];
			}
			if (sum > maxsum) {
				index = s;
				maxsum = sum;
			}
		}

		//actual key character is index+'a'
		shifts[i] = index;
		cout << index << endl;
	}
}

void decrypt(string subs[], int key[], int keylen) {
	string plain = "";
	char temp = ' ';
	for (int i = 0; i < subs[0].length(); i++) {
		for (int j = 0; j < keylen; j++) {
			temp = subs[j][i] - (char) key[j];
			if (temp >= 'a') {
				cout << temp;
			}
			else {
				cout << (char) (temp + 26);
			}
		}
	}
	//cout << plain;
}

void main() {
	int keylen = 0;
	string substrings[6];
	int shifts[6];
	string key;
	//read in ciphertext
	string in = read();
	//find likely lengths of key
	int pcount[5] = { 0,0,0,0,0 };
	int psize = 5;
	pairs(in, pcount);
	// print out number of gcds of the differences between similar pairs
	print_array(pcount);
	// find the most occuring gcd and print it as most likely
	cout << endl << "The most likely key length is: " << array_max(pcount, psize) + 2 << endl;

	// ask for a key length to use
	cout << "What key length would you like to check: ";
	cin >> keylen;
	while (keylen < 1 || keylen > 6) {
		cout << "Invalid, try again: ";
		cin >> keylen;
	}
	// start frequency analysis on seperate sections
	// split string every keylen letters
	split(in, substrings, keylen);
	// analyze frequency of letters in each substring
	freq_analysis(substrings, keylen, shifts);
	// print out shifts (numerical)
	for (int i = 0; i < keylen; i++) {
		cout << shifts[i] << " ";
	}
	// print out actual character of key
	cout << endl << "The likely key is: ";
	for (int i = 0; i < keylen; i++) {
		cout << (char)(shifts[i] + 'a');
	}
	cout << endl;
	// adding yes or no for functionality of testing, not because required
	cout << "Would you like to decrpyt with this key? (y/n)";
	char answer;
	cin >> answer;
	if (answer == 'y') {
		decrypt(substrings, shifts, keylen);
	}

	system("pause");
	return;
}