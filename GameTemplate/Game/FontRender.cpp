#include "stdafx.h"
#include "FontRender.h"
namespace nsMuscle {
	void FontRender::PostRender(RenderContext& rc)
	{		
		if (m_isActive == false) {
			return;
		}
		m_PosX_font.Begin(rc);
		m_PosX_font.Draw(m_text.c_str(), m_position, m_color, m_rotation, m_scale, m_pivot);
		m_PosX_font.End(rc);
	}
}