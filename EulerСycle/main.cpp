#include <iostream>
#include <sstream>
#include <vector>

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
		this->_outdeg++;
		it->_indeg++;
	}
	bool CheckEdge() { return _indeg == _outdeg; }
	bool IsEmpty() { return _incidents.empty(); }
	Node *PopIncident()
	{
		auto tmp = _incidents.back();
		_incidents.pop_back();
		return tmp;
	}
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

int main()
{
	string line;
	char name;

	getline(cin, line);
	istringstream buf(line);

	while (buf >> name)
		Nodes.push_back(new Node(name));

	for (auto it : Nodes)
	{
		getline(cin, line);
		istringstream buf(line);
		char tmp;
		int i = 0;
		while (buf >> tmp)
			if (isdigit(tmp))
			{
				if(GetDig(tmp) == 1)
					it->AddIncident(Nodes[i]);
				i++;
			}
	}

	if (CheckEulerCycle())
	{
		DfsModified(Nodes[0]);
		for (auto it : Path)
			cout << *it << "<-";
		cout << *Nodes[0] << endl;
	}

	ClearNodes();
	return 0;
}

/*
1 2 3 4 5 6
0 1 1 0 0 0 
0 0 0 1 1 0
0 1 0 1 0 0
1 0 0 0 0 1 
1 0 0 0 0 0 
0 0 1 0 0 0
*/