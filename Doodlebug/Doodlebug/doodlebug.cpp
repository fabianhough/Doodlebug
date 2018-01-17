#include <iostream>
#include <cstdlib>	//For random number function
#include <ctime>
using namespace std;

/*
Program that simulates a predator vs prey world
Has Ants and Doodlebugs
*/

const int XBOUND = 20;	//Constant variables for world size
const int YBOUND = 20;
const int ANTPOP = 100;	//Constant variables for initial population sizes
const int DOODLEPOP = 5;

//Organism object that can move, breed, starve
class Organism
{
private:
	int breedTimer;	//Breed value holder
	int starveTimer;	//Starve value holder
	bool flag;	//Flag that determines whether an action has taken place or not

public:
	Organism() :breedTimer(0), starveTimer(0), flag(false) {}	//Default constructor

																//Accessors
	int get_breedTime() const { return breedTimer; }
	bool get_flag() const { return flag; }
	int get_starveTime() const { return starveTimer; }

	//Mutators
	void set_breedTimer(int newTime) { breedTimer = newTime; }
	void set_flag(bool newflag) { flag = newflag; }
	void set_starveTimer(int newTime) { starveTimer = newTime; }

	void incrementBreedTimer() { breedTimer++; }	//Incrementers for each timer
	void incrementStarveTimer() { starveTimer++; }
	virtual void move(Organism*** map, int xpos, int ypos);	//Moves the organism, virtual
	virtual void breed(Organism*** map, int xpos, int ypos);	//Breeds the organism, basically pure virtual
	virtual char radar() { return '-'; }	//Function that returns the identifier for the organism

};

//Inherited class that acts like an Ant
class Ant : public Organism
{
public:
	Ant() : Organism() {}	//Default constructor

	void breed(Organism*** map, int xpos, int ypos);	//Breeds after 3 time steps
	char radar() { return 'o'; }	//Returns the Ant specific identifier
};

//Inherited class that acts like a Doodlebug
class Doodlebug : public Organism
{
public:
	Doodlebug() : Organism() {}	//Default constructor

	void move(Organism*** map, int xpos, int ypos);	//Moves the Doodlebug, lets it eat ants, and figures out if they starve
	void breed(Organism*** map, int xpos, int ypos);	//Breeds the Doodlebug after 8 time steps
	char radar() { return 'X'; }	//Returns the Doodlebug specific identifier
};

//Function prototypes
void huntWorld();
void createLife(Organism*** map);
void printWorld(Organism*** map);
void movement(Organism*** map);
void breeding(Organism*** map);

int main() {

	huntWorld();	//Executes the world program

	return 0;
}

//Function that is a predator-prey simulation
void huntWorld() {
	srand(time(0));	//Sets seed for random number

	Organism*** map = new Organism**[XBOUND];	//Array of arrays of Organism pointers
	for (int i = 0; i < XBOUND; i++)
		map[i] = new Organism*[YBOUND];
	for (int i = 0; i < XBOUND; i++) {
		for (int j = 0; j < YBOUND; j++)
			map[i][j] = new Organism();	//Default for all values is an Organism
	}

	createLife(map);	//Creates life in the world

	while (true) {	//Continues for all time
		printWorld(map);	//Prints the status of the current map
		movement(map);	//Moves all organisms; also lets Doodlebugs eat Ants, and starve
		breeding(map);	//Breeds all organisms
		system("pause");	//Pauses at each time step
	}
}

//Function that randomly places a given value of Organisms
void createLife(Organism*** map) {
	int ant_total = ANTPOP;	//Total values for each population
	int doodle_total = DOODLEPOP;

	while (ant_total > 0) {	//Continues until the total Ant population hits the default value
		int xpos = rand() % XBOUND;	//Randomizes location
		int ypos = rand() % YBOUND;

		if (map[xpos][ypos]->radar() == '-') {	//Places Ant there if a valid 'empty' location
			map[xpos][ypos] = new Ant();
			ant_total--;
		}
	}

	while (doodle_total > 0) {	//Also randomizes Doodlebug locations
		int xpos = rand() % XBOUND;
		int ypos = rand() % YBOUND;

		if (map[xpos][ypos]->radar() == '-') {
			map[xpos][ypos] = new Doodlebug();
			doodle_total--;
		}
	}
}

//Function that prints out the current layout of the world
void printWorld(Organism*** map) {
	for (int j = 0; j < YBOUND; j++) {
		for (int i = 0; i < XBOUND; i++)
			cout << map[i][j]->radar() << "  ";	//Prints out the identifier of each object
		cout << endl;
	}
}

//Function that moves all Organisms
void movement(Organism*** map) {
	for (int i = 0; i < XBOUND; i++) {	//Doodlebugs move first
		for (int j = 0; j < YBOUND; j++) {
			if ((map[i][j]->radar() == 'X') && (map[i][j]->get_flag() == false)) {	//Only moves them if they are a Doodlebug, and they have not been moved yet
				map[i][j]->move(map, i, j);	//Moves the Doodlebug, and allows it to eat Ants
			}
		}
	}

	for (int i = 0; i < XBOUND; i++) {	//Ants move second, in a similar set of circumstances
		for (int j = 0; j < YBOUND; j++) {
			if ((map[i][j]->radar() == 'o') && (map[i][j]->get_flag() == false))
				map[i][j]->move(map, i, j);
		}
	}

	for (int i = 0; i < XBOUND; i++) {	//Runs through entire array again
		for (int j = 0; j < YBOUND; j++) {
			map[i][j]->set_flag(false);	//Resets all flags
			if (map[i][j]->get_starveTime() == 3) {	//Starves the Doodlebug if its starveTimer has hit the critical value
				delete map[i][j];
				map[i][j] = new Organism();
			}
		}
	}
}

//Function that conducts all breeding operations
void breeding(Organism*** map) {
	for (int i = 0; i < XBOUND; i++) {
		for (int j = 0; j < YBOUND; j++) {
			if ((map[i][j]->radar() == 'o') && (map[i][j]->get_breedTime() >= 3))
				map[i][j]->breed(map, i, j);	//Breeds Ants
			else if ((map[i][j]->radar() == 'X') && (map[i][j]->get_breedTime() >= 8))
				map[i][j]->breed(map, i, j);	//Breeds Doodlebugs
		}
	}
}


//Default move operation for all Organisms
void Organism::move(Organism*** map, int xpos, int ypos) {
	int direction = rand() % 4;	//Picks a random direction
	int newx = xpos, newy = ypos;	//Possible new location for organism

	switch (direction)
	{
	case 0:	//Left
		newx--;
		break;
	case 1:	//Right
		newx++;
		break;
	case 2:	//Down
		newy--;
		break;
	case 3:	//Up
		newy++;
		break;
	default:
		break;
	}

	if ((newx >= 0) && (newx < XBOUND) && (newy >= 0) && (newy < YBOUND)) {	//Only if the new location is within bounds
		if (map[newx][newy]->radar() == '-') {	//Only if the new location is empty
			delete map[newx][newy];	//Moves the object
			map[newx][newy] = map[xpos][ypos];
			map[xpos][ypos] = new Organism();
		}
	}

	set_flag(true);	//Flag that says the Organism was moved

	breedTimer++;	//Breed time step incrementation
}

//Empty virtual function
void Organism::breed(Organism*** map, int xpos, int ypos) {

}

//Breeds new ants
void Ant::breed(Organism*** map, int xpos, int ypos) {
	int newx = xpos, newy = ypos;	//Potential new location for spawn

									//Checks each direction for a valid place to put spawn
	if ((xpos - 1 >= 0) && (map[xpos - 1][ypos]->radar() == '-')) {
		newx--;
	}
	else if ((xpos + 1 < XBOUND) && (map[xpos + 1][ypos]->radar() == '-')) {
		newx++;
	}
	else if ((ypos + 1 < YBOUND) && (map[xpos][ypos + 1]->radar() == '-')) {
		newy++;
	}
	else if ((ypos - 1 >= 0) && (map[xpos][ypos - 1]->radar() == '-')) {
		newy--;
	}

	if ((newx != xpos) || (newy != ypos)) {	//Places new spawn if new location is found
		delete map[newx][newy];
		map[newx][newy] = new Ant();
		set_breedTimer(0);
	}
}

//Move and eat operation for Doodlebugs
void Doodlebug::move(Organism*** map, int xpos, int ypos) {
	int newx = xpos, newy = ypos;	//Potential locations for Ants

									//Checks each direction for an Ant
	if ((xpos - 1 >= 0) && (map[xpos - 1][ypos]->radar() == 'o')) {
		newx--;
	}
	else if ((xpos + 1 < XBOUND) && (map[xpos + 1][ypos]->radar() == 'o')) {
		newx++;
	}
	else if ((ypos + 1 < YBOUND) && (map[xpos][ypos + 1]->radar() == 'o')) {
		newy++;
	}
	else if ((ypos - 1 >= 0) && (map[xpos][ypos - 1]->radar() == 'o')) {
		newy--;
	}

	if ((newx != xpos) || (newy != ypos)) {	//"Eats" the Ant, and takes its place
		delete map[newx][newy];
		map[newx][newy] = map[xpos][ypos];
		map[xpos][ypos] = new Organism();
		set_flag(true);	//Set flag that Doodlebug has moved
		set_starveTimer(0);	//Resets the Starve Timer because the Doodlebug has just ate
	}
	else {
		Organism::move(map, xpos, ypos);	//Executes default move operation
		incrementStarveTimer();	//Increments the starveTimer
	}

	incrementBreedTimer();	//Increments the Breed Timer
}

//Breeds more Doodlebugs
void Doodlebug::breed(Organism*** map, int xpos, int ypos) {
	int newx = xpos, newy = ypos;	//Potential new location for spawn

									//Checks each direction for valid empty space
	if ((xpos - 1 >= 0) && (map[xpos - 1][ypos]->radar() == '-')) {
		newx--;
	}
	else if ((xpos + 1 < XBOUND) && (map[xpos + 1][ypos]->radar() == '-')) {
		newx++;
	}
	else if ((ypos + 1 < YBOUND) && (map[xpos][ypos + 1]->radar() == '-')) {
		newy++;
	}
	else if ((ypos - 1 >= 0) && (map[xpos][ypos - 1]->radar() == '-')) {
		newy--;
	}

	if ((newx != xpos) || (newy != ypos)) {	//Spawns new Doodlebug if a valid location is found
		delete map[newx][newy];
		map[newx][newy] = new Doodlebug();
		set_breedTimer(0);	//Resets the Breed Timer
	}
}
