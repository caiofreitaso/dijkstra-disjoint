#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <random>
#include <cmath>
#include "disjointset.h"

std::mt19937 *r;

unsigned rng()
{
	static unsigned count = 0;

	if (r == 0)
		r = new std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
	
	if (count == r->state_size)
	{
		r->seed(std::chrono::system_clock::now().time_since_epoch().count());
		count = 0;
	}

	count++;

	return (*r)();
}
inline unsigned rng(unsigned max, unsigned min = 0)
{
	unsigned diff = max - min + 1;
	return (rng() % diff) + min;
}
inline double drng()
{
	double ret = rng();
	return ret / r->max();
}

int main(int argc, char const *argv[])
{
	std::chrono::time_point<std::chrono::system_clock> a;
	std::chrono::duration<double> tt;

	if (argc == 1)
	{
		unsigned sizes[] = { 1 << 8, 1 << 10, 1 << 12, 1 << 14, 1 << 16, 1 << 18, 1 << 20 };
		unsigned vert[] = { 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9, 1 << 10 };

		
		for (unsigned sz = 0; sz < 7; sz++)
		{
			a = std::chrono::system_clock::now();

			std::cout << "TEST " << sz << ". Edges: " << sizes[sz] << ", vertices: " << vert[sz] << "\n";
			
			Graph::MultiGraph<int> graph;
			graph.vertices(vert[sz]);

			for (unsigned e = 0; e < sizes[sz]; e++)
			{
				unsigned v = rng(graph.vertices());
				unsigned u = rng(graph.vertices());
				double w = drng();

				graph.insert(v,u,w);
			}

			unsigned v = rng(graph.vertices());
			unsigned u = rng(graph.vertices());

			while (u == v)
				u = rng(graph.vertices());

			tt = std::chrono::system_clock::now() - a;

			std::cout << "CREATION TIME: " << tt.count() << "\n\n";

			graph.printerr();

			a = std::chrono::system_clock::now();
			std::pair<std::vector<unsigned>,double> path = dijkstra(graph, v, u);
			tt = std::chrono::system_clock::now() - a;

			std::cout << "EXECUTION TIME: " << tt.count() << "\n\n";
			std::cout << "From " << v << " to " << u << "\n";
			std::cout << "Cost: " << path.second << "\n";
			std::cout << "Path:\n";
			if (path.first.size())
			{
				for (unsigned i = 0; i < path.first.size()-1; i++)
					std::cout << path.first[i] << " - " << path.first[i+1] <<"\n";
				std::cout << path.first.back() << " - " << u <<"\n\n\n";
			} else
				std::cout << "NONE\n\n\n";
		}
	} else {
		std::fstream file;
		std::string buffer;
		std::stringstream ss;
		Graph::MultiGraph<int> graph;
		
		file.open(argv[1]);

		std::getline(file,buffer);
		while (buffer[0] == 'c')
			std::getline(file,buffer);
		ss.str(buffer);

		unsigned vertices;
		unsigned edges;

		ss >> buffer;
		ss >> buffer;

		ss >> vertices;
		ss >> edges;

		graph.vertices(vertices);
		std::cout << graph.vertices() << "\n";

		while (buffer[0] != 'a')
			std::getline(file,buffer);

		for (unsigned i = 0; i < edges; i++)
		{
			unsigned a, b;
			double w;
			char d;
			
			ss.clear();
			ss.str(buffer);
			ss >> d;

			ss >> a;
			ss >> b;
			ss >> w;
			graph.insert(a,b,w);
			std::getline(file,buffer);
		}

		file.close();


		unsigned v = rng(graph.vertices());
		unsigned u = rng(graph.vertices());

		while (u == v)
			u = rng(graph.vertices());

		a = std::chrono::system_clock::now();
		std::pair<std::vector<unsigned>,double> path = dijkstra(graph, v, u);
		tt = std::chrono::system_clock::now() - a;

		std::cout << "EXECUTION TIME: " << tt.count() << "\n\n";
		std::cout << "From " << v << " to " << u << "\n";
		std::cout << "Cost: " << path.second << "\n";
		std::cout << "Path:\n";
		if (path.first.size())
		{
			for (unsigned i = 0; i < path.first.size()-1; i++)
				std::cout << path.first[i] << " - " << path.first[i+1] <<"\n";
			std::cout << path.first.back() << " - " << u <<"\n\n\n";
		} else
			std::cout << "NONE\n\n\n";
	}

	return 0;
}