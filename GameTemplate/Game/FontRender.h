#pragma once

namespace nsMuscle {
	class FontRender : public IGameObject
	{
	public:
		/**
			 * @brief �t�H���g�̕`��@PostRender�Ȃ̂Ń��f�������`���I�������ɕ`�����
			 * @param rc �����_�[�R���e�L�X�g
			 * @param camera �`�悷��J����
			*/
		void PostRender(RenderContext& rc)override;

		/**
		 * @brief �t�H���g�̃e�L�X�g���Z�b�g
		 * @param text �e�L�X�g
		*/
		void SetText(const std::wstring& text) { m_text = text; }

		/**
		 * @brief �t�H���g�̍��W���Z�b�g
		 * @param position ���W
		*/
		void SetPosition(const Vector2& position) { m_position = position; }

		/**
		 * @brief �t�H���g�̐F���Z�b�g
		 * @param �F
		*/
		void SetColor(const Vector4& color) { m_color = color; }

		/**
		 * @brief �t�H���g�̉�]�p�x���Z�b�g
		 * @param ��]�p�x
		*/
		void SetRotation(const float& rotation) { m_rotation = rotation;; }

		/**
		 * @brief �t�H���g�̊g�嗦���Z�b�g
		 * @param scale �g�嗦
		*/
		void SetScale(const float& scale) { m_scale = scale; }

		/**
		 * @brief �t�H���g�̃s�{�b�g���Z�b�g(���삵�Ă��܂���)
		 * @param pivot �s�{�b�g
		*/
		void SetPivot(const Vector2& pivot) { m_pivot = pivot; }


		/**
		 * @brief �t�H���g�̃A�E�g���C���̕`��t���O���Z�b�g
		 * @param flag �`�悷��:true �`�悵�Ȃ�:false
		*/
		void SetShadowFlag(bool flag)
		{
			m_drawShadowFlag = flag;
			m_PosX_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}

		/**
		 * @brief �t�H���g�̃A�E�g���C���̑������Z�b�g
		 * @param offset �A�E�g���C���̑���
		*/
		void SetShadowOffset(float offset)
		{
			m_shadowOffset = offset;
			m_PosX_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}

		/**
		 * @brief �t�H���g�̃A�E�g���C���̐F���Z�b�g
		 * @param color �A�E�g���C���̐F
		*/
		void SetShadowColor(Vector4 color)
		{
			m_shadowColor = color;
			m_PosX_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}
		/// <summary>
		/// �t�H���g�����_�[���A�N�e�B�u������B
		/// </summary>
		void Activate()
		{
			m_isActive = true;
		}
		/// <summary>
		/// �t�H���g�����_�[���A�N�e�B�u������B
		/// </summary>
		/// <remark>
		/// ��A�N�e�B�u������ƁA���f�����`�悳��Ȃ��Ȃ�܂��B
		/// �ꎞ�I�ɕ\�����I�t�ɂ������ꍇ�ȂǂɎg�p���Ă��������B
		/// </remark>
		void Deactivate()
		{
			m_isActive = false;

		}
	private:
		Font m_PosX_font;

		std::wstring m_text = L"";							//�\������e�L�X�g
		Vector2 m_position = { 0.0f,0.0f };					//�\���ʒu
		Vector4 m_color = { 1.0f,1.0f,1.0f,1.0f };			//�t�H���g�̐F
		float m_rotation = 0.0f;							//�t�H���g�̉�]�p�x
		float m_scale = { 1.0f };					//�t�H���g�̊g�嗦
		Vector2 m_pivot = { 0.5f,0.5f };					//�t�H���g�̃s�{�b�g(�����Ă��Ȃ�?)
		Vector2 m_aspect = { 1.0f,1.0f };					//�t�H���g�̃A�X�y�N�g��(1���,2��ʂ̐؂�ւ��Ŏg��)
		bool m_drawShadowFlag = false;						//�t�H���g�̃A�E�g���C����`�����̃t���O
		float m_shadowOffset = 1.0f;						//�t�H���g�̃A�E�g���C���̑���
		Vector4 m_shadowColor = { 1.0f,1.0f,1.0f,1.0f };	//�t�H���g�̃A�E�g���C����

		bool m_isActive = true;						//�A�N�e�B�u�t���O
	};
}