// Programmed by Jonathan Alley
// asbg.cpp - Primary runtime source file
// A Simple Battle Game
// Uses classes for the instantion of Player, Enemy, Screen, and Game
// objects and methods
// player and enemy classes, and randnum() are defined in game.cpp
// The screen class is defined in asbg.h

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>
#include "asbg.h"
#include "game.cpp"

using namespace std;

// Initialize a global display class that provides methods
// for all class objects to access
// See implementation in asbg.h for details
screen display;

// Defines the class game CurGame, a collection of routines centering
// around a prototypical game engine that updates once per turn.

class game
{
	public:
	
	// Input files for various data
	//
	// EnemDB and tempDB are the streams to all monsters to be
	// loaded, in the format of [MonsterName] [Difficulty], in
	// monsters.txt
	// PlayDB and save are the streams to access the primary character 
	// savefile data in players.txt
	ifstream EnemDB, PlayDB;
	ofstream tempDB, save;
	
	// Strings to store the accessed monster and player names
	string monsters[255], pnames[255];
	
	// Variables for database and stat generation
	int DBsize, savesize, erand, diff[255], pstats[255][8];
	
	// Instantiates a player Play1 in the game class so the classes
	// objects and methods are useful. See implementation in game.cpp
	// for details
	player Play1;
	
	// Instantiates a enemy Enem1 in the game class so the classes 
	// objects and methods are useful. See implementation in game.cpp
	// for details.
	enemy Enem1;
	
	// CurGame's primary method, this encapsulates the entire battle
	// routine, the routine ends as soon as the enemy or the player
	// reach 0 or less hit points. Returns a boolean to determine if
	// the player died during combat or an attempt to run away.
	bool battle()
	{
		// Initialize battle()'s variables to NULL
		// maxhp stores the players original max health so the value
		// can be restored if the player survives the encounter
		int select=0, maxhp=Play1.health;
		bool playDeath=false, enemDeath=false;
		
		// Keep fighting until the player runs or someone dies
		do
		{
			// Clear the display to draw the next frame
			display.Clear();
			
			// Display the battle screen
			select = battleMenu();
			
			// Check which option was chosen
			// The enemy is implemented without AI and will follow
			// suit with the player, a simple AI would evaluate 
			// which of its stats are greater than the player's
			switch( select )
			{
				// Strength based attack
				case 1:
					enemDeath = playAttRound(1);
					playDeath = enemAttRound(1);
					display.pause();
					break;
				// Dexterity based attack
				case 2:
					enemDeath = playAttRound(2);
					playDeath = enemAttRound(2);
					display.pause();
					break;
				// Spellcasting (Intelligence based attack)
				case 3:
					enemDeath = playAttRound(3);
					playDeath = enemAttRound(3);
					display.pause();
					break;
				// Attempt to run away from combat	
				case 4:
					// If they successfully run away
					if ( run() )
					{
						Enem1.xpvalue = 0;
						cout << "\nYou ran away successfully!!!\n";
						display.pause();
						return true;
					}
					// Or else the enemy gets a free attack on the player
					else
					{
						cout << "\nYou failed to get away, the enemy hits you!\n";
						playDeath = enemAttRound( randnum(3, 127) );
						display.pause();
					}
					break;
				// Let the user know their input is invalid	
				default:
					cout << "\nPlease enter a correct choice!!!\n";
                    display.pause();
					break;
			}

			// Check to see if either or both of our competitors have died
			if( playDeath && enemDeath )
			{
				cout << "\n\nYou simultaneously strike each other fatally!!! A true warrior's death...\n";
				display.pause();
				return false;
			}
			else if( playDeath && !enemDeath )
			{
				cout << "\n\nYou died!!!\n";
				display.pause();
				return false;
			}
			
			else if( enemDeath && !playDeath )
			{
				cout << "\n\nYou have slain the " << Enem1.ename << "!!!\n";
				// Reset the players health to maximum after combat
				Play1.health=maxhp;
				display.pause();
				return true;
			}
	
		}while( !playDeath && !enemDeath );
	};
	
	// Function to check if player successfully runs away
	bool run()
	{
		if( randnum(100, 999) > 50)
			return true;
		else
			return false;
	};
	
	// Function that initializes a new game
	void initialize()
	{
		cout << "\nPlease give your hero a name:\n::> ";
		cin >> Play1.name;
		// Allocate the new players stats randomly
		Play1.allocate();
		// Select a monster from the array for the next battle
        erand = randnum(DBsize, 51);
        Enem1.ename=monsters[erand];
        Enem1.difficulty=diff[erand];
        // Generate the enemy from the given difficulty and the players
        // level
		Enem1.generate(Play1.level, Enem1.difficulty);
	};
	
	// Function that generates a new enemy
	void newEnemy()
	{
		// Select a monster from the array for the next battle
		erand = randnum(DBsize, 211);
        Enem1.ename=monsters[erand];
        Enem1.difficulty=diff[erand];
        // Generate the enemy from the given difficulty and the players
        // level
		Enem1.generate(Play1.level, Enem1.difficulty);
	};
	
	// Function that displays a screen for the experience and levels
	// gained from the previous battle
	void reward()
	{
		display.Clear();
		cout << "Previous Level: " << Play1.level << " Previous XP: " << Play1.experience << endl;
		cout << "Gained " << Enem1.xpvalue << " experience!!!" << endl;
		Play1.setXP(Enem1.xpvalue);
		cout << endl << "Current Experience: " << Play1.experience << endl;
		display.pause();
	};

	// Function that saves the current player into players.txt
	void Save()
	{
		// Open the save file in append mode to add more to the end
		save.open("players.txt", ios::app);
		save << endl << Play1.name << " " << Play1.level << " " << Play1.experience << " " << Play1.health << " " << Play1.attack << " " << Play1.defense << " " << Play1.pstr << " " << Play1.pdex << " " << Play1.pint;
		save.close();
	}
	
	// Function that loads the player saves from players.txt
	// Creates a dummy save as a fail safe if players.txt
	// cannot be found
	void Load()
	{
		// Initialize variables for load
		int savechoice=0, loop=0;
		// Null the size of the savefile
		savesize=0;
		// Open players.txt to load the saves
		PlayDB.open("players.txt");
		// If players.txt exists
		if ( PlayDB )
		{
			// Copy the entire contents of the saves
			do
			{
				PlayDB >> pnames[savesize] >> pstats[savesize][0] >> pstats[savesize][1] >> pstats[savesize][2] >> pstats[savesize][3] >> pstats[savesize][4] >> pstats[savesize][5] >> pstats[savesize][6] >> pstats[savesize][7];
				savesize++;
			} while ( !PlayDB.eof() );
			PlayDB.close();
		}
		// Else if players.txt doesn't exist create a dummy and load it
		// instead
		else
		{
			PlayDB.close();
			save.open("players.txt");
			save << "DummySave 1 1 1 1 1 1 1 1";
			save.close();
			PlayDB.open("players.txt");
			
			if ( PlayDB )
			{
				do
				{
					PlayDB >> pnames[savesize] >> pstats[savesize][0] >> pstats[savesize][1] >> pstats[savesize][2] >> pstats[savesize][3] >> pstats[savesize][4] >> pstats[savesize][5] >> pstats[savesize][6] >> pstats[savesize][7];
					savesize++;
				} while ( !PlayDB.eof() );
				PlayDB.close();
			}
		}
		
		display.Clear();
		// Prompt the user to select a save
		cout << "\nPlease select from one of the following saves:\n";
		do
		{
			cout << loop + 1 << ". " << pnames[loop] << ", Level " << pstats[loop][0] << ", Exp. " << pstats[loop][1] << endl;
			loop++;
		} while ( loop < (savesize - 1) );
		cout << "\n::> ";
		cin >> savechoice;
		// Adjustment for array logic
		savechoice -= 1;
		Play1.name = pnames[savechoice];
		Play1.level = pstats[savechoice][0];
		Play1.experience = pstats[savechoice][1];
		Play1.health = pstats[savechoice][2];
		Play1.attack = pstats[savechoice][3];
		Play1.defense = pstats[savechoice][4];
		Play1.pstr = pstats[savechoice][5];
		Play1.pdex = pstats[savechoice][6];
		Play1.pint = pstats[savechoice][7];
	}

	// Function that loads the list of enemies from monsters.txt
	// or creates a default monsters.txt if it cannot be found
	void LoadEnemDB()
	{
		// Null the database size
		DBsize=0;
		// Open monsters.txt to load the monster names and difficulties
		EnemDB.open("monsters.txt");
		// If monsters.txt exists
		if( EnemDB )
		{
			// Load the entire contents of the file
	        do
	        {
	                EnemDB >> monsters[DBsize] >> diff[DBsize];
	                DBsize++;
	        } while( !EnemDB.eof() );
	        EnemDB.close();
		}
		// Else create a default monsters.txt
        else
        {
			EnemDB.close();
			tempDB.open("monsters.txt");
			tempDB << "Skeleton 1\nZombie 2\nVampire 3";
			tempDB.close();
			EnemDB.open("monsters.txt");
			// Load the contents of the dummy file into memory
			if( EnemDB )
			{
		        do
		        {
		                EnemDB >> monsters[DBsize] >> diff[DBsize];
		                DBsize++;
		        } while( !EnemDB.eof() );
			}
	        EnemDB.close();
		}
	}
	
	// Function that displays the menu during the battle screen
	// Returns players selection
	int battleMenu()
	{
		int selection = 0;
		Play1.showstats();
		cout << "\nvs.\n";
		Enem1.showstats();
		cout << "Select from the following:\n(1) Strong Attack (2) Flourish (3) Cast Spell (4) Run Away\n::> ";
		cin >> selection;
		cin.ignore();
		return selection;
	};
	
	// Function that calculates the players attack for the round
	// Returns whether the enemy died this round
	bool playAttRound( int attype )
	{
		int pdam = 0;
		
		// Check to see if the players attack was sucessful
		if ( Play1.Attack(attype) > Enem1.Defend(attype) )
		{
			// The player's stat is subtracted from the enemy's stat
			// as a form of damage resistance
			switch( attype )
			{
				case 1:
					pdam = randnum(Play1.pstr, 30) - randnum(Enem1.estr, 40);
					break;
					
				case 2:
					pdam = randnum(Play1.pdex, 30) - randnum(Enem1.edex, 40);
					break;
					
				case 3:
					pdam = randnum(Play1.pint, 30) - randnum(Enem1.eint, 40);
					break;
			}
			// If the player damages the enemy
			if ( pdam > 0 )
			{
				Enem1.ehealth -= pdam;
				cout << "\nYou mightily strike the enemy! The enemy took " << pdam << " damage.\n";
			}
			// Otherwise it's a glancing blow
			else
			{
				pdam = 0;
				cout << "\nYou mightily strike the enemy! The enemy seems unshaken however.";
			}
			// Check to see if the enemy died
			if ( Enem1.ehealth <= 0 )
			{
				cout << "\nThe enemy collapsed from your devastating blow and died!\n";
				return true;
			}
		}
		// If not the player missed
		else
		{
			cout << "\nYour clumsy blow missed! The enemy took no damage.\n";
		}
		return false;				
	};
	
	// Function that calculates the enemy's attack for the round
	// Returns whether the player died this round
	bool enemAttRound( int ettype )
	{
		int edam = 0;
		// Check to see if the enemy's attack is successful
		if ( Enem1.Attack(ettype) > Play1.Defend(ettype) )
		{
			// Subtract the enemies stats from the players stats 
			// as a form of damage reduction
			switch( ettype )
			{
				case 1:
					edam = randnum(Enem1.estr, 50) - randnum(Play1.pstr, 60);
					break;
					
				case 2:
					edam = randnum(Enem1.estr, 50) - randnum(Play1.pstr, 60);
					break;
					
				case 3:
					edam = randnum(Enem1.estr, 50) - randnum(Play1.pstr, 60);
					break;
			}
			// If the enemy does damage
			if ( edam > 0 )
			{
				Play1.health -= edam;
				cout << "\n\nThe enemy hit! You took " << edam << " damage.\n";
			}
			// Or else it's a glancing blow
			else
			{
				edam = 0;
				cout << "\n\nThe enemy hit! You failed to feel the blow however\n";
			}
			// Check to see if the player died
			if ( Play1.health <= 0 )
			{
				cout << "The enemy devastated you with that attack!!!\n";
				return true;
			}
		}
		// Or else the enemy missed
		else
		{
			cout << "\n\nThe enemy clumsily missed! You took no damage!\n";
		}
		return false;
	};
	
} CurGame;

int main()
{
	// Initialize the game enviroment variables
	int choice = 0, option = 0;
	bool quit = false, victory = true, begin = true;
	
	// Load enemies from monsters.txt
	CurGame.LoadEnemDB();
	
	// Display the title screen
	display.TitleScroll();
	
	// Pause on the title screen
	display.pause();
	
	// Display initial New or Continue screen and return selection
	option=display.Start();
	
	do
	{
		switch( option )
		{
			// New Game
			case 1:
				CurGame.initialize();
				begin = false;
				break;
			// Continue
			case 2:
				CurGame.Load();
				CurGame.newEnemy();
				begin = false;
				break;
			// Exit
			case 3:
				return 0;
				break;
			// Let the user know that option doesn't exist
			default:
				cout << "\n\nPlease select a correct option!!!\n\n";
				display.pause();
				display.Clear();
				break;
		}
	} while ( begin );

	do
	{
		// Display the main menu
		display.Clear();
		CurGame.Play1.showstats();
		choice=display.MMenu();
		
		switch( choice )
		{
			// New battle
			case 1:
				victory = CurGame.battle();
				// If the player died
				if( !victory )
				{
					// Displays GAME OVER and exits without saving
					display.GO();
					quit = true;
				}
				// Else reward the player for their battle
				else
				{
					CurGame.reward();
				}
				break;
			// Save and quit
			case 2:
				CurGame.Save();
				quit = true;
				break;
			// Let the user know that option doesn't exist
			default:
				cout << "\n\nPlease select a correct option!!!\n\n";
				display.pause();
				display.Clear();
				break;
		}
		// Generate a new enemy after the battle
		CurGame.newEnemy();
	} while( !quit );
	
	return 0;
}
