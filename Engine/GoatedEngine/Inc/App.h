#pragma once

namespace GoatedEngine
{
	class AppState;

	struct AppConfig
	{
		std::wstring appName = L"AppName";
		uint32_t winWidth = 1200;
		uint32_t winHeight = 720;
		bool fullscreen = false;
	};

	class App final
	{
	public:
		void Run(const AppConfig& config);
		void Quit();

		template<class StateType>
		void AddState(const std::string& stateName)
		{
			static_assert(std::is_base_of_v<AppState, StateType>,
				"App: AddState must be of type AppState");

			auto [iter, success] = mAppStates.try_emplace(stateName, nullptr);
			if (success)
			{
				auto& ptr = iter->second;
				ptr = std::make_unique<StateType>();
				if(mCurrentState == nullptr)
				{
					LOG("App: Current state %s", stateName.c_str());
					mCurrentState = ptr.get();
				}
			}

		}

		void ChangeState(const std::string& stateName);
	private:
		bool mRunning = false;
		using AppStateMap = std::map<std::string, std::unique_ptr<AppState>>;
		// list of all available states in the app
		AppStateMap mAppStates;
		// the current running state 
		AppState* mCurrentState = nullptr;
		// the desired next state 
		AppState* mNextState = nullptr;
	};
}