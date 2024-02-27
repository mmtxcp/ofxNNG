#include "ofFileUtils.h"
#ifndef TARGET_WIN32
#include <pwd.h>
#endif

//#include "ofUtils.h"
#include <fstream>

#ifdef TARGET_OSX
#include <mach-o/dyld.h>       /* _NSGetExecutablePath */
#include <limits.h>        /* PATH_MAX */
#endif


//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
// -- ofBuffer
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

//--------------------------------------------------
ofBuffer::ofBuffer()
	:currentLine(end(), end()) {
	buffer.resize(1);
}

//--------------------------------------------------
ofBuffer::ofBuffer(const char* _buffer, std::size_t size)
	: buffer(_buffer, _buffer + size)
	, currentLine(end(), end()) {
	buffer.resize(buffer.size() + 1, 0);
}

//--------------------------------------------------
ofBuffer::ofBuffer(const string& text)
	:buffer(text.begin(), text.end())
	, currentLine(end(), end()) {
	buffer.resize(buffer.size() + 1, 0);
}

//--------------------------------------------------
ofBuffer::ofBuffer(istream& stream, size_t ioBlockSize)
	:currentLine(end(), end()) {
	set(stream, ioBlockSize);
}

//--------------------------------------------------
bool ofBuffer::set(istream& stream, size_t ioBlockSize) {
	if (stream.bad()) {
		clear();
		return false;
	}
	else {
		buffer.clear();
	}

	vector<char> aux_buffer(ioBlockSize);
	while (stream.good()) {
		stream.read(&aux_buffer[0], ioBlockSize);
		buffer.insert(buffer.end(), aux_buffer.begin(), aux_buffer.begin() + stream.gcount());
	}
	buffer.push_back(0);
	return true;
}

//--------------------------------------------------
bool ofBuffer::writeTo(ostream& stream) const {
	if (stream.bad()) {
		return false;
	}
	stream.write(&(buffer[0]), buffer.size() - 1);
	return stream.good();
}

//--------------------------------------------------
void ofBuffer::set(const char* _buffer, std::size_t _size) {
	buffer.assign(_buffer, _buffer + _size);
	buffer.resize(buffer.size() + 1, 0);
}

//--------------------------------------------------
void ofBuffer::set(const string& text) {
	set(text.c_str(), text.size());
}

//--------------------------------------------------
void ofBuffer::append(const string& _buffer) {
	append(_buffer.c_str(), _buffer.size());
}

//--------------------------------------------------
void ofBuffer::append(const char* _buffer, std::size_t _size) {
	buffer.insert(buffer.end() - 1, _buffer, _buffer + _size);
	buffer.back() = 0;
}

//--------------------------------------------------
void ofBuffer::clear() {
	buffer.resize(1, 0);
}

//--------------------------------------------------
void ofBuffer::allocate(std::size_t _size) {
	clear();
	//we always add a 0 at the end to avoid problems with strings
	buffer.resize(_size + 1, 0);
}

//--------------------------------------------------
char* ofBuffer::getData() {
	if (buffer.empty()) {
		return nullptr;
	}
	return &buffer[0];
}

//--------------------------------------------------
const char* ofBuffer::getData() const {
	if (buffer.empty()) {
		return nullptr;
	}
	return &buffer[0];
}

//--------------------------------------------------
char* ofBuffer::getBinaryBuffer() {
	return getData();
}

//--------------------------------------------------
const char* ofBuffer::getBinaryBuffer() const {
	return getData();
}

//--------------------------------------------------
string ofBuffer::getText() const {
	if (buffer.empty()) {
		return "";
	}
	return &buffer[0];
}

//--------------------------------------------------
ofBuffer::operator string() const {
	return getText();
}

//--------------------------------------------------
ofBuffer& ofBuffer::operator=(const string& text) {
	set(text);
	return *this;
}

//--------------------------------------------------
long ofBuffer::size() const {
	if (buffer.empty()) {
		return 0;
	}
	//we always add a 0 at the end to avoid problems with strings
	return buffer.size() - 1;
}

//--------------------------------------------------
string ofBuffer::getNextLine() {
	if (currentLine.empty()) {
		currentLine = getLines().begin();
	}
	else {
		++currentLine;
	}
	return currentLine.asString();
}

//--------------------------------------------------
string ofBuffer::getFirstLine() {
	currentLine = getLines().begin();
	return currentLine.asString();
}

//--------------------------------------------------
bool ofBuffer::isLastLine() {
	return currentLine == getLines().end();
}

//--------------------------------------------------
void ofBuffer::resetLineReader() {
	currentLine = getLines().begin();
}

//--------------------------------------------------
vector<char>::iterator ofBuffer::begin() {
	return buffer.begin();
}

//--------------------------------------------------
vector<char>::iterator ofBuffer::end() {
	return buffer.end();
}

//--------------------------------------------------
vector<char>::const_iterator ofBuffer::begin() const {
	return buffer.begin();
}

//--------------------------------------------------
vector<char>::const_iterator ofBuffer::end() const {
	return buffer.end();
}

//--------------------------------------------------
vector<char>::reverse_iterator ofBuffer::rbegin() {
	return buffer.rbegin();
}

//--------------------------------------------------
vector<char>::reverse_iterator ofBuffer::rend() {
	return buffer.rend();
}

//--------------------------------------------------
vector<char>::const_reverse_iterator ofBuffer::rbegin() const {
	return buffer.rbegin();
}

//--------------------------------------------------
vector<char>::const_reverse_iterator ofBuffer::rend() const {
	return buffer.rend();
}

//--------------------------------------------------
ofBuffer::Line::Line(vector<char>::iterator _begin, vector<char>::iterator _end)
	:_current(_begin)
	, _begin(_begin)
	, _end(_end) {
	if (_begin == _end) {
		line = "";
		return;
	}

	bool lineEndWasCR = false;
	while (_current != _end && *_current != '\n') {
		if (*_current == '\r') {
			lineEndWasCR = true;
			break;
		}
		else if (*_current == 0 && _current + 1 == _end) {
			break;
		}
		else {
			_current++;
		}
	}
	line = string(_begin, _current);
	if (_current != _end) {
		_current++;
	}
	// if lineEndWasCR check for CRLF
	if (lineEndWasCR && _current != _end && *_current == '\n') {
		_current++;
	}
}

//--------------------------------------------------
const string& ofBuffer::Line::operator*() const {
	return line;
}

//--------------------------------------------------
const string* ofBuffer::Line::operator->() const {
	return &line;
}

//--------------------------------------------------
const string& ofBuffer::Line::asString() const {
	return line;
}

//--------------------------------------------------
ofBuffer::Line& ofBuffer::Line::operator++() {
	*this = Line(_current, _end);
	return *this;
}

//--------------------------------------------------
ofBuffer::Line ofBuffer::Line::operator++(int) {
	Line tmp(*this);
	operator++();
	return tmp;
}

//--------------------------------------------------
bool ofBuffer::Line::operator!=(Line const& rhs) const {
	return rhs._begin != _begin || rhs._end != _end;
}

//--------------------------------------------------
bool ofBuffer::Line::operator==(Line const& rhs) const {
	return rhs._begin == _begin && rhs._end == _end;
}

bool ofBuffer::Line::empty() const {
	return _begin == _end;
}

//--------------------------------------------------
ofBuffer::Lines::Lines(vector<char>& buffer)
	:_begin(buffer.begin())
	, _end(buffer.end()) {}

//--------------------------------------------------
ofBuffer::Line ofBuffer::Lines::begin() {
	return Line(_begin, _end);
}

//--------------------------------------------------
ofBuffer::Line ofBuffer::Lines::end() {
	return Line(_end, _end);
}

//--------------------------------------------------
ofBuffer::Lines ofBuffer::getLines() {
	return ofBuffer::Lines(buffer);
}

//--------------------------------------------------
ostream& operator<<(ostream& ostr, const ofBuffer& buf) {
	buf.writeTo(ostr);
	return ostr;
}

//--------------------------------------------------
istream& operator>>(istream& istr, ofBuffer& buf) {
	buf.set(istr);
	return istr;
}
string ofToDataPath(const string& path, bool makeAbsolute) {
	return path;
}

//--------------------------------------------------
ofBuffer ofBufferFromFile(const string& path, bool binary) {
	ios_base::openmode mode = binary ? ifstream::binary : ios_base::in;
	ifstream istr(ofToDataPath(path, true).c_str(), mode);
	ofBuffer buffer(istr);
	istr.close();
	return buffer;
}

//--------------------------------------------------
bool ofBufferToFile(const string& path, ofBuffer& buffer, bool binary) {
	ios_base::openmode mode = binary ? ofstream::binary : ios_base::out;
	ofstream ostr(ofToDataPath(path, true).c_str(), mode);
	bool ret = buffer.writeTo(ostr);
	ostr.close();
	return ret;
}