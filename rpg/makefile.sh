# makefile.sh (for fman/lure)
# by _______
# @????

# uses server.c and client.c
# super lame version of a makefile
# written by hand for my sake

space() (
	echo ""
)

space
space
echo "!COMPILING server.c!"
space
space
gcc ./server.c -o ./server
space
space
echo "!COMPILING client.c!"
space
space
gcc ./client.c -o ./client
space
space
echo "!COMPILING FINISHED!"
space
space
