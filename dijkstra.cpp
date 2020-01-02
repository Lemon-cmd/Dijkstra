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
                head->prev = nullptr;
                bottom->prev = head;
                count += 1;
            }

            else if (bottom->current == nullptr && count == 1)    // if bottom is empty
            {
                if (head->current->cost < item->cost)
                {
                    bottom->current = head->current;
                    bottom->prev = head;
                    head->current = item;
                    head->next = bottom;
                    head->prev = nullptr;
                    count += 1;
                }

                else 
                {
                    bottom->current = item;
                    bottom->prev = head;
                    head->next = bottom;
                    count += 1;
                }
            }

            else    // if neither are empty
            {
                if (head->current->cost <= item->cost)  // check if head is lower than new item
                {
                    //set new item as the new head
                    Node* oldH = new Node();
                    oldH->current = head->current;
                    oldH->next = head->next;
                    head->next->prev = oldH;
                    head->current = item; 
                    head->next = oldH;
                    head->prev = nullptr;
                    oldH->prev = head;
                    count += 1;
                }

                else if (bottom->current->cost >= item->cost)   // check if bottom is greater than new item
                {
                    //set new item as the new bottom
                    Node* oldB = new Node();
                    oldB->current = bottom->current;
                    oldB->prev = bottom->prev;
                    bottom->prev->next = oldB;
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
                    while (start->prev != nullptr && start->current->cost < item->cost)
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
            
            if (count > 0)
            {
                if (count == 1)
                {
                    display_path(head->current->path);
                }

                else if (count > 1)
                {
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
            pair<string, int> origin_dest;
            origin_dest = make_pair(destination, cost); 
            
            pair<string, int> dest_origin;
            dest_origin = make_pair(origin, cost);
            
            //bi-directional edge
            neighbors[origin].push_back(origin_dest);           
            neighbors[destination].push_back(dest_origin);
           
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
                //route.display();
                Node current_entry = route.pop();                   // Grab the pop'd Node
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

                        route.push(new_edge);                                       //push in the new edge
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
    search.make("N");
    search.make("O");
    search.make("P");

    search.make("AA");
    search.make("BB");
    search.make("CC");
    search.make("DD");
    search.make("EE");
    search.make("FF");
    search.make("GG");
    search.make("HH");
    search.make("II");
    search.make("JJ");
    search.make("KK");
    search.make("LL");
    
    search.make("W1");
    search.make("W2");
    search.make("W3");
    search.make("W4");
    

    search.make("Z0");
    search.make("Z1");
    search.make("Z3");
    search.make("Z4");
    
    search.make("Z5");
    search.make("Z6");
    search.make("Z7");
    search.make("Z8");

    search.make("Z9");
    search.make("Z10");
    search.make("Z11");
    search.make("Z12");
    
    search.create("A", "B", 1);
    search.create("A", "W1", 1);
    search.create("A", "AA", 1);

    search.create("W1", "W2", 2);
    search.create("W2", "W3", 1);
    search.create("W3", "W4", 2);

    search.create("B", "W2", 1);
    search.create("B", "BB", 1);
    search.create("B", "C", 3);
    
    search.create("C", "W3", 1);
    search.create("C", "CC", 1);
    search.create("C", "D", 2);

    search.create("D", "W4", 1);
    search.create("D", "E", 1);
    search.create("D", "DD", 1);

    search.create("E", "F", 1);
    search.create("E", "EE", 1);

    search.create("F", "G", 1);
    search.create("F", "FF", 1);
    
    search.create("G", "H", 5);
    search.create("G", "GG", 1);
    
    search.create("H", "I", 1);
    search.create("H", "HH", 1);
    
    search.create("I", "II", 1);
    search.create("I", "J", 1);
    
    search.create("J", "K", 6);
    search.create("J", "JJ", 1);
    
    search.create("K", "L", 1);
    search.create("K", "KK", 1);
    
    search.create("L", "M", 1);
    search.create("L", "LL", 1);
    
    search.create("M", "N", 3);
    search.create("N", "O", 1);
    search.create("O", "P", 1);
    
    search.create("M", "Z0", 3);
    search.create("N", "Z1", 5);
    search.create("O", "Z2", 2);
    search.create("P", "Z3", 1);
    
    search.create("Z0", "Z5", 1);
    search.create("Z1", "Z6", 1);
    search.create("Z2", "Z7", 1);
    search.create("Z3", "Z8", 1);

    search.create("Z1", "Z9", 2);
    search.create("Z9", "Z10", 1);
    search.create("Z10", "Z11", 2);
    search.create("Z11", "Z12", 1);

    search.create("Z8", "Z12", 10);
    
    search.create("AA", "BB", 1);
    search.create("BB", "CC", 1);
    search.create("CC", "DD", 1);
    search.create("DD", "EE", 1);
    search.create("EE", "FF", 1);
    search.create("FF", "GG", 1);
    search.create("GG", "HH", 1);
    search.create("HH", "II", 1);
    search.create("JJ", "KK", 1);
    search.create("KK", "LL", 1);
    search.search("A", "Z12");
     
}
