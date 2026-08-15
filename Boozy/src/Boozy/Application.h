#pragma once
#include "Core.h"

namespace Boozy {

	class BOOZY_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// 在客户端中定义
	Application* CreateApplication();

}