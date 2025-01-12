
compile:
	g++ -c -Wall -Wextra -Wconversion -pedantic -Werror ./src/*.cpp   
	g++ *.o -o app -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

run:
	make compile
	./app

clean:
	rm ./*.o app

	