#pragma once
#include <SFML/Graphics.hpp>
#include "SceneSerializer.h"
#include "UI_Text.h"
#include "UI_Button.h"
#include "Debug.h"

string SerializeVector(Vector2f vector);
string SerializeCharacterSize(int size);
string SerializeFontColor(sf::Color color);
string SerializeUI_Text(UI_Text ui_Text);
string SerializeUI_Button(UI_Button ui_Button);
vector<string> splitter(string s, string delimiter);

string SceneSerializer::SerializeScene(Scene& scene) // suppose we return the string that we will be writing to the file?
{
	// we write to a file objects and values

	string yaml = scene.GetScenePath() + "\n"; // maybe give it the scene name on the first line
	DebugLog("GameObjects");
	DebugLog(scene.GetGameObjects().size());

	for (const auto& object : scene.GetGameObjects()) // foreach
	{
		// sigh it's difuclt to dynamicly cast stuff, takes alot of messy ugly code

		string classType = typeid(*object).name();
		DebugLog(classType);
		if (object->GetComponents().size() == 0) continue;
		yaml += "GameObject\n";
		//yaml += SerializeVector(object->transform->getPosition()); // get read acces violation, propably a null pointer

		auto d = dynamic_cast<UI_Text&>(*object->GetComponents()[0]); // for now we only use ui and a gameobject will only ever have one component, ACCES VIOLATION
		DebugLog(typeid(d).name());
		if (typeid(d).name() == "class UI_Text")
		{
			DebugLog("We got class ui text");
			yaml += SerializeUI_Text(d);
			yaml += SerializeCharacterSize(d.getCharacterSize());
			yaml += SerializeFontColor(d.getFillColor());
		}
		else if (classType == "class UI_Button")
		{
			auto d = dynamic_cast<UI_Button&>(*object->GetComponents()[0]);
			yaml += SerializeUI_Button(d);
		}
	}
	//DebugLog(yaml);
	return yaml;
}


void SceneSerializer::DeSerializeScene(string& yaml)
{
	// we read from a file and create objects with values

	vector<string> objects = splitter(yaml, "GameObject");

	//// i have a string that has lines of text
	//// gameobjects starts with the line GameObject
	//// with that we can seperate how many objects there are and what they each contain
	//// say we seperate whole text into sections of each object 
	//// then we go through that text and find keywords, like vector and color and depending and what we find we will save to unique values and in the end create the gameObject with them

	for (const auto& object : objects) // it needs to be able to distinguish between objects though or well the component type
	{
		Vector2f v(0, 0);
		string text = "";
		Font font;
		if (!font.loadFromFile("BebasNeue-Regular.ttf")) DebugLog("Couldn't load font");
		int size = 0;
		sf::Color color = Color::White;
		if (object.find("vector") != string::npos) // all objects WILL contain a transformable which means they will have a vector
		{
			// ehm how the hell do i pull out the information now!?!?

		}
		if (object.find("character size") != string::npos) // suppose we could remove the previous found string from the string so it's quicker to search through, but yeah, alot of work, or do it differently
		{


		}
		if (object.find("color") != string::npos)
		{

		}
		if (object.find("vector") != string::npos)
		{

		}

		// for now im simply only using UI_Text

		UI_Text ui_Text = UI_Text(v, text, size);
		GameObject g = GameObject(ui_Text); // it has to know it's a text element to create the UI_Text
	}
}

string SerializeVector(Vector2f vector)
{
	return "vector(" + to_string(vector.x) + "," + to_string(vector.y) + ")\n";
}

string SerializeCharacterSize(int size)
{
	return "character size(" + to_string(size) + ")\n";
}

string SerializeFontColor(sf::Color color)
{
	return "color(" + to_string(color.r) + "," + to_string(color.g) + "," + to_string(color.b) + "," + to_string(color.a) + ")\n";
}

string SerializeUI_Text(UI_Text ui_Text)
{
	return ui_Text.getString() + "\n";
}

string SerializeUI_Button(UI_Button ui_Button)
{
	//return ui_Button.textValue + "\n";
	return "";
}

vector<string> splitter(string s, string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}