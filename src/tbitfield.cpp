// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

static const int BITS_PER_ELEM = sizeof(TELEM) * 8;


TBitField::TBitField(int len)
{
    if (len < 0)
        throw "TBitField: negative length";

    BitLen = len;
    MemLen = (len + BITS_PER_ELEM - 1) / BITS_PER_ELEM;
    pMem   = (MemLen > 0) ? new TELEM[MemLen]() : nullptr;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem   = (MemLen > 0) ? new TELEM[MemLen] : nullptr;
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}


int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / BITS_PER_ELEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return (TELEM)1 << (n % BITS_PER_ELEM);
}


int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw "TBitField::SetBit: index out of range";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw "TBitField::ClrBit: index out of range";
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw "TBitField::GetBit: index out of range";
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
}

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;

    delete[] pMem;

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem   = (MemLen > 0) ? new TELEM[MemLen] : nullptr;
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i]) return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int newLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(newLen);

    for (int i = 0; i < MemLen; i++)
        result.pMem[i] |= pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        result.pMem[i] |= bf.pMem[i];

    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int newLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(newLen);

    for (int i = 0; i < MemLen && i < bf.MemLen; i++)
        result.pMem[i] = pMem[i] & bf.pMem[i];

    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(*this);

    for (int i = 0; i < result.MemLen; i++)
        result.pMem[i] = ~result.pMem[i];

    if (result.BitLen > 0 && result.BitLen % BITS_PER_ELEM != 0)
    {
        int tail = result.BitLen % BITS_PER_ELEM;
        TELEM tailMask = ((TELEM)1 << tail) - 1;
        result.pMem[result.MemLen - 1] &= tailMask;
    }

    return result;
}


istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int len;
    istr >> len;
    if (len < 0) len = 0;

    delete[] bf.pMem;
    bf.BitLen = len;
    bf.MemLen = (len + BITS_PER_ELEM - 1) / BITS_PER_ELEM;
    bf.pMem   = (bf.MemLen > 0) ? new TELEM[bf.MemLen]() : nullptr;

    for (int i = 0; i < bf.BitLen; i++)
    {
        int bit;
        istr >> bit;
        if (bit) bf.SetBit(i);
    }

    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf)
{
    for (int i = bf.BitLen - 1; i >= 0; i--)
        ostr << bf.GetBit(i);
    return ostr;
}