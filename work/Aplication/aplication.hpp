#pragma once
#include "../../emulator/Emulator.hpp"

#include <conio.h>
#include <windows.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace Key {
	static const int W = 119;
	static const int S = 115;
	static const int A = 97;
	static const int D = 100;
	static const int Q = 113;
	static const int E = 101;
}

double distance(Location first, Location second);

class Application {
private:
	static SHORT floor(double v);
	
	void print(Location location, char c);
	
	void print(Location location, std::string str);
	
	void print(Location location, Direction size, char c);
	
	void print(Location location, double radius, char c);
	
	int getchar();
	
	bool runAuto(State& state);
	
	bool runManually(State& state);

public:
	explicit Application(Mode mode);
	
	bool run(State& state);
	
	void draw(State& state);

private:
	HANDLE mOutput;
	Mode mMode;
	std::vector<Location> mLocations;
	double carSpeed;
};