# makefile.sh (for fman/lure)

# uses server.s and client.c
# super lame version of a makefile
# written by hand for my sake

space() (
	echo ""
	echo ""
)

space
#echo "!CREATING DATA dir!"
space
#mkdir ./DATA/
space
#echo "!CLEANING database.txt!"
space
#rm ./DATA/database.txt
#touch ./DATA/database.txt
space
#echo "!COMPILING server.c!"
space
#gcc ./server.c -o ./server
space
echo "!COMPILING client.c!"
space
gcc ./client.c -o ./client -lSDL2 -lSDL2_ttf
space
echo "!COMPILING FINISHED!"
space
