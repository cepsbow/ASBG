// Programmed by Jonathan Alley
// game.cpp - Primary class resource file
// A Simple Battle Game
// Defines the player and enemy classes which provides objects and
// methods used by the game class, as well as the randnum() function

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

// An independant incrementor to prevent duplicate results from use of 
// rand() and srand() 
int increment=0;

// Pseudo-random number generator, relies on time, a provided modifier,
// and the current running iteration. Returns up to a maximum of max.
int randnum( int max, int modifier )
{
	int RandNum;
	srand( time(NULL) + (modifier*29814) + increment );
	RandNum = rand() % max + 1;
	increment++;
	return RandNum;
};

// Defines the player class, used to manage the
// players data
class player
{
	public:
	
	// Initializes variables for the players stats and info
	int health, pstr, pdex, pint, attack, defense, level, experience;
	string name;
	
	// Displays the players current status
	void showstats()
	{
	        cout << "----------------------------------------------------------------------------------------------------\n";
	        cout << "Hero:\t" << name << "\t\tCurrent HP:\t" << health << "\tCurrent Level:\t" << level <<  endl << endl;
	        cout << "Hero's Stats:\n\t[Strength]\t|" << pstr << "|\t[Dexterity]\t|" << pdex << "|\t[Intelligence]\t|" << pint << "|\n";
	        cout << "\t[Attack]\t|" << attack << "|\t[Defense]\t|" << defense << "|\t[Experience]\t|" << experience << "|\n";
	        cout << "----------------------------------------------------------------------------------------------------\n";
	}
	
	// Increases the players experience by Exp and checks if the player
	// eligible for the next level
	void setXP( int Exp )
	{
		// Hierarchy trigger, checks to see if player levels
		// beyond the current cases experience requirements
		bool nextcat = false;
		// Add Exp to experience
		experience += Exp;
		// Continue checking if the player is eligible for the next
		// hierarchy of level until they no longer meet the requirements
		do
		{
			// If player is level 1 through 5
			if( level >= 1 && level <= 5 )
			{
				// Then the experience for the next level equals
				// level * 200 in experience
				if( experience > level*200 )
				{
					// Continue checking in this bracket until
					// the player levels up beyond it or they run
					// out of experience
					do
					{
						if( level > 5 )
						{
							nextcat = true;
							break;
						}
						// Subtract the requirement from their current
						// experience
						experience -= level * 200;
						level++;
						cout << "Ding! Level up! Congrats on level " << level << "!" << endl;
						LevelUp();
					} while( experience > level*200 );
				}
			}
			// Else if the player is level 6 through 10
			else if( level >= 6 && level <= 10 )
			{
				// If execution came from the earlier case, reset the
				// flag
				if( nextcat = true )
					nextcat = false;
				
				// The experience for the next level equals
				// level * 500 in experience	
				if( experience > level*500 )
				{
					// Continue checking in this bracket until
					// the player levels up beyond it or they run
					// out of experience
					do
					{
						if( level > 10 )
						{
							nextcat = true;
							break;
						}
						experience -= level * 500;
						level++;
						cout << "Ding! Level up! Congrats on level " << level << "!" << endl;
						LevelUp();
					} while( experience > level*500 );
				}
			}
			// Else if the player is greater than level 10
			else
			{
				// If execution came from the earlier case, reset the
				// flag
				if( nextcat = true )
					nextcat = false;
				
				// The experience for the next level equals
				// level^2 * 500 in experience
				if( experience > pow(level, 2) * 500 )
				{
					do
					{
						experience -=  ( pow(level, 2) * 500 );
						level++;
						cout << "Ding! Level up! Congrats on level " << level << "!" << endl;
						LevelUp();
					} while( experience > pow(level, 2) * 500 );
				}
			}
		} while( nextcat );
	};

	// Function that generates a new players initial stats
	void allocate()
	{
		pstr = randnum(18, 5);
		pdex = randnum(18, 6);
		pint = randnum(18, 7);
		health = randnum( (pstr+12), 8 );
		attack = randnum( ( 10 + (pstr/3) + (pdex/3) + (pint/3) ), 9 );
		defense = randnum( ( 10 + (pstr/3) + (pdex/3) + (pint/3) ), 41 );
		level = 1;
		experience = 0;
	};
	
	// Function that levels the player up
	void LevelUp()
	{
		// Initialize temporary variables for outputting the difference
		// gained from each level
		int npstr=0, npdex=0, npint=0, nhealth=0, nattack=0, ndefense=0;
			
		cout << endl << "You gained: ";
		npstr = randnum(8, 91);
		pstr += npstr;
		cout << "+" << npstr << " Strength ";
		npdex = randnum(8, 92);
		pdex += npdex;
		cout << "+" << npdex << " Dexterity ";
		npint = randnum(8, 93);
		pint += npint;
		cout << "+" << npint << " Intelligence ";
		nhealth =  randnum(8, 94) + randnum( (pstr/4), 95 );
		health += nhealth;
		cout << "+" << nhealth << " Health ";
		nattack = randnum( ( (pstr/3) + (pdex/3) + (pint/3) ), 96);
		attack += nattack;
		cout << "+" << nattack << " Attack ";
		ndefense = randnum( ( (pstr/3) + (pdex/3) + (pint/3) ), 97);
		defense += ndefense;
		cout << "+" << ndefense << " Defense!!!" << endl;
	};
	
	// Function that calculates the players attack roll for a given
	// type of attack
	int Attack( int atype )
	{
		// Initialize the roll's result
		int result=0;
		
		switch( atype )
		{
			// Strength attack
			case 1:
				result = randnum(attack, 11) + randnum(pstr, 12);
				return result;
				break;
			// Dexterity attack
			case 2:
				result = randnum(attack, 13) + randnum(pdex, 14);
				return result;
				break;
			// Spell
			case 3:
				result = randnum(attack, 15) + randnum(pint, 16);
				return result;
				break;
		}
	};
	
	// Function that calculates the players defense roll for a given
	// type of defense
	int Defend( int atype )
	{
		// Initialize the roll's result
		int result=0;
		
		switch( atype )
		{
			// Strength defense
			case 1:
				result = randnum(defense, 17) + randnum(pstr, 18);
				return result;
				break;
			// Dexterity defense
			case 2:
				result = randnum(defense, 19) + randnum(pdex, 21);
				return result;
				break;
			// Spell defense
			case 3:
				result = randnum(defense, 22) + randnum(pint, 23);
				return result;
				break;
		}
	};

};

// Defines the enemy class, used to manage the
// enemy's data
class enemy
{
	public:
	
	// Initialize the enemy's variables for stats and info
	int ehealth, estr, edex, eint, eattack, edefense, elevel, xpvalue, difficulty;
	string ename;

	// Function that generates the enemy's stats
	void generate( int playlvl, int diff )
	{
		// Set all stats to null
		ehealth=0,estr=0,edex=0,eint=0,eattack=0,edefense=0,elevel=0,xpvalue=0;
		// Generate the enemy's level based on the player's
		elevel = randnum((playlvl + diff), 8);
		// Level the enemy to their current level
		for ( int loop=0; loop < elevel; loop++ )
		{
		        estr += randnum((6 + diff), 1);
		        edex += randnum((6 + diff), 2);
		        eint += randnum((6 + diff), 3);
		        ehealth += randnum((6 + diff), 4);
		        eattack += randnum( ( (estr/3) + (edex/3) + (eint/3) + diff ), 5);
		        edefense += randnum( ( (estr/3) + (edex/3) + (eint/3) + diff ), 6);
		        // Experience formulas for the enemy
		        if( difficulty > 0 )
					xpvalue += (randnum(100, 7)) * difficulty * elevel;
				else
					xpvalue += (randnum(100, 7)) * elevel;
		}
	};
	// Function that displays the current enemy's stats
	void showstats()
	{
		cout << "----------------------------------------------------------------------------------------------------\n";
	    cout << "Enemy:\t" << ename << "\t\tCurrent HP:\t" << ehealth << "\tDifficulty:\t" << difficulty << endl << endl;
	    cout << "Enemy's Stats:\n\t[Strength]\t|" << estr << "|\t[Dexterity]\t|" << edex << "|\t[Intelligence]\t|" << eint << "|\n";
	    cout << "\t[Attack]\t|" << eattack << "|\t[Defense]\t|" << edefense << "|\t[Level]\t|" << elevel << "|\n";
	    cout << "----------------------------------------------------------------------------------------------------\n";
	};
	// Function that calculates the enemy's attack roll for a given
	// type of attack
	int Attack( int atype )
	{
		// Initialize the roll's result
		int result=0;
		
		switch( atype )
		{
			// Strength attack
			case 1:
				result = randnum(eattack, 24) + randnum(estr, 25);
				return result;
				break;
			// Dexterity attack
			case 2:
				result = randnum(eattack, 26) + randnum(edex, 27);
				return result;
				break;
			// Spell attack
			case 3:
				result = randnum(eattack, 28) + randnum(eint, 29);
				return result;
				break;
		}
	};
	// Function that calculates the enemy's defense roll for a given
	// type of defense
	int Defend( int atype )
	{
		// Initialize the roll's result
		int result=0;
		
		switch( atype )
		{
			// Strength defense
			case 1:
				result = randnum(edefense, 31) + randnum(estr, 32);
				return result;
				break;
			// Dexterity defense
			case 2:
				result = randnum(edefense, 33) + randnum(edex, 34);
				return result;
				break;
			// Intelligence defense
			case 3:
				result = randnum(edefense, 35) + randnum(eint, 36);
				return result;
				break;
		}
	};
	
};
