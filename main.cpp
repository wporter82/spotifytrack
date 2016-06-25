#include <iostream>
#include <string.h>
#include <windows.h>

using namespace std;

static char* VERSION = "1.1.0";
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
char songTitle[255];

void printSongTitle(unsigned int length = 255)
{
	LPARAM lParam = 0;
	EnumWindows(EnumWindowsProc, lParam);

	// Check if Spotify is paused
	if(strcmp(songTitle, "Spotify") == 0)
	{
		char pausedMessage[] = "Paused";
		int pauseLen = strlen(pausedMessage);
		strncpy(songTitle, pausedMessage, pauseLen);
		songTitle[pauseLen] = 0;
	}

	// Truncate the title
	if(strlen(songTitle) > length)
	{
		int titleLen = strlen(songTitle);
		int truncateLen = titleLen - length;
		char* copy = new char[length+1];

		strncpy(copy, songTitle, titleLen - truncateLen);
		// Add null terminator to end of the string
		copy[length] = 0;

		cout << copy << endl;
	}
	else
	{
		cout << songTitle << endl;
	}
	return;
}

void printUsage(char* argv[])
{
	cerr << "Usage: " << argv[0] << " [OPTION]...\n"
		<< "Print the currently playing track on Spotify\n"
		<< "\n"
		<< "Options:\n"
		<< "\t-l, --length n\t\tLimit the output to n characters\n"
		<< "\t-h, --help\t\tShow this usage message\n"
		<< "\t-v, --version\t\tShow the version number\n";
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printSongTitle();
	}
	else if(argc == 2)
	{
		if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
		{
			printUsage(argv);
		}
		else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
		{
			cout << "Spotify Now Playing - Version: " << VERSION << endl;
		}
		else
		{
			printUsage(argv);
		}
	}
	else if(argc == 3)
	{
		if(strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--length") == 0)
		{
			int length = atoi(argv[2]);
			if(length < 1 || length > 255)
			{
				cerr << "Error: " << argv[2] << " is invalid. Number must be positive and less than 255."
					<< endl;
			}
			else
			{
				printSongTitle(length);
			}
		}
		else
		{
			printUsage(argv);
		}
	}
	else
	{
		printUsage(argv);
	}

	return 0;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	char className[80];
	GetClassName(hwnd,className,sizeof(className));

	if(strcmp(className, "SpotifyMainWindow") == 0)
	{
		// Found the Spotify window, now get the window title
		GetWindowText(hwnd,songTitle,sizeof(songTitle));
		// Return false to stop enumerating
		return false;
	}
	// Return true to keep looking for the Spotify window
	return true;
}
