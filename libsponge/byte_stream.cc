#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : 
    _readidx(0), _writeidx(0), _capacity(capacity), _buffer(capacity), _input_ended_flag(false),
    _writecnt(0), _readcnt(0), _cnt(0)
{

}

size_t ByteStream::write(const string &data) {
    size_t len = std::min(remaining_capacity(), data.length());
    for (size_t i = 0; i < len; ++i, ++_writecnt)
    {
        _buffer[_writeidx] = data[i];
        _writeidx = (_writeidx + 1) % _capacity;
    }
    _cnt += len;
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    std::string ret;
    size_t actural_len = std::min(len, buffer_size());
    size_t tmpreadidx = _readidx;
    for (size_t i = 0; i < actural_len; ++i)
    {
        ret.push_back(_buffer[tmpreadidx]);
        tmpreadidx = (tmpreadidx + 1) % _capacity;
    }
    return ret;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t actural_len = std::min(len, buffer_size());
    _readcnt += actural_len;
    _readidx = (_readidx + actural_len) % _capacity;
    _cnt -= actural_len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    auto ret = peek_output(len);
    pop_output(len);
    return ret;
}

void ByteStream::end_input() { _input_ended_flag = true; }

bool ByteStream::input_ended() const { return _input_ended_flag; }

size_t ByteStream::buffer_size() const { return _cnt; }

bool ByteStream::buffer_empty() const { return _cnt == 0; }

bool ByteStream::eof() const { return buffer_empty() && input_ended(); }

size_t ByteStream::bytes_written() const { return _writecnt; }

size_t ByteStream::bytes_read() const { return _readcnt; }

size_t ByteStream::remaining_capacity() const { return _capacity - _cnt; }
