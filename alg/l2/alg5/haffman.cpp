#include "haffman.h"
//#include "Huffman.h"

class BitQriter
{
    public:
    BitWriter();
    void WriteBit(unsigned char bit)
    {
        if (bitCount == buffer.size() * 8)
        {
            buffer.push_back(0);
        }
        buffer[bitCount/8] |bit <<(7-bitCount%8 );
        bitCount++;
    }
    void WrityBity(unsigned char byte)
    {
        if (bitCount %8 == 0)
        {
            buffer.push_back(byte);
        }
        else
        {
            int offset = bitCount %8;
            buffer[bitCount/8] |= byte >> offset;
            buffer.push_back(byte << (8 - offset));
        }
        bitCount += 8;
    }
private:
    size_t bitCount;
    std::vector<unsigned char> buffer;

};


static void copyStream(IInputStream& input, IOutputStream& output)
{
    byte value;
    while (input.Read(value))
    {
        output.Write(value);
    }
}

void Encode(IInputStream& original, IOutputStream& compressed)
{
    copyStream(original, compressed);
}

void Decode(IInputStream& compressed, IOutputStream& original)
{
    copyStream(compressed, original);
}
