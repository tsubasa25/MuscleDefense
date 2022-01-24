#include "stdafx.h"
#include "Path.h"
namespace nsPathConstant {
	//�������p�����[�^
	//const float alpha = 0.5f;
	//const Vector3 alpha = { 0.5f,0.5f,0.5f };
	//const float beta = 0.2f;
	//const Vector3 bata = { 0.2f,0.2f,0.2f };
}
void Path::Build()
{
	m_sectionArray.resize(m_pointArray.size() - 1);

	// ���������Ă����B	
	//�꒼���̃p�X�����
	//Vector3 streat =m_pointArray[m_pointArray.size()-2] - m_pointArray[0];
	//streat = streat/(m_pointArray.size());
	//
	//for (int pointNo = 0; pointNo < m_pointArray.size()-1 ; pointNo++) {
	//	m_pointArraySmooth.at(pointNo) = streat;
	//	streat += streat;
	//	//���[�t�łȂ߂炩�ɂ���
	//	m_pointArray.at(pointNo).Lerp(0.1f, m_pointArray.at(pointNo), m_pointArraySmooth.at(pointNo));
	//}

	// �Z�N�V�������\�z���Ă����B
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
		// �p�X���\�z����Ă��Ȃ�
		return pos;
	}
	SSection& currentSection = m_sectionArray.at(m_sectionNo);
	// �Z�N�V�����̏I�_�Ɍ������x�N�g�����v�Z����B
	Vector3 toEnd = currentSection.endPos - pos;
	toEnd.Normalize();
	pos += toEnd * moveSpeed;

	Vector3 toEnd2 = currentSection.endPos - pos;
	toEnd2.Normalize();

	if (toEnd.Dot(toEnd2) <= 0.0f) {
		// �������ς�����̂ŏI�_�𒴂����B
		pos = currentSection.endPos;
		if (m_sectionNo == m_sectionArray.size() - 1) {
			// �I�_
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