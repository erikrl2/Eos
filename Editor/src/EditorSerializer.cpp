#include "eospch.h"
#include "EditorSerializer.h"

#include "EditorStyle.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace Eos {

	static std::string ThemeTypeToString(Theme theme)
	{
		switch (theme)
		{
			case Theme_Dark1:			return "Dark1";
			case Theme_Dark2:			return "Dark2";
			case Theme_Dark3:			return "Dark3";
		}

		EOS_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static Theme ThemeTypeFromString(const std::string& themeString)
	{
		if (themeString == "Dark1")				return Theme_Dark1;
		if (themeString == "Dark2")				return Theme_Dark2;
		if (themeString == "Dark3")				return Theme_Dark3;

		EOS_CORE_ASSERT(false, "Unknown body type");
		return Theme_Dark1;
	}

	static std::string FontTypeToString(Font font)
	{
		switch (font)
		{
			case Font_OpenSansRegular:		return "OpenSans-Regular";
			case Font_RobotoMedium:			return "Roboto-Medium";
			case Font_RudaRegular:			return "Ruda-Regular";
		}

		EOS_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static Font FontTypeFromString(const std::string& fontString)
	{
		if (fontString == "OpenSans-Regular")		return Font_OpenSansRegular;
		if (fontString == "Roboto-Medium")			return Font_RobotoMedium;
		if (fontString == "Ruda-Regular")			return Font_RudaRegular;

		EOS_CORE_WARN("Unknown body type");
		return Font_OpenSansRegular;
	}

	EditorSerializer::EditorSerializer(EditorSettings* settings)
		: m_Settings(settings)
	{
	}

	void EditorSerializer::Serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Theme" << YAML::Value << ThemeTypeToString(m_Settings->Theme);
		out << YAML::Key << "Font" << YAML::Value << FontTypeToString(m_Settings->Font);
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool EditorSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e)
		{
			return false;
		}

		auto theme = data["Theme"];
		if (theme)
			m_Settings->Theme = ThemeTypeFromString(theme.as<std::string>());

		auto font = data["Font"];
		if (font)
			m_Settings->Font = FontTypeFromString(font.as<std::string>());

		return true;
	}

}
