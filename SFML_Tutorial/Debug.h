#pragma once
#include <SFML/Graphics.hpp>
#include "Pch.h"
#include "BoxCollider.h"

// dude, so cool!!
#ifdef GE_DEBUG
#define DebugLog(x) Debug::GetInstance()->Log(x) // I do wonder if this actually affects performance where you first call getInstance and then log, as it does check if an instance of the class exist each time
#define DebugFrameRate(x) Debug::GetInstance()->DisplayFrameRate(&x) 
//#define DebugMemory() Debug::GetInstance()->DisplayMemory() 
#else
#define DebugLog(x) 
#define DebugFrameRate(x) 
#endif

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


	inline void Log(float line)
	{
		Log(to_string(line));
	}
	inline void Log(double line)
	{
		Log(to_string(line));
	}
	inline void Log(long double line)
	{
		Log(to_string(line));
	}
	inline void Log(int line)
	{
		Log(to_string(line));
	}
	inline void Log(unsigned int line)
	{
		Log(to_string(line));
	}
	inline void Log(long long line)
	{
		Log(to_string(line));
	}
	inline void Log(long int line)
	{
		Log(to_string(line));
	}
	inline void Log(size_t line)
	{
		Log(to_string(line));
	}
	inline void Log(const Vector2f& line)
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
	inline void Log(FloatRect line)
	{
		Log(string_view("x: " + to_string(line.width) + ", y: " + to_string(line.height)));
	}
	inline void Log(Time line)
	{
		Log(string_view("Milliseconds: " + to_string(line.asMilliseconds())));
	}

	inline void Log(BoxCollider& line)
	{
		Log(string_view(line.ToString()));
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

	void DisplayMemory()
	{
	}

	void DisplayFrameRate(Time* _time)
	{
		Int32 ms = _time->asMilliseconds();
		duration += ms;

		if (duration < 1000)
			fpsCounter++;

		else if (duration >= 1000)
		{
			double averageFps = (1000.0f / duration) * fpsCounter;

			string averagefp = "fps: " + to_string(int(averageFps));
			Log(averagefp);
			duration = fpsCounter = 0;
		}
	}

protected:
	static Debug* _debug;

	vector<TextLog> ConsoleLogs = vector<TextLog>();

	Debug() {};

	int duration = 0;
	int fpsCounter = 0;

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

	void WriteLine(string_view line)
	{
		cout << "\n";
		cout << line;
	}

};
