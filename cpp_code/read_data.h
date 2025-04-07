#ifndef read_data_h
#define read_data_h

#include <stdio.h>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

const int numberAssets=83;
const int numberReturns=700;
const int N=numberAssets+2;
class Csv {	// read and parse comma-separated values
	// sample input: "LU",86.25,"11/4/1998","2:19PM",+4.0625
	
public:
	Csv(istream& fin = cin, string sep = ",") : 
	fin(fin), fieldsep(sep) {}
	
	int getline(string& str)
	{	
		char c;
		for (line = ""; fin.get(c) && !endofline(c); )
			line += c;
		split();
		str = line;
		return !fin.eof();
	}
	// getfield: return n-th field
	string getfield(int n)
	{
		if (n < 0 || n >= nfield)
			return "";
		else
			return field[n];
	}

	int getnfield() const { return nfield; }
private:
	istream& fin;			// input file pointer
	string line;			// input line
	vector<string> field;	// field strings
	int nfield;				// number of fields
	string fieldsep;		// separator characters
	
	// split: split line into fields
    int split()
	{
		string fld;
		int i, j;
		
		nfield = 0;
		if (line.length() == 0)
			return 0;
		i = 0;
		
		do {
			if (i < line.length() && line[i] == '"')
				j = advquoted(line, fld, ++i);	// skip quote
			else
				j = advplain(line, fld, i);
			if (nfield >= field.size())
				field.push_back(fld);
			else
				field[nfield] = fld;
			nfield++;
			i = j + 1;
		} while (j < line.length());
		
		return nfield;
	}

	int endofline(char c)
	{
		int eol;
		
		eol = (c=='\r' || c=='\n');
		if (c == '\r') {
			fin.get(c);
			if (!fin.eof() && c != '\n')
				fin.putback(c);	// read too far
		}
		return eol;
	}

	// advquoted: quoted field; return index of next separator
    int advquoted(const string& s, string& fld, int i)
	{
		int j;
		
		fld = "";
		for (j = i; j < s.length(); j++) {
			if (s[j] == '"' && s[++j] != '"') {
				int k = s.find_first_of(fieldsep, j);
				if (k > s.length())	// no separator found
					k = s.length();
				for (k -= j; k-- > 0; )
					fld += s[j++];
				break;
			}
			fld += s[j];
		}
		return j;
	}

	// advplain: unquoted field; return index of next separator
    int advplain(const string& s, string& fld, int i)
	{
		int j;
		
		j = s.find_first_of(fieldsep, i); // look for separator
		if (j > s.length())               // none found
			j = s.length();
		fld = string(s, i, j-i);
		return j;
	}

    // getline: get one line, grow as needed
};

double string_to_double( const std::string& s )
{
	std::istringstream i(s);
	double x;
	if (!(i >> x))
		return 0;
	return x;
} 
void readData(double data[][numberReturns],string fileName)
{	 
	char tmp[20];
	ifstream file (strcpy(tmp, fileName.c_str()));
	Csv csv(file);
	string line;
	if (file.is_open())
	{	
		int i=0;
		while (csv.getline(line) != 0) {
         	for (int j = 0; j < csv.getnfield(); j++)
            {
               double temp=string_to_double(csv.getfield(j));
               data[j][i]=temp;
            }
            i++;
		}
		
		file.close();
	}
	else {cout <<fileName <<" missing\n";exit(0);}                                                   
}
#endif
