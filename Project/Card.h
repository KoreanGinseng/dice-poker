#pragma once
#include "Mof.h"

constexpr int CardSizeW = 136;
constexpr int CardSizeH = 200;
constexpr int CardMax   =  13;
constexpr int CardMark  =   4;

#define CARD_FLAGMAX 0x003FFFFFFFFFFFFF

class CCard
{
private:

	MofU64 m_CardFlag{ 0 };

	int m_JokerCount{ 2 };

public:
	
	std::vector<std::pair<int, int>> m_Cards;

	explicit CCard(void) = default;
	
	~CCard(void) = default;

	void Shuffle(void);

	void SetJokerCount(const int& jokerCount);

	int GetJokerCount(void) const;

	static CRectangle GetCardRect(void);
};

