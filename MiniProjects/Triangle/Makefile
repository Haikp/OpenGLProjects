# when using 'make', 
# replace the name of the ${workspaceFolder} with the actual folder name
# or if already in the same dir, then delete ${workspaceFolder}/ all together
output: main.o glad.o
	g++ main.o glad.o -o app

main.o: main.cpp
# g++ -c main.cpp -std=c++17 -fdiagnostics-color=always -Wall -g -I${workspaceFolder}/dependencies/include -L${workspaceFolder}/dependencies/library ${workspaceFolder}/dependencies/library/libglfw.3.3.dylib ${workspaceFolder}/*.cpp ${workspaceFolder}/glad.c -o ${workspaceFolder}/app -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecate
	g++ -c main.cpp -std=c++17 -fdiagnostics-color=always -Wall -g -Idependencies/include -Ldependencies/library dependencies/library/libglfw.3.3.dylib *.cpp glad.c -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecated

clean:
	rm *.o app