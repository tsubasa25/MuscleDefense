#include "stdafx.h"
#include "Path.h"
namespace nsPathConstant {
	//平準化パラメータ
	//const float alpha = 0.5f;
	//const Vector3 alpha = { 0.5f,0.5f,0.5f };
	//const float beta = 0.2f;
	//const Vector3 bata = { 0.2f,0.2f,0.2f };
}
void Path::Build()
{
	m_sectionArray.resize(m_pointArray.size() - 1);

	// 平準化していく。	
	//一直線のパスを作る
	//Vector3 streat =m_pointArray[m_pointArray.size()-2] - m_pointArray[0];
	//streat = streat/(m_pointArray.size());
	//
	//for (int pointNo = 0; pointNo < m_pointArray.size()-1 ; pointNo++) {
	//	m_pointArraySmooth.at(pointNo) = streat;
	//	streat += streat;
	//	//リーフでなめらかにする
	//	m_pointArray.at(pointNo).Lerp(0.1f, m_pointArray.at(pointNo), m_pointArraySmooth.at(pointNo));
	//}

	// セクションを構築していく。
	for (int pointNo = 0; pointNo < m_pointArray.size() - 1; pointNo++) {
		auto& section = m_sectionArray.at(pointNo);
		section.startPos = m_pointArray.at(pointNo);
		section.endPos = m_pointArray.at(pointNo + 1);
		section.direction = section.endPos - section.startPos;
		section.length = section.direction.Length();
		section.direction.Normalize();
	}	
	
}
Vector3 Path::Move(
	Vector3 pos,
	float moveSpeed,
	bool& isEnd
) {
	if (m_sectionArray.empty()
		|| m_sectionNo >= m_sectionArray.size()
		) {
		// パスが構築されていない
		return pos;
	}
	SSection& currentSection = m_sectionArray.at(m_sectionNo);
	// セクションの終点に向かうベクトルを計算する。
	Vector3 toEnd = currentSection.endPos - pos;
	toEnd.Normalize();
	pos += toEnd * moveSpeed;

	Vector3 toEnd2 = currentSection.endPos - pos;
	toEnd2.Normalize();

	if (toEnd.Dot(toEnd2) <= 0.0f) {
		// 向きが変わったので終点を超えた。
		pos = currentSection.endPos;
		if (m_sectionNo == m_sectionArray.size() - 1) {
			// 終点
			isEnd = true;
		}
		else {
			m_sectionNo++;
		}
	}	
	return pos;
}
void Path::SectionArrayPopBack(int num)
{
	for (int i = 0; i < num; i++)
	{
		m_sectionArray.pop_back();
	}	
}