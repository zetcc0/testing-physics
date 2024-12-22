
compile:
	g++ -c ./src/*.cpp   
	g++ *.o -o app -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

run:
	make compile
	./app

clean:
	rm ./*.o app

	