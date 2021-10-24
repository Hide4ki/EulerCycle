#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>  

using namespace std;

class Node
{
	char _name;
	vector<Node*> _incidents;
	int _indeg = 0;
	int _outdeg = 0;
public:
	Node(char name) :_name(name){}
	void AddIncident(Node* it) 
	{
		_incidents.push_back(it); 
		_outdeg++;
		it->_indeg++;
	}
	bool CheckEdge() const { return _indeg == _outdeg; }
	bool IsEmpty() const { return _incidents.empty(); }
	Node *PopIncident()
	{
		auto tmp = _incidents.back();
		_incidents.pop_back();
		return tmp;
	}
	char GetName() const { return _name; }
	vector<Node*>::const_iterator begin() const { return _incidents.begin(); }
	vector<Node*>::const_iterator end() const { return _incidents.end(); }
	friend ostream& operator<<(ostream& out, const Node& it);
};

ostream& operator<<(ostream& out, const Node& it)
{	
	out << it._name;
	return out;
}

vector<Node*> Nodes;
vector<Node*> Path;

int GetDig(char ch)
{
	return ch - '0';
}

bool CheckEulerCycle()
{
	for (auto it : Nodes)
		if (!it->CheckEdge())
			return false;
	return true;
}

void ClearNodes()
{
	for (auto it : Nodes)
		delete it;
}

void DfsModified(Node *v)
{
	while (!v->IsEmpty())
	{
		auto u = v->PopIncident();
		DfsModified(u);
		Path.push_back(u);
	}
}

void WriteGraph()
{
	ofstream graph("graph.dot");
	graph << "digraph A" << endl;
	graph << "{" << endl;
	for (auto node : Nodes)
		for(auto it : *node)
			graph << node->GetName() << "->" << it->GetName() << endl;
	graph << "}" << endl;
	graph.close();
}

void SearchAndWriteEulerCycle()
{
	ofstream cycle("EulerCycle.dot");
	cycle << "digraph A" << endl;
	cycle << "{" << endl;
	if (CheckEulerCycle())
	{
		DfsModified(Nodes[0]);
		auto prev = Path.back();
		auto i = Path.size();
		for (auto it : Path)
		{
			cycle << *it << "->" << *prev << "[label = " << i-- << "]" << endl;
			prev = it;
		}
	}
	cycle << "}" << endl;
	cycle.close();
}

void ReadNodesName()
{
	string line;
	char name;

	getline(cin, line);
	istringstream buf(line);

	while (buf >> name)
		Nodes.push_back(new Node(name));
}

void ReadAdjacencyMatrix()
{
	string line;
	for (auto it : Nodes)
	{
		getline(cin, line);
		istringstream buf(line);
		char tmp;
		int i = 0;
		while (buf >> tmp)
			if (isdigit(tmp))
			{
				if (GetDig(tmp) == 1)
					it->AddIncident(Nodes[i]);
				i++;
			}
	}
}

int main()
{
	ReadNodesName();
	ReadAdjacencyMatrix();
	WriteGraph();
	SearchAndWriteEulerCycle();
	ClearNodes();
	return 0;
}

/*
a b c d e f
0 1 1 0 0 0 
0 0 0 1 1 0
0 1 0 1 0 0
1 0 0 0 0 1 
1 0 0 0 0 0 
0 0 1 0 0 0
*/