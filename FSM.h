
class Menu 
{
	private:
		Sensor* sensor = new Sensor[16];
		int curSensor;

	public:
		Menu();
		void newSensor();
		void deleteSensor();

		void input();
		void right();
		void left();
		void record();
		void select();
}