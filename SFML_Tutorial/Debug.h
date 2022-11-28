#pragma once
#include <SFML/Graphics.hpp>
#include "Pch.h"

using namespace sf;
using namespace std;

struct TextLog
{
	string text;
	int index;
};

class Debug
{
public:
	Debug(Debug& other) = delete;

	void operator=(const Debug&) = delete;

	static Debug* GetInstance(const string& value);

	string value() const {
		return value_;
	}

	void Log(float line) 
	{
		Log(to_string(line));
	}
	void Log(double line) 
	{
		Log(to_string(line));
	}
	void Log(int line) 
	{
		Log(to_string(line));
	}	
	void Log(size_t line)
	{
		Log(to_string(line));
	}
	void Log(Vector2f& line) // apparently it is impossible to send my own added classes, "Gameobject undeclared identifier" i HAVE tried to declared it
	{
		Log(string_view("x: " + to_string(line.x) + ", y: " + to_string(line.y)));
	}	
	void Log(FloatRect line) // apparently it is impossible to send my own added classes, "Gameobject undeclared identifier" i HAVE tried to declared it
	{
		Log(string_view("x: " + to_string(line.width) + ", y: " + to_string(line.height)));
	}

	void Log(string_view line)
	{
		Clear();

		bool increasedCountOnLogText = false;

		for (size_t i = 0; i < ConsoleLogs.size(); i++)
		{
			if (ConsoleLogs[i].text == line)
			{
				ConsoleLogs[i].index++;
				increasedCountOnLogText = true;
				break;
			}
		}

		if (!increasedCountOnLogText)
		{
			TextLog t;
			t.text = line;
			t.index = 1;
			ConsoleLogs.push_back(t);
		}

		WriteAllLines();
	}

protected:
	string value_;
	static Debug* _debug;

	vector<TextLog> ConsoleLogs = vector<TextLog>();

	Debug(const string& value) : value_(value) {}; // constructor

	void Clear()
	{
		cout << "\x1B[2J\x1B[H";
	}

	void WriteAllLines() // 
	{
		for (size_t i = 0; i < ConsoleLogs.size(); i++)
		{
			if (ConsoleLogs[i].index <= 1)
			{
				WriteLine(ConsoleLogs[i].text);
			}
			else
			{
				WriteLine(ConsoleLogs[i].text + " (" + to_string(ConsoleLogs[i].index) + ")");
			}
		}
	}

	void WriteLine(string line)
	{
		cout << "\n" + line;
	}

};
