#include <iostream>
#include <sstream>
#include <string.h>
#include <getopt.h>
#include <windows.h>

using namespace std;

static string VERSION = "1.1.1";
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
char songTitle[255];

void printSongTitle(unsigned int length)
{
	LPARAM lParam = 0;
	EnumWindows(EnumWindowsProc, lParam);

	string song = songTitle;
	// Check if Spotify is paused
	if(song == "Spotify")
	{
		song = "Paused";
	}

	// Truncate if needed
	cout << song.substr(0,length) << endl;
	return;
}

bool is_number(const std::string& s)
{
	string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

void printUsage(string exe_name)
{
	cerr << "Usage: " << exe_name << " [OPTION]...\n"
		<< "Print the currently playing track on Spotify\n"
		<< "\n"
		<< "Options:\n"
		<< "\t-l, --length n\t\tLimit the output to n characters\n"
		<< "\t-h, --help\t\tShow this usage message\n"
		<< "\t-v, --version\t\tShow the version number\n";
}

int main(int argc, char* argv[])
{
	int c;
	unsigned int length = 255;

	while (1)
	{
		static struct option long_options[] =
		{
			{"length",		required_argument,	0, 'l'},
			{"version",		no_argument,		0, 'v'},
			{"help",		no_argument,		0, 'h'},
			{0,0,0,0}
		};
		int option_index = 0;

		c = getopt_long (argc, argv, "l:vh", long_options, &option_index);

		if (c == -1)
			break;

		switch (c)
		{
			case 0:
				if (long_options[option_index].flag != 0)
					break;
				cout << "option " << long_options[option_index].name;
				if (optarg)
					cout << " with arg " << optarg;
				cout << endl;
				return 1;
				break;

			case 'l':
				{
					if (is_number(optarg))
					{
						stringstream s(optarg);
						s >> length;
					}
					else
					{
						cerr << optarg << " is not a valid number" << endl;
						return 1;
					}
					break;
				}

			case 'v':
				cout << "Spotify Now Playing - Version: " << VERSION << endl;
				return 0;
				break;

			case 'h':
				printUsage(argv[0]);
				return 0;
				break;

			case '?':
				return 1;
				break;

			default:
				abort();
		}
	}

	if (optind < argc)
	{
		cerr << "Unknown argument(s): ";
		while (optind < argc)
			cerr << argv[optind++] << " ";
		cerr << endl;
		return 1;
	}

	printSongTitle(length);

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
