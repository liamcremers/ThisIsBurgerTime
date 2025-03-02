#pragma once  
#include "BaseComponent.h"  

namespace dae
{
	class ThrashTheCacheComponent final : public BaseComponent
	{
	public:
		ThrashTheCacheComponent(GameObject& owner,int windowWidth, int windowHeigth);
		void RenderUI() const override;
	private:
		void RenderExercise1Window() const;
		void RenderExercise2Window() const;

		//not liking this mutable, but it's the only way to get the ImGui buttons to work
		mutable int m_SamplesExercise1 = 10;
		mutable int m_SamplesExercise2 = 100;

		int m_WindowWidth;
		int m_WindowHeight;
	};
}
