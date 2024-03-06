#pragma once

#include "ofx/utils/ofConstants.h"
using namespace std;

//----------------------------------------------------------
// ofBuffer
//----------------------------------------------------------

class ofBuffer {

public:
	ofBuffer();
	ofBuffer(const char* buffer, std::size_t size);
	ofBuffer(const string& text);
	ofBuffer(istream& stream, size_t ioBlockSize = 1024);

	void set(const char* _buffer, std::size_t _size);
	void set(const string& text);
	bool set(istream& stream, size_t ioBlockSize = 1024);
	void append(const string& _buffer);
	void append(const char* _buffer, std::size_t _size);

	bool writeTo(ostream& stream) const;

	void clear();

	void allocate(std::size_t _size);

	char* getData();
	const char* getData() const;
	OF_DEPRECATED_MSG("Use getData instead", char* getBinaryBuffer());
	OF_DEPRECATED_MSG("Use getData instead", const char* getBinaryBuffer() const);

	string getText() const;
	operator string() const;  // cast to string, to use a buffer as a string
	ofBuffer& operator=(const string& text);

	long size() const;

	OF_DEPRECATED_MSG("use a lines iterator instead", string getNextLine());
	OF_DEPRECATED_MSG("use a lines iterator instead", string getFirstLine());
	OF_DEPRECATED_MSG("use a lines iterator instead", bool isLastLine());
	OF_DEPRECATED_MSG("use a lines iterator instead", void resetLineReader());

	friend ostream& operator<<(ostream& ostr, const ofBuffer& buf);
	friend istream& operator>>(istream& istr, ofBuffer& buf);

	vector<char>::iterator begin();
	vector<char>::iterator end();
	vector<char>::const_iterator begin() const;
	vector<char>::const_iterator end() const;
	vector<char>::reverse_iterator rbegin();
	vector<char>::reverse_iterator rend();
	vector<char>::const_reverse_iterator rbegin() const;
	vector<char>::const_reverse_iterator rend() const;

	struct Line : public std::iterator<std::forward_iterator_tag, Line> {
		Line(vector<char>::iterator _begin, vector<char>::iterator _end);
		const string& operator*() const;
		const string* operator->() const;
		const string& asString() const;
		Line& operator++();
		Line operator++(int);
		bool operator!=(Line const& rhs) const;
		bool operator==(Line const& rhs) const;
		bool empty() const;

	private:
		string line;
		vector<char>::iterator _current, _begin, _end;
	};

	struct Lines {
		Lines(vector<char>& buffer);
		Line begin();
		Line end();

	private:
		vector<char>::iterator _begin, _end;
	};

	Lines getLines();

private:
	vector<char> 	buffer;
	Line			currentLine;
};

//--------------------------------------------------
ofBuffer ofBufferFromFile(const string& path, bool binary = false);

//--------------------------------------------------
bool ofBufferToFile(const string& path, ofBuffer& buffer, bool binary = false);

