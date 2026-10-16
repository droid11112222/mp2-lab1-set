// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
}

TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= MaxPower) return 0;
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower) return;
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower) return;
    BitField.ClrBit(Elem);
}

TSet& TSet::operator=(const TSet &s)
{
    if (this == &s) return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const
{
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet result(*this);
    result.BitField = BitField | s.BitField;
    result.MaxPower = result.BitField.GetLength();
    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "TSet::operator+: element out of range";

    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "TSet::operator-: element out of range";

    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet result(*this);
    result.BitField = BitField & s.BitField;
    result.MaxPower = result.BitField.GetLength();
    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(*this);
    result.BitField = ~BitField;
    return result;
}


istream &operator>>(istream &istr, TSet &s)
{
    int mp;
    istr >> mp;
    if (mp < 0) mp = 0;

    s.MaxPower = mp;
    s.BitField = TBitField(mp);

    for (int i = 0; i < mp; i++)
    {
        int bit;
        istr >> bit;
        if (bit) s.InsElem(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TSet &s)
{
    ostr << "{";
    bool first = true;
    for (int i = 0; i < s.MaxPower; i++)
    {
        if (s.IsMember(i))
        {
            if (!first) ostr << ", ";
            ostr << i;
            first = false;
        }
    }
    ostr << "}";
    return ostr;
}