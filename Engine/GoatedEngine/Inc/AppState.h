#pragma once

namespace GoatedEngine
{
	class AppState
	{
	public:
		// if function is = 0; then is becomes an abstract class and the child class must override the function
		virtual ~AppState() = default;
		virtual void Initialize() {}
		virtual void Terminate() {}
		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}
	};
}