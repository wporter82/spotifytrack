#include <iostream>
#include <sstream>
#include <getopt.h>
#include <algorithm>
#ifndef _WSL
#include <windows.h>
#include <psapi.h>
#else
#include <memory>
#include <string>
#include <array>
#endif

#ifdef _WSL
std::string exec(const char* cmd);
#endif

// Trim whitespace from start of string (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

// Trim whitespace from end of string (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

static std::string VERSION = "1.2";
#ifndef _WSL
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
#else
std::string getSongTitle();
#endif
char *songTitle;

void printSongTitle(unsigned int length)
{
	std::string song;
	song = "";

#ifndef _WSL
	if(!EnumWindows(EnumWindowsProc, 0))
	{
		song = songTitle;
	}
	else
	{
		song = "Not running";
	}
#else
	song = getSongTitle();
#endif
	ltrim(song);
	rtrim(song);
	// Check if Spotify is paused
	if(song == "Spotify" || song == "Spotify Premium")
	{
		song = "Paused";
	}

	// Truncate if needed
	std::cout << song.substr(0,length) << std::endl;
	return;
}

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

void printUsage(std::string exe_name)
{
	std::cerr	<< "Usage: " << exe_name << " [OPTION]...\n"
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
				std::cout << "option " << long_options[option_index].name;
				if (optarg)
					std::cout << " with arg " << optarg;
				std::cout << std::endl;
				return 1;
				break;

			case 'l':
				{
					if (is_number(optarg))
					{
						// Convert the argument to an int
						std::stringstream s(optarg);
						s >> length;
					}
					else
					{
						std::cerr << optarg << " is not a valid number" << std::endl;
						return 1;
					}
					break;
				}

			case 'v':
				std::cout << "Spotify Now Playing - Version: " << VERSION << std::endl;
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
		std::cerr << "Unknown argument(s): ";
		while (optind < argc)
			std::cerr << argv[optind++] << " ";
		std::cerr << std::endl;
		return 1;
	}

	printSongTitle(length);

	return 0;
}

#ifndef _WSL
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	char className[255];
	GetClassName(hwnd,className,sizeof(className));
	std::string sClassName = className;

	// Look for the class name that Spotify is registered as
	if(sClassName == "Chrome_WidgetWin_0" && IsWindowVisible(hwnd))
	{
		// Get the process ID so we can find the exe path
		DWORD procID;
		DWORD threadID;
		threadID = GetWindowThreadProcessId(hwnd, &procID);

		HANDLE threadHandle;
		threadHandle = OpenProcess(
					PROCESS_QUERY_INFORMATION |
					PROCESS_QUERY_LIMITED_INFORMATION,
					false, procID);

		TCHAR fileName[MAX_PATH];
		GetProcessImageFileName(threadHandle, fileName, MAX_PATH);

		if(fileName != NULL && strstr(fileName, "Spotify.exe"))
		{
			TCHAR windowTitle[MAX_PATH];
			GetWindowText(hwnd, windowTitle, MAX_PATH);

			// Find the one that isn't blank
			if(windowTitle != NULL && windowTitle[0] != 0)
			{
				songTitle = windowTitle;
				// Return false to stop enumerating
				return false;
			}
		}

	}
	// Return true to keep looking for the Spotify window
	return true;
}
#else
std::string getSongTitle() {
	std::string result;
	result = exec("$(wslpath \"C:\\Windows\\System32\")/tasklist.exe /V /FO LIST /FI \"IMAGENAME eq Spotify.exe\" /FI \"STATUS eq RUNNING\" | grep -i \"Window Title\" | grep -iv \"N/A\" | sed 's/Window Title: //'");
	return result;
}

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}
#endif
