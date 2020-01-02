#include <iostream>
#include <string> 
#include <vector> 
#include <map> 
#include <unordered_set>

using namespace std;

// create an edge structure
struct Connection 
{
    int cost; 
    vector<string> path;
};

// create node structure for heap
struct Node 
{
    Connection* current; 
    Node* next; 
    Node* prev; 
};

class MinHeap
{
    /* Heap Class */
    private:  
        // declare head and bottom
        Node* head;
        Node* bottom;
        int count = 0;  // integer for keeping track of size

        void display_path(vector<string> path)
        {
            //display path of the node
            for (int i = 0; i < path.size(); i ++)
            {
                cout << path[i] << " ";
            }
            cout << " " << endl;
        }

    public:    
        MinHeap() 
        {
            //construct head and bottom
            //initialize their values to null pointers
            head = (Node*)malloc(sizeof(Node));
            bottom = (Node*)malloc(sizeof(Node));
            
            head->current = nullptr;
            head->next = nullptr; 
            head->prev = nullptr; 

            bottom->current = nullptr;
            bottom->next = nullptr; 
            bottom->prev = nullptr;
        }
        
        bool isEmpty()
        {
            // A method for checking if queue is empty
            if (count == 0)
            {
                return true;
            }
            return false;
        }

        void push(Connection* item)
        {
            /* Push Method; higher to low == left to right */
            if (head->current == nullptr)   // if head is empty
            {
                head->current = item;
                head->next = bottom;
                bottom->prev = head;
                count += 1;
            }

            else if (bottom->current == nullptr)    // if bottom is empty
            {
                if (head->current->cost < item->cost)
                {
                    Node* oldH =  head; 
                    bottom->current  =  oldH->current;
                    head->current = item;
                    count += 1;
                }

                else 
                {
                    bottom->current = item;
                    count += 1;
                }
            }

            else    // if neither are empty
            {
                if (head->current->cost <= item->cost)  // check if head is lower than new item
                {
                    //set new item as the new head
                    Node* oldH = head; 
                    head->current = item; 
                    head->next = oldH;
                    oldH->prev = head; 
                    head->prev = nullptr;
                    count += 1;
                }

                else if (bottom->current->cost >= item->cost)   // check if bottom is greater than new item
                {
                    //set new item as the new bottom
                    Node* oldB = bottom;
                    bottom->current = item;
                    bottom->prev = oldB;
                    bottom->next = nullptr; 
                    oldB->next = bottom; 
                    count += 1;
                }

                else 
                {   
                    //Look up a node that is less than item
                    Node* start = bottom;
                    while (start->prev != nullptr && start->current->cost <= item->cost)
                    {
                        start = start->prev;
                    }
                    //insert the new entry in between start and start->next
                    Node* new_entry = new Node();
                    new_entry->current = item; 
                    new_entry->next = start->next; 
                    new_entry->prev = start;
                    start->next->prev = new_entry;
                    start->next = new_entry;

                    count += 1;
                }
            }

        }

        Node pop()
        {
            if (count == 1)     
            {
                //pop head
                Node oldH = {head->current, nullptr, nullptr};
                head->current = nullptr;
                count -= 1;

                return oldH;
            }

            else if (count == 2)
            {
                //pop bottom
                Node oldB = {bottom->current, nullptr, nullptr};
                bottom->current = nullptr;
                count -= 1;
                
                return oldB;
            }

            else if (count > 2)
            {
                //pop bottom and set bottom = its previous node
                Node oldB = {bottom->current, nullptr, nullptr};
                bottom = bottom->prev; 
                bottom->next = nullptr;
                count -= 1;

                return oldB;
            }
            
            else 
            {
                cout << "CANNOT POP AN EMPTY HEAP" << endl;
            }
        }

        void display()
        {
            // Method for displaying the Heap
            Node* start = bottom;
            while (start->prev != nullptr)
            {
                display_path(start->current->path);
                start = start->prev;
            }

            if (start->current != nullptr)
            {
                display_path(start->current->path);
            }
        }
};

class Dijkstra
{
    private: 
        // create a map for neighbors 
        map< string, vector<pair<string, int> > > neighbors; 

    public:     
        void make(string name)
        {
            //Create an entry in the map and set it equals to a vector
            vector <pair<string, int> > list; 
            neighbors[name] = list;
        }

        void create(string origin, string destination, int cost)
        {
            //Create an Edge using a pair
            pair<string, int> item;
            item = make_pair(destination, cost); 
            
            neighbors[origin].push_back(item);    //push into the corresponding list of the origin in neighbor
            //cout << "Origin: " << origin << " # of edges: " << neighbors[origin].size() << endl;
        }

        void search(string origin, string destination)
        {
            /* Search Method */

            MinHeap route = MinHeap();
            
            //Grab all of origin's neighbors (edges)
            for (int i = 0; i < neighbors[origin].size(); i ++)
            {
                Connection* edge = new Connection();
                int cost = neighbors[origin][i].second;
                
                edge->cost = cost;
                edge->path.push_back(origin);
                edge->path.push_back(neighbors[origin][i].first);
                route.push(edge);
            }
            
            unordered_set<string> visited;          // create a set of strings
            visited.insert(origin);                 // insert origin
            
            while (route.isEmpty() != true)
            {

                Node current_entry = route.pop();       // Grab the pop'd Node
                int cost = current_entry.current->cost;             // set cost
                vector<string> path = current_entry.current->path;  // set vector

                string current = path[path.size()-1];               // set current which is the last item in the path (vector)
                cout << "Current: " << current << endl;

                if (visited.find(current) != visited.end())         // if current is in visited, continue
                {
                    continue;
                }

                if (current == destination)                         // if current is destination, output and stop
                {
                    cout << "\n-----FOUND DESTINATION-----" << endl;
                    cout << "COST: " << cost << " ";
                    cout << "\tPATH: ";

                    for (int i = 0; i < path.size(); i  ++)
                    {
                        cout  << path[i] << "  " ;
                    }
                    cout << " " << endl;
                    break;
                }

                // Grab all of current's edges with its neighbors
                for (int i = 0; i < neighbors[current].size(); i ++ )
                {
                    if ( visited.find(neighbors[current][i].first) == visited.end() )
                    {
                        int new_cost = cost + neighbors[current][i].second;         // set neighbor cost + current cost
                        vector<string> new_path = path;                             // set a new path
                        new_path.push_back(neighbors[current][i].first);            // new path appends neighbor
                        
                        Connection* new_edge = new Connection();                    // create a new connection
                        new_edge->cost = new_cost;                                  // assign new values to struct
                        new_edge->path = new_path;

                        route.push(new_edge);           //push in the new edge
                    }
                }
                
                visited.insert(current);                // insert current to visited set

            }
        
        }
        
};

int main()
{
    Dijkstra search = Dijkstra();
    search.make("A");
    search.make("B");
    search.make("C");
    search.make("D");
    search.make("E");
    search.make("F");
    search.make("G");
    search.make("H");
    search.make("I");
    search.make("J");
    search.make("K");
    search.make("L");
    search.make("M");

    search.create("A", "B", 3);
    search.create("A", "G", 2);
    search.create("A", "H", 2);
    search.create("B", "C", 4);
    search.create("C", "D", 2);
    search.create("D", "E", 1);
    search.create("E", "M", 2);
    search.create("G", "F", 6);
    search.create("B", "G", 1);
    search.create("C", "F", 2);
    search.create("F", "E", 1);
    search.create("E", "L", 1);
    search.create("H", "I", 2);
    search.create("G", "H", 1);
    search.create("I", "K", 3);
    search.create("K", "L", 5);
    search.create("L", "M", 1);
    
    search.search("A", "M");

}