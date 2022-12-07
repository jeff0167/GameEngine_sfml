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

	static Debug* GetInstance();

	void Log(float line) 
	{
		Log(to_string(line));
	}
	inline void Log(double line) 
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
	inline void Log(Vector2f& line) // apparently it is impossible to send my own added classes, "Gameobject undeclared identifier" i HAVE tried to declared it
	{
		Log(string_view("x: " + to_string(line.x) + ", y: " + to_string(line.y)));
	}	
	inline void Log(Vector2i& line) 
	{
		Log(string_view("x: " + to_string(line.x) + ", y: " + to_string(line.y)));
	}	
	inline void Log(Vector2u& line) 
	{
		Log(string_view("x: " + to_string(line.x) + ", y: " + to_string(line.y)));
	}	
	void Log(FloatRect line) 
	{
		Log(string_view("x: " + to_string(line.width) + ", y: " + to_string(line.height)));
	}

	inline void Log(string_view line)
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
	double fps_ms = 0;
	Int32 ms;
	vector<double> fpsTimes;

	void DisplayFrameRate(Time _time) // would also like to show memory allocations, show in game, toggle
	{
		if (fps_ms < 1000)
		{
			ms = _time.asMilliseconds();
			fps_ms += ms;
			fpsTimes.push_back(ms); // show fps, with debug info or something
		}
		else if (fps_ms >= 1000)
		{
			double totalTime = 0; // technically want be 1, but around there
			for (size_t i = 0; i < fpsTimes.size(); i++)     // 1.1 s 60 frames
			{
				totalTime += fpsTimes[i];
			}
			double averageFps = (1000 / totalTime) * fpsTimes.size();  // this is not really necesary at high refesh rate, the higher the rate the closer to 1000 the less deviance

			string averagefp = "fps: " + to_string(int(averageFps)); // it's like 1 fps off when having 100 fps
			Log(averagefp);
			fpsTimes.clear(); // well i do want to clear the values
			fps_ms = 0;
		}
	}

protected:
	static Debug* _debug;

	vector<TextLog> ConsoleLogs = vector<TextLog>();

	Debug() {}; 

	void Clear()
	{
		cout << "\x1B[2J\x1B[H"; // this doesn't actually clear the window altough it makes it look like it
	}

	void WriteAllLines() 
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
