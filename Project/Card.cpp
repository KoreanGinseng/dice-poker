#include "Card.h"

void CCard::Shuffle(void)
{
	m_CardFlag = 0;
	m_Cards.clear();
	while (m_CardFlag != CARD_FLAGMAX)
	{
		// 0~53‚Ì”š¶¬
		int random = CUtilities::Random(52 + m_JokerCount);

		if (m_CardFlag & (MofU64)1 << random)
		{
			continue;
		}

		m_CardFlag |= (MofU64)1 << random;
		if (random >= 52)
		{
			m_Cards.push_back(std::pair<int, int>(4, 0));
		}
		else
		{
			m_Cards.push_back(std::pair<int, int>(random / CardMax, random % CardMax));
		}
	}
}

void CCard::SetJokerCount(const int & jokerCount)
{
	m_JokerCount = jokerCount;
}

int CCard::GetJokerCount(void) const
{
	return m_JokerCount;
}

CRectangle CCard::GetCardRect(void)
{
	return CRectangle{ 0, 0, (MofFloat)CardSizeW, (MofFloat)CardSizeH };
}
