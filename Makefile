CFLAGS = -std=c++20 -O3

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

lrns: main.cpp
	g++ $(CFLAGS) -o bin/lrns main.cpp $(LDFLAGS)

alt: alt.cpp
	g++ $(CFLAGS) -o bin/alt alt.cpp $(LDFLAGS)

.PHONY: test clean

test: lrns
	./bin/lrns

testalt: alt
	./bin/alt

clean:
	rm -f bin/lrns bin/alt

shaders: shaders/glsl/shader.vert shaders/glsl/shader.frag
	glslc shaders/glsl/shader.vert -o shaders/spv/vertex.spv
	glslc shaders/glsl/shader.frag -o shaders/spv/fragment.spv
