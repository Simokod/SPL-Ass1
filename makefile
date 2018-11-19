# All Targets
all: rest

rest: bin/Dish.o bin/Customer.o bin/Table.o bin/Action.o bin/Restaurant.o bin/Main.o
	@echo 'Building target: rest'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/rest bin/Dish.o bin/Customer.o bin/Table.o bin/Action.o bin/Restaurant.o bin/Main.o
	@echo 'Finished building target: rest'
	@echo ' '

# Depends on the source and header files
bin/Dish.o: src/Dish.cpp
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Dish.o src/Dish.cpp
# Depends on the source and header files
bin/Customer.o: src/Customer.cpp
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp
# Depends on the source and header files
bin/Table.o: src/Table.cpp
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Table.o src/Table.cpp
# Depends on the source and header files
bin/Action.o: src/Action.cpp
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp
# Depends on the source and header files
bin/Restaurant.o: src/Restaurant.cpp
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp
# Depends on the source and header files
bin/Main.o: src/Main.cpp
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

#Clean the build directory
clean: 
	rm -f bin/*