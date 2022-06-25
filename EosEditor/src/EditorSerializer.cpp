#include "eospch.h"
#include "EditorSerializer.h"

#include "EditorStyle.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace Eos {

	static std::string ThemeTypeToString(Style::Theme theme)
	{
		switch (theme)
		{
			case Style::Theme::Dark1:			return "Dark1";
			case Style::Theme::Dark2:			return "Dark2";
			case Style::Theme::Dark3:			return "Dark3";
			case Style::Theme::Dark4:			return "Dark4";
			case Style::Theme::Dark5:			return "Dark5";
			case Style::Theme::Dark6:			return "Dark6";
			case Style::Theme::Dark7:			return "Dark7";
			case Style::Theme::Photoshop:		return "Photoshop";
			case Style::Theme::SonicRiders:		return "SonicRiders";
			case Style::Theme::Unreal:			return "Unreal";
			case Style::Theme::VisualStudio:	return "VisualStudio";
			case Style::Theme::DarkRuda:		return "DarkRuda";
		}

		EOS_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static Style::Theme ThemeTypeFromString(const std::string& themeString)
	{
		if (themeString == "Dark1")				return Style::Theme::Dark1;
		if (themeString == "Dark2")				return Style::Theme::Dark2;
		if (themeString == "Dark3")				return Style::Theme::Dark3;
		if (themeString == "Dark4")				return Style::Theme::Dark4;
		if (themeString == "Dark5")				return Style::Theme::Dark5;
		if (themeString == "Dark6")				return Style::Theme::Dark6;
		if (themeString == "Dark7")				return Style::Theme::Dark7;
		if (themeString == "Photoshop")			return Style::Theme::Photoshop;
		if (themeString == "SonicRiders")		return Style::Theme::SonicRiders;
		if (themeString == "Unreal")			return Style::Theme::Unreal;
		if (themeString == "VisualStudio")		return Style::Theme::VisualStudio;
		if (themeString == "DarkRuda")			return Style::Theme::DarkRuda;

		EOS_CORE_ASSERT(false, "Unknown body type");
		return Style::Theme::Dark1;
	}

	static std::string FontTypeToString(Style::Font font)
	{
		switch (font)
		{
			case Style::Font::OpenSansRegular:		return "OpenSans-Regular";
			case Style::Font::OpenSansBold:			return "OpenSans-Bold";
			case Style::Font::RobotoMedium:			return "Roboto-Medium";
			case Style::Font::RudaRegular:			return "Ruda-Regular";
			case Style::Font::RudaBold:				return "Ruda-Bold";
		}

		EOS_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static Style::Font FontTypeFromString(const std::string& fontString)
	{
		if (fontString == "OpenSans-Regular")		return Style::Font::OpenSansRegular;
		if (fontString == "OpenSans-Bold")			return Style::Font::OpenSansBold;
		if (fontString == "Roboto-Medium")			return Style::Font::RobotoMedium;
		if (fontString == "Ruda-Regular")			return Style::Font::RudaRegular;
		if (fontString == "Ruda-Bold")				return Style::Font::RudaBold;

		EOS_CORE_WARN("Unknown body type");
		return Style::Font::OpenSansRegular;
	}

	EditorSerializer::EditorSerializer(EditorSettings* settings)
		: m_Settings(settings)
	{
	}

	void EditorSerializer::Serialize(const std::filesystem::path& filepath)
	{
		EditorSettings& settings = *m_Settings;

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Theme" << YAML::Value << ThemeTypeToString(settings.Theme);
		out << YAML::Key << "Font" << YAML::Value << FontTypeToString(settings.Font);
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

		EditorSettings& settings = *m_Settings;

		auto theme = data["Theme"];
		if (theme)
			settings.Theme = ThemeTypeFromString(theme.as<std::string>());

		auto font = data["Font"];
		if (font)
			settings.Font = FontTypeFromString(font.as<std::string>());

		return true;
	}

}
