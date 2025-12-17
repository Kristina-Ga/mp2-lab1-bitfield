// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
using namespace std;

TBitField::TBitField(int len)
{
    if (len <= 0) {
        throw("invalid len");
    }
    BitLen = len;
    int size = sizeof(TELEM) * 8;
    MemLen = (BitLen + size - 1) / size;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }

}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    MemLen = bf.MemLen;
    BitLen = bf.BitLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
    //указатель
    pMem = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) {
        throw("invalid index");
    }
    int size = sizeof(TELEM) * 8;
    return n / size;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) {
        throw("invalid index");
    }
    int size = sizeof(TELEM) * 8;
    return 1 << (n % size);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] = pMem[index] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] = pMem[index] & ~mask;

}

int TBitField::GetBit(const int n) const // получить значение бита
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) != 0;  // проверяем установлен ли бит
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 0; //false
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0; //false
        }
    }
    return 1; //true
}
/*PLTCM*/
int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int maxlen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxlen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        result.pMem[i] = result.pMem[i] | bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int maxlen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxlen);
    int minmlen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minmlen; i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen); //Создаем объект той же длины
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }
    // Очищаем лишние биты в последней ячейке
    int bitsInTELEM = sizeof(TELEM) * 8;
    int extraBits = BitLen % bitsInTELEM;
    if (extraBits > 0) {
        TELEM mask = (1 << extraBits) - 1;
        result.pMem[MemLen - 1] &= mask;
    }
    return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    //Читаем биты как последовательность 0 и 1
    for (int i = 0; i < bf.BitLen; i++) {
        char bit;
        istr >> bit; // читаем один символ

        //Устанавливаем или очищаем бит в зависимости от ввода
        if (bit == '1') {
            bf.SetBit(i);
        }
        else if (bit == '0') {
            bf.ClrBit(i);
        }
        else {
            // Если введен не 0 и не 1, прерываем ввод
            break;
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    ostr << "{ ";
    //Выводим все биты
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
        if (i < bf.BitLen - 1) {
            ostr << ", ";
        }
    }
    ostr << " }";
    return ostr;
}
