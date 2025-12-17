// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
using namespace std;

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
    if (mp <= 0)
        throw("power should be positive");
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0 || Elem >= MaxPower)
        throw ("invalid element");
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw("invalid element");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw("invalid element");
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this != &s) {
        BitField = s.BitField;
        MaxPower = s.MaxPower;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    int maxMP = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    TSet result(maxMP);
    for (int i = 0; i < MaxPower; i++) {
        if (IsMember(i))
            result.InsElem(i);
    }
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.IsMember(i))
            result.InsElem(i);
    }
    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    int maxMP = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    TSet result(maxMP);
    int minMP = (MaxPower < s.MaxPower) ? MaxPower : s.MaxPower;
    for (int i = 0; i < minMP; i++) {
        if (IsMember(i) && s.IsMember(i)) {
            result.InsElem(i);
        }

    }
    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(MaxPower);
    for (int i = 0; i < MaxPower; i++) {
        if (!IsMember(i)) {
            result.InsElem(i);
        }
    }
    return result;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    for (int i = 0; i < s.GetMaxPower(); i++) {
        s.InsElem(i);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    ostr << "{";
    bool first = true;
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if (s.IsMember(i)) {
            if (!first) {
                ostr << " , ";
                ostr << i;
                first = false;
            }
        }
    }
    ostr << "}";
    return ostr;
}
