#include <cvt/math/graph/Graph.h>
#include <iostream>

using namespace cvt;

class MyVistor : public GraphVisitor<int,int>
{
	void visitNode( GraphNode<int,int>* node )
	{
		std::cout << node->data() << std::endl;
	}
};

int main()
{
	Graph<int,int> g;
	GraphNode<int,int>* node1 = g.addNode( 1 );
	GraphNode<int,int>* node2 = g.addNode( 2 );
	GraphNode<int,int>* node3 = g.addNode( 3 );
	GraphNode<int,int>* node4 = g.addNode( 4 );
	node1->addEdgeTo( node2, 0 );
	node1->addEdgeTo( node3, 0 );
	node3->addEdgeTo( node4, 0 );
	MyVistor v;
	g.dfs( v );
}