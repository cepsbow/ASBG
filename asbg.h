// Programmed by Jonathan Alley
// asbg.h - Primary header file
// A Simple Battle Game
// Defines a screen class for captivation and control of iostream
// elements, objects, and methods

#ifndef __ASBG_H_INCLUDED__
#define __ASBG_H_INCLUDED__

using namespace std;

// Defines the screen class, provides most display functions outside of
// battle. Also provides various methods that are utilized in the battle
// routine as well.
class screen
{
	// Define some constants for our display functions.
	// step indicates a single frame step used in the TitleScroll()
	// function
	// logo defines an ASCII image of the games initials
	static const double step = 1;
	const string logo[8] = {"        AAAA              SSSSS       BBBBBBB        GGGGGGG  ",
						    "       AA  AA            S     S      BB    BB      G       G ",
						    "      AA    AA           S     S      BB    BB     G        G ",
						    "     AAAAAAAAAA          SSSS         BBBBBBB      G          ",
						    "    AAAAAAAAAAAA           SSSS       BBBBBBB      G       GGG",
						    "   AA          AA       S     S       BB    BB     G        G ",
						    "  AA            AA      S     S       BB    BB      G       G ",
						    " AA              AA      SSSSS        BBBBBBB        GGGGGGG  "};
	
    public:
    
		// Animates a side-scroll in ASCII logo
		void TitleScroll()
		{
			int loop=0, rows;
			// Run until the whole logo is displayed
			do
			{
				// Initial blank screen
				if( loop == 0 )
				{
					Clear();
					for(rows=0; rows < 8; rows++)
					{
						space( logo[rows].size() );
						cout << endl;
					}
					ticks(step);
				}
				// Slide in motion from right
				else if( loop > 0 && loop < (logo[0].size() - 1) )
				{
					Clear();
					for( rows=0; rows < 8; rows++ )
					{
						space( (logo[rows].size() - loop) );
						cout << logo[rows].substr( 0, loop ) << endl;
					}
					// Equivalent to Frameskipping 1
					if( loop % 2 == 0 )
						ticks(step);
				}
				// Blink effect with pause
				else
				{
					Clear();
					for( rows=0; rows < 8; rows++ )
					{
						cout << logo[rows] << endl;
					}
					ticks(step);
					Clear();
					
					for(rows=0; rows < 8; rows++)
					{
						space( logo[rows].size() );
						cout << endl;
					}
					ticks(step);
					Clear();
					for(rows=0; rows < 8; rows++)
					{
						cout << logo[rows] << endl;
					}
				}
				loop++;
					
			} while( loop < logo[0].size() );
		};
		
		// Similar to Windows sleep() or *nix's usleep()
		// pauses execution for x seconds
		// sleep was avoided to keep from using non-standard libraries
		void ticks(double len)
		{
			// Only accurate to a second, maximum precision for standard 
			// C++ libraries.
			time_t begin = time(NULL);
			while( difftime(time(NULL), begin) < len ){};
		};
		// Defines a function to clear the screen
		void Clear()
		{
			// If Windows, or else use *nix
		    #ifdef _WIN32
				system("cls");
			#else
				system("clear");
			#endif
		};
		
		// Shows the inital game menu and returns selection
		int Start()
		{
			int opt=0;
			Clear();
			cout << "A Simple Battle Game!\n\n1.New Game\n2.Continue\n3.Exit\n::> ";
			cin >> opt;
			return opt;
		}
		// Displays the main menu and returns the selection
		int MMenu()
		{
			int option;
			cout << "Please select from the following:\n\n1. New Fight\n2. Save and Quit\n::> ";
			cin >> option;
			return option;
		};
		// Prints x spaces
		void space(int spaces)
		{
		        for ( int count = 0; count < spaces; count++ )
		        {
		                cout << " ";
		        }
		};
		// Pause flow, universal, make sure to use
		// cin.ignore() if you take input and use
		// this function
		void pause()
		{
			cout << endl << "Press enter to continue...";
			cin.get();
		};
};

#endif
