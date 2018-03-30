#pragma once

namespace Editor
{
	ref class RecourcesWindow;
	ref class RunningEnvironmentWindow;
	ref class PropertiesWindow;

	ref class Globals
	{
	public:
		RecourcesWindow^ m_recourcesWindow;
		RunningEnvironmentWindow^ m_runningEnvironmentWindow;
		PropertiesWindow^ m_propertiesWindow;
	};
};
