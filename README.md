# spotifytrack

## Purpose

The current Spotify API doesn't have a way to get the currently playing track
so this app is a small workaround for that.

The main reason this was created was I wanted to display the current track in the
tmux status bar while working in Cygwin.

## Building

### Cygwin
Required packages:

	gcc-g++
	make
	w32api-headers

Just clone the repo and run make to build it.

### Visual Studio C++
Requires:
	[getopt.h](https://github.com/skandhurkat/Getopt-for-Visual-Studio/blob/master/getopt.h)

VS project files are included in the repo and can be used to build.

To use the Visual studio compiled version in some Cygwin apps, you may need to strip off '\r' from the
output due to the way Windows outputs newlines (\r\n instead of just \n).

See my tmux usage for an example:

	set -g status-right "#[fg=brightwhite,bg=black] ♫ #[fg=brightgreen]#(spotifytrack -l 80 | tr -d '\r') #[fg=brightblue,bg=colour241,bold] %m/%d #[fg=colour233,bg=colour245,bold] %I:%M:%S %P "

![SpotifyTrack in tmux](http://i.imgur.com/gIcSzhz.png)

## Usage

For the latest usage directions, use the help flag "-h, --help" to see the current
usage for the build you are running.

	Usage: spotifytrack.exe [OPTION]...
	Print the currently playing track on Spotify

	Options:
			-l, --length n		Limit the output to n characters
			-h, --help			Show this usage message
			-v, --version		Show the version number

