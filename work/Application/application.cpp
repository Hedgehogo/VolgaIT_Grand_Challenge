#include "application.hpp"

double distance(Location first, Location second) {
	return std::sqrt(std::pow(second.x - first.x, 2) + std::pow(second.y - first.y, 2));
}

SHORT Application::floor(double v) //
{
	return static_cast<SHORT>(std::floor(v));
}

void Application::print(Location location, char c) {
	// Напечатать символ на координатах
	const auto x = floor(location.x);
	const auto y = floor(location.y);
	
	if (x > 0 && y > 0) {
		COORD pos = { x, y };
		SetConsoleCursorPosition(mOutput, pos);
		std::cout << c;
	}
	
	mLocations.push_back(location);
}

void Application::print(Location location, std::string str) {
	for(char i : str) {
		print(location, i);
		location.x += 1;
	}
}

void Application::print(Location location, Direction size, char c) {
	Location end{location.x + size.dx, location.y + size.dy};
	for(int y = floor(location.y); y < floor(end.y); ++y) {
		for(int x = floor(location.x); x < floor(end.x); ++x) {
			print(Location{static_cast<double>(x), static_cast<double>(y)}, c);
		}
	}
}

void Application::print(Location location, double radius, char c) {
	Location start{std::max(location.x - radius, 0.0), std::max(location.y - radius, 0.0)};
	Location end{location.x + radius, location.y + radius};
	
	for(int y = floor(start.y); y < floor(end.y); ++y) {
		for(int x = floor(start.x); x < floor(end.x); ++x) {
			Location pointLocation{static_cast<double>(x), static_cast<double>(y)};
			if(distance(location, pointLocation) < radius)
				print(pointLocation, c);
		}
	}
}

int Application::getchar() {
	// Читаем один символ если такие там есть
	if (_kbhit())
		return _getch();
	return 0;
}

bool Application::runAuto(State &state) {
	// Мой автомобиль
	auto& car = state.getCar();
	
	// Текущее время (номер итерации или число уже вызванных run) и его лимит (лимит итераций)
	const auto currentTime = state.getCurrentTime();
	const auto timeLimit = state.getTimeLimit();
	
	// Текущее положение, направление и скорость
	const auto location = car.getLocation();
	const auto direction = car.getDirection();
	const auto speed = car.getCurrentSpeed();
	
	// Полосы движения
	for (const auto& lane : state.getRoad().getLanes()) {
		// Положение, ширина и длина дороги
		const auto laneLocation = lane.getLocation();
		const auto laneDirection = lane.getDirection();
	}
	
	// Дорожные знаки
	for (const auto& trafficSign : state.getRoad().getSigns()) {
		// Знак и значение на нем
		const auto type = trafficSign.getType();
		const auto value = trafficSign.getValue();
		// Индекс полосы и положение знака относительно ее начала
		const auto index = trafficSign.getLaneIndex();
		const auto location = trafficSign.getLocation();
		
		switch (type) {
			case TrafficSign::MinimumSpeedLimit:
				// Ограничение минимальной скорости
				break;
			
			case TrafficSign::MaximumSpeedLimit:
				// Ограничение максимальной скорости
				break;
		}
	}
	
	// Другие учатники движения
	for (const auto& vehicle : state.getVisibleVehicles()) {
		const auto otherLocation = vehicle.getLocation();
	}
	
	// Точка назначения
	const auto destination = car.getDestination();
	
	if(distance(destination.center, location) - destination.radius > 0) {
		// Разгоняемся до максимальной скорости
		car.setGear(Gear::Forward);
		if(speed < 1) {
			car.pressGasPedal(1);
		}
		
		// Поворачиваем в сторону точки назначения
		double currentAngle = std::atan((destination.center.y - location.y) / (destination.center.x - location.x));
		double desiredAngle = std::atan(direction.dy / direction.dx);
		if(currentAngle > desiredAngle) {
			car.turnSteeringWheel(std::abs(currentAngle - desiredAngle) / speed);
		} else {
			car.turnSteeringWheel(-std::abs(currentAngle - desiredAngle) / speed);
		}
	} else {
		// Тормозим
		car.setGear(Gear::Neutral);
		car.pressBrakePedal(1);
	}
	
	// Пробуем что-то отобразить
	draw(state);
	
	// Продолжаем работу если мы не достигли точки назначения или не столкнулись
	return !state.isDestination() && !state.isCollision();
}

bool Application::runManually(State &state) {
	// Мой автомобиль
	auto& car = state.getCar();
	
	// Текущее положение, направление и скорость
	const auto speed = car.getCurrentSpeed();
	
	// Очищаем значения с прошлого кадра
	car.turnSteeringWheel(0);
	car.setGear(Gear::Neutral);
	
	// Читаем клавиши с клавиатуры
	std::vector<int> keyList;
	int key = getchar();
	while(key != 0) {
		keyList.push_back(key);
		key = getchar();
	}
	
	// Определяем, что пользователь хотел сделать
	if(std::find(keyList.begin(), keyList.end(), Key::W) != keyList.end())
		car.setGear(Gear::Forward);
	if(std::find(keyList.begin(), keyList.end(), Key::S) != keyList.end())
		car.setGear(Gear::Reverse);
	for(const auto &key: keyList) {
		switch(key) {
			case Key::A:
				car.turnSteeringWheel(-1);
				break;
			case Key::D:
				car.turnSteeringWheel(1);
				break;
			case Key::E:
				carSpeed += 0.1;
				break;
			case Key::Q:
				carSpeed -= 0.1;
				break;
			default:
				break;
		}
		if(carSpeed > speed) {
			car.pressGasPedal(1);
		} else if(carSpeed < speed) {
			car.pressBrakePedal(1);
		}
	}
	
	// Пробуем что-то отобразить
	draw(state);
	
	// Продолжаем работу если мы не достигли точки назначения или не столкнулись
	return !state.isDestination() && !state.isCollision();
}

Application::Application(Mode mode) : mMode(mode), mOutput(GetStdHandle(STD_OUTPUT_HANDLE)) {
	if (mMode == Mode::HandsOff) {
		// Режим автоматического управления
	}
	if (mMode == Mode::HandsOn) {
		// Режим ручного управления
	}
}

bool Application::run(State &state) {
	/*if (const auto c = getchar(); c == ' ')
		return false;*/
	
	// Запускаем определенный режим
	if (mMode == Mode::HandsOn) {
		return runManually(state);
	} else {
		return runAuto(state);
	}
}

void Application::draw(State &state) {
	// Очищаем старые символы
	{
		for (auto location : mLocations)
			print(location, ' ');
		
		mLocations.clear();
	}
	
	// Добавляем дорогу
	{
		auto laneList = state.getRoad().getLanes();
		for(auto &lane: laneList) {
			Location location{lane.getLocation()};
			Direction direction{lane.getDirection()};
			location.y += 2;
			double endY = location.y + direction.dy;
			if(static_cast<int>(endY) - static_cast<int>(location.y) > 1) {
				print(location, direction, ':');
			} else {
				print(location, direction, '=');
			}
		}
		
		auto signList = state.getRoad().getSigns();
		for(auto &sign: signList) {
			Location laneLocation{laneList[sign.getLaneIndex()].getLocation()};
			Location signLocation{state.getRoad().getLanes()[sign.getLaneIndex()].getLocation()};
			signLocation = {laneLocation.x + signLocation.x, laneLocation.y + signLocation.y + 2};
			if(sign.getType() == TrafficSign::Type::MaximumSpeedLimit) {
				print(signLocation, '-');
			} else {
				print(signLocation, '+');
			}
		}
	}
	
	// Добавляем точку назначения
	{
		Circle destination = state.getCar().getDestination();
		print(destination.center, destination.radius, '*');
	}
	
	// Добавляем остальные автомобили
	for (const auto& vehicle : state.getVisibleVehicles()) {
		auto location = vehicle.getLocation();
		location.y += 2;
		print(location, 'o');
	}
	
	// Добавляем символ и скорость автомобиля
	{
		const auto& car = state.getCar();
		auto location = car.getLocation();
		location.y += 2;
		print(location, '@');
		std::string speed = std::to_string(car.getCurrentSpeed());
		print(Location{1, 1},
			  std::string("current speed: ") + std::to_string(car.getCurrentSpeed()) +
			  std::string(" desired speed: ") + std::to_string(carSpeed));
	}
}
