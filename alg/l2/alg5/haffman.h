#ifndef HAFFMAN_H
#define HAFFMAN_H


typedef unsigned char byte;
#define interface struct

interface IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};

#endif // HAFFMAN_H
