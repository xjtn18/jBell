#pragma once
#include <ctime>
#include <string>

#include <SFML/Graphics.hpp>
#include <Debug.hpp>

#define PI 3.14159265
#define is_in(x,y)   y.find(x) != y.end()
#define not_in(x,y)  y.find(x) == y.end()
#define DEV false


// global constants
extern sf::Font INCON_FONT;
void load_font();

const int WINW = 750;
const int WINH = 750;



namespace jb {

	struct Transform {
		static const Transform Zero;

		int x,y,w,h;
	};


	struct Time {
		int hour;
		int minute;

		Time()
			: hour(0), minute(0)
		{ }

		Time(int hr, int mn)
			: hour(hr), minute(mn)
		{ }

		bool operator ==(const Time& other) const { // equals operator
			return minute == other.minute && hour == other.hour;
		}

		Time(const Time& other) // copy constructor
			: hour(other.hour), minute(other.minute)
		{ }

		Time operator +(const int& increment) const {
			std::time_t sinceEpoch = std::time(nullptr);
			std::tm startT = *std::localtime(&sinceEpoch);
			startT.tm_hour = this->hour;
			startT.tm_min = this->minute;
			std::time_t t = std::mktime(&startT) + increment * 60;
			std::tm newT = *std::localtime(&t);
			return Time(newT.tm_hour, newT.tm_min);
		}

		friend std::ostream& operator <<(std::ostream& os, const Time& time){
			os << "Time(" << time.hour << "h, " << time.minute << "m)";
			return os;
		}

		operator std::string() const {
			std::string str;
			int hour_nonmilitary = (hour % 12 == 0) ? 12 : hour % 12;
			str += std::to_string(hour_nonmilitary) + ":";
			if (minute < 10){
				str += "0"; // add extra front zero
			}
			str += std::to_string(minute);
			str += (hour % 12 == hour) ? " am" : " pm";
			return str;
		}
	};



	enum Direc {
		UP = -1,
		TOP = 0,
		DOWN = 1,
		BOTTOM = 2
	};

	extern const char* rootPath;
	const Time current_time();
	const char* rtrim(const char* s, size_t len, const char target);
	const char* get_resource(const char* filename);
	const char* get_image(const char* filename);


	template <typename T, typename T2>
	bool clamp(T& value, T low, T2 high){
		// clamps the value to the bounds given, returns true if value was constrained, false otherwise.
		if (value < low){
			value = low;
			return true;
		} else if (value >= high){
			value = (high - 1 >= low) ? high - 1 : high;
			return true;
		}
		return false;
	}

}


